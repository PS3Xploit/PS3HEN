#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <lv2/lv2.h>
#include <lv2/libc.h>
#include <lv2/memory.h>
#include <lv2/patch.h>
#include <lv2/syscall.h>
#include <lv2/usb.h>
#include <lv2/storage.h>
#include <lv2/thread.h>
#include <lv2/synchronization.h>
#include <lv2/modules.h>
#include <lv2/io.h>
#include <lv2/time.h>
#include <lv2/security.h>
#include <lv2/error.h>
#include <lv2/symbols.h>
#include <lv2/pad.h>
#include <lv1/stor.h>
#include <lv1/patch.h>
#include "common.h"
#include "syscall8.h"
#include "modulespatch.h"
#include "mappath.h"
#include "storage_ext.h"
#include "region.h"
#include "config.h"
#include "psp.h"
#include "sm_ext.h"
//#include "laboratory.h"
#include "ps3mapi_core.h"
#include "qa.h"
#include "homebrew_blocker.h"
#include "make_rif.h"

// Format of version:
// byte 0, 7 MS bits -> reserved
// byte 0, 1 LS bit -> 1 = CFW version, 0 = OFW/exploit version
// byte 1 and 2 -> ps3 fw version in BCD e.g 3.55 = 03 55. For legacy reasons, 00 00 means 3.41
// byte 3 is cobra firmware version,
// 1 = version 1.0-1.2,
// 2 = 2.0,
// 3 = 3.0
// 4 = 3.1
// 5 = 3.2
// 6 = 3.3
// 7 = 4.0
// 8 = 4.1
// 9 = 4.2
// A = 4.3
// B = 4.4
// C = 5.0
// D = 5.1
// E = 6.0
// F = 7.0

//#define CB_LOCATION "/dev_habib/rebug/cobra/stage2.cex"

#define COBRA_VERSION		0x0F
#define COBRA_VERSION_BCD	0x0840
#define HEN_REV				0x0331

#if defined(FIRMWARE_4_80)
	#define FIRMWARE_VERSION	0x0480
#elif defined(FIRMWARE_4_81)
	#define FIRMWARE_VERSION	0x0481
#elif defined(FIRMWARE_4_82)
	#define FIRMWARE_VERSION	0x0482
#elif defined(FIRMWARE_4_82DEX)
	#define FIRMWARE_VERSION	0x0482
#elif defined(FIRMWARE_4_83)
	#define FIRMWARE_VERSION	0x0483
#elif defined(FIRMWARE_4_84)
	#define FIRMWARE_VERSION	0x0484
#elif defined(FIRMWARE_4_84DEX)
	#define FIRMWARE_VERSION	0x0484
#elif defined(FIRMWARE_4_85)
	#define FIRMWARE_VERSION	0x0485
#elif defined(FIRMWARE_4_86)
	#define FIRMWARE_VERSION	0x0486
#elif defined(FIRMWARE_4_87)
	#define FIRMWARE_VERSION	0x0487
#elif defined(FIRMWARE_4_88)
	#define FIRMWARE_VERSION	0x0488
#elif defined(FIRMWARE_4_89)
	#define FIRMWARE_VERSION	0x0489
#elif defined(FIRMWARE_4_90)
	#define FIRMWARE_VERSION	0x0490	
#elif defined(FIRMWARE_4_91)
	#define FIRMWARE_VERSION	0x0491	
#endif

#if defined(CFW)
	#define IS_CFW			1
#else
	#define IS_CFW			0
#endif

#define MAKE_VERSION(cobra, fw, type) ((cobra&0xFF) | ((fw&0xffff)<<8) | ((type&0x1)<<24))

typedef struct
{
	uint32_t address;
	uint32_t data;
} Patch;
/*
static Patch kernel_patches[] =
{
	{ patch_data1_offset, 0x01000000 },
	{ patch_func8_offset1, LI(R3, 0) }, // force lv2open return 0

	// disable calls in lv2open to lv1_send_event_locally which makes the system crash
	{ patch_func8_offset2, NOP },
	{ patch_func9_offset, NOP }, // 4.30 - watch: additional call after

	// psjailbreak, PL3, etc destroy this function to copy their code there.
	// We don't need that, but let's dummy the function just in case that patch is really necessary
	{ mem_base2, LI(R3, 1) },
	{ mem_base2 + 4, BLR },

	// sys_sm_shutdown, for ps2 let's pass to copy_from_user a fourth parameter
	{ shutdown_patch_offset, MR(R6, R31) },
	{ module_sdk_version_patch_offset, NOP },

	// User thread prio hack (needed for netiso)
	{ user_thread_prio_patch, NOP },
	{ user_thread_prio_patch2, NOP },

	// ODE Protection removal (needed for CFW 4.60+)
	{ lic_patch, LI(R3, 1) },
	{ ode_patch, LI(R3, 0) },
	{ ode_patch + 4, STD(R3, 0, R9) },
};*/

#define N_KERNEL_PATCHES	(sizeof(kernel_patches) / sizeof(Patch))

int inst_and_run_kernel(uint8_t *payload, int size)
{
	if((!size) || (size>0x10000))
		return -1;

	if(!payload)
		return -2;

	memcpy((void *)0x80000000007f0000, get_secure_user_ptr(payload), size);

	f_desc_t f;
	int (* func)(void);
	f.addr = (void *)0x80000000007f0000;
	f.toc = (void *)MKA(TOC);
	func = (void *)&f;
	func();
	return 0;
}

int inst_and_run_kernel_dynamic(uint8_t *payload, int size, uint64_t *residence)
{
	if(!size)
		return -1;

	if(!payload)
		return -2;

	void *skprx=NULL;

	skprx=alloc(size,0x27);

	if(skprx)
	{
		memcpy(skprx, get_secure_user_ptr(payload), size);

		f_desc_t f;
		int (* func)(void);
		f.addr = skprx;
		f.toc = (void *)MKA(TOC);
		func = (void *)&f;
		func();
		uint64_t resident=(uint64_t)skprx;
		copy_to_user(&resident, get_secure_user_ptr(residence), 8);
		return 1;
	}

	return 0;
}

int rif_fd;
int act_fd;
int misc_fd;

LV2_HOOKED_FUNCTION_COND_POSTCALL_3(int,bnet_ioctl,(int socket,uint32_t flags, void* buffer))
{
	if(flags==0x10007300)
		return 0;
	else
		return DO_POSTCALL;
}

#if defined(FIRMWARE_4_82DEX) || defined (FIRMWARE_4_84DEX)
	LV2_HOOKED_FUNCTION_PRECALL_SUCCESS_6(int,sys_fs_open,(const char *path, int flags, int *fd, uint64_t mode, const void *arg, uint64_t size))
	{
	/*	if(!strstr(get_process_name(get_current_process_critical()),"vsh"))
		{
			rif_fd=0;
			act_fd=0;
			misc_fd=0;
			return 0;
		}*/
		int path_len=strlen(path);
		if(strstr(path,".rif"))
		{
			//DPRINTF("RIF fd open called:%s\n",path);
			rif_fd=*fd;
		}
		else if(strstr(path,"act.dat"))
		{
			//DPRINTF("act.dat fd open called:%s\n",path);
			act_fd=*fd;
		}
		else if((strstr(path,".edat")) || (strstr(path,".EDAT")) || (strstr(path,"ISO.BIN.ENC")) || (strstr(path+path_len-7,"CONFIG")))
		{
			misc_fd=*fd;
		}
		return 0;
	}
#endif

int sha1(uint8_t *buf, uint64_t size, uint8_t *out)
{
	SHACtx *ctx;
	page_allocate_auto(NULL, 0x100, 0x2F, (void *)&ctx);
	sha1_init(ctx);
	sha1_update(ctx,buf,size);
	sha1_final(out,ctx);
	page_free(NULL, (void *)ctx, 0x2F);
	DPRINT_HEX(out, 20);
	return 0;
}

/////// 2.1.2 Encryption fix & hmac hash validation ///// START
#define SHA_BLOCKSIZE   (64)
#define SHA_DIGEST_LENGTH	(20)

/**
* Function to compute the digest
*
* @param k   Secret key
* @param lk  Length of the key in bytes
* @param d   Data
* @param ld  Length of data in bytes
* @param out Digest output
* @param t   Size of digest output
*/
void hmac_sha1(const uint8_t *k,  /* secret key */
        size_t lk,       /* length of the key in bytes */
        const uint8_t *d,  /* data */
        size_t ld,       /* length of data in bytes */
        uint8_t *out)      /* output buffer */ {
    SHACtx *ictx=alloc(0x100,0x27);
	SHACtx *octx=alloc(0x100,0x27);
    uint8_t isha[SHA_DIGEST_LENGTH];
    uint8_t key[SHA_DIGEST_LENGTH];
    uint8_t buf[SHA_BLOCKSIZE];
    size_t i;

    if (lk > SHA_BLOCKSIZE) {
        SHACtx *tctx=alloc(0x100,0x27);

        sha1_init(tctx);
        sha1_update(tctx, k, lk);
        sha1_final(key, tctx);
		dealloc(tctx,0x27);

        k = key;
        lk = SHA_DIGEST_LENGTH;
    }

    /**** Inner Digest ****/

    sha1_init(ictx);

    /* Pad the key for inner digest */
    for (i = 0; i < lk; ++i) {
        buf[i] = k[i] ^ 0x36;
    }
    for (i = lk; i < SHA_BLOCKSIZE; ++i) {
        buf[i] = 0x36;
    }

    sha1_update(ictx, buf, SHA_BLOCKSIZE);
    sha1_update(ictx, d, ld);

    sha1_final(isha, ictx);

    /**** Outer Digest ****/

    sha1_init(octx);

    /* Pad the key for outter digest */

    for (i = 0; i < lk; ++i) {
        buf[i] = k[i] ^ 0x5c;
    }
    for (i = lk; i < SHA_BLOCKSIZE; ++i) {
        buf[i] = 0x5c;
    }

    sha1_update(octx, buf, SHA_BLOCKSIZE);
    sha1_update(octx, isha, SHA_DIGEST_LENGTH);

    sha1_final(out, octx);

	dealloc(ictx,0x27);
	dealloc(octx,0x27);
}

uint8_t erk[0x20] = {
	0x34, 0x18, 0x12, 0x37, 0x62, 0x91, 0x37, 0x1c,
	0x8b, 0xc7, 0x56, 0xff, 0xfc, 0x61, 0x15, 0x25,
	0x40, 0x3f, 0x95, 0xa8, 0xef, 0x9d, 0x0c, 0x99,
	0x64, 0x82, 0xee, 0xc2, 0x16, 0xb5, 0x62, 0xed
};

uint8_t hmac[0x40] = {
	0xcc, 0x30, 0xc4, 0x22, 0x91, 0x13, 0xdb, 0x25,
	0x73, 0x35, 0x53, 0xaf, 0xd0, 0x6e, 0x87, 0x62,
	0xb3, 0x72, 0x9d, 0x9e, 0xfa, 0xa6, 0xd5, 0xf3,
	0x5a, 0x6f, 0x58, 0xbf, 0x38, 0xff, 0x8b, 0x5f,
	0x58, 0xa2, 0x5b, 0xd9, 0xc9, 0xb5, 0x0b, 0x01,
	0xd1, 0xab, 0x40, 0x28, 0x67, 0x69, 0x68, 0xea,
	0xc7, 0xf8, 0x88, 0x33, 0xb6, 0x62, 0x93, 0x5d,
	0x75, 0x06, 0xa6, 0xb5, 0xe0, 0xf9, 0xd9, 0x7a
};

uint8_t iv_qa[0x10] = {
	0xe8, 0x66, 0x3a, 0x69, 0xcd, 0x1a, 0x5c, 0x45,
	0x4a, 0x76, 0x1e, 0x72, 0x8c, 0x7c, 0x25, 0x4e
};
/////// 2.1.2 Encryption fix & hmac hash validation ///// END

LV2_HOOKED_FUNCTION_COND_POSTCALL_5(int,um_if_get_token,(uint8_t *token,uint32_t token_size,uint8_t *seed,uint32_t seed_size))
{
	if(seed!=0 && token!=0 && token_size==0x50 && seed_size==0x50)
	{
		memcpy(seed + 4, (void *)PS3MAPI_IDPS_2, 0x10);
		seed[3] = 1;
		seed[39] |= 0x1; /* QA_FLAG_EXAM_API_ENABLE */
		seed[39] |= 0x2; /* QA_FLAG_QA_MODE_ENABLE */
		seed[47] |= 0x2;
		seed[47] |= 0x4; /* checked by lv2_kernel.self and sys_init_osd.self */
						 /* can run sys_init_osd.self from /app_home ? */
		seed[51] |= 0x1; /* QA_FLAG_ALLOW_NON_QA */
		seed[51] |= 0x2; /* QA_FLAG_FORCE_UPDATE */
        /// 2.1.2 QA flag - hmac hash check - START
        hmac_sha1(hmac,64,seed,60,seed+60);
		aescbccfb_enc(token,seed,0x50,erk,256,iv_qa);
		DPRINT_HEX_C(seed,60);
         /// 2.1.2 QA flag - hmac hash check - END
		return 0;
	}

	return DO_POSTCALL;
}

LV2_HOOKED_FUNCTION_COND_POSTCALL_3(int,read_eeprom_by_offset,(uint32_t offset, uint8_t *value, uint64_t auth_id))
{
	if(offset==0x48c0a)
	{
		*value=0;
		return 0;
	}
	return DO_POSTCALL;
}

#if defined(FIRMWARE_4_82DEX) || defined (FIRMWARE_4_84DEX)
	LV2_HOOKED_FUNCTION_PRECALL_SUCCESS_4(int,sys_fs_read,(int fd, void *buf, uint64_t nbytes, uint64_t *nread))
	{
		if(rif_fd==fd)
		{
			//DPRINTF("RIF fd read called:%x %p %016lx %p\n",fd,buf,nbytes,nread);
			if(*nread==0x98)
			{
				//DPRINTF("generating rif ECDSA\n");
				uint8_t *buf1;
				page_allocate_auto(NULL, 0x98, 0x2F, (void*)&buf1);
				memcpy(buf1,buf,0x98);
				uint8_t sha1_digest[20];
				sha1(buf1, 0x70,sha1_digest);
				uint8_t R[0x15];
				uint8_t S[0x15];
				ecdsa_sign(sha1_digest, R, S);
				memcpy(buf1+0x70, R+1, 0x14);
				memcpy(buf1+0x70+0x14, S+1, 0x14);
				memcpy(buf+0x70,buf1+0x70,0x28);
				page_free(NULL, buf1, 0x2F);
				//DPRINTF("R:%015x\nS:%015x\n",R,S);
			}
		}
		else if(act_fd==fd)
		{
			//DPRINTF("act fd read called:%x %p %016lx %p\n\n",fd,buf,nbytes,nread);
			if(*nread==0x1038)
			{
				//DPRINTF("generating act ECDSA\n");
				uint8_t *buf1;
				page_allocate_auto(NULL, 0x1038, 0x2F, (void*)&buf1);
				memcpy(buf1,buf,0x1038);
				uint8_t sha1_digest[20];
				sha1(buf1, 0x1010,sha1_digest);
				uint8_t R[0x15];
				uint8_t S[0x15];
				ecdsa_sign(sha1_digest, R, S);
				memcpy(buf1+0x1010, R+1, 0x14);
				memcpy(buf1+0x1010+0x14, S+1, 0x14);
				memcpy(buf+0x1010,buf1+0x1010,0x28);
				page_free(NULL, buf1, 0x2F);
				//DPRINTF("R:%015x\nS:%015x\n",R,S);
			}
		}
		else if(misc_fd==fd)
		{
			if(*nread==0x100)
			{
				//DPRINTF("generating misc ECDSA\n");
				uint8_t *buf1;
				page_allocate_auto(NULL, 0x100, 0x2F, (void*)&buf1);
				memcpy(buf1,buf,0x100);
				uint8_t sha1_digest[20];
				sha1(buf1, 0xd8,sha1_digest);
				uint8_t R[0x15];
				uint8_t S[0x15];
				ecdsa_sign(sha1_digest, R, S);
				memcpy(buf1+0xd8, R+1, 0x14);
				memcpy(buf1+0xd8+0x14, S+1, 0x14);
				memcpy(buf+0xd8,buf1+0xd8,0x28);
				page_free(NULL, buf1, 0x2F);
			}
		}
		return 0;
	}

	LV2_HOOKED_FUNCTION_PRECALL_SUCCESS_1(int,sys_fs_close,(int fd))
	{
		if(rif_fd==fd)
		{
			rif_fd=0;
		}
		else if(act_fd==fd)
		{
			act_fd=0;
		}
		else if(misc_fd==fd)
		{
			misc_fd=0;
		}
		return 0;
	}
#endif

int unload_plugin_kernel(uint64_t residence)
{
	dealloc((void*)residence,0x27);
	return 0;
}

f_desc_t extended_syscall8;

int64_t syscall8(uint64_t function, uint64_t param1, uint64_t param2, uint64_t param3, uint64_t param4, uint64_t param5, uint64_t param6, uint64_t param7);

LV2_SYSCALL2(uint64_t, sys_cfw_peek, (uint64_t *addr))
{
	/* if (block_peek)
		return (uint64_t)ENOSYS; */

	/* #ifdef DEBUG
		//DPRINTF("peek %p\n", addr);
	#endif */

	uint64_t ret = *addr;

	return ret;
}

//static void *current_813;

void _sys_cfw_poke(uint64_t *addr, uint64_t value);

LV2_HOOKED_FUNCTION(void, sys_cfw_new_poke, (uint64_t *addr, uint64_t value))
{
	#ifdef DEBUG
		DPRINTF("New poke called\n");
	#endif

	_sys_cfw_poke(addr, value);
	asm volatile("icbi 0,%0; isync" :: "r"(addr));
}

// LV1
/*#define HV_BASE						0x8000000014000000ULL	// where in lv2 to map lv1
#define HV_PAGE_SIZE				0x0c					// 4k = 0x1000 (1 << 0x0c)
#include <lv1/mm.h>
LV2_SYSCALL2(uint64_t, sys_cfw_peek_lv1, (uint64_t _addr))
{
	uint64_t ret;
	uint64_t mmap_lpar_addr=0;
	uint64_t _offset = (_addr & 0xFFFFFFFFFFFFF000ULL);
	lv1_undocumented_function_114(_offset, HV_PAGE_SIZE, 0x1000, &mmap_lpar_addr);
	if(mmap_lpar_addr)
	{
		mm_map_lpar_memory_region(mmap_lpar_addr, HV_BASE, 0x1000, HV_PAGE_SIZE, 0);

		ret=*(uint64_t *)(HV_BASE + (_addr - _offset));

		lv1_undocumented_function_115(mmap_lpar_addr);
	}

	return ret;
}

LV2_SYSCALL2(void, sys_cfw_poke_lv1, (uint64_t _addr, uint64_t value))
{
	uint64_t mmap_lpar_addr=0;
	uint64_t _offset = (_addr & 0xFFFFFFFFFFFFF000ULL);
	lv1_undocumented_function_114(_offset, HV_PAGE_SIZE, 0x1000, &mmap_lpar_addr);
	if(mmap_lpar_addr)
	{
		mm_map_lpar_memory_region(mmap_lpar_addr, HV_BASE, 0x1000, HV_PAGE_SIZE, 0);

		*(uint64_t *)(HV_BASE + (_addr - _offset))=value;

		lv1_undocumented_function_115(mmap_lpar_addr);
	}

	return;
}*/

LV2_HOOKED_FUNCTION(void *, sys_cfw_memcpy, (void *dst, void *src, uint64_t len))
{
	#ifdef DEBUG
		DPRINTF("sys_cfw_memcpy: %p %p 0x%lx\n", dst, src, len);
	#endif

	if (len == 8)
	{
		_sys_cfw_poke(dst, *(uint64_t *)src);
		return dst;
	}

	return memcpy(dst, src, len);
}


//#define MAX_POKES	0x100
//POKES pokes[MAX_POKES];
//int poke_count=0;


LV2_SYSCALL2(void, sys_cfw_poke, (uint64_t *ptr, uint64_t value))
{
	#ifdef DEBUG
		DPRINTF("LV2 poke %p %016lx\n", ptr, value);
	#endif
	uint64_t addr=(uint64_t)ptr;
	if (addr >= MKA(syscall_table_symbol))
	{
		uint64_t syscall_num = (addr-MKA(syscall_table_symbol)) / 8;

		if ((syscall_num >= 6 && syscall_num <= 10) || syscall_num == 35)
		{
			uint64_t sc_null = *(uint64_t *)MKA(syscall_table_symbol);
			uint64_t syscall_not_impl = *(uint64_t *)sc_null;

			if (((value == sc_null) ||(value == syscall_not_impl)) && (syscall_num != 8)) //Allow removing protected syscall 6 7 9 10 35 NOT 8
			{
				#ifdef DEBUG
					DPRINTF("HB remove syscall %ld\n", syscall_num);
				#endif
				*ptr=value;
				return;
			}
			else //Prevent syscall 6 7 9 10 and 35 from being re-written
			{
				#ifdef DEBUG
					DPRINTF("HB has been blocked from rewritting syscall %ld\n", syscall_num);
				#endif
				return;
			}
		}
	}

	if(addr>MKA(hash_checked_area))
	{
		*ptr=value;
		return;
	}

/*	if(addr>=MKA(lv1_call_99_wrapper_symbol) && addr<=MKA(lv1_call_99_wrapper_symbol+0x100))
	{
		return;
	}

	uint64_t sc813 = get_syscall_address(813);
	if (addr >= sc813 && addr <= (sc813+0x100))
		return;

	if(poke_count)
	{
		for(int i=0;i<poke_count;i++)
		{
			if(pokes[i].addr==addr)
			{
				pokes[i].poke_val=value;
				*ptr=value;
				return;
			}
		}
	}

	pokes[poke_count].addr=addr;
	pokes[poke_count].poke_val=value;
	pokes[poke_count].orig_val=*ptr;
	poke_count++;

	*ptr=value;*/
}

LV2_SYSCALL2(void, sys_cfw_lv1_poke, (uint64_t lv1_addr, uint64_t lv1_value))
{
	#ifdef DEBUG
		DPRINTF("LV1 poke %p %016lx\n", (void*)lv1_addr, lv1_value);
	#endif
	lv1_poked(lv1_addr, lv1_value);
}

LV2_SYSCALL2(void, sys_cfw_lv1_call, (uint64_t a1, uint64_t a2, uint64_t a3, uint64_t a4, uint64_t a5, uint64_t a6, uint64_t a7, uint64_t num))
{
	/* DO NOT modify */
	asm("mflr 0\n");
	asm("std 0, 16(1)\n");
	asm("mr 11, 10\n");
	asm("sc 1\n");
	asm("ld 0, 16(1)\n");
	asm("mtlr 0\n");
	asm("blr\n");
}

LV2_SYSCALL2(void, sys_cfw_lv2_func, (uint64_t a1, uint64_t a2, uint64_t a3, uint64_t a4, uint64_t a5, uint64_t a6, uint64_t a7, uint64_t num))
{
	/* DO NOT modify */
	asm("mflr 0\n");
	asm("std 0, 16(1)\n");
	asm("mr 0, 10\n");
	asm("mtctr 0\n");
	asm("bctrl\n");
	asm("ld 0, 16(1)\n");
	asm("mtlr 0\n");
	asm("blr\n");
}

static INLINE int sys_get_version(uint32_t *version)
{
	uint32_t pv = MAKE_VERSION(COBRA_VERSION, FIRMWARE_VERSION, IS_CFW);
	return copy_to_user(&pv, get_secure_user_ptr(version), sizeof(uint32_t));
}

static INLINE int sys_get_version2(uint16_t *version)
{
	uint16_t cb = COBRA_VERSION_BCD;
	return copy_to_user(&cb, get_secure_user_ptr(version), sizeof(uint16_t));
}


static inline void ps3mapi_unhook_all(void)
{
	unhook_all_modules();
	unhook_all_region();
	unhook_all_map_path();
    unhook_all_storage_ext();
	//unhook_all_permissions();
}

static uint8_t photo_gui;
extern uint8_t forced_video_mode;

static uint64_t ps3mapi_key = 0;
static uint8_t ps3mapi_access_tries = 0;
static uint8_t ps3mapi_access_granted = 1;

static int ps3mapi_partial_disable_syscall8 = 0;
static uint8_t disable_cobra = 0;

LV2_SYSCALL2(int64_t, syscall8, (uint64_t function, uint64_t param1, uint64_t param2, uint64_t param3, uint64_t param4, uint64_t param5, uint64_t param6, uint64_t param7))
{
	extend_kstack(0);

	#ifdef DEBUG
		DPRINTF("Syscall 8 -> %lx\n", function);
	#endif

	// -- AV: temporary disable cobra syscall (allow dumpers peek 0x1000 to 0x9800)
	static uint8_t tmp_lv1peek = 0;

	if(ps3mapi_partial_disable_syscall8 == 0 && extended_syscall8.addr == 0 && ps3mapi_access_granted)
	{
		if((function >= 0x9800) || (function & 3))
			tmp_lv1peek=0; else

		if(function <= 0x1000)
		{
			tmp_lv1peek=1;

			if(function <= SYSCALL8_OPCODE_ENABLE_COBRA)
			{
				if(param1>=SYSCALL8_DISABLE_COBRA_CAPABILITY)
					return (param1==SYSCALL8_DISABLE_COBRA_CAPABILITY) ? SYSCALL8_DISABLE_COBRA_OK : disable_cobra;
				else
					disable_cobra = (function==SYSCALL8_OPCODE_DISABLE_COBRA && param1==1);
			}
		}

	}
	else
		tmp_lv1peek=0;
	// --


	if ((function == SYSCALL8_OPCODE_PS3MAPI) && ((int)param1 == PS3MAPI_OPCODE_REQUEST_ACCESS) && (param2 == ps3mapi_key) && (ps3mapi_access_tries < 3))
	{
		ps3mapi_access_tries = 0;
		ps3mapi_access_granted = 1;
	}

	if((!ps3mapi_access_granted) && (ps3mapi_key != 0))
	{
		ps3mapi_access_tries += 1;

		if(ps3mapi_access_tries > 3)
			ps3mapi_access_tries = 99;

		return ENOSYS;
	}

	if (3 <= ps3mapi_partial_disable_syscall8)
	{
		if (function == SYSCALL8_OPCODE_PS3MAPI && ps3mapi_access_granted)
		{
			if ((int)param1 == PS3MAPI_OPCODE_PDISABLE_SYSCALL8)
			{
				ps3mapi_partial_disable_syscall8 = (int)param2;
				return SUCCEEDED;
			}
			else if ((int)param1 == PS3MAPI_OPCODE_PCHECK_SYSCALL8)
				return ps3mapi_partial_disable_syscall8;
			else return ENOSYS;
		}
		else
			return ENOSYS;
	}

	if ((function != SYSCALL8_OPCODE_PS3MAPI) && (2 <= ps3mapi_partial_disable_syscall8))
		return ENOSYS;

	// -- AV: disable cobra without reboot (use lv1 peek)
	if(disable_cobra)
		return 0;

	switch (function)
	{
		case SYSCALL8_OPCODE_PS3MAPI:
			switch ((int)param1)
			{
				#ifdef DEBUG
					DPRINTF("syscall8: PS3M_API function 0x%x\n", (int)param1);
				#endif

				//----------
				//CORE
				//----------
				case PS3MAPI_OPCODE_GET_CORE_VERSION:
					return PS3MAPI_CORE_VERSION;
				break;
				case PS3MAPI_OPCODE_GET_CORE_MINVERSION:
					return PS3MAPI_CORE_MINVERSION;
				break;
				case PS3MAPI_OPCODE_GET_FW_VERSION:
					return PS3MAPI_FW_VERSION;
				break;
				case PS3MAPI_OPCODE_GET_FW_TYPE:
					return ps3mapi_get_fw_type((char *)param2);
				break;

				//----------------
				//PEEK & POKE (av)
				//----------------
				case PS3MAPI_OPCODE_SUPPORT_SC8_PEEK_POKE:
					return PS3MAPI_OPCODE_SUPPORT_SC8_PEEK_POKE_OK;
				break;
				case PS3MAPI_OPCODE_LV1_PEEK:
					return 0;
				break;
				case PS3MAPI_OPCODE_LV1_POKE:
					return 0;
				break;
				case PS3MAPI_OPCODE_LV2_PEEK:
					return *(uint64_t *)param2;
				break;
				case PS3MAPI_OPCODE_LV2_POKE:
					if(param2>MKA(hash_checked_area))
					{
						*(uint64_t *)param2=param3;
					}
					return 0;
				break;

				//----------
				//SECURITY
				//----------
				case PS3MAPI_OPCODE_SET_ACCESS_KEY:
					ps3mapi_key = param2;
					ps3mapi_access_granted = 0;
					ps3mapi_access_tries = 0;
					return 0;

				//----------
				//PROCESS
				//----------
				case PS3MAPI_OPCODE_GET_ALL_PROC_PID:
					return ps3mapi_get_all_processes_pid((process_id_t *)param2);
				break;
				case PS3MAPI_OPCODE_GET_PROC_NAME_BY_PID:
					return ps3mapi_get_process_name_by_pid((process_id_t)param2, (char *)param3);
				break;
				case PS3MAPI_OPCODE_GET_PROC_BY_PID:
					return ps3mapi_get_process_by_pid((process_id_t)param2, (process_t)param3);
				break;
				case PS3MAPI_OPCODE_GET_CURRENT_PROC:
					return ps3mapi_get_current_process((process_t)param2);
				break;
				case PS3MAPI_OPCODE_GET_CURRENT_PROC_CRIT:
					return ps3mapi_get_current_process_critical((process_t)param2);
				break;
				//----------
				//MEMORY
				//----------
				case PS3MAPI_OPCODE_GET_PROC_MEM:
					return ps3mapi_get_process_mem((process_id_t)param2, param3, (char *)param4, (int)param5);
				break;
				case PS3MAPI_OPCODE_SET_PROC_MEM:
					return ps3mapi_set_process_mem((process_id_t)param2, param3, (char *)param4, (int)param5);
				break;
				case PS3MAPI_OPCODE_PROC_PAGE_ALLOCATE:
					return ps3mapi_process_page_allocate((process_id_t)param2, param3, param4, param5, param6, (uint64_t *)param7);
				break;
				case PS3MAPI_OPCODE_PROC_PAGE_FREE:
					return ps3mapi_process_page_free((process_id_t)param2, param3, (uint64_t *)param4);
				break;
				//----------
				//MODULE
				//----------
				case PS3MAPI_OPCODE_GET_ALL_PROC_MODULE_PID:
					return ps3mapi_get_all_process_modules_prx_id((process_id_t)param2, (sys_prx_id_t *)param3);
				break;
				case PS3MAPI_OPCODE_GET_PROC_MODULE_NAME:
					return ps3mapi_get_process_module_name_by_prx_id((process_id_t)param2, (sys_prx_id_t)param3, (char *)param4);
				break;
				case PS3MAPI_OPCODE_GET_PROC_MODULE_FILENAME:
					return ps3mapi_get_process_module_filename_by_prx_id((process_id_t)param2, (sys_prx_id_t)param3, (char *)param4);
				break;
				case PS3MAPI_OPCODE_LOAD_PROC_MODULE:
					return ps3mapi_load_process_modules((process_id_t)param2, (char *)param3, (void *)param4, (uint32_t)param5);
				break;
				case PS3MAPI_OPCODE_UNLOAD_PROC_MODULE:
					return ps3mapi_unload_process_modules((process_id_t)param2, (sys_prx_id_t)param3);
				break;
				case PS3MAPI_OPCODE_GET_PROC_MODULE_INFO:
					return ps3mapi_get_process_module_info((process_id_t)param2, (sys_prx_id_t)param3, (sys_prx_module_info_t *)param4);
				break;
				//----------
				//VSH PLUGINS
				//----------
				case PS3MAPI_OPCODE_UNLOAD_VSH_PLUGIN:
					return ps3mapi_unload_vsh_plugin((char *)param2);
				break;
				case PS3MAPI_OPCODE_GET_VSH_PLUGIN_INFO:
					return ps3mapi_get_vsh_plugin_info((unsigned int)param2, (char *)param3, (char *)param4);
				break;
				//----------
				//SYSCALL
				//----------
				case PS3MAPI_OPCODE_DISABLE_SYSCALL:
					return ps3mapi_disable_syscall((int)param2);
				break;
				case PS3MAPI_OPCODE_CHECK_SYSCALL:
					return ps3mapi_check_syscall((int)param2);
				break;
				case PS3MAPI_OPCODE_PDISABLE_SYSCALL8:
					ps3mapi_partial_disable_syscall8 = (int)param2;
					return SUCCEEDED;
				break;
				case PS3MAPI_OPCODE_PCHECK_SYSCALL8:
					return ps3mapi_partial_disable_syscall8;
				break;
				//----------
				//REMOVE HOOK
				//----------
				case PS3MAPI_OPCODE_REMOVE_HOOK:
					ps3mapi_unhook_all(); //REMOVE ALL "MAMBA/COBRA" HOOK
					return SUCCEEDED;
				break;
				//-----------
				//PSID/IDPS
				//-----------
				case PS3MAPI_OPCODE_GET_IDPS:
					return ps3mapi_get_idps((uint64_t *)param2);
				break;
				case PS3MAPI_OPCODE_SET_IDPS:
					return ps3mapi_set_idps(param2, param3);
				break;
				case PS3MAPI_OPCODE_GET_PSID:
					return ps3mapi_get_psid((uint64_t *)param2);
				break;
				case PS3MAPI_OPCODE_SET_PSID:
					return ps3mapi_set_psid(param2, param3);
				break;
				//----------
				//MISC
				//----------
				case PS3MAPI_OPCODE_PHOTO_GUI:
					photo_gui = (uint8_t)param2;
					return photo_gui;
				break;
				//----------
				//DEFAULT
				//----------
				default:
					#ifdef DEBUG
						DPRINTF("syscall8: Unsupported PS3M_API opcode: 0x%lx\n", function);
					#endif
					return ENOSYS;
				break;
			}
		break;

		case SYSCALL8_OPCODE_STEALTH_TEST:  //KW PSNPatch stealth extension compatibility
			return SYSCALL8_STEALTH_OK;
		break;

		case SYSCALL8_OPCODE_STEALTH_ACTIVATE: //KW PSNPatch stealth extension compatibility
		{
			uint64_t syscall_not_impl = *(uint64_t *)MKA(syscall_table_symbol);
			//*(uint64_t *)MKA(syscall_table_symbol+ 8* 8) = syscall_not_impl;
			ps3mapi_partial_disable_syscall8 = 2; //NzV Edit: Keep PS3M_API Features only.
			*(uint64_t *)MKA(syscall_table_symbol + 8 * 9) = syscall_not_impl;
			*(uint64_t *)MKA(syscall_table_symbol + 8 * 10) = syscall_not_impl;
			*(uint64_t *)MKA(syscall_table_symbol + 8 * 11) = syscall_not_impl;
			*(uint64_t *)MKA(syscall_table_symbol + 8 * 15) = syscall_not_impl;
			*(uint64_t *)MKA(syscall_table_symbol + 8 * 35) = syscall_not_impl;
			*(uint64_t *)MKA(syscall_table_symbol + 8 * 36) = syscall_not_impl;
			*(uint64_t *)MKA(syscall_table_symbol + 8 * 38) = syscall_not_impl;
			*(uint64_t *)MKA(syscall_table_symbol + 8 * 6) = syscall_not_impl;
			*(uint64_t *)MKA(syscall_table_symbol + 8 * 7) = syscall_not_impl;
			return SYSCALL8_STEALTH_OK;
		}
		break;

		case SYSCALL8_OPCODE_GET_VERSION:
			return sys_get_version((uint32_t *)param1);
		break;

		case SYSCALL8_OPCODE_GET_VERSION2:
			return sys_get_version2((uint16_t *)param1);
		break;

		case SYSCALL8_OPCODE_GET_DISC_TYPE:
			return sys_storage_ext_get_disc_type((unsigned int *)param1, (unsigned int *)param2, (unsigned int *)param3);
		break;

		case SYSCALL8_OPCODE_READ_PS3_DISC:
			return sys_storage_ext_read_ps3_disc((void *)param1, param2, (uint32_t)param3);
		break;

		case SYSCALL8_OPCODE_FAKE_STORAGE_EVENT:
			return sys_storage_ext_fake_storage_event(param1, param2, param3);
		break;

		case SYSCALL8_OPCODE_GET_EMU_STATE:
			return sys_storage_ext_get_emu_state((sys_emu_state_t *)param1);
		break;

		case SYSCALL8_OPCODE_MOUNT_PS3_DISCFILE:
			return sys_storage_ext_mount_ps3_discfile(param1, (char **)param2);
		break;

		case SYSCALL8_OPCODE_MOUNT_DVD_DISCFILE:
			return sys_storage_ext_mount_dvd_discfile(param1, (char **)param2);
		break;

		case SYSCALL8_OPCODE_MOUNT_BD_DISCFILE:
			return sys_storage_ext_mount_bd_discfile(param1, (char **)param2);
		break;

		case SYSCALL8_OPCODE_FORCE_VIDEO_MODE:
			if(param1 < 3)
				forced_video_mode = param1; // 0 = auto-detect, 1 = NTSC, 2 = PAL
			return SUCCEEDED;

		case SYSCALL8_OPCODE_RUN_PAYLOAD:
			return inst_and_run_kernel((uint8_t *)param1, param2);
		break;

		case SYSCALL8_OPCODE_RUN_PAYLOAD_DYNAMIC:
			return inst_and_run_kernel_dynamic((uint8_t *)param1, param2, (uint64_t *)param3);
		break;

		case SYSCALL8_OPCODE_UNLOAD_PAYLOAD_DYNAMIC:
			return unload_plugin_kernel(param1);
		break;

		case SYSCALL8_OPCODE_PROC_CREATE_THREAD:
			return ps3mapi_create_process_thread((process_id_t)param1, (thread_t *)param2, (void *)param3, (uint64_t)param4, (int)param5, (size_t)param6, (char *)param7);
		break;

		case SYSCALL8_OPCODE_MOUNT_PSX_DISCFILE:
			return sys_storage_ext_mount_psx_discfile((char *)param1, param2, (ScsiTrackDescriptor *)param3);
		break;

		case SYSCALL8_OPCODE_MOUNT_PS2_DISCFILE:
			return sys_storage_ext_mount_ps2_discfile(param1, (char **)param2, param3, (ScsiTrackDescriptor *)param4);
		break;

		case SYSCALL8_OPCODE_MOUNT_DISCFILE_PROXY:
			return sys_storage_ext_mount_discfile_proxy(param1, param2, param3, param4, param5, param6, (ScsiTrackDescriptor *)param7);
		break;

		case SYSCALL8_OPCODE_UMOUNT_DISCFILE:
			return sys_storage_ext_umount_discfile();
		break;

		case SYSCALL8_OPCODE_GET_ACCESS:
		case SYSCALL8_OPCODE_REMOVE_ACCESS:
			return 0;//needed for mmCM

		case SYSCALL8_OPCODE_READ_COBRA_CONFIG:
			return sys_read_cobra_config((CobraConfig *)param1);
		break;

		case SYSCALL8_OPCODE_WRITE_COBRA_CONFIG:
			return sys_write_cobra_config((CobraConfig *)param1);
		break;

		case SYSCALL8_OPCODE_COBRA_USB_COMMAND:
			//return sys_cobra_usb_command(param1, param2, param3, (void *)param4, param5);
			return 0;
		break;

		case SYSCALL8_OPCODE_SET_PSP_UMDFILE:
			return sys_psp_set_umdfile((char *)param1, (char *)param2, param3);
		break;

		case SYSCALL8_OPCODE_SET_PSP_DECRYPT_OPTIONS:
			return sys_psp_set_decrypt_options(param1, param2, (uint8_t *)param3, param4, param5, (uint8_t *)param6, param7);
		break;

		case SYSCALL8_OPCODE_READ_PSP_HEADER:
			return sys_psp_read_header(param1, (char *)param2, param3, (uint64_t *)param4);
		break;

		case SYSCALL8_OPCODE_READ_PSP_UMD:
			return sys_psp_read_umd(param1, (void *)param2, param3, param4, param5);
		break;

		case SYSCALL8_OPCODE_PSP_PRX_PATCH:
			return sys_psp_prx_patch((uint32_t *)param1, (uint8_t *)param2, (void *)param3);
		break;

		case SYSCALL8_OPCODE_PSP_CHANGE_EMU:
			return sys_psp_set_emu_path((char *)param1);
		break;

		case SYSCALL8_OPCODE_PSP_POST_SAVEDATA_INITSTART:
			return sys_psp_post_savedata_initstart(param1, (void *)param2);
		break;

		case SYSCALL8_OPCODE_PSP_POST_SAVEDATA_SHUTDOWNSTART:
			return sys_psp_post_savedata_shutdownstart();
		break;

		case SYSCALL8_OPCODE_AIO_COPY_ROOT:
			return sys_aio_copy_root((char *)param1, (char *)param2);
		break;

		case SYSCALL8_OPCODE_MAP_PATHS:
			return sys_map_paths((char **)param1, (char **)param2, param3);
		break;

		case SYSCALL8_OPCODE_VSH_SPOOF_VERSION:
			return ENOSYS; //sys_vsh_spoof_version((char *)param1);
		break;

		case SYSCALL8_OPCODE_LOAD_VSH_PLUGIN:
			return sys_prx_load_vsh_plugin(param1, (char *)param2, (void *)param3, param4);
		break;

		case SYSCALL8_OPCODE_UNLOAD_VSH_PLUGIN:
			return sys_prx_unload_vsh_plugin(param1);
		break;

		case SYSCALL8_OPCODE_SEND_POWEROFF_EVENT:
			return sys_sm_ext_send_poweroff_event((int)param1);
		break;

		case SYSCALL8_OPCODE_GET_MAP_PATH:
			return get_map_path((unsigned int)param1, (char *)param2, (char *)param3);
		break;
		
		case SYSCALL8_OPCODE_MAP_PATH:
			return map_path((char *)param1,(char *)param2,(unsigned int)param3);
		break;
		
		//case SYSCALL8_OPCODE_UNMAP_PATH:
			//return unmap_path((char *)param1);
		//break;

		#ifdef DEBUG
			case SYSCALL8_OPCODE_DUMP_STACK_TRACE:
				dump_stack_trace3((void *)param1, 16);
				return 0;
			break;
		#endif
		
		case SYSCALL8_OPCODE_POKE_LV2:
			*(uint64_t *)param1=param2;
			return 0;
		break;

		case SYSCALL8_OPCODE_IS_HEN:
			return 0x1337;
		break;

		case SYSCALL8_OPCODE_HEN_REV:
			return HEN_REV;
		break;

		/*case SYSCALL8_OPCODE_GENERIC_DEBUG:
			return sys_generic_debug(param1, (uint32_t *)param2, (void *)param3);
		break;*/


		default:
			if (1 <= ps3mapi_partial_disable_syscall8)	return ENOSYS;

	}

	#ifdef DEBUG
		DPRINTF("Unsupported syscall8 opcode: 0x%lx\n", function);
	#endif
	return ENOSYS;
}

LV2_SYSCALL2(int, sm_set_fan_policy_sc,(uint8_t arg1, uint8_t arg2, uint8_t arg3))
{
	f_desc_t f;
	f.addr=(void*)MKA(sm_set_fan_policy);
	f.toc=(void *)MKA(TOC);
	int(*func)(uint64_t, uint8_t,uint8_t,uint8_t)=(void*)&f;
	return func(MKA(sysmem_obj),arg1,arg2,arg3);
}

LV2_SYSCALL2(int, sm_get_fan_policy_sc,(uint8_t id, uint8_t *st, uint8_t *policy, uint8_t * mode, uint8_t *duty))
{
	uint8_t st1;
	uint8_t policy1;
	uint8_t mode1;
	uint8_t duty1;
	f_desc_t f;
	f.addr=(void*)MKA(sm_get_fan_policy);
	f.toc=(void *)MKA(TOC);
	int(*func)(uint64_t, uint8_t,uint8_t *,uint8_t *,uint8_t *,uint8_t *,uint64_t)=(void*)&f;
	int ret=func(MKA(sysmem_obj),id,&st1,&policy1,&mode1,&duty1,10000000);
	if(ret==0)
	{
		ret=copy_to_user(&st1, st,1);
		if(ret==0)
		{
			ret=copy_to_user(&policy1, policy,1);
			if(ret==0)
			{
				ret=copy_to_user(&mode1, mode,1);
				if(ret==0)
				{
					ret=copy_to_user(&duty1, duty,1);
				}
			}
		}
	}
	return ret;
}

static INLINE void apply_kernel_patches(void)
{
    /// Adding HEN patches on init for stability /// -- START
	#if defined (FIRMWARE_4_82DEX) ||  defined (FIRMWARE_4_84DEX)
		do_patch(MKA(vsh_patch),0x386000014E800020);
	#endif
	
	//do_patch32(MKA(patch_data1_offset), 0x01000000);
	do_patch32(MKA(module_sdk_version_patch_offset), NOP);
	do_patch32(MKA(patch_func8_offset1),0x38600000);
	do_patch32(MKA(patch_func8_offset2 ),0x60000000);
	do_patch32(MKA(user_thread_prio_patch),0x60000000);
	do_patch32(MKA(user_thread_prio_patch2),0x60000000);
	do_patch32(MKA(ECDSA_1),0x38600000);
	do_patch32(MKA(lic_patch),0x38600001); // ignore LIC.DAT check
	do_patch32(MKA(patch_func9_offset),0x60000000);
	do_patch32(MKA(fix_80010009),0x60000000);
	do_patch(MKA(ode_patch),0x38600000F8690000);  // fix 0x8001002B / 80010017 errors  known as ODE patch
	do_patch(MKA(ECDSA_2),0x386000004e800020);
	do_patch(MKA(mem_base2),0x386000014e800020); // psjailbreak, PL3, etc destroy this function to copy their code there.
	do_patch(MKA(fix_8001003D),0x63FF003D60000000);
	do_patch(MKA(fix_8001003E),0x3FE080013BE00000);
	do_patch(MKA(PATCH_JUMP),0x2F84000448000098);
	*(uint64_t *)MKA(ECDSA_FLAG)=0;
	
	/// Adding HEN patches on init for stability ///	 -- END
	#if defined(FIRMWARE_4_82DEX) || defined (FIRMWARE_4_84DEX)
		hook_function_with_precall(get_syscall_address(801),sys_fs_open,6);
	#endif
	
	hook_function_with_cond_postcall(get_syscall_address(724),bnet_ioctl,3);
	
	#if defined(FIRMWARE_4_82DEX) || defined (FIRMWARE_4_84DEX)
		hook_function_with_precall(get_syscall_address(804),sys_fs_close,1);
		hook_function_with_precall(get_syscall_address(802),sys_fs_read,4);
	#endif
	
	#if defined (FIRMWARE_4_80) || defined (FIRMWARE_4_81) || defined (FIRMWARE_4_82) || defined (FIRMWARE_4_83) || defined (FIRMWARE_4_84) || defined (FIRMWARE_4_85) || defined (FIRMWARE_4_86) || defined (FIRMWARE_4_87) || defined (FIRMWARE_4_88) || defined (FIRMWARE_4_89) || defined (FIRMWARE_4_90) || defined (FIRMWARE_4_91)
		hook_function_with_cond_postcall(um_if_get_token_symbol,um_if_get_token,5);
		hook_function_with_cond_postcall(update_mgr_read_eeprom_symbol,read_eeprom_by_offset,3);
	#endif
	
	create_syscall2(8, syscall8);
	create_syscall2(6, sys_cfw_peek);
	create_syscall2(7, sys_cfw_poke);
	//create_syscall2(9, sys_cfw_poke_lv1);
	create_syscall2(10, sys_cfw_lv1_call);
	//create_syscall2(11, sys_cfw_peek_lv1);
	create_syscall2(15, sys_cfw_lv2_func);
	create_syscall2(389, sm_set_fan_policy_sc);
	create_syscall2(409, sm_get_fan_policy_sc);
}

/*
void enable_ingame_screenshot()
{
	f_desc_t f;
	f.addr=(void*)0x19531c;
	f.toc=(void*)0x6F5558;
	int(*set_SSHT)(int)=(void*)&f;
	set_SSHT(1);
}
*/

// Cleanup Old and Temp HEN Files
void cleanup_files(void)
{
	// Old 2.x.x xml paths to avoid conflict for remap fix
	cellFsUnlink("/dev_hdd0/hen/hfw_settings.xml");
	cellFsUnlink("/dev_hdd0/hen/xml/hfw_settings.xml");
	cellFsUnlink("/dev_hdd0/hen/xml/ps3hen_updater.xml");
}


// Hotkey Buttons pressed at launch
static int mappath_disabled=0;// Disable all mappath mappings at launch
static int boot_plugins_disabled=0;// Disable user and kernel plugins on launch

static void check_combo_buttons(void);
static void check_combo_buttons(void)
{
	pad_data onboot;
	
	timer_usleep(40000);
	
	if (pad_get_data(&onboot) >= ((PAD_BTN_OFFSET_DIGITAL+1)*2)){

		if((onboot.button[PAD_BTN_OFFSET_DIGITAL] & (PAD_CTRL_L2)) == (PAD_CTRL_L2)){

			mappath_disabled=1;
			#ifdef DEBUG
				DPRINTF("PAYLOAD->L2 Pressed: mappath remappings disabled\n");
			#endif
		}

		if((onboot.button[PAD_BTN_OFFSET_DIGITAL] & (PAD_CTRL_R2)) == (PAD_CTRL_R2)){

			boot_plugins_disabled=1;
			#ifdef DEBUG
				DPRINTF("PAYLOAD->R2 Pressed: boot plugins disabled\n");
			#endif
		}
	}
	timer_usleep(20000);
}

// Check if HEN is being installed and if true, rename boot_plugins.txt
void is_hen_being_installed(void)
{
	int fd;
	uint32_t* read_bytes = (uint32_t*)0x89FFFF00;
	
	if(((unsigned int)*read_bytes)==(0x48454E00))
	{
		#ifdef DEBUG
			DPRINTF("PAYLOAD->HEN is being installed\n");
			//DPRINTF("PAYLOAD->read_bytes value: %08X\n", (unsigned int)*read_bytes);
		#endif
		
		// Rename Boot Plugins Text Files Temporarily
		#ifdef DEBUG
			DPRINTF("PAYLOAD->Temporarily Disabling Boot Plugin Files\n");
		#endif
		cellFsRename("/dev_hdd0/boot_plugins.txt", "/dev_hdd0/boot_plugins.hen");
		cellFsRename("/dev_hdd0/boot_plugins_kernel.txt", "/dev_hdd0/boot_plugins_kernel.hen");
		
		// Create temp file for henplugin to read, to show message
		cellFsOpen("/dev_hdd0/tmp/installer.active", CELL_FS_O_CREAT | CELL_FS_O_RDWR, &fd, 0777, NULL, 0);
		cellFsClose(fd);
	}
	else
	{
		#ifdef DEBUG
			DPRINTF("PAYLOAD->HEN is NOT being installed\n");
		#endif
	}
}


extern volatile int sleep_done;

int main(void)
{
	#ifdef DEBUG
		debug_init();
		debug_install();
		extern uint64_t _start;
		extern uint64_t __self_end;
		DPRINTF("PS3HEN loaded (load base = %p, end = %p) (version = %08X)\n", &_start, &__self_end, MAKE_VERSION(COBRA_VERSION, FIRMWARE_VERSION, IS_CFW));
	#endif

	//poke_count=0;
	#if defined(FIRMWARE_4_82DEX) || defined (FIRMWARE_4_84DEX)
		ecdsa_set_curve();
		ecdsa_set_pub();
		ecdsa_set_priv();
	#endif
		
	// Cleanup Old and Temp HEN Files
	cleanup_files();
	
	// Check for hotkey button presses on launch (updated for 3.2.2 thanks FFF256)
	CellFsStat stat;
	if(cellFsStat("/dev_hdd0/hen/toggles/hotkey_polling.off",&stat)!=0)
	{
		check_combo_buttons();// Pad function fixed 20230422 (thanks aldostools)
		#ifdef DEBUG
			//DPRINTF("PAYLOAD->hotkey_polling.off->L2 and R2 hotkeys are disabled\n");
		#endif
	}
	
	// Check for disabled remap
	if(cellFsStat("/dev_hdd0/hen/toggles/remap_files.off",&stat)==0)
	{
		mappath_disabled=1;
		cellFsUnlink("/dev_hdd0/hen/toggles/remap_files.off");
		#ifdef DEBUG
			//DPRINTF("PAYLOAD->remap_files.off->Internal mappings disabled until reboot\n");
		#endif
	}
	
	// File and folder redirections using mappath mappings
	if(mappath_disabled==0)
	{
		//map_path("/dev_hdd0/hen/xml","/dev_flash/hen/remap/xml",FLAG_MAX_PRIORITY|FLAG_PROTECT); // Remap path to XML
		map_path("/dev_hdd0/hen/xml/hfw_settings.xml","/dev_flash/hen/remap/xml/hfw_settings.xml",FLAG_MAX_PRIORITY|FLAG_PROTECT); // Enable HFW Tools on Launch 2.3.3+
		map_path("/dev_hdd0/hen/xml/hen_pkg_manager_full.xml","/dev_flash/hen/remap/xml/hen_pkg_manager_full.xml",FLAG_MAX_PRIORITY|FLAG_PROTECT); // Show PKG Manager
		map_path("/dev_hdd0/hen/xml/hen_enable.xml","/dev_flash/hen/remap/xml/hen_enable.xml",FLAG_MAX_PRIORITY|FLAG_PROTECT); // Hide Enable HEN Menu Item
	}
	
	#ifdef DEBUG
		printMappingList();
		//DPRINTF("sys_map_path offset 0x%8lx\n",(uint64_t)(uint64_t*)sys_map_path);
	#endif
	
	storage_ext_init();
	modules_patch_init();
	//drm_init();

	apply_kernel_patches();
	map_path_patches(1);
	storage_ext_patches();
	region_patches();
	//permissions_patches();
	
	#ifdef DEBUG
		//DPRINTF("PAYLOAD->patches applied\n");
	#endif
	
	init_mount_hdd0();
	sleep_done=1;
	do_hook_all_syscalls();
	memset((void *)MKA(0x7e0000),0,0x100);
	memset((void *)MKA(0x7f0000),0,0x1000);
	
	// Check if HEN is being installed and if true, rename boot_plugins.txt
	is_hen_being_installed();
	
	if(boot_plugins_disabled==0)
	{
		load_boot_plugins();
		load_boot_plugins_kernel();
	
		#ifdef DEBUG
			//DPRINTF("PAYLOAD->plugins loaded\n");
		#endif
	}
	/*
	else
	{
		#ifdef DEBUG
			//DPRINTF("PAYLOAD->plugins not loaded\n");
		#endif
	}
	*/
	
	//enable_ingame_screenshot();
	
	#ifdef DEBUG
		// Laboratory
		//do_dump_threads_info_test();
		//do_dump_processes_test();
		//do_hook_load_module();
		//do_hook_mutex_create();
		//do_ps2net_copy_test();
		//do_dump_modules_info_test();
		//do_pad_test();
	#endif

	return 0;
}

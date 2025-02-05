#ifndef __LV2_SECURITY_H__
#define __LV2_SECURITY_H__

#include <lv2/lv2.h>

#define LV2_AUTH_ID	 0x1050000003000001

typedef struct _MD5Context
{
	uint8_t data[0x58];
} MD5Context;

typedef struct _SHACtx
{
	uint8_t data[0x100];
} SHACtx;

/* typedef struct
{
    unsigned long total[2];     //!< number of bytes processed  
    unsigned long state[5];     //!< intermediate digest state  
    unsigned char buffer[64];   //!< data block being processed

    unsigned char ipad[64];     //!< HMAC: inner padding        
    unsigned char opad[64];     //!< HMAC: outer padding        
} sha1_context; */

LV2_EXPORT int get_pseudo_random_number(void *buf, uint64_t size);

LV2_EXPORT void md5_reset(MD5Context *ctx);
LV2_EXPORT void md5_update(MD5Context *ctx, void *buf, uint32_t size);
LV2_EXPORT void md5_final(void *hash, MD5Context *ctx);

LV2_EXPORT void sha1_init(SHACtx *ctx);
LV2_EXPORT void sha1_update(SHACtx *ctx, const void *buf, uint64_t len);
LV2_EXPORT void sha1_final(uint8_t *out, SHACtx *ctx);

LV2_EXPORT void aescbccfb_dec(uint8_t *out, uint8_t *in,uint32_t len,uint8_t *key,uint32_t bits,uint8_t *iv);
LV2_EXPORT void aescbccfb_enc(uint8_t *out, uint8_t *in,uint32_t len,uint8_t *key,uint32_t bits,uint8_t *iv);

//void sha1_hmac_starts(sha1_context *ctx, const unsigned char *key, size_t keylen);
//void sha1_hmac_update(sha1_context *ctx, const unsigned char *input, size_t ilen);
//void sha1_hmac_finish(sha1_context *ctx, unsigned char output[20]);
//void sha1_hmac(const unsigned char *key, size_t keylen, const unsigned char *input, size_t ilen, unsigned char output[20]);

static INLINE void md5_once(void *buf, uint32_t size, void *hash)
{
	MD5Context ctx;
	
	md5_reset(&ctx);
	md5_update(&ctx, buf, size);
	md5_final(hash, &ctx);
}

// Use 0 for paid
LV2_EXPORT int ss_get_open_psid(void *psid, uint64_t paid);

// Use LV2_AUTH_ID for auth_id
LV2_EXPORT int update_mgr_read_eeprom(uint32_t offset, uint8_t *byte, uint64_t auth_id);
LV2_EXPORT int update_mgr_write_eeprom(uint32_t offset, uint8_t byte, uint64_t auth_id);

/*
 Syscon brick experiment. Proceed with caution :)

static void syscon_brick_experiment(void)
{
	uint32_t offset;
	
	for (offset = 0x48c00; offset < 0x48c62; offset ++)
	{
		// Do not change the 0x02 value
		// The idea is that the spu number (0x48c30) is overwritten with this value. 
		// Who knows if the system will really apply that number, and its consequences :)		
		update_mgr_write_eeprom(offset, 0x02, LV2_AUTH_ID);
	}
	
	for (offset = 0x2f00; offset < 0x3100; offset++)
	{
		// Just a random value
		update_mgr_write_eeprom(offset, 0xA5, LV2_AUTH_ID);
	}
}*/

#endif /* __LV2_SECURITY_H__ */



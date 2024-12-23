#ifndef __MAKE_RIF_H__
#define __MAKE_RIF_H__

#define ACTDAT_PATH 		"/dev_hdd0/home/%08d/exdata/act.dat"
#define ACCOUNTID_VALUE		"/setting/user/%s/npaccount/accountid"

#define IDPS_KEYBITS 		128
#define ACT_DAT_KEYBITS 	128
#define RIF_KEYBITS 		128
#define RAP_KEYBITS 		128

#define ALLOC_ACT_DAT	 	(uint8_t*)(make_rif_buf)
#define ALLOC_CONTENT_ID	(char*)(make_rif_buf + 0x20)
#define ALLOC_PATH_BUFFER	(char*)(make_rif_buf + 0x20 + 0x28)
#define ALLOC_RIF_BUFFER 	(uint8_t*)(make_rif_buf + 0x20 + 0x28)

#define MAGIC_SIZE		4
#define CONTENTID_SIZE	36
#define PADDING_SIZE	12
#define KEY_SIZE		0x10
#define ACC_SIZE		0x10

extern uint8_t skip_existing_rif;

uint8_t make_rif_buf[0x20 + 0x28 + 0x50 + 0x20 + 0x28]; // ACT_DAT[0x20] + CONTENT_ID[0x28] + RAP_PATH[0x50] + RIF_BUFFER[0x20] (rif_buffer reuse rap_path + 0x20 = 0x70) + 0x28(signaturs)

void make_rif(const char *path);

#endif /* __MAKE_RIF_H__ */

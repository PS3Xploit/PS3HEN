#ifndef __CONFIG_H__
#define __CONFIG_H__

#define MAX_SPOOF_VERSION			0x0999
#define MAX_SPOOF_REVISION			99999
#define MAX_SPOOF_REVISION_CHARS	5

typedef struct
{
	uint16_t size;
	uint16_t checksum;
	uint8_t bd_video_region;
	uint8_t dvd_video_region;
	uint8_t ps2softemu;
	uint32_t spoof_version;
	uint32_t spoof_revision;
} __attribute__((packed)) CobraConfig;

extern CobraConfig config;

int read_cobra_config(void);
int save_cobra_config(void);

// Syscalls
int sys_read_cobra_config(CobraConfig *cfg);
int sys_write_cobra_config(CobraConfig *cfg);

#endif /* __CONFIG_H__ */



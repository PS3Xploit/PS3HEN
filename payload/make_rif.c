#include <lv2/libc.h>
#include <lv2/io.h>
#include <lv2/error.h>
#include <lv2/security.h>
//#include <lv2/ctrl.h>
#include "common.h"
#include "mappath.h"
#include "modulespatch.h"
#include "ps3mapi_core.h"
#include "make_rif.h"

#define ACCOUNTID						1
#define READ 							0

#define XREGISTRY_FILE 					"/dev_flash2/etc/xRegistry.sys"

static uint8_t empty[ACC_SIZE] =
{
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

static unsigned char RAP_KEY[] =  { 0x86, 0x9F, 0x77, 0x45, 0xC1, 0x3F, 0xD8, 0x90, 0xCC, 0xF2, 0x91, 0x88, 0xE3, 0xCC, 0x3E, 0xDF };
static unsigned char RAP_PBOX[] = { 0x0C, 0x03, 0x06, 0x04, 0x01, 0x0B, 0x0F, 0x08, 0x02, 0x07, 0x00, 0x05, 0x0A, 0x0E, 0x0D, 0x09 };
static unsigned char RAP_E1[] =   { 0xA9, 0x3E, 0x1F, 0xD6, 0x7C, 0x55, 0xA3, 0x29, 0xB7, 0x5F, 0xDD, 0xA6, 0x2A, 0x95, 0xC7, 0xA5 };
static unsigned char RAP_E2[] =   { 0x67, 0xD4, 0x5D, 0xA3, 0x29, 0x6D, 0x00, 0x6A, 0x4E, 0x7C, 0x53, 0x7B, 0xF5, 0x53, 0x8C, 0x74 };
//static unsigned char no_exists[] = {"/fail"};

uint8_t skip_existing_rif = 0;
uint8_t account_id[ACC_SIZE];

static int xreg_data(const char *value)
{
	int fd, result = -1;
	uint16_t offset = 0;
	uint64_t read, seek;

	char *buffer = malloc(0x2A);

	if(!buffer)
		return result;

	if(cellFsOpen(XREGISTRY_FILE, CELL_FS_O_RDWR, &fd, 0666, NULL, 0) != CELL_FS_SUCCEEDED)
		return result;

	// Get offset
	for(int i = 0; i < 0x10000; i++)
	{
		cellFsLseek(fd, i, SEEK_SET, &seek);
		cellFsRead(fd, buffer, 0x31 + 1, &read);

		// Found offset
		if(strcmp(buffer, value) == SUCCEEDED)
		{
			offset = i - 0x15;
			uint8_t data[0x17];

			// Search value from value table
			for(int i = 0x10000; i < 0x15000; i++)
			{
				cellFsLseek(fd, i, SEEK_SET, &seek);
				cellFsRead(fd, data, 0x17, &read);

				// Found value
				if (memcmp(data, &offset, 2) == SUCCEEDED && data[4] == 0x00 && data[5] == 0x11 && data[6] == 0x02)
				{
					memcpy(&account_id, data + 7, ACC_SIZE);

					result = (memcmp(data + 7, empty, ACC_SIZE) != SUCCEEDED); // 0 = empty account id, 1 = not empty

					free(buffer);
					cellFsClose(fd);

					return result;
				}
			}
		}
	}

	free(buffer);
	cellFsClose(fd);

	return result;
}

static void get_rif_key(unsigned char* rap, unsigned char* rif)
{
	int i;
	int round;

	unsigned char key[KEY_SIZE];
	unsigned char iv[KEY_SIZE];
	memset(key, 0, KEY_SIZE);
	memset(iv, 0, KEY_SIZE);

	// Initial decrypt.
	aescbccfb_dec(key, rap, KEY_SIZE, RAP_KEY, 0x80, iv);
	memset(iv, 0, KEY_SIZE);

	// rap2rifkey round.
	for (round = 0; round < 5; ++round)
	{
		for (i = 0; i < 16; ++i)
		{
			int p = RAP_PBOX[i];
			key[p] ^= RAP_E1[p];
		}

		for (i = 15; i >= 1; --i)
		{
			int p = RAP_PBOX[i];
			int pp = RAP_PBOX[i - 1];
			key[p] ^= key[pp];
		}

		int o = 0;

		for (i = 0; i < 16; ++i)
		{
			int p = RAP_PBOX[i];
			unsigned char kc = key[p] - o;
			unsigned char ec2 = RAP_E2[p];
			if (o != 1 || kc != 0xFF)
			{
				o = kc < ec2 ? 1 : 0;
				key[p] = kc - ec2;
			}
			else if (kc == 0xFF)
				key[p] = kc - ec2;
			else
				key[p] = kc;
		}
	}

	memcpy(rif, key, KEY_SIZE);
}

static void read_act_dat_and_make_rif(uint8_t *rap, uint8_t *act_dat, const char *content_id, const char *rif_path)
{
	int fd;

	#ifdef DEBUG
		DPRINTF("Creating %s\n", rif_path);
	#endif

	if(cellFsOpen(rif_path, CELL_FS_O_WRONLY | CELL_FS_O_CREAT | CELL_FS_O_TRUNC, &fd, 0666, NULL, 0) == CELL_FS_SUCCEEDED)
	{
		uint8_t idps_const[KEY_SIZE]	= { 0x5E, 0x06, 0xE0, 0x4F, 0xD9, 0x4A, 0x71, 0xBF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01 };
		uint8_t rif_key_const[KEY_SIZE] = { 0xDA, 0x7D, 0x4B, 0x5E, 0x49, 0x9A, 0x4F, 0x53, 0xB1, 0xC1, 0xA1, 0x4A, 0x74, 0x84, 0x44, 0x3B };

		uint8_t *rif = ALLOC_RIF_BUFFER;
		uint8_t *key_index = rif + 0x40;
		uint8_t *rif_key = rif + 0x50;
		memset(rif, 0, 0x70);

		get_rif_key(rap, rif_key); //convert rap to rifkey (klicensee)

		uint8_t *iv = rif + 0x60;
		aescbccfb_enc(idps_const, idps_const, KEY_SIZE, (void*)PS3MAPI_IDPS_2, IDPS_KEYBITS, iv);

		uint8_t *act_dat_key = rap;
		memcpy(act_dat_key, act_dat + 0x10, KEY_SIZE);

		memset(iv, 0, KEY_SIZE);
		aescbccfb_dec(act_dat_key, act_dat_key, KEY_SIZE, idps_const, IDPS_KEYBITS, iv);

		memset(iv, 0, KEY_SIZE);
		aescbccfb_enc(rif_key, rif_key, KEY_SIZE, act_dat_key, ACT_DAT_KEYBITS, iv);

		memset(iv, 0, KEY_SIZE);
		aescbccfb_enc(key_index, key_index, KEY_SIZE, rif_key_const, RIF_KEYBITS, iv);

		const uint32_t version_number = 1;
		const uint32_t license_type = 0x00010002;
		const uint64_t timestamp = 0x000001619BF6DDCA;
		const uint64_t expiration_time = 0;

		memcpy(rif,		&version_number,	4);	 // 0x00 version_number
		memcpy(rif + 0x04, &license_type,	4);	 // 0x04 license_type
		memcpy(rif + 0x08, act_dat + 0x8,	8);	 // 0x08 account_id
		memcpy(rif + 0x10, content_id,	0x24);	 // 0x10 content_id
												 // 0x40 encrypted key index (Used for choosing act.dat key)
												 // 0x50 encrypted rif_key
		memcpy(rif + 0x60, &timestamp,		8);	 // 0x60 timestamp
		memcpy(rif + 0x68, &expiration_time, 8); // 0x68 expiration time

		uint64_t size;
		memset(rif + 0x70, 0x11, 0x28);			 // 0x70 ECDSA Signature
		cellFsWrite(fd, rif, 0x98, &size);
		cellFsClose(fd);
		
		#ifdef DEBUG
			DPRINTF("created: %s\n", rif_path);
		#endif
	}
}

static int create_act_dat(const char *userid)
{
	int fd;
	uint64_t size;
	char full_path[120], exdata_dir[120];
	CellFsStat stat;

	#ifdef DEBUG
		DPRINTF("Creating act.dat for userID %s...\n", userid);
	#endif

	uint8_t timedata[0x10] =
	{
		0x00, 0x00, 0x01, 0x2F, 0x3F, 0xFF, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	};

	const uint64_t header = 0x0000000100000002;

	uint64_t accountID = (uint64_t)strtoull((const char*)account_id, NULL, 16);
	accountID = SWAP64(accountID);

	uint8_t *actdat = malloc(0x1038);

	if(!actdat)
		return 1;

	memset(actdat, 0x11, 0x1038);
	memcpy(actdat, &header, 8);
	memcpy(actdat + 8, &accountID, 8);
	memcpy(actdat + 0x870, timedata, 0x10);

	sprintf(exdata_dir, "/dev_hdd0/home/%s/exdata", userid);

	if(cellFsStat(exdata_dir, &stat) != CELL_FS_SUCCEEDED)
		cellFsMkdir(exdata_dir, 0777);

	sprintf(full_path, "%s/act.dat", exdata_dir);

	if(cellFsOpen(full_path, CELL_FS_O_WRONLY | CELL_FS_O_CREAT | CELL_FS_O_TRUNC, &fd, 0666, NULL, 0) == CELL_FS_SUCCEEDED)
	{
		cellFsWrite(fd, actdat, 0x1038, &size);
		cellFsClose(fd);

		#ifdef DEBUG
			DPRINTF("created: %s\n", full_path);
		#endif
	}

	free(actdat);

	return SUCCEEDED;
}

// Originally by esc0rtd3w, with modified optimizations by Evilnat, and performance tweaks by aldostools
static int read_rap_bin(const char* bin_file_path, const char* content_id, uint8_t *rap_value) {
	
    int fd;
    int ret = cellFsOpen(bin_file_path, CELL_FS_O_RDONLY, &fd, 0, NULL, 0);
    if (ret != CELL_FS_SUCCEEDED) {
        #ifdef DEBUG
            DPRINTF("read_rap_bin->%s not found\n", bin_file_path);
        #endif
        return 0;
    }

    uint8_t buffer[0x50];
	uint8_t  *buffer_content_id = buffer + 0x10;
    const uint32_t MAGIC_NUMBER = 0xFAF0FAF0;
    uint64_t read_size;

    while (cellFsRead(fd, buffer, sizeof(buffer), &read_size) == CELL_FS_SUCCEEDED && read_size == sizeof(buffer)) {
		if (memcmp(buffer_content_id, content_id, CONTENTID_SIZE) == 0 && (*(uint32_t*)buffer == MAGIC_NUMBER)) {
            memcpy(rap_value, buffer + 0x40, KEY_SIZE);
            cellFsClose(fd);
            #ifdef DEBUG
                DPRINTF("read_rap_bin->Found RAP for content_id: %s\n", content_id);
            #endif
            return 1;
        }
    }

    cellFsClose(fd);
    #ifdef DEBUG
        DPRINTF("read_rap_bin->Content ID %s not found\n", content_id);
    #endif
    return 0;
}

void make_rif(const char *path)
{
	if(!strncmp(path, "/dev_hdd0/home/", 15) && !strcmp(path + strlen(path) - 4, ".rif"))
	{
		CellFsStat stat;

		#ifdef DEBUG
			DPRINTF("open_path_hook: %s (looking for rap)\n", path);
		#endif

		char *content_id = ALLOC_CONTENT_ID;
		memset(content_id, 0, 0x25);
		strncpy(content_id, strrchr(path, '/') + 1, CONTENTID_SIZE);

		char *rap_path = ALLOC_PATH_BUFFER;

		uint8_t is_pslauncher = !strncmp(content_id, "2P0001-PS2U10000_00-0000111122223333", CONTENTID_SIZE) ||	// is_ps2_classic
								!strncmp(content_id, "UP0001-PSPC66820_00-0000111122223333", CONTENTID_SIZE);	// is_psp_launcher
		
		// ContentID and RAP cached values
		static char cached_content_id[CONTENTID_SIZE] = {0};
		static uint8_t cached_rap[KEY_SIZE] = {0};

		uint8_t *rap = cached_rap;
		int found_rap_in_bin = 0;

		if(!is_pslauncher)
		{
			// Check cache first
			if (strncmp(cached_content_id, content_id, CONTENTID_SIZE) == SUCCEEDED)
			{
				//memcpy(rap, cached_rap, KEY_SIZE); // "rap" already has the value from cached_rap
				found_rap_in_bin = 1;
				#ifdef DEBUG
					DPRINTF("Using cached RAP value for content_id: %s\n", content_id);
				#endif
			}
			else
			{
				// Try to read RAP from rap.bin
				found_rap_in_bin = read_rap_bin("/dev_hdd0/game/PS3XPLOIT/USRDIR/rap.bin", content_id, rap);// PS3HEN default path

				if(!found_rap_in_bin) found_rap_in_bin = read_rap_bin("/dev_hdd0/exdata/rap.bin", content_id, rap);
				if(!found_rap_in_bin) found_rap_in_bin = read_rap_bin("/dev_usb000/exdata/rap.bin", content_id, rap);
				if(!found_rap_in_bin) found_rap_in_bin = read_rap_bin("/dev_usb001/exdata/rap.bin", content_id, rap);

				// cache content_id for current value in cached_rap
				if(found_rap_in_bin)
				{
					strncpy(cached_content_id, content_id, CONTENTID_SIZE);
					
					/*
					#ifdef DEBUG
						char buf[0x100];
						char *ptr = buf;
						int offset = 0;

						// Iterate over each byte of rap_value and convert it to hex format
						for (int i = 0; i < KEY_SIZE; i++) {
							offset += sprintf(ptr + offset, "%02X ", rap[i]);
						}

						DPRINTF("rap_value: %s\n", buf);
					#endif
					*/
				}
			}

			// Support for rap and RAP extension (By aldostools)
			if(!found_rap_in_bin)
			{
				const char *ext = "rap";
				for(uint8_t i = 0; i < 2; i++)
				{
					sprintf(rap_path, "/dev_usb000/exdata/%36s.%s", content_id, ext);

					if(cellFsStat(rap_path, &stat) != CELL_FS_SUCCEEDED) {
						rap_path[10] = '1'; //dev_usb001
						if(cellFsStat(rap_path, &stat) != CELL_FS_SUCCEEDED)
							sprintf(rap_path, "/dev_hdd0/exdata/%36s.%s", content_id, ext);
					}

					if(cellFsStat(rap_path, &stat) != CELL_FS_SUCCEEDED)
						ext = "RAP";
					else
						break;
				}
			}
		}

		int fd;
		if(found_rap_in_bin || is_pslauncher || cellFsOpen(rap_path, CELL_FS_O_RDONLY, &fd, 0666, NULL, 0) == CELL_FS_SUCCEEDED)
		{
			uint64_t nread = 0;

			if (found_rap_in_bin) {
				// rap already has the value copied from cached_rap
				#ifdef DEBUG
					DPRINTF("found_rap_in_bin\n");
				#endif
			} else if (is_pslauncher) {
				#ifdef DEBUG
					DPRINTF("is_pslauncher\n");
				#endif
				// Use the hardcoded values for PS2 and PSP launchers
				memcpy(rap, (uint8_t[]){ 0xF5, 0xDE, 0xCA, 0xBB, 0x09, 0x88, 0x4F, 0xF4, 0x02, 0xD4, 0x12, 0x3C, 0x25, 0x01, 0x71, 0xD9 }, KEY_SIZE);
			} else {
				#ifdef DEBUG
					DPRINTF("rap_path: %s output: %s\n", rap_path, path);
				#endif
				cellFsRead(fd, rap, KEY_SIZE, &nread); // cached_rap
				cellFsClose(fd);

				// cache content_id for current value in cached_rap
				strncpy(cached_content_id, content_id, CONTENTID_SIZE);
			}

			// get path for act.dat
			char *act_path = ALLOC_PATH_BUFFER;
			memset(act_path, 0, 0x50);
			strncpy(act_path, path, strrchr(path, '/') - path);
			strcpy(act_path + strlen(act_path), "/act.dat\0");

			// check if a fake act.dat should be created
			if(cellFsStat(act_path, &stat) != CELL_FS_SUCCEEDED)
			{
				char buffer[120];

				// Search act.dat in home dirs
				for(int i = 1; i < 100; i++)
				{
					sprintf(buffer, ACTDAT_PATH, i);

					if(cellFsStat(buffer, &stat) == CELL_FS_SUCCEEDED)
					{
						#ifdef DEBUG
							DPRINTF("Exit make_rif(): Found act.dat in %08d\n", i);
						#endif
						return; // exit make_rif() -> the act.dat from another user account will be used
					}
				}

				//create a fake act.dat for the current user
				char userid[9];
				memcpy(userid, path + 15, 8);
				userid[8] = '\0';
				sprintf(buffer, ACCOUNTID_VALUE, userid);

				if(xreg_data(buffer))
					create_act_dat(userid);
			}

			#ifdef DEBUG
				DPRINTF("act_path: %s content_id: %s\n", act_path, content_id);
			#endif

			if(cellFsOpen(act_path, CELL_FS_O_RDONLY, &fd, 0666, NULL, 0) == CELL_FS_SUCCEEDED)
			{
				uint8_t *act_dat = ALLOC_ACT_DAT;
				cellFsRead(fd, act_dat, 0x20, &nread); // size: 0x1038 but only first 0x20 are used to make rif
				cellFsClose(fd);

				if(nread == 0x20)
				{
					char *rif_path = ALLOC_PATH_BUFFER;
					sprintf(rif_path, "/%s", path);

					// Skip the creation of rif license if it already exists - By aldostools
					if(skip_existing_rif && cellFsStat(rif_path, &stat) == SUCCEEDED)
					{
						//DPRINTF("license already exists, omitting...);
						return;
					}

					uint8_t rap_key[KEY_SIZE];
					memcpy(rap_key, rap, KEY_SIZE);
					read_act_dat_and_make_rif(rap_key, act_dat, content_id, rif_path);
				}
			}
		}
	}
}

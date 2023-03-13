#include <stdio.h>
#include <lv2/lv2.h>
#include <lv2/libc.h>
#include <lv2/process.h>
#include <lv2/patch.h>
#include <lv2/syscall.h>
#include "common.h"
#include "region.h"

#define N_PS3_REGIONS	12

uint8_t dvd_video_region;
uint8_t bd_video_region;

typedef struct
{
	uint8_t ps3_region;
	uint8_t region;
} RegionMap;

// Maps extracted from bdp_disc_check_plugin reverse
static RegionMap dvd_video_region_map[N_PS3_REGIONS] =
{
	{ 0x83, DVD_VIDEO_REGION_2 }, // Japan
	{ 0x84, DVD_VIDEO_REGION_1 }, // Usa
	{ 0x85, DVD_VIDEO_REGION_2 }, // Europe
	{ 0x86, DVD_VIDEO_REGION_3 }, // Korea
	{ 0x87, DVD_VIDEO_REGION_2 }, // UK
	{ 0x88, DVD_VIDEO_REGION_4 }, // Mexico
	{ 0x89, DVD_VIDEO_REGION_4 }, // Australia
	{ 0x8A, DVD_VIDEO_REGION_3 }, // South Asia
	{ 0x8B, DVD_VIDEO_REGION_3 }, // Taiwan
	{ 0x8C, DVD_VIDEO_REGION_5 }, // Russia
	{ 0x8D, DVD_VIDEO_REGION_6 }, // China
	{ 0x8E, DVD_VIDEO_REGION_3 }, // Hong Kong
};

// Maps extracted from bdp_BDMV.self reverse
static RegionMap bd_video_region_map[N_PS3_REGIONS] =
{
	{ 0x83, BD_VIDEO_REGION_A }, // Japan
	{ 0x84, BD_VIDEO_REGION_A }, // Usa
	{ 0x85, BD_VIDEO_REGION_B }, // Europe
	{ 0x86, BD_VIDEO_REGION_A }, // Korea
	{ 0x87, BD_VIDEO_REGION_B }, // UK
	{ 0x88, BD_VIDEO_REGION_A }, // Mexico
	{ 0x89, BD_VIDEO_REGION_B }, // Australia
	{ 0x8A, BD_VIDEO_REGION_A }, // South Asia
	{ 0x8B, BD_VIDEO_REGION_A }, // Taiwan
	{ 0x8C, BD_VIDEO_REGION_C }, // Russia
	{ 0x8D, BD_VIDEO_REGION_C }, // China
	{ 0x8E, BD_VIDEO_REGION_A }, // Hong Kong
};

static INLINE void set_dvd_video_region(uint8_t *region)
{
	uint8_t fake_region = 0;
	
	for (int i = 0; i < N_PS3_REGIONS; i++)
	{
		if (*region == dvd_video_region_map[i].ps3_region)
		{
			if (dvd_video_region_map[i].region == dvd_video_region)
				return;
		}
		else if (dvd_video_region_map[i].region == dvd_video_region)		
			fake_region = dvd_video_region_map[i].ps3_region;		
	}
	
	if (fake_region != 0)
		*region = fake_region;
}

static INLINE void set_bd_video_region(uint8_t *region)
{
	uint8_t fake_region = 0;
	
	for (int i = 0; i < N_PS3_REGIONS; i++)
	{
		if (*region == bd_video_region_map[i].ps3_region)
		{
			if (bd_video_region_map[i].region == bd_video_region)
				return;
		}
		else if (bd_video_region_map[i].region == bd_video_region)		
			fake_region = bd_video_region_map[i].ps3_region;		
	}
	
	if (fake_region != 0)
		*region = fake_region;
}

LV2_HOOKED_FUNCTION_PRECALL_SUCCESS_2(int, region_func, (uint64_t func, uint8_t *buf))
{
	if (func == 0x19004)
	{
		#ifdef DEBUG
			//DPRINTF("We are originally in region %02X\n", buf[3]);
		#endif
		
		char *procname = get_process_name(get_current_process_critical());
		if (procname)
		{
			if (strcmp(procname+8, "_main_bdp_BDVD.self") == 0)
			{
				if (dvd_video_region != 0)				
					set_dvd_video_region(&buf[3]);				
			}
			else if (strcmp(procname+8, "_main_bdp_BDMV.self") == 0)
			{
				if (bd_video_region != 0)				
					set_bd_video_region(&buf[3]);				
			}
		}		
	}
	
	
	return 0;
}

void region_patches(void)
{
	hook_function_on_precall_success(get_syscall_address(867), region_func, 2);
}

void unhook_all_region(void)
{
	suspend_intr();
	unhook_function_on_precall_success(get_syscall_address(867), region_func, 2);
	resume_intr();
}

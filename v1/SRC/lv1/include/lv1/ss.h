
/*
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; version 2 of the License.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifndef _SS_H_
#define _SS_H_

#include <lv1/lv1.h>

#define SS_FID_VTRM								0x2000
#define SS_PID_VTRM_GET_STATUS					0x2002

#define SS_FID_SRTC								0x3000
#define SS_PID_SRTC_GET_TIME					0x3002

#define SS_FID_SM								0x5000
#define SS_PID_SM_SET_ENCDEC_KEY				0x5001
#define SS_PID_SM_SET_DEL_ENCDEC_KEY			0x5002
#define SS_PID_SM_GET_RND_NUMBER				0x5003
#define SS_PID_SM_DRIVE_AUTH					0x5004
#define SS_PID_SM_PS2_DISC_AUTH					0x5005
#define SS_PID_SM_GET_VERSION					0x5006
#define SS_PID_SM_DRIVE_CTRL					0x5007

#define SS_FID_UM								0x6000
#define SS_PID_UM_UPDATE_PKG					0x6001
#define SS_PID_UM_INSPECT_PKG					0x6002
#define SS_PID_UM_GET_PKG_INFO					0x6003
#define SS_PID_UM_GET_FIX_INSTR					0x6004
#define SS_PID_UM_EXTRACT_PKG					0x6005
#define SS_PID_UM_GET_EXTRACT_PKG				0x6006
#define SS_PID_UM_GET_TOKEN_SEED				0x6009
#define SS_PID_UM_SET_TOKEN						0x600a
#define SS_PID_UM_READ_EPROM					0x600b
#define SS_PID_UM_WRITE_EPROM					0x600c
#define SS_PID_UM_CHECK_INT						0x6010
#define SS_PID_UM_GET_APPL_VER					0x6011

#define SS_FID_SCM								0x9000
#define SS_PID_SCM_GET_REGION_DATA				0x9006
#define SS_PID_SCM_GET_TIME						0x9009
#define SS_PID_SCM_READ_EPROM					0x900b
#define SS_PID_SCM_WRITE_EPROM					0x900c
#define SS_PID_SCM_GET_SC_STATUS				0x900e

#define SS_FID_SECURE_PROFILE_LOADER			0x15000

#define SS_FID_IIM								0x17000
#define SS_PID_IIM_GET_DATA_SIZE				0x17001
#define SS_PID_IIM_GET_DATA						0x17002
#define SS_PID_IIM_GET_CISD_SIZE				0x17015

#define SS_FID_AIM								0x19000
#define SS_PID_AIM_GET_DEV_TYPE					0x19002
#define SS_PID_AIM_GET_DEV_ID					0x19003
#define SS_PID_AIM_GET_PS_CODE					0x19004
#define SS_PID_AIM_GET_OPEN_PS_ID				0x19005

#define SS_FID_USB_DONGLE_AUTH					0x24000
#define SS_PID_USB_DONGLE_AUTH_GEN_CHALLENGE	0x24001
#define SS_PID_USB_DONGLE_AUTH_VERIFY_RESP		0x24002

#define SS_FID_USER_TOKEN_MGR					0x25000

struct ss_header
{
	u64 packet_id;
	u64 function_id;
	u32 retval;
	u8 res[4];
	u64 laid;
	u64 paid;
};

struct ss_vtrm_get_status 
{
	u32 field0;
	u8 res1[4];
	u32 field8;
	u8 res2[4];
	u32 field10;
	u8 res3[4];
};

struct ss_srtc_get_time 
{
	u64 field0;
	u64 field8;
	u64 field10;
};

struct ss_sm_set_encdec_key 
{
	u8 key[24];
	u64 key_size;
	u64 param;
};

struct ss_sm_set_del_encdec_key 
{
	u64 param;
};

struct ss_sm_get_rnd_number 
{
	u8 field0[24];
};

struct ss_sm_drive_auth 
{
	u64 param;
};

struct ss_sm_ps2_disc_auth 
{
	u8 field0[56];
};

struct ss_sm_get_version 
{
	u8 field0[8];
};

struct ss_sm_drive_ctrl 
{
	u64 param;
	u8 field8[16];
};

struct ss_um_update_pkg 
{
	u32 in_lpar_mem;
	u8 res1[4];
	u32 pkg_type;
	u8 res2[4];
	u32 flags;
	u8 res3[4];
	u64 lpar_id;
	u64 pkg_size;
	union 
	{
		struct 
		{
			u64 lpar_addr;
			u64 size;
			u64 field10;
		} lpar_mem_segs[0];

		u8 raw[0];
	} pkg_data;
	/* u64 request_id */
};

struct ss_um_get_pkg_info 
{
	u32 type;
	u8 res[4];
	u64 version;
};

struct ss_um_get_fix_instr 
{
	u8 field0[12];
};

struct ss_um_get_extract_pkg 
{
	u32 in_lpar_mem;
	u8 res1[4];
	u64 field8;
	u32 field10;
	u8 res2[4];
	u64 request_id;
	u64 buf_size;
	u8 buf[0];
};

struct ss_um_read_eprom
{
	u32 offset;
	u8 res[4];
	u8 value;
};

struct ss_um_write_eprom
{
	u32 offset;
	u8 res[4];
	u8 value;
};

struct ss_um_get_applicable_version
{
	u32 field0;
};

struct ss_um_get_package_info
{
	u32 type;
	u8 res[4];
	u64 version;
};

struct ss_um_get_token_seed
{
	u64 token_size;
	u8 token[80];
	u64 seed_size;
	u8 seed[80];
};

struct ss_um_set_token
{
	u64 token_size;
	u8 token[80];
};

struct ss_um_inspect_pkg
{
	u32 field0;
	u8 res1[4];
	u32 pkg_type;
	u8 res2[4];
	u32 field10;
	u8 res3[4];
	u64 lpar_id;
	u64 pkg_size;
	u8 pkg_data[0];
	/* u64 reques_id */
};

struct ss_secure_profile_loader_get
{
	u8 profile_name[256];
	u32 contents_type;
	u8 res[4];
	u8 contents_name[32];
	u64 contents_size;
	u8 contents[0];
	/* u64 fieldX; */
	/* u64 request_type; */
};

struct ss_indi_info_mgr_read_eid0_size
{
	u64 field0;
	u64 field8;
};

struct ss_indi_info_mgr_read_eid0
{
	u64 field0;
	u64 field8;
	u8 field10[0];
	/* u64 fieldX */;
};

struct ss_virtual_trm_mgr_decrypt_master
{
	u8 seed[16];
	u8 msg[64];
};

struct ss_scm_get_region_data 
{
	u64 id;
	u64 data_size;
	u8 data[0];
};

struct ss_scm_get_time 
{
	u64 tid;
	u64 field8;
	u64 field10;
};

struct ss_scm_read_eprom 
{
	u32 offset;
	u8 res1[4];
	u32 nread;
	u8 res2[4];
	u64 buf_size;
	u8 buf[0];
};

struct ss_scm_write_eprom 
{
	u32 offset;
	u8 res1[4];
	u32 nwrite;
	u8 res2[4];
	u64 buf_size;
	u8 buf[0];
};

struct ss_scm_get_sc_status 
{
	u32 version;
	u8 res1[4];
	u32 mode;
	u8 res2[4];
};

struct ss_iim_get_data_size 
{
	u64 index;
	u64 size;
};

struct ss_iim_get_data 
{
	u64 index;
	u64 buf_size;
	u8 buf[0];
	/* u64 data_size */
};

struct ss_iim_get_cisd_size 
{
	u64 size;
};

struct ss_aim_get_dev_type 
{
	u8 field0[16];
};

struct ss_aim_get_dev_id 
{
	u8 field0[16];
};

struct ss_aim_get_ps_code 
{
	u8 field0[8];
};

struct ss_aim_get_open_ps_id 
{
	u8 field0[16];
};

struct ss_usb_dongle_auth_gen_challenge 
{
	u8 header[3];
	u8 challenge[20];
};

struct ss_usb_dongle_auth_verify_resp 
{
	u8 header[3];
	u16 dongle_id;
	u8 response[20];
} __attribute__ ((packed));

#endif

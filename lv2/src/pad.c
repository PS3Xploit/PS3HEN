#include <lv2/lv2.h>
#include <lv2/libc.h>
#include <lv2/hid.h>
#include <lv2/pad.h>

// 20230422 Fixed hanging up issue with more than one device connected (thanks aldostools)
int pad_get_data(pad_data *data)
{
	int ret, timeout = 0;

	const int max_timeout = 1000; // this value may need to be adjusted

	memset(data, 0, sizeof(pad_data));

	while ((ret = hid_mgr_read_usb(0, data, 0x40, 0)) == 0) if(++timeout >= max_timeout) break;

	if (ret == 0xFFFFFFD8 || timeout >= max_timeout)
	{
		// USB failed, try BT now...

		uint16_t len; timeout = 0;

		do
		{
			len = 0x40;
			ret = hid_mgr_read_bt(0, data, &len, 1);

			if (ret == 0)			
				ret = len;			

			if (++timeout >= max_timeout) break;

		} while (ret == 0);
	}

	return ret;
}


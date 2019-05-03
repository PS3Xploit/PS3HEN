#ifndef __SM_EXT_H__
#define __SM_EXT_H__

/* Causes a power off event, which will be caught by vsh. Clean poweroff and reboots, unlike calling sys_sm_shutdown */
int sys_sm_ext_send_poweroff_event(int reboot);

#endif /* __SM_EXT_H__ */

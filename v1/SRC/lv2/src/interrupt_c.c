#include <lv2/lv2.h>
#include <lv2/interrupt.h>
#include <lv2/thread.h>

uint64_t spin_lock_irqsave(void) 
{
	uint8_t *ta = (uint8_t *)get_current_thread();
	return spin_lock_irqsave_ex(ta+0x28);
}

void spin_unlock_irqrestore(uint64_t restore)
{
	uint8_t *ta = (uint8_t *)get_current_thread();
	spin_unlock_irqrestore_ex(ta+0x28, restore, 0);
}




#ifndef __LV2_INTERRUPT_H__
#define __LV2_INTERRUPT_H__

#include <lv2/lv2.h>

void suspend_intr(void);
void resume_intr(void);

LV2_EXPORT uint64_t spin_lock_irqsave_ex(void *ta);
LV2_EXPORT void spin_unlock_irqrestore_ex(void *ta, uint64_t restore, int zero);

uint64_t spin_lock_irqsave(void);
void spin_unlock_irqrestore(uint64_t restore);

#endif /* __LV2_INTERRUPT_H__ */


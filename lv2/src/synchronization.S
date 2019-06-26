#include <lv2/macros.h.S>
#include <lv2/symbols.h>

LV2_FUNCTION(mutex_create, mutex_create_symbol)
LV2_FUNCTION(mutex_destroy, mutex_destroy_symbol)
LV2_FUNCTION(mutex_lock, mutex_lock_symbol)
LV2_FUNCTION(mutex_lock_ex, mutex_lock_ex_symbol)
LV2_FUNCTION(mutex_trylock, mutex_trylock_symbol)
LV2_FUNCTION(mutex_unlock, mutex_unlock_symbol)

#ifdef cond_create_symbol
LV2_FUNCTION(cond_create, cond_create_symbol)
LV2_FUNCTION(cond_destroy, cond_destroy_symbol)
LV2_FUNCTION(cond_wait, cond_wait_symbol)
LV2_FUNCTION(cond_wait_ex, cond_wait_ex_symbol)
LV2_FUNCTION(cond_signal, cond_signal_symbol)
LV2_FUNCTION(cond_signal_all, cond_signal_all_symbol)
#endif

#ifdef semaphore_initialize_symbol
LV2_FUNCTION(semaphore_initialize, semaphore_initialize_symbol)
LV2_FUNCTION(semaphore_wait_ex, semaphore_wait_ex_symbol)
LV2_FUNCTION(semaphore_trywait, semaphore_trywait_symbol)
LV2_FUNCTION(semaphore_post_ex, semaphore_post_ex_symbol)
#endif

LV2_FUNCTION(event_port_create, event_port_create_symbol)
LV2_FUNCTION(event_port_destroy, event_port_destroy_symbol)
LV2_FUNCTION(event_port_connect, event_port_connect_symbol)
LV2_FUNCTION(event_port_disconnect, event_port_disconnect_symbol)
LV2_FUNCTION(event_port_send, event_port_send_symbol)
LV2_FUNCTION(event_port_send_ex, event_port_send_ex_symbol)

LV2_FUNCTION(event_queue_create, event_queue_create_symbol)
LV2_FUNCTION(event_queue_destroy, event_queue_destroy_symbol)
LV2_FUNCTION(event_queue_drain, event_queue_drain_symbol)
LV2_FUNCTION(event_queue_receive, event_queue_receive_symbol)
LV2_FUNCTION(event_queue_tryreceive, event_queue_tryreceive_symbol)



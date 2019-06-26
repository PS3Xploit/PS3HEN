#ifndef __LV2_SYNCHRONIZATION_H__
#define __LV2_SYNCHRONIZATION_H__

#include <stdint.h>
#include <sys/types.h>
#include <lv2/lv2.h>

#define EVENT_PORT_REMOTE		0x00
#define EVENT_PORT_LOCAL		0x01

/* Values for attr_protocol */
#define SYNC_FIFO				0x00001
#define SYNC_PRIORITY			0x00002
#define SYNC_PRIORITY_INHERIT	0x00003
#define SYNC_RETRY				0x00004

/* Values for attr_recursive */
#define SYNC_RECURSIVE			0x00010
#define SYNC_NOT_RECURSIVE		0x00020

/* Values for attr_pshared */
#define SYNC_NOT_PROCESS_SHARED	0x00200

typedef struct
{
	uint64_t unk_00;
	// ...
} __attribute__((packed)) *mutex_t;

typedef struct
{
	uint64_t unk_00;
	// ...
} __attribute__((packed)) *cond_t;

typedef struct
{
	uint8_t unk[0x38];
} __attribute__((packed)) sema_t;

typedef struct
{
	uint64_t unk_00;
} __attribute__((packed)) *event_port_t;

typedef struct
{
	uint64_t source;
	uint64_t data1;
	uint64_t data2;
	uint64_t data3;
} event_t;

typedef struct
{
	uint64_t unk_00;
} __attribute__((packed)) *event_queue_t;

// User defines
typedef uint32_t sys_event_queue_t;
typedef uint32_t sys_event_port_t;

LV2_EXPORT int mutex_create(mutex_t *mutex, uint32_t attr_protocol, uint32_t attr_recursive);
LV2_EXPORT int mutex_destroy(mutex_t mutex);
LV2_EXPORT int mutex_lock(mutex_t mutex, uint64_t timeout);
LV2_EXPORT int mutex_lock_ex(mutex_t mutex, uint64_t timeout, uint64_t unk);
LV2_EXPORT int mutex_trylock(mutex_t mutex);
LV2_EXPORT int mutex_unlock(mutex_t mutex);

LV2_EXPORT int cond_create(cond_t *condition, mutex_t mutex);
LV2_EXPORT int cond_destroy(cond_t condition);
LV2_EXPORT int cond_wait(cond_t condition, uint64_t timeout);
LV2_EXPORT int cond_wait_ex(cond_t condition, uint64_t timeout, uint64_t unk);
LV2_EXPORT int cond_signal(cond_t condition);
LV2_EXPORT int cond_signal_all(cond_t condition);

LV2_EXPORT void semaphore_initialize(sema_t *sem, uint32_t attr_protocol, int initial_val, int max_val, uint32_t attr_pshared);
LV2_EXPORT int semaphore_wait_ex(sema_t *sem, uint64_t timeout, int unk);
static INLINE int semaphore_wait(sema_t *sem, uint64_t timeout) { return semaphore_wait_ex(sem, timeout, 0); }
LV2_EXPORT int semaphore_trywait_ex(sema_t *sem);
LV2_EXPORT int semaphore_post_ex(sema_t *sem, int val, int unk);
static INLINE int semaphore_post(sema_t *sem, int val) { return semaphore_post_ex(sem, val, 1); }

LV2_EXPORT int event_port_create(event_port_t *event_port, int port_type);
LV2_EXPORT int event_port_destroy(event_port_t event_port);
LV2_EXPORT int event_port_connect(event_port_t event_port, event_queue_t event_queue);
LV2_EXPORT int event_port_disconnect(event_port_t event_port);
LV2_EXPORT int event_port_send(event_port_t event_port, uint64_t data1, uint64_t data2, uint64_t data3);
LV2_EXPORT int event_port_send_ex(event_port_t, uint64_t data1, uint64_t data2, uint64_t data3, uint64_t unk);

LV2_EXPORT int event_queue_create(event_queue_t *event_queue, uint32_t attr_protocol, uint64_t event_queue_key, int size);
LV2_EXPORT int event_queue_destroy(event_queue_t event_queue);
LV2_EXPORT int event_queue_drain(event_queue_t event_queue);
LV2_EXPORT int event_queue_receive(event_queue_t event_queue, event_t *event, uint64_t timeout);
LV2_EXPORT int event_queue_tryreceive(event_queue_t event_queue, event_t *event_array, int size, int *number);

#endif /* __LV2_SYNCHRONIZATION_H__ */


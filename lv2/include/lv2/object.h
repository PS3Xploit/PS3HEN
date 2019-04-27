#ifndef __LV2_OBJECT_H__ 
#define __LV2_OBJECT_H__

#include <lv2/lv2.h>

#define SYS_PROCESS_OBJECT				(0x01UL)
#define SYS_MEM_OBJECT					(0x08UL)
#define SYS_INTR_TAG_OBJECT				(0x0AUL)
#define SYS_INTR_SERVICE_HANDLE_OBJECT	(0x0BUL)
#define SYS_EVENT_PORT_OBJECT			(0x0EUL)
#define SYS_TIMER_OBJECT				(0x11UL)
#define SYS_TRACE_OBJECT				(0x21UL)
#define SYS_SPUIMAGE_OBJECT				(0x22UL)
#define SYS_PRX_OBJECT					(0x23UL)
#define SYS_SPUPORT_OBJECT				(0x24UL)
#define SYS_IO_BUFFER_OBJECT			(0x44UL)
#define SYS_STORAGE_HANDLE_OBJECT		(0x45UL)
#define SYS_FS_FD_OBJECT				(0x73UL)
#define SYS_MUTEX_OBJECT				(0x85UL)
#define SYS_COND_OBJECT					(0x86UL)
#define SYS_RWLOCK_OBJECT				(0x88UL)
#define SYS_EVENT_QUEUE_OBJECT			(0x8DUL)
#define SYS_LWMUTEX_OBJECT				(0x95UL)
#define SYS_SEMAPHORE_OBJECT			(0x96UL)
#define SYS_LWCOND_OBJECT				(0x97UL)
#define SYS_EVENT_FLAG_OBJECT			(0x98UL)

typedef struct
{
	void *dummy;
	// ...
} *object_handle_t;

/* Creates a kernel object */
LV2_EXPORT int create_kernel_object(void *object_table, void *private_kernel_object, uint32_t *uid, uint32_t object_type, object_handle_t *handle);
/* Destroys a kernel object only for this process. The process with object_table cannot access this kernel object by uid anymore. Private kernel object is not destroyed. */
LV2_EXPORT int destroy_kernel_object(void *object_table, object_handle_t handle);
/* Destroys a kernel object for all processes. Kernel object is not accesible by uid anymore. Private kernel object is not destroyed. */
LV2_EXPORT int destroy_shared_kernel_object(void *object_table, object_handle_t handle);
/* Opens a kernel object. */
LV2_EXPORT int open_kernel_object(void *object_table, uint32_t uid, void **private_kernel_object, object_handle_t *handle, uint32_t object_type);
/* Opens a kernel object that may be shared between processes. 
   If shared is 0 and the object is already owned by some process, the function will return an error. Otherwise it will succeed. */
LV2_EXPORT int open_shared_kernel_object(void *object_table, uint32_t uid, void **private_kernel_object, object_handle_t *handle, uint32_t object_type, int shared);
/* Close the kernel object handle returned by create/open kernel_object functions */
LV2_EXPORT int close_kernel_object_handle(void *object_table, object_handle_t handle);


#endif /* __LV2_OBJECT_H__ */

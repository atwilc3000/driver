#ifndef __ATL_platfrom_H__
#define __ATL_platfrom_H__

/*!  
*  @file	atl_platform.h
*  @brief	platform specific file for Linux port
*  @author	syounan
*  @sa		atl_os_wrapper.h top level OS wrapper file
*  @date	15 Dec 2010
*  @version	1.0
*/


/******************************************************************
        Feature support checks
*******************************************************************/

// CONFIG_ATL_THREAD_FEATURE is implemented

/* remove the following block when implementing its feature */
#ifdef CONFIG_ATL_THREAD_SUSPEND_CONTROL
#error This feature is not supported by this OS
#endif

/* remove the following block when implementing its feature */
#ifdef CONFIG_ATL_THREAD_STRICT_PRIORITY
#error This feature is not supported by this OS
#endif

// CONFIG_ATL_SEMAPHORE_FEATURE is implemented

/* remove the following block when implementing its feature 
#ifdef CONFIG_ATL_SEMAPHORE_TIMEOUT
#error This feature is not supported by this OS
#endif*/

// CONFIG_ATL_SLEEP_FEATURE is implemented

/* remove the following block when implementing its feature */
//#ifdef CONFIG_ATL_SLEEP_HI_RES
//#error This feature is not supported by this OS
//#endif

// CONFIG_ATL_TIMER_FEATURE is implemented

// CONFIG_ATL_TIMER_PERIODIC is implemented

// CONFIG_ATL_MEMORY_FEATURE is implemented

/* remove the following block when implementing its feature */
#ifdef CONFIG_ATL_MEMORY_POOLS
#error This feature is not supported by this OS
#endif

/* remove the following block when implementing its feature */
#ifdef CONFIG_ATL_MEMORY_DEBUG
#error This feature is not supported by this OS
#endif

/* remove the following block when implementing its feature */
#ifdef CONFIG_ATL_ASSERTION_SUPPORT
#error This feature is not supported by this OS
#endif

// CONFIG_ATL_STRING_UTILS is implemented

// CONFIG_ATL_MSG_QUEUE_FEATURE is implemented

/* remove the following block when implementing its feature */
#ifdef CONFIG_ATL_MSG_QUEUE_IPC_NAME
#error This feature is not supported by this OS
#endif

/* remove the following block when implementing its feature */
/*#ifdef CONFIG_ATL_MSG_QUEUE_TIMEOUT
#error This feature is not supported by this OS
#endif*/

// CONFIG_ATL_FILE_OPERATIONS_FEATURE is implemented

// CONFIG_ATL_FILE_OPERATIONS_STRING_API is implemented

/* remove the following block when implementing its feature */
#ifdef CONFIG_ATL_FILE_OPERATIONS_PATH_API
#error This feature is not supported by this OS
#endif

// CONFIG_ATL_TIME_FEATURE is implemented

/* remove the following block when implementing its feature */
#ifdef CONFIG_ATL_TIME_UTC_SINCE_1970
#error This feature is not supported by this OS
#endif

/* remove the following block when implementing its feature */
#ifdef CONFIG_ATL_TIME_CALENDER
#error This feature is not supported by this OS
#endif

/* remove the following block when implementing its feature */
#ifdef CONFIG_ATL_EVENT_FEATURE
#error This feature is not supported by this OS
#endif

/* remove the following block when implementing its feature */
#ifdef CONFIG_ATL_EVENT_TIMEOUT
#error This feature is not supported by this OS
#endif

// CONFIG_ATL_MATH_OPERATIONS_FEATURE is implemented

// CONFIG_ATL_EXTENDED_FILE_OPERATIONS is implemented

// CONFIG_ATL_EXTENDED_STRING_OPERATIONS is implemented

// CONFIG_ATL_EXTENDED_TIME_OPERATIONS is implemented

/* remove the following block when implementing its feature */
#ifdef CONFIG_ATL_SOCKET_FEATURE
#error This feature is not supported by this OS
#endif

/******************************************************************
	OS specific includes
*******************************************************************/
#define _XOPEN_SOURCE 600

#include <linux/kthread.h>
#include <linux/semaphore.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/kernel.h>
#include <linux/delay.h>
#include <linux/types.h>
#include <linux/stat.h> 
#include <linux/time.h> 
#include <linux/version.h>
#include "linux/string.h"
/******************************************************************
	OS specific types
*******************************************************************/

typedef struct task_struct *ATL_ThreadHandle;

typedef void* ATL_MemoryPoolHandle;
typedef struct semaphore ATL_SemaphoreHandle;

typedef struct timer_list ATL_TimerHandle ;



/* Message Queue type is a structure */
typedef struct __Message_struct
{
	void* pvBuffer;
	ATL_Uint32 u32Length;
	struct __Message_struct *pstrNext;
} Message;

typedef struct __MessageQueue_struct
{	
	ATL_SemaphoreHandle hSem;
	spinlock_t strCriticalSection;
	ATL_Bool bExiting;
	ATL_Uint32 u32ReceiversCount;
	Message * pstrMessageList;
} ATL_MsgQueueHandle;



/*Time represented in 64 bit format*/
typedef time_t ATL_Time;


/*******************************************************************
	others
********************************************************************/

/* Generic printf function */
#define ATL_PRINTF(...) printk(__VA_ARGS__)
#define __ATL_FILE__		__FILE__
#define __ATL_FUNCTION__	__FUNCTION__
#define __ATL_LINE__		__LINE__
#endif

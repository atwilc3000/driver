#ifndef __ATL_OSWRAPPER_H__
#define __ATL_OSWRAPPER_H__

/*!  
*  @file	atl_os_wrapper.h
*  @brief	Top level OS Wrapper, include this file and it will include all 
		other files as necessary
*  @author	syounan
*  @date	10 Aug 2010
*  @version	1.0
*/

/* OS Wrapper interface version */
#define ATL_OSW_INTERFACE_VER 2

/* Integer Types */
typedef unsigned char      ATL_Uint8;
typedef unsigned short     ATL_Uint16;
typedef unsigned int       ATL_Uint32;
typedef unsigned long long ATL_Uint64;
typedef signed char        ATL_Sint8;
typedef signed short       ATL_Sint16;
typedef signed int         ATL_Sint32;
typedef signed long long   ATL_Sint64;

/* Floating types */
typedef float  ATL_Float;
typedef double ATL_Double;

/* Boolean type */
typedef enum {
	ATL_FALSE = 0,
	ATL_TRUE = 1	
}ATL_Bool;

/* Character types */
typedef char ATL_Char;
typedef ATL_Uint16 ATL_WideChar;

#define ATL_OS_INFINITY (~((ATL_Uint32)0))
#define ATL_NULL ((void*)0)

/* standard min and max macros */
#define ATL_MIN(a, b)  (((a) < (b)) ? (a) : (b))
#define ATL_MAX(a, b)  (((a) > (b)) ? (a) : (b))

/* Os Configuration File */
#include "atl_os_config.h"

/* Platform specific include */
#if ATL_PLATFORM == ATL_WIN32
#include "atl_os_wrapper/source/win32/include/atl_platform.h"
#elif ATL_PLATFORM == ATL_NU
#include "atl_os_wrapper/source/nucleus/include/atl_platform.h"
#elif ATL_PLATFORM == ATL_MTK
#include "atl_os_wrapper/source/mtk/include/atl_platform.h"
#elif ATL_PLATFORM == ATL_LINUX
#include "atl_os_wrapper/source/linux/include/atl_platform.h"
#elif ATL_PLATFORM == ATL_LINUXKERNEL
#include "atl_platform.h"
#else
#error "OS not supported"
#endif

/* Logging Functions */
#include "atl_log.h"

/* Error reporting and handling support */
#include "atl_error_support.h"

/* Thread support */
#ifdef CONFIG_ATL_THREAD_FEATURE
#include "atl_thread.h"
#endif

/* Semaphore support */
#ifdef CONFIG_ATL_SEMAPHORE_FEATURE
#include "atl_semaphore.h"
#endif

/* Sleep support */
#ifdef CONFIG_ATL_SLEEP_FEATURE
#include "atl_sleep.h"
#endif

/* Timer support */
#ifdef CONFIG_ATL_TIMER_FEATURE
#include "atl_timer.h"
#endif

/* Memory support */
#ifdef CONFIG_ATL_MEMORY_FEATURE
#include "atl_memory.h"
#endif

/* String Utilities */
#ifdef CONFIG_ATL_STRING_UTILS
#include "atl_str_utils.h"
#endif

/* Message Queue */
#ifdef CONFIG_ATL_MSG_QUEUE_FEATURE
#include "atl_msg_queue.h"
#endif

/* File operations */
#ifdef CONFIG_ATL_FILE_OPERATIONS_FEATURE
#include "atl_file_ops.h"
#endif

/* Time operations */
#ifdef CONFIG_ATL_TIME_FEATURE
#include "atl_time.h"
#endif

/* Event support */
#ifdef CONFIG_ATL_EVENT_FEATURE
#include "atl_event.h"
#endif

/* Socket operations */
#ifdef CONFIG_ATL_SOCKET_FEATURE
#include "atl_socket.h"
#endif

/* Math operations */
#ifdef CONFIG_ATL_MATH_OPERATIONS_FEATURE
#include "atl_math.h"
#endif



#endif

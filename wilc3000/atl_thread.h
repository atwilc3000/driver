#ifndef __ATL_THREAD_H__
#define __ATL_THREAD_H__

/*!  
*  @file		atl_thread.h
*  @brief		Thread OS Wrapper functionality
*  @author		syounan
*  @sa			ATWILC_OSWrapper.h top level OS wrapper file
*  @date		10 Aug 2010
*  @version		1.0
*/

#ifndef CONFIG_ATL_THREAD_FEATURE
#error the feature ATL_OS_FEATURE_THREAD must be supported to include this file
#endif

typedef void (*tpfATL_ThreadFunction)(void*);

typedef enum 
{
	#ifdef CONFIG_ATL_THREAD_STRICT_PRIORITY
	ATL_OS_THREAD_PIORITY_0 = 0,
	ATL_OS_THREAD_PIORITY_1 = 1,
	ATL_OS_THREAD_PIORITY_2 = 2,
	ATL_OS_THREAD_PIORITY_3 = 3,
	ATL_OS_THREAD_PIORITY_4 = 4,
	#endif

	ATL_OS_THREAD_PIORITY_HIGH = 0,
	ATL_OS_THREAD_PIORITY_NORMAL = 2,
	ATL_OS_THREAD_PIORITY_LOW = 4
}tenuATL_ThreadPiority;

/*!
*  @struct 		ATL_ThreadAttrs
*  @brief		Thread API options 
*  @author		syounan
*  @date		10 Aug 2010
*  @version		1.0
*/
typedef struct
{
	/*!<
	stack size for use with ATL_ThreadCreate, default is ATL_OS_THREAD_DEFAULT_STACK
	*/
	ATL_Uint32 u32StackSize;

	/*!<
	piority for the thread, if ATL_OS_FEATURE_THREAD_STRICT_PIORITY is defined
	this value is strictly observed and can take a larger resolution
	*/
	tenuATL_ThreadPiority enuPiority;
	
	#ifdef CONFIG_ATL_THREAD_SUSPEND_CONTROL
	/*!
	if true the thread will be created suspended
	*/
	ATL_Bool bStartSuspended;
	#endif
	
}tstrATL_ThreadAttrs;

#define ATL_OS_THREAD_DEFAULT_STACK (10*1024)

/*!
*  @brief	Fills the ATL_ThreadAttrs with default parameters
*  @param[out]	pstrAttrs structure to be filled
*  @sa		ATL_ThreadAttrs
*  @author	syounan
*  @date	10 Aug 2010
*  @version	1.0
*/

static void ATL_ThreadFillDefault(tstrATL_ThreadAttrs* pstrAttrs)
{
	pstrAttrs->u32StackSize = ATL_OS_THREAD_DEFAULT_STACK;
	pstrAttrs->enuPiority = ATL_OS_THREAD_PIORITY_NORMAL;
	
	#ifdef CONFIG_ATL_THREAD_SUSPEND_CONTROL
	pstrAttrs->bStartSuspended = ATL_FALSE;
	#endif
}

/*!
*  @brief	Creates a new thread
*  @details	if the feature ATL_OS_FEATURE_THREAD_SUSPEND_CONTROL is 
		defined and tstrATL_ThreadAttrs.bStartSuspended is set to true 
		the new thread will be created in suspended state, otherwise
		it will start executing immeadiately
		if the feature ATL_OS_FEATURE_THREAD_STRICT_PIORITY is defined
		piorities are strictly observed, otherwise the underlaying OS 
		may not observe piorities
*  @param[out]	pHandle handle to the newly created thread object
*  @param[in]	pfEntry pointer to the entry point of the new thread
*  @param[in]	pstrAttrs Optional attributes, NULL for default
*  @return	Error code indicating sucess/failure
*  @sa		ATL_ThreadAttrs
*  @author	syounan
*  @date	10 Aug 2010
*  @version	1.0
*/
ATL_ErrNo ATL_ThreadCreate(ATL_ThreadHandle* pHandle, tpfATL_ThreadFunction pfEntry,
	void* pvArg, tstrATL_ThreadAttrs* pstrAttrs);

/*!
*  @brief	Destroys the Thread object
*  @details	This function is used for clean up and freeing any used resources
*		This function will block until the destroyed thread exits cleanely,
*		so, the thread code thould handle an exit case before this calling
*		this function
*  @param[in]	pHandle handle to the thread object
*  @param[in]	pstrAttrs Optional attributes, NULL for default
*  @return	Error code indicating sucess/failure
*  @sa		ATL_ThreadAttrs
*  @author	syounan
*  @date	10 Aug 2010
*  @version	1.0
*/
ATL_ErrNo ATL_ThreadDestroy(ATL_ThreadHandle* pHandle,
	tstrATL_ThreadAttrs* pstrAttrs);

#ifdef CONFIG_ATL_THREAD_SUSPEND_CONTROL

/*!
*  @brief	Suspends an executing Thread object
*  @param[in]	pHandle handle to the thread object
*  @param[in]	pstrAttrs Optional attributes, NULL for default
*  @return	Error code indicating sucess/failure
*  @sa		ATL_ThreadAttrs
*  @note	Optional part, ATL_OS_FEATURE_THREAD_SUSPEND_CONTROL must be enabled
*  @author	syounan
*  @date	10 Aug 2010
*  @version	1.0
*/
ATL_ErrNo ATL_ThreadSuspend(ATL_ThreadHandle* pHandle,
	tstrATL_ThreadAttrs* pstrAttrs);

/*!
*  @brief	Resumes a suspened Thread object
*  @param[in]	pHandle handle to the thread object
*  @param[in]	pstrAttrs Optional attributes, NULL for default
*  @return	Error code indicating sucess/failure
*  @sa		ATL_ThreadAttrs
*  @note	Optional part, ATL_OS_FEATURE_THREAD_SUSPEND_CONTROL must be enabled
*  @author	syounan
*  @date	10 Aug 2010
*  @version	1.0
*/
ATL_ErrNo ATL_ThreadResume(ATL_ThreadHandle* pHandle,
	tstrATL_ThreadAttrs* pstrAttrs);

#endif


#endif

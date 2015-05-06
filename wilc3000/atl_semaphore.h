#ifndef __ATL_SEMAPHORE_H__
#define __ATL_SEMAPHORE_H__

/*!  
*  @file		atl_semaphore.h
*  @brief		Semaphore OS Wrapper functionality
*  @author		syounan
*  @sa			ATWILC_OSWrapper.h top level OS wrapper file
*  @date		10 Aug 2010
*  @version		1.0
*/


#ifndef CONFIG_ATL_SEMAPHORE_FEATURE
#error the feature ATL_OS_FEATURE_SEMAPHORE must be supported to include this file
#endif

/*!
*  @struct 		ATL_SemaphoreAttrs
*  @brief		Semaphore API options 
*  @author		syounan
*  @date		10 Aug 2010
*  @version		1.0
*/
typedef struct
{
	/*!<
	Initial count when the semaphore is created. default is 1
	*/
	ATL_Uint32 u32InitCount;

	#ifdef CONFIG_ATL_SEMAPHORE_TIMEOUT
	/*!<
	Timeout for use with ATL_SemaphoreAcquire, 0 to return immediately and 
	ATL_OS_INFINITY to wait forever. default is ATL_OS_INFINITY
	*/
	ATL_Uint32 u32TimeOut; 
	#endif
	
}tstrATL_SemaphoreAttrs;


/*!
*  @brief	Fills the ATL_SemaphoreAttrs with default parameters
*  @param[out]	pstrAttrs structure to be filled
*  @sa		ATL_SemaphoreAttrs
*  @author	syounan
*  @date	10 Aug 2010
*  @version	1.0
*/
static void ATL_SemaphoreFillDefault(tstrATL_SemaphoreAttrs* pstrAttrs)
{
	pstrAttrs->u32InitCount = 1;
	#ifdef CONFIG_ATL_SEMAPHORE_TIMEOUT
	pstrAttrs->u32TimeOut = ATL_OS_INFINITY;
	#endif
}
/*!
*  @brief	Creates a new Semaphore object
*  @param[out]	pHandle handle to the newly created semaphore
*  @param[in]	pstrAttrs Optional attributes, NULL for defaults
				pstrAttrs->u32InitCount controls the initial count
*  @return	Error code indicating success/failure
*  @sa		ATL_SemaphoreAttrs
*  @author	syounan
*  @date	10 Aug 2010
*  @version	1.0
*/
ATL_ErrNo ATL_SemaphoreCreate(ATL_SemaphoreHandle* pHandle, 
	tstrATL_SemaphoreAttrs* pstrAttrs);

/*!
*  @brief	Destroyes an existing Semaphore, releasing any resources
*  @param[in]	pHandle handle to the semaphore object
*  @param[in]	pstrAttrs Optional attributes, NULL for defaults
*  @return	Error code indicating success/failure
*  @sa		ATL_SemaphoreAttrs
*  @todo	need to define behaviour if the semaphore delayed while it is pending
*  @author	syounan
*  @date	10 Aug 2010
*  @version	1.0
*/
ATL_ErrNo ATL_SemaphoreDestroy(ATL_SemaphoreHandle* pHandle,
	tstrATL_SemaphoreAttrs* pstrAttrs);

/*!
*  @brief	Acquire the Semaphore object
*  @details	This function will block until it can Acquire the given 
*		semaphore, if the feature ATL_OS_FEATURE_SEMAPHORE_TIMEOUT is
*		eanbled a timeout value can be passed in pstrAttrs
*  @param[in]	pHandle handle to the semaphore object
*  @param[in]	pstrAttrs Optional attributes, NULL for default
*  @return	Error code indicating success/failure
*  @sa		ATL_SemaphoreAttrs
*  @author	syounan
*  @date	10 Aug 2010
*  @version	1.0
*/
ATL_ErrNo ATL_SemaphoreAcquire(ATL_SemaphoreHandle* pHandle,
	tstrATL_SemaphoreAttrs* pstrAttrs);

/*!
*  @brief	Release the Semaphore object
*  @param[in]	pHandle handle to the semaphore object
*  @param[in]	pstrAttrs Optional attributes, NULL for default
*  @return	Error code indicating sucess/failure
*  @sa		ATL_SemaphoreAttrs
*  @author	syounan
*  @date	10 Aug 2010
*  @version	1.0
*/
ATL_ErrNo ATL_SemaphoreRelease(ATL_SemaphoreHandle* pHandle,
	tstrATL_SemaphoreAttrs* pstrAttrs);


#endif

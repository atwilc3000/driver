
#include "atl_os_wrapper.h"
#ifdef CONFIG_ATL_SEMAPHORE_FEATURE


ATL_ErrNo ATL_SemaphoreCreate(ATL_SemaphoreHandle* pHandle, 
	tstrATL_SemaphoreAttrs* pstrAttrs)
{
	tstrATL_SemaphoreAttrs strDefaultAttrs;
	if(pstrAttrs == ATL_NULL)
	{
		ATL_SemaphoreFillDefault(&strDefaultAttrs);
		pstrAttrs = &strDefaultAttrs;
	}

	sema_init(pHandle,pstrAttrs->u32InitCount);	
	return ATL_SUCCESS;
	
}


ATL_ErrNo ATL_SemaphoreDestroy(ATL_SemaphoreHandle* pHandle,
	tstrATL_SemaphoreAttrs* pstrAttrs)
{
	/* nothing to be done ! */
	
	return ATL_SUCCESS;	

}


ATL_ErrNo ATL_SemaphoreAcquire(ATL_SemaphoreHandle* pHandle,
	tstrATL_SemaphoreAttrs* pstrAttrs)
{
	ATL_ErrNo s32RetStatus = ATL_SUCCESS;

	#ifndef CONFIG_ATL_SEMAPHORE_TIMEOUT
	//while(down_interruptible(pHandle)); //jude
	down(pHandle);

	#else
	if(pstrAttrs == ATL_NULL)
	{
		down(pHandle);
	}
	else
	{
		
		s32RetStatus = down_timeout(pHandle, msecs_to_jiffies(pstrAttrs->u32TimeOut));
	}
	#endif

	if(s32RetStatus == 0)
	{
		return ATL_SUCCESS;
	}
	else if (s32RetStatus == -ETIME)
	{
		return ATL_TIMEOUT;
	}
	else
	{
		return ATL_FAIL;
	}
	
	return ATL_SUCCESS;
	
}

ATL_ErrNo ATL_SemaphoreRelease(ATL_SemaphoreHandle* pHandle,
	tstrATL_SemaphoreAttrs* pstrAttrs)
{

	up(pHandle);
	return ATL_SUCCESS;
	
}

#endif

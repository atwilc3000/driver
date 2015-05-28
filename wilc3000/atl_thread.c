
#include "atl_os_wrapper.h"

#ifdef CONFIG_ATL_THREAD_FEATURE



ATL_ErrNo ATL_ThreadCreate(ATL_ThreadHandle* pHandle, tpfATL_ThreadFunction pfEntry,
	void* pvArg, tstrATL_ThreadAttrs* pstrAttrs)
{


	*pHandle = kthread_run((int(*)(void *))pfEntry ,pvArg, "ATL_kthread");


	if(IS_ERR(*pHandle))
	{
		return ATL_FAIL;
	}
	else
	{
		return ATL_SUCCESS;
	}
	
}

ATL_ErrNo ATL_ThreadDestroy(ATL_ThreadHandle* pHandle,
	tstrATL_ThreadAttrs* pstrAttrs)
{	
	ATL_ErrNo s32RetStatus = ATL_SUCCESS;

	kthread_stop(*pHandle);
	return s32RetStatus;
}



#endif

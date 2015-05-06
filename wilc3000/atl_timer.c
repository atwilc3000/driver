
#include "atl_os_wrapper.h"

#ifdef CONFIG_ATL_TIMER_FEATURE



ATL_ErrNo ATL_TimerCreate(ATL_TimerHandle* pHandle, 
	tpfATL_TimerFunction pfCallback, tstrATL_TimerAttrs* pstrAttrs)
{	
	ATL_ErrNo s32RetStatus = ATL_SUCCESS;
	setup_timer(pHandle,(void(*)(unsigned long))pfCallback,0);

	return s32RetStatus;
}

ATL_ErrNo ATL_TimerDestroy(ATL_TimerHandle* pHandle, 
	tstrATL_TimerAttrs* pstrAttrs)
{
	ATL_ErrNo s32RetStatus = ATL_FAIL;
	if(pHandle != NULL){
		s32RetStatus = del_timer_sync(pHandle);
		pHandle = NULL;
	}

	return s32RetStatus;
}

	
ATL_ErrNo ATL_TimerStart(ATL_TimerHandle* pHandle, ATL_Uint32 u32Timeout, 
	void* pvArg, tstrATL_TimerAttrs* pstrAttrs)
{
	ATL_ErrNo s32RetStatus = ATL_FAIL;
	if(pHandle != NULL){
		pHandle->data = (unsigned long)pvArg;
		s32RetStatus = mod_timer(pHandle, (jiffies + msecs_to_jiffies(u32Timeout)));
		}
	return s32RetStatus;	
}

ATL_ErrNo ATL_TimerStop(ATL_TimerHandle* pHandle, 
	tstrATL_TimerAttrs* pstrAttrs)
{
	ATL_ErrNo s32RetStatus = ATL_FAIL;
	if(pHandle != NULL)
		s32RetStatus = del_timer(pHandle);

	return s32RetStatus;
}

#endif

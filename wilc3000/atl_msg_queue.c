
#include "atl_os_wrapper.h"
#include <linux/spinlock.h>
#ifdef CONFIG_ATL_MSG_QUEUE_FEATURE


/*!
*  @author		syounan
*  @date		1 Sep 2010
*  @note		copied from FLO glue implementatuion
*  @version		1.0
*/
ATL_ErrNo ATL_MsgQueueCreate(ATL_MsgQueueHandle* pHandle,
			tstrATL_MsgQueueAttrs* pstrAttrs)
{
	tstrATL_SemaphoreAttrs strSemAttrs;
	ATL_SemaphoreFillDefault(&strSemAttrs);
	strSemAttrs.u32InitCount = 0;

	spin_lock_init(&pHandle->strCriticalSection);
	if( (ATL_SemaphoreCreate(&pHandle->hSem, &strSemAttrs) == ATL_SUCCESS))
	{
	
		pHandle->pstrMessageList = NULL;
		pHandle->u32ReceiversCount = 0;
		pHandle->bExiting = ATL_FALSE;

		return ATL_SUCCESS;
	}
	else
	{
		return ATL_FAIL;
	}
}

/*!
*  @author		syounan
*  @date		1 Sep 2010
*  @note		copied from FLO glue implementatuion
*  @version		1.0
*/
ATL_ErrNo ATL_MsgQueueDestroy(ATL_MsgQueueHandle* pHandle,
			tstrATL_MsgQueueAttrs* pstrAttrs)
{

	pHandle->bExiting = ATL_TRUE;

	// Release any waiting receiver thread.
	while(pHandle->u32ReceiversCount > 0)
	{
		ATL_SemaphoreRelease(&(pHandle->hSem), ATL_NULL);	
		pHandle->u32ReceiversCount--;
	}

	ATL_SemaphoreDestroy(&pHandle->hSem, ATL_NULL);
	
	
	while(pHandle->pstrMessageList != NULL)
	{
		Message * pstrMessge = pHandle->pstrMessageList->pstrNext;
		ATL_FREE(pHandle->pstrMessageList);
		pHandle->pstrMessageList = pstrMessge;	
	}

	return ATL_SUCCESS;
}

/*!
*  @author		syounan
*  @date		1 Sep 2010
*  @note		copied from FLO glue implementatuion
*  @version		1.0
*/
ATL_ErrNo ATL_MsgQueueSend(ATL_MsgQueueHandle* pHandle,
			const void * pvSendBuffer, ATL_Uint32 u32SendBufferSize,
			tstrATL_MsgQueueAttrs* pstrAttrs)
{
	ATL_ErrNo s32RetStatus = ATL_SUCCESS;
	unsigned long flags;
	Message * pstrMessage = NULL;
	
	if( (pHandle == NULL) || (u32SendBufferSize == 0) || (pvSendBuffer == NULL) )
	{
		ATL_ERRORREPORT(s32RetStatus, ATL_INVALID_ARGUMENT);
	}

	if(pHandle->bExiting == ATL_TRUE)
	{
		ATL_ERRORREPORT(s32RetStatus, ATL_FAIL);
	}

	spin_lock_irqsave(&pHandle->strCriticalSection,flags);
	
	
	/* construct a new message */
	pstrMessage = ATL_NEW(Message, 1);
	ATL_NULLCHECK(s32RetStatus, pstrMessage);
	pstrMessage->u32Length = u32SendBufferSize;
	pstrMessage->pstrNext = NULL;
	pstrMessage->pvBuffer = ATL_MALLOC(u32SendBufferSize);
	ATL_NULLCHECK(s32RetStatus, pstrMessage->pvBuffer);
	ATL_memcpy(pstrMessage->pvBuffer, pvSendBuffer, u32SendBufferSize);
	

	/* add it to the message queue */
	if(pHandle->pstrMessageList == NULL)
	{
		pHandle->pstrMessageList  = pstrMessage;
	}
	else
	{
		Message * pstrTailMsg = pHandle->pstrMessageList;
		while(pstrTailMsg->pstrNext != NULL)
		{
			pstrTailMsg = pstrTailMsg->pstrNext;
		}
		pstrTailMsg->pstrNext = pstrMessage;
	}	
	
	
	spin_unlock_irqrestore(&pHandle->strCriticalSection,flags);
	
	ATL_SemaphoreRelease(&pHandle->hSem, ATL_NULL);
	
	ATL_CATCH(s32RetStatus)
	{
		/* error occured, free any allocations */
		if(pstrMessage != NULL)
		{
			if(pstrMessage->pvBuffer != NULL)
			{
				ATL_FREE(pstrMessage->pvBuffer);
			}
			ATL_FREE(pstrMessage);
		}
	}

	return s32RetStatus; 
}



/*!
*  @author		syounan
*  @date		1 Sep 2010
*  @note		copied from FLO glue implementatuion
*  @version		1.0
*/
ATL_ErrNo ATL_MsgQueueRecv(ATL_MsgQueueHandle* pHandle, 
			void * pvRecvBuffer, ATL_Uint32 u32RecvBufferSize, 
			ATL_Uint32* pu32ReceivedLength,
			tstrATL_MsgQueueAttrs* pstrAttrs)
{

	Message * pstrMessage;
	ATL_ErrNo s32RetStatus = ATL_SUCCESS;
	tstrATL_SemaphoreAttrs strSemAttrs;
	unsigned long flags;
	if( (pHandle == NULL) || (u32RecvBufferSize == 0) 
		|| (pvRecvBuffer == NULL) || (pu32ReceivedLength == NULL) )
	{
		ATL_ERRORREPORT(s32RetStatus, ATL_INVALID_ARGUMENT);
	}

	if(pHandle->bExiting == ATL_TRUE)
	{
		ATL_ERRORREPORT(s32RetStatus, ATL_FAIL);
	}
	
	
	spin_lock_irqsave(&pHandle->strCriticalSection,flags);
	pHandle->u32ReceiversCount++;
	
	spin_unlock_irqrestore(&pHandle->strCriticalSection,flags);
	ATL_SemaphoreFillDefault(&strSemAttrs);
	#ifdef CONFIG_ATL_MSG_QUEUE_TIMEOUT
	if(pstrAttrs != ATL_NULL)
	{
		strSemAttrs.u32TimeOut = pstrAttrs->u32Timeout;
	}
	#endif
	s32RetStatus = ATL_SemaphoreAcquire(&(pHandle->hSem), &strSemAttrs);

	if(s32RetStatus == ATL_TIMEOUT)
	{
		// timed out, just exit without consumeing the message 
		
		spin_lock_irqsave(&pHandle->strCriticalSection,flags);
		pHandle->u32ReceiversCount--;
		spin_unlock_irqrestore(&pHandle->strCriticalSection,flags);
		
	}
	else
	{
		/* other non-timeout scenarios */
		ATL_ERRORCHECK(s32RetStatus);

		if(pHandle->bExiting)
		{
			ATL_ERRORREPORT(s32RetStatus, ATL_FAIL);
		}

	
		spin_lock_irqsave(&pHandle->strCriticalSection,flags);
		
		pstrMessage = pHandle->pstrMessageList;
		if(pstrMessage == NULL)
		{


		spin_unlock_irqrestore(&pHandle->strCriticalSection,flags);

			ATL_ERRORREPORT(s32RetStatus, ATL_FAIL);
		}

		/* check buffer size */
		if(u32RecvBufferSize < pstrMessage->u32Length)
		{	
			spin_unlock_irqrestore(&pHandle->strCriticalSection,flags);
			
		ATL_SemaphoreRelease(&pHandle->hSem, ATL_NULL);
			
			ATL_ERRORREPORT(s32RetStatus, ATL_BUFFER_OVERFLOW);
		}

		/* consume the message */
	
		pHandle->u32ReceiversCount--;
		ATL_memcpy(pvRecvBuffer, pstrMessage->pvBuffer, pstrMessage->u32Length);
		*pu32ReceivedLength = pstrMessage->u32Length;

		pHandle->pstrMessageList = pstrMessage->pstrNext;
		
		ATL_FREE(pstrMessage->pvBuffer);
		ATL_FREE(pstrMessage);	
		
		
		spin_unlock_irqrestore(&pHandle->strCriticalSection,flags);
	}

	ATL_CATCH(s32RetStatus)
	{
	}
	
	return s32RetStatus;
}

#endif

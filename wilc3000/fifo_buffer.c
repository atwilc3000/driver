

#include "atl_os_wrapper.h"
#include "fifo_buffer.h"



ATL_Uint32 FIFO_InitBuffer(tHANDLE * hBuffer,ATL_Uint32 u32BufferLength)
{
	ATL_Uint32 u32Error = 0;
	tstrFifoHandler * pstrFifoHandler = ATL_MALLOC(sizeof(tstrFifoHandler));
	if(pstrFifoHandler)
	{
		ATL_memset(pstrFifoHandler,0,sizeof(tstrFifoHandler));
		pstrFifoHandler->pu8Buffer = ATL_MALLOC(u32BufferLength);
		if(pstrFifoHandler->pu8Buffer)
		{
			tstrATL_SemaphoreAttrs strSemBufferAttrs;
			pstrFifoHandler->u32BufferLength = u32BufferLength;
			ATL_memset(pstrFifoHandler->pu8Buffer,0,u32BufferLength);
			//create semaphore
			ATL_SemaphoreFillDefault(&strSemBufferAttrs);	
			strSemBufferAttrs.u32InitCount = 1;
			ATL_SemaphoreCreate(&pstrFifoHandler->SemBuffer, &strSemBufferAttrs);						
			*hBuffer = pstrFifoHandler;
		}
		else
		{
			*hBuffer = NULL;
			u32Error = 1;
		}
	}
	else
	{
		u32Error = 1;
	}
	return u32Error;	
}
ATL_Uint32 FIFO_DeInit(tHANDLE hFifo)
{
	ATL_Uint32 u32Error = 0;
	tstrFifoHandler * pstrFifoHandler = (tstrFifoHandler*)hFifo;
	if(pstrFifoHandler)
	{
		if(pstrFifoHandler->pu8Buffer)
		{
			ATL_FREE(pstrFifoHandler->pu8Buffer);			
		}
		else
		{
			u32Error = 1;
		}

		ATL_SemaphoreDestroy(&pstrFifoHandler->SemBuffer, ATL_NULL);
		
		ATL_FREE(pstrFifoHandler);
	}
	else
	{
		u32Error = 1;
	}
	return u32Error;
}
ATL_Uint32 FIFO_ReadBytes(tHANDLE hFifo,ATL_Uint8 *pu8Buffer,ATL_Uint32 u32BytesToRead,ATL_Uint32 *pu32BytesRead)
{
	ATL_Uint32 u32Error = 0;
	tstrFifoHandler * pstrFifoHandler = (tstrFifoHandler*)hFifo;
	if(pstrFifoHandler && pu32BytesRead)
	{
		if(pstrFifoHandler->u32TotalBytes)
		{			
			if(ATL_SemaphoreAcquire(&pstrFifoHandler->SemBuffer, ATL_NULL) == ATL_SUCCESS)
			{
				if(u32BytesToRead > pstrFifoHandler->u32TotalBytes)
				{
					*pu32BytesRead = pstrFifoHandler->u32TotalBytes;
				}
				else
				{
					*pu32BytesRead = u32BytesToRead;
				}
				if((pstrFifoHandler->u32ReadOffset + u32BytesToRead) <= pstrFifoHandler->u32BufferLength)
				{
					ATL_memcpy(pu8Buffer,pstrFifoHandler->pu8Buffer + pstrFifoHandler->u32ReadOffset,
						*pu32BytesRead);
					//update read offset and total bytes
					pstrFifoHandler->u32ReadOffset += u32BytesToRead;
					pstrFifoHandler->u32TotalBytes -= u32BytesToRead;

				}
				else
				{
					ATL_Uint32 u32FirstPart =
						pstrFifoHandler->u32BufferLength - pstrFifoHandler->u32ReadOffset;
					ATL_memcpy(pu8Buffer,pstrFifoHandler->pu8Buffer + pstrFifoHandler->u32ReadOffset,
						u32FirstPart);
					ATL_memcpy(pu8Buffer + u32FirstPart,pstrFifoHandler->pu8Buffer,
						u32BytesToRead - u32FirstPart);
					//update read offset and total bytes
					pstrFifoHandler->u32ReadOffset = u32BytesToRead - u32FirstPart;
					pstrFifoHandler->u32TotalBytes -= u32BytesToRead;
				}				
				ATL_SemaphoreRelease(&pstrFifoHandler->SemBuffer, ATL_NULL);
			}
			else
			{
				u32Error = 1;
			}
		}
		else
		{
			u32Error = 1;
		}
	}
	else
	{
		u32Error = 1;
	}
	return u32Error;
}

ATL_Uint32 FIFO_WriteBytes(tHANDLE hFifo,ATL_Uint8 *pu8Buffer,ATL_Uint32 u32BytesToWrite,ATL_Bool bForceOverWrite)
{
	ATL_Uint32 u32Error = 0;
	tstrFifoHandler * pstrFifoHandler = (tstrFifoHandler*)hFifo;
	if(pstrFifoHandler)
	{
		if(u32BytesToWrite < pstrFifoHandler->u32BufferLength)
		{
			if((pstrFifoHandler->u32TotalBytes + u32BytesToWrite) <= pstrFifoHandler->u32BufferLength || 
				bForceOverWrite)
			{
				if(ATL_SemaphoreAcquire(&pstrFifoHandler->SemBuffer, ATL_NULL) == ATL_SUCCESS)
				{					
					if((pstrFifoHandler->u32WriteOffset + u32BytesToWrite) <= pstrFifoHandler->u32BufferLength)
					{
						ATL_memcpy(pstrFifoHandler->pu8Buffer + pstrFifoHandler->u32WriteOffset,pu8Buffer,
							u32BytesToWrite);
						//update read offset and total bytes
						pstrFifoHandler->u32WriteOffset += u32BytesToWrite;
						pstrFifoHandler->u32TotalBytes  += u32BytesToWrite;

					}
					else 					
					{
						ATL_Uint32 u32FirstPart =
							pstrFifoHandler->u32BufferLength - pstrFifoHandler->u32WriteOffset;
						ATL_memcpy(pstrFifoHandler->pu8Buffer + pstrFifoHandler->u32WriteOffset,pu8Buffer,
							u32FirstPart);
						ATL_memcpy(pstrFifoHandler->pu8Buffer,pu8Buffer + u32FirstPart,
							u32BytesToWrite - u32FirstPart);
						//update read offset and total bytes
						pstrFifoHandler->u32WriteOffset = u32BytesToWrite - u32FirstPart;
						pstrFifoHandler->u32TotalBytes += u32BytesToWrite;
					}
					//if data overwriten
					if(pstrFifoHandler->u32TotalBytes > pstrFifoHandler->u32BufferLength)
					{
						//adjust read offset to the oldest data available
						pstrFifoHandler->u32ReadOffset = pstrFifoHandler->u32WriteOffset;
						//data availabe is the buffer length
						pstrFifoHandler->u32TotalBytes = pstrFifoHandler->u32BufferLength;
					}
					ATL_SemaphoreRelease(&pstrFifoHandler->SemBuffer, ATL_NULL);
				}
			}
			else
			{
				u32Error = 1;
			}
		}
		else
		{
			u32Error = 1;
		}
	}
	else
	{
		u32Error = 1;
	}
	return u32Error;
}
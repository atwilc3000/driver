
#include "atl_os_wrapper.h"


#define tHANDLE	void *

typedef struct
{
	ATL_Uint8		*pu8Buffer;
	ATL_Uint32	u32BufferLength;
	ATL_Uint32	u32WriteOffset;
	ATL_Uint32	u32ReadOffset;
	ATL_Uint32	u32TotalBytes;	
	ATL_SemaphoreHandle	SemBuffer;
}tstrFifoHandler;


extern ATL_Uint32 FIFO_InitBuffer(tHANDLE * hBuffer,ATL_Uint32 u32BufferLength);
extern ATL_Uint32 FIFO_DeInit(tHANDLE hFifo);
extern ATL_Uint32 FIFO_ReadBytes(tHANDLE hFifo,ATL_Uint8 *pu8Buffer,ATL_Uint32 u32BytesToRead,
								 ATL_Uint32 *pu32BytesRead);
extern ATL_Uint32 FIFO_WriteBytes(tHANDLE hFifo,ATL_Uint8 *pu8Buffer,ATL_Uint32 u32BytesToWrite,
								  ATL_Bool bForceOverWrite);
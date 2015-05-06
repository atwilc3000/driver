#ifndef __ATL_MSG_QUEUE_H__
#define __ATL_MSG_QUEUE_H__

/*!  
*  @file	atl_msg_queue.h
*  @brief	Message Queue OS wrapper functionality
*  @author	syounan
*  @sa		ATWILC_OSWrapper.h top level OS wrapper file
*  @date	30 Aug 2010
*  @version	1.0
*/

#ifndef CONFIG_ATL_MSG_QUEUE_FEATURE
#error the feature CONFIG_ATL_MSG_QUEUE_FEATURE must be supported to include this file
#endif

/*!
*  @struct 		tstrATL_MsgQueueAttrs
*  @brief		Message Queue API options 
*  @author		syounan
*  @date		30 Aug 2010
*  @version		1.0
*/
typedef struct
{
	#ifdef CONFIG_ATL_MSG_QUEUE_IPC_NAME
	ATL_Char* pcName;
	#endif

	#ifdef CONFIG_ATL_MSG_QUEUE_TIMEOUT
	ATL_Uint32 u32Timeout;
	#endif
	
	/* a dummy member to avoid compiler errors*/
	ATL_Uint8 dummy;
	
}tstrATL_MsgQueueAttrs;

/*!
*  @brief		Fills the MsgQueueAttrs with default parameters
*  @param[out]	pstrAttrs structure to be filled
*  @sa			ATL_TimerAttrs
*  @author		syounan
*  @date		30 Aug 2010
*  @version		1.0
*/
static void ATL_MsgQueueFillDefault(tstrATL_MsgQueueAttrs* pstrAttrs)
{
	#ifdef CONFIG_ATL_MSG_QUEUE_IPC_NAME
	pstrAttrs->pcName = ATL_NULL;
	#endif

	#ifdef CONFIG_ATL_MSG_QUEUE_TIMEOUT
	pstrAttrs->u32Timeout = ATL_OS_INFINITY;
	#endif
}
/*!
*  @brief		Creates a new Message queue
*  @details		Creates a new Message queue, if the feature
				CONFIG_ATL_MSG_QUEUE_IPC_NAME is enabled and pstrAttrs->pcName
				is not Null, then this message queue can be used for IPC with
				any other message queue having the same name in the system
*  @param[in,out]	pHandle handle to the message queue object
*  @param[in]	pstrAttrs Optional attributes, NULL for default
*  @return		Error code indicating sucess/failure
*  @sa			tstrATL_MsgQueueAttrs
*  @author		syounan
*  @date		30 Aug 2010
*  @version		1.0
*/
ATL_ErrNo ATL_MsgQueueCreate(ATL_MsgQueueHandle* pHandle,
			tstrATL_MsgQueueAttrs* pstrAttrs);


/*!
*  @brief		Sends a message
*  @details		Sends a message, this API will block unil the message is 
				actually sent or until it is timedout (as long as the feature
				CONFIG_ATL_MSG_QUEUE_TIMEOUT is enabled and pstrAttrs->u32Timeout
				is not set to ATL_OS_INFINITY), zero timeout is a valid value
*  @param[in]	pHandle handle to the message queue object
*  @param[in]	pvSendBuffer pointer to the data to send
*  @param[in]	u32SendBufferSize the size of the data to send
*  @param[in]	pstrAttrs Optional attributes, NULL for default
*  @return		Error code indicating sucess/failure
*  @sa			tstrATL_MsgQueueAttrs
*  @author		syounan
*  @date		30 Aug 2010
*  @version		1.0
*/
ATL_ErrNo ATL_MsgQueueSend(ATL_MsgQueueHandle* pHandle,
			const void * pvSendBuffer, ATL_Uint32 u32SendBufferSize,
			tstrATL_MsgQueueAttrs* pstrAttrs);


/*!
*  @brief		Receives a message
*  @details		Receives a message, this API will block unil a message is 
				received or until it is timedout (as long as the feature
				CONFIG_ATL_MSG_QUEUE_TIMEOUT is enabled and pstrAttrs->u32Timeout
				is not set to ATL_OS_INFINITY), zero timeout is a valid value
*  @param[in]	pHandle handle to the message queue object
*  @param[out]	pvRecvBuffer pointer to a buffer to fill with the received message
*  @param[in]	u32RecvBufferSize the size of the receive buffer
*  @param[out]	pu32ReceivedLength the length of received data
*  @param[in]	pstrAttrs Optional attributes, NULL for default
*  @return		Error code indicating sucess/failure
*  @sa			tstrATL_MsgQueueAttrs
*  @author		syounan
*  @date		30 Aug 2010
*  @version		1.0
*/
ATL_ErrNo ATL_MsgQueueRecv(ATL_MsgQueueHandle* pHandle, 
			void * pvRecvBuffer, ATL_Uint32 u32RecvBufferSize, 
			ATL_Uint32* pu32ReceivedLength,
			tstrATL_MsgQueueAttrs* pstrAttrs);


/*!
*  @brief		Destroys an existing  Message queue
*  @param[in]	pHandle handle to the message queue object
*  @param[in]	pstrAttrs Optional attributes, NULL for default
*  @return		Error code indicating sucess/failure
*  @sa			tstrATL_MsgQueueAttrs
*  @author		syounan
*  @date		30 Aug 2010
*  @version		1.0
*/
ATL_ErrNo ATL_MsgQueueDestroy(ATL_MsgQueueHandle* pHandle,
			tstrATL_MsgQueueAttrs* pstrAttrs);



#endif

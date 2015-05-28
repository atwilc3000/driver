#ifndef __ATL_ERRORSUPPORT_H__
#define __ATL_ERRORSUPPORT_H__

/*!  
*  @file		atl_error_support.h
*  @brief		Error reporting and handling support
*  @author		syounan
*  @sa			ATWILC_OSWrapper.h top level OS wrapper file
*  @date		10 Aug 2010
*  @version		1.0
*/

/* Psitive Numbers to indicate sucess with special status */
#define	ATL_ALREADY_EXSIT	+100	/** The requested object already exists */

/* Generic success will return 0 */
#define ATL_SUCCESS 		0	/** Generic success */

/* Negative numbers to indicate failures */
#define	ATL_FAIL                -100	/** Generic Fail */		
#define	ATL_BUSY                -101	/** Busy with another operation*/
#define	ATL_INVALID_ARGUMENT    -102	/** A given argument is invalid*/
#define	ATL_INVALID_STATE      	-103	/** An API request would violate the Driver state machine (i.e. to start PID while not camped)*/
#define	ATL_BUFFER_OVERFLOW     -104	/** In copy operations if the copied data is larger than the allocated buffer*/
#define ATL_NULL_PTR		-105	/** null pointer is passed or used */
#define	ATL_EMPTY               -107
#define ATL_FULL				-108
#define	ATL_TIMEOUT            	-109
#define ATL_CANCELED		-110	/** The required operation have been canceled by the user*/
#define ATL_INVALID_FILE	-112	/** The Loaded file is corruped or having an invalid format */
#define ATL_NOT_FOUND		-113	/** Cant find the file to load */
#define ATL_NO_MEM 		-114
#define ATL_UNSUPPORTED_VERSION -115
#define ATL_FILE_EOF			-116


/* Error type */
typedef ATL_Sint32 ATL_ErrNo;

#define ATL_IS_ERR(__status__) (__status__ < ATL_SUCCESS)

#define ATL_ERRORCHECK(__status__) do{\
	if(ATL_IS_ERR(__status__))\
	{\
		ATL_ERROR("ATL_ERRORCHECK(%d)\n", __status__);\
		goto ERRORHANDLER;\
	}\
}while(0)
	
#define ATL_ERRORREPORT(__status__, __err__) do{\
	ATL_ERROR("ATL_ERRORREPORT(%d)\n", __err__);\
	__status__ = __err__;\
	goto ERRORHANDLER;\
}while(0)

#define  ATL_NULLCHECK(__status__, __ptr__)	do{\
	if(__ptr__ == ATL_NULL)\
	{\
		ATL_ERRORREPORT(__status__, ATL_NULL_PTR);\
	}\
}while(0)

#define ATL_CATCH(__status__) \
ERRORHANDLER :\
if(ATL_IS_ERR(__status__)) \

#ifdef CONFIG_ATL_ASSERTION_SUPPORT

/**
 * @brief	prints a diagnostic message and aborts the program
 * @param[in] 	pcExpression The expression that triggered the assertion
 * @param[in] 	pcFileName The name of the current source file.
 * @param[in] 	u32LineNumber The line number in the current source file.
 * @warning DO NOT CALL DIRECTLY. USE EQUIVALENT MACRO FUNCTION INSTEAD.
 */
void ATL_assert_INTERNAL(ATL_Char* pcExpression, ATL_Char* pcFileName, ATL_Uint32 u32LineNumber);

#define ATL_assert(__expression__) (void)(!!(__expression__) || (ATL_assert_INTERNAL((#__expression__), __ATL_FILE__, __ATL_LINE__), 0))

#else
#define ATL_assert(__expression__) ((void)0)
#endif

#endif

#ifndef __ATL_TIME_H__
#define __ATL_TIME_H__

/*!  
*  @file		atl_time.h
*  @brief		Time retrival functionality
*  @author		syounan
*  @sa			ATWILC_OSWrapper.h top level OS wrapper file
*  @date		2 Sep 2010
*  @version		1.0
*/

#ifndef CONFIG_ATL_TIME_FEATURE
#error the feature CONFIG_ATL_TIME_FEATURE must be supported to include this file
#endif

/*!
*  @struct 		ATL_ThreadAttrs
*  @brief		Thread API options 
*  @author		syounan
*  @date		2 Sep 2010
*  @version		1.0
*/
typedef struct
{
	/* a dummy type to prevent compile errors on empty structure*/
	ATL_Uint8 dummy;
}tstrATL_TimeAttrs;

typedef struct
{
	/*!< current year */
	ATL_Uint16	u16Year;
	/*!< current month */
	ATL_Uint8	u8Month;
	/*!< current day */
	ATL_Uint8	u8Day;
	
	/*!< current hour (in 24H format) */
	ATL_Uint8	u8Hour;
	/*!< current minute */
	ATL_Uint8	u8Miute;
	/*!< current second */
	ATL_Uint8	u8Second;
	
}tstrATL_TimeCalender;

/*!
*  @brief		returns the number of msec elapsed since system start up
*  @return		number of msec elapsed singe system start up
*  @note		since this returned value is 32 bit, the caller must handle 
				wraparounds in values every about 49 of continous operations
*  @author		syounan
*  @date		2 Sep 2010
*  @version		1.0
*/
ATL_Uint32 ATL_TimeMsec(void);



#ifdef CONFIG_ATL_EXTENDED_TIME_OPERATIONS
/**
*  @brief 		 
*  @details 	function returns the implementation's best approximation to the
				processor time used by the process since the beginning of an
				implementation-dependent time related only to the process invocation.
*  @return 		ATL_Uint32
*  @note 		
*  @author		remil
*  @date		11 Nov 2010
*  @version		1.0
*/
ATL_Uint32 ATL_Clock();

/**
*  @brief 		 
*  @details 	The difftime() function computes the difference between two calendar
				times (as returned by ATL_GetTime()): time1 - time0.
*  @param[in] 	ATL_Time time1
*  @param[in] 	ATL_Time time0
*  @return 		ATL_Double
*  @note 		
*  @author		remil
*  @date		11 Nov 2010
*  @version		1.0
*/
ATL_Double ATL_DiffTime(ATL_Time time1, ATL_Time time0);

/**
*  @brief 		 
*  @details 	The gmtime() function converts the time in seconds since
				the Epoch pointed to by timer into a broken-down time,
				expressed as Coordinated Universal Time (UTC).
*  @param[in] 	const ATL_Time* timer
*  @return 		ATL_tm*
*  @note 		
*  @author		remil
*  @date		11 Nov 2010
*  @version		1.0
*/
ATL_tm* ATL_GmTime(const ATL_Time* timer);


/**
*  @brief 		 
*  @details 	The localtime() function converts the time in seconds since
				the Epoch pointed to by timer into a broken-down time, expressed
				as a local time. The function corrects for the timezone and any 
				seasonal time adjustments. Local timezone information is used as 
				though localtime() calls tzset().
*  @param[in] 	const ATL_Time* timer
*  @return 		ATL_tm*
*  @note 		
*  @author		remil
*  @date		11 Nov 2010
*  @version		1.0
*/
ATL_tm* ATL_LocalTime(const ATL_Time* timer);


/**
*  @brief 		 
*  @details 	The mktime() function converts the broken-down time,
				expressed as local time, in the structure pointed to by timeptr, 
				into a time since the Epoch value with the same encoding as that 
				of the values returned by time(). The original values of the tm_wday
				and tm_yday components of the structure are ignored, and the original 
				values of the other components are not restricted to the ranges described 
				in the <time.h> entry.
*  @param[in] 	ATL_tm* timer
*  @return 		ATL_Time
*  @note 		
*  @author		remil
*  @date		11 Nov 2010
*  @version		1.0
*/
ATL_Time ATL_MkTime(ATL_tm* timer);


/**
*  @brief 		 
*  @details 	The strftime() function places bytes into the array 
				pointed to by s as controlled by the string pointed to by format.
*  @param[in] 	ATL_Char* s
*  @param[in]	ATL_Uint32 maxSize
*  @param[in]	const ATL_Char* format
*  @param[in]	const ATL_tm* timptr
*  @return 		ATL_Uint32
*  @note 		
*  @author		remil
*  @date		11 Nov 2010
*  @version		1.0
*/
ATL_Uint32 ATL_StringFormatTime(ATL_Char* s,
								ATL_Uint32 maxSize,
								const ATL_Char* format,
								const ATL_tm* timptr);


/**
*  @brief 		The ATL_GetTime() function returns the value of time in seconds since the Epoch.
*  @details 	The tloc argument points to an area where the return value is also stored.
				If tloc is a null pointer, no value is stored.
*  @param[in] 	ATL_Time* tloc
*  @return 		ATL_Time
*  @note 		
*  @author		remil
*  @date		11 Nov 2010
*  @version		1.0
*/
ATL_Time ATL_GetTime(ATL_Time* tloc);

#endif

#ifdef CONFIG_ATL_TIME_UTC_SINCE_1970

/*!
*  @brief		returns the number of seconds elapsed since 1970 (in UTC)
*  @param[in]	pstrAttrs Optional attributes, NULL for default
*  @return		number of seconds elapsed since 1970 (in UTC)
*  @sa			tstrATL_TimeAttrs
*  @author		syounan
*  @date		2 Sep 2010
*  @version		1.0
*/
ATL_Uint32 ATL_TimeUtcSince1970(tstrATL_TimeAttrs* pstrAttrs);

#endif

#ifdef CONFIG_ATL_TIME_CALENDER

/*!
*  @brief		gets the current calender time
*  @return		number of seconds elapsed since 1970 (in UTC)
*  @param[out]	ptstrCalender calender structure to be filled with time
*  @param[in]	pstrAttrs Optional attributes, NULL for default
*  @sa			ATL_ThreadAttrs
*  @author		syounan
*  @date		2 Sep 2010
*  @version		1.0
*/
ATL_ErrNo ATL_TimeCalender(tstrATL_TimeCalender* ptstrCalender, 
	tstrATL_TimeAttrs* pstrAttrs);

#endif

#endif

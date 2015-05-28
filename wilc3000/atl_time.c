
#define _CRT_SECURE_NO_DEPRECATE
#include "atl_os_wrapper.h"

#ifdef CONFIG_ATL_TIME_FEATURE


ATL_Uint32 ATL_TimeMsec(void)
{
	ATL_Uint32 u32Time = 0;
	struct timespec current_time;

	current_time = current_kernel_time();
	u32Time = current_time.tv_sec * 1000;
	u32Time += current_time.tv_nsec / 1000000;
	
	
	return u32Time;
}


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
ATL_Uint32 ATL_Clock()
{
	/*printk("Not implemented")*/
}


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
ATL_Double ATL_DiffTime(ATL_Time time1, ATL_Time time0)
{
	/*printk("Not implemented")*/
}



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
ATL_tm* ATL_GmTime(const ATL_Time* timer)
{
	/*printk("Not implemented")*/
}


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
ATL_tm* ATL_LocalTime(const ATL_Time* timer)
{
	/*printk("Not implemented")*/
}


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
ATL_Time ATL_MkTime(ATL_tm* timer)
{
	/*printk("Not implemented")*/
}


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
								const ATL_tm* timptr)
{
	/*printk("Not implemented")*/
}


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
ATL_Time ATL_GetTime(ATL_Time* tloc)
{
	/*printk("Not implemented")*/
}


#endif
#endif



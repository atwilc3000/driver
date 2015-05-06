#ifndef __ATL_SLEEP_H__
#define __ATL_SLEEP_H__

/*!  
*  @file		atl_sleep.h
*  @brief		Sleep OS Wrapper functionality
*  @author		syounan
*  @sa			ATWILC_OSWrapper.h top level OS wrapper file
*  @date		10 Aug 2010
*  @version		1.0
*/

#ifndef CONFIG_ATL_SLEEP_FEATURE
#error the feature ATL_OS_FEATURE_SLEEP must be supported to include this file
#endif

/*!
*  @brief	forces the current thread to sleep until the given time has elapsed
*  @param[in]	u32TimeMilliSec Time to sleep in Milli seconds
*  @sa		ATL_SleepMicrosec
*  @author	syounan
*  @date	10 Aug 2010
*  @version	1.0
*  @note	This function offers a relatively innacurate and low resolution 
		sleep, for accurate high resolution sleep use u32TimeMicoSec
*/
void ATL_Sleep(ATL_Uint32 u32TimeMilliSec);

#ifdef CONFIG_ATL_SLEEP_HI_RES
/*!
*  @brief	forces the current thread to sleep until the given time has elapsed
*  @param[in]	u32TimeMicoSec Time to sleep in Micro seconds
*  @sa		ATL_Sleep
*  @author	syounan
*  @date	10 Aug 2010
*  @version	1.0
*  @note	This function offers an acurare high resolution sleep, depends on
		the feature ATL_OS_FEATURE_SLEEP_HI_RES and may not be supported
		on all Operating Systems
*/
void ATL_SleepMicrosec(ATL_Uint32 u32TimeMicoSec);
#endif


#endif

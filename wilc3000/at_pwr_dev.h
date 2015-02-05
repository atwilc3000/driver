/*!  
*  @file	at_pwr_dev.h
*  @brief	Operations to create a device to control the chip's power for Bluetooth
*  @author	Adham.Abozaeid
*  @date	4 Dec 2014
*  @version	1.0
*/

#ifndef AT_PWR_DEV_H
#define AT_PWR_DEV_H

#define PWR_DEV_SRC_WIFI 0
#define PWR_DEV_SRC_BT 1
#define PWR_DEV_SRC_MAX 2

#include <linux/mutex.h>
#include "wilc_wlan_if.h"

/*!
*	@brief		Initialize bluetooth power device
*	@details		
*	@return		0 for success
*				negative value other wise
*	@author		Adham Abozaeid
*	@date		4 Dec 2014
*	@version	1.0
*/
int at_pwr_dev_init(void);

/*!
*	@brief		Deinitialize bluetooth power device
*	@details		
*	@return		None
*	@author		Adham Abozaeid
*	@date		4 Dec 2014
*	@version	1.0
*/
int at_pwr_dev_deinit(void);

/*!
*	@brief		Register bus
*	@details		
*	@return		Negative value if error, 0 otherwise
*	@author		Adham Abozaeid
*	@date		4 Dec 2014
*	@version	1.0
*/
int at_pwr_register_bus(int source);

/*!
*	@brief		Unregister bus
*	@details		
*	@return		Negative value if error, 0 otherwise
*	@author		Adham Abozaeid
*	@date		4 Dec 2014
*	@version	1.0
*/
int at_pwr_unregister_bus(int source);

/*!
*	@brief		Power the chip up
*	@details		
*	@return		Negative value if error, 0 otherwise
*	@author		Adham Abozaeid
*	@date		4 Dec 2014
*	@version	1.0
*/
int at_pwr_power_up(int source);

/*!
*	@brief		Power the chip down
*	@details		
*	@return		Negative value if error, 0 otherwise
*	@author		Adham Abozaeid
*	@date		4 Dec 2014
*	@version	1.0
*/
int at_pwr_power_down(int source);

/*!
*	@brief		Get bus protection mutes
*	@details	All modules that needs to access the bus should
*				protect the bus access using the returned mutex
*	@return		Bus protection mutex
*	@author		Adham Abozaeid
*	@date		4 Dec 2014
*	@version	1.0
*/

void chip_allow_sleep(void);
void chip_wakeup(void);

void acquire_bus(BUS_ACQUIRE_T acquire);
void release_bus(BUS_RELEASE_T release, int source);

struct mutex * at_pwr_dev_get_bus_lock(void);

#endif //AT_PWR_DEV_H
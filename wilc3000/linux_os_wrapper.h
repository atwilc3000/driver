/*!  
*  @file	host_interface.h
*  @brief	File containg linux os APIs
*  @author	Adham Abozaeid
*  @sa		
*  @date	25 December 2014
*  @version	1.0
*/

#ifndef LINUX_OS_WRAPPER_H
#define LINUX_OS_WRAPPER_H

#include "wilc_type.h"

void linux_wlan_msleep(uint32_t msc);

void linux_wlan_atomic_msleep(uint32_t msc);

void linux_wlan_dbg(uint8_t *buff);

void* linux_wlan_malloc_atomic(uint32_t sz);

void* linux_wlan_malloc(uint32_t sz);

void linux_wlan_free(void* vp);

void* internal_alloc(uint32_t size, uint32_t flag);

void linux_wlan_init_lock(char* lockName, void* plock,int count);

void linux_wlan_deinit_lock(void* plock);

void linux_wlan_lock(void* vp);

int linux_wlan_lock_timeout(void* vp,uint32_t timeout);

void linux_wlan_unlock(void* vp);

void linux_wlan_init_mutex(char* lockName, void* plock,int count);

void linux_wlan_deinit_mutex(void* plock);

void linux_wlan_lock_mutex(void* vp);

void linux_wlan_unlock_mutex(void* vp);

void linux_wlan_init_spin_lock(char* lockName, void* plock,int count);

void linux_wlan_deinit_spin_lock(void* plock);

void linux_wlan_spin_lock(void* vp, unsigned long *flags);

void linux_wlan_spin_unlock(void* vp, unsigned long *flags);


#endif //LINUX_OS_WRAPPER_H

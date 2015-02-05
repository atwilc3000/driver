
#include "linux_os_wrapper.h"

#include "linux_wlan_common.h"

#include <linux/semaphore.h>

/*
	OS functions
*/
void linux_wlan_msleep(uint32_t msc){
	if(msc <= 4000000)
	{
		WILC_Uint32 u32Temp = msc * 1000;
#if LINUX_VERSION_CODE > KERNEL_VERSION(2,6,35)
		usleep_range(u32Temp, u32Temp);
#else
	/* This is delay not sleep !!!, has to be changed*/
		msleep(msc);
#endif
	}
	else
	{
    		msleep(msc);
	}		
}

void linux_wlan_atomic_msleep(uint32_t msc){
	mdelay(msc);	
}
void linux_wlan_dbg(uint8_t *buff){
	printk(buff);
#if defined (WILC_DEBUGFS)
	kmsgdump_write(buff);
#endif
}

void* linux_wlan_malloc_atomic(uint32_t sz){
	char* pntr = NULL;
	pntr = (char*)kmalloc(sz,GFP_ATOMIC );	
	PRINT_D(MEM_DBG,"Allocating %d bytes at address %p\n",sz,pntr);
 	return ((void*)pntr);

}
void* linux_wlan_malloc(uint32_t sz){
	char* pntr = NULL;
	pntr = (char*)kmalloc(sz,GFP_KERNEL );
	PRINT_D(MEM_DBG,"Allocating %d bytes at address %p\n",sz,pntr);
 	return ((void*)pntr);
}

void linux_wlan_free(void* vp){
	if(vp != NULL){
			PRINT_D(MEM_DBG,"Freeing %p\n",vp);
			kfree(vp);
		}
}


void* internal_alloc(uint32_t size, uint32_t flag){
	char* pntr = NULL;
	pntr = (char*)kmalloc(size,flag);
	PRINT_D(MEM_DBG,"Allocating %d bytes at address %p\n",size,pntr);
 	return ((void*)pntr);
}


void linux_wlan_init_lock(char* lockName, void* plock,int count)
{
	sema_init((struct semaphore*)plock,count);
	PRINT_D(LOCK_DBG,"Initializing [%s][%p]\n",lockName,plock);

}

void linux_wlan_deinit_lock(void* plock)
{
	//mutex_destroy((struct mutex*)plock);
}

void linux_wlan_lock(void* vp)
{
	PRINT_D(LOCK_DBG,"Locking %p\n",vp);
	if(vp != NULL)
	{		
		while(down_interruptible((struct semaphore*)vp)); 
	}
	else
	{
		PRINT_ER("Failed, mutex is NULL\n");
	}
}

int linux_wlan_lock_timeout(void* vp,uint32_t timeout)
{
	int error = -1;
	PRINT_D(LOCK_DBG,"Locking %p\n",vp);
	if(vp != NULL)
	{		
		error = down_timeout((struct semaphore*)vp, msecs_to_jiffies(timeout));
	}
	else
	{
		PRINT_ER("Failed, mutex is NULL\n");
	}
	return error;
}

void linux_wlan_unlock(void* vp)
{
	PRINT_D(LOCK_DBG,"Unlocking %p\n",vp);
	if(vp != NULL)
	{
		up((struct semaphore*)vp);			
	}
	else
	{
		PRINT_ER("Failed, mutex is NULL\n");
	}
}


void linux_wlan_init_mutex(char* lockName, void* plock,int count)
{
	mutex_init((struct mutex*)plock);
	PRINT_D(LOCK_DBG,"Initializing mutex [%s][%p]\n",lockName,plock);

}

void linux_wlan_deinit_mutex(void* plock)
{
	mutex_destroy((struct mutex*)plock);
}

void linux_wlan_lock_mutex(void* vp)
{
	PRINT_D(LOCK_DBG,"Locking mutex %p\n",vp);
	if(vp != NULL)
	{
	/*
		if(mutex_is_locked((struct mutex*)vp))
		{
			//PRINT_ER("Mutex already locked - %p \n",vp);
		}
*/
		mutex_lock((struct mutex*)vp);		

	}else
	{
		PRINT_ER("Failed, mutex is NULL\n");
	}
}

void linux_wlan_unlock_mutex(void* vp){
	PRINT_D(LOCK_DBG,"Unlocking mutex %p\n",vp);
	if(vp != NULL){

		if(mutex_is_locked((struct mutex*)vp)){
			mutex_unlock((struct mutex*)vp);			
			}else{
					//PRINT_ER("Mutex already unlocked  - %p\n",vp);
			}
			
		}else{
		PRINT_ER("Failed, mutex is NULL\n");
		}
}


/*Added by Amr - BugID_4720*/
void linux_wlan_init_spin_lock(char* lockName, void* plock,int count)
{
	spin_lock_init((spinlock_t*)plock);
	PRINT_D(SPIN_DEBUG,"Initializing mutex [%s][%p]\n",lockName,plock);

}

void linux_wlan_deinit_spin_lock(void* plock)
{
	
}
void linux_wlan_spin_lock(void* vp, unsigned long *flags){
	unsigned long lflags;
	PRINT_D(SPIN_DEBUG,"Lock spin %p\n",vp);
	if(vp != NULL){
		spin_lock_irqsave((spinlock_t*)vp, lflags);
		*flags = lflags;
	}
	else{
		PRINT_ER("Failed, spin lock is NULL\n");
	}
}

void linux_wlan_spin_unlock(void* vp, unsigned long *flags){
	unsigned long lflags= *flags;
	PRINT_D(SPIN_DEBUG,"Unlock spin %p\n",vp);
	if(vp != NULL){
		spin_unlock_irqrestore((spinlock_t*)vp, lflags);
		*flags = lflags;
	}
	else{
		PRINT_ER("Failed, spin lock is NULL\n");
	}
}

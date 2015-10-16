
#include <linux/mmc/sdio_func.h>
#include <linux/mmc/card.h>
#include <linux/mmc/sdio_ids.h>
#include <linux/mmc/sdio.h>
#include <linux/mmc/host.h>

#include "linux_wlan_sdio.h"
#include "linux_wlan_common.h"
#include "at_pwr_dev.h"


#if defined (NM73131_0_BOARD)
#define SDIO_MODALIAS "atwilc_sdio"
#else
#define SDIO_MODALIAS "atwilc_sdio"
#endif


struct sdio_func* local_sdio_func = NULL;

static isr_handler_t isr_handler = NULL;

extern struct semaphore sdio_probe_sync;
extern void linux_wlan_unlock(void* vp);

extern int atwilc_netdev_init(void);
extern int sdio_clear_int(void);


#define SDIO_VENDOR_ID_ATWILC 0x0296
#define SDIO_DEVICE_ID_ATWILC 0x5347

static const struct sdio_device_id atwilc_sdio_ids[] = {
	{ SDIO_DEVICE(SDIO_VENDOR_ID_ATWILC,SDIO_DEVICE_ID_ATWILC) },
};

#ifndef ATWILC_SDIO_IRQ_GPIO
// [[ added by rachel 13-07-18
typedef enum {
	ATWILC_SDIO_HOST_NO_TAKEN = 0,
	ATWILC_SDIO_HOST_IRQ_TAKEN = 1,
	ATWILC_SDIO_HOST_DIS_TAKEN = 2,
} sdio_host_lock;
static sdio_host_lock	sdio_intr_lock = ATWILC_SDIO_HOST_NO_TAKEN;
static wait_queue_head_t sdio_intr_waitqueue;
// ]]
#endif

static void atwilc_sdio_interrupt(struct sdio_func *func)
{
#ifndef ATWILC_SDIO_IRQ_GPIO
	if(sdio_intr_lock == ATWILC_SDIO_HOST_DIS_TAKEN)
		return;
	sdio_intr_lock = ATWILC_SDIO_HOST_IRQ_TAKEN;
	
	sdio_release_host(func);
	if(isr_handler != NULL)
		isr_handler();
	sdio_claim_host(func);

	sdio_intr_lock = ATWILC_SDIO_HOST_NO_TAKEN;
	wake_up_interruptible(&sdio_intr_waitqueue);
#endif
}


int linux_sdio_cmd52(sdio_cmd52_t *cmd){
	struct sdio_func *func = local_sdio_func;
	int ret;
	u8 data;

	sdio_claim_host(func);

	func->num = cmd->function;
	if (cmd->read_write) {	/* write */
		if (cmd->raw) {
			sdio_writeb(func, cmd->data, cmd->address, &ret);
			data = sdio_readb(func, cmd->address, &ret);
			cmd->data = data;
		} else {
			sdio_writeb(func, cmd->data, cmd->address, &ret);
		}
	} else {	/* read */
		data = sdio_readb(func, cmd->address, &ret);
		cmd->data = data;
	}

	sdio_release_host(func);

	if (ret < 0) {
		printk("atwilc_sdio_cmd52..failed, err(%d)\n", ret);
		return 0;
	}
	return 1;
}


 int linux_sdio_cmd53(sdio_cmd53_t *cmd){
	struct sdio_func *func = local_sdio_func;
	int size, ret;

	sdio_claim_host(func);

	func->num = cmd->function;
	func->cur_blksize = cmd->block_size;
	if (cmd->block_mode)
		size = cmd->count * cmd->block_size;
	else
		size = cmd->count;

	if (cmd->read_write) {	/* write */
		ret = sdio_memcpy_toio(func, cmd->address, (void *)cmd->buffer, size);		
	} else {	/* read */
		ret = sdio_memcpy_fromio(func, (void *)cmd->buffer, cmd->address,  size);		
	}

	sdio_release_host(func);


	if (ret < 0) {
		printk("atwilc_sdio_cmd53..failed, err(%d)\n", ret);
		return 0;
	}

	return 1;
}

volatile int probe = 0;
static int linux_sdio_probe(struct sdio_func *func, const struct sdio_device_id *id){	

	printk("probe function\n");
	
	if(local_sdio_func != NULL)
	{		
		local_sdio_func = func;
		probe = 1;

		linux_wlan_unlock(&sdio_probe_sync);	
		
		return 0;
	}
	local_sdio_func = func;

	linux_wlan_unlock(&sdio_probe_sync);
	
	return 0;
}

static void linux_sdio_remove(struct sdio_func *func)
{
	/**
		TODO
	**/
	
}

int sdio_init(atwilc_wlan_inp_t *inp, atwilc_debug_func func);
int sdio_deinit(void *pv);
void chip_wakeup(int source);
void chip_allow_sleep(int source);
extern void (*pf_chip_sleep_manually)(ATL_Uint32 , int );
extern int (*pf_get_num_conn_ifcs)(void);
extern void (*pf_host_wakeup_notify)(int);
extern void (*pf_host_sleep_notify)(int);
extern int (*pf_get_u8SuspendOnEvent_value)(void);
static int atwilc_sdio_suspend(struct device *dev)
{
	printk("\n\n << SUSPEND >>\n\n");
	chip_wakeup(0);
	/*if there is no events , put the chip in low power mode */
	if(pf_get_u8SuspendOnEvent_value()== 0)
		
	{
		/*BugID_5213*/
		/*Allow chip sleep, only if both interfaces are not connected*/
		if(!pf_get_num_conn_ifcs())
		{
			pf_chip_sleep_manually(0xFFFFFFFF,0);
		}
	}
	else
	{
		/*notify the chip that host will sleep*/
		pf_host_sleep_notify(0);
		chip_allow_sleep(0);
	}
	/*reset SDIO to allow kerenl reintilaization at wake up*/
	sdio_deinit(NULL);
	/*claim the host to prevent driver SDIO access before resume is called*/
	sdio_claim_host(local_sdio_func);
	return 0 ;
}
static int atwilc_sdio_resume(struct device *dev)
{
	sdio_release_host(local_sdio_func);
	/*wake the chip to compelete the re-intialization*/
	chip_wakeup(0);
	printk("\n\n  <<RESUME>> \n\n");	
	/*Init SDIO block mode*/
	sdio_init(NULL,NULL);
	/*if there is an event , notify the chip that the host is awake now*/
	if(pf_get_u8SuspendOnEvent_value()== 1)
		pf_host_wakeup_notify(0);

	chip_allow_sleep(0);
    return 0;

}

static const struct dev_pm_ops atwilc_sdio_pm_ops = {	
     .suspend = atwilc_sdio_suspend,    
     .resume    = atwilc_sdio_resume,
    	};

struct sdio_driver atwilc_bus = {
	.name		= SDIO_MODALIAS,
	.id_table	= atwilc_sdio_ids,
	.probe		= linux_sdio_probe,
	.remove		= linux_sdio_remove,

    .drv      = {
                  .pm = &atwilc_sdio_pm_ops,
               }
};

int enable_sdio_interrupt(isr_handler_t p_isr_handler){
	int ret = 0;
#ifndef ATWILC_SDIO_IRQ_GPIO
	
	sdio_intr_lock  = ATWILC_SDIO_HOST_NO_TAKEN;

	isr_handler = p_isr_handler;
	
	sdio_claim_host(local_sdio_func);
	ret = sdio_claim_irq(local_sdio_func, atwilc_sdio_interrupt);
	sdio_release_host(local_sdio_func);

	if (ret < 0) {
		PRINT_ER("can't claim sdio_irq, err(%d)\n", ret);
		ret = -EIO;
	}
	
#endif
	return ret;
}

void disable_sdio_interrupt(void){

#ifndef ATWILC_SDIO_IRQ_GPIO
	int ret;

	if(sdio_intr_lock  == ATWILC_SDIO_HOST_IRQ_TAKEN ) 
		wait_event_interruptible(sdio_intr_waitqueue, sdio_intr_lock == ATWILC_SDIO_HOST_NO_TAKEN );
	sdio_intr_lock  = ATWILC_SDIO_HOST_DIS_TAKEN;

	sdio_claim_host(local_sdio_func);
	ret = sdio_release_irq(local_sdio_func);
	if (ret < 0) {
			PRINT_ER("can't release sdio_irq, err(%d)\n", ret);
		}
	sdio_release_host(local_sdio_func);
	
	sdio_intr_lock  = ATWILC_SDIO_HOST_NO_TAKEN;
#endif
}

int linux_sdio_init(void* pv){
#ifndef ATWILC_SDIO_IRQ_GPIO
	init_waitqueue_head(&sdio_intr_waitqueue);
#endif
	return 1;
}

void linux_sdio_deinit(void *pv){
	sdio_unregister_driver(&atwilc_bus);
}


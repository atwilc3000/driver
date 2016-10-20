/*
 * Atmel WILC1000 802.11 b/g/n driver
 *
 * Copyright (c) 2015 Atmel Corportation
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include "wilc_wfi_netdevice.h"

#include <linux/mmc/sdio_func.h>
#include <linux/mmc/card.h>
#include <linux/mmc/sdio_ids.h>
#include <linux/mmc/sdio.h>
#include <linux/mmc/host.h>

#include "linux_wlan_sdio.h"
#define SDIO_MODALIAS "wilc_sdio"

struct wilc_wlan_os_context  g_linux_sdio_os_context;
struct sdio_func *local_sdio_func = NULL;
extern int wilc_netdev_init(void);
extern int sdio_clear_int(void);
extern void wilc_handle_isr(void);
extern int sdio_init(struct wilc_wlan_inp *inp);
extern int sdio_reset(void *pv);
void chip_wakeup(void);
void chip_allow_sleep(void);
void chip_sleep_manually(u32 u32SleepTime);
void host_wakeup_notify(void);
void host_sleep_notify(void);

extern uint8_t u8ResumeOnEvent;

#define SDIO_VENDOR_ID_WILC 0x0296
#define SDIO_DEVICE_ID_WILC 0x5347

static const struct sdio_device_id wilc_sdio_ids[] = {
	{ SDIO_DEVICE(SDIO_VENDOR_ID_WILC, SDIO_DEVICE_ID_WILC) },

	{ }
};


static void wilc_sdio_interrupt(struct sdio_func *func)
{
#ifndef WILC_SDIO_IRQ_GPIO
	sdio_release_host(func);
	wilc_handle_isr();
	sdio_claim_host(func);
#endif /* WILC_SDIO_IRQ_GPIO */
}

int linux_sdio_cmd52(struct sdio_cmd52_t *cmd)
{
	struct sdio_func *func = local_sdio_func;
	int ret;
	u8 data;

	sdio_claim_host(func);

	func->num = cmd->function;
	if (cmd->read_write) {
		if (cmd->raw) {
			sdio_writeb(func, cmd->data, cmd->address, &ret);
			data = sdio_readb(func, cmd->address, &ret);
			cmd->data = data;
		} else {
			sdio_writeb(func, cmd->data, cmd->address, &ret);
		}
	} else {
		data = sdio_readb(func, cmd->address, &ret);
		cmd->data = data;
	}

	sdio_release_host(func);

	if (ret < 0) {
		PRINT_ER("wilc_sdio_cmd52..failed, err(%d)\n", ret);
		return 0;
	}
	return 1;
}

int linux_sdio_cmd53(struct sdio_cmd53_t *cmd)
{
	struct sdio_func *func = local_sdio_func;
	int size, ret;

	sdio_claim_host(func);

	func->num = cmd->function;
	func->cur_blksize = cmd->block_size;
	if (cmd->block_mode)
		size = cmd->count * cmd->block_size;
	else
		size = cmd->count;

	if (cmd->read_write) {
		ret = sdio_memcpy_toio(func,
				       cmd->address,
				       (void *)cmd->buffer,
				       size);
	} else {
		ret = sdio_memcpy_fromio(func,
					 (void *)cmd->buffer,
					 cmd->address,
					 size);
	}

	sdio_release_host(func);

	if (ret < 0) {
		PRINT_ER("wilc_sdio_cmd53..failed, err(%d)\n", ret);
		return 0;
	}

	return 1;
}

static int linux_sdio_probe(struct sdio_func *func,
			    const struct sdio_device_id *id)
{
	PRINT_D(INIT_DBG, "probe function\n");

	PRINT_D(INIT_DBG,"Initializing netdev\n");
	local_sdio_func = func;
	if(wilc_netdev_init()){
		PRINT_ER("Couldn't initialize netdev\n");
		return -1;
	}


	return 0;
}

static void linux_sdio_remove(struct sdio_func *func)
{
}

static int wilc_sdio_suspend(struct device *dev)
{
	printk("\n\n << SUSPEND >>\n\n");
	chip_wakeup();
	/*if there is no events , put the chip in low power mode */
	if(u8ResumeOnEvent == 0)
		chip_sleep_manually(0xffffffff);
	else
	{
		/*notify the chip that host will sleep*/
		host_sleep_notify();
		chip_allow_sleep();
	}

	if((g_linux_sdio_os_context.hif_critical_section) != NULL)
		mutex_lock((struct mutex*)(g_linux_sdio_os_context.hif_critical_section));
	/*reset SDIO to allow kerenl reintilaization at wake up*/
	sdio_reset(NULL);
	/*claim the host to prevent driver SDIO access before resume is called*/
	sdio_claim_host(local_sdio_func);
	return 0 ;
}

static int wilc_sdio_resume(struct device *dev)
{
	sdio_release_host(local_sdio_func);
	/*wake the chip to compelete the re-intialization*/
	chip_wakeup();
	printk("\n\n << RESUME >>\n\n");	
	/*Init SDIO block mode*/
	sdio_init(NULL);

	if((g_linux_sdio_os_context.hif_critical_section)!= NULL){
		if (mutex_is_locked((struct mutex*)(g_linux_sdio_os_context.hif_critical_section))){
			mutex_unlock((struct mutex*)(g_linux_sdio_os_context.hif_critical_section));
		}
	}

	/*if there is an event , notify the chip that the host is awake now*/
	if(u8ResumeOnEvent == 1)
		host_wakeup_notify();

	chip_allow_sleep();
    return 0;

}

static const struct dev_pm_ops wilc_sdio_pm_ops = {	
     .suspend = wilc_sdio_suspend,    
     .resume    = wilc_sdio_resume,
    	};

struct sdio_driver wilc_bus = {
	.name		= SDIO_MODALIAS,
	.id_table	= wilc_sdio_ids,
	.probe		= linux_sdio_probe,
	.remove		= linux_sdio_remove,

    .drv      = {
                  .pm = &wilc_sdio_pm_ops,
               }
};

int enable_sdio_interrupt(void)
{
	int ret = 0;
#ifndef WILC_SDIO_IRQ_GPIO
	sdio_claim_host(local_sdio_func);
	ret = sdio_claim_irq(local_sdio_func, wilc_sdio_interrupt);
	sdio_release_host(local_sdio_func);

	if (ret < 0) {
		PRINT_ER("can't claim sdio_irq, err(%d)\n", ret);
		ret = -EIO;
	}
#endif /* WILC_SDIO_IRQ_GPIO */
	return ret;
}

void disable_sdio_interrupt(void)
{
#ifndef WILC_SDIO_IRQ_GPIO
	int ret;


	sdio_claim_host(local_sdio_func);
	ret = sdio_release_irq(local_sdio_func);
	if (ret < 0)
		PRINT_ER("can't release sdio_irq, err(%d)\n", ret);

	sdio_release_host(local_sdio_func);
#endif /* WILC_SDIO_IRQ_GPIO */
}

int linux_sdio_init(void *pv)
{
	PRINT_D(INIT_DBG, "SDIO speed: %d\n", 
		local_sdio_func->card->host->ios.clock);
	memcpy(&g_linux_sdio_os_context,(struct wilc_wlan_os_context*) pv,sizeof(struct wilc_wlan_os_context));
	return 1;
}

void linux_sdio_deinit(void *pv)
{
	sdio_unregister_driver(&wilc_bus);
}


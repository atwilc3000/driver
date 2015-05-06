#include "at_pwr_dev.h"

#include "linux_wlan_common.h"
#include "linux_os_wrapper.h"
//#include "atwilc_wlan_if.h"
//#include "atwilc_wfi_netdevice.h"
//#include "atwilc_wlan.h"


#ifndef ALLWINNER_BOARD	// tony
#include <asm/gpio.h>
#endif

#include <linux/version.h>
#include <linux/module.h>
#include <linux/version.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/cdev.h>

#include <linux/firmware.h>

#ifdef ATWILC_SDIO
#include "linux_wlan_sdio.h"
#include <linux/mmc/host.h>
#include <linux/mmc/card.h>
#else
#include "linux_wlan_spi.h"
#endif

#ifdef ATWILC_FULLY_HOSTING_AP
#include "atwilc_host_ap.h"
#endif

#include "linux_os_wrapper.h"

unsigned int int_clrd;

/*****************************************************************************/
/*								Macros										 */
/*****************************************************************************/ 
#define DOWNLOAD_BT_FW_ONCE	/* tony */
#define rCOEXIST_CTL 			(0x161E00)
#define rGLOBAL_MODE_CONTROL	(0x1614)

/*****************************************************************************/
/*								Externs										 */
/*****************************************************************************/ 
#ifdef ATWILC_SDIO
#ifdef RESCAN_SDIO
extern struct mmc_host* mmc_host_backup[10];
extern void mmc_start_host(struct mmc_host *host);
extern void mmc_stop_host(struct mmc_host *host);
#endif //RESCAN_SDIO
#endif

/*****************************************************************************/
/*								Type Definitions							 */
/*****************************************************************************/
typedef struct {
	struct mutex cs;
	uint8_t bus_registered[PWR_DEV_SRC_MAX];
	uint8_t power_status[PWR_DEV_SRC_MAX];
	atwilc_hif_func_t hif_func;
	struct mutex hif_cs;
#ifdef DOWNLOAD_BT_FW_ONCE
	uint8_t is_bt_fw_ready;
#endif
}pwr_dev_t;

typedef int (cmd_handler)(int);

typedef struct {
    const char *cmd_str;
    cmd_handler *handle_cmd;
} cmd_handle_entry_t;


/*****************************************************************************/
/*								Private Declarations						 */
/*****************************************************************************/

static dev_t chc_dev_no; // Global variable for the first device number
static struct cdev str_chc_dev; // Global variable for the character device structure
static struct class *chc_dev_class; // Global variable for the device class
struct device *dev;
pwr_dev_t pwr_dev;


#ifdef ATWILC_SDIO
struct semaphore sdio_probe_sync;
#else
struct semaphore spi_probe_sync;
#endif

/* Character device operations*/
static int pwr_dev_open(struct inode *i, struct file *f);
static int pwr_dev_close(struct inode *i, struct file *f);
static ssize_t pwr_dev_read(struct file *f, char __user *buf, size_t len, loff_t *off);
static ssize_t pwr_dev_write(struct file *f, const char __user *buff, size_t len, loff_t *off);

/* Command handlers */
static int cmd_handle_bt_download_fw(int source);
static int cmd_handle_bt_power_up(int source);
static int cmd_handle_bt_power_down(int source);

static int atwilc_bt_firmware_download(void);
static int atwilc_bt_start(void);
static int linux_wlan_device_power(int on_off);
static int linux_wlan_device_detection(int on_off);
void prepare_inp(atwilc_wlan_inp_t* nwi);



static const cmd_handle_entry_t cmd_table[] = {
		{"BT_DOWNLOAD_FW", cmd_handle_bt_download_fw},
		{"BT_POWER_UP", cmd_handle_bt_power_up},
		{"BT_POWER_DOWN", cmd_handle_bt_power_down},
		{(const char *) NULL, NULL}, // Keep the NULL handler at the end of the table
};

static struct file_operations pugs_fops =
{
	.owner = THIS_MODULE,
	.open = pwr_dev_open,
	.release = pwr_dev_close,
	.read = pwr_dev_read,
	.write = pwr_dev_write
};

extern atwilc_hif_func_t hif_sdio;
extern atwilc_hif_func_t hif_spi;

CHIP_PS_STATE_T genuChipPSstate = CHIP_WAKEDUP;
CHIP_PS_STATE_T genuChipPSstateFromWifi;

/*****************************************************************************/
/*								Public Functions							 */
/*****************************************************************************/

int at_pwr_dev_init(void) /* Constructor */
{
	int ret =0;
	
	PRINT_D(PWRDEV_DBG, "at_pwr_dev: registered\n");
	printk("@@@@@@@@@@@@@@@@@@@@@@@ at_pwr_dev: init\n");
	memset(&pwr_dev, 0, sizeof(pwr_dev));
	
	if ((ret = alloc_chrdev_region(&chc_dev_no, 0, 1, "atmel")) < 0)
	{
		return ret;
	}
	if (IS_ERR(chc_dev_class = class_create(THIS_MODULE, "atmel")))
	{
		unregister_chrdev_region(chc_dev_no, 1);
		return PTR_ERR(chc_dev_class);
	}
	if (IS_ERR(dev = device_create(chc_dev_class, NULL, chc_dev_no, NULL, "at_pwr_dev")))
	{
		class_destroy(chc_dev_class);
		unregister_chrdev_region(chc_dev_no, 1);
		return PTR_ERR(dev);
	}

	cdev_init(&str_chc_dev, &pugs_fops);
	if ((ret = cdev_add(&str_chc_dev, chc_dev_no, 1)) < 0)
	{
		device_destroy(chc_dev_class, chc_dev_no);
		class_destroy(chc_dev_class);
		unregister_chrdev_region(chc_dev_no, 1);
		return ret;
	}

#ifdef ATWILC_SDIO
	linux_wlan_init_lock("sdio_probe_sync",&sdio_probe_sync,0);
#else
	linux_wlan_init_lock("spi_probe_sync",&spi_probe_sync,0);
#endif

	linux_wlan_init_mutex("pwr_dev_lock/cs",&pwr_dev.cs,1);
	linux_wlan_init_mutex("hif_lock/hif_cs",&pwr_dev.hif_cs,1);

	return ret;
}

int at_pwr_dev_deinit(void) /* Destructor */
{
	printk("@@@@@@@@@@@@@@@@@@@@@@@ at_pwr_dev: deinit\n");

	if(&pwr_dev.hif_cs != NULL)
		linux_wlan_deinit_mutex(&pwr_dev.hif_cs);
	
	if(&pwr_dev.cs != NULL)
		linux_wlan_deinit_mutex(&pwr_dev.cs);
	
#ifdef ATWILC_SDIO
	linux_wlan_deinit_lock(&sdio_probe_sync);
#else
	linux_wlan_deinit_lock(&spi_probe_sync);
#endif
	
	cdev_del(&str_chc_dev);
	device_destroy(chc_dev_class, chc_dev_no);
	class_destroy(chc_dev_class);
	unregister_chrdev_region(chc_dev_no, 1);
	PRINT_D(PWRDEV_DBG, "at_pwr_dev: unregistered\n");
}

struct mutex * at_pwr_dev_get_bus_lock()
{
	return &pwr_dev.hif_cs;
}

int at_pwr_power_down(int source)
{	
	linux_wlan_lock_mutex(&pwr_dev.cs);
	
	PRINT_D(PWRDEV_DBG, "source: %s, current bus status Wifi: %d, BT: %d\n", (source==PWR_DEV_SRC_WIFI? "Wifi":"BT"), 
		pwr_dev.power_status[PWR_DEV_SRC_WIFI], pwr_dev.power_status[PWR_DEV_SRC_BT]);
	
	if(pwr_dev.power_status[source] == false)
	{
		PRINT_ER("power down request for already powered down source %s\n", 
			(source==PWR_DEV_SRC_WIFI? "Wifi":"BT"));
	}
	else if(((source == PWR_DEV_SRC_WIFI) &&  (pwr_dev.power_status[PWR_DEV_SRC_BT] == true)) ||
		((source == PWR_DEV_SRC_BT) &&  (pwr_dev.power_status[PWR_DEV_SRC_WIFI] == true)))
	{
		PRINT_WRN(PWRDEV_DBG, "Another device is preventing power down. request source is %s\n",
			(source==PWR_DEV_SRC_WIFI? "Wifi":"BT"));
	}
	else
	{
	#if defined(PLAT_ALLWINNER_A20)
		linux_wlan_device_power(0);
		sw_mci_rescan_card(ATWILC_SDIO_CARD_ID, 0);
	#endif
	#if defined(PLAT_ALLWINNER_A23)
		linux_wlan_device_power(0);
		msleep(100);
		linux_wlan_device_detection(0);
	#endif
	#if defined(PLAT_ALLWINNER_A31) || defined(PLAT_SAMA5D4)
		linux_wlan_device_detection(0);
		linux_wlan_device_power(0);
	#endif

	#ifdef DOWNLOAD_BT_FW_ONCE
		pwr_dev.is_bt_fw_ready = false;
	#endif
	}
	pwr_dev.power_status[source] = false;
	
	linux_wlan_unlock_mutex(&pwr_dev.cs);

	return 0;
}

int at_pwr_register_bus(int source)
{
	int ret = 0;

	linux_wlan_lock_mutex(&pwr_dev.cs);

	PRINT_D(PWRDEV_DBG, "source: %s, current bus status Wifi: %d, BT: %d\n", (source==PWR_DEV_SRC_WIFI? "Wifi":"BT"), 
		pwr_dev.bus_registered[PWR_DEV_SRC_WIFI], pwr_dev.bus_registered[PWR_DEV_SRC_BT]);
	
	if(pwr_dev.bus_registered[source] == true)
	{
		PRINT_ER("Registering bus request for already registered source %s\n", 
			(source==PWR_DEV_SRC_WIFI? "Wifi":"BT"));
	}
	else
	{
		if((pwr_dev.bus_registered[PWR_DEV_SRC_WIFI] == true) ||
			(pwr_dev.bus_registered[PWR_DEV_SRC_BT] == true))
		{
			pwr_dev.bus_registered[source] = true;

			if(source == PWR_DEV_SRC_BT)
			{
			#ifdef ATWILC_SDIO
				memcpy((void *)&pwr_dev.hif_func, &hif_sdio, sizeof(atwilc_hif_func_t));
			#else
				memcpy((void *)&pwr_dev.hif_func, &hif_spi, sizeof(atwilc_hif_func_t));
			#endif
			}
		}
		else
		{
			atwilc_wlan_inp_t inp;
			prepare_inp(&inp);

		#if defined(PLAT_ALLWINNER_A31) ||  defined(PLAT_SAMA5D4)
			linux_wlan_device_detection(1);
		#endif	
		#ifdef PANDA_BOARD
		#ifdef RESCAN_SDIO
			// Rescan mmc_host
			mmc_stop_host(mmc_host_backup[2]);
			mmc_start_host(mmc_host_backup[2]);
			msleep(30); // TODO: mabubakr: may need to be removed
		#endif //RESCAN_SDIO
		#endif // PANDA_BOARD
			
#ifdef ATWILC_SDIO
			ret = sdio_register_driver(&atwilc_bus);
			if (ret < 0) {
				printk("init_atwilc_driver: Failed to register sdio driver\n");
			}
			else
			{			
				PRINT_D(INIT_DBG,"Waiting for sdio probe \n");

				if(linux_wlan_lock_timeout(&sdio_probe_sync, 1000) < 0)
				{
					PRINT_D(INIT_DBG,"sdio probe TimedOUT\n");
					ret = -1; /* ERROR */
				}
				else
				{
					PRINT_D(INIT_DBG,"sdio probe is called\n");
					pwr_dev.bus_registered[source] = true;
					if (!hif_sdio.hif_init(&inp, atwilc_debug)) {
						/* EIO	5 */
						ret = -5;
						//goto _fail_;
					}
					memcpy((void *)&pwr_dev.hif_func, &hif_sdio, sizeof(atwilc_hif_func_t));
				}
			}
#else
			if(!linux_spi_init(NULL)){
				PRINT_ER("Can't initialize SPI \n");
		        	ret = -1; /* ERROR */
			}
			else
			{
				PRINT_D(INIT_DBG,"Waiting for spi probe \n");

				if(linux_wlan_lock_timeout(&spi_probe_sync, 1000) < 0)
				{
					PRINT_D(INIT_DBG,"spi probe TimedOUT\n");
					ret = -1; /* ERROR */
				}
				else
				{
					PRINT_D(INIT_DBG,"spi probe is called\n");
					pwr_dev.bus_registered[source] = true;
					if (!hif_spi.hif_init(&inp, atwilc_debug)) {
						/* EIO	5 */
						ret = -5;
						//goto _fail_;
					}
					memcpy((void *)&pwr_dev.hif_func, &hif_spi, sizeof(atwilc_hif_func_t));
				}
			}
#endif
		}
	}

	linux_wlan_unlock_mutex(&pwr_dev.cs);
	return ret;
}

int at_pwr_unregister_bus(int source)
{
	linux_wlan_lock_mutex(&pwr_dev.cs);
	
	PRINT_D(INIT_DBG, "source: %s, current bus status Wifi: %d, BT: %d\n", (source==PWR_DEV_SRC_WIFI? "Wifi":"BT"), 
		pwr_dev.bus_registered[PWR_DEV_SRC_WIFI], pwr_dev.bus_registered[PWR_DEV_SRC_BT]);

	if(pwr_dev.bus_registered[source] == false)
	{
		PRINT_ER("Unregistering bus request for already unregistered source %s\n", 
			(source==PWR_DEV_SRC_WIFI? "Wifi":"BT"));
	}
	else if(((source == PWR_DEV_SRC_WIFI) &&  (pwr_dev.bus_registered[PWR_DEV_SRC_BT] == true)) ||
		((source == PWR_DEV_SRC_BT) &&  (pwr_dev.bus_registered[PWR_DEV_SRC_WIFI] == true)))
	{
		PRINT_WRN(PWRDEV_DBG, "Another device is preventing bus unregisteration. request source is %s\n",
			(source==PWR_DEV_SRC_WIFI? "Wifi":"BT"));
	}
	else
	{
	#ifndef ATWILC_SDIO
		hif_spi.hif_deinit(NULL);
		PRINT_D(INIT_DBG,"SPI unregsiter...\n");	
		spi_unregister_driver(&atwilc_bus);
	#else
		PRINT_D(INIT_DBG,"SDIO unregsiter...\n");
		hif_sdio.hif_deinit(NULL);
		sdio_unregister_driver(&atwilc_bus);	
	#endif
	}

	pwr_dev.bus_registered[source] = false;
	linux_wlan_unlock_mutex(&pwr_dev.cs);

	return 0;
}

static uint32_t dbgflag = N_INIT|N_ERR|N_INTR|N_TXQ|N_RXQ;

void atwilc_debug(uint32_t flag, char *fmt, ...)
{
	char buf[256];
	va_list args;
	int len;

	if (flag & dbgflag) {
		va_start(args, fmt);
		len = vsprintf(buf, fmt, args);
		va_end(args);
		
		linux_wlan_dbg(buf);
	}

	return;
}

/*****************************************************************************/
/*								Private Functions							 */
/*****************************************************************************/

static int pwr_dev_open(struct inode *i, struct file *f)
{
	PRINT_D(PWRDEV_DBG, "at_pwr_dev: open()\n");
	return 0;
}
static int pwr_dev_close(struct inode *i, struct file *f)
{
	PRINT_D(PWRDEV_DBG, "at_pwr_dev: close()\n");
	return 0;
}
static ssize_t pwr_dev_read(struct file *f, char __user *buf, size_t len, loff_t *off)
{
	PRINT_D(PWRDEV_DBG, "at_pwr_dev: read()\n");
	return 0;
}
static ssize_t pwr_dev_write(struct file *f, const char __user *buff, size_t len, loff_t *off)
{
	cmd_handle_entry_t *cmd_entry;
	 
	PRINT_D(PWRDEV_DBG, "at_pwr_dev: dev_write size %d\n", len);
	if(len > 0)
	{
		PRINT_D(PWRDEV_DBG, "received%s\n", buff);

		// call the appropriate command handler
		cmd_entry = (cmd_handle_entry_t *)cmd_table;
        while (cmd_entry->handle_cmd != NULL)
        {
            if (strncmp(cmd_entry->cmd_str, buff, strlen(cmd_entry->cmd_str)) == 0)
            {
                cmd_entry->handle_cmd(PWR_DEV_SRC_BT);
                break;
            }
            cmd_entry++;
        }
	}
	else
	{
		PRINT_D(PWRDEV_DBG, "received invalid size <=0: %d\n", len);
	}
   return len;
}


static int cmd_handle_bt_power_up(int source)
{
	PRINT_D(PWRDEV_DBG, "AT PWR: bt_power_up\n");
	at_pwr_power_up(PWR_DEV_SRC_BT);
	at_pwr_register_bus(PWR_DEV_SRC_BT);
	return 0;
//	atwilc_bus_read
}

static int cmd_handle_bt_power_down(int source)
{
	int ret;
	uint32_t reg;
	
	PRINT_D(PWRDEV_DBG, "AT PWR: bt_power_down\n");

	if((pwr_dev.bus_registered[PWR_DEV_SRC_BT] == false) 
	     && (pwr_dev.power_status[PWR_DEV_SRC_BT] == true))
	{
		
		at_pwr_register_bus(PWR_DEV_SRC_BT);
	}
	
	/* Adjust coexistence module. This should be done from the FW in the future*/
	if(pwr_dev.bus_registered[PWR_DEV_SRC_BT] == true)
	{		
		acquire_bus(ACQUIRE_AND_WAKEUP);
		
		ret = pwr_dev.hif_func.hif_read_reg(rGLOBAL_MODE_CONTROL, &reg);
		if (!ret) {
			PRINT_ER("[atwilc start]: fail read reg %x ...\n", rGLOBAL_MODE_CONTROL);
			if(genuChipPSstateFromWifi == CHIP_SLEEPING_AUTO)
			{
				release_bus(RELEASE_ALLOW_SLEEP, PWR_DEV_SRC_BT);
			}
			else
			{
				release_bus(RELEASE_ONLY, PWR_DEV_SRC_BT);
			}
			return ret;
		}
		// Clear BT mode
		reg &= ~ BIT1;
		ret = pwr_dev.hif_func.hif_write_reg(rGLOBAL_MODE_CONTROL, reg);
		if (!ret) {
			PRINT_ER("[atwilc start]: fail write reg %x ...\n", rGLOBAL_MODE_CONTROL);
			if(genuChipPSstateFromWifi == CHIP_SLEEPING_AUTO)
			{
				release_bus(RELEASE_ALLOW_SLEEP, PWR_DEV_SRC_BT);
			}
			else
			{
				release_bus(RELEASE_ONLY, PWR_DEV_SRC_BT);
			}
			return ret;
		}
		
		ret = pwr_dev.hif_func.hif_read_reg(rCOEXIST_CTL, &reg);
		if (!ret) {
			PRINT_ER("[atwilc start]: fail read reg %x ...\n", rCOEXIST_CTL);
			if(genuChipPSstateFromWifi == CHIP_SLEEPING_AUTO)
			{
				release_bus(RELEASE_ALLOW_SLEEP, PWR_DEV_SRC_BT);
			}
			else
			{
				release_bus(RELEASE_ONLY, PWR_DEV_SRC_BT);
			}
			return ret;
		}
		// Stop forcing BT and force Wifi
		reg &= ~ BIT9;
		reg |= BIT11;
		ret = pwr_dev.hif_func.hif_write_reg(rCOEXIST_CTL, reg);
		if (!ret) {
			PRINT_ER( "[atwilc start]: fail write reg %x ...\n", rCOEXIST_CTL);
			if(genuChipPSstateFromWifi == CHIP_SLEEPING_AUTO)
			{
				release_bus(RELEASE_ALLOW_SLEEP, PWR_DEV_SRC_BT);
			}
			else
			{
				release_bus(RELEASE_ONLY, PWR_DEV_SRC_BT);
			}
			return ret;
		}
		
		if(genuChipPSstateFromWifi == CHIP_SLEEPING_AUTO)
		{
			release_bus(RELEASE_ALLOW_SLEEP, PWR_DEV_SRC_BT);
		}
		else
		{
			release_bus(RELEASE_ONLY, PWR_DEV_SRC_BT);
		}
		
	}
	
	at_pwr_unregister_bus(PWR_DEV_SRC_BT);
	at_pwr_power_down(PWR_DEV_SRC_BT);
	
	return 0;
}
	
static int cmd_handle_bt_download_fw(int source)
{
	PRINT_D(PWRDEV_DBG, "AT PWR: bt_download_fw\n");
	
#ifdef DOWNLOAD_BT_FW_ONCE
	linux_wlan_lock_mutex(&pwr_dev.cs);
	if(pwr_dev.is_bt_fw_ready == true)
	{
		PRINT_WRN(PWRDEV_DBG,"BT FW already downloaded. Skip!\n");
		linux_wlan_unlock_mutex(&pwr_dev.cs);
		return 0;
	}
	linux_wlan_unlock_mutex(&pwr_dev.cs);
#endif

	if(atwilc_bt_firmware_download() != 0)
	{
		PRINT_ER("Failed to download BT FW");

		at_pwr_unregister_bus(PWR_DEV_SRC_BT);
		
		return -1;
	}

	if(atwilc_bt_start() != 0)
	{
		PRINT_ER("Failed to start BT FW");

		at_pwr_unregister_bus(PWR_DEV_SRC_BT);
		
		return -1;
	}

#ifdef DOWNLOAD_BT_FW_ONCE
	linux_wlan_lock_mutex(&pwr_dev.cs);
	pwr_dev.is_bt_fw_ready = true;
	linux_wlan_unlock_mutex(&pwr_dev.cs);
#endif

	at_pwr_unregister_bus(PWR_DEV_SRC_BT);

	return 0;
}

void prepare_inp(atwilc_wlan_inp_t* nwi)
{	
	nwi->os_context.os_private = (void *)&pwr_dev;
	
	nwi->os_func.os_sleep = linux_wlan_msleep;
	nwi->os_func.os_atomic_sleep = linux_wlan_atomic_msleep;
	nwi->os_func.os_debug = linux_wlan_dbg;
	nwi->os_func.os_malloc = linux_wlan_malloc;
	nwi->os_func.os_malloc_atomic = linux_wlan_malloc_atomic;
	nwi->os_func.os_free = linux_wlan_free;
	nwi->os_func.os_lock = linux_wlan_lock;
	nwi->os_func.os_unlock = linux_wlan_unlock;
	nwi->os_func.os_wait = linux_wlan_lock_timeout;
	nwi->os_func.os_signal = linux_wlan_unlock;
	nwi->os_func.os_enter_cs = linux_wlan_lock_mutex;
	nwi->os_func.os_leave_cs = linux_wlan_unlock_mutex;

	/*Added by Amr - BugID_4720*/
	nwi->os_func.os_spin_lock = linux_wlan_spin_lock;
	nwi->os_func.os_spin_unlock = linux_wlan_spin_unlock;
	
#ifdef ATWILC_SDIO
	nwi->io_func.io_type = HIF_SDIO;
	nwi->io_func.io_init = linux_sdio_init;
	nwi->io_func.io_deinit = linux_sdio_deinit;
	nwi->io_func.u.sdio.sdio_cmd52 = linux_sdio_cmd52;
	nwi->io_func.u.sdio.sdio_cmd53 = linux_sdio_cmd53;
	nwi->io_func.u.sdio.sdio_set_max_speed = linux_sdio_set_max_speed;
	nwi->io_func.u.sdio.sdio_set_default_speed = linux_sdio_set_default_speed;
#else
	nwi->io_func.io_type = HIF_SPI;
	nwi->io_func.io_init = linux_spi_init;
	nwi->io_func.io_deinit = linux_spi_deinit;
	nwi->io_func.u.spi.spi_tx = linux_spi_write;
	nwi->io_func.u.spi.spi_rx = linux_spi_read;
	nwi->io_func.u.spi.spi_trx = linux_spi_write_read;
	nwi->io_func.u.spi.spi_max_speed = linux_spi_set_max_speed;
#endif
}


void chip_allow_sleep(void)
{
	uint32_t reg=0;

	//ATL_PRINTF("in chip_allow_sleep()\n");

	#ifdef ATWILC_SDIO
	pwr_dev.hif_func.hif_read_reg(0xf0, &reg);
	pwr_dev.hif_func.hif_write_reg(0xf0, reg & ~(1 << 0));	
	#else
	pwr_dev.hif_func.hif_read_reg(0x1, &reg);
	pwr_dev.hif_func.hif_write_reg(0x1, reg & ~(1 << 1));	
	#endif
	
	genuChipPSstate = CHIP_SLEEPING_AUTO;
}

void chip_wakeup(void)
{
	uint32_t wakeup_reg_val, clk_status_reg_val, trials=0; 
#ifdef ATWILC_SDIO
	uint32_t u32WakeupReg = 0xf0;
	uint32_t u32ClkStsReg = 0xf0;
	uint32_t u32WakepBit = BIT0;
	uint32_t u32ClkStsBit = BIT4;
#else
	uint32_t u32WakeupReg = 0x1;
	uint32_t u32ClkStsReg = 0x13;
	uint32_t u32WakepBit = BIT1;
	uint32_t u32ClkStsBit = BIT2;
#endif

	//ATL_PRINTF("in chip_wakeup()\n");

	int wake_seq_trials = 5;
	//g_wlan.hif_func.hif_read_reg(0xf0, &wakeup_reg_val);
	pwr_dev.hif_func.hif_read_reg(u32WakeupReg, &wakeup_reg_val);	
	do
	{
		/* Clear bit 0 */
		//g_wlan.hif_func.hif_write_reg(0xf0, wakeup_reg_val & ~(1 << 0));
		/* Set bit 0 */
		//g_wlan.hif_func.hif_write_reg(0xf0, wakeup_reg_val | (1 << 0));
		pwr_dev.hif_func.hif_write_reg(u32WakeupReg, wakeup_reg_val | u32WakepBit);

		//linux_wlan_msleep(1);			
						
		// Check the clock status
		//g_wlan.hif_func.hif_read_reg(0xf0, &clk_status_reg_val);
		pwr_dev.hif_func.hif_read_reg(u32ClkStsReg, &clk_status_reg_val);
		
		// in case of clocks off, wait 2ms, and check it again.
		// if still off, wait for another 2ms, for a total wait of 6ms.
		// If still off, redo the wake up sequence
		while( ((clk_status_reg_val & u32ClkStsBit) == 0) && (((++trials) %3) == 0))
		{
			/* Wait for the chip to stabilize*/
			linux_wlan_msleep(1);			

			// Make sure chip is awake. This is an extra step that can be removed
			// later to avoid the bus access overhead
			//g_wlan.hif_func.hif_read_reg(0xf0, &clk_status_reg_val);
			pwr_dev.hif_func.hif_read_reg(u32ClkStsReg, &clk_status_reg_val);
			
			if((clk_status_reg_val & u32ClkStsBit) == 0)
			{
				atwilc_debug(N_ERR, "clocks still OFF. Wake up failed\n");
			}
		}
		// in case of failure, Reset the wakeup bit to introduce a new edge on the next loop
		if((clk_status_reg_val & u32ClkStsBit) == 0)
		{
			// Reset bit 0
			//g_wlan.hif_func.hif_write_reg(0xf0, wakeup_reg_val & (~ (1 << 0)));
			pwr_dev.hif_func.hif_write_reg(u32WakeupReg, wakeup_reg_val & (~u32WakepBit));
		}
	}while(((clk_status_reg_val & u32ClkStsBit) == 0) && (wake_seq_trials-- >0));


	if(genuChipPSstate == CHIP_SLEEPING_MANUAL)
	{
		uint32_t reg_val;
		
		//g_wlan.hif_func.hif_read_reg(0x1C0C, &reg);
		pwr_dev.hif_func.hif_read_reg(0x1C0C, &reg_val);
		reg_val &= ~(1<<0);
		//g_wlan.hif_func.hif_write_reg(0x1C0C, reg);
		pwr_dev.hif_func.hif_write_reg(0x1C0C, reg_val);

			/* Enable PALDO back right after wakeup *//* Don't need for ATWILC3000 for now*/
	}
	genuChipPSstate = CHIP_WAKEDUP;
}

/*BugID_5213*/
void acquire_bus(BUS_ACQUIRE_T acquire)
{

	//g_wlan.os_func.os_enter_cs(g_wlan.hif_lock);
	linux_wlan_lock_mutex(&pwr_dev.hif_cs);
	
	if(genuChipPSstate != CHIP_WAKEDUP)
	{
		if(acquire == ACQUIRE_AND_WAKEUP)
			chip_wakeup();
	}	
}
void release_bus(BUS_RELEASE_T release, int source)
{
	if(release == RELEASE_ALLOW_SLEEP)
		chip_allow_sleep();

	
	if(source == PWR_DEV_SRC_WIFI)
	{
		genuChipPSstateFromWifi = genuChipPSstate;
	}
	
	//g_wlan.os_func.os_leave_cs(g_wlan.hif_lock);
	linux_wlan_unlock_mutex(&pwr_dev.hif_cs);
}

#if defined(PLAT_ALLWINNER_A23)
 extern void sunxi_mci_rescan_card(unsigned id, unsigned insert);
 extern void wifi_pm_power(int on);
 #define ATWILC_SDIO_CARD_ID	1

 #define _linux_wlan_device_power_on()          wifi_pm_power(1)
 #define _linux_wlan_device_power_off()         wifi_pm_power(0)

 #define _linux_wlan_device_detection()         sunxi_mci_rescan_card(ATWILC_SDIO_CARD_ID,1)
 #define _linux_wlan_device_removal()           sunxi_mci_rescan_card(ATWILC_SDIO_CARD_ID,0)
#elif defined(PLAT_ALLWINNER_A31)
 extern void sw_mci_rescan_card(unsigned id, unsigned insert);
 extern void wifi_pm_power(int on);	// tony to keep allwinner's rule
 #define ATWILC_SDIO_CARD_ID	1

 #define _linux_wlan_device_power_on()          wifi_pm_power(1)
 #define _linux_wlan_device_power_off()         wifi_pm_power(0)

 #define _linux_wlan_device_detection()         sw_mci_rescan_card(ATWILC_SDIO_CARD_ID,1)
 #define _linux_wlan_device_removal()           sw_mci_rescan_card(ATWILC_SDIO_CARD_ID,0)
#elif defined(PLAT_ALLWINNER_A20)
 extern void sw_mci_rescan_card(unsigned id, unsigned insert);
 extern void wifi_pm_power(int on);	// tony to keep allwinner's rule
 #define ATWILC_SDIO_CARD_ID	3

 #define _linux_wlan_device_power_on()          wifi_pm_power(1)
 #define _linux_wlan_device_power_off()         wifi_pm_power(0)

 #define _linux_wlan_device_detection()         sw_mci_rescan_card(ATWILC_SDIO_CARD_ID,1)
 #define _linux_wlan_device_removal()           sw_mci_rescan_card(ATWILC_SDIO_CARD_ID,0)
 #elif defined(PLAT_SAMA5D4)
 #define _linux_wlan_device_detection()		{}
 #define _linux_wlan_device_removal()		{}
 #define _linux_wlan_device_power_on()		{}
 #define _linux_wlan_device_power_off()		{} 
 #else
 #define _linux_wlan_device_detection() 	{}
 #define _linux_wlan_device_removal()		{}
 #define _linux_wlan_device_power_on()		{}
 #define _linux_wlan_device_power_off()		{} 

#endif

static int linux_wlan_device_power(int on_off)
{
    PRINT_D(INIT_DBG,"linux_wlan_device_power.. (%d)\n", on_off);

    if ( on_off )
    {
        _linux_wlan_device_power_on();
    }
    else
    {
        _linux_wlan_device_power_off();
    }

    return 0;
}

static int linux_wlan_device_detection(int on_off)
{
    PRINT_D(INIT_DBG,"linux_wlan_device_detection.. (%d)\n", on_off);

#ifdef ATWILC_SDIO
    if ( on_off )
    {
        _linux_wlan_device_detection();
    }
    else
    {
        _linux_wlan_device_removal();
    }
#endif

    return 0;
}

int at_pwr_power_up(int source)
{
	linux_wlan_lock_mutex(&pwr_dev.cs);

	PRINT_D(PWRDEV_DBG, "source: %s, current bus status Wifi: %d, BT: %d\n", (source==PWR_DEV_SRC_WIFI? "Wifi":"BT"), 
		pwr_dev.power_status[PWR_DEV_SRC_WIFI], pwr_dev.power_status[PWR_DEV_SRC_BT]);
	
	if(pwr_dev.power_status[source] == true)
	{
		PRINT_ER("power up request for already powered up source %s\n", 
			(source==PWR_DEV_SRC_WIFI? "Wifi":"BT"));
	}
	else if((pwr_dev.power_status[PWR_DEV_SRC_WIFI] == true) ||
		(pwr_dev.power_status[PWR_DEV_SRC_BT] == true))
	{
		PRINT_WRN(PWRDEV_DBG, "Device already up. request source is %s\n",(source==PWR_DEV_SRC_WIFI? "Wifi":"BT"));
	}
	else
	{
	#if defined(PLAT_ALLWINNER_A20)
		wifi_pm_power(1);
		sw_mci_rescan_card(ATWILC_SDIO_CARD_ID, 1);
	#endif
	#if defined(PLAT_ALLWINNER_A23)
		ATL_PRINTF("DEVICE POWER UP\n");
	    linux_wlan_device_power(1);
		msleep(100);
		linux_wlan_device_detection(1);
	#endif
	#if defined(PLAT_ALLWINNER_A31) || defined(PLAT_SAMA5D4)
		printk("PLAT_ALLWINNER_A31 POWER UP\n");
		linux_wlan_device_power(0);
		linux_wlan_device_power(1);
		msleep(100);
		//linux_wlan_device_detection(1);
	#endif	
#ifdef PANDA_BOARD
	#ifdef RESCAN_SDIO
		// Rescan mmc_host
		mmc_stop_host(mmc_host_backup[2]);
		mmc_start_host(mmc_host_backup[2]);
		msleep(30); // TODO: mabubakr: may need to be removed
		//If added to initialize A delay is required after interrogation, otherwise 
		// mac_open never gets called!
	#endif //RESCAN_SDIO
#endif // PANDA_BOARD
		
	}
	pwr_dev.power_status[source] = true;
	linux_wlan_unlock_mutex(&pwr_dev.cs);

	return 0;
}

static int atwilc_bt_firmware_download(void)
{
	uint32_t offset;
	uint32_t addr, size, size2, blksz;
	uint8_t *dma_buffer;
	int ret = 0;
	uint32_t reg;
	const struct firmware* atwilc_bt_firmware;
	uint8_t *buffer;
	size_t buffer_size;
	
	PRINT_WRN(PWRDEV_DBG, "Bluetooth firmware: %s\n", BT_FIRMWARE);
#ifdef ATWILC_SDIO
	if( request_firmware(&atwilc_bt_firmware,BT_FIRMWARE, dev) != 0){
		PRINT_ER("%s - firmare not available. Skip!\n",BT_FIRMWARE);
		goto _fail_1;
	}
#else
	if( request_firmware(&atwilc_bt_firmware,BT_FIRMWARE, dev) != 0){
		PRINT_ER("%s - firmare not available. Skip!\n",BT_FIRMWARE);
		goto _fail_1;
	}
#endif

	buffer = atwilc_bt_firmware->data;
	buffer_size = (size_t)atwilc_bt_firmware->size;
	if(buffer_size <= 0)
	{
		PRINT_ER("Firmware size = 0!\n");
		goto _fail_1;
	}
	acquire_bus(ACQUIRE_AND_WAKEUP);
	
	ret = pwr_dev.hif_func.hif_write_reg(0x4f0000, 0x71);
	if (!ret) {
		PRINT_ER( "[atwilc start]: fail write reg 0x4f0000 ...\n");
		if(genuChipPSstateFromWifi == CHIP_SLEEPING_AUTO)
		{
			release_bus(RELEASE_ALLOW_SLEEP, PWR_DEV_SRC_BT);
		}
		else
		{
			release_bus(RELEASE_ONLY, PWR_DEV_SRC_BT);
		}
		
		goto _fail_1;
	}

	/**
	Avoid booting from BT boot ROM. Make sure that Drive IRQN [SDIO platform]
	or SD_DAT3 [SPI platform] to ?1?
	**/
	/* Set cortus reset register to register control. */
	ret = pwr_dev.hif_func.hif_read_reg(0x3b0090, &reg);
	if (!ret) {
		PRINT_ER( "[atwilc start]: fail read reg 0x3b0090 ...\n");
		if(genuChipPSstateFromWifi == CHIP_SLEEPING_AUTO)
		{
			release_bus(RELEASE_ALLOW_SLEEP, PWR_DEV_SRC_BT);
		}
		else
		{
			release_bus(RELEASE_ONLY, PWR_DEV_SRC_BT);
		}
		goto _fail_1;
	}
	reg |= (1 << 0);
	ret = pwr_dev.hif_func.hif_write_reg(0x3b0090, reg);
	if (!ret) {
		PRINT_ER( "[atwilc start]: fail write reg 0x3b0090 ...\n");
		if(genuChipPSstateFromWifi == CHIP_SLEEPING_AUTO)
		{
			release_bus(RELEASE_ALLOW_SLEEP, PWR_DEV_SRC_BT);
		}
		else
		{
			release_bus(RELEASE_ONLY, PWR_DEV_SRC_BT);
		}
		goto _fail_1;
	}
	
	pwr_dev.hif_func.hif_read_reg(0x3B0400,&reg);

	if(reg & (1ul<<2))
	{
		reg &= ~(1ul << 2);
	}
	else
	{
		reg |= (1ul << 2);
		pwr_dev.hif_func.hif_write_reg(0x3B0400, reg);
		reg &= ~(1ul << 2);
	}
	pwr_dev.hif_func.hif_write_reg(0x3B0400, reg);
	//pwr_dev.os_func.os_leave_cs(p->hif_lock);
	
	if(genuChipPSstateFromWifi == CHIP_SLEEPING_AUTO)
	{
		release_bus(RELEASE_ALLOW_SLEEP, PWR_DEV_SRC_BT);
	}
	else
	{
		release_bus(RELEASE_ONLY, PWR_DEV_SRC_BT);
	}
	
	// blocks of sizes > 512 causes the wifi to hang!
	blksz = (1ul << 9);//(1ul << 12); /* Bug 4703: 4KB Good enough size for most platforms = PAGE_SIZE. */
	/* Allocate a DMA coherent  buffer. */
	dma_buffer = (uint8_t *)linux_wlan_malloc(blksz);
	if (dma_buffer == NULL) {
		/*EIO	5*/
		ret = -5;
		PRINT_ER("Can't allocate buffer for BT firmware download IO error\n ");		
		goto _fail_1;
	}
	
	PRINT_D(GENERIC_DBG,"Downloading BT firmware size = %d ...\n",buffer_size);
	/**
		load the firmware
	**/

	offset = 0;
//	do {
		//memcpy(&addr, &buffer[offset], 4);
		addr = 0x400000;
		//memcpy(&size, &buffer[offset+4], 4);
		size = buffer_size;
#ifdef BIG_ENDIAN
		addr = BYTE_SWAP(addr);
		size = BYTE_SWAP(size);
#endif
		//p->os_func.os_enter_cs(p->hif_lock);
		//offset += 8;		
		offset = 0;

		while(((int)size) && (offset < buffer_size)) {
			if(size <= blksz) {
				size2 = size;
			} else {
				size2 = blksz;				
			}
			/* Copy firmware into a DMA coherent buffer */
			memcpy(dma_buffer, &buffer[offset], size2);
			
			acquire_bus(ACQUIRE_AND_WAKEUP);
			
			ret = pwr_dev.hif_func.hif_block_tx(addr, dma_buffer, size2);

			#ifdef PLAT_ALLWINNER_A31
			linux_wlan_msleep(1); // linux_wlan_atomic_msleep
			#endif
			
			if(genuChipPSstateFromWifi == CHIP_SLEEPING_AUTO)
			{
				release_bus(RELEASE_ALLOW_SLEEP, PWR_DEV_SRC_BT);
			}
			else
			{
				release_bus(RELEASE_ONLY, PWR_DEV_SRC_BT);
			}
			
			if (!ret) break;

			#ifdef PLAT_ALLWINNER_A31
			// Ticket #878: delay after the block tx, or else the FW will be downloaded corrupted in the IRAM
			// for an unknown reason			
			linux_wlan_msleep(1);
			#endif				
		
			addr += size2;
			offset += size2;
			size -= size2;			
		}
		
		if (!ret){ 
			/*EIO	5*/
			ret = -5;
			PRINT_ER("Can't download BT firmware IO error\n ");			
			goto _fail_;
		}
		PRINT_D(GENERIC_DBG,"BT Offset = %d\n",offset);
//	} while (offset < buffer_size); 

_fail_:
		if(dma_buffer) linux_wlan_free(dma_buffer);
_fail_1:

	/* Freeing FW buffer */
	PRINT_D(INIT_DBG,"Freeing BT FW buffer ...\n");
	PRINT_D(GENERIC_DBG,"Releasing BT firmware\n");
	release_firmware(atwilc_bt_firmware);

	PRINT_D(INIT_DBG,"BT Download Succeeded \n");	
	
	return (ret < 0)? ret:0;
}

static int atwilc_bt_start(void)
{
	uint32_t val32 = 0;
	int ret=0;

	/**
		Go...
	**/

	//p->hif_func.hif_write_reg(0x150014, reg);
	
	acquire_bus(ACQUIRE_AND_WAKEUP);

	PRINT_D(GENERIC_DBG,"Starting BT firmware\n");

	/**
	Write the firmware download complete magic value 0x10ADD09E at 
	location 0xFFFF000C (Cortus map) or C000C (AHB map).
	This will let the boot-rom code execute from RAM.
	**/
	pwr_dev.hif_func.hif_write_reg(0x4F000c, 0x10add09e);


	pwr_dev.hif_func.hif_read_reg(0x3B0400, &val32);
	//val32 &= ~((1ul << 2));
	val32 &= ~((1ul << 2) | (1ul << 3));
	pwr_dev.hif_func.hif_write_reg(0x3B0400, val32);
	
	linux_wlan_atomic_msleep(100);

	//val32 |= ((1ul << 2));
	val32 |= ((1ul << 2) | (1ul << 3));

	pwr_dev.hif_func.hif_write_reg(0x3B0400, val32);
			
	PRINT_D(INIT_DBG,"BT Start Succeeded \n");
		
	if(genuChipPSstateFromWifi == CHIP_SLEEPING_AUTO)
	{
		release_bus(RELEASE_ALLOW_SLEEP, PWR_DEV_SRC_BT);
	}
	else
	{
		release_bus(RELEASE_ONLY, PWR_DEV_SRC_BT);
	}
	
	return (ret<0)?ret:0;
}

EXPORT_SYMBOL(linux_wlan_init_spin_lock);
EXPORT_SYMBOL(genuChipPSstate);
EXPORT_SYMBOL(linux_wlan_atomic_msleep);
EXPORT_SYMBOL(linux_wlan_lock);
EXPORT_SYMBOL(at_pwr_power_down);
EXPORT_SYMBOL(linux_wlan_malloc_atomic);
EXPORT_SYMBOL(linux_wlan_lock_mutex);
EXPORT_SYMBOL(acquire_bus);
EXPORT_SYMBOL(linux_wlan_deinit_lock);
#ifdef ATWILC_SDIO
EXPORT_SYMBOL(local_sdio_func);
EXPORT_SYMBOL(disable_sdio_interrupt);
EXPORT_SYMBOL(enable_sdio_interrupt);
#else
EXPORT_SYMBOL(atwilc_spi_dev);
#endif //ATWILC_SDIO
EXPORT_SYMBOL(linux_wlan_deinit_mutex);
EXPORT_SYMBOL(linux_wlan_unlock_mutex);
EXPORT_SYMBOL(hif_spi);
EXPORT_SYMBOL(at_pwr_dev_get_bus_lock);
EXPORT_SYMBOL(linux_wlan_malloc);
EXPORT_SYMBOL(release_bus);
EXPORT_SYMBOL(hif_sdio);
EXPORT_SYMBOL(linux_wlan_deinit_spin_lock);
EXPORT_SYMBOL(atwilc_debug);
EXPORT_SYMBOL(linux_wlan_spin_unlock);
EXPORT_SYMBOL(linux_wlan_lock_timeout);
EXPORT_SYMBOL(linux_wlan_init_mutex);
EXPORT_SYMBOL(linux_wlan_unlock);
EXPORT_SYMBOL(linux_wlan_spin_lock);
EXPORT_SYMBOL(linux_wlan_dbg);
EXPORT_SYMBOL(linux_wlan_init_lock);
EXPORT_SYMBOL(at_pwr_register_bus);
EXPORT_SYMBOL(at_pwr_power_up);
EXPORT_SYMBOL(chip_allow_sleep);
EXPORT_SYMBOL(linux_wlan_free);
EXPORT_SYMBOL(at_pwr_unregister_bus);
EXPORT_SYMBOL(internal_alloc);
EXPORT_SYMBOL(linux_wlan_msleep);

#ifdef ATWILC_DEBUGFS
EXPORT_SYMBOL(atwilc_debugfs_init);
EXPORT_SYMBOL(atwilc_debugfs_remove);
EXPORT_SYMBOL(kmsgdump_write);
EXPORT_SYMBOL(REGION);
EXPORT_SYMBOL(DEBUG_LEVEL);
#endif //ATWILC_DEBUGFS

MODULE_LICENSE("GPL");


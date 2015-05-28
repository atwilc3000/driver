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
#include "atl_os_wrapper.h"
//#ifdef ATWILC_SDIO
#include "linux_wlan_sdio.h"
//#endif //ATWILC_SDIO
//#include "atwilc_wlan_if.h"


#define HIF_SDIO           (0)
#define HIF_SPI            (1 << 0)
#define HIF_SDIO_GPIO_IRQ  (1 << 2)

#define N_INIT		0x00000001
#define N_ERR		0x00000002
#define N_TXQ		0x00000004
#define N_INTR 		0x00000008
#define N_RXQ		0x00000010

typedef enum {
	ACQUIRE_ONLY				 = 0,
	ACQUIRE_AND_WAKEUP     	= 1,
} BUS_ACQUIRE_T;

typedef enum {
	RELEASE_ONLY				= 0,
	RELEASE_ALLOW_SLEEP		= 1,
} BUS_RELEASE_T;

typedef enum {
	CHIP_WAKEDUP     		= 0,
	CHIP_SLEEPING_AUTO      = 1,
	CHIP_SLEEPING_MANUAL  = 2
} CHIP_PS_STATE_T;

typedef struct {
	void *os_private;

	void *hif_critical_section;

	uint32_t tx_buffer_size;
	void *txq_critical_section;
	
	/*Added by Amr - BugID_4720*/
	void *txq_add_to_head_critical_section;
	void *txq_spin_lock;
	
	void *txq_wait_event;
#ifdef MEMORY_STATIC
	uint32_t rx_buffer_size;
#endif
	void *rxq_critical_section;
	void *rxq_wait_event;

	void *cfg_wait_event;
} atwilc_wlan_os_context_t;

typedef struct {
	void (*os_sleep)(uint32_t);
	void (*os_atomic_sleep)(uint32_t);
	void (*os_debug)(uint8_t *);
	void *(*os_malloc)(uint32_t);
	void *(*os_malloc_atomic)(uint32_t);
	void (*os_free)(void *);
	void (*os_lock)(void *);
	void (*os_unlock)(void *);
	int (*os_wait)(void *,ATL_Uint32);
	void (*os_signal)(void *);
	void (*os_enter_cs)(void *);
	void (*os_leave_cs)(void *);

	/*Added by Amr - BugID_4720*/
	void (*os_spin_lock)(void *, unsigned long *);
	void (*os_spin_unlock)(void *, unsigned long *);
	
} atwilc_wlan_os_func_t;

typedef struct {
	int io_type;
	int (*io_init)(void *);
	void (*io_deinit)(void *);
	union {
		struct {
			int (*sdio_cmd52)(sdio_cmd52_t *);
			int (*sdio_cmd53)(sdio_cmd53_t *);
		} sdio;
		struct {
			int (*spi_tx)(uint8_t *, uint32_t);
			int (*spi_rx)(uint8_t *, uint32_t);
			int (*spi_trx)(uint8_t *, uint8_t *, uint32_t);
		} spi;
	} u;
} atwilc_wlan_io_func_t;

typedef struct {
	void (*rx_indicate)(uint8_t *, uint32_t,uint32_t);
	void (*rx_complete)(void);
} atwilc_wlan_net_func_t;

typedef struct {
	void (*mac_indicate)(int);
} atwilc_wlan_indicate_func_t;

typedef struct {
	atwilc_wlan_os_context_t os_context;
	atwilc_wlan_os_func_t os_func;
	atwilc_wlan_io_func_t io_func;
	atwilc_wlan_net_func_t net_func;
	atwilc_wlan_indicate_func_t indicate_func;
} atwilc_wlan_inp_t;

typedef void (*atwilc_debug_func)(uint32_t, char *, ...);

typedef struct {
	int (*hif_init)(atwilc_wlan_inp_t *, atwilc_debug_func);
	int (*hif_deinit)(void *);
	int (*hif_read_reg)(uint32_t, uint32_t *);
	int (*hif_write_reg)(uint32_t, uint32_t);
	int (*hif_block_rx)(uint32_t, uint8_t *, uint32_t);
	int (*hif_block_tx)(uint32_t, uint8_t *, uint32_t);	
	int (*hif_sync)(void);
	int (*hif_clear_int)(void);	
	int (*hif_read_int)(uint32_t *);
	int (*hif_clear_int_ext)(uint32_t);
	int (*hif_read_size)(uint32_t *);
	int (*hif_block_tx_ext)(uint32_t, uint8_t *, uint32_t);	
	int (*hif_block_rx_ext)(uint32_t, uint8_t *, uint32_t);	
	int (*hif_sync_ext)(int);	
} atwilc_hif_func_t;

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

void chip_allow_sleep(int source);
void chip_wakeup(int source);

void acquire_bus(BUS_ACQUIRE_T acquire, int source);
void release_bus(BUS_RELEASE_T release, int source);

void atwilc_debug(uint32_t flag, char *fmt, ...);

struct mutex * at_pwr_dev_get_bus_lock(void);

#endif //AT_PWR_DEV_H

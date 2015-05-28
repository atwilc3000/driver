#ifndef ATWILC_WLAN_H
#define ATWILC_WLAN_H

#include "atwilc_type.h"


#define ISATWILC3000(id)   (((id & 0xfffff000) == 0x300000) ? 1 : 0) 


/********************************************

	Mac eth header length 

********************************************/
#define DRIVER_HANDLER_SIZE 4
#define MAX_MAC_HDR_LEN         26 //QOS_MAC_HDR_LEN
#define SUB_MSDU_HEADER_LENGTH  14
#define SNAP_HDR_LEN            8
#define ETHERNET_HDR_LEN          14
#define WORD_ALIGNMENT_PAD        0		

#define ETH_ETHERNET_HDR_OFFSET   (MAX_MAC_HDR_LEN + SUB_MSDU_HEADER_LENGTH + \
                                   SNAP_HDR_LEN - ETHERNET_HDR_LEN + WORD_ALIGNMENT_PAD)
     
/*Bug3959: transmitting mgmt frames received from host*/
#define HOST_HDR_OFFSET		4
#define ETHERNET_HDR_LEN          14
#define IP_HDR_LEN                20
#define IP_HDR_OFFSET             ETHERNET_HDR_LEN
#define UDP_HDR_OFFSET            (IP_HDR_LEN + IP_HDR_OFFSET)
#define UDP_HDR_LEN               8
#define UDP_DATA_OFFSET           (UDP_HDR_OFFSET + UDP_HDR_LEN)
#define ETH_CONFIG_PKT_HDR_LEN    UDP_DATA_OFFSET

#define ETH_CONFIG_PKT_HDR_OFFSET (ETH_ETHERNET_HDR_OFFSET + \
                                   ETH_CONFIG_PKT_HDR_LEN)
#define   ACTION         0xD0
#define   PROBE_REQ   0x40
#ifdef ATWILC_FULLY_HOSTING_AP
#define	FH_TX_HOST_HDR_OFFSET	24
#endif

/********************************************

	Endian Conversion 

********************************************/

#define BYTE_SWAP(val) ((((val) & 0x000000FF) << 24) + \
                                   		(((val) & 0x0000FF00) << 8)  + \
                                   		(((val) & 0x00FF0000) >> 8)   + \
                                   		(((val) & 0xFF000000) >> 24))

/********************************************

	Register Defines

********************************************/
#define ATWILC_PERIPH_REG_BASE 0x1000
/*BugID_5137*/
#define ATWILC_CHANGING_VIR_IF                     (0x108c)
#define ATWILC_CHIPID	(ATWILC_PERIPH_REG_BASE)
#define ATWILC_GLB_RESET_0 (ATWILC_PERIPH_REG_BASE + 0x400)
#define ATWILC_PIN_MUX_0 (ATWILC_PERIPH_REG_BASE + 0x408)
#define ATWILC_HOST_TX_CTRL (ATWILC_PERIPH_REG_BASE + 0x6c)
#define ATWILC_HOST_RX_CTRL_0 (ATWILC_PERIPH_REG_BASE + 0x70)
#define ATWILC_HOST_RX_CTRL_1 (ATWILC_PERIPH_REG_BASE + 0x74)
#define ATWILC_HOST_VMM_CTL	(ATWILC_PERIPH_REG_BASE + 0x78)
#define ATWILC_HOST_RX_CTRL	(ATWILC_PERIPH_REG_BASE + 0x80)
#define ATWILC_HOST_RX_EXTRA_SIZE	(ATWILC_PERIPH_REG_BASE + 0x84)
#define ATWILC_HOST_TX_CTRL_1	(ATWILC_PERIPH_REG_BASE + 0x88)
#define ATWILC_INTERRUPT_CORTUS_0	(ATWILC_PERIPH_REG_BASE + 0xa8)
#define ATWILC_MISC	(ATWILC_PERIPH_REG_BASE+0x428)
#define ATWILC_INTR_REG_BASE (ATWILC_PERIPH_REG_BASE+0xa00)
#define ATWILC_INTR_ENABLE (ATWILC_INTR_REG_BASE)
#define ATWILC_INTR2_ENABLE (ATWILC_INTR_REG_BASE+4)

#define ATWILC_INTR_POLARITY (ATWILC_INTR_REG_BASE+0x10)
#define ATWILC_INTR_TYPE (ATWILC_INTR_REG_BASE+0x20)
#define ATWILC_INTR_CLEAR (ATWILC_INTR_REG_BASE+0x30)
#define ATWILC_INTR_STATUS (ATWILC_INTR_REG_BASE+0x40)

#define ATWILC_VMM_TBL_SIZE 64
#define ATWILC_VMM_TX_TBL_BASE (0x150400)
#define ATWILC_VMM_RX_TBL_BASE (0x150500)

#define ATWILC_VMM_BASE 0x150000
#define ATWILC_VMM_CORE_CTL (ATWILC_VMM_BASE)
#define ATWILC_VMM_TBL_CTL (ATWILC_VMM_BASE+0x4)
#define ATWILC_VMM_TBL_ENTRY (ATWILC_VMM_BASE+0x8)
#define ATWILC_VMM_TBL0_SIZE (ATWILC_VMM_BASE+0xc)
#define ATWILC_VMM_TO_HOST_SIZE (ATWILC_VMM_BASE+0x10)
#define ATWILC_VMM_CORE_CFG (ATWILC_VMM_BASE+0x14)
#define ATWILC_VMM_TBL_ACTIVE (ATWILC_VMM_BASE+040)
#define ATWILC_VMM_TBL_STATUS (ATWILC_VMM_BASE+0x44)

#define ATWILC_SPI_REG_BASE 0xe800
#define ATWILC_SPI_CTL (ATWILC_SPI_REG_BASE)
#define ATWILC_SPI_MASTER_DMA_ADDR (ATWILC_SPI_REG_BASE+0x4)
#define ATWILC_SPI_MASTER_DMA_COUNT (ATWILC_SPI_REG_BASE+0x8)
#define ATWILC_SPI_SLAVE_DMA_ADDR (ATWILC_SPI_REG_BASE+0xc)
#define ATWILC_SPI_SLAVE_DMA_COUNT (ATWILC_SPI_REG_BASE+0x10)
#define ATWILC_SPI_TX_MODE (ATWILC_SPI_REG_BASE+0x20)
#define ATWILC_SPI_PROTOCOL_CONFIG (ATWILC_SPI_REG_BASE+0x24)
#define ATWILC_SPI_INTR_CTL (ATWILC_SPI_REG_BASE+0x2c)

#define ATWILC_SPI_PROTOCOL_OFFSET (ATWILC_SPI_PROTOCOL_CONFIG-ATWILC_SPI_REG_BASE)

#define ATWILC_AHB_DATA_MEM_BASE 0x30000
#define ATWILC_AHB_SHARE_MEM_BASE 0xd0000

#define ATWILC_VMM_TBL_RX_SHADOW_BASE ATWILC_AHB_SHARE_MEM_BASE /* Bug 4477 fix */
#define ATWILC_VMM_TBL_RX_SHADOW_SIZE (256) /* Bug 4477 fix */

#define ATWILC_GP_REG_0   0x149c
#define ATWILC_GP_REG_1   0x14a0

#define rCOEXIST_CTL 			(0x161E00)
#define rGLOBAL_MODE_CONTROL	(0x1614)

#define rHAVE_SDIO_IRQ_GPIO_BIT      (0)
#define rHAVE_USE_PMU_BIT            (1)
#define rHAVE_SLEEP_CLK_SRC_RTC_BIT  (2)
#define rHAVE_SLEEP_CLK_SRC_XO_BIT   (3)
#define rHAVE_EXT_PA_INV_TX_RX_BIT   (4)
//#define rHAVE_LEGACY_RF_SETTINGS_BIT (5)
//#define rHAVE_XTAL_24_BIT            (6)
//#define rHAVE_DISABLE_ATWILC_UART_BIT   (7)
#define rFILTER_WIFI_LOGS_BIT        (8)

#define ATWILC_HAVE_SDIO_IRQ_GPIO       (1 << rHAVE_SDIO_IRQ_GPIO_BIT)
#define ATWILC_HAVE_USE_PMU             (1 << rHAVE_USE_PMU_BIT)
#define ATWILC_HAVE_SLEEP_CLK_SRC_RTC   (1 << rHAVE_SLEEP_CLK_SRC_RTC_BIT)
#define ATWILC_HAVE_SLEEP_CLK_SRC_XO    (1 << rHAVE_SLEEP_CLK_SRC_XO_BIT)
#define ATWILC_HAVE_EXT_PA_INV_TX_RX    (1 << rHAVE_EXT_PA_INV_TX_RX_BIT)
//#define ATWILC_HAVE_LEGACY_RF_SETTINGS  (1 << rHAVE_LEGACY_RF_SETTINGS_BIT)
//#define ATWILC_HAVE_XTAL_24             (1 << rHAVE_XTAL_24_BIT)
//#define ATWILC_HAVE_DISABLE_ATWILC_UART    (1 << rHAVE_DISABLE_ATWILC_UART_BIT)
#define ATWILC_FILTER_WIFI_LOGS         (1 << rFILTER_WIFI_LOGS_BIT)

/********************************************

	Wlan Defines

********************************************/
#define ATWILC_CFG_PKT	1
#define ATWILC_NET_PKT 0
/*Bug3959: transmitting mgmt frames received from host*/
#ifdef ATWILC_AP_EXTERNAL_MLME
#define ATWILC_MGMT_PKT 2

#ifdef ATWILC_FULLY_HOSTING_AP
#define ATWILC_FH_DATA_PKT 4
#endif

#endif /*ATWILC_AP_EXTERNAL_MLME*/
#define ATWILC_CFG_SET 1
#define ATWILC_CFG_QUERY 0

#define ATWILC_CFG_RSP	1
#define ATWILC_CFG_RSP_STATUS 2
#define ATWILC_CFG_RSP_SCAN 3

#ifdef ATWILC_SDIO
#define ATWILC_PLL_TO	4
#else
#define ATWILC_PLL_TO	2
#endif


#define ABORT_INT   (1<<31)

/*******************************************/
/*        E0 and later Interrupt flags.    */
/*******************************************/
/*******************************************/
/*        E0 and later Interrupt flags.    */
/*           IRQ Status word               */
/* 15:0 = DMA count in words.              */
/* 16: INT0 flag                           */
/* 17: INT1 flag                           */
/* 18: INT2 flag                           */
/* 19: INT3 flag                           */
/* 20: INT4 flag                           */
/* 21: INT5 flag                           */
/*******************************************/
#define                 IRG_FLAGS_OFFSET 16
#define IRQ_DMA_WD_CNT_MASK ((1ul << IRG_FLAGS_OFFSET) - 1)
#define INT_0           (1<<(IRG_FLAGS_OFFSET))
#define INT_1           (1<<(IRG_FLAGS_OFFSET+1))
#define INT_2           (1<<(IRG_FLAGS_OFFSET+2))
#define INT_3           (1<<(IRG_FLAGS_OFFSET+3))
#define INT_4           (1<<(IRG_FLAGS_OFFSET+4))
#define INT_5           (1<<(IRG_FLAGS_OFFSET+5))
#define MAX_NUM_INT     (6)

/*******************************************/
/*        E0 and later Interrupt flags.    */
/*           IRQ Clear word                */
/* 0: Clear INT0                           */
/* 1: Clear INT1                           */
/* 2: Clear INT2                           */
/* 3: Clear INT3                           */
/* 4: Clear INT4                           */
/* 5: Clear INT5                           */
/* 6: Select VMM table 1                   */
/* 7: Select VMM table 2                   */
/* 8: Enable VMM                           */
/*******************************************/
#define CLR_INT0             (1 << 0)
#define CLR_INT1             (1 << 1)
#define CLR_INT2             (1 << 2)
#define CLR_INT3             (1 << 3)
#define CLR_INT4             (1 << 4)
#define CLR_INT5             (1 << 5)
#define SEL_VMM_TBL0         (1 << 6)
#define SEL_VMM_TBL1         (1 << 7)
#define EN_VMM               (1 << 8)

#define DATA_INT_EXT	INT_0
#define PLL_INT_EXT	    INT_1
#define ALL_INT_EXT     (DATA_INT_EXT|PLL_INT_EXT)
#define NUM_INT_EXT     (2)

#define DATA_INT_CLR	CLR_INT0
#define PLL_INT_CLR	    CLR_INT1
#define SLEEP_INT_CLR	CLR_INT2

#define ENABLE_RX_VMM   (SEL_VMM_TBL1 | EN_VMM)
#define ENABLE_TX_VMM   (SEL_VMM_TBL0 | EN_VMM)


/*time for expiring the semaphores of cfg packets*/
#define CFG_PKTS_TIMEOUT	2000


/********************************************

	Tx/Rx Queue Structure

********************************************/

struct txq_entry_t {
	struct txq_entry_t *next;
	struct txq_entry_t *prev;
	int type;
	int tcp_PendingAck_index;
	uint8_t *buffer;
	int buffer_size;
	void *priv;
	int status;
	void (*tx_complete_func)(void *, int);
};

struct rxq_entry_t  {
	struct rxq_entry_t *next;
	uint8_t *buffer;
	int buffer_size;
};

/********************************************

	Configuration Structure

********************************************/

#define MAX_CFG_FRAME_SIZE 1468

typedef struct {
	uint8_t ether_header[14];
	uint8_t ip_header[20];
	uint8_t udp_header[8];
	uint8_t wid_header[8];
	uint8_t frame[MAX_CFG_FRAME_SIZE];
} atwilc_cfg_frame_t;

typedef struct {
	int (*wlan_tx)(uint8_t *, uint32_t, atwilc_tx_complete_func_t);
} atwilc_wlan_cfg_func_t;

typedef struct {
	int type;
	uint32_t seq_no;
} atwilc_cfg_rsp_t; 

typedef struct {
	int (*cfg_wid_set)(uint8_t *, uint32_t, uint16_t, uint8_t *, int);
	int (*cfg_wid_get)(uint8_t *, uint32_t, uint16_t);
	int (*cfg_wid_get_val)(uint16_t, uint8_t *, uint32_t);
	int (*rx_indicate)(uint8_t *, int, atwilc_cfg_rsp_t *);
	int (*cfg_init)(atwilc_debug_func);
} atwilc_cfg_func_t;

#endif

/*
 * Atmel WILC3000 802.11 b/g/n and Bluetooth Combo driver
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

#ifndef __WILC_HOST_AP__
#define __WILC_HOST_AP__

#include "wilc_wlan_if.h"
#include "wilc_wlan.h"
#include "host_interface.h"
#include "wilc_wfi_netdevice.h"

#ifdef WILC_FULLY_HOSTING_AP
#define ITIM				5 /* Traffic Information Map */
#define DEFAULT_TIM_LEN			4
#define FCS_LEN				4
#define VBMAP_SIZE			256
#define MAX_LISTEN_INT			10
#define VBMAP_SIZE			256
#define MIN_TIM_LEN			4
#define PS_PKT_Q_MAX_LEN		16
/* 
 * Time out for discarding a packets in
 * PS queues in millisecs
 */
#define PS_BUFF_TO			500 
#define AGING_THR_IN_SEC		60
#define WORD_OFFSET_4			4
#define TX_DSCR_NEXT_ADDR_WORD_OFFSET	WORD_OFFSET_4
#define WIFI_PERIPH_BASE		0x00000000
#define WIFI_PA_BASE			(WIFI_PERIPH_BASE + 0x9800)
#define PA_BASE				WIFI_PA_BASE
#define rMAC_DTIM_COUNT_ADDR		((unsigned int)(PA_BASE + 0x021C))
#define rMAC_BEACON_PERIOD		((unsigned int)(PA_BASE + 0x020C))
#define rMAC_DTIM_PERIOD		((unsigned int)(PA_BASE + 0x0210))
#define rMAC_TSF_CON			((unsigned int)(PA_BASE + 0x0200))
#define BEST_EFFORT_PRIORITY		0
#define NORMAL_ACK			0
#define	BCAST_NO_ACK			4
#define PROTOCOL_VERSION		0x00
#define MAX_MSDU_LEN			1596
#define PS_POLL_LEN			20
#define QOS_CTRL_FIELD_LEN		2
#define HT_CTRL_FIELD_LEN		4 /* Length of HT Control Field */
#define IS_MANAGMEMENT			0x100
#define IS_MANAGMEMENT_CALLBACK		0x080
#define IS_MGMT_STATUS_SUCCES		0x040
#define GET_PKT_OFFSET(a)		(((a) >> 22) & 0x1ff)

/* Maximum number of hash values possible for the Sta Entry Table */
#define MAX_HASH_VALUES			16
#define SNAP_HDR_ID_LEN			6
#define ETH_PKT_TYPE_OFFSET		12
#define IP_TYPE				0x0800
#define ARP_TYPE			0x0806
#define ONE_X_TYPE			0x888E
#define VLAN_TYPE			0x8100
#define LLTD_TYPE			0x88D9
#define UDP_TYPE			0x11
#define TCP_TYPE			0x06

#define NEXT_ELEMENT_ADDR(base, ofst)	(*((unsigned int *)((u8 *)base + ofst)))

/* Basic Frame Type Codes (2-bit) */
enum BASICTYPE {
	CONTROL		= 0x04,
	DATA_BASICTYPE	= 0x08,
	MANAGEMENT	= 0x00,
	RESERVED	= 0x0C
};

enum STA_PS_STATE {
	ACTIVE_PS	= 0,
	POWER_SAVE	= 1
};

enum TIMBIT {
	AID0_BIT	= 0,
	DTIM_BIT	= 1
};

enum REQUEUE_STATUS {
	RE_Q_ERROR		= 0,
	NO_PKT_IN_QUEUE		= 1,
	PKT_REQUEUED		= 2,
	END_OF_QUEUE		= 3,
	PKT_NOT_REQUEUED	= 4
};

/* Data Pathes */
enum Data_Interface {
	HOST_TO_WLAN	= 0,
	WLAN_TO_WLAN	= 1,
	MONITOR_TO_WLAN	= 2
};

/* Capability Information field bit assignments  */
enum CAPABILITY {
	ESS		= 0x01,		/* ESS capability */
	IBSS		= 0x02,		/* IBSS mode */
	POLLABLE	= 0x04,		/* CF Pollable */
	POLLREQ		= 0x08,		/* Request to be polled */
	PRIVACY		= 0x10,		/* WEP encryption supported */
	SHORTPREAMBLE	= 0x20,		/* Short Preamble is supported */
	SHORTSLOT	= 0x400,	/* Short Slot is supported */
	PBCC		= 0x40,		/* PBCC */
	CHANNELAGILITY	= 0x80,		/* Channel Agility */
	SPECTRUM_MGMT	= 0x100,	/* Spectrum Management */
	DSSS_OFDM	= 0x2000	/* DSSS-OFDM */
};

/* Authentication/Association states of STAs maintained by the Access Point  */
enum STATIONSTATE {
	AUTH_COMPLETE	= 1,	/* Authenticated */
	AUTH_KEY_SEQ1	= 2,	/* Shared Key Auth (Sequence 1) done */
	ASOC		= 3	/* Associated */
};

enum OFFSET {
	TYPE_OFFSET		= 0,
	LENGTH_OFFSET		= 1,
	DTIM_CNT_OFFSET		= 2,
	DTIM_PERIOD_OFFSET	= 3,
	BMAP_CTRL_OFFSET	= 4,
	TIM_OFFSET		= 5
};

/* Frame Type and Subtype Codes (6-bit) */
enum TYPESUBTYPE {
	ASSOC_REQ		= 0x00,
	ASSOC_RSP		= 0x10,
	REASSOC_REQ		= 0x20,
	REASSOC_RSP		= 0x30,
	PROBE_REQ		= 0x40,
	PROBE_RSP		= 0x50,
	BEACON			= 0x80,
	ATIM			= 0x90,
	DISASOC			= 0xA0,
	AUTH			= 0xB0,
	DEAUTH			= 0xC0,
	ACTION			= 0xD0,
	PS_POLL			= 0xA4,
	RTS			= 0xB4,
	CTS			= 0xC4,
	ACK			= 0xD4,
	CFEND			= 0xE4,
	CFEND_ACK		= 0xF4,
	DATA			= 0x08,
	DATA_ACK		= 0x18,
	DATA_POLL		= 0x28,
	DATA_POLL_ACK		= 0x38,
	NULL_FRAME		= 0x48,
	CFACK			= 0x58,
	CFPOLL			= 0x68,
	CFPOLL_ACK		= 0x78,
	QOS_DATA		= 0x88,
	QOS_DATA_ACK		= 0x98,
	QOS_DATA_POLL		= 0xA8,
	QOS_DATA_POLL_ACK	= 0xB8,
	QOS_NULL_FRAME		= 0xC8,
	QOS_CFPOLL		= 0xE8,
	QOS_CFPOLL_ACK		= 0xF8,
	BLOCKACK_REQ		= 0x84,
	BLOCKACK		= 0x94
};

/*****************************************************************************/
/* Structures                                                                */
/*****************************************************************************/
struct q_entry {
	void *fa;			/* Pointer to the element in the queue */
	struct _q_entry_t *tqe_next;	/* Pointer to the next element */
	struct _q_entry_t *tqe_prev;	/* Pointer to the previous element */
};

struct q_head {
	struct q_entry *head;
	struct q_entry *tail;
};

struct list_buff {
	void *head;		/* Head Element of the List */
	void *tail;		/* Tail Element of the List */
	u16 count;		/* Number of Elements in the List */
	u16 lnk_byte_ofst;	/* Offset to Link Pointer in the List in Bytes */
};

/*
 * Basic rate set.
 * This contains the set of basic rates supported by any IBSS.
 */
struct rate_t {
	u8 rates[MAX_RATES_SUPPORTED];
	u8 num_rates;
};

struct beacon_data {
	int size;
	void *buff;
};

struct beacon_info {
	u16 u16beacon_len;
	u8 *u8beacon_frame;
	u8 u8tim_element_index;
	u16 u16tim_element_trailer_len;
	u8 u8vbmap[VBMAP_SIZE];
	u8 u8DTIMPeriod;
	u16 u16Beacon_Period;
};

struct wlan_rx {
	u8 service_class;		/* Service class extracted from header */
	u8 priority_val;		/* Priority value extracted from header */
	u8 u8hdr_len;			/* Length of header including offset */
	u16 u16data_len;		/* Length of the frame excluding headers */
	u16 u16rx_len;			/* Length of the frame including the headers */
	u8 *u8sa_entry;			/* Source Address station entry */
	u8 *u8msa;			/* MAC header start address */
	u8 u8addr1[6];			/* Address1 field extracted from header */
	u8 u8addr2[6];			/* Address2 field extracted from header */
	u8 u8addr3[6];			/* Address3 field extracted from header */
	u8   *u8sa;			/* Pointer to source address */
	u8   *u8da;			/* Pointer to destination address */
	u8   *u8bssid;			/* Pointer to BSSID */
	u8   *u8ta;			/* Pointer to Transmitter address */
	enum BASICTYPE enumType;	/* Packet type extracted from the header */
	u8 u8Sub_type;			/* Packet Sub type extracted from the header */
	u8 u8frm_ds;			/* From DS field of MAC header */
	u8 u8to_ds;			/* To DS field of MAC header */
	bool bIs_grp_addr;		/* Group address flag */
};

struct WILC_ieee80211_sta {
	u32 supp_rates[IEEE80211_NUM_BANDS];
	u8 addr[ETH_ALEN];			/* MAC address of the STA */
	u16 u16AID;				/* Association ID of the STA */
	enum STATIONSTATE state;		/* Auth/Asoc state of the STA */
	struct ieee80211_sta_ht_cap ht_cap;
	unsigned int aging_cnt;			/* Aging cnt for STA */
	struct rate_t op_rates;			/* Operational rates of the STA */
	enum STA_PS_STATE ps_state;		/* Power save state of the STA */
	struct list_buff ps_q_lgcy;		/* Power save queue for the STA */
	u16 num_ps_pkt;				/* Number of UC pkts queued in PSQ */
	u16 num_qd_pkt;				/* Number of UC pkts queued in HW */
	bool ps_poll_rsp_qed;			/* Flag to indicate PS poll rsp is already queued and pending */
};

struct table_elmnt {
	u8 key[6];				/* Key, i.e. the MAC address */
	void *element;				/* Pointer to the table element */
	struct table_elmnt *next_hash_elmnt;	/* Pointer to next bucket element */
};

signed int host_add_beacon(struct WFIDrvHandle *hWFIDrv, unsigned int u32Interval,
			   unsigned int u32DTIMPeriod,
			   unsigned int u32HeadLen, u8 *pu8Head,
			   unsigned int u32TailLen, u8 *pu8Tail);
signed int host_del_beacon(struct WFIDrvHandle *hWFIDrv);
inline void WILC_AP_AddSta(u8 *mac, struct station_parameters *params);
inline void WILC_AP_EditSta(u8 *mac, struct station_parameters *params);
inline void WILC_AP_RemoveSta(u8 *mac);
void process_tbtt_isr(void);
struct wilc_wlan_dev *Get_wlan_context(u16 *pu16size);
#ifdef WILC_AP_EXTERNAL_MLME
int wilc_wlan_txq_add_mgmt_pkt(void *priv, uint8_t *buffer, uint32_t buffer_size, wilc_tx_complete_func_t func);
#endif
int wilc_FH_wlan_txq_add_net_pkt(void *priv, uint8_t *buffer, uint32_t buffer_size, wilc_tx_complete_func_t func);
void WILC_Process_rx_frame(u8 *u8Buff, unsigned int u32Size);
int WILC_Xmit_data(void *ptx_data, enum Data_Interface InterfaceType);
bool filter_monitor_data_frames(u8 *buf, u16 len);
#endif /*  WILC_FULLY_HOSTING_AP */
#endif

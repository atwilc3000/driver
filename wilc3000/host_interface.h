/*!  
*  @file	host_interface.h
*  @brief	File containg host interface APIs
*  @author	zsalah
*  @sa		host_interface.c
*  @date	8 March 2012
*  @version	1.0
*/

#ifndef HOST_INT_H
#define HOST_INT_H

#include "core_configurator.h"
#ifdef SIMULATION
#include "core_config_simulator.h"
#endif //SIMULATION
/*****************************************************************************/
/*								Macros                                       */
/*****************************************************************************/
#if 0
#define WID_BSS_TYPE				0x0000
#define WID_CURRENT_TX_RATE			0x0001
#define WID_CURRENT_CHANNEL			0x0002
#define WID_PREAMBLE				0x0003
#define WID_STATUS					0x0005
#define WID_SCAN_TYPE				0x0007
#define WID_KEY_ID					0x0009
#define	WID_DTIM_PERIOD				0x0010
#define	WID_POWER_MANAGEMENT		0x000B
#define WID_AUTH_TYPE				0x000D
#define WID_SITE_SURVEY				0x000E
#define WID_DTIM_PERIOD				0x0010
#define WID_DISCONNECT				0x0016
#define WID_SHORT_SLOT_ALLOWED		0x001A
#define WID_START_SCAN_REQ			0x001E
#define WID_RSSI					0x001F
#define WID_JOIN_REQ				0x0020
#define WID_11N_TXOP_PROT_DISABLE	0x00B0
#define WID_RTS_THRESHOLD			0x1000
#define WID_FRAG_THRESHOLD			0x1001
#define WID_SHORT_RETRY_LIMIT		0x1002
#define WID_LONG_RETRY_LIMIT		0x1003
#define WID_BEACON_INTERVAL			0x1006
#define WID_ACTIVE_SCAN_TIME		0x100C
#define WID_PASSIVE_SCAN_TIME		0x100D
#define WID_SITE_SURVEY_SCAN_TIME	0x100E
#define WID_AUTH_TIMEOUT			0x1010
#define WID_11I_PSK					0x3008
#define WID_SITE_SURVEY_RESULTS		0x3012
#define WID_ADD_PTK					0x301B
#define WID_ADD_RX_GTK				0x301C
#define WID_ADD_TX_GTK				0x301D
#define WID_ADD_WEP_KEY				0x3019
#define	WID_REMOVE_WEP_KEY			0x301A
#define WID_REMOVE_KEY				0x301E
#define WID_ASSOC_REQ_INFO			0x301F
#define WID_ASSOC_RES_INFO			0x3020
#define WID_PMKID_INFO				0x3082
#define WID_SCAN_CHANNEL_LIST		0x4084
#define WID_11I_MODE			    0x000C
#endif
#define FAIL		0x0000
#define SUCCESS		0x0001

#define IP_ALEN  4

#define AP_MODE     	0x01
#define STATION_MODE	0x02
#define GO_MODE	0x03
#define CLIENT_MODE	0x04


#define MAX_NUM_STA                  9
#define ACTIVE_SCAN_TIME			10
#define PASSIVE_SCAN_TIME			1200
#define MIN_SCAN_TIME				10
#define MAX_SCAN_TIME				1200
#define DEFAULT_SCAN				0
#define USER_SCAN					BIT0
#define OBSS_PERIODIC_SCAN			BIT1
#define OBSS_ONETIME_SCAN			BIT2
#define GTK_RX_KEY_BUFF_LEN			24
#define ADDKEY						0x1
#define REMOVEKEY					0x2
#define DEFAULTKEY					0x4
#define ADDKEY_AP					0x8
#define MAX_NUM_SCANNED_NETWORKS	100 //30		// rachel
#define MAX_NUM_SCANNED_NETWORKS_SHADOW	130
#define MAX_NUM_PROBED_SSID            10  /*One more than the number of scanned ssids*/
#define CHANNEL_SCAN_TIME 			250//250

#define TX_MIC_KEY_LEN				8
#define RX_MIC_KEY_LEN				8
#define PTK_KEY_LEN					16

#define TX_MIC_KEY_MSG_LEN			26
#define RX_MIC_KEY_MSG_LEN			48
#define PTK_KEY_MSG_LEN				39

#define PMKSA_KEY_LEN				22
#define ETH_ALEN  6
#define PMKID_LEN  					16
#define AT_MAX_NUM_PMKIDS  16
#define ATWILC_SUPP_MCS_SET_SIZE	16
#define ATWILC_ADD_STA_LENGTH	40 /* Not including the rates field cause it has variable length*/
#define SCAN_EVENT_DONE_ABORTED 
/*****************************************************************************/
/* Data Types                                                                */
/*****************************************************************************/
//typedef unsigned char	uint8;
//typedef signed char     int8;
//typedef unsigned short	uint16;
//typedef unsigned long   uint32;
//typedef uint32   Bool;

#if 0
typedef enum {WID_CHAR  = 0,
              WID_SHORT = 1,
              WID_INT   = 2,
              WID_STR   = 3,
              WID_ADR   = 4,
              WID_BIN   = 5,
              WID_IP    = 6,
              WID_UNDEF = 7
} WID_TYPE_T;
#endif
typedef struct
{
    ATL_Uint16     cfg_wid;
    WID_TYPE_T	  cfg_type;
    ATL_Sint8     *pu8Para;
} cfg_param_t;

typedef struct _tstrStatistics
{
	ATL_Uint8 	u8LinkSpeed;
	ATL_Sint8	s8RSSI;
	ATL_Uint32 	u32TxCount;
	ATL_Uint32 	u32RxCount;
	ATL_Uint32 	u32TxFailureCount;
	
}tstrStatistics;


typedef enum
{
	HOST_IF_IDLE  					= 0,
	HOST_IF_SCANNING	  			= 1,
	HOST_IF_CONNECTING		= 2,
	HOST_IF_WAITING_CONN_RESP		= 3,
	HOST_IF_CONNECTED				= 4,
	HOST_IF_P2P_LISTEN				=5,
	HOST_IF_FORCE_32BIT  			= 0xFFFFFFFF
}tenuHostIFstate;

typedef struct _tstrHostIFpmkid
{
	ATL_Uint8 bssid[ETH_ALEN];
	ATL_Uint8 pmkid[PMKID_LEN];
}tstrHostIFpmkid;

typedef struct _tstrHostIFpmkidAttr
{
	ATL_Uint8 numpmkid;
	tstrHostIFpmkid pmkidlist[AT_MAX_NUM_PMKIDS];
}tstrHostIFpmkidAttr;
#if 0
/* Scan type parameter for scan request */
typedef enum
{
	PASSIVE_SCAN = 0,
    ACTIVE_SCAN  = 1,
    NUM_SCANTYPE
} tenuScanType;

typedef enum {SITE_SURVEY_1CH    = 0,
              SITE_SURVEY_ALL_CH = 1,
              SITE_SURVEY_OFF    = 2
} SITE_SURVEY_T;
#endif
typedef enum{AUTORATE	= 0,
			 MBPS_1		= 1,
			 MBPS_2		= 2,
			 MBPS_5_5	= 5,
			 MBPS_11	= 11,
			 MBPS_6		= 6,
			 MBPS_9		= 9,
			 MBPS_12	= 12,
			 MBPS_18	= 18,
			 MBPS_24	= 24,
			 MBPS_36	= 36,
			 MBPS_48	= 48,
			 MBPS_54	= 54

}CURRENT_TX_RATE_T;

typedef struct
{
	ATL_Uint32 u32SetCfgFlag;
	ATL_Uint8 ht_enable;
	ATL_Uint8 bss_type;
	ATL_Uint8 auth_type;
	ATL_Uint16 auth_timeout;
	ATL_Uint8 power_mgmt_mode;
	ATL_Uint16 short_retry_limit;
	ATL_Uint16 long_retry_limit;
	ATL_Uint16 frag_threshold;
	ATL_Uint16 rts_threshold;
	ATL_Uint16 preamble_type;
	ATL_Uint8 short_slot_allowed;
	ATL_Uint8 txop_prot_disabled;
	ATL_Uint16 beacon_interval;
	ATL_Uint16 dtim_period;
	SITE_SURVEY_T site_survey_enabled;
	ATL_Uint16 site_survey_scan_time;
	ATL_Uint8 scan_source;
	ATL_Uint16 active_scan_time;
	ATL_Uint16 passive_scan_time;
	CURRENT_TX_RATE_T curr_tx_rate;

}tstrCfgParamVal;

typedef enum {
	RETRY_SHORT		= 1 << 0,
	RETRY_LONG		= 1 << 1,
	FRAG_THRESHOLD	= 1 << 2,
	RTS_THRESHOLD	= 1 << 3,
	BSS_TYPE  = 1 << 4,
	AUTH_TYPE = 1 << 5,
	AUTHEN_TIMEOUT = 1 << 6,
	POWER_MANAGEMENT = 1 << 7,
	PREAMBLE = 1 << 8,
	SHORT_SLOT_ALLOWED = 1 << 9,
	TXOP_PROT_DISABLE = 1 << 10,
	BEACON_INTERVAL = 1 << 11,
	DTIM_PERIOD = 1 << 12,
	SITE_SURVEY = 1 << 13,
	SITE_SURVEY_SCAN_TIME = 1 << 14,
	ACTIVE_SCANTIME = 1 << 15,
	PASSIVE_SCANTIME = 1 << 16,
	CURRENT_TX_RATE = 1 << 17,
	HT_ENABLE = 1 <<18,
}tenuCfgParam;

typedef struct
{
	ATL_Uint8 au8bssid[6];
	ATL_Sint8 s8rssi;
}tstrFoundNetworkInfo;

typedef enum {SCAN_EVENT_NETWORK_FOUND  = 0,
SCAN_EVENT_DONE = 1,
SCAN_EVENT_ABORTED = 2,
SCAN_EVENT_FORCE_32BIT  = 0xFFFFFFFF
}tenuScanEvent;

typedef enum
{
	CONN_DISCONN_EVENT_CONN_RESP  		= 0,
	CONN_DISCONN_EVENT_DISCONN_NOTIF 	= 1,
       CONN_DISCONN_EVENT_FORCE_32BIT  	= 0xFFFFFFFF
}tenuConnDisconnEvent;

typedef enum
{
	WEP,
	WPARxGtk,
	//WPATxGtk,
	WPAPtk,
	PMKSA,
}tenuKeyType;


/*Scan callBack function definition*/
typedef void(*tATWILCpfScanResult)(tenuScanEvent, tstrNetworkInfo*, void*,void*);

/*Connect callBack function definition*/
typedef void(*tATWILCpfConnectResult)(tenuConnDisconnEvent, 
									tstrConnectInfo*, 
									ATL_Uint8, 
									tstrDisconnectNotifInfo*, 
									void*);

#ifdef ATWILC_P2P
typedef void(*tATWILCpfRemainOnChanExpired)(void*, ATL_Uint32);	/*Remain on channel expiration callback function*/
typedef void(*tATWILCpfRemainOnChanReady)(void*);	/*Remain on channel callback function*/
#endif

typedef void(*tATWILCpfFrmToLinux)(ATL_Uint8*, ATL_Uint32, ATL_Uint32);
typedef void(*tATWILCpfFreeEAPBuffParams)(void*);

//typedef ATL_Uint32 ATWILC_WFIDrvHandle;
typedef struct
{
	ATL_Sint32 s32Dummy;
}
*ATWILC_WFIDrvHandle;

/*!
*  @struct 		tstrRcvdNetworkInfo
*  @brief		Structure to hold Received Asynchronous Network info
*  @details		
*  @todo		
*  @sa			
*  @author		Mostafa Abu Bakr
*  @date		25 March 2012
*  @version		1.0 
*/
typedef struct _tstrRcvdNetworkInfo
{
	ATL_Uint8* pu8Buffer;
	ATL_Uint32 u32Length;
} tstrRcvdNetworkInfo;

/*BugID_4156*/
typedef struct _tstrHiddenNetworkInfo
{
	ATL_Uint8  *pu8ssid;
	ATL_Uint8  u8ssidlen;
	
}tstrHiddenNetworkInfo;

typedef struct _tstrHiddenNetwork
{
  //MAX_SSID_LEN
	tstrHiddenNetworkInfo *pstrHiddenNetworkInfo;
  	ATL_Uint8  u8ssidnum;
	
} tstrHiddenNetwork;

typedef struct
{		
	/* Scan user call back function */
	tATWILCpfScanResult	pfUserScanResult;

	/* User specific parameter to be delivered through the Scan User Callback function */
	void* u32UserScanPvoid;

	ATL_Uint32 u32RcvdChCount;
	tstrFoundNetworkInfo astrFoundNetworkInfo[MAX_NUM_SCANNED_NETWORKS];	
}tstrATWILC_UsrScanReq;

typedef struct
{
	ATL_Uint8* pu8bssid;
	ATL_Uint8* pu8ssid;
	ATL_Uint8 u8security;
	AUTHTYPE_T tenuAuth_type;
	size_t ssidLen;
	ATL_Uint8* pu8ConnReqIEs;
	size_t ConnReqIEsLen;
	/* Connect user call back function */
	tATWILCpfConnectResult	pfUserConnectResult;
	ATL_Bool	IsHTCapable;
	/* User specific parameter to be delivered through the Connect User Callback function */
	void* u32UserConnectPvoid;
}tstrATWILC_UsrConnReq;

typedef struct
{
	ATL_Uint32	u32Address;
}tstrHostIfSetDrvHandler;

typedef struct
{
	ATL_Uint32	u32Mode;
}tstrHostIfSetOperationMode;

/*BugID_5077*/
typedef struct
{
	ATL_Uint8	u8MacAddress[ETH_ALEN];
}tstrHostIfSetMacAddress;

/*BugID_5213*/
typedef struct
{
	ATL_Uint8*	u8MacAddress;
}tstrHostIfGetMacAddress;

/*BugID_5222*/
typedef struct
{
	ATL_Uint8	au8Bssid[ETH_ALEN];
	ATL_Uint8	u8Ted;
	ATL_Uint16	u16BufferSize;
	ATL_Uint16	u16SessionTimeout;
}tstrHostIfBASessionInfo;

#ifdef ATWILC_P2P
typedef struct
{
	ATL_Uint16     u16Channel;
	ATL_Uint32  u32duration;
	tATWILCpfRemainOnChanExpired pRemainOnChanExpired;
	tATWILCpfRemainOnChanReady pRemainOnChanReady;
	void *  pVoid;
	ATL_Uint32 u32ListenSessionID;
}tstrHostIfRemainOnChan;

typedef struct
{

ATL_Bool        bReg;
ATL_Uint16     u16FrameType;
ATL_Uint8       u8Regid;


}tstrHostIfRegisterFrame;

#define   ACTION         0xD0
#define   PROBE_REQ   0x40
#define   PROBE_RESP  0x50
#define   ACTION_FRM_IDX   0
#define   PROBE_REQ_IDX     1


enum p2p_listen_state
{
	P2P_IDLE,
	P2P_LISTEN,
	P2P_GRP_FORMATION
};

#endif
typedef struct
{
	/* Scan user structure */
	tstrATWILC_UsrScanReq strATWILC_UsrScanReq;
	
	/* Connect User structure */
	tstrATWILC_UsrConnReq strATWILC_UsrConnReq;

	#ifdef ATWILC_P2P
	/*Remain on channel struvture*/
	tstrHostIfRemainOnChan strHostIfRemainOnChan;
	ATL_Uint8 u8RemainOnChan_pendingreq;
	ATL_Uint64 u64P2p_MgmtTimeout;
	ATL_Uint8   u8P2PConnect;
	#endif

	tenuHostIFstate enuHostIFstate;

	//ATL_Bool bPendingConnRequest;

	#ifndef CONNECT_DIRECT
	ATL_Uint32 u32SurveyResultsCount;
	wid_site_survey_reslts_s astrSurveyResults[MAX_NUM_SCANNED_NETWORKS];
	#endif
	
	ATL_Uint8 au8AssociatedBSSID[ETH_ALEN];
	tstrCfgParamVal strCfgValues;
//semaphores
	ATL_SemaphoreHandle gtOsCfgValuesSem;
	ATL_SemaphoreHandle hSemTestKeyBlock;
	
	ATL_SemaphoreHandle hSemTestDisconnectBlock;
	ATL_SemaphoreHandle hSemGetRSSI;
	ATL_SemaphoreHandle hSemGetLINKSPEED;
	ATL_SemaphoreHandle hSemGetCHNL;
	ATL_SemaphoreHandle hSemInactiveTime;
//timer handlers
	ATL_TimerHandle hScanTimer;
	ATL_TimerHandle hConnectTimer;
	#ifdef ATWILC_P2P
	ATL_TimerHandle hRemainOnChannel;
	#endif

	ATL_Bool IFC_UP;
}tstrATWILC_WFIDrv;

/*!
*  @enum 		tenuATWILC_StaFlag
*  @brief			Used to decode the station flag set and mask in tstrATWILC_AddStaParam
*  @details		
*  @todo		
*  @sa			tstrATWILC_AddStaParam, enum nl80211_sta_flags
*  @author		Enumeraion's creator
*  @date			12 July 2012
*  @version		1.0 Description
*/

typedef enum
{
	ATWILC_STA_FLAG_INVALID = 0,
	ATWILC_STA_FLAG_AUTHORIZED,			/*!<  station is authorized (802.1X)*/
	ATWILC_STA_FLAG_SHORT_PREAMBLE,	/*!< station is capable of receiving frames	with short barker preamble*/
	ATWILC_STA_FLAG_WME,				/*!< station is WME/QoS capable*/
	ATWILC_STA_FLAG_MFP,					/*!< station uses management frame protection*/
	ATWILC_STA_FLAG_AUTHENTICATED		/*!< station is authenticated*/
}tenuATWILC_StaFlag;

typedef struct
{
	ATL_Uint8 au8BSSID[ETH_ALEN];
	ATL_Uint16 u16AssocID;
	ATL_Uint8 u8NumRates;
	ATL_Uint8* pu8Rates;
	ATL_Bool bIsHTSupported;
	ATL_Uint16 u16HTCapInfo;
	ATL_Uint8 u8AmpduParams;
	ATL_Uint8 au8SuppMCsSet[16];
	ATL_Uint16 u16HTExtParams;
	ATL_Uint32 u32TxBeamformingCap;
	ATL_Uint8 u8ASELCap;
	ATL_Uint16 u16FlagsMask;		/*<! Determines which of u16FlagsSet were changed>*/
	ATL_Uint16 u16FlagsSet;		/*<! Decoded according to tenuATWILC_StaFlag */
}tstrATWILC_AddStaParam;

//extern void CfgDisconnected(void* pUserVoid, ATL_Uint16 u16reason, ATL_Uint8 * ie, size_t ie_len);

/*****************************************************************************/
/*																			 */
/*							Host Interface API								 */
/*																			 */
/*****************************************************************************/

/**
*  @brief 			host_int_send_buffered_eap
*  @details 		Sends a buffered eap to WPAS
*  @param[in,out] handle to the wifi driver,
*  @param[in]		pointer to frm_to_linux function
				eapol buffer
				size
				packet offset
				priv
*  @return 		Error code indicating success/failure
*  @note 		
*  @author		aabdelmoghny
*  @date			26 March 2015
*  @version		1.0
*/ 
ATL_Sint32 host_int_send_buffered_eap(ATWILC_WFIDrvHandle hWFIDrv, tATWILCpfFrmToLinux pfFrmToLinux
											, tATWILCpfFreeEAPBuffParams pfFreeEAPBuffParams
											, ATL_Uint8* pu8Buff, ATL_Uint32 u32Size, ATL_Uint32 u32PktOffset
											, void* pvUserArg);
/**
*  @brief 		removes wpa/wpa2 keys
*  @details 	only in BSS STA mode if External Supplicant support is enabled. 
				removes all WPA/WPA2 station key entries from MAC hardware.
*  @param[in,out] handle to the wifi driver
*  @param[in] 	6 bytes of Station Adress in the station entry table
*  @return 		Error code indicating success/failure
*  @note 		
*  @author		zsalah
*  @date		8 March 2012
*  @version		1.0
*/
ATL_Sint32 host_int_remove_key(ATWILC_WFIDrvHandle hWFIDrv,const ATL_Uint8* pu8StaAddress);
/**
*  @brief 		removes WEP key
*  @details 	valid only in BSS STA mode if External Supplicant support is enabled.
				remove a WEP key entry from MAC HW. 
				The BSS Station automatically finds the index of the entry using its 
				BSS ID and removes that entry from the MAC hardware.
*  @param[in,out] handle to the wifi driver
*  @param[in] 	6 bytes of Station Adress in the station entry table
*  @return 		Error code indicating success/failure
*  @note 		NO need for the STA add since it is not used for processing 
*  @author		zsalah
*  @date		8 March 2012
*  @version		1.0
*/
ATL_Sint32 host_int_remove_wep_key(ATWILC_WFIDrvHandle hWFIDrv,ATL_Uint8 u8Index);
/**
*  @brief 		sets WEP deafault key
*  @details 	Sets the index of the WEP encryption key in use, 
				in the key table	
*  @param[in,out] handle to the wifi driver
*  @param[in] 	key index ( 0, 1, 2, 3)
*  @return 		Error code indicating success/failure
*  @note 		
*  @author		zsalah
*  @date		8 March 2012
*  @version		1.0
*/
ATL_Sint32 host_int_set_WEPDefaultKeyID(ATWILC_WFIDrvHandle hWFIDrv,ATL_Uint8 u8Index);

/**
*  @brief 		sets WEP deafault key
*  @details 	valid only in BSS STA mode if External Supplicant support is enabled. 
				sets WEP key entry into MAC hardware when it receives the 
				corresponding request from NDIS.
*  @param[in,out] handle to the wifi driver
*  @param[in] 	message containing WEP Key in the following format
				|---------------------------------------|
				|Key ID Value | Key Length |	Key		|
				|-------------|------------|------------|
				|	1byte	  |		1byte  | Key Length	|
				|---------------------------------------|

*  @return 		Error code indicating success/failure
*  @note 		
*  @author		zsalah
*  @date		8 March 2012
*  @version		1.0
*/
ATL_Sint32 host_int_add_wep_key_bss_sta(ATWILC_WFIDrvHandle hWFIDrv, const ATL_Uint8* pu8WepKey, ATL_Uint8 u8WepKeylen, ATL_Uint8 u8Keyidx);
/**
*  @brief 		host_int_add_wep_key_bss_ap
*  @details 	valid only in AP mode if External Supplicant support is enabled. 
				sets WEP key entry into MAC hardware when it receives the 
				corresponding request from NDIS.
*  @param[in,out] handle to the wifi driver


*  @return 		Error code indicating success/failure
*  @note 		
*  @author		mdaftedar
*  @date		28 Feb 2013
*  @version		1.0
*/
ATL_Sint32 host_int_add_wep_key_bss_ap(ATWILC_WFIDrvHandle hWFIDrv, const ATL_Uint8* pu8WepKey, ATL_Uint8 u8WepKeylen, ATL_Uint8 u8Keyidx,ATL_Uint8 u8mode, AUTHTYPE_T tenuAuth_type);

/**
*  @brief 		adds ptk Key
*  @details 	
*  @param[in,out] handle to the wifi driver
*  @param[in] 	message containing PTK Key in the following format
|-------------------------------------------------------------------------|
|Sta Adress | Key Length |	Temporal Key | Rx Michael Key |Tx Michael Key |
|-----------|------------|---------------|----------------|---------------|
|	6 bytes |	1byte	 |   16 bytes	 |	  8 bytes	  |	   8 bytes	  |
|-------------------------------------------------------------------------|
*  @return 		Error code indicating success/failure
*  @note 		
*  @author		zsalah
*  @date		8 March 2012
*  @version		1.0
*/
ATL_Sint32 host_int_add_ptk(ATWILC_WFIDrvHandle hWFIDrv,ATL_Uint8* pu8Ptk,ATL_Uint8 u8PtkKeylen,
	const ATL_Uint8* mac_addr,ATL_Uint8* pu8RxMic,ATL_Uint8* pu8TxMic,ATL_Uint8 mode,ATL_Uint8 u8Ciphermode,ATL_Uint8 u8Idx);

/**
*  @brief 		host_int_get_inactive_time
*  @details 	
*  @param[in,out] handle to the wifi driver
*  @param[in] 	message containing inactive time

*  @return 		Error code indicating success/failure
*  @note 		
*  @author		mdaftedar
*  @date		15 April 2013
*  @version		1.0
*/
ATL_Sint32 host_int_get_inactive_time(ATWILC_WFIDrvHandle hWFIDrv,ATL_Uint8 * mac,ATL_Uint32* pu32InactiveTime);

/**
*  @brief 		adds Rx GTk Key
*  @details 	
*  @param[in,out] handle to the wifi driver
*  @param[in] 	message containing Rx GTK Key in the following format
|----------------------------------------------------------------------------|
|Sta Address | Key RSC | KeyID | Key Length | Temporal Key	| Rx Michael Key |
|------------|---------|-------|------------|---------------|----------------|
|	6 bytes	 | 8 byte  |1 byte |  1 byte	|   16 bytes	|	  8 bytes	 |
|----------------------------------------------------------------------------|
*  @return 		Error code indicating success/failure
*  @note 		
*  @author		zsalah
*  @date		8 March 2012
*  @version		1.0
*/
	ATL_Sint32 host_int_add_rx_gtk(ATWILC_WFIDrvHandle hWFIDrv, ATL_Uint8* pu8RxGtk,ATL_Uint8 u8GtkKeylen,
			ATL_Uint8 u8KeyIdx,ATL_Uint32 u32KeyRSClen, ATL_Uint8* KeyRSC,
			ATL_Uint8* pu8RxMic,ATL_Uint8* pu8TxMic,ATL_Uint8 mode,ATL_Uint8 u8Ciphermode);


/**
*  @brief 		adds Tx GTk Key
*  @details 	
*  @param[in,out] handle to the wifi driver
*  @param[in] 	message containing Tx GTK Key in the following format
				|----------------------------------------------------|
				| KeyID | Key Length | Temporal Key	| Tx Michael Key |
				|-------|------------|--------------|----------------|
				|1 byte |  1 byte	 |   16 bytes	|	  8 bytes	 |
				|----------------------------------------------------|
*  @return 		Error code indicating success/failure
*  @note 		
*  @author		zsalah
*  @date		8 March 2012
*  @version		1.0
*/
ATL_Sint32 host_int_add_tx_gtk(ATWILC_WFIDrvHandle hWFIDrv,ATL_Uint8 u8KeyLen,ATL_Uint8* pu8TxGtk,ATL_Uint8 u8KeyIdx);

/**
*  @brief 		caches the pmkid 
*  @details 	valid only in BSS STA mode if External Supplicant	 	
				support is enabled. This Function sets the PMKID in firmware 
				when host drivr receives the corresponding request from NDIS. 
				The firmware then includes theset PMKID in the appropriate 
				management frames		 	
*  @param[in,out] handle to the wifi driver
*  @param[in] 	message containing PMKID Info in the following format
|-----------------------------------------------------------------|
|NumEntries |	BSSID[1] | PMKID[1] |  ...	| BSSID[K] | PMKID[K] |
|-----------|------------|----------|-------|----------|----------|
|	   1	|		6	 |   16		|  ...	|	 6	   |	16	  |
|-----------------------------------------------------------------|
*  @return 		Error code indicating success/failure
*  @note 		
*  @author		zsalah
*  @date		8 March 2012
*  @version		1.0
*/

ATL_Sint32 host_int_set_pmkid_info(ATWILC_WFIDrvHandle hWFIDrv, tstrHostIFpmkidAttr* pu8PmkidInfoArray);
/**
*  @brief 		gets the cached the pmkid info
*  @details 	valid only in BSS STA mode if External Supplicant	 	
				support is enabled. This Function sets the PMKID in firmware 
				when host drivr receives the corresponding request from NDIS. 
				The firmware then includes theset PMKID in the appropriate 
				management frames		 	
*  @param[in,out] handle to the wifi driver, 

				  message containing PMKID Info in the following format
		|-----------------------------------------------------------------|
		|NumEntries |	BSSID[1] | PMKID[1] |  ...	| BSSID[K] | PMKID[K] |
		|-----------|------------|----------|-------|----------|----------|
		|	   1	|		6	 |   16		|  ...	|	 6	   |	16	  |
		|-----------------------------------------------------------------|
*  @param[in] 	
*  @return 		Error code indicating success/failure
*  @note 		
*  @author		zsalah
*  @date		8 March 2012
*  @version		1.0
*/

ATL_Sint32 host_int_get_pmkid_info(ATWILC_WFIDrvHandle hWFIDrv,ATL_Uint8* pu8PmkidInfoArray, 
									   ATL_Uint32 u32PmkidInfoLen);

/**
*  @brief 		sets the pass phrase
*  @details 	AP/STA mode. This function gives the pass phrase used to
				generate the Pre-Shared Key when WPA/WPA2 is enabled	
				The length of the field can vary from 8 to 64 bytes, 
				the lower layer should get the  	
*  @param[in,out] handle to the wifi driver, 	
*  @param[in] 	 String containing PSK 
*  @return 		Error code indicating success/failure
*  @note 		
*  @author		zsalah
*  @date		8 March 2012
*  @version		1.0
*/
ATL_Sint32 host_int_set_RSNAConfigPSKPassPhrase(ATWILC_WFIDrvHandle hWFIDrv, ATL_Uint8* pu8PassPhrase, 
														  ATL_Uint8 u8Psklength);
/**
*  @brief 		gets the pass phrase
*  @details 	AP/STA mode. This function gets the pass phrase used to
				generate the Pre-Shared Key when WPA/WPA2 is enabled	
				The length of the field can vary from 8 to 64 bytes, 
				the lower layer should get the  	
*  @param[in,out] handle to the wifi driver, 
				  String containing PSK 
*  @return 		Error code indicating success/failure
*  @note 		
*  @author		zsalah
*  @date		8 March 2012
*  @version		1.0
*/
ATL_Sint32 host_int_get_RSNAConfigPSKPassPhrase(ATWILC_WFIDrvHandle hWFIDrv,
												ATL_Uint8* pu8PassPhrase, ATL_Uint8 u8Psklength);

/**
*  @brief 		gets mac address
*  @details
*  @param[in,out] handle to the wifi driver,

*  @return 		Error code indicating success/failure
*  @note
*  @author		mdaftedar
*  @date		19 April 2012
*  @version		1.0
*/
ATL_Sint32 host_int_get_MacAddress(ATWILC_WFIDrvHandle hWFIDrv,ATL_Uint8* pu8MacAddress);

/**
*  @brief 		sets mac address
*  @details
*  @param[in,out] handle to the wifi driver,

*  @return 		Error code indicating success/failure
*  @note
*  @author		mabubakr
*  @date		16 July 2012
*  @version		1.0
*/
ATL_Sint32 host_int_set_MacAddress(ATWILC_WFIDrvHandle hWFIDrv,ATL_Uint8* pu8MacAddress);

/**
*  @brief 		wait until msg q is empty
*  @details
*  @param[in,out] 

*  @return 		Error code indicating success/failure
*  @note
*  @author		asobhy
*  @date		19 march 2014
*  @version		1.0
*/
ATL_Sint32 host_int_wait_msg_queue_idle(void);

/**
*  @brief 		gets the site survey results
*  @details 	  	
*  @param[in,out] handle to the wifi driver, 
				  Message containing  site survey results in the 
				  following formate 
|---------------------------------------------------|
| MsgLength | fragNo.	| MsgBodyLength	| MsgBody	|					
|-----------|-----------|---------------|-----------|
|	 1		|	  1		|		1		|	 1		|					
-----------------------------------------	 |  ----------------
										     |   						
						|---------------------------------------|
						| Network1 | Netweork2 | ... | Network5 |
						|---------------------------------------|
						|	44	   |	44	   | ... |	 44		|
-------------------------- | ---------------------------------------
							 |
|---------------------------------------------------------------------|
| SSID | BSS Type | Channel | Security Status| BSSID | RSSI |Reserved |
|------|----------|---------|----------------|-------|------|---------|
|  33  |	 1	  |	  1		|		1		 |	  6	 |	 1	|	 1	  |
|---------------------------------------------------------------------|
*  @return 		Error code indicating success/failure
*  @note 		
*  @author		zsalah
*  @date		8 March 2012
*  @version		1.0
*/
#ifndef CONNECT_DIRECT
ATL_Sint32 host_int_get_site_survey_results(ATWILC_WFIDrvHandle hWFIDrv, 
											ATL_Uint8 ppu8RcvdSiteSurveyResults[][MAX_SURVEY_RESULT_FRAG_SIZE], 
											ATL_Uint32 u32MaxSiteSrvyFragLen);
#endif

/**
*  @brief 		sets a start scan request
*  @details 	 	
*  @param[in,out] handle to the wifi driver,
*  @param[in]	Scan Source one of the following values
				DEFAULT_SCAN        0									
				USER_SCAN           BIT0								
				OBSS_PERIODIC_SCAN  BIT1								
				OBSS_ONETIME_SCAN   BIT2	
*  @return 		Error code indicating success/failure
*  @note 		
*  @author		zsalah
*  @date		8 March 2012
*  @version		1.0
*/

ATL_Sint32 host_int_set_start_scan_req(ATWILC_WFIDrvHandle hWFIDrv,ATL_Uint8 scanSource);
/**
*  @brief 		gets scan source of the last scan
*  @details 	 	
*  @param[in,out] handle to the wifi driver,
				Scan Source one of the following values
				DEFAULT_SCAN        0									
				USER_SCAN           BIT0								
				OBSS_PERIODIC_SCAN  BIT1								
				OBSS_ONETIME_SCAN   BIT2	
*  @return 		Error code indicating success/failure
*  @note 		
*  @author		zsalah
*  @date		8 March 2012
*  @version		1.0
*/
 ATL_Sint32 host_int_get_start_scan_req(ATWILC_WFIDrvHandle hWFIDrv,ATL_Uint8* pu8ScanSource);

/**
*  @brief 		sets a join request
*  @details 	 	
*  @param[in,out] handle to the wifi driver,
*  @param[in]	Index of the bss descriptor 	
*  @return 		Error code indicating success/failure
*  @note 		
*  @author		zsalah
*  @date		8 March 2012
*  @version		1.0
*/

ATL_Sint32 host_int_set_join_req(ATWILC_WFIDrvHandle hWFIDrv, ATL_Uint8* pu8bssid,
							     ATL_Uint8* pu8ssid, size_t ssidLen,
							     const ATL_Uint8* pu8IEs, size_t IEsLen,
							     tATWILCpfConnectResult pfConnectResult, void* pvUserArg, 
							     ATL_Uint8 u8security, AUTHTYPE_T tenuAuth_type, 
							     ATL_Uint8 u8channel,
							     void* pJoinParams);

/**
*  @brief 		Flush a join request parameters to FW, but actual connection
*  @details 	The function is called in situation where ATWILC is connected to AP and 
			required to switch to hybrid FW for P2P connection 	
*  @param[in] handle to the wifi driver,
*  @return 		Error code indicating success/failure
*  @note 		
*  @author		Amr Abdel-Moghny
*  @date		19 DEC 2013
*  @version		8.0
*/

ATL_Sint32 host_int_flush_join_req(ATWILC_WFIDrvHandle hWFIDrv);


/**
*  @brief 		disconnects from the currently associated network
*  @details 	 	
*  @param[in,out] handle to the wifi driver,
*  @param[in]	Reason Code of the Disconnection
*  @return 		Error code indicating success/failure
*  @note 		
*  @author		zsalah
*  @date		8 March 2012
*  @version		1.0
*/
ATL_Sint32 host_int_disconnect(ATWILC_WFIDrvHandle hWFIDrv, ATL_Uint16 u16ReasonCode);

/**
*  @brief 		disconnects a sta
*  @details 	 	
*  @param[in,out] handle to the wifi driver,
*  @param[in]	Association Id of the station to be disconnected	
*  @return 		Error code indicating success/failure
*  @note 		
*  @author		zsalah
*  @date		8 March 2012
*  @version		1.0
*/
ATL_Sint32 host_int_disconnect_station(ATWILC_WFIDrvHandle hWFIDrv,ATL_Uint8 assoc_id);
/**
*  @brief 		gets a Association request info
*  @details 	 	
*  @param[in,out] handle to the wifi driver,
				Message containg assoc. req info in the following format
------------------------------------------------------------------------
|                        Management Frame Format                    |
|-------------------------------------------------------------------| 
|Frame Control|Duration|DA|SA|BSSID|Sequence Control|Frame Body|FCS |
|-------------|--------|--|--|-----|----------------|----------|----|
| 2           |2       |6 |6 |6    |		2       |0 - 2312  | 4  |
|-------------------------------------------------------------------|
|                                                                   |
|             Association Request Frame - Frame Body                |
|-------------------------------------------------------------------|
| Capability Information | Listen Interval | SSID | Supported Rates |
|------------------------|-----------------|------|-----------------|
|			2            |		 2         | 2-34 |		3-10        |
| ---------------------------------------------------------------------
*  @return 		Error code indicating success/failure
*  @note 		
*  @author		zsalah
*  @date		8 March 2012
*  @version		1.0
*/

ATL_Sint32 host_int_get_assoc_req_info(ATWILC_WFIDrvHandle hWFIDrv, ATL_Uint8* pu8AssocReqInfo, 
										    ATL_Uint32 u32AssocReqInfoLen);
/**
*  @brief 		gets a Association Response info
*  @details 	 	
*  @param[in,out] handle to the wifi driver,
				Message containg assoc. resp info 
*  @return 		Error code indicating success/failure
*  @note 		
*  @author		zsalah
*  @date		8 March 2012
*  @version		1.0
*/

ATL_Sint32 host_int_get_assoc_res_info(ATWILC_WFIDrvHandle hWFIDrv, ATL_Uint8* pu8AssocRespInfo, 
										    ATL_Uint32 u32MaxAssocRespInfoLen, ATL_Uint32* pu32RcvdAssocRespInfoLen);
/**
*  @brief 		gets a Association Response info
*  @details 	Valid only in STA mode. This function gives the RSSI	
				values observed in all the channels at the time of scanning.
				The length of the field is 1 greater that the total number of 
				channels supported. Byte 0 contains the number of channels while
				each of Byte N contains	the observed RSSI value for the channel index N.							
*  @param[in,out] handle to the wifi driver,
				array of scanned channels' RSSI 
*  @return 		Error code indicating success/failure
*  @note 		
*  @author		zsalah
*  @date		8 March 2012
*  @version		1.0
*/
ATL_Sint32 host_int_get_rx_power_level(ATWILC_WFIDrvHandle hWFIDrv, ATL_Uint8* pu8RxPowerLevel, 
										   ATL_Uint32 u32RxPowerLevelLen);

/**
*  @brief 		sets a channel
*  @details 	 	
*  @param[in,out] handle to the wifi driver, 
*  @param[in]	Index of the channel to be set		
|-------------------------------------------------------------------| 
|          CHANNEL1      CHANNEL2 ....		             CHANNEL14	|
|  Input:         1             2					            14	|
|-------------------------------------------------------------------|
*  @return 		Error code indicating success/failure
*  @note 		
*  @author		zsalah
*  @date		8 March 2012
*  @version		1.0
*/
ATL_Sint32 host_int_set_mac_chnl_num(ATWILC_WFIDrvHandle hWFIDrv,ATL_Uint8 u8ChNum);

/**
*  @brief 		gets the current channel index
*  @details 	 	
*  @param[in,out] handle to the wifi driver,
				current channel index
|-----------------------------------------------------------------------| 
|          CHANNEL1      CHANNEL2 ....                     CHANNEL14	|
|  Input:         1             2                                 14	|
|-----------------------------------------------------------------------|
*  @return 		Error code indicating success/failure
*  @note 		
*  @author		zsalah
*  @date		8 March 2012
*  @version		1.0
*/
ATL_Sint32 host_int_get_host_chnl_num(ATWILC_WFIDrvHandle hWFIDrv, ATL_Uint8* pu8ChNo);
/**
*  @brief 		gets the sta rssi
*  @details 	gets the currently maintained RSSI value for the station. 
				The received signal strength value in dB. 
				The range of valid values is -128 to 0.
*  @param[in,out] handle to the wifi driver,
				rssi value in dB
*  @return 		Error code indicating success/failure
*  @note 		
*  @author		zsalah
*  @date		8 March 2012
*  @version		1.0
*/
ATL_Sint32 host_int_get_rssi(ATWILC_WFIDrvHandle hWFIDrv, ATL_Sint8* ps8Rssi);
ATL_Sint32 host_int_get_link_speed(ATWILC_WFIDrvHandle hWFIDrv, ATL_Sint8* ps8lnkspd);
/**
*  @brief 		scans a set of channels
*  @details 	
*  @param[in,out] handle to the wifi driver,
*  @param[in]		Scan source
				Scan Type	PASSIVE_SCAN = 0,							
							ACTIVE_SCAN  = 1
				Channels Array
				Channels Array length
				Scan Callback function
				User Argument to be delivered back through the Scan Cllback function
*  @return 		Error code indicating success/failure
*  @note 		
*  @author		zsalah
*  @date		8 March 2012
*  @version		1.0
*/
ATL_Sint32 host_int_scan(ATWILC_WFIDrvHandle hWFIDrv, ATL_Uint8 u8ScanSource,
					     ATL_Uint8 u8ScanType, ATL_Uint8* pu8ChnlFreqList,
					     ATL_Uint8 u8ChnlListLen, const ATL_Uint8* pu8IEs,
					     size_t IEsLen, tATWILCpfScanResult ScanResult,
					     void* pvUserArg,tstrHiddenNetwork  *pstrHiddenNetwork);
/**
*  @brief 		sets configuration wids values
*  @details 	
*  @param[in,out] handle to the wifi driver,
*  @param[in]	WID, WID value				
*  @return 		Error code indicating success/failure
*  @note 		
*  @author		zsalah
*  @date		8 March 2012
*  @version		1.0
*/
ATL_Sint32 hif_set_cfg(ATWILC_WFIDrvHandle hWFIDrv, tstrCfgParamVal * pstrCfgParamVal);

/**
*  @brief 		gets configuration wids values
*  @details 	
*  @param[in,out] handle to the wifi driver, 
				WID value
*  @param[in]	WID, 				
*  @return 		Error code indicating success/failure
*  @note 		
*  @author		zsalah
*  @date		8 March 2012
*  @version		1.0
*/
ATL_Sint32 hif_get_cfg(ATWILC_WFIDrvHandle hWFIDrv,ATL_Uint16 u16WID,ATL_Uint16* pu16WID_Value);
/*****************************************************************************/
/*							Notification Functions							 */
/*****************************************************************************/
/**
*  @brief 		notifies host with join and leave requests
*  @details 	This function prepares an Information frame having the
				information about a joining/leaving station.
*  @param[in,out] handle to the wifi driver,
*  @param[in]	6 byte Sta Adress
				Join or leave flag:
				Join = 1,
				Leave =0
*  @return 		Error code indicating success/failure
*  @note 		
*  @author		zsalah
*  @date		8 March 2012
*  @version		1.0
*/
void host_int_send_join_leave_info_to_host
						(ATL_Uint16 assocId, ATL_Uint8* stationAddr, ATL_Bool joining);

/**
*  @brief 		notifies host with stations found in scan
*  @details 	sends the beacon/probe response from scan
*  @param[in,out] handle to the wifi driver,
*  @param[in]	Sta Address,
				Frame length,
				Rssi of the Station found
*  @return 		Error code indicating success/failure
*  @note 		
*  @author		zsalah
*  @date		8 March 2012
*  @version		1.0
*/
void host_int_send_network_info_to_host
			(ATL_Uint8 *macStartAddress,ATL_Uint16 u16RxFrameLen, ATL_Sint8 s8Rssi);

/**
*  @brief 		host interface initialization function
*  @details 	
*  @param[in,out] handle to the wifi driver,
*  @note 		
*  @author		zsalah
*  @date		8 March 2012
*  @version		1.0
*/
ATL_Sint32 host_int_init(ATWILC_WFIDrvHandle* phWFIDrv);

/**
*  @brief 		host interface initialization function
*  @details 	
*  @param[in,out] handle to the wifi driver,
*  @note 		
*  @author		zsalah
*  @date		8 March 2012
*  @version		1.0
*/
ATL_Sint32 host_int_deinit(ATWILC_WFIDrvHandle hWFIDrv);


/*!
 *  @fn		ATL_Sint32 host_int_add_beacon(ATWILC_WFIDrvHandle hWFIDrv,ATL_Uint8 u8Index)
 *  @brief		Sends a beacon to the firmware to be transmitted over the air
 *  @details	
 *  @param[in,out]	hWFIDrv		handle to the wifi driver
 *  @param[in]	u32Interval	Beacon Interval. Period between two successive beacons on air
 *  @param[in]	u32DTIMPeriod DTIM Period. Indicates how many Beacon frames
 * 		(including the current frame) appear before the next DTIM	 
 *  @param[in]	u32Headlen	Length of the head buffer in bytes
 *  @param[in]	pu8Head		Pointer to the beacon's head buffer. Beacon's head
 *		is the part from the beacon's start till the TIM element, NOT including the TIM
 *  @param[in]	u32Taillen	Length of the tail buffer in bytes
 *  @param[in]	pu8Tail		Pointer to the beacon's tail buffer. Beacon's tail
 *		starts just after the TIM inormation element
 *  @return	0 for Success, error otherwise
 *  @todo
 *  @sa
 *  @author		Adham Abozaeid
 *  @date		10 Julys 2012
 *  @version		1.0 Description

 */
ATL_Sint32 host_int_add_beacon(ATWILC_WFIDrvHandle hWFIDrv, ATL_Uint32 u32Interval,
									 ATL_Uint32 u32DTIMPeriod,
									 ATL_Uint32 u32HeadLen, ATL_Uint8* pu8Head,
									 ATL_Uint32 u32TailLen, ATL_Uint8* pu8tail);


/*!
 *  @fn		ATL_Sint32 host_int_del_beacon(ATWILC_WFIDrvHandle hWFIDrv)
 *  @brief		Removes the beacon and stops transmitting it over the air
 *  @details	
 *  @param[in,out]	hWFIDrv		handle to the wifi driver
 *  @return	0 for Success, error otherwise
 *  @todo
 *  @sa
 *  @author		Adham Abozaeid
 *  @date		10 Julys 2012
 *  @version		1.0 Description
 */
ATL_Sint32 host_int_del_beacon(ATWILC_WFIDrvHandle hWFIDrv);

/*!
 *  @fn		ATL_Sint32 host_int_add_station(ATWILC_WFIDrvHandle hWFIDrv, tstrATWILC_AddStaParam strStaParams)
 *  @brief		Notifies the firmware with a new associated stations
 *  @details	
 *  @param[in,out]	hWFIDrv		handle to the wifi driver
 *  @param[in]	pstrStaParams	Station's parameters
 *  @return	0 for Success, error otherwise
 *  @todo
 *  @sa
 *  @author		Adham Abozaeid
 *  @date		12 July 2012
 *  @version		1.0 Description
 */
ATL_Sint32 host_int_add_station(ATWILC_WFIDrvHandle hWFIDrv, tstrATWILC_AddStaParam* pstrStaParams);
/*!
 *  @fn		ATL_Sint32 host_int_del_allstation(ATWILC_WFIDrvHandle hWFIDrv, ATL_Uint8* pu8MacAddr)
 *  @brief		Deauthenticates clients when group is terminating
 *  @details	
 *  @param[in,out]	hWFIDrv		handle to the wifi driver
 *  @param[in]	pu8MacAddr	Station's mac address
 *  @return	0 for Success, error otherwise
 *  @todo
 *  @sa
 *  @author		Mai Daftedar
 *  @date		09 April 2014
 *  @version		1.0 Description
 */
ATL_Sint32 host_int_del_allstation(ATWILC_WFIDrvHandle hWFIDrv, ATL_Uint8 pu8MacAddr[][ETH_ALEN]);
/*!
 *  @fn		ATL_Sint32 host_int_del_station(ATWILC_WFIDrvHandle hWFIDrv, ATL_Uint8* pu8MacAddr)
 *  @brief		Notifies the firmware with a new deleted station
 *  @details	
 *  @param[in,out]	hWFIDrv		handle to the wifi driver
 *  @param[in]	pu8MacAddr	Station's mac address
 *  @return	0 for Success, error otherwise
 *  @todo
 *  @sa
 *  @author		Adham Abozaeid
 *  @date		15 July 2012
 *  @version		1.0 Description
 */
ATL_Sint32 host_int_del_station(ATWILC_WFIDrvHandle hWFIDrv, ATL_Uint8* pu8MacAddr);

/*!
 *  @fn		ATL_Sint32 host_int_edit_station(ATWILC_WFIDrvHandle hWFIDrv, tstrATWILC_AddStaParam strStaParams)
 *  @brief		Notifies the firmware with new parameters of an already associated station
 *  @details	
 *  @param[in,out]	hWFIDrv		handle to the wifi driver
 *  @param[in]	pstrStaParams	Station's parameters
 *  @return	0 for Success, error otherwise
 *  @todo
 *  @sa
 *  @author		Adham Abozaeid
 *  @date		15 July 2012
 *  @version		1.0 Description
 */
ATL_Sint32 host_int_edit_station(ATWILC_WFIDrvHandle hWFIDrv, tstrATWILC_AddStaParam* pstrStaParams);

/*!
 *  @fn		ATL_Sint32 host_int_set_power_mgmt(ATWILC_WFIDrvHandle hWFIDrv, ATL_Bool bIsEnabled, ATL_Uint32 u32Timeout)
 *  @brief		Set the power management mode to enabled or disabled
 *  @details	
 *  @param[in,out]	hWFIDrv		handle to the wifi driver
 *  @param[in]	bIsEnabled	TRUE if enabled, FALSE otherwise
 *  @param[in]	u32Timeout	A timeout value of -1 allows the driver to adjust
 *							the dynamic ps timeout value
 *  @return	0 for Success, error otherwise
 *  @todo
 *  @sa
 *  @author		Adham Abozaeid
 *  @date		24 November 2012
 *  @version		1.0 Description
 */
ATL_Sint32 host_int_set_power_mgmt(ATWILC_WFIDrvHandle hWFIDrv, ATL_Bool bIsEnabled, ATL_Uint32 u32Timeout);

/*!
 *  @fn		ATL_Sint32 host_int_setup_multicast_filter(ATWILC_WFIDrvHandle hWFIDrv, ATL_Bool bIsEnabled, ATL_Uint8 u8count)
 *  @brief		Set the multicast filter paramters
 *  @details	
 *  @param[in,out]	hWFIDrv		handle to the wifi driver
 *  @param[in]	bIsEnabled	TRUE if enabled, FALSE otherwise
 *  @param[in]	u8count		count of mac address entries in the filter table
 *
 *  @return	0 for Success, error otherwise
 *  @todo
 *  @sa
 *  @author		Adham Abozaeid
 *  @date		24 November 2012
 *  @version		1.0 Description
 */
ATL_Sint32 host_int_setup_multicast_filter(ATWILC_WFIDrvHandle hWFIDrv, ATL_Bool bIsEnabled, ATL_Uint32 u32count);
/**
*  @brief           host_int_setup_ipaddress
*  @details 	   set IP address on firmware
*  @param[in]    
*  @return 	    Error code.
*  @author		Abdelrahman Sobhy
*  @date	
*  @version	1.0
*/
ATL_Sint32 host_int_setup_ipaddress(ATWILC_WFIDrvHandle hWFIDrv, ATL_Uint8* pu8IPAddr, ATL_Uint8 idx);


/**
*  @brief           host_int_delBASession
*  @details 	   Delete single Rx BA session
*  @param[in]    
*  @return 	    Error code.
*  @author		Abdelrahman Sobhy
*  @date	
*  @version	1.0
*/
ATL_Sint32 host_int_delBASession(ATWILC_WFIDrvHandle hWFIDrv, char* pBSSID,char TID);

/**
*  @brief           host_int_delBASession
*  @details 	   Delete all Rx BA session
*  @param[in]    
*  @return 	    Error code.
*  @author		Abdelrahman Sobhy
*  @date	
*  @version	1.0
*/
ATL_Sint32 host_int_del_All_Rx_BASession(ATWILC_WFIDrvHandle hWFIDrv, char* pBSSID,char TID);


/**
*  @brief           host_int_get_ipaddress
*  @details 	   get IP address on firmware
*  @param[in]    
*  @return 	    Error code.
*  @author		Abdelrahman Sobhy
*  @date	
*  @version	1.0
*/
ATL_Sint32 host_int_get_ipaddress(ATWILC_WFIDrvHandle hWFIDrv, ATL_Uint8* pu8IPAddr, ATL_Uint8 idx);

#ifdef ATWILC_P2P
/**
*  @brief           host_int_remain_on_channel
*  @details 	   
*  @param[in]    
*  @return 	    Error code.
*  @author	
*  @date	
*  @version	1.0
*/
ATL_Sint32 host_int_remain_on_channel(ATWILC_WFIDrvHandle hWFIDrv, ATL_Uint32 u32SessionID, ATL_Uint32 u32duration,ATL_Uint16 chan,tATWILCpfRemainOnChanExpired RemainOnChanExpired, tATWILCpfRemainOnChanReady RemainOnChanReady,void* pvUserArg);

/**
*  @brief           	host_int_ListenStateExpired
*  @details 	   
*  @param[in]    	Handle to wifi driver
				Duration to remain on channel
				Channel to remain on
				Pointer to fn to be called on receive frames in listen state
				Pointer to remain-on-channel expired fn
				Priv
*  @return 	    	Error code.
*  @author	
*  @date	
*  @version		1.0
*/
ATL_Sint32 host_int_ListenStateExpired(ATWILC_WFIDrvHandle hWFIDrv, ATL_Uint32 u32SessionID);

/**
*  @brief           host_int_frame_register
*  @details 	   
*  @param[in]    
*  @return 	    Error code.
*  @author	
*  @date	
*  @version	1.0
*/
ATL_Sint32 host_int_frame_register(ATWILC_WFIDrvHandle hWFIDrv, ATL_Uint16 u16FrameType,ATL_Bool bReg);
#endif
/**
*  @brief           host_int_set_wfi_drv_handler
*  @details 	   
*  @param[in]    
*  @return 	    Error code.
*  @author	
*  @date	
*  @version	1.0
*/
ATL_Sint32 host_int_set_wfi_drv_handler(ATL_Uint32 u32address);
ATL_Sint32 host_int_set_operation_mode(ATWILC_WFIDrvHandle hWFIDrv, ATL_Uint32 u32mode);

#ifdef ATWILC_BT_COEXISTENCE
ATL_Sint32 host_int_change_bt_coex_mode(ATWILC_WFIDrvHandle hWFIDrv,ATL_Uint8 u8BtCoexMode);
#endif

ATL_Sint32 Handle_ScanDone(void* drvHandler,tenuScanEvent enuEvent);

static int host_int_addBASession(ATWILC_WFIDrvHandle hWFIDrv, char* pBSSID,char TID,short int BufferSize,
	short int SessionTimeout,void * drvHandler);


void host_int_freeJoinParams(void* pJoinParams);

ATL_Sint32 host_int_get_statistics(ATWILC_WFIDrvHandle hWFIDrv, tstrStatistics* pstrStatistics);

/*****************************************************************************/
/*																			 */
/*									EOF										 */
/*																			 */
/*****************************************************************************/
#endif 

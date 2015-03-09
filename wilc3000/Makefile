obj-$(CONFIG_WILC3000) += wilc3000.o 

WILC3000_STA_FW_VERSION = STA_FIRMWARE=\"wifi_firmware.bin\"
WILC3000_AP_FW_VERSION = AP_FIRMWARE=\"wifi_firmware_ap.bin\"
WILC3000_P2P_FIRMWARE = P2P_CONCURRENCY_FIRMWARE=\"wifi_firmware_p2p_concurrency.bin\"
#WILC3000_BT_FIRMWARE = BT_FIRMWARE=\"bt_fw_BR921600_FC_Off.bin\"
#WILC3000_BT_FIRMWARE = BT_FIRMWARE=\"bt_fw_BR921600_FC_13_21.bin\"
WILC3000_BT_FIRMWARE = BT_FIRMWARE=\"bt_fw_BR1375000_FC_13_21.bin\"

ccflags-y += -D$(WILC3000_STA_FW_VERSION)
ccflags-y += -D$(WILC3000_AP_FW_VERSION)
ccflags-y += -D$(WILC3000_P2P_FIRMWARE)
ccflags-y += -D$(WILC3000_BT_FIRMWARE)

ccflags-y += -DSAMA5D4

WILC3000_SRC_VERSION = 4.2
PATCHLEVEL = 2
WILC3000_FW_VERSION = 0

ccflags-y += -D__DRIVER_VERSION__=\"$(WILC3000_SRC_VERSION).$(PATCHLEVEL)\"
ccflags-y += -D__FW_VERSION__=\"$(WILC3000_SRC_VERSION).$(WILC3000_FW_VERSION)\"

ccflags-$(CONFIG_WILC3000_SDIO) += -DWILC_SDIO
ccflags-$(CONFIG_WILC3000_SPI) += -DWILC_SPI
ccflags-$(CONFIG_WILC3000_HW_OOB_INTR) += -DWILC_SDIO_IRQ_GPIO
ccflags-$(CONFIG_WILC3000_STATICALLY_ALLOCATE_MEMORY) += -DMEMORY_STATIC
ccflags-$(CONFIG_WILC3000_ENABLE_DEBUGFS) += -DWILC_DEBUGFS

ccflags-y += -DWILC_ASIC_A0 -DPLL_WORKAROUND -DCONNECT_DIRECT -DWILC_PARSE_SCAN_IN_HOST -DTCP_ACK_FILTER -DAGING_ALG -DDISABLE_PWRSAVE_AND_SCAN_DURING_IP \
			-DWILC_PLATFORM=WILC_LINUXKERNEL  -Wno-unused-function -DUSE_WIRELESS -DWILC1000_BT_COEXISTENCE \

wilc3000-y += wilc_wfi_netdevice.o
wilc3000-y += wilc_wfi_cfgoperations.o
wilc3000-y += linux_wlan.o
wilc3000-y += linux_mon.o
wilc3000-y += linus_os_wrapper.o
wilc3000-y += at_pwr_dev.o
wilc3000-$(CONFIG_WILC3000_SDIO) += linux_wlan_sdio.o
wilc3000-$(CONFIG_WILC3000_SPI) += linux_wlan_spi.o
wilc3000-y += wilc_memory.o
wilc3000-y += wilc_msgqueue.o
wilc3000-y += wilc_semaphore.o
wilc3000-y += wilc_sleep.o
wilc3000-y += wilc_strutils.o
wilc3000-y += wilc_thread.o
wilc3000-y += wilc_time.o
wilc3000-y += wilc_timer.o
wilc3000-y += coreconfigurator.o
wilc3000-y += host_interface.o
wilc3000-y += wilc_sdio.o
wilc3000-y += wilc_spi.o
wilc3000-y += wilc_wlan_cfg.o
wilc3000-y += wilc_wlan.o
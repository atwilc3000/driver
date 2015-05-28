obj-$(CONFIG_WILC3000) += wilc3000.o 

WILC3000_STA_FW_VERSION = STA_FIRMWARE=\"wifi_firmware.bin\"
WILC3000_AP_FW_VERSION = AP_FIRMWARE=\"wifi_firmware_ap.bin\"
WILC3000_P2P_FIRMWARE = P2P_CONCURRENCY_FIRMWARE=\"wifi_firmware_p2p_concurrency.bin\"
WILC3000_BT_FIRMWARE = BT_FIRMWARE=\"bt_firmware.bin\"

TOP := $(dir $(lastword $(MAKEFILE_LIST)))

ccflags-y += -D$(WILC3000_STA_FW_VERSION)
ccflags-y += -D$(WILC3000_AP_FW_VERSION)
ccflags-y += -D$(WILC3000_P2P_FIRMWARE)
ccflags-y += -D$(WILC3000_BT_FIRMWARE)

ccflags-y += -DPLAT_SAMA5D4

WILC3000_SRC_VERSION = 6.0
PATCHLEVEL = 0
WILC3000_FW_VERSION = 0

ccflags-y += -D__DRIVER_VERSION__=\"$(WILC3000_SRC_VERSION).$(PATCHLEVEL)\"
ccflags-y += -D__FW_VERSION__=\"$(WILC3000_SRC_VERSION).$(WILC3000_FW_VERSION)\"

ccflags-$(CONFIG_WILC3000_SDIO) += -DATWILC_SDIO
ccflags-$(CONFIG_WILC3000_SPI) += -DATWILC_SPI
ccflags-$(CONFIG_WILC3000_HW_OOB_INTR) += -DATWILC_SDIO_IRQ_GPIO
ccflags-$(CONFIG_WILC3000_STATICALLY_ALLOCATE_MEMORY) += -DMEMORY_STATIC
ccflags-$(CONFIG_WILC3000_ENABLE_DEBUGFS) += -DATWILC_DEBUGFS

#ccflags-y += -DDOWNLOAD_BT_FW

#ccflags-y += -DATWILC_DISABLE_PMU
#ccflags-y += -DATWILC_SLEEP_CLK_SRC_XO
#ccflags-y += -DATWILC_EXT_PA_INV_TX_RX

#sama5d4-xplained should not set
#ccflags-y += -DATWILC_SLEEP_CLK_SRC_RTC

ccflags-y += -DATWILC_ASIC_A0 -DCONNECT_DIRECT -DATWILC_PARSE_SCAN_IN_HOST -DAGING_ALG -DDISABLE_PWRSAVE_AND_SCAN_DURING_IP \
			-DATL_PLATFORM=ATL_LINUXKERNEL -Wno-unused-function -DUSE_WIRELESS -DATWILC_BT_COEXISTENCE -DDOWNLOAD_BT_FW\ 

wilc3000-y += at_pwr_dev_main.o
wilc3000-y += atwilc_debugfs.o
wilc3000-y += atwilc_queue.o
wilc3000-y += atwilc_sdio.o
wilc3000-y += atwilc_spi.o
wilc3000-y += linux_os_wrapper.o
wilc3000-$(CONFIG_WILC3000_SDIO) += linux_wlan_sdio.o
wilc3000-$(CONFIG_WILC3000_SPI) += linux_wlan_spi.o

wilc3000-y += atl_memory.o
wilc3000-y += atl_msg_queue.o
wilc3000-y += atl_semaphore.o
wilc3000-y += atl_sleep.o
wilc3000-y += atl_str_utils.o
wilc3000-y += atl_thread.o
wilc3000-y += atl_time.o
wilc3000-y += atl_timer.o
wilc3000-y += atwilc_host_ap.o 
wilc3000-y += atwilc_wfi_cfg_operations.o
wilc3000-y += atwilc_wfi_netdevice.o
wilc3000-y += atwilc_wlan.o
wilc3000-y += atwilc_wlan_cfg.o
wilc3000-y += core_configurator.o
wilc3000-y += fifo_buffer.o
wilc3000-y += host_interface.o
wilc3000-y += linux_mon.o
wilc3000-y += linux_wlan.o
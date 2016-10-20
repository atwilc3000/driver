obj-$(CONFIG_WILC1000) += wilc1000.o

ccflags-$(CONFIG_WILC1000_SDIO) += -DWILC_SDIO 

ccflags-$(CONFIG_WILC1000_HW_OOB_INTR) += -DWILC_SDIO_IRQ_GPIO
ccflags-$(CONFIG_WILC1000_SPI) += -DWILC_SPI

ccflags-y += -I$(src)/ -DEXPORT_SYMTAB  -D__CHECK_ENDIAN__ -Wno-unused-function

ccflags-y += -DCONNECT_DIRECT -DWILC_PARSE_SCAN_IN_HOST
ccflags-y += -DAGING_ALG -DDISABLE_PWRSAVE_AND_SCAN_DURING_IP
ccflags-y += -DSAMA5D4_BOARD
ccflags-y += -DTCP_ACK_FILTER
ccflags-y += -DWILC_OPTIMIZE_SLEEP_INT
#EXTRA_CFLAGS += -DANT_SWTCH_SNGL_GPIO_CTRL
#EXTRA_CFLAGS += -DANT_SWTCH_DUAL_GPIO_CTRL
#EXTRA_CFLAGS += -DANT_1_GPIO_NUM=4
#EXTRA_CFLAGS += -DANT_2_GPIO_NUM=6

ccflags-$(CONFIG_WILC1000_PREALLOCATE_DURING_SYSTEM_BOOT) += -DMEMORY_STATIC \
								-DWILC_PREALLOC_AT_BOOT

ccflags-$(CONFIG_WILC1000_PREALLOCATE_AT_LOADING_DRIVER) += -DMEMORY_STATIC \
								-DWILC_PREALLOC_AT_INSMOD

ccflags-$(CONFIG_WILC1000_DYNAMICALLY_ALLOCATE_MEMROY) += -DWILC_NORMAL_ALLOC


wilc1000-objs := wilc_wfi_cfgoperations.o linux_wlan.o linux_mon.o \
			wilc_msgqueue.o coreconfigurator.o host_interface.o wilc_sdio.o wilc_spi.o \
			wilc_wlan_cfg.o wilc_wlan.o

wilc1000-$(CONFIG_WILC1000_SDIO) += linux_wlan_sdio.o
wilc1000-$(CONFIG_WILC1000_SPI) += linux_wlan_spi.o

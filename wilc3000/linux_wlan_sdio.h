#ifndef LINUX_WLAN_SDIO_H
#define LINUX_WLAN_SDIO_H

#ifdef ATWILC_SDIO
extern struct sdio_func *local_sdio_func;
extern struct sdio_driver atwilc_bus;
#endif //ATWILC_SDIO

#include <linux/mmc/sdio_func.h>
#include "atwilc_type.h"

typedef struct {
	uint32_t read_write:1;
	uint32_t function:3;
	uint32_t raw:1;
	uint32_t address:17;
	uint32_t data:8;
} sdio_cmd52_t;

typedef struct {
	//struct {
		uint32_t read_write:1;
		uint32_t function:3;
		uint32_t block_mode:1;
		uint32_t increment:1;
		uint32_t address:17;
		uint32_t count:9;
	//} bit;
	uint8_t *buffer;
	uint32_t block_size;
} sdio_cmd53_t;

typedef void (*isr_handler_t)(void);
int linux_sdio_init(void*);
void linux_sdio_deinit(void*);
int linux_sdio_cmd52(sdio_cmd52_t *cmd);
int linux_sdio_cmd53(sdio_cmd53_t *cmd);
int enable_sdio_interrupt(isr_handler_t isr_handler);
void disable_sdio_interrupt(void);
int linux_sdio_set_default_speed(void);

#endif //LINUX_WLAN_SDIO_H

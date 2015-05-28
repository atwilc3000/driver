////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) Atmel Corp.  All rights reserved.
//
// Module Name:  atwilc_wlan_cfg.h
//
//
//////////////////////////////////////////////////////////////////////////////

#ifndef ATWILC_WLAN_CFG_H
#define ATWILC_WLAN_CFG_H

typedef struct {
	uint16_t id;
	uint16_t val;
} atwilc_cfg_byte_t;

typedef struct {
	uint16_t id;
	uint16_t val;
} atwilc_cfg_hword_t;

typedef struct {
	uint32_t id;
	uint32_t val;
} atwilc_cfg_word_t;

typedef struct {
	uint32_t id;
	uint8_t *str;
} atwilc_cfg_str_t;

#endif

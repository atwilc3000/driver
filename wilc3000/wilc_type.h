////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) Atmel Corporation.  All rights reserved.
//
// Module Name:  wilc_type.h
//
//
//////////////////////////////////////////////////////////////////////////////
#ifndef WILC_TYPE_H
#define WILC_TYPE_H

/********************************************

	Type Defines

********************************************/
#ifdef  WIN32
typedef char 	int8_t; 
typedef short	int16_t;
typedef long 	int32_t;
typedef unsigned char 	uint8_t;
typedef unsigned short 	uint16_t;
typedef unsigned long 	uint32_t;
#else
#ifdef _linux_
/*typedef unsigned char 	uint8_t;
typedef unsigned short 	uint16_t;
typedef unsigned long 	uint32_t;*/
#include <stdint.h>
#else
#include "wilc_oswrapper.h"
#endif

/* Bit Values */
#define BIT31                   ((uint32_t)(1 << 31))
#define BIT30                   ((uint32_t)(1 << 30))
#define BIT29                   ((uint32_t)(1 << 29))
#define BIT28                   ((uint32_t)(1 << 28))
#define BIT27                   ((uint32_t)(1 << 27))
#define BIT26                   ((uint32_t)(1 << 26))
#define BIT25                   ((uint32_t)(1 << 25))
#define BIT24                   ((uint32_t)(1 << 24))
#define BIT23                   ((uint32_t)(1 << 23))
#define BIT22                   ((uint32_t)(1 << 22))
#define BIT21                   ((uint32_t)(1 << 21))
#define BIT20                   ((uint32_t)(1 << 20))
#define BIT19                   ((uint32_t)(1 << 19))
#define BIT18                   ((uint32_t)(1 << 18))
#define BIT17                   ((uint32_t)(1 << 17))
#define BIT16                   ((uint32_t)(1 << 16))
#define BIT15                   ((uint32_t)(1 << 15))
#define BIT14                   ((uint32_t)(1 << 14))
#define BIT13                   ((uint32_t)(1 << 13))
#define BIT12                   ((uint32_t)(1 << 12))
#define BIT11                   ((uint32_t)(1 << 11))
#define BIT10                   ((uint32_t)(1 << 10))
#define BIT9                    ((uint32_t)(1 << 9))
#define BIT8                    ((uint32_t)(1 << 8))
#define BIT7                    ((uint32_t)(1 << 7))
#define BIT6                    ((uint32_t)(1 << 6))
#define BIT5                    ((uint32_t)(1 << 5))
#define BIT4                    ((uint32_t)(1 << 4))
#define BIT3                    ((uint32_t)(1 << 3))
#define BIT2                    ((uint32_t)(1 << 2))
#define BIT1                    ((uint32_t)(1 << 1))
#define BIT0                    ((uint32_t)(1 << 0))
#define ALL                     0xFFFF

#endif
#endif

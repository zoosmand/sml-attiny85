/*
 * Filename: def.h
 * Description: A set of common definitions. 
 *
 * Project: Simple Multitasking Logic
 * Platform: MicroChip ATTiny85
 * Created: 02.08.2025 9:53:17 AM
 * Author: Dmitry Slobodchikov
*/ 
#ifndef DEF_H_
#define DEF_H_


#include "main.h"


#define SYS_TICK_THOLD  194
#define SEC_TICK_MASK   0x03ff


/* Flags definitions */
#define _SYSTF_   0 // System Tick Flag
#define _SECTF_   1 // Seconds Tick Flag
#define _DF_      2 // Delay Flag


#endif /* DEF_H_ */

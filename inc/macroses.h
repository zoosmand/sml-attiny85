/*
 * Filename: macroses.h
 * Description: A set of common macroses. 
 *
 * Project: Simple Multitasking Logic
 * Platform: MicroChip ATTiny85
 * Created: 02.08.2025 9:53:17 AM
 * Author : Dmitry Slobodchikov
*/
#ifndef MACROSES_H_
#define MACROSES_H_


#include "main.h"


/* Flag management macroses */
#define FLAG_SET(reg, flag)     reg |= _BV(flag)
#define FLAG_CLR(reg, flag)     reg &= ~_BV(flag)
#define FLAG_CHECK(reg, flag)   (reg & _BV(flag))


/* No operation masros */
#define _NOP do {__asm__ __volatile__ ("nop");} while(0)



#endif /* MACROSES_H_ */

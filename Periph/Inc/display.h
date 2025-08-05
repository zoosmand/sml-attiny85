/*
 * Filename: display.h
 * Description: The file contains display definitions.
 *
 * Project: Simple Multitasking Logic
 * Platform: MicroChip ATTiny85
 * Created: 03.08.2025 10:38:01 AM
 * Author : Dmitry Slobodchikov
 */

#ifndef _DISPLAY_H
#define _DISPLAY_H

#include "main.h"
   
/* Exported functions prototypes */
void WH1602_I2C_Init(void);
void WH1602_I2C_Write(uint8_t, uint8_t, const char*);
void WH1602_I2C_Read(uint16_t, uint8_t*);
int putc_dspl(char, FILE*);
// int uart_putchar(char c, FILE *stream);

/* --- WH0802A commands --- */
#define _1602A_ADDR_        0x27 // WH1602 I2C Address
#define _1602A_8BBUS_       0x03 // 8-bit initial bus initialization
#define _1602A_CURUPLEFT_   0x02 // Cursor positioin up an left
#define	_1602A_4BBUS2L_     0x28 // 4-bit bus, LCD of 2 lines
// #define	_1602A_4BBUS2L_     0x20 // 4-bit bus, LCD of 1 line inverted
#define _1602A_DSPLSW_	    0x0c // Display on, cursor off, blink off
#define _1602A_CLRDSLP_     0x01 // Clear display
#define _1602A_1LS_         0x80 // Position at 1-st line, start 
#define _1602A_2LS_         0xc0 // Position ar 2-nd line, start
#define _1602A_NOCMD_       0x00 // No command

/* --- WH0802A control parameters --- */
#define _1602A_Bl           3
#define _1602A_E            2
#define _1602A_Rw 	        1
#define _1602A_Rs 	        0
#define _1602A_4BMASK_      0xf0

/* --- WH0802A control mascroses --- */
#define _WR1NCHAR(ch)       ((ch & _1602A_4BMASK_)|_BV(_1602A_E)|_BV(_1602A_Bl)|_BV(_1602A_Rs))
#define _WR2NCHAR(ch)       ((ch & _1602A_4BMASK_)|_BV(_1602A_Bl)|_BV(_1602A_Rs))
#define _WR1NCMD(cmd)       ((cmd & _1602A_4BMASK_)|_BV(_1602A_E)|_BV(_1602A_Bl))
#define _WR2NCMD(cmd)       ((cmd & _1602A_4BMASK_)|_BV(_1602A_Bl))

/* --- Display end of line parameters --- */
#define _0DCF_              0
#define _0ACF_              1


#endif // _DISPLAY_H
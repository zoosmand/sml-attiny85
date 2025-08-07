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

#define DSPL_SSD1315
// #define DSPL_WH1602
   
/* Exported functions prototypes */
void Init_Display(void);
int putc_dspl(char, FILE*);
void WH1602_Write(uint8_t, uint8_t, const char*);
uint8_t SSD1315_Write(uint8_t*, uint8_t*);


/* --- WH0802A commands --- */
#define _1602A_ADDR_        0x27 // WH1602 I2C Address
#define _1602A_8BBUS_       0x03 // 8-bit initial bus initialization
#define _1602A_CURUPLEFT_   0x02 // Cursor positioin up an left
#define	_1602A_4BBUS2L_     0x28 // 4-bit bus, LCD of 2 lines
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

/* --- SSD1315 commands --- */
#define _SSD1315_ADDR_      0x3c // SSD1315 I2C Address
#define _SSD1315_Co_        7 // Co bit
#define _SSD1315_DC_        6 // DC bit (1 - data, 0 - command)

/* --- Display end of line parameters --- */
#define _0DCF_              0
#define _0ACF_              1


#endif // _DISPLAY_H
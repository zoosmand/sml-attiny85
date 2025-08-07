/*
 * Filename: display.c
 * Description: The file contains display code.
 *
 * Project: Simple Multitasking Logic
 * Platform: MicroChip ATTiny85
 * Created: 03.08.2025 10:38:01 AM
 * Author: Dmitry Slobodchikov
 */

#include "display.h"

/* Private variables */
static volatile uint8_t _DSPLREG_ = 0;
static uint8_t diplPrintPos       = 0;
volatile static uint8_t* _i2creg;


/* Private function prototypes */
static uint16_t Calc_BufferLength(const char*);

#if defined(DSPL_WH1602)
  static void WH1602_I2C_Init(void);
  static void WH1602_WriteChar(uint8_t);
  static void WH1602_WriteCommand(uint8_t, uint16_t);
  // static void WH1602_I2C_ReadByte(uint8_t);
  // static void WH1602_I2C_Read(uint16_t, uint8_t*);
#endif

#if defined(DSPL_SSD1315)
  static uint8_t SSD1315_I2C_Init(void);
  static uint8_t SSD1315_WriteCommand(uint8_t);
  static uint8_t SSD1315_WriteData(uint8_t);
#endif


/* Private costants */
#if defined(DSPL_WH1602)
  const uint8_t wh1602InitParams[8] PROGMEM = {
    _1602A_8BBUS_,
    _1602A_8BBUS_,
    _1602A_8BBUS_,
    _1602A_CURUPLEFT_,
    _1602A_4BBUS2L_,
    _1602A_DSPLSW_,
    _1602A_CLRDSLP_,
    _1602A_1LS_
  };

  const uint16_t wh1602InitDelays[8] PROGMEM = {
    4100,
    100,
    40,
    1640,
    40,
    40,
    1640,
    40
  }; 
#endif 


#if defined(DSPL_SSD1315)
  const uint8_t ssd1315InitParams[24] PROGMEM = {
	  0xae,       // turn off oled panel
	  0xd5, 0x80, // set display clock divide ratio/oscillator frequency
	  0xa8, 0xfc, // set multiples ratio(1to64)
	  0xd3, 0x00, // set display offset, no offset
	  0x40,       // set start line address
	  0x8d, 0x14, // set charge pump enable/disable, disable
	  0xa1, 0xc8, // set segment re-map 127 to 0   a0:0 to seg127
	  0xda, 0x12, // set com pins hardware configuration
	  0x81, 0xcf, // set contrast control register
	  0xd9, 0xf1, // set pre-charge period
	  0xdb, 0x40, // set vcomh
	  0xa4, 0xa6,
	  0xaf,       // turn on oled panel
	  0x00        // set low column address
  };
#endif


/**
 * @brief  Writes/Sends a command to WH1602A display
 * @param  buf: pointer to the character/text buffer
 * @retval The buffer length 
 */
static uint16_t Calc_BufferLength(const char* buf) {
  uint16_t len = 0;
  while(*buf) {
    len++;
    buf++;
    if (*buf == 0x0a) break;
  }
  return len;
}


/**
 * @brief  Writes/Sends character to the given display
 * @param  ch: character to write
 * @param  stream: Standard stream
 * @retval (int) status
 */
int putc_dspl(char ch, FILE *stream){
  // if (ch == '\n') putc_dspl('\r', stream);
#if defined(DSPL_WH1602)
  I2C_WRITE;
  if ((FLAG_CHECK(_DSPLREG_, _0DCF_)) || (FLAG_CHECK(_DSPLREG_, _0ACF_))) {
    FLAG_CLR(_DSPLREG_, _0DCF_);
    FLAG_CLR(_DSPLREG_, _0ACF_);

    I2C_Start();
    I2C_SendAddress(_1602A_ADDR_);
    WH1602_WriteCommand(_1602A_CLRDSLP_, 1640);
    WH1602_WriteCommand(_1602A_1LS_, 40);
    I2C_Stop();
    diplPrintPos = 0;
  }
  if ((ch != 0x0a) && (ch != 0x0d)) {
    I2C_Start();
    I2C_SendAddress(_1602A_ADDR_);
    if (diplPrintPos > 15) {
      WH1602_WriteCommand(_1602A_2LS_, 40);
      diplPrintPos = 0;
    }
    WH1602_WriteChar(ch);
    I2C_Stop();
    diplPrintPos++;
  }
#endif

  if (ch == 0x0a) FLAG_SET(_DSPLREG_, _0DCF_);
  if (ch == 0x0d) FLAG_SET(_DSPLREG_, _0ACF_);

  return 0;
}



/**
 * @brief  Initializes display
 * @retval None
 */
void Init_Display(void) {
  _i2creg = Get_I2CREG();

#if defined(DSPL_WH1602)
  WH1602_I2C_Init();
#endif
#if defined(DSPL_SSD1315)
  if (SSD1315_I2C_Init()) {

    uint8_t cmd[6]= {
      0x21, 0x00, 0x05, 0x22, 0x00, 0x00
    };

    uint8_t buf[6]= {
      0b00111110,
      0b01000001,
      0b01000001,
      0b01000001,
      0b00100010,
      0b00000000
    };

    SSD1315_Write(cmd, buf);
  }
#endif
}




/* --- SSD1315 block --- */
#if defined(DSPL_SSD1315)

/**
 * @brief  Initializes WH1602A display
 * @retval None
 */
static uint8_t SSD1315_I2C_Init(void) {
 
  /* Initial delay according ssd1315 documentation */
  _delay_us(50000);
  I2C_WRITE;
  I2C_Start();
  _delay_us(48);
  I2C_SendAddress(_SSD1315_ADDR_);
  /* --- Control ACK on sending address --- */
  if (!FLAG_CHECK(*_i2creg, _I2C_ACKF_)) {
    FLAG_SET(*_i2creg, _I2C_BERF_);
    I2C_Stop();
    return 0;
  }
  /* --- Initialization commands --- */
  for (uint8_t i = 0; i < sizeof(ssd1315InitParams); i++) {
    if (!SSD1315_WriteCommand(pgm_read_byte(&ssd1315InitParams[i]))) {
      FLAG_SET(*_i2creg, _I2C_BERF_);
      I2C_Stop();
      return 0;
    }
  }
  I2C_Stop();
  return 1;
}


/**
 * @brief  Writes/Sends a command to SSD1315 display
 * @param  cmd: ssd1315 command
 * @retval (uint8_t) status of operation
 */
static uint8_t SSD1315_WriteCommand(uint8_t cmd) {
  /* --- Send control byte --- */
  uint8_t ctrl = 0;
  ctrl &= ~(_BV(_SSD1315_Co_)|_BV(_SSD1315_DC_));
  I2C_SendByte(ctrl);
  if (!FLAG_CHECK(*_i2creg, _I2C_ACKF_)) {
    return 0;
  }
  /* --- Send data byte --- */
  I2C_SendByte(cmd);
  if (!FLAG_CHECK(*_i2creg, _I2C_ACKF_)) {
    return 0;
  }
  return 1;
}


/**
 * @brief  Writes/Sends a data to SSD1315 display
 * @param  data: ssd1315 data byte
 * @retval (uint8_t) status of operation
 */
static uint8_t SSD1315_WriteData(uint8_t data) {
  /* --- Send control byte --- */
  uint8_t ctrl = 0;
  ctrl &= ~_BV(_SSD1315_Co_);
  ctrl |= _BV(_SSD1315_DC_);
  I2C_SendByte(ctrl);
  if (!FLAG_CHECK(*_i2creg, _I2C_ACKF_)) {
    return 0;
  }
  /* --- Send data byte --- */
  I2C_SendByte(data);
  if (!FLAG_CHECK(*_i2creg, _I2C_ACKF_)) {
    return 0;
  }
  return 1;
}


/**
 * @brief  Writes/Sends a text buffer to SSD1315 display
 * @param  buf: pointer to the character/text buffer
 * @retval (uint8_t) status of operation
 */
uint8_t SSD1315_Write(uint8_t* cmd, uint8_t* buf) {
  I2C_WRITE;
  I2C_Start();
  _delay_us(48);
  I2C_SendAddress(_SSD1315_ADDR_);
  /* --- Control ACK on sending address --- */
  if (!FLAG_CHECK(*_i2creg, _I2C_ACKF_)) {
    FLAG_SET(*_i2creg, _I2C_BERF_);
    I2C_Stop();
    return 0;
  }
  /* --- Send commands --- */
  
  for (uint8_t i = 0; i < 6; i++) {
    if (!SSD1315_WriteCommand(*(cmd++))) {
      FLAG_SET(*_i2creg, _I2C_BERF_);
      I2C_Stop();
      return 0;
    }
  }

  /* --- Send buffer data --- */
  // uint16_t len = Calc_BufferLength(buf);
  for (uint8_t i = 0; i < 6; i++) {
    if (!SSD1315_WriteData(*(buf++))) {
      FLAG_SET(*_i2creg, _I2C_BERF_);
      I2C_Stop();
      return 0;
    }
  }
  I2C_Stop();
  return 1;
}


#endif




/* --- WH1602A block --- */
#if defined(DSPL_WH1602)

/**
 * @brief  Initializes WH1602A display
 * @retval None
 */
void WH1602_I2C_Init(void) {
 
  /* Initial delay according 1602a documentation */
  _delay_us(15000);
 
  /* Initial parameter-delay pairs */
  I2C_WRITE;
  I2C_Start();
  I2C_SendAddress(_1602A_ADDR_);
  for(uint8_t i = 0; i < sizeof(wh1602InitParams); i++) {
    I2C_SendByte(_WR1NCMD(pgm_read_byte(&wh1602InitParams[i])));
    I2C_SendByte(_WR2NCMD(pgm_read_byte(&wh1602InitParams[i])));
    I2C_SendByte(_WR1NCMD(pgm_read_byte(&wh1602InitParams[i]) << 4));
    I2C_SendByte(_WR2NCMD(pgm_read_byte(&wh1602InitParams[i]) << 4));
    _delay_us(pgm_read_word(&wh1602InitDelays[i]));
  }
  I2C_Stop();
}


/**
 * @brief  Writes/Sends a character symbol to WH1602A display
 * @param  ch: ACSII character
 * @retval None
 */
void WH1602_WriteChar(uint8_t ch) {
  I2C_WRITE;
  I2C_SendByte(_WR1NCHAR(ch));
  I2C_SendByte(_WR2NCHAR(ch));
  I2C_SendByte(_WR1NCHAR(ch << 4));
  I2C_SendByte(_WR2NCHAR(ch << 4));
  _delay_us(40);
}


/**
 * @brief  Writes/Sends a command to WH1602A display
 * @param  cmd: 1602a command
 * @param  delay: command delay according documentation
 * @retval None
 */
void WH1602_WriteCommand(uint8_t cmd, uint16_t delay) {  
  I2C_WRITE;
  I2C_SendByte(_WR1NCMD(cmd));
  I2C_SendByte(_WR2NCMD(cmd));
  I2C_SendByte(_WR1NCMD(cmd << 4));
  I2C_SendByte(_WR2NCMD(cmd << 4));
  _delay_us(delay);
}


/**
 * @brief  Writes/Sends a text buffer to WH1602A display
 * @param  line: 1602a display line [1,2]
 * @param  extraCmd: extra 1602a command, e.g. clear display
 * @param  buf: pointer to the character/text buffer
 * @retval None
 */
void WH1602_Write(uint8_t line, uint8_t extraCmd, const char* buf) {
  
  I2C_WRITE;
  I2C_Start();
  I2C_SendAddress(_1602A_ADDR_);

  if (extraCmd) {
    WH1602_WriteCommand(extraCmd, 1640);
  }

  uint8_t dsplAddr = (line == 2) ? _1602A_2LS_ : _1602A_1LS_;
  WH1602_WriteCommand(dsplAddr, 40);
  
  uint16_t len = Calc_BufferLength(buf);
  
  for(uint16_t i = 0 ; i < len ; i++) {
    WH1602_WriteChar(*(buf++));
  }
  I2C_Stop();
}


// /**
//  * @brief  Reads a byte from WH1602A display
//  * @param  rxByte: received byte
//  * @retval None
//  */
// static void WH1602_I2C_ReadByte(uint8_t rxByte) {

// }


// /**
//  * @brief  Reads a text buffer from WH1602A display
//  * @param  bufLen: received buffer length
//  * @param  buf: pointer to the character/text buffer to receive
//  * @retval None
//  */
// static void WH1602_I2C_Read(uint16_t bufLen, uint8_t* buf) {

// }


#endif // End of WH1602A code
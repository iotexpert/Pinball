#include "LiquidCrystal_I2C.h"
#include <inttypes.h>
#include <project.h>

// commands
#define LCD_CLEARDISPLAY 0x01
#define LCD_RETURNHOME 0x02
#define LCD_ENTRYMODESET 0x04
#define LCD_DISPLAYCONTROL 0x08
#define LCD_CURSORSHIFT 0x10
#define LCD_FUNCTIONSET 0x20
#define LCD_SETCGRAMADDR 0x40
#define LCD_SETDDRAMADDR 0x80

// flags for display entry mode
#define LCD_ENTRYRIGHT 0x00
#define LCD_ENTRYLEFT 0x02
#define LCD_ENTRYSHIFTINCREMENT 0x01
#define LCD_ENTRYSHIFTDECREMENT 0x00

// flags for display on/off control
#define LCD_DISPLAYON 0x04
#define LCD_DISPLAYOFF 0x00
#define LCD_CURSORON 0x02
#define LCD_CURSOROFF 0x00
#define LCD_BLINKON 0x01
#define LCD_BLINKOFF 0x00

// flags for display/cursor shift
#define LCD_DISPLAYMOVE 0x08
#define LCD_CURSORMOVE 0x00
#define LCD_MOVERIGHT 0x04
#define LCD_MOVELEFT 0x00

// flags for function set
#define LCD_8BITMODE 0x10
#define LCD_4BITMODE 0x00
#define LCD_2LINE 0x08
#define LCD_1LINE 0x00
#define LCD_5x10DOTS 0x04
#define LCD_5x8DOTS 0x00

typedef enum BackLightStatus {
        OFF = 0x00,
        ON = 0x01 
} BackLightStatus;
       
typedef enum RsType {
    Command=0x0,
    Data=0x1
} RsType;

typedef enum DataPosition {
    Low,
    High
} DataPosition;
    
static void LiquidCrystal_I2C_sendByte(uint8_t, RsType);
static void LiquidCrystal_I2C_write4bits(uint8_t,RsType);
static void LiquidCrystal_I2C_expanderWrite(uint8_t);
static void LiquidCrystal_I2C_pulseEnable(uint8_t);

static uint8_t _Addr;
static uint8_t _displayfunction;
static uint8_t _displaycontrol;
static uint8_t _displaymode;
static uint8_t _cols;
static uint8_t _rows;

static BackLightStatus _backLightStatus = OFF;
static int _backLightBitPosition = 3;
#define _backLightBitMask (_backLightStatus << _backLightBitPosition )

static int _rwBitPosition = 1;
#define _rwBitMask (_rwBitPosition << _rwBitPosition)

static int _rsBitPosition = 0; 
#define _rsBitMask (_rsBitPosition << _rsBitPosition)

static int _eBitPosition = 2;
#define _eBitMask (0x01 << _eBitPosition)


static DataPosition _dataPosition = High;

#include <stdio.h>
char buff[128];


// When the display powers up, it is configured as follows:
//
// 1. Display clear
// 2. Function set: 
//    DL = 1; 8-bit interface data 
//    N = 0; 1-line display 
//    F = 0; 5x8 dot character font 
// 3. Display on/off control: 
//    D = 0; Display off 
//    C = 0; Cursor off 
//    B = 0; Blinking off 
// 4. Entry mode set: 
//    I/D = 1; Increment by 1
//    S = 0; No shift 
//
// Note, however, that resetting the Arduino doesn't reset the LCD, so we
// can't assume that its in that state when a sketch starts (and the
// LiquidCrystal constructor is called).

void LiquidCrystal_I2C_Start(uint8_t lcd_Addr,uint8_t lcd_cols,uint8_t lcd_rows)
{
  _Addr = lcd_Addr;
  _cols = lcd_cols;
  _rows = lcd_rows;
    
 
    I2C_Start();
    
	_displayfunction = LCD_4BITMODE | LCD_2LINE | LCD_5x8DOTS;
/*
    LiquidCrystal_I2C_begin(_cols, _rows,LCD_5x8DOTS);  
}

void LiquidCrystal_I2C_begin(uint8_t cols, uint8_t lines, uint8_t dotsize) {
    (void)cols;
    (void)dotsize;
    
    
	if (lines > 1) {
		_displayfunction |= LCD_2LINE;
	}
	_numlines = lines;
 */

	
	// SEE PAGE 45/46 FOR INITIALIZATION SPECIFICATION!
	// according to datasheet, we need at least 40ms after power rises above 2.7V
	// before sending commands. Arduino can turn on way befer 4.5V so we'll wait 50
	//CyDelayUs(50000); 
  
	// Now we pull both RS and R/W low to begin commands
    LiquidCrystal_I2C_expanderWrite(_backLightBitMask);	// reset expanderand turn backlight off (Bit 8 =1)
	CyDelay(1000);
        
  	//put the LCD into 4 bit mode
	// this is according to the hitachi HD44780 datasheet
	// figure 24, pg 46
	
	// we start in 8bit mode, try to set 4 bit mode
	LiquidCrystal_I2C_write4bits(0x03, Command);
	CyDelayUs(4500); // wait min 4.1ms
	
	// second try
	LiquidCrystal_I2C_write4bits(0x03, Command);
	CyDelayUs(4500); // wait min 4.1ms
	
	// third go!
	LiquidCrystal_I2C_write4bits(0x03, Command); 
	CyDelayUs(150);
	
	// finally, set to 4-bit interface
	LiquidCrystal_I2C_write4bits(0x02, Command); 


	// set # lines, font size, etc.
	LiquidCrystal_I2C_WriteControl(LCD_FUNCTIONSET | _displayfunction);  
	
	// turn the display on with no cursor or blinking default
	_displaycontrol = LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF;
	LiquidCrystal_I2C_DisplayOn();
	
	// clear it off
	LiquidCrystal_I2C_ClearDisplay();
	
	// Initialize to default text direction (for roman languages)
	_displaymode = LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT;
	
	// set the entry mode
	LiquidCrystal_I2C_WriteControl(LCD_ENTRYMODESET | _displaymode);
	
	LiquidCrystal_I2C_Home();
  
}

void LiquidCrystal_I2C_PutChar(char c)
{
    LiquidCrystal_I2C_WriteData(c);
}


/********** high level commands, for the user! */
void LiquidCrystal_I2C_ClearDisplay(){
	LiquidCrystal_I2C_WriteControl(LCD_CLEARDISPLAY);// clear display, set cursor position to zero
	CyDelayUs(2000);  // this command takes a long time!
}

void LiquidCrystal_I2C_Home(){
	LiquidCrystal_I2C_WriteControl(LCD_RETURNHOME);  // set cursor position to zero
	CyDelayUs(2000);  // this command takes a long time!
}

void LiquidCrystal_I2C_Position(uint8_t col, uint8_t row){
	int row_offsets[] = { 0x00, 0x40, 0x14, 0x54 };
	if ( row > _rows ) {
		row = _rows; 
	}
	LiquidCrystal_I2C_WriteControl(LCD_SETDDRAMADDR | (col + row_offsets[row]));
}

// Turn the display on/off (quickly)
void LiquidCrystal_I2C_DisplayOff() {
	_displaycontrol &= ~LCD_DISPLAYON;
	LiquidCrystal_I2C_WriteControl(LCD_DISPLAYCONTROL | _displaycontrol);
}
void LiquidCrystal_I2C_DisplayOn() {
	_displaycontrol |= LCD_DISPLAYON;
	LiquidCrystal_I2C_WriteControl(LCD_DISPLAYCONTROL | _displaycontrol);
}

// Turns the underline cursor on/off
void LiquidCrystal_I2C_CursorOff() {
	_displaycontrol &= ~LCD_CURSORON;
	LiquidCrystal_I2C_WriteControl(LCD_DISPLAYCONTROL | _displaycontrol);
}
void LiquidCrystal_I2C_CursorOn() {
	_displaycontrol |= LCD_CURSORON;
	LiquidCrystal_I2C_WriteControl(LCD_DISPLAYCONTROL | _displaycontrol);
}

// Turn on and off the blinking cursor
void LiquidCrystal_I2C_BlinkOn() {
	_displaycontrol &= ~LCD_BLINKON;
	LiquidCrystal_I2C_WriteControl(LCD_DISPLAYCONTROL | _displaycontrol);
}
void LiquidCrystal_I2C_BlinkOff() {
	_displaycontrol |= LCD_BLINKON;
	LiquidCrystal_I2C_WriteControl(LCD_DISPLAYCONTROL | _displaycontrol);
}

// These commands scroll the display without changing the RAM
void LiquidCrystal_I2C_ScrollDisplayLeft(void) {
	LiquidCrystal_I2C_WriteControl(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVELEFT);
}
void LiquidCrystal_I2C_ScrollDisplayRight(void) {
	LiquidCrystal_I2C_WriteControl(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVERIGHT);
}

// This is for text that flows Left to Right
void LiquidCrystal_I2C_PrintRight(void) {
	_displaymode |= LCD_ENTRYLEFT;
	LiquidCrystal_I2C_WriteControl(LCD_ENTRYMODESET | _displaymode);
}

// This is for text that flows Right to Left
void LiquidCrystal_I2C_PrintLeft(void) {
	_displaymode &= ~LCD_ENTRYLEFT;
	LiquidCrystal_I2C_WriteControl(LCD_ENTRYMODESET | _displaymode);
}

// This will 'right justify' text from the cursor
void LiquidCrystal_I2C_AutoscrollOn(void) {
	_displaymode |= LCD_ENTRYSHIFTINCREMENT;
	LiquidCrystal_I2C_WriteControl(LCD_ENTRYMODESET | _displaymode);
}

// This will 'left justify' text from the cursor
void LiquidCrystal_I2C_AutoscrollOff(void) {
	_displaymode &= ~LCD_ENTRYSHIFTINCREMENT;
	LiquidCrystal_I2C_WriteControl(LCD_ENTRYMODESET | _displaymode);
}



/*********** mid level commands, for sending data/cmds */

inline void LiquidCrystal_I2C_WriteControl(uint8_t value) {
	LiquidCrystal_I2C_sendByte(value, Command);
}

inline void LiquidCrystal_I2C_WriteData(uint8_t value) {
	LiquidCrystal_I2C_sendByte(value, Data);
}


/************ low level data pushing commands **********/

// write either command or data
void LiquidCrystal_I2C_sendByte(uint8_t value, RsType mode) {
	uint8_t highnib=(value & 0xf0) >> 4 ;
	uint8_t lownib=value & 0x0F;
	LiquidCrystal_I2C_write4bits(highnib, mode);
	LiquidCrystal_I2C_write4bits(lownib, mode);
}

void LiquidCrystal_I2C_write4bits(uint8_t value, RsType mode) {
    sig_Write(1);
    
    uint8 outByte = 0;
    
    switch(_dataPosition)
    {
        case High:
            outByte = value << 4 & 0xF0;
        break;
        case Low:
            outByte = value & 0x0F;
        break;
    }
    
    outByte = (value << 4) | _backLightBitMask | mode; 
    
	LiquidCrystal_I2C_expanderWrite(outByte);
	LiquidCrystal_I2C_pulseEnable(outByte);
    sig_Write(0);
}

void processI2CErrors(uint32 rval)
{
     switch(rval)
    {
        case I2C_I2C_MSTR_NO_ERROR:
            //UART_UartPutString("No error\n");
        
        break;
        case I2C_I2C_MSTR_NOT_READY:
            UART_UartPutString("Master Not Ready\n");
        break;
        case I2C_I2C_MSTR_ERR_LB_NAK:
            UART_UartPutString("Master Last Byte NAK\n");
        break;
        case I2C_I2C_MSTR_ERR_ARB_LOST:
            UART_UartPutString("Master Arbitration Lost\n");
        break;
        case I2C_I2C_MSTR_ERR_BUS_ERR:
             UART_UartPutString("Master Bus Error\n");
        break;
        case I2C_I2C_MSTR_ERR_ABORT_START:
             UART_UartPutString("Master Aborted Start\n");
            break;
        case I2C_I2C_MSTR_BUS_BUSY:
             UART_UartPutString("Master Bus Busy\n");
            break;
    }
}

void LiquidCrystal_I2C_expanderWrite(uint8_t _data){    
    
   uint32 rval;
   rval = I2C_I2CMasterSendStart(_Addr,I2C_I2C_WRITE_XFER_MODE);
   processI2CErrors(rval);
    rval = I2C_I2CMasterWriteByte(_data | _backLightBitMask);
   
    processI2CErrors(rval);
    rval = I2C_I2CMasterSendStop();
    
   processI2CErrors(rval);    
}

void LiquidCrystal_I2C_pulseEnable(uint8_t _data){
    sig1_Write(1);
	LiquidCrystal_I2C_expanderWrite(_data | _eBitMask);	// En high
	CyDelayUs(1);		// enable pulse must be >450ns
	sig1_Write(0);
	LiquidCrystal_I2C_expanderWrite(_data & ~_eBitMask);	// En low
	CyDelayUs(50);		// commands need > 37us to settle
} 


void LiquidCrystal_I2C_load_custom_character(uint8_t char_num, uint8_t *rows){
		LiquidCrystal_I2C_createChar(char_num, rows);
}

void LiquidCrystal_I2C_setBacklight(BackLightStatus newVal){
    _backLightStatus = newVal;
    LiquidCrystal_I2C_expanderWrite(_backLightBitMask);
}
inline  void LiquidCrystal_I2C_BackLightOff() {LiquidCrystal_I2C_setBacklight(OFF); }
inline  void LiquidCrystal_I2C_BackLightOn() { LiquidCrystal_I2C_setBacklight(ON); }


// LiquidCrystal_I2C V2.0
#include <inttypes.h>

#ifndef LiquidCrystal_I2C_h
#define LiquidCrystal_I2C_h

#include <inttypes.h>


void LiquidCrystal_I2C_Start(uint8_t lcd_Addr,uint8_t lcd_cols,uint8_t lcd_rows);
void LiquidCrystal_I2C_Stop();

//int LiquidCrystal_I2C_PutString(char *str);
void LiquidCrystal_I2C_PutChar(char c);
void LiquidCrystal_I2C_Position(uint8_t col, uint8_t row); 
void LiquidCrystal_I2C_WriteData(uint8_t);
void LiquidCrystal_I2C_WriteControl(uint8_t);
void LiquidCrystal_I2C_ClearDisplay();
// isReady()
// Sleep()
// wakeup
// init
// enable
// saveconfig
// restoreconfig

void LiquidCrystal_I2C_DisplayOn();
void LiquidCrystal_I2C_DisplayOff();

void LiquidCrystal_I2C_Home();
void LiquidCrystal_I2C_BlinkOn();
void LiquidCrystal_I2C_BlinkOff();
void LiquidCrystal_I2C_CursorOff();
void LiquidCrystal_I2C_CursorOn();
void LiquidCrystal_I2C_ScrollDisplayLeft();
void LiquidCrystal_I2C_ScrollDisplayRight();
void LiquidCrystal_I2C_PrintLeft();
void LiquidCrystal_I2C_PrintRight();
void LiquidCrystal_I2C_AutoScrollOn();
void LiquidCrystal_I2C_AutoScrollOff(); 
void LiquidCrystal_I2C_createChar(uint8_t, uint8_t[]);
void LiquidCrystal_I2C_BackLightOn();      	 					
void LiquidCrystal_I2C_BackLightOff();      					

#endif

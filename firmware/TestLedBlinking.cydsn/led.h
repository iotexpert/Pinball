#ifndef LED_H
#define LED_H

// Define the legal LED States
typedef enum MatrixLed_LedState {
    ON,
    OFF,
    BLINK
} MatrixLed_LedState;

void MatrixLed_Start(int callBackNumber); // turn on systick and register the callback

void MatrixLed_Enable(int row,int col); // turn the state to ON
void MatrixLed_Disable(int row,int col); // turn the state to OFF
void MatrixLed_Toggle(int row,int col); // toggle the LED
void MatrixLed_Blink(int row,int col,int frequency); // turn the state to blink and set frequency
void MatrixLed_DisableBlink(int row,int col); // Turn off blinking
MatrixLed_LedState MatrixLed_Read(int row,int col); // Return the state of the LED

#endif



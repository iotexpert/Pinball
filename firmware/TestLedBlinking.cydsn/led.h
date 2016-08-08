#ifndef LED_H
#define LED_H

typedef enum LedState {
    ON,
    OFF,
    BLINK
} LedState;

void ledStart(int callBackNumber); // turn on systick and register the callback

void enableLed(int row,int col); // turn the state to ON
void disableLed(int row,int col); // turn the state to OFF
void toggleLed(int row,int col); // toggle the LED
void blinkLed(int row,int col,int frequency); // turn the state to blink and set frequency
LedState readLed(int row,int col);    

#endif
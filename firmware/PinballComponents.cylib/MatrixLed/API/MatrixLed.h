#ifndef `$INSTANCE_NAME`_MATRIXLED_H
#define `$INSTANCE_NAME`_MATRIXLED_H

    
// These two need to be less than 8
#define `$INSTANCE_NAME`_NUMROWS (`$ROWS`)
#define `$INSTANCE_NAME`_NUMCOLS (`$COLS`)

// Define the legal LED States
typedef enum `$INSTANCE_NAME`_LedState {
    ON,
    OFF,
    BLINK
} `$INSTANCE_NAME`_LedState;

void `$INSTANCE_NAME`_Start(int callBackNumber); // turn on systick and register the callback

void `$INSTANCE_NAME`_Enable(int row,int col); // turn the state to ON
void `$INSTANCE_NAME`_Disable(int row,int col); // turn the state to OFF
void `$INSTANCE_NAME`_Toggle(int row,int col); // toggle the LED
void `$INSTANCE_NAME`_Blink(int row,int col,int frequency); // turn the state to blink and set frequency
void `$INSTANCE_NAME`_DisableBlink(int row,int col); // Turn off blinking
`$INSTANCE_NAME`_LedState `$INSTANCE_NAME`_Read(int row,int col); // Return the state of the LED

#endif

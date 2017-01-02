#include <project.h>
#include "`$INSTANCE_NAME`_MatrixLed.h"

// number of systicks to cycle the refresh LEDs ... hopefully systick is 1 ms
// which means each row gets updated every 3*5ms = 15ms or 
// this is essentially the prescaler for the software PWM
#define `$INSTANCE_NAME`_REFRESH (`$REFRESH`)

// raw 1 & 0's to be written into the rows by state machine
// the columns are held as a bit mask
static uint8 `$INSTANCE_NAME`_ledRows[`$INSTANCE_NAME`_NUMROWS]; 
// an array of the states ... used for blinking
static `$INSTANCE_NAME`_LedState `$INSTANCE_NAME`_ledStates[`$INSTANCE_NAME`_NUMROWS][`$INSTANCE_NAME`_NUMCOLS];


// 10hz update frequency
//  = 1000 / 10hz = 100ms
// half period = 100ms / 2 = 50 ms
// toggle clock ticks = 50ms/refresh = 5
//  ledTogglePeriods is the reset value for the software PWM
// ltp = 1000/2/freq/refresh (what to reset to)
static int `$INSTANCE_NAME`_ledTogglePeriods[`$INSTANCE_NAME`_NUMROWS][`$INSTANCE_NAME`_NUMCOLS]; 
// ledCurrentCount is the counter for the PWM
static int `$INSTANCE_NAME`_ledCurrentCount[`$INSTANCE_NAME`_NUMROWS][`$INSTANCE_NAME`_NUMCOLS]; // downcounter for toggles 

// turn the state to ON
inline void `$INSTANCE_NAME`_Enable(int row,int col)
{
    `$INSTANCE_NAME`_ledRows[row] |= 0x01<<col;
}
// turn the state to OFF
inline void `$INSTANCE_NAME`_Disable(int row,int col)
{
    `$INSTANCE_NAME`_ledRows[row] &= ~(0x01<<col);
}

// Toggle the state of the LED
inline void `$INSTANCE_NAME`_Toggle(int row,int col)
{
    `$INSTANCE_NAME`_ledRows[row] ^= 1<<col;   
}

// Turn the state to blink and set frequency
void `$INSTANCE_NAME`_Blink(int row,int col,int frequency)
{
    `$INSTANCE_NAME`_ledStates[row][col] = BLINK;
    `$INSTANCE_NAME`_ledTogglePeriods[row][col] = 1000/2/frequency/`$INSTANCE_NAME`_REFRESH; 
    `$INSTANCE_NAME`_ledCurrentCount[row][col] = `$INSTANCE_NAME`_ledTogglePeriods[row][col];
}

// Stop the blinking by setting the state to OFF
void `$INSTANCE_NAME`_DisableBlink(int row,int col)
{
    `$INSTANCE_NAME`_ledStates[row][col] = OFF;
}

// Return the curren state of an LED
inline `$INSTANCE_NAME`_LedState readLed(int row,int col) 
{
    return `$INSTANCE_NAME`_ledStates[row][col];
}    


// `$INSTANCE_NAME`_UpdateLeds - This function writes to the row and column PINs with the
// current row/column values.  Notice that the columns are active low.  It then goes
// onto the next row
inline static void `$INSTANCE_NAME`_UpdateLeds()
{
    static int currentRow = 0;
    `$RowComponent`_Write(1<<currentRow); // turn on the current row
    `$ColComponent`_Write(~`$INSTANCE_NAME`_ledRows[currentRow]); // the columns are active LOW
    currentRow = (currentRow + 1) % `$INSTANCE_NAME`_NUMROWS ;  // wrap around at the end
}

// `$INSTANCE_NAME`_UpdateBlinkingLeds - This function implements a Software PWM that toggles
// the LED every specified number of periods.  It is called by the RunStateMachine function
// which pre-scales the SysTick
inline static void `$INSTANCE_NAME`_UpdateBlinkingLeds()
{
    int row,col;
    for(row=0;row<`$INSTANCE_NAME`_NUMROWS;row++)
    {
        for(col=0;col<`$INSTANCE_NAME`_NUMCOLS;col++)
        {
            if(`$INSTANCE_NAME`_ledStates[row][col] == BLINK) // if the LED is set to blink then run the PWM
            {
                `$INSTANCE_NAME`_ledCurrentCount[row][col] -= 1;
                // if the counter = 0 then toggle the led and reset the count
                if( `$INSTANCE_NAME`_ledCurrentCount[row][col] == 0)
                {
                    `$INSTANCE_NAME`_Toggle(row,col);
                    `$INSTANCE_NAME`_ledCurrentCount[row][col] = `$INSTANCE_NAME`_ledTogglePeriods[row][col];
                }
            }
        }
    }
}

// This function is called by the systick interrupt
// every "REFRESH" it runs the function to update the current blinking
// leds and then it writes the current state to the LED
static void `$INSTANCE_NAME`_RunStateMachine()
{
    static int `$INSTANCE_NAME`_sysTickCount=0;

    `$INSTANCE_NAME`_sysTickCount += 1;
    // This is essentially a prescaler
    if(`$INSTANCE_NAME`_sysTickCount % `$INSTANCE_NAME`_REFRESH) 
        return;
    
    //IntService_Write(1);  // This was put here to see how long this takes by toggling a pin
    `$INSTANCE_NAME`_UpdateBlinkingLeds();
    `$INSTANCE_NAME`_UpdateLeds();
    //IntService_Write(0); // This was put here to see how long this takes by toggling a pin
}

// This is the main start function
// It sets up the systick callback and initializes everything to OFF
void `$INSTANCE_NAME`_Start(int callBackNumber)
{
    int row,col;
    for(row = 0;row<`$INSTANCE_NAME`_NUMROWS;row ++)
    {
        `$INSTANCE_NAME`_ledRows[row] = 0; // turn all columns off
        for(col = 0 ;  col < `$INSTANCE_NAME`_NUMCOLS; col ++ )
        {
            `$INSTANCE_NAME`_ledStates[row][col] = OFF;
        }
    }
    CySysTickStart();
    CySysTickSetCallback(callBackNumber,`$INSTANCE_NAME`_RunStateMachine);
}

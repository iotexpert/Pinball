#include <project.h>
#include "led.h"

// These two need to be less than 8
#define MatrixLed_NUMROWS (3)
#define MatrixLed_NUMCOLS (3)

// number of systicks to cycle the refresh LEDs ... hopefully systick is 1 ms
// which means each row gets updated every 3*5ms = 15ms or 
// this is essentially the prescaler for the software PWM
#define MatrixLed_REFRESH (5)

// raw 1 & 0's to be written into the rows by state machine
// the columns are held as a bit mask
static uint8 MatrixLed_ledRows[MatrixLed_NUMROWS]; 
// an array of the states ... used for blinking
static MatrixLed_LedState MatrixLed_ledStates[MatrixLed_NUMROWS][MatrixLed_NUMCOLS];

// 10hz update frequency
//  = 1000 / 10hz = 100ms
// half period = 100ms / 2 = 50 ms
// toggle clock ticks = 50ms/refresh = 5
//  ledTogglePeriods is the reset value for the software PWM
static int MatrixLed_ledTogglePeriods[MatrixLed_NUMROWS][MatrixLed_NUMCOLS]; // ltp = 1000/2/freq/refresh (what to reset to)
// ledCurrentCount is the counter for the PWM
static int MatrixLed_ledCurrentCount[MatrixLed_NUMROWS][MatrixLed_NUMCOLS]; // downcounter for toggles 

// turn the state to ON
inline void MatrixLed_Enable(int row,int col)
{
    MatrixLed_ledRows[row] |= 0x01<<col;
}
// turn the state to OFF
inline void MatrixLed_Disable(int row,int col)
{
    MatrixLed_ledRows[row] &= ~(0x01<<col);
}

// Toggle the state of the LED
inline void MatrixLed_Toggle(int row,int col)
{
    MatrixLed_ledRows[row] ^= 1<<col;   
}

// Turn the state to blink and set frequency
void MatrixLed_Blink(int row,int col,int frequency)
{
    MatrixLed_ledStates[row][col] = BLINK;
    MatrixLed_ledTogglePeriods[row][col] = 1000/2/frequency/MatrixLed_REFRESH; 
    MatrixLed_ledCurrentCount[row][col] = MatrixLed_ledTogglePeriods[row][col];
}

// Stop the blinking by setting the state to OFF
void MatrixLed_DisableBlink(int row,int col)
{
    MatrixLed_ledStates[row][col] = OFF;
}

// Return the curren state of an LED
inline MatrixLed_LedState readLed(int row,int col) 
{
    return MatrixLed_ledStates[row][col];
}    


// MatrixLed_UpdateLeds - This function writes to the row and column PINs with the
// current row/column values.  Notice that the columns are active low.  It then goes
// onto the next row
inline static void MatrixLed_UpdateLeds()
{
    static int currentRow = 0;
    row_Write(1<<currentRow); // turn on the current row
    col_Write(~MatrixLed_ledRows[currentRow]); // the columns are active LOW
    currentRow = (currentRow + 1) % MatrixLed_NUMROWS ;  // wrap around at the end
}

// MatrixLed_UpdateBlinkingLeds - This function implements a Software PWM that toggles
// the LED every specified number of periods.  It is called by the RunStateMachine function
// which pre-scales the SysTick
inline static void MatrixLed_UpdateBlinkingLeds()
{
    int row,col;
    for(row=0;row<MatrixLed_NUMROWS;row++)
    {
        for(col=0;col<MatrixLed_NUMCOLS;col++)
        {
            if(MatrixLed_ledStates[row][col] == BLINK) // if the LED is set to blink then run the PWM
            {
                MatrixLed_ledCurrentCount[row][col] -= 1;
                // if the counter = 0 then toggle the led and reset the count
                if( MatrixLed_ledCurrentCount[row][col] == 0)
                {
                    MatrixLed_Toggle(row,col);
                    MatrixLed_ledCurrentCount[row][col] = MatrixLed_ledTogglePeriods[row][col];
                }
            }
        }
    }
}

// This function is called by the systick interrupt
// every "REFRESH" it runs the function to update the current blinking
// leds and then it writes the current state to the LED
static void MatrixLed_RunStateMachine()
{
    static int MatrixLed_sysTickCount=0;

    MatrixLed_sysTickCount += 1;
    // This is essentially a prescaler
    if(MatrixLed_sysTickCount % MatrixLed_REFRESH) 
        return;
    MatrixLed_UpdateBlinkingLeds();
    MatrixLed_UpdateLeds();
}

// This is the main start function
// It sets up the systick callback and initializes everything to OFF
void MatrixLed_Start(int callBackNumber)
{
    int row,col;
    for(row = 0;row<MatrixLed_NUMROWS;row ++)
    {
        MatrixLed_ledRows[row] = 0; // turn all columns off
        for(col = 0 ;  col < MatrixLed_NUMCOLS; col ++ )
        {
            MatrixLed_ledStates[row][col] = OFF;
        }
    }
    CySysTickStart();
    CySysTickSetCallback(callBackNumber,MatrixLed_RunStateMachine);
}
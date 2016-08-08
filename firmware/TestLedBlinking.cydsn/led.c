#include "led.h"
#include <project.h>

#define NUMROWS (3)
#define NUMCOLS (3)

// number of systicks to refresh the LEDs ... hopefully 1 ms
#define REFRESH (10)

// raw 1,0 to be written into the rows by state machine
static uint8 ledRows[NUMROWS]; 
static LedState ledStates[NUMROWS][NUMCOLS];

// 10hz update frequency
//  = 1000 / 10hz = 100ms
// half period = 100ms / 2 = 50 ms
// toggle clock ticks = 50ms/refresh = 5
static int ledTogglePeriods[NUMROWS][NUMCOLS]; // ltp = 1000/2/freq/refresh (what to reset to)
static int ledCurrentCount[NUMROWS][NUMCOLS]; // downcounter for toggles 

// turn the state to ON
inline void enableLed(int row,int col)
{
    ledRows[row] |= 0x01<<col;
}
// turn the state to OFF
inline void disableLed(int row,int col)
{
    ledRows[row] &= ~(0x01<<col);
}

void blinkLed(int row,int col,int frequency)
{// turn the state to blink and set frequency

    ledStates[row][col] = BLINK;
    ledTogglePeriods[row][col] = 1000/2/frequency/REFRESH; 
    ledCurrentCount[row][col] = ledTogglePeriods[row][col];
}

inline LedState readLed(int row,int col) 
{
    return ledStates[row][col];
}    

// Private functions
inline static void updateLeds();
static int sysTickCount=0;

inline static void updateLeds()
{
    static int currentRow = 0;
    row_Write(ledRows[currentRow]);
    currentRow = (currentRow + 1) % NUMROWS ;
}

inline static void updateBlinkingLeds()
{
    int row,col;
    for(row=0;row<NUMROWS;row++)
    {
        for(col=0;col<NUMCOLS;col++)
        {
            if (ledStates[row][col] == BLINK)
            {
                ledCurrentCount[row][col] -= 1;
                // if the counter = 0 then toggle the led and reset the count
                if( ledCurrentCount[row][col] == 0)
                {
                    toggleLed(row,col);
                    ledCurrentCount[row][col] = ledTogglePeriods[row][col];
                }
            }
        }
    }
}

static void runStateMachine()
{
    sysTickCount = sysTickCount + 1;
    if(sysTickCount % REFRESH)
        return;
    updateBlinkingLeds();    
    updateLeds();
}

void ledStart(int callBackNumber)
{
    int row,col;
    for(row = 0;row<NUMROWS;row ++)
    {
        ledRows[row] = 0;
        for(col = 0 ;  col < NUMCOLS; col ++ )
        {
            ledStates[row][col] = OFF;
        }
    }
    CySysTickEnable();
    CySysTickSetCallback(callBackNumber,runStateMachine);
}
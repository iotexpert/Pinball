#include <project.h>
#include "SwitchMatrix_SwitchMatrix.h"

// how often to read each switch in MS
#define SwitchMatrix_READ_INTERVAL (50)


//static int SwitchMatrix_updateInterval=1000/SwitchMatrix_READ_INTERVAL/SwitchMatrix_NUMCOLS;
static int SwitchMatrix_currentCol=0;

static uint8 SwitchMatrix_temps[SwitchMatrix_NUMCOLS];
static SwitchMatrix_MASK_TYPE SwitchMatrix_currentStatus;

// bitwise flags to indicate interest in interrupt
static SwitchMatrix_MASK_TYPE SwitchMatrix_fallingInterrupt;
static SwitchMatrix_MASK_TYPE SwitchMatrix_risingInterrupt;
static SwitchMatrix_MASK_TYPE SwitchMatrix_bothInterrupt;

// bitwise flags to indicate which one changed
static SwitchMatrix_MASK_TYPE SwitchMatrix_fallingInterruptStatus;
static SwitchMatrix_MASK_TYPE SwitchMatrix_risingInterruptStatus;
static SwitchMatrix_MASK_TYPE SwitchMatrix_bothInterruptStatus;


void SwitchMatrix_SetInterruptMode(int row,int col,SwitchMatrix_Inter_Mode mode)
{
}


/*****************************************************************************\
 * Function:    switchCallBack
 * Input:       void
 * Returns:     void
 * Description: 
 *     This function is called by the systick timer and reads the vaule of the switchs
\*****************************************************************************/
void SwitchMatrix_SwitchCallBack()
{
    
    static int count=0;
    volatile uint32_t rowMask = (2^SwitchMatrix_NUMROWS)-1;
    rowMask = 7; // this is just a temporary fix to get the right bitmask

    if(count++<SwitchMatrix_READ_INTERVAL/SwitchMatrix_NUMCOLS)
        return;
    
    count = 0;
    
    SwitchMatrix_temps[SwitchMatrix_currentCol] = ~SwitchMatrix_Row_Read() & rowMask;
    
    // calculate the bit mask
    int i;
    SwitchMatrix_MASK_TYPE  nextStatus=0;
    
    for(i=0;i<SwitchMatrix_NUMCOLS;i++)
    {
        nextStatus |= SwitchMatrix_temps[i] << (i * SwitchMatrix_NUMROWS);
    }
    
    
    SwitchMatrix_fallingInterruptStatus |= ~SwitchMatrix_currentStatus & nextStatus & SwitchMatrix_fallingInterrupt;
    SwitchMatrix_risingInterruptStatus |= SwitchMatrix_currentStatus & ~nextStatus & SwitchMatrix_risingInterrupt;
    SwitchMatrix_bothInterruptStatus |= (SwitchMatrix_fallingInterruptStatus | SwitchMatrix_risingInterruptStatus) & SwitchMatrix_bothInterrupt;
    
    SwitchMatrix_currentStatus = nextStatus;
    
    
    SwitchMatrix_Col_Write(~(1<<SwitchMatrix_currentCol)); // setup for the next column
    
    SwitchMatrix_currentCol = (SwitchMatrix_currentCol + 1) % SwitchMatrix_NUMCOLS;
    
    #ifdef SwitchMatrix_SWITCHED_CALLBACK
    if(SwitchMatrix_risingInterruptStatus | SwitchMatrix_fallingInterruptStatus | bothInterruptStatus)
    {
        (*SwitchMatrix_SWITCHED_CALLBACK());
    
    }
   
    #endif
    
}

SwitchMatrix_MASK_TYPE SwitchMatrix_ReadAll()
{ 
    return SwitchMatrix_currentStatus;
}

/*****************************************************************************\
 * Function:    Start
 * Input:       
 * Returns:     
 * Description: 
 *     Register the interrupt and initialize the counters to get things going
\*****************************************************************************/
void SwitchMatrix_Start(int intNumber)
{
    CySysTickStart();
    CySysTickSetCallback(intNumber,SwitchMatrix_SwitchCallBack);
}


/*****************************************************************************\
 * Function:    Read
 * Input:       row,col
 * Returns:     switch state
 * Description: 
 *     This returns the current state of the switch identified by row and col
\*****************************************************************************/
inline int SwitchMatrix_Read(int row,int col)
{
    return (SwitchMatrix_currentStatus & 1<<(col*SwitchMatrix_NUMROWS+row))?1:0;
}

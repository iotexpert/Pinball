#include <project.h>
#include "cyapicallbacks.h"
#include "`$INSTANCE_NAME`_SwitchMatrix.h"
#define `$INSTANCE_NAME`_NUMCOLS (`$NumCols`)
#define `$INSTANCE_NAME`_NUMROWS (`$NumRows`)


// the total period from first to last switch read in ms
#define `$INSTANCE_NAME`_READ_INTERVAL (`$RefreshInterval`)
#define `$INSTANCE_NAME`_ROWMASK ((1<<`$INSTANCE_NAME`_NUMROWS)-1)


//static int SwitchMatrix_updateInterval=1000/SwitchMatrix_READ_INTERVAL/SwitchMatrix_NUMCOLS;
static int `$INSTANCE_NAME`_currentCol=0;

static uint8 `$INSTANCE_NAME`_temps[`$INSTANCE_NAME`_NUMCOLS];
static `$INSTANCE_NAME`_MASK_TYPE `$INSTANCE_NAME`_currentStatus;

// bitwise flags to indicate interest in interrupt
static `$INSTANCE_NAME`_MASK_TYPE `$INSTANCE_NAME`_fallingInterrupt;
static `$INSTANCE_NAME`_MASK_TYPE `$INSTANCE_NAME`_risingInterrupt;

// bitwise flags to indicate which one changed
static `$INSTANCE_NAME`_MASK_TYPE `$INSTANCE_NAME`_fallingInterruptStatus;
static `$INSTANCE_NAME`_MASK_TYPE `$INSTANCE_NAME`_risingInterruptStatus;


/*****************************************************************************\
 * Function:    `$INSTANCE_NAME`_SetInterruptMode
 * Input:       row,column and which mode you want (falling,rising,both,none)
 * Returns:     
 * Description: 
 *     enables a call back when there is a rising,falling,both of a certain switch
 * in row,column.  
\*****************************************************************************/

void `$INSTANCE_NAME`_SetInterruptMode(int row,int col,`$INSTANCE_NAME`_Inter_Mode mode)
{
    `$INSTANCE_NAME`_MASK_TYPE temp;
    temp = 1 << (col*`$INSTANCE_NAME`_NUMCOLS + row); // make a bit mask
    `$INSTANCE_NAME`_fallingInterrupt &= ~temp; // turn off the falling interrupt flag
    `$INSTANCE_NAME`_risingInterrupt &= ~temp; // turn off the rising interryupt flag
    
    switch(mode)
    {
        case NONE: // not required as we start with them all off
        break;
        case RISING:
            `$INSTANCE_NAME`_risingInterrupt |= temp;
        break;
        case FALLING:
            `$INSTANCE_NAME`_fallingInterrupt |= temp;
        break;
        case BOTH:
         `$INSTANCE_NAME`_fallingInterrupt |= temp;
         `$INSTANCE_NAME`_risingInterrupt |= temp;  
        break;
    }
    
}

/*****************************************************************************\
 * Function:    `$INSTANCE_NAME`_ClearInterrupt()
 * Input:       void
 * Returns:     
 * Description: 
 *     Clears both interrupt masks and return or of rising and falling
\*****************************************************************************/
`$INSTANCE_NAME`_MASK_TYPE `$INSTANCE_NAME`_ClearInterrupt()
{
    `$INSTANCE_NAME`_MASK_TYPE rval = `$INSTANCE_NAME`_risingInterruptStatus | `$INSTANCE_NAME`_fallingInterruptStatus;
    `$INSTANCE_NAME`_risingInterruptStatus = 0;
    `$INSTANCE_NAME`_fallingInterruptStatus = 0;
    return rval;
}


/*****************************************************************************\
 * Function:    `$INSTANCE_NAME`_switchCallBack
 * Input:       void
 * Returns:     void
 * Description: 
 *     This function is called by the systick timer and reads the vaule of the switchs
 * calls the interrupt if requied based on the interrupt
\*****************************************************************************/
static void `$INSTANCE_NAME`_SwitchCallBack()
{
    
    static int count=0;

    // if isnt time to process the switches then return
    if(count++<`$INSTANCE_NAME`_READ_INTERVAL/`$INSTANCE_NAME`_NUMCOLS)
        return;
    count = 0;
    
    // read the switch into the temps array and mask off the other bits
    `$INSTANCE_NAME`_temps[`$INSTANCE_NAME`_currentCol] = ~`$INSTANCE_NAME`_Row_Read() & `$INSTANCE_NAME`_ROWMASK;
    
    // calculate the bit mask by combining all of the rows together.
    int i;
    `$INSTANCE_NAME`_MASK_TYPE  nextStatus=0;
    
    for(i=0;i<`$INSTANCE_NAME`_NUMCOLS;i++)
    {
        nextStatus |= `$INSTANCE_NAME`_temps[i] << (i * `$INSTANCE_NAME`_NUMROWS);
    }
    
    
    // if you have a falling or rising then set the coresponding flag
    // I made the truth table 
    `$INSTANCE_NAME`_fallingInterruptStatus |= `$INSTANCE_NAME`_currentStatus & ~nextStatus & `$INSTANCE_NAME`_fallingInterrupt;
    `$INSTANCE_NAME`_risingInterruptStatus |= ~`$INSTANCE_NAME`_currentStatus & nextStatus & `$INSTANCE_NAME`_risingInterrupt;
    
    // save the current status
    `$INSTANCE_NAME`_currentStatus = nextStatus;
    
    
    `$INSTANCE_NAME`_Col_Write(~(1<<`$INSTANCE_NAME`_currentCol)); // setup for the next column
    
    // round robin through the columns
    `$INSTANCE_NAME`_currentCol = (`$INSTANCE_NAME`_currentCol + 1) % `$INSTANCE_NAME`_NUMCOLS;
    
    // if they have asked for an interrupt when a rising, falling or both happens
    #ifdef `$INSTANCE_NAME`_SWITCHED_CALLBACK
    if(`$INSTANCE_NAME`_risingInterruptStatus | `$INSTANCE_NAME`_fallingInterruptStatus )
    {
        `$INSTANCE_NAME`_SWITCHED_CALLBACK();
    }
   
    #endif
    
}

/*****************************************************************************\
 * Function:    `$INSTANCE_NAME`_ReadAll
 * Input:       none
 * Returns:     returns a bitmask of the current switch states (1=on 0=off)
 * Description: 
 *    The bit mask is (from LSB) R0/C0 R1/C0 R2/C0 R0/C1 ...
\*****************************************************************************/
`$INSTANCE_NAME`_MASK_TYPE `$INSTANCE_NAME`_ReadAll()
{ 
    return `$INSTANCE_NAME`_currentStatus;
}

/*****************************************************************************\
 * Function:    `$INSTANCE_NAME`_ReadFallingStatus
 * Input:       none
 * Returns:     returns a bitmask of the current falling status
 * Description: 
 *    The bit mask is (from LSB) R0/C0 R1/C0 R2/C0 R0/C1 ...
 * does not clear the mask
\*****************************************************************************/
`$INSTANCE_NAME`_MASK_TYPE `$INSTANCE_NAME`_ReadFallingInterruptStatus()
{ 
    return `$INSTANCE_NAME`_fallingInterruptStatus;
}


/*****************************************************************************\
 * Function:    `$INSTANCE_NAME`_ReadRisingStatus
 * Input:       none
 * Returns:     returns a bitmask of the current falling status
 * Description: 
 *    The bit mask is (from LSB) R0/C0 R1/C0 R2/C0 R0/C1 ...
 * does not clear the mask
\*****************************************************************************/
`$INSTANCE_NAME`_MASK_TYPE `$INSTANCE_NAME`_ReadRisingInterruptStatus()
{ 
    return `$INSTANCE_NAME`_risingInterruptStatus;
}

/*****************************************************************************\
 * Function:    `$INSTANCE_NAME`_Start
 * Input:       
 * Returns:     
 * Description: 
 *     Register the interrupt and initialize the counters to get things going
\*****************************************************************************/
void `$INSTANCE_NAME`_Start(int intNumber)
{
   
    CySysTickStart();
    CySysTickSetCallback(intNumber,`$INSTANCE_NAME`_SwitchCallBack);
}


/*****************************************************************************\
 * Function:    `$INSTANCE_NAME`_Read
 * Input:       row,col
 * Returns:     switch state
 * Description: 
 *     This returns the current state of the switch identified by row and col
 *     true means switched, false means not active/not switched
\*****************************************************************************/
inline int `$INSTANCE_NAME`_Read(int row,int col)
{
    return (`$INSTANCE_NAME`_currentStatus & 1<<(col*`$INSTANCE_NAME`_NUMROWS+row))?1:0;
}

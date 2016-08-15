#include <project.h>
#include "SwitchMatrix.h"

// how often to read each switch in Hz
#define SwitchMatrix_READ_INTERVAL (50)

static int SwitchMatrix_updateInterval=1000/SwitchMatrix_READ_INTERVAL/SwitchMatrix_NUMCOLS;
static int SwitchMatrix_currentCol=0;
static uint8 SwitchMatrix_vals[SwitchMatrix_NUMCOLS];


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
    count+=1;
    if(count<10)
        return;
    count = 0;
    
    SwitchMatrix_vals[SwitchMatrix_currentCol] = ~Row_Read();
    SwitchMatrix_currentCol = (SwitchMatrix_currentCol+1) % SwitchMatrix_NUMCOLS;
    Col_Write(SwitchMatrix_currentCol); // setup for the next column
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
    return (SwitchMatrix_vals[row] & 1<<col)?1:0;
}
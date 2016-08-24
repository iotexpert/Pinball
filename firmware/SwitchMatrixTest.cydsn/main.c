#include <project.h>
#include <stdio.h>

int switchedFlag = 0; // a flag that a switch interrupt has happened
int sysTimeFlag=0; // a flag that 200ms has passed

/*****************************************************************************\
 * Function:    switchedCallback
 * Input:       none
 * Returns:     none
 * Description: 
 *     This function is setup in the cyapicallbacks.h file to be called back when
 * a swtich interrupt is called
\*****************************************************************************/
void switchedCallBack()
{
    switchedFlag = SwitchMatrix_ClearInterrupt();
}

/*****************************************************************************\
 * Function:    systemTimer
 * Input:       none
 * Returns:     none
 * Description: 
 *     This function is a callback from the systick to create a 200 ms timer
\*****************************************************************************/
void systemTimer()
{
    static int sysCount=0;
    if(sysCount++<200)
        return;
    sysCount = 0;
    sysTimeFlag = 1;
}


int main(void)
{
    char buff[128];
    int c;
    int printFlag = 0;
    
    CyGlobalIntEnable; 
    UART_Start();
    
    SwitchMatrix_Start(0);
    CySysTickSetCallback(1,systemTimer);
    
    for(;;)
    {
        c = UART_UartGetChar();
        switch(c)
        {
            case 'a': // print current status of all of switches
                sprintf(buff,"Status = %x\n",SwitchMatrix_ReadAll());
                UART_UartPutString(buff);
            break;
            case 'f': // set falling flag for 00
                SwitchMatrix_SetInterruptMode(0,0,FALLING);
            break;
            case 'r': // set rising flag for 0,0
                SwitchMatrix_SetInterruptMode(0,0,RISING);
            break;
            case 'b': // set both flag for 0,0
                SwitchMatrix_SetInterruptMode(0,0,BOTH);
            break;
            case 'n': // turn off interrupt flag for 0,0
                SwitchMatrix_SetInterruptMode(0,0,NONE);
            break;
            case 'p': // turn off the repeating print 
                printFlag = 0;
            break;
            case 'P': // start a printout of the switches every 200ms
                printFlag = 1;
            break;
        }
        
        
        if(switchedFlag)
        {
            sprintf(buff,"%x\n",switchedFlag);
            UART_UartPutString(buff);
            switchedFlag = 0;
        }

        // if 200ms has passed and the printFlag is set then printout the current status
        if(sysTimeFlag && printFlag)
        {
            sysTimeFlag = 0;
            sprintf(buff,"%x\n",SwitchMatrix_ReadAll());
            UART_UartPutString(buff);
        }
        
    }
}

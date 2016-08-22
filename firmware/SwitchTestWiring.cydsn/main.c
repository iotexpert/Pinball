#include <project.h>
#include <stdio.h>

int printStatus=0; // flag to cause a print out of the bit status

// This function is used for the SysTick callback
// it sets a global flag called "printStatus" every 200ms
void sysTickCount()
{
    static int sysCount=0;
    if(sysCount++ < 200)
        return;
    printStatus = 1;
    sysCount=0;
}

int main()
{
    char buff[128];
    CyGlobalIntEnable;
    UART_Start();
    COL_Write(~0); // turn off the columns    
    CySysTickStart();
    CySysTickSetCallback(0,sysTickCount);
    for(;;)
    {
        int c;
        c = UART_UartGetChar();
        switch(c)
        {
            case '0': // turn off all of the columns
                COL_Write(~0); 
                break;
            case '1': // turn on column 0 (bit 0)
                COL_Write(~1);
            break;
            case '2': // turn on column 2 (bit 1)
                COL_Write(~2);
            break;
            case '3': // turn on column 3 (but 2)
                COL_Write(~4);
            break;
        }
        if(printStatus)
        {
           uint8 val,b0,b1,b2;
           val= ROW_Read();
           b0 = (val & 0x01)?1:0;
           b1 = (val & 0x02)?1:0;
           b2 = (val & 0x04)?1:0;
        
           sprintf(buff,"%d%d%d\n",b0,b1,b2);
           UART_UartPutString(buff);
           printStatus=0;
        }
    }
}

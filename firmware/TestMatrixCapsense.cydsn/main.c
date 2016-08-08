#include <project.h>
#include <stdio.h>

int go=0;

int main()
{
    char buff[128];
    CyGlobalIntEnable; 
    UART_Start();
    
    CapSense_Start();
    CapSense_InitializeEnabledBaselines();
    CapSense_ScanEnabledWidgets();
    
    for(;;)
    {
        int row,col;
        for(row=0;row<3;row++)
        {
            for(col=0;col<3;col++)
            {
                sprintf(buff,"row = %d col=%d\n",row,col);
                UART_UartPutString(buff);
                row_Write(1<<row);
                col_Write(~(1<<col));
                CyDelay(500);
            }
        }
    }
}

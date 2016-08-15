#include <project.h>
#include <stdio.h>

#define NUMROWS (4)
#define NUMCOLS (4)

int main()
{
    char buff[128];
    CyGlobalIntEnable; 
    UART_Start();
    
    for(;;)
    {
        int row,col;
        for(row=0;row<NUMROWS;row++)
        {
            for(col=0;col<NUMCOLS;col++)
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

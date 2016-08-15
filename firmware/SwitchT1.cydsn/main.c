/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#include <project.h>

int main()
{
    CyGlobalIntEnable; /* Enable global interrupts. */
    RED_Write(1);
    UART_Start();

    for(;;)
    {
        int c;
        c= UART_UartGetChar();
        
        switch(c)
        {
            case 'c':
            RED_Write(1);
            break;
            case 'C':
            RED_Write(0);
            break;
        }
        
        if(Col_Read() == 0)
        {
            RED_Write(0);
        }
        else
        {
            RED_Write(1);
        }
    }
}

/* [] END OF FILE */

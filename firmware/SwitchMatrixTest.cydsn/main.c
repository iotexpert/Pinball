
#include <project.h>
#include <stdio.h>

int main(void)
{
    char buff[128];
    
    CyGlobalIntEnable; 
    UART_Start();
    
    SwitchMatrix_Start(0);
    
    
    for(;;)
    {
        uint16 temp,temp1;
        
        temp = SwitchMatrix_ReadAll();
        temp1 = SwitchMatrix_Read(1,2);
        sprintf(buff,"%x R1C2=%x\n",temp,temp1);
        UART_UartPutString(buff);
        CyDelay(200);
        
        
    }
}

/* [] END OF FILE */

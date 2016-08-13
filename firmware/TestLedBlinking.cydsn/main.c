#include <project.h>
#include <stdio.h>
#include "led.h"

int sequenceCounter=0;

// increment the global clock which counts SysTicks of 1ms
void counterCallBack()
{
	sequenceCounter++;
}


int main()
{
	char buff[128];
	int row=0,col=0;  
	int c;	
	int sequenceMode = 0;
	
	CyGlobalIntEnable; 
	UART_Start();

	CySysTickStart();
	CySysTickSetCallback(1,counterCallBack);	
	MatrixLed_Start(0);
	
	while(1)
	{
		c = UART_UartGetChar();
		
		switch(c)
		{
			case 'a':
                sequenceMode=0;
	            MatrixLed_Enable(0,0);
	            MatrixLed_Enable(0,1);
	            MatrixLed_Enable(0,2);
	            MatrixLed_Enable(1,0);
	            MatrixLed_Enable(1,1);
	            MatrixLed_Enable(1,2);
	            MatrixLed_Enable(2,0);
	            MatrixLed_Enable(2,1);
	            MatrixLed_Enable(2,2);
	        break;
                
            case 'A':
                sequenceMode=0;
                MatrixLed_Disable(0,0);
                MatrixLed_Disable(0,1);
                MatrixLed_Disable(0,2);
                MatrixLed_Disable(1,0);
                MatrixLed_Disable(1,1);
                MatrixLed_Disable(1,2);
                MatrixLed_Disable(2,0);
                MatrixLed_Disable(2,1);
                MatrixLed_Disable(2,2);
                 
            break;
            case 'b':
                sequenceMode=0;
                MatrixLed_DisableBlink(0,0);
                MatrixLed_DisableBlink(0,1);
                MatrixLed_DisableBlink(0,2);
                MatrixLed_DisableBlink(1,0);
                MatrixLed_DisableBlink(1,1);
                MatrixLed_DisableBlink(1,2);
                MatrixLed_DisableBlink(2,0);
                MatrixLed_DisableBlink(2,1);
                MatrixLed_DisableBlink(2,2);
            break;
           
            case's':
                sequenceMode=0;
            	MatrixLed_Blink(0,0,10);
            	MatrixLed_Blink(2,2,2);
            	MatrixLed_Blink(1,1,5);
            	MatrixLed_Blink(2,0,3);
        	break;
	
        	case 'd':
        		sequenceMode = !sequenceMode;
        	break;
		}
		
        // If you are in sequence mode and 500ms has passed then
        // turn off the current LED and light the next one in the
        // sequence
		if(sequenceMode && (sequenceCounter%500==0))
		{
		    MatrixLed_Disable(row,col);
			col = (col + 1) % 3;
            if(col == 0)
            {
                row = (row + 1) % 3;
            }
            MatrixLed_Enable(row,col);
        }
    }
}
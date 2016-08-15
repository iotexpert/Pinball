#include <project.h>
#include <stdio.h>

int next=0;
// increment the global clock which counts SysTicks of 1ms
void counterCallBack()
{
    static int sequenceCounter=0;
    sequenceCounter++;
    if(sequenceCounter > 500)
    {
        sequenceCounter=0;
        next = 1;
    }
}


void allOff()
{
    int row,col;
    for(row = 0 ; row<MatrixLed_NUMROWS; row++)
    {
        for(col = 0 ; col<MatrixLed_NUMCOLS; col ++)
        {
            MatrixLed_DisableBlink(row,col);
            MatrixLed_Disable(row,col);
        }
    }
}

void allOn()
{
    int row,col;
    for(row = 0 ; row<MatrixLed_NUMROWS; row++)
    {
        for(col = 0 ; col<MatrixLed_NUMCOLS; col ++)
        {
            MatrixLed_DisableBlink(row,col);
            MatrixLed_Enable(row,col);
        }
    }
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
            
	           allOff();
	        break;
                
            case 'A':
               sequenceMode=0;
               allOn();      
            break;
           
            case's':
                sequenceMode=0;
                allOff();
            	MatrixLed_Blink(0,0,10);
            	MatrixLed_Blink(2,2,2);
            	MatrixLed_Blink(1,1,5);
            	MatrixLed_Blink(2,0,3);
                MatrixLed_Blink(0,2,6);
                MatrixLed_Blink(3,3,7);
                
        	break;
	
        	case 'd':
                allOff();
        		sequenceMode = !sequenceMode;
        	break;
		}
		
        // If you are in sequence mode and 500ms has passed then
        // turn off the current LED and light the next one in the
        // sequence
		if(sequenceMode && (next))
		{
            next=0;
		    MatrixLed_Disable(row,col);
			col = (col + 1) % MatrixLed_NUMCOLS;
            if(col == 0)
            {
                row = (row + 1) % MatrixLed_NUMROWS;
            }
            MatrixLed_Enable(row,col);
        }
    }
}
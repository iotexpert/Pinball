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
#include <stdio.h>
#include <project.h>

char buff[128];

int main()
{
    char c;    
    int note=0; // current Notes
    int printFlag=0; // if set to 0 it prints the notes when they change
 
    CyGlobalIntEnable; /* Enable global interrupts. */
    CySysTickStart();
    
    Music_Start(0);
 
    UART_Start();
   
    sprintf(buff,"Started\n");
    UART_UartPutString(buff);

    for(;;)
    {
        c = UART_UartGetChar();
        switch(c)
        {
            case 's':
                UART_UartPutString("Start\n");
                Music_PlaySong(0);
            break;
            case 'S':
                UART_UartPutString("Stop\n");
                Music_Stop();
            break;
            case 'p':
                printFlag=0;
                break;
            case 'P':
                printFlag = 1;
            break;
            case 't': // put it back to the default
                Music_SetBPM(0);
            break;
            case 'T':
                Music_SetBPM(60);
            break;
                
            case '1':
                Music_PlaySong(0);
            break;
            case '2':
                Music_PlaySong(1);
            break;
        }
        
        if(note != Music_GetNote() && printFlag)
        {
            note = Music_GetNote();
            sprintf(buff,"Note = %d\n",note);
            UART_UartPutString(buff);
        }
    }
}


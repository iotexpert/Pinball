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
#include "music.h"

extern int newNoteFlag;

char buff[128];
int main()
{
    CyGlobalIntEnable; /* Enable global interrupts. */
    char c;
    
    CySysTickStart();
    
    Music_Start(0);
   
    UART_Start();
    
    int note;
    int p=0;
    
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
                p=0;
                break;
            case 'P':
                p = 1;
                break;
                
        }
        
        if(newNoteFlag)
        {
            note = Music_GetNote();
            sprintf(buff,"Note = %d\n",newNoteFlag);
            newNoteFlag = 0;
            UART_UartPutString(buff);
        }
    
    }
}


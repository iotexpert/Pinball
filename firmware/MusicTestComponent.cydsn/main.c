#include <project.h>
#include <stdio.h>
char buff[128];

static Note scaleNotes[] = {
    { NoteA4,QUARTER_NOTE},
    { NoteB4,QUARTER_NOTE},
    { NoteC4,QUARTER_NOTE},
    { NoteD4,QUARTER_NOTE},
    { NoteE4,QUARTER_NOTE},
    { NoteF4,QUARTER_NOTE},
    { NoteG4,QUARTER_NOTE},
};

static Song scale = {
    .numNotes = 7,
    .bpm = 40,
    .notes = scaleNotes
};

int main()
{
    char c;    
    int note=0; // current note
    int printFlag=0; // if set to 0 it prints the notes when they change
 
    CyGlobalIntEnable;
    CySysTickStart();
    Music_Start(0);
    UART_Start();
    UART_UartPutString("Started\n");

    Music_AddSong(1,&scale);
    
    for(;;)
    {
        c = UART_UartGetChar();
        switch(c)
        {
            #ifdef Music_TWOCHANNELS
            case 'b': Music_BuzzOn(300,0);    break; // turn on the buzzer 1
            case 'B': Music_BuzzOff();        break; // turn off the buzzer 1
            case 'n': Music_BuzzOn(200,500);  break; // turn on the buzzer for 500ms
            #endif
            case 'p': printFlag=0;            break; // turn off the note printer
            case 'P': printFlag = 1;          break; // turn on the note printer
            case 't': Music_SetBPM(0);        break; // Put the song back to the default
            case 'T': Music_SetBPM(60);       break; // Put the current song to 60BPM
            case '1': Music_PlaySong(0,0);    break; // Play the 1st Song
            case '2': Music_PlaySong(1,0);    break; // Play the 2nd Song
            case 's':
                UART_UartPutString("Stop\n");
                Music_Stop();
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
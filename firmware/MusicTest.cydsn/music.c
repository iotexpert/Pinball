#include "music.h"
#include <project.h>
#include <stdio.h>

#define NOSONG 0xFF
#define PWMDIVIDER (128)
#define CLOCK (12000000)
#define BPM (25)

int silenceCount;
int nextNote=0;
int currentSong=NOSONG; // no song
int noteDelayCount=0;

#define DELAY_NOTE 32

Note maryHadALittleLamb[] = {
    { 14, 0 },
    { NoteC4,QUARTER_NOTE},
    { NoteC4,QUARTER_NOTE},
    { NoteG4,QUARTER_NOTE},
    { NoteG4,QUARTER_NOTE},
    { NoteA4,QUARTER_NOTE},
    { NoteA4,QUARTER_NOTE},
    { NoteG4,HALF_NOTE},
    { NoteF4,QUARTER_NOTE},
    { NoteF4,QUARTER_NOTE},
    { NoteE4,QUARTER_NOTE},
    { NoteE4,QUARTER_NOTE},
    { NoteD4,QUARTER_NOTE},
    { NoteD4,QUARTER_NOTE},
    { NoteC4,HALF_NOTE}
};

int newNoteFlag=0;

Note *Songs[] = { maryHadALittleLamb };
int Music_GetNote() { return nextNote; }
char buff[128];

void Music_NextNote()
{
    // calculate the PWM Period and Compare
    
   
    newNoteFlag = nextNote;
    nextnotepin_Write(!nextnotepin_Read());
    PWM_Stop();
    int period = CLOCK/PWMDIVIDER/Songs[currentSong][nextNote].frequency;
    PWM_WriteCounter(period);
    PWM_WritePeriod(period);
    PWM_WriteCompare(period>>1);
    PWM_Start();
    noteDelayCount = 60*1000/BPM * Songs[currentSong][nextNote].duration / 64;
    silenceCount = noteDelayCount/5;
    
    nextNote = nextNote + 1;
    
    if(nextNote > Songs[currentSong][0].frequency)
        nextNote = 1;
    
}

void Music_SysTickCallBack()
{
    if(currentSong == NOSONG)
        return;
    
    noteDelayCount -= 1;
    
    if(noteDelayCount == silenceCount)
    {
        PWM_Stop();
    }
    
    if(noteDelayCount == 0)
        Music_NextNote();
 
}

void Music_Start(int sysTickNumber)
{
    CySysTickSetCallback(sysTickNumber,Music_SysTickCallBack);
    
}

void Music_Stop()
{
    currentSong = NOSONG;
    PWM_Stop();
}


void Music_PlaySong(int number)
{
    currentSong = number;
    nextNote = 1;
    Music_NextNote();
}

void Music_SetBPM(int beatsPerMinute)
{
    (void)beatsPerMinute;
}


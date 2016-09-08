#include "music.h"
#include <project.h>
#include <stdio.h>

#define NOSONG 0xFF
#define PWMDIVIDER (128)
#define CLOCK (12000000)

static int Music_BPM = 25;
static int Music_nextNote=0;
static int Music_currentSong=NOSONG; // no song
static int Music_noteDelayCount=0;
static int Music_silenceCount=0;

static Note Music_twinkleLittleStar[] = {
    { 14, 25 },
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

static Note *Music_Songs[] = { Music_twinkleLittleStar, 0 , 0, 0 }; // up to 4 songs

int Music_GetNote()
{
    return Music_nextNote;
}

void Music_NextNote()
{
    // calculate the PWM Period and Compare
    PWM_Stop();
    int period = CLOCK/PWMDIVIDER/Music_Songs[Music_currentSong][Music_nextNote].frequency;
    PWM_WriteCounter(period);
    PWM_WritePeriod(period);
    PWM_WriteCompare(period>>1);
    PWM_Start();
    Music_noteDelayCount = 60*1000/Music_BPM * Music_Songs[Music_currentSong][Music_nextNote].duration / 64;
    Music_silenceCount = Music_noteDelayCount/5;
    
    Music_nextNote += 1;
    
    if(Music_nextNote > Music_Songs[Music_currentSong][0].frequency)
        Music_nextNote = 1;
    
}

void Music_SysTickCallBack()
{
    if(Music_currentSong == NOSONG)
        return;
    
    Music_noteDelayCount -= 1;
    
    if(Music_noteDelayCount == Music_silenceCount)
    {
        PWM_Stop();
    }
    
    if(Music_noteDelayCount == 0)
        Music_NextNote();
 
}

void Music_Start(int sysTickNumber)
{
    CySysTickSetCallback(sysTickNumber,Music_SysTickCallBack);
    
}

void Music_Stop()
{
    Music_currentSong = NOSONG;
    PWM_Stop();
}


void Music_PlaySong(int number)
{
    Music_currentSong = number;
    Music_BPM = Music_Songs[Music_currentSong][0].duration;
    Music_nextNote = 1;
    Music_NextNote();
}

void Music_SetBPM(int beatsPerMinute)
{
    if(beatsPerMinute)
        Music_BPM = beatsPerMinute;
    else
        Music_BPM = Music_Songs[Music_currentSong][0].duration; // set it back to default
}

void Music_AddSong(int number,Note song[])
{
    Music_Songs[number] = song;
}

#include "`$INSTANCE_NAME`_MusicPlayer.h"
#include <project.h>

#define NOSONG 0xFF
#define `$INSTANCE_NAME`_PWM0DIVIDER (128)
#define CLOCK (12000000)

static int `$INSTANCE_NAME`_BPM = 25;
static int `$INSTANCE_NAME`_nextNote=0;
static int `$INSTANCE_NAME`_currentSong=NOSONG; // no song
static int `$INSTANCE_NAME`_noteDelayCount=0;
static int `$INSTANCE_NAME`_silenceCount=0;

static Note `$INSTANCE_NAME`_twinkleLittleStar[] = {
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

static Note *`$INSTANCE_NAME`_Songs[] = { `$INSTANCE_NAME`_twinkleLittleStar, 0 , 0, 0 }; // up to 4 songs

int `$INSTANCE_NAME`_GetNote()
{
    return `$INSTANCE_NAME`_nextNote;
}

void `$INSTANCE_NAME`_NextNote()
{
    // calculate the `$INSTANCE_NAME`_PWM0 Period and Compare
    `$INSTANCE_NAME`_PWM0_Stop();
    int period = CLOCK/`$INSTANCE_NAME`_PWM0DIVIDER/`$INSTANCE_NAME`_Songs[`$INSTANCE_NAME`_currentSong][`$INSTANCE_NAME`_nextNote].frequency;
    `$INSTANCE_NAME`_PWM0_WriteCounter(period);
    `$INSTANCE_NAME`_PWM0_WritePeriod(period);
    `$INSTANCE_NAME`_PWM0_WriteCompare(period>>1);
    `$INSTANCE_NAME`_PWM0_Start();
    `$INSTANCE_NAME`_noteDelayCount = 60*1000/`$INSTANCE_NAME`_BPM * `$INSTANCE_NAME`_Songs[`$INSTANCE_NAME`_currentSong][`$INSTANCE_NAME`_nextNote].duration / 64;
    `$INSTANCE_NAME`_silenceCount = `$INSTANCE_NAME`_noteDelayCount/5;
    
    `$INSTANCE_NAME`_nextNote += 1;
    
    if(`$INSTANCE_NAME`_nextNote > `$INSTANCE_NAME`_Songs[`$INSTANCE_NAME`_currentSong][0].frequency)
        `$INSTANCE_NAME`_nextNote = 1;
    
}

void `$INSTANCE_NAME`_SysTickCallBack()
{
    if(`$INSTANCE_NAME`_currentSong == NOSONG)
        return;
    
    `$INSTANCE_NAME`_noteDelayCount -= 1;
    
    if(`$INSTANCE_NAME`_noteDelayCount == `$INSTANCE_NAME`_silenceCount)
    {
        `$INSTANCE_NAME`_PWM0_Stop();
    }
    
    if(`$INSTANCE_NAME`_noteDelayCount == 0)
        `$INSTANCE_NAME`_NextNote();
 
}

void `$INSTANCE_NAME`_Start(int sysTickNumber)
{
    CySysTickSetCallback(sysTickNumber,`$INSTANCE_NAME`_SysTickCallBack);
    
}

void `$INSTANCE_NAME`_Stop()
{
    `$INSTANCE_NAME`_currentSong = NOSONG;
    `$INSTANCE_NAME`_PWM0_Stop();
}


void `$INSTANCE_NAME`_PlaySong(int number)
{
    `$INSTANCE_NAME`_currentSong = number;
    `$INSTANCE_NAME`_BPM = `$INSTANCE_NAME`_Songs[`$INSTANCE_NAME`_currentSong][0].duration;
    `$INSTANCE_NAME`_nextNote = 1;
    `$INSTANCE_NAME`_NextNote();
}

void `$INSTANCE_NAME`_SetBPM(int beatsPerMinute)
{
    if(beatsPerMinute)
        `$INSTANCE_NAME`_BPM = beatsPerMinute;
    else
        `$INSTANCE_NAME`_BPM = `$INSTANCE_NAME`_Songs[`$INSTANCE_NAME`_currentSong][0].duration; // set it back to default
}

void `$INSTANCE_NAME`_AddSong(int number,Note song[])
{
    `$INSTANCE_NAME`_Songs[number] = song;
}

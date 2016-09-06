#include "music.h"
#include <project.h>

#define NOSONG 0xFF
#define PWMDIVIDER (128)
#define CLOCK (12000000)

int nextNote=0;
int currentSong=NOSONG; // no song
int noteDelayCount=0;

void Music_NextNote()
{
    // calculate the PWM Period and Compare
    
    PWM_Stop();
    int period = CLOCK/PWMDIVIDER/Songs[currentSong][nextNote].frequency;
    PWM_WritePeriod(period);
    PWM_WriteCompare(period>>1);
    PWM_Start();
    noteDelayCount = 60*1000/BPM * Songs[currentSong][nextNote].duration / 64;
    
    nextNote = nextNote + 1;
    
    if(nextNote > Songs[currentSong][0].frequency)
        nextNote = 1;
    
}

void Music_SysTickCallBack()
{
    if(currentSong == NOSONG)
        return;
    
    if(noteDelayCount <= 0)
        Music_NextNote();
    else
        noteDelayCount -= 1;
    
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


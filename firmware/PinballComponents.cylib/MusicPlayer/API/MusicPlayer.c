#include <project.h>
#include "`$INSTANCE_NAME`_MusicPlayer.h"

#define `$INSTANCE_NAME`_NO_SONG (0xFF)
#define `$INSTANCE_NAME`_PWM_DIVIDER (128)
#define CLOCK (12000000)

static int `$INSTANCE_NAME`_BPM = 25; // current tempo
static int `$INSTANCE_NAME`_nextNote=0; // what is the next note in the song
static int `$INSTANCE_NAME`_currentSong=`$INSTANCE_NAME`_NO_SONG; // no song
static int `$INSTANCE_NAME`_noteDelayCount=0; // how much time is left in the current note
static int `$INSTANCE_NAME`_silenceCount=0; // where you turn off the note

#ifdef `$INSTANCE_NAME`_TWOCHANNELS

// If there are two channels... then have the variable to turn off the PWM
static int `$INSTANCE_NAME`_pwm1_offTime;
#endif

// Mozarts classic song..
// the first structure holds the # of notes and the BPM of the song
static Note `$INSTANCE_NAME`_twinkleLittleStarNotes[] = {
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

static Song `$INSTANCE_NAME`_twinkleLittleStarSong = {
    .numNotes = sizeof(`$INSTANCE_NAME`_twinkleLittleStarNotes)/sizeof(Note),
    .bpm = 25,
    .notes = `$INSTANCE_NAME`_twinkleLittleStarNotes
    };


// Allocate an array to hold songs
#define `$INSTANCE_NAME`_MAX_SONGS (4)
static Song *`$INSTANCE_NAME`_Songs[`$INSTANCE_NAME`_MAX_SONGS];


/*****************************************************************************\
 * Function:    `$INSTANCE_NAME`_GetNotes
 * Input:       void
 * Returns:     int = Returns the note number of the currently playing song
 * Description: 
 *     Returns the note number of the currently playing song
\*****************************************************************************/
int `$INSTANCE_NAME`_GetNote()
{
    return `$INSTANCE_NAME`_nextNote;
}

/*****************************************************************************\
 * Function:    `$INSTANCE_NAME`_frequencyToPeriod
 * Input:       the frequency of the buzzer
 * Returns:     the period to set PWM to acheive the input frequency
 * Description: 
 *     A helper to calculate the period given the PWM settings & clock
\*****************************************************************************/
static inline int `$INSTANCE_NAME`_frequencyToPeriod(int frequency)
{
    return CLOCK/`$INSTANCE_NAME`_PWM_DIVIDER/frequency;
}

/*****************************************************************************\
 * Function:    `$INSTANCE_NAME`_NextNote()
 * Input:       void
 * Returns:     void
 * Description: 
 *     This function is called by the timer to move to the next note in the
 *     song.  I can also be used to startup a song playing
\*****************************************************************************/
static void `$INSTANCE_NAME`_NextNote()
{
    `$INSTANCE_NAME`_PWM0_Stop();
    // calculate the `$INSTANCE_NAME`_PWM0 Period and Compare
    int period = `$INSTANCE_NAME`_frequencyToPeriod(`$INSTANCE_NAME`_Songs[`$INSTANCE_NAME`_currentSong]->notes[`$INSTANCE_NAME`_nextNote].frequency);
    `$INSTANCE_NAME`_PWM0_WriteCounter(period);
    `$INSTANCE_NAME`_PWM0_WritePeriod(period);
    `$INSTANCE_NAME`_PWM0_WriteCompare(period>>1);
    `$INSTANCE_NAME`_PWM0_Start();
    `$INSTANCE_NAME`_noteDelayCount = 60*1000/`$INSTANCE_NAME`_BPM * `$INSTANCE_NAME`_Songs[`$INSTANCE_NAME`_currentSong]->notes[`$INSTANCE_NAME`_nextNote].duration / 64;
    
    // the 5 means that the note will PLAY for 0.8 of a and will be silent 0.2
    `$INSTANCE_NAME`_silenceCount = `$INSTANCE_NAME`_noteDelayCount/5;
    
    `$INSTANCE_NAME`_nextNote += 1;
    
    // if you get to the end... then wrap around
    if(`$INSTANCE_NAME`_nextNote == `$INSTANCE_NAME`_Songs[`$INSTANCE_NAME`_currentSong]->numNotes)
        `$INSTANCE_NAME`_nextNote = 0;
    
}

/*****************************************************************************\
 * Function:    `$INSTANCE_NAME`_SysTickCallBack()
 * Input:       void
 * Returns:     void
 * Description: 
 *     This function is called by the SysTick timer.  It has two counters
 *     one for PWM0 and one for PWM1 to time the buzzing
\*****************************************************************************/
static void `$INSTANCE_NAME`_SysTickCallBack()
{
    
#ifdef `$INSTANCE_NAME`_TWOCHANNELS
    if(`$INSTANCE_NAME`_pwm1_offTime)
    {
        `$INSTANCE_NAME`_pwm1_offTime -= 1;
        if(`$INSTANCE_NAME`_pwm1_offTime == 0)
        {
             `$INSTANCE_NAME`_PWM1_Stop();   
        }
    }
#endif

    if(`$INSTANCE_NAME`_currentSong == `$INSTANCE_NAME`_NO_SONG)
        return;
    
    `$INSTANCE_NAME`_noteDelayCount -= 1;

    // if it is time to turn off the PWM (for the non-buzzing part of the note
    if(`$INSTANCE_NAME`_noteDelayCount == `$INSTANCE_NAME`_silenceCount)
    {
        `$INSTANCE_NAME`_PWM0_Stop();
    }
    
    if(`$INSTANCE_NAME`_noteDelayCount == 0)
        `$INSTANCE_NAME`_NextNote();
 
}

/*****************************************************************************\
 * Function:    `$INSTANCE_NAME`_Start(int sysTickNumber)
 * Input:       sysTickNumber - which sysTick vector your want to use
 * Returns:     void
 * Description: 
 *     This function starts the component by registering the systick callback
\*****************************************************************************/
void `$INSTANCE_NAME`_Start(int sysTickNumber)
{
    `$INSTANCE_NAME`_AddSong(0,&`$INSTANCE_NAME`_twinkleLittleStarSong);
#ifdef `$INSTANCE_NAME`_TWOCHANNELS
    `$INSTANCE_NAME`_pwm1_offTime = 0;
#endif
    `$INSTANCE_NAME`_nextNote=0; // no next note
    `$INSTANCE_NAME`_currentSong=`$INSTANCE_NAME`_NO_SONG; // no song
    CySysTickSetCallback(sysTickNumber,`$INSTANCE_NAME`_SysTickCallBack);
}

/*****************************************************************************\
 * Function:    `$INSTANCE_NAME`_Stop()
 * Input:       void
 * Returns:     void
 * Description: 
 *     This is a helper function
\*****************************************************************************/
void `$INSTANCE_NAME`_Stop()
{
    `$INSTANCE_NAME`_currentSong = `$INSTANCE_NAME`_NO_SONG;
    `$INSTANCE_NAME`_PWM0_Stop();
}

/*****************************************************************************\
 * Function:    `$INSTANCE_NAME`_PlaySong(int number,int bpm)
 * Input:       Which song... and what temp(aka BPM).. bpm=0 means default tempo
 * Returns:     ErrorCodes
 * Description: 
 *     Start a song playing
\*****************************************************************************/
`$INSTANCE_NAME`_ErrorCodes `$INSTANCE_NAME`_PlaySong(int number,int bpm)
{
    `$INSTANCE_NAME`_ErrorCodes rval = OK;
    
    if(number < 0 || number >= `$INSTANCE_NAME`_MAX_SONGS)
        return INVALID_SONG_NUMBER;
    if(`$INSTANCE_NAME`_Songs[number] == (Song *)0)
        return NO_SONG;
    
    `$INSTANCE_NAME`_currentSong = number;
    rval = `$INSTANCE_NAME`_SetBPM(bpm);
    `$INSTANCE_NAME`_nextNote = 0;
    `$INSTANCE_NAME`_NextNote();
    return rval;
}
/*****************************************************************************\
 * Function:    `$INSTANCE_NAME`_SetBPM
 * Input:       int beatsPerMinute
 * Returns:     ErrorCodes
 * Description: 
 *     Set the tempo of the playback in BPM
\*****************************************************************************/
`$INSTANCE_NAME`_ErrorCodes `$INSTANCE_NAME`_SetBPM(int beatsPerMinute)
{
    // ARH decided that >250 is stupid
    if(beatsPerMinute<0 || beatsPerMinute > 250)
        return OUT_OF_RANGE;
    if(beatsPerMinute)
        `$INSTANCE_NAME`_BPM = beatsPerMinute;
    else
        `$INSTANCE_NAME`_BPM = `$INSTANCE_NAME`_Songs[`$INSTANCE_NAME`_currentSong]->bpm; // set it back to default
    return OK;
}
/*****************************************************************************\
 * Function:    `$INSTANCE_NAME`_AddSong
 * Input:       The song number + an array of notes
 * Returns:     ErrorCodes
 * Description: 
 *     This function adds a new song to the song table... it must be less than
 *     the maximum number of songs
\*****************************************************************************/
`$INSTANCE_NAME`_ErrorCodes `$INSTANCE_NAME`_AddSong(int number,Song *song)
{
    if(number<0 || number>= `$INSTANCE_NAME`_MAX_SONGS)
        return OUT_OF_RANGE;
    `$INSTANCE_NAME`_Songs[number] = song;
    return OK;
}

#ifdef `$INSTANCE_NAME`_TWOCHANNELS
/*****************************************************************************\
 * Function:    `$INSTANCE_NAME`_BuzzOn(int frequency,int timeMs)
 * Input:       the frequency and length of time in MS to play the buzzer
 * Returns:     ErrorCodes
 * Description: 
 *     This function turns on the buzzer for timeMs milliseconds.  If you do 
 *     a 0 for the time it will run until you manually turn it off
\*****************************************************************************/
`$INSTANCE_NAME`_ErrorCodes `$INSTANCE_NAME`_BuzzOn(int frequency,int timeMs)
{
    // ARH decided <50hz and >10khz is stupid
    if(frequency<50 || frequency>10000)
        return OUT_OF_RANGE;
    
    if(timeMs<0)
        return OUT_OF_RANGE;
    
     int period = `$INSTANCE_NAME`_frequencyToPeriod(frequency);
    `$INSTANCE_NAME`_PWM1_Start();
    `$INSTANCE_NAME`_PWM1_WriteCounter(period);
    `$INSTANCE_NAME`_PWM1_WritePeriod(period);
    `$INSTANCE_NAME`_PWM1_WriteCompare(period>>1);
    `$INSTANCE_NAME`_pwm1_offTime = timeMs;
    return OK;
}

/*****************************************************************************\
 * Function:    `$INSTANCE_NAME`_BuzzOff()
 * Input:       void
 * Returns:     void
 * Description: 
 *     Turn of the 2nd Buzzer
\*****************************************************************************/
void `$INSTANCE_NAME`_BuzzOff()
{
    `$INSTANCE_NAME`_PWM1_Stop();
    `$INSTANCE_NAME`_pwm1_offTime = 0;
}
#endif
#ifndef `$INSTANCE_NAME`_MUSIC_PLAYER_H
#define `$INSTANCE_NAME`_MUSIC_PLAYER_H

// This #define turns on the APIs for the 2nd PWM channel
`=($TwoChannels)?"#define `$INSTANCE_NAME`_TWOCHANNELS":""`
    
typedef struct Note {
    int frequency;
    int duration; // duration in 64's of a note aka whole note = 64
} Note;

#define NoteA4 (440)
#define NoteB4 (494)
#define NoteC4 (262)
#define NoteD4 (294)
#define NoteE4 (330)
#define NoteF4 (349)
#define NoteG4 (392)

#define WHOLE_NOTE (64)
#define HALF_NOTE (32)
#define QUARTER_NOTE (16)

typedef enum `$INSTANCE_NAME`_ErrorCodes {
    OK,
    NO_SONG,
    INVALID_SONG_NUMBER,
    OUT_OF_RANGE
    
}`$INSTANCE_NAME`_ErrorCodes;

typedef struct Song {
    int numNotes;
    int bpm;
    Note *notes;
} Song;

void `$INSTANCE_NAME`_Start(int sysTickNumber);
`$INSTANCE_NAME`_ErrorCodes `$INSTANCE_NAME`_PlaySong(int number,int bpm);
void `$INSTANCE_NAME`_Stop();
`$INSTANCE_NAME`_ErrorCodes `$INSTANCE_NAME`_SetBPM(int beatsPerMinute);
int `$INSTANCE_NAME`_GetNote();
`$INSTANCE_NAME`_ErrorCodes `$INSTANCE_NAME`_AddSong(int number,Song *song);

// If the 2nd channel is on then turn on the buzzing APIs
#ifdef `$INSTANCE_NAME`_TWOCHANNELS
`$INSTANCE_NAME`_ErrorCodes `$INSTANCE_NAME`_BuzzOn(int frequency,int timeMs);
void `$INSTANCE_NAME`_BuzzOff();
#endif

#endif   
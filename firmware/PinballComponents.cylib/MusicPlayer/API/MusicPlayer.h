#ifndef `$INSTANCE_NAME`_MUSIC_PLAYER_H
#define `$INSTANCE_NAME`_MUSIC_PLAYER_H
 
    
typedef struct Note {
    int frequency;
    int duration; // duration in 64's of a note
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

void `$INSTANCE_NAME`_Start(int sysTickNumber);
void `$INSTANCE_NAME`_PlaySong(int number);
void `$INSTANCE_NAME`_Stop();
void `$INSTANCE_NAME`_SetBPM(int beatsPerMinute);
int `$INSTANCE_NAME`_GetNote();
    
#endif   
#ifndef MUSIC_H
#define MUSIC_H
 
    
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

void Music_Start(int sysTickNumber);
void Music_PlaySong(int number);
void Music_Stop();
void Music_SetBPM(int beatsPerMinute);
int Music_GetNote();
    
#endif    
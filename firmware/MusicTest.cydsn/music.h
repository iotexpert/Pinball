#ifndef MUSIC_H
#define MUSIC_H
 
#define BPM (110)
    
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


Note maryHadALittleLamb[] = {
    { 14, 0 },
    { NoteC4,WHOLE_NOTE},
    { NoteC4,WHOLE_NOTE},
    { NoteG4,WHOLE_NOTE},
    { NoteG4,WHOLE_NOTE},
    { NoteA4,WHOLE_NOTE},
    { NoteA4,WHOLE_NOTE},
    { NoteG4,WHOLE_NOTE},
    { NoteF4,WHOLE_NOTE},
    { NoteF4,WHOLE_NOTE},
    { NoteE4,WHOLE_NOTE},
    { NoteE4,WHOLE_NOTE},
    { NoteD4,WHOLE_NOTE},
    { NoteD4,WHOLE_NOTE},
    { NoteC4,WHOLE_NOTE}
};


Note *Songs[] = { maryHadALittleLamb };


void Music_Start(int sysTickNumber);
void Music_PlaySong(int number);
void Music_Stop();
void Music_SetBPM(int beatsPerMinute);
    
#endif    
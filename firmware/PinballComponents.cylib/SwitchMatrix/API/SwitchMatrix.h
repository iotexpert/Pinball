#ifndef SwitchMatrix_SWITCHMATRIX_H
#include <stdint.h>
#define SwitchMatrix_SWITCHMATRIX_H    
#define SwitchMatrix_NUMCOLS (3)
#define SwitchMatrix_NUMROWS (3)

#define SwitchMatrix_MASK_TYPE uint32_t
    
typedef enum SwitchMatrix_Inter_Mode {
    NONE,
    RISING,
    FALLING,
    BOTH
} SwitchMatrix_Inter_Mode;
    
void SwitchMatrix_Start(int intNumber);
int SwitchMatrix_Read(int row,int col);
SwitchMatrix_MASK_TYPE SwitchMatrix_ReadAll();
void SwitchMatrix_SetInterruptMode(int row,int col,SwitchMatrix_Inter_Mode mode);



#endif

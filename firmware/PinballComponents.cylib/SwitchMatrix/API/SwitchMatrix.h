#ifndef `$INSTANCE_NAME`_SWITCHMATRIX_H
#include <stdint.h>
#define `$INSTANCE_NAME`_SWITCHMATRIX_H    


#define `$INSTANCE_NAME`_MASK_TYPE uint64_t
    
typedef enum `$INSTANCE_NAME`_Inter_Mode {
    NONE,
    RISING,
    FALLING,
    BOTH
} `$INSTANCE_NAME`_Inter_Mode;
    
void `$INSTANCE_NAME`_Start(int intNumber);
int `$INSTANCE_NAME`_Read(int row,int col);
`$INSTANCE_NAME`_MASK_TYPE `$INSTANCE_NAME`_ReadAll();
void `$INSTANCE_NAME`_SetInterruptMode(int row,int col,`$INSTANCE_NAME`_Inter_Mode mode);
`$INSTANCE_NAME`_MASK_TYPE `$INSTANCE_NAME`_ClearInterrupt();
`$INSTANCE_NAME`_MASK_TYPE `$INSTANCE_NAME`_ReadFallingInterruptStatus();
`$INSTANCE_NAME`_MASK_TYPE `$INSTANCE_NAME`_ReadRisingInterruptStatus();


#endif

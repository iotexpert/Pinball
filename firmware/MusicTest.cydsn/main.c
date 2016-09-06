/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#include <project.h>
#include "music.h"

int main()
{
    CyGlobalIntEnable; /* Enable global interrupts. */

    CySysTickStart();
    
    Music_Start(0);
    
    Music_PlaySong(0);

    for(;;)
    {
        /* Place your application code here. */
    }
}

/* [] END OF FILE */

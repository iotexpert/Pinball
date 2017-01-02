#include "project.h"
#include "u8g2.h"
#include <stdio.h>


/* 
 * A PSoC I2C Hardware Interface for U8X8
*/

uint8_t u8x8_byte_hw_i2c(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr)
{
    #ifdef DEBUG_UART
    char buff[20];
    #endif
    uint8_t *data;
    switch(msg)
    {
        case U8X8_MSG_BYTE_SEND:
            data = (uint8_t *)arg_ptr;
            while( arg_int > 0 )
            {
                #ifdef DEBUG_UART
                sprintf(buff,"%x\n",*data);
                UART_UartPutString(buff);
                #endif
                (void)I2C_I2CMasterWriteByte(*data); // No Error Checking..
  	            data++;
	            arg_int--;
            }
        break;
      
        case U8X8_MSG_BYTE_INIT: // Using the HW block you dont need to set 1/1
        break;
        case U8X8_MSG_BYTE_SET_DC:
        break;
        case U8X8_MSG_BYTE_START_TRANSFER:
            (void)I2C_I2CMasterSendStart(u8x8_GetI2CAddress(u8x8)>>1,I2C_I2C_WRITE_XFER_MODE); // no Error Checking
         break;
        
        case U8X8_MSG_BYTE_END_TRANSFER:
            (void)I2C_I2CMasterSendStop(); // no error checking
        break;
    
        default:
            return 0;
    }
    
    return 1;
}

/* 
 * A PSoC implementation of the delay and gpio callback:
 * 
*/
uint8_t psoc_gpio_and_delay_cb(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr)
{
    (void)u8x8;
    (void)arg_ptr;
    switch(msg)
    {
        case U8X8_MSG_GPIO_AND_DELAY_INIT: // for now who cares?
        break;
        case U8X8_MSG_DELAY_MILLI:
            CyDelay(arg_int);
        break;
        case U8X8_MSG_DELAY_10MICRO:
            CyDelayUs(10);
        break;
        case U8X8_MSG_DELAY_100NANO:
            CyDelayCycles((CYDEV_BCLK__SYSCLK__HZ/1000000) * 100 - 16); //PSoC system reference guide says ~16 extra cycles 
        break;
        /*    - My Display has only I2C... which I have implemented in HW
         * If you want to use a software interface or have these pins then you 
         * need to read and write them
        case U8X8_MSG_GPIO_SPI_CLOCK:
        case U8X8_MSG_GPIO_SPI_DATA:
        case U8X8_MSG_GPIO_CS:
        case U8X8_MSG_GPIO_DC:
        case U8X8_MSG_GPIO_RESET:
        case U8X8_MSG_GPIO_SPI_CLOCK:
        case U8X8_MSG_GPIO_SPI_DATA:		
        case U8X8_MSG_GPIO_D0:    
        case U8X8_MSG_GPIO_D1:			
        case U8X8_MSG_GPIO_D2:		
        case U8X8_MSG_GPIO_D3:		
        case U8X8_MSG_GPIO_D4:	
        case U8X8_MSG_GPIO_D5:	
        case U8X8_MSG_GPIO_D6:	
        case U8X8_MSG_GPIO_D7:
        case U8X8_MSG_GPIO_E: 	
        case U8X8_MSG_GPIO_I2C_CLOCK:
        case U8X8_MSG_GPIO_I2C_DATA:
        break;  */
    }
    return 1;
}

void u8x8Main()
{
    CyDelay(1000);
    CyGlobalIntEnable; /* Enable global interrupts. */

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    u8x8_t u8x8;
    

    I2C_Start();
    UART_Start();
        
    UART_UartPutString("Calling Setup\n");
    
    u8x8_Setup(&u8x8, u8x8_d_ssd1306_128x64_noname, u8x8_cad_ssd13xx_i2c, u8x8_byte_hw_i2c, psoc_gpio_and_delay_cb);
    
    //u8x8_Setup(&u8x8, u8x8_d_sh1106_128x64_noname, u8x8_cad_ssd13xx_i2c, u8x8_byte_hw_i2c, psoc_gpio_and_delay_cb);
    
        
    UART_UartPutString("Calling Init\n");
    u8x8_InitDisplay(&u8x8);  

    u8x8_SetPowerSave(&u8x8,0);
    
    UART_UartPutString("Calling ClearDisplay\n");
    u8x8_ClearDisplay(&u8x8);
    
    UART_UartPutString("Calling Set Font\n");
    u8x8_SetFont(&u8x8,u8x8_font_amstrad_cpc_extended_f);
    
   //u8x8_SetFont(&u8x8, u8x8_font_amstrad_cpc_extended_f);
  //  u8x8_SetFont(&u8x8, u8x8_font_amstrad_cpc_extended_f);
  
    UART_UartPutString("Sending drawstring\n");
     //u8x8_DrawString(&u8x8, 0, 0, "x=0, y=0");
     //u8x8_DrawString(&u8x8, 10, 10, "x=10, y=10");
     //u8x8_DrawString(&u8x8, 20, 20, "x=20, y=20");
    // u8x8_DrawString(&u8x8, 30, 30, "x=30, y=30");

     //u8x8_DrawString(&u8x8, 40, 40, "x=40, y=40");
     //u8x8_DrawString(&u8x8, 50, 50, "x=50, y=50");
     //u8x8_DrawString(&u8x8, 60, 60, "x=60, y=60");
    
// u8g2_Setup_ssd1306_i2c_128x64_noname_2(&u8g2, U8G2_R0, u8x8_byte_sw_i2c, u8x8_gpio_and_delay_lpc11u3x); // Prepare memory for u8glib
//u8g2_InitDisplay(&u8g2); // This will emit U8X8_MSG_GPIO_AND_DELAY_INIT and also send the startup code for the display
//u8g2_SetPowerSave(&u8g2, 0); // this will finally enlight the display, so it might be a clever idea to clean the display RAM first
    
    char buff[10];
    int i;
    int j=0;
    for(i=0;i<8;i=i+1)
    {
        for(j=0;j<16;j++)
        {
            sprintf(buff,"%c",j+65);
            u8x8_DrawString(&u8x8, j, i, buff);
        }
    }
    
    while(1);
}

int main()
{
    

    CyGlobalIntEnable; 
            
    I2C_Start();
    UART_Start();
        
    UART_UartPutString("Calling Setup\n");

    //u8g2Main();
    u8x8Main();
    while(1);
}

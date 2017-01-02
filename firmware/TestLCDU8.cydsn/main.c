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

int updateScreen=0;

void systemCounter()
{
    static int updateCounter=0;
    
    updateCounter = (updateCounter+1)%200;
    if(!updateCounter)
    {
        updateScreen = 1;
    }
}

int main()
{
    
    char buff[20];

    CyGlobalIntEnable; 
    CyGlobalIntEnable; /* Enable global interrupts. */
    
    PWM_1_Start();
    PWM_1_WritePeriod(12000000/440);
    PWM_1_WriteCompare(PWM_1_ReadPeriod()>>1);
    PWM_1_Stop();
    
    PWM_0_Start();
    PWM_0_WritePeriod(12000000/262);
    PWM_0_WriteCompare(PWM_0_ReadPeriod()>>1);
    PWM_0_Stop();
  
            
    I2C_Start();

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    u8x8_t u8x8;
    
    u8x8_Setup(&u8x8, u8x8_d_ssd1306_128x64_noname, u8x8_cad_ssd13xx_i2c, u8x8_byte_hw_i2c, psoc_gpio_and_delay_cb);
        
    u8x8_InitDisplay(&u8x8);  
    u8x8_SetPowerSave(&u8x8,0);
    u8x8_ClearDisplay(&u8x8);
    u8x8_SetFont(&u8x8,u8x8_font_amstrad_cpc_extended_f);
    u8x8_DrawString(&u8x8,3,0,"IoT Expert");
    u8x8_DrawString(&u8x8,1,1,"Newton's Attic");
    u8x8_DrawString(&u8x8,4,2,"Pinball");
    
    
    PWM_L_Start();
    PWM_R_Start();
    
    
    CR_L_Write(0);
    CR_R_Write(0);

    CapSense_Start();
    CapSense_InitializeEnabledBaselines();
    CapSense_ScanEnabledWidgets();
    CySysTickStart();
    
    SwitchMatrix_Start(1);
    
    MatrixLed_Start(0);
    
    CySysTickSetCallback(2,systemCounter);
    
    int row=0,col=0;
    MatrixLed_Enable(row,col);
    
    uint8_t b0=0,b1=0,prevb0=0,prevb1=0;
    while(1)
    {
        if(!CapSense_IsBusy())
        {
            
            b0 = CapSense_CheckIsWidgetActive(CapSense_BUTTON0__BTN);
            b1 = CapSense_CheckIsWidgetActive(CapSense_BUTTON1__BTN);
            CapSense_UpdateEnabledBaselines();
            CapSense_ScanEnabledWidgets();
        }
        if(b0 != prevb0)
        {
            //Control_Reg_1_Write(0); // both motors left
            
            u8x8_DrawString(&u8x8,0,5,(b0?"On ":"Off"));
            (b0?PWM_0_Start():PWM_0_Stop());
            if(b0)
            {
                MatrixLed_Disable(row,col);
                col = (col + 1) % 4;
                if(col == 0)
                    row = (row + 1)% 4;
                MatrixLed_Enable(row,col);
                sprintf(buff,"row=%d  col=%d",row,col);
                u8x8_DrawString(&u8x8,0,7,buff);
                
                CR_L_Write(2);
                
            }
            else
                CR_L_Write(0);
        }
        if(b1 != prevb1)
        {
            //Control_Reg_1_Write(3); // both motors right
            u8x8_DrawString(&u8x8,10,5,(b1?"On ":"Off"));
            (b1?PWM_1_Start():PWM_1_Stop());
            if(b1)
                CR_R_Write(2);
            else
                CR_R_Write(0);
        
        }
        
        
        prevb0=b0;
        prevb1=b1;
        
        if(updateScreen)
        {
            sprintf(buff,"Switch = %2X",(int)SwitchMatrix_ReadAll());
            u8x8_DrawString(&u8x8,0,6,buff);
            updateScreen = 0;
        }
    }
}

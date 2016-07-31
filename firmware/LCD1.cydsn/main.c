
#include <project.h>
#include "LiquidCrystal_I2C.h"

void sendData(uint8 val)
{
    sig_Write(1);
    I2C_I2CMasterSendStart(0x7e>>1,I2C_I2C_WRITE_XFER_MODE);
    I2C_I2CMasterWriteByte(val);
    I2C_I2CMasterSendStop();
    sig_Write(0);
   
}

int main()
{
    CyGlobalIntEnable; 
    UART_Start();
      
    for(;;)
    {
        int c = UART_UartGetChar();
        if(c)
            UART_UartPutChar(c);
        switch(c)
        {
            case 0:
            break;
            case 'a':
                LiquidCrystal_I2C_Start(0x7E>>1,16,2);
            break;
            
           case 's':
                LiquidCrystal_I2C_Position(0,0);
           break;
           
           case 'S':
                LiquidCrystal_I2C_Position(0,1);
           break;
           
           case 'b':
                LiquidCrystal_I2C_BlinkOn();
                break;
            case 'B':
                LiquidCrystal_I2C_BlinkOff();
            break;
            case 'q':
                I2C_Start();
            break;
           
           case 'L':
                LiquidCrystal_I2C_BackLightOn();
            break;
           case 'l':
                LiquidCrystal_I2C_BackLightOff();
            
            case 'c':
                LiquidCrystal_I2C_CursorOff();
            break;
            
            case 'C':
                LiquidCrystal_I2C_CursorOn();
            break;
                                
            case 'p':
                LiquidCrystal_I2C_PrintLeft();
            break;
            
            case 'P':
                LiquidCrystal_I2C_PrintRight();
            break;
            
           case 'e':
                LiquidCrystal_I2C_ScrollDisplayRight();
           break;
           
           case 'E':
                LiquidCrystal_I2C_ScrollDisplayLeft();
           break;
                
           case 'w':
                LiquidCrystal_I2C_ClearDisplay();
                break;
                
            case '_':
                sendData(0);
                break;
       
            case '0':
                sendData(0x01);
                break;
            case '1':
                sendData(0x02);
                break;
            case '2':
                sendData(0x04);
                break;
            case '3':
                sendData(0x08);
                break;
            case '4':
                sendData(0x10);
                break;
            case '5':
                sendData(0x20);
                break;
            case '6':
                sendData(0x40);
                break;
            case '7':
                sendData(0x80);
            break;
            
            default:
                LiquidCrystal_I2C_PutChar(c);
                break;
        }
    }
}

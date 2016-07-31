#include <project.h>
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
    I2C_Start();     
    for(;;)
    {
        int c = UART_UartGetChar();
        if(c)
            UART_UartPutChar(c);
        switch(c)
        {
            case 0:
            break;
            case '_': sendData(0); break;
            case '0': sendData(0x01); break;
            case '1': sendData(0x02); break;
            case '2': sendData(0x04); break;
            case '3': sendData(0x08); break;
            case '4': sendData(0x10); break;
            case '5': sendData(0x20); break;
            case '6': sendData(0x40); break;
            case '7': sendData(0x80); break;
        }
    }
}

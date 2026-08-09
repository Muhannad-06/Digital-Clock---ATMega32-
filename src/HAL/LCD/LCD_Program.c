#include "../../LIBs/STD_TYPES.h"
#include "../../LIBs/BIT_MATH.h"
#include "../../MCAL/DIO/DIO_Interface.h"

#include "LCD_Interface.h"
#include "LCD_Private.h"
#include "LCD_Config.h"

#include <util/delay.h>

void LCD_voidInit()
{
	portMode(LCD_CTRL_PORT, DIO_PORT_OUTPUT);
	    portMode(LCD_DATA_PORT, DIO_PORT_OUTPUT);

	    _delay_ms(40);

	    /* 8-bit mode, 2 lines, 5x8 font */
	    LCD_voidSendCOM(0x3F);
	    _delay_us(50);

	    /* Display ON, Cursor ON, Blink ON */
	    LCD_voidSendCOM(0x0F);
	    _delay_us(50);

	    /* Clear display */
	    LCD_voidSendCOM(0x01);
	    _delay_ms(2);

	    /* Entry mode increment */
	    LCD_voidSendCOM(0x06);
	    _delay_ms(2);
}

void LCD_voidSendCOM(u8 Copy_u8Instruction)
{
    /*RS = 0 RW = 0 E = 1*/
    digitalWrite(LCD_CTRL_PORT, LCD_PIN_RS, DIO_PIN_LOW);
    digitalWrite(LCD_CTRL_PORT, LCD_PIN_RW, DIO_PIN_LOW);
    digitalWrite(LCD_CTRL_PORT, LCD_PIN_EN, DIO_PIN_HIGH);

    digitalWritePORT(LCD_DATA_PORT, Copy_u8Instruction);

    /* E  1 --> 0 */
    digitalWrite(LCD_CTRL_PORT, LCD_PIN_EN, DIO_PIN_HIGH);
    _delay_ms(5);
    digitalWrite(LCD_CTRL_PORT, LCD_PIN_EN, DIO_PIN_LOW);
    
}

void LCD_voidWriteCHAR(u8 Copy_u8DATA)
{
    /* RS = 1  RW = 0 */
    digitalWrite(LCD_CTRL_PORT, LCD_PIN_RS, DIO_PIN_HIGH);
    digitalWrite(LCD_CTRL_PORT, LCD_PIN_RW, DIO_PIN_LOW);
    digitalWrite(LCD_CTRL_PORT, LCD_PIN_EN, DIO_PIN_HIGH);

    digitalWritePORT(LCD_DATA_PORT, Copy_u8DATA);
    /* E  1 -> 0*/
    digitalWrite(LCD_CTRL_PORT, LCD_PIN_EN, DIO_PIN_HIGH);
    _delay_ms(5);
    digitalWrite(LCD_CTRL_PORT, LCD_PIN_EN, DIO_PIN_LOW);
}

void LCD_voidWriteSTRING(u8* Copy_u8String)
{
    u8 i = 0;
    if( Copy_u8String != NULL ){
        while (Copy_u8String[i] != '\0')
        {
            LCD_voidWriteCHAR(Copy_u8String[i]);
            i++;
        }
    }
}

void LCD_voidGoToXY(u8 Copy_u8ROW, u8 Copy_u8COL)
{
	u8 XY_u8DDRAM_ADDRESS = 0;
	    if(Copy_u8ROW == 0)
	    {
	        XY_u8DDRAM_ADDRESS = Copy_u8COL;
	    }
	    else if(Copy_u8ROW == 1)
	    {
	        XY_u8DDRAM_ADDRESS = 0x40 + Copy_u8COL;
	    }

	    /* Bit 7 must be high to set DDRAM address (0x80) */
	    LCD_voidSendCOM(0x80 | XY_u8DDRAM_ADDRESS);
}

void LCD_voidClearDisplay()
{
	LCD_voidSendCOM(0x01);
    _delay_ms(2);
}

void LCD_voidCreateCustomChar(u8* Copy_u8Pattern, u8 Copy_u8Location)
{
    Copy_u8Location &= 0x07;
	LCD_voidSendCOM( 0x40 | (Copy_u8Location << 3));
	for(u8 i=0 ; i<8 ; i++)
	{
		LCD_voidWriteCHAR(Copy_u8Pattern[i]);
	}
	LCD_voidSendCOM(0x80);
}

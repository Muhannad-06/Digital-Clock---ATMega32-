/*
 *<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<   LCD_Program.c   >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
 *
 *  Author : Muhannad Mahfouz El-Shahiedy
 *  Layer  : HAL
 *  HW    : LCD
 *
 */ 

#include "../../LIBs/STD_TYPES.h"
#include "../../LIBs/BIT_MATH.h"
#include "../../MCAL/DIO/DIO_Interface.h"

#include "LCD_Interface.h"
#include "LCD_Private.h"
#include "LCD_Config.h"

//#include <util/delay.h>

void LCD_voidInit()
{

		portMode(LCD_CTRL_PORT, DIO_PORT_OUTPUT);
	    portMode(LCD_DATA_PORT, DIO_PORT_OUTPUT);

	//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<      8 Bits Mode       >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

	#if  LCD_MODE == LCD_8BITS
	    _delay_ms(40);

		portMode  ( LCD_DATA_PORT, DIO_PORT_OUTPUT);
		pinMode  ( LCD_CTRL_PORT , LCD_PIN_RS , DIO_PIN_OUTPUT  );
	    pinMode  ( LCD_CTRL_PORT , LCD_PIN_RW , DIO_PIN_OUTPUT  );
	    pinMode  ( LCD_CTRL_PORT , LCD_PIN_EN , DIO_PIN_OUTPUT  );

	    /* 8-bit mode, 2 lines, 5x8 font */
	    LCD_voidSendCommand(lcd_home);
	    _delay_us(50);

		LCD_voidSendCommand(EIGHT_BITS);
	    _delay_us(50);

	    /* Display ON, Cursor ON, Blink ON */
	    LCD_voidSendCommand(lcd_DisplayOn_CursorOn);
	    _delay_us(50);

	    /* Clear display */
	    LCD_voidSendCommand(lcd_Clear);
	    _delay_ms(2);

	    /* Entry mode increment */
	    LCD_voidSendCommand(lcd_EntryMode);
	    _delay_ms(2);
	//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<      4 Bits Mode       >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

	#elif  LCD_MODE == LCD_4BITS
	    _delay_ms(40);

	pinMode  ( LCD_DATA_PORT, DIO_PIN4,DIO_PIN_OUTPUT);
	pinMode  ( LCD_DATA_PORT, DIO_PIN5,DIO_PIN_OUTPUT);
	pinMode  ( LCD_DATA_PORT, DIO_PIN6,DIO_PIN_OUTPUT);
	pinMode  ( LCD_DATA_PORT, DIO_PIN7,DIO_PIN_OUTPUT);
	pinMode  ( LCD_CTRL_PORT , LCD_PIN_RS , DIO_PIN_OUTPUT  );
	pinMode  ( LCD_CTRL_PORT , LCD_PIN_RW , DIO_PIN_OUTPUT  );
	pinMode  ( LCD_CTRL_PORT , LCD_PIN_EN , DIO_PIN_OUTPUT  );

		LCD_voidSendCommand(lcd_home);
	    _delay_us(50);

	    /* Set 4-bit mode */
	    LCD_voidSendCommand(FOUR_BITS);
	    _delay_us(50);

		LCD_voidSendCommand(lcd_DisplayOn_CursorOn);
		_delay_us(50);

		LCD_voidSendCommand(lcd_Clear);
		_delay_ms(2);

	    /* 4-bit mode, 2 lines, 5x8 font */
	    LCD_voidSendCommand(lcd_EntryMode);
	    _delay_us(50);
	#endif
}

void LCD_voidSendCommand(u8 Copy_u8Instruction)
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

void LCD_voidSendData(u8 Copy_u8DATA)
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

void LCD_voidSendString(u8* pvu8String)
{
    u8 i = 0;
    if( pvu8String != NULL ){
        while (pvu8String[i] != '\0')
        {
            LCD_voidSendData(pvu8String[i]);
            i++;
        }
    }
}

void LCD_voidSendNumber(u64 Copy_u64Number)
{
	u8 buffer[20]; // Buffer to hold the string representation of the number
	u8 i = 0;

	if (Copy_u64Number == 0) {
		LCD_voidSendData('0');
		return;
	}

	// Convert number to string in reverse order
	while (Copy_u64Number > 0) {
		buffer[i++] = (Copy_u64Number % 10) + '0'; // Get last digit and convert to char
		Copy_u64Number /= 10; // Remove last digit
	}

	// Send the digits in correct order
	for (u8 j = i; j > 0; j--) {
		LCD_voidSendData(buffer[j - 1]);
	}
}

void LCD_voidSetPosition(u8 Copy_u8ROW, u8 Copy_u8COL)
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
	    LCD_voidSendCommand(0x80 | XY_u8DDRAM_ADDRESS);
}

void LCD_voidClearScreen()
{
	LCD_voidSendCommand(0x01);
    _delay_ms(2);
}

void LCD_voidCreateCustomChar(u8* Copy_u8Pattern, u8 Copy_u8Location)
{
    Copy_u8Location &= 0x07;
	LCD_voidSendCommand( 0x40 | (Copy_u8Location << 3));
	for(u8 i=0 ; i<8 ; i++)
	{
		LCD_voidSendData(Copy_u8Pattern[i]);
	}
	LCD_voidSendCommand(0x80);
}

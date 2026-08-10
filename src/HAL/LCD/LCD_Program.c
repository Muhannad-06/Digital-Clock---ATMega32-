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

#include <util/delay.h>

#if LCD_MODE == LCD_4BITS
/* Puts the low nibble of Copy_u8Nibble (bits 0..3) onto D4..D7
 * (DIO_PIN4..DIO_PIN7) of LCD_DATA_PORT and pulses EN. Only ever touches
 * pins 4..7 of the port, so PA0..PA3 (ADC0..ADC3 / POT) are never
 * disturbed. */
static void LCD_voidWriteNibble(u8 Copy_u8Nibble)
{
    digitalWrite(LCD_DATA_PORT, DIO_PIN4, READ_BIT(Copy_u8Nibble, 0));
    digitalWrite(LCD_DATA_PORT, DIO_PIN5, READ_BIT(Copy_u8Nibble, 1));
    digitalWrite(LCD_DATA_PORT, DIO_PIN6, READ_BIT(Copy_u8Nibble, 2));
    digitalWrite(LCD_DATA_PORT, DIO_PIN7, READ_BIT(Copy_u8Nibble, 3));

    digitalWrite(LCD_CTRL_PORT, LCD_PIN_EN, DIO_PIN_HIGH);
    _delay_us(50);
    digitalWrite(LCD_CTRL_PORT, LCD_PIN_EN, DIO_PIN_LOW);
    _delay_us(50);
}
#endif

void LCD_voidInit()
{
	/* LCD_CTRL_PORT (RS/RW/EN, and BUZZ on the same physical port) is
	 * fully dedicated in this design, safe to configure as a whole port. */
	portMode(LCD_CTRL_PORT, DIO_PORT_OUTPUT);

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

		LCD_voidSendCommand(0x38);
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

	/* Only D4..D7 are physically wired (to PA4..PA7) - PA0..PA3 stay
	 * whatever the ADC driver has configured them as (POT is on PA0). */
	pinMode  ( LCD_DATA_PORT, DIO_PIN4,DIO_PIN_OUTPUT);
	pinMode  ( LCD_DATA_PORT, DIO_PIN5,DIO_PIN_OUTPUT);
	pinMode  ( LCD_DATA_PORT, DIO_PIN6,DIO_PIN_OUTPUT);
	pinMode  ( LCD_DATA_PORT, DIO_PIN7,DIO_PIN_OUTPUT);
	pinMode  ( LCD_CTRL_PORT , LCD_PIN_RS , DIO_PIN_OUTPUT  );
	pinMode  ( LCD_CTRL_PORT , LCD_PIN_RW , DIO_PIN_OUTPUT  );
	pinMode  ( LCD_CTRL_PORT , LCD_PIN_EN , DIO_PIN_OUTPUT  );

	digitalWrite(LCD_CTRL_PORT, LCD_PIN_RS, DIO_PIN_LOW);
	digitalWrite(LCD_CTRL_PORT, LCD_PIN_RW, DIO_PIN_LOW);

	/* HD44780 power-on reset sequence: the controller always starts up
	 * expecting 8-bit instructions, so the first nibbles we push out
	 * must be sent RAW (single nibble = high nibble of an 8-bit
	 * "Function Set" command), NOT through LCD_voidSendCommand() (which
	 * would already assume 4-bit two-nibble framing). This is the
	 * standard datasheet-mandated way to force the LCD into 4-bit mode
	 * when only D4..D7 are wired up. */
	LCD_voidWriteNibble(0x03);
	_delay_ms(5);
	LCD_voidWriteNibble(0x03);
	_delay_us(150);
	LCD_voidWriteNibble(0x03);
	_delay_us(150);
	LCD_voidWriteNibble(0x02); /* switch to 4-bit interface */
	_delay_us(150);

	/* From here on the LCD is in true 4-bit mode: every command/data
	 * byte is sent as high-nibble-then-low-nibble by LCD_voidSendCommand()
	 * / LCD_voidSendData(). */
	LCD_voidSendCommand(FOUR_BITS); /* 0x28: 4-bit, 2-line, 5x8 font */
	_delay_us(50);

	LCD_voidSendCommand(lcd_DisplayOff_CursorOff);
	_delay_us(50);

	LCD_voidSendCommand(lcd_Clear);
	_delay_ms(2);

	LCD_voidSendCommand(lcd_EntryMode);
	_delay_us(50);

	LCD_voidSendCommand(lcd_DisplayOn_CursorOff);
	_delay_us(50);
	#endif
}

void LCD_voidSendCommand(u8 Copy_u8Instruction)
{
    /*RS = 0 RW = 0*/
    digitalWrite(LCD_CTRL_PORT, LCD_PIN_RS, DIO_PIN_LOW);
    digitalWrite(LCD_CTRL_PORT, LCD_PIN_RW, DIO_PIN_LOW);

    #if LCD_MODE == LCD_4BITS
        LCD_voidWriteNibble((Copy_u8Instruction >> 4) & 0x0F); /* high nibble first */
        LCD_voidWriteNibble(Copy_u8Instruction & 0x0F);        /* then low nibble   */
    #elif LCD_MODE == LCD_8BITS
        digitalWrite(LCD_CTRL_PORT, LCD_PIN_EN, DIO_PIN_HIGH);
        digitalWritePORT(LCD_DATA_PORT, Copy_u8Instruction);
        _delay_us(50);
        digitalWrite(LCD_CTRL_PORT, LCD_PIN_EN, DIO_PIN_LOW);
    #endif

    _delay_ms(2);
}

void LCD_voidSendData(u8 Copy_u8DATA)
{
    /* RS = 1  RW = 0 */
    digitalWrite(LCD_CTRL_PORT, LCD_PIN_RS, DIO_PIN_HIGH);
    digitalWrite(LCD_CTRL_PORT, LCD_PIN_RW, DIO_PIN_LOW);

    #if LCD_MODE == LCD_4BITS
        LCD_voidWriteNibble((Copy_u8DATA >> 4) & 0x0F);
        LCD_voidWriteNibble(Copy_u8DATA & 0x0F);
    #elif LCD_MODE == LCD_8BITS
        digitalWrite(LCD_CTRL_PORT, LCD_PIN_EN, DIO_PIN_HIGH);
        digitalWritePORT(LCD_DATA_PORT, Copy_u8DATA);
        _delay_us(50);
        digitalWrite(LCD_CTRL_PORT, LCD_PIN_EN, DIO_PIN_LOW);
    #endif

    _delay_us(100);
}

void LCD_voidSendString(const u8* pvu8String)
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

	/* LCD_ROW_1/LCD_ROW_2 and LCD_COL_1..16 (see LCD_Interface.h) are
	 * 1-based, but DDRAM addressing is 0-based, so convert here. */
	u8 Local_u8Row = Copy_u8ROW - 1;
	u8 Local_u8Col = Copy_u8COL - 1;

	    if(Local_u8Row == 0)
	    {
	        XY_u8DDRAM_ADDRESS = Local_u8Col;
	    }
	    else if(Local_u8Row == 1)
	    {
	        XY_u8DDRAM_ADDRESS = 0x40 + Local_u8Col;
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

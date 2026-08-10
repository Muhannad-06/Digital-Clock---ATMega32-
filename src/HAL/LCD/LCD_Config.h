/*
 *<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<   LCD_Config.h   >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
 *
 *  Author : Muhannad Mahfouz El-Shahiedy
 *  Layer  : HAL
 *  HW    : LCD
 *
 */ 

#ifndef LCD_CONFIG_H_
#define LCD_CONFIG_H_


#define LCD_4BITS 4
#define LCD_8BITS 8

/* LCD Mode */

	/* Choose 8 for 8_bit connection, 4 for 4_bit connection */

/* $ Options :-
*
*        LCD_8BITS
*        LCD_4BITS                 
*/
/* Schematic only wires the LCD's D4..D7 pins (to PA4..PA7); D0..D3 are
 * left unconnected. LCD_voidSendCommand()/LCD_voidSendData() split every
 * byte into a high-nibble write followed by a low-nibble write (two EN
 * pulses), each nibble placed only on D4..D7, so PA0..PA3 (shared with
 * ADC0..ADC3, i.e. POT) are never touched. Keep this set to LCD_4BITS. */
#define LCD_MODE            LCD_4BITS

/***************************************************************************************/



/* D0 ------> D7 */

/*

  Options:-
 	 1-DIO_PORTA
 	 2-DIO_PORTB
 	 3-DIO_PORTC
 	 4-DIO_PORTD

 */
/***************************************************************************************/


#define LCD_DATA_PORT       DIO_PORTA
/* RS,RW,EN */
#define LCD_CTRL_PORT       DIO_PORTC

/***************************************************************************************/


/*

 Options:-
 	 1-DIO_PIN0
 	 2-DIO_PIN1
 	 3-DIO_PIN2
 	 4-DIO_PIN3
 	 5-DIO_PIN4
 	 6-DIO_PIN5
 	 7-DIO_PIN6
 	 8-DIO_PIN7

 */
/* Per schematic: U1 PC0/SCL -> RS, PC1/SDA -> RW, PC2/TCK -> EN */
#define LCD_PIN_RS             DIO_PIN0
#define LCD_PIN_RW             DIO_PIN1
#define LCD_PIN_EN             DIO_PIN2

/***************************************************************************************/


#endif /* LCD_CONFIG_H */

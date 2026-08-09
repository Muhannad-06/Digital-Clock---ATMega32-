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
#define LCD_CTRL_PORT       DIO_PORTB

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
#define LCD_PIN_RS             DIO_PIN1
#define LCD_PIN_RW             DIO_PIN2
#define LCD_PIN_EN             DIO_PIN3

/***************************************************************************************/


#endif /* LCD_CONFIG_H */
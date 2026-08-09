#ifndef DIO_INTERFACE_H_
#define DIO_INTERFACE_H_

/*Port ID*/
#define DIO_PORTA         0
#define DIO_PORTB         1
#define DIO_PORTC         2
#define DIO_PORTD         3

/*Pin ID*/
#define DIO_PIN0          0
#define DIO_PIN1          1
#define DIO_PIN2          2
#define DIO_PIN3          3
#define DIO_PIN4          4
#define DIO_PIN5          5
#define DIO_PIN6          6
#define DIO_PIN7          7

/*Pin Mode*/
#define DIO_PIN_INPUT        0
#define DIO_PIN_OUTPUT       1
#define DIO_PIN_INPUT_PULLUP 2

/*Pin Value*/
#define DIO_PIN_LOW       0
#define DIO_PIN_HIGH      1

/*Port Mode*/
#define DIO_PORT_INPUT    0x00 /*0b00000000*/
#define DIO_PORT_OUTPUT   0xFF /*0b11111111*/

/*Port Value*/
#define DIO_PORT_LOW    0x00 /*0b00000000*/
#define DIO_PORT_HIGH   0xFF /*0b11111111*/

ErrorCode digitalWrite(u8 Copy_u8PortID, u8 Copy_u8PinID, u8 Copy_u8Value);
ErrorCode pinMode(u8 Copy_u8PortID, u8 Copy_u8PinID, u8 Copy_u8Mode);
ErrorCode digitalToggle(u8 Copy_u8PortID, u8 Copy_u8PinID);
ErrorCode digitalRead(u8 Copy_u8PortID, u8 Copy_u8PinID, u8* Ptr_u8InputData);

ErrorCode digitalWritePORT(u8 Copy_u8PortID, u8 Copy_u8Value);
ErrorCode portMode(u8 Copy_u8PortID, u8 Copy_u8Mode);
ErrorCode digitalTogglePORT(u8 Copy_u8PortID);
ErrorCode digitalReadPORT(u8 Copy_u8PortID, u8* Ptr_u8InputData);



#endif /*DIO_INTERFACE_H_*/
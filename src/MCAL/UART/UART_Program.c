#include "../../LIBs/STD_TYPES.h"
#include "../../LIBs/BIT_MATH.h"

#include "UART_Interface.h"
#include "UART_Private.h"
#include "UART_Config.h"

#include "../DIO/DIO_Interface.h"

void UART_voidInit(void)
{
    u8 Local_u8UCSRC = 0;
    u16 Local_u16UBRR = 0;

    /* 1. Ensure UCSRA is cleared (Normal Speed Mode, U2X = 0) */
    UCSRA = 0;

    /* 2. Calculate UBRR Value for Normal Asynchronous Mode */
    Local_u16UBRR = (F_CPU / (16UL * UART_BAUD_RATE)) - 1;

    /* 3. Set Baud Rate */
    /* Datasheet: UBRRL must be written first, then UBRRH */
    UBRRL = (u8)Local_u16UBRR;
    /* Force bit 7 (URSEL) to 0 to ensure we write to UBRRH, not UCSRC */
    UBRRH = (u8)(Local_u16UBRR >> 8) & 0x7F;

    /* 4. Enable Receiver and Transmitter */
    SET_BIT(UCSRB, RXEN);
    SET_BIT(UCSRB, TXEN);

    /* 5. Configure Frame Format: 8 data bits, No parity, 1 stop bit */
    SET_BIT(Local_u8UCSRC, URSEL); /* URSEL must be 1 to write to UCSRC */

    CLEAR_BIT(Local_u8UCSRC, UMSEL); /* Asynchronous */
    CLEAR_BIT(Local_u8UCSRC, UPM0);  /* No Parity */
    CLEAR_BIT(Local_u8UCSRC, UPM1);
    CLEAR_BIT(Local_u8UCSRC, USBS);  /* 1 Stop Bit */

    /* 8-bit Data Size */
    SET_BIT(Local_u8UCSRC, UCSZ0);
    SET_BIT(Local_u8UCSRC, UCSZ1);
    CLEAR_BIT(UCSRB, UCSZ2);

    /* Write the configuration to UCSRC in one single instruction */
    UCSRC = Local_u8UCSRC;

    pinMode(UART_PORT, RX, DIO_PIN_INPUT);
    pinMode(UART_PORT, TX, DIO_PIN_OUTPUT);
}

void UART_voidTransmitData(u8 Copy_u8Data)
{
    while (READ_BIT(UCSRA, UDRE) == 0);

    /* Put data into buffer, sends the data */
    UDR = Copy_u8Data;
}

u8 UART_u8ReceiveData(void)
{
    /* Wait for data to be received (RXC flag) */
    while (READ_BIT(UCSRA, RXC) == 0);

    /* READ and return received data from buffer */
    return UDR;
}

void UART_voidSendString(const u8 *Copy_pu8String)
{
    u8 Local_u8Counter = 0;

    /* Loop through the string until the null terminator */
    while (Copy_pu8String[Local_u8Counter] != '\0')
    {
        UART_voidTransmitData(Copy_pu8String[Local_u8Counter]);
        Local_u8Counter++;
    }
}

#ifndef UART_INTERFACE_H
#define UART_INTERFACE_H

#include "../../LIBs/STD_TYPES.h"

void UART_voidInit(void);

void UART_voidTransmitData(u8 Copy_u8Data);

u8 UART_u8ReceiveData(void);

void UART_voidSendString(const u8 *Copy_pu8String);

#endif /* UART_INTERFACE_H */

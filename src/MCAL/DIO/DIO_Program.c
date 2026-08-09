#include "../../LIBs/STD_TYPES.h"
#include "../../LIBs/BIT_MATH.h"

#include "DIO_Interface.h"
#include "DIO_Private.h"
#include "DIO_Config.h"

ErrorCode pinMode(u8 Copy_u8PortID, u8 Copy_u8PinID, u8 Copy_u8Mode)
{
    ErrorCode Local_enumValidationStatus = VALID;
    if(Copy_u8PortID <= DIO_PORTD && Copy_u8PinID <= DIO_PIN7)
    {
        if(Copy_u8Mode == DIO_PIN_OUTPUT)
        {
            switch(Copy_u8PortID)
            {
                case DIO_PORTA: SET_BIT(DDRA, Copy_u8PinID); break;
                case DIO_PORTB: SET_BIT(DDRB, Copy_u8PinID); break;
                case DIO_PORTC: SET_BIT(DDRC, Copy_u8PinID); break;
                case DIO_PORTD: SET_BIT(DDRD, Copy_u8PinID); break;
                default: Local_enumValidationStatus = INVALID; break;
            }
        }
        else if(Copy_u8Mode == DIO_PIN_INPUT)
        {
            switch(Copy_u8PortID)
            {
                case DIO_PORTA: CLEAR_BIT(DDRA, Copy_u8PinID); CLEAR_BIT(PORTA, Copy_u8PinID); break;
                case DIO_PORTB: CLEAR_BIT(DDRB, Copy_u8PinID); CLEAR_BIT(PORTB, Copy_u8PinID); break;
                case DIO_PORTC: CLEAR_BIT(DDRC, Copy_u8PinID); CLEAR_BIT(PORTC, Copy_u8PinID); break;
                case DIO_PORTD: CLEAR_BIT(DDRD, Copy_u8PinID); CLEAR_BIT(PORTD, Copy_u8PinID); break;
                default: Local_enumValidationStatus = INVALID; break;
            }
        }
        else if(Copy_u8Mode == DIO_PIN_INPUT_PULLUP)
        {
            switch(Copy_u8PortID)
            {
                case DIO_PORTA: CLEAR_BIT(SFIOR, PUD); CLEAR_BIT(DDRA, Copy_u8PinID); SET_BIT(PORTA, Copy_u8PinID); break;
                case DIO_PORTB: CLEAR_BIT(SFIOR, PUD); CLEAR_BIT(DDRB, Copy_u8PinID); SET_BIT(PORTB, Copy_u8PinID); break;
                case DIO_PORTC: CLEAR_BIT(SFIOR, PUD); CLEAR_BIT(DDRC, Copy_u8PinID); SET_BIT(PORTC, Copy_u8PinID); break;
                case DIO_PORTD: CLEAR_BIT(SFIOR, PUD); CLEAR_BIT(DDRD, Copy_u8PinID); SET_BIT(PORTD, Copy_u8PinID); break;
                default: Local_enumValidationStatus = INVALID; break;
            }
        }
        else
        {
            Local_enumValidationStatus = INVALID;
        }
    }
    else
    {
        Local_enumValidationStatus = INVALID;
    }
    return Local_enumValidationStatus;
}

ErrorCode digitalWrite(u8 Copy_u8PortID, u8 Copy_u8PinID, u8 Copy_u8Value)
{
    ErrorCode Local_enumValidationStatus = VALID;
    if((Copy_u8PortID <= DIO_PORTD) && (Copy_u8PinID <= DIO_PIN7) && (Copy_u8Value == DIO_PIN_LOW || Copy_u8Value == DIO_PIN_HIGH))
    {
        if(Copy_u8Value == DIO_PIN_HIGH)
        {
            switch(Copy_u8PortID)
            {
                case DIO_PORTA: SET_BIT(PORTA, Copy_u8PinID); break;
                case DIO_PORTB: SET_BIT(PORTB, Copy_u8PinID); break;
                case DIO_PORTC: SET_BIT(PORTC, Copy_u8PinID); break;
                case DIO_PORTD: SET_BIT(PORTD, Copy_u8PinID); break;
                default: Local_enumValidationStatus = INVALID; break;
            }
        }
        else if(Copy_u8Value == DIO_PIN_LOW)
        {
            switch(Copy_u8PortID)
            {
                case DIO_PORTA: CLEAR_BIT(PORTA, Copy_u8PinID); break;
                case DIO_PORTB: CLEAR_BIT(PORTB, Copy_u8PinID); break;
                case DIO_PORTC: CLEAR_BIT(PORTC, Copy_u8PinID); break;
                case DIO_PORTD: CLEAR_BIT(PORTD, Copy_u8PinID); break;
                default: Local_enumValidationStatus = INVALID; break;
            }
        }
    }
    else
    {
        Local_enumValidationStatus = INVALID;
    }
    return Local_enumValidationStatus;
}

ErrorCode digitalToggle(u8 Copy_u8PortID, u8 Copy_u8PinID)
{
    ErrorCode Local_enumValidationStatus = VALID;
    if((Copy_u8PortID <= DIO_PORTD) && (Copy_u8PinID <= DIO_PIN7))
    {
        switch(Copy_u8PortID)
        {
            case DIO_PORTA: TOGGLE_BIT(PORTA, Copy_u8PinID); break;
            case DIO_PORTB: TOGGLE_BIT(PORTB, Copy_u8PinID); break;
            case DIO_PORTC: TOGGLE_BIT(PORTC, Copy_u8PinID); break;
            case DIO_PORTD: TOGGLE_BIT(PORTD, Copy_u8PinID); break;
            default: Local_enumValidationStatus = INVALID; break;
        }
    }
    else
    {
        Local_enumValidationStatus = INVALID;
    }
    return Local_enumValidationStatus;
}

ErrorCode digitalRead(u8 Copy_u8PortID, u8 Copy_u8PinID, u8* Ptr_u8InputData)
{
    ErrorCode Local_enumValidationStatus = VALID;
    if((Copy_u8PortID <= DIO_PORTD) && (Copy_u8PinID <= DIO_PIN7) && (Ptr_u8InputData != NULL))
    {
        switch(Copy_u8PortID)
        {
            case DIO_PORTA: *Ptr_u8InputData = READ_BIT(PINA, Copy_u8PinID); break;
            case DIO_PORTB: *Ptr_u8InputData = READ_BIT(PINB, Copy_u8PinID); break;
            case DIO_PORTC: *Ptr_u8InputData = READ_BIT(PINC, Copy_u8PinID); break;
            case DIO_PORTD: *Ptr_u8InputData = READ_BIT(PIND, Copy_u8PinID); break;
            default: Local_enumValidationStatus = INVALID; break;
        }
    }
    else
    {
        Local_enumValidationStatus = INVALID;
    }
    return Local_enumValidationStatus;
}

ErrorCode digitalWritePORT(u8 Copy_u8PortID, u8 Copy_u8Value)
{
    ErrorCode Local_enumValidationStatus = VALID;
    if(Copy_u8PortID <= DIO_PORTD)
    {
        switch(Copy_u8PortID)
        {
            case DIO_PORTA: PORTA = Copy_u8Value;           break;
            case DIO_PORTB: PORTB = Copy_u8Value;           break;
            case DIO_PORTC: PORTC = Copy_u8Value;           break;
            case DIO_PORTD: PORTD = Copy_u8Value;           break;
            default: Local_enumValidationStatus = INVALID;  break;
        }
    }
    else
    {
        Local_enumValidationStatus = INVALID;
    }
    return Local_enumValidationStatus;
}

ErrorCode portMode(u8 Copy_u8PortID, u8 Copy_u8Mode)
{
    ErrorCode Local_enumValidationStatus = VALID;
    if(Copy_u8PortID <= DIO_PORTD)
    {
        switch(Copy_u8PortID)
        {
            case DIO_PORTA: DDRA = Copy_u8Mode;            break;
            case DIO_PORTB: DDRB = Copy_u8Mode;            break;
            case DIO_PORTC: DDRC = Copy_u8Mode;            break;
            case DIO_PORTD: DDRD = Copy_u8Mode;            break;
            default: Local_enumValidationStatus = INVALID; break;
        }
    }
    else
    {
        Local_enumValidationStatus = INVALID;
    }
    return Local_enumValidationStatus;
}

ErrorCode digitalTogglePORT(u8 Copy_u8PortID)
{
    ErrorCode Local_enumValidationStatus = VALID;
    if(Copy_u8PortID <= DIO_PORTD)
    {
        switch(Copy_u8PortID)
        {
            case DIO_PORTA: PORTA = ~PORTA;                break;
            case DIO_PORTB: PORTB = ~PORTB;                break;
            case DIO_PORTC: PORTC = ~PORTC;                break;
            case DIO_PORTD: PORTD = ~PORTD;                break;
            default: Local_enumValidationStatus = INVALID; break;
        }
    }
    else
    {
        Local_enumValidationStatus = INVALID;
    }
    return Local_enumValidationStatus;
}

ErrorCode digitalReadPORT(u8 Copy_u8PortID, u8* Ptr_u8InputData)
{
    ErrorCode Local_enumValidationStatus = VALID;
    if(Copy_u8PortID <= DIO_PORTD && (Ptr_u8InputData != NULL))
    {
        switch(Copy_u8PortID)
        {
            case DIO_PORTA: *Ptr_u8InputData = PINA;            break;
            case DIO_PORTB: *Ptr_u8InputData = PINB;            break;
            case DIO_PORTC: *Ptr_u8InputData = PINC;            break;
            case DIO_PORTD: *Ptr_u8InputData = PIND;            break;
            default: Local_enumValidationStatus = INVALID;      break;
        }
    }
    else
    {
        Local_enumValidationStatus = INVALID;
    }
    return Local_enumValidationStatus;
}
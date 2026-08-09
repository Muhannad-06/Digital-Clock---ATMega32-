#include "../../LIBs/STD_TYPES.h"
#include "../../LIBs/BIT_MATH.h"
#include "../../MCAL/DIO/DIO_Interface.h"

#include "DC_Interface.h"
#include "DC_Config.h"
#include "DC_Private.h"

ErrorCode DC_Init(void)
{
    pinMode(DC_MOTOR_PORT, DC_MOTOR_DIRECTION_Clockwise, DIO_PIN_OUTPUT);
    pinMode(DC_MOTOR_PORT, DC_MOTOR_DIRECTION_CounterClockwise, DIO_PIN_OUTPUT);
    

    return VALID;
}

ErrorCode DC_SetDirection(u8 Copy_u8Direction)
{
    if(Copy_u8Direction == DC_MOTOR_DIRECTION_Clockwise || Copy_u8Direction == DC_MOTOR_DIRECTION_CounterClockwise)
    {
        switch (Copy_u8Direction)
        {
            case DC_MOTOR_DIRECTION_Clockwise:
            DC_MotorOFF();
            digitalWrite(DC_MOTOR_PORT, DC_MOTOR_DIRECTION_Clockwise, DIO_PIN_HIGH);

            break;

            case DC_MOTOR_DIRECTION_CounterClockwise:
            DC_MotorOFF();
            digitalWrite(DC_MOTOR_PORT, DC_MOTOR_DIRECTION_CounterClockwise, DIO_PIN_HIGH);

            break;

            default:
            DC_MotorOFF();
        }
    }
    else
    {
        return INVALID;
    }
        return VALID;
}

ErrorCode DC_MotorOFF()
{
    digitalWrite(DC_MOTOR_PORT, DC_MOTOR_DIRECTION_CounterClockwise, DIO_PIN_LOW);
    digitalWrite(DC_MOTOR_PORT, DC_MOTOR_DIRECTION_Clockwise, DIO_PIN_LOW);
    
    return VALID;
}

#include "../../LIBs/STD_TYPES.h"
#include "../../LIBs/BIT_MATH.h"
#include "../../MCAL/DIO/DIO_Interface.h"

#include "Seg_Interface.h"
#include "Seg_Private.h"
#include "Seg_Config.h"

static const u8 Seg_u8Numbers[10] = {
    0b00111111, /* 0 */
    0b00000110, /* 1 */
    0b01011011, /* 2 */
    0b01001111, /* 3 */
    0b01100110, /* 4 */
    0b01101101, /* 5 */
    0b01111101, /* 6 */
    0b00000111, /* 7 */
    0b01111111, /* 8 */
    0b01101111  /* 9 */
};

ErrorCode Seg_enumInit(u8 Copy_u8PortID)
{
    if (Copy_u8PortID > DIO_PORTD)
    {
        return INVALID;
    }

    portMode(Copy_u8PortID, DIO_PORT_OUTPUT);
    digitalWritePORT(Copy_u8PortID, DIO_PORT_LOW);

    return VALID;
}

ErrorCode Seg_enumDisplay(u8 Copy_u8PortID, u8 Copy_u8SegValue, u8 Copy_u8SegType)
{
    if (Copy_u8PortID > DIO_PORTD || Copy_u8SegValue > 9)
    {
        return INVALID;
    }

    switch (Copy_u8SegType)
    {
        case COMMON_CATHODE:
            digitalWritePORT(Copy_u8PortID, Seg_u8Numbers[Copy_u8SegValue]);
            break;

        case COMMON_ANODE:
            digitalWritePORT(Copy_u8PortID, ~Seg_u8Numbers[Copy_u8SegValue]);
            break;

        default:
            return INVALID;
    }

    return VALID;
}

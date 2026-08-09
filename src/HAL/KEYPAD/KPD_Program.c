#include "../../LIBs/STD_TYPES.h"
#include "../../LIBs/BIT_MATH.h"
#include "../../MCAL/DIO/DIO_Interface.h"
#include "KPD_Interface.h"
#include "KPD_Config.h"
#include "KPD_Private.h"

//#define F_CPU 8000000UL
#include <util/delay.h>

static const u8 KEYPAD_Au8KEYs[NumROWs][NumCOLs] = KEYPAD_KEYS;
static const u8 KEYPAD_Au8ROWsPINs[NumROWs] = {KEYPAD_ROW1, KEYPAD_ROW2, KEYPAD_ROW3};
static const u8 KEYPAD_Au8COLsPINs[NumCOLs] = {KEYPAD_COL1, KEYPAD_COL2, KEYPAD_COL3};

void KEYPAD_voidInit(void)
{
    _delay_ms(5);

    // 1. Initialize ROWs as OUTPUT and set them HIGH
    for(u8 i = 0 ; i < NumROWs ; i++)
    {
        pinMode(KEYPAD_PORT, KEYPAD_Au8ROWsPINs[i], DIO_PIN_OUTPUT);
        digitalWrite(KEYPAD_PORT, KEYPAD_Au8ROWsPINs[i], DIO_PIN_HIGH);
    }

    // 2. Initialize COLs as INPUT_PULLUP
    for(u8 i = 0 ; i < NumCOLs ; i++)
    {
        pinMode(KEYPAD_PORT, KEYPAD_Au8COLsPINs[i], DIO_PIN_INPUT_PULLUP);
        digitalWrite(KEYPAD_PORT, KEYPAD_Au8COLsPINs[i], DIO_PIN_HIGH);
    }

    _delay_ms(5);
}

ErrorCode KEYPAD_enumGetKeyState(u8* Copy_puReturnKEY)
{
    ErrorCode KEYPAD_enumGetKeyValidation = VALID;
    u8 Local_u8ROWsCounter, Local_u8COLsCounter, Local_u8PinVal, Local_u8Flag = 0;
    
    if(Copy_puReturnKEY != NULL)
    {
        *Copy_puReturnKEY = KEYPAD_KEYs_notPressed;

        for(Local_u8ROWsCounter = 0 ; Local_u8ROWsCounter < NumROWs ; Local_u8ROWsCounter++)
        {
            // Activate the current row by pulling it LOW
            digitalWrite(KEYPAD_PORT, KEYPAD_Au8ROWsPINs[Local_u8ROWsCounter], DIO_PIN_LOW);
            
            for(Local_u8COLsCounter = 0 ; Local_u8COLsCounter < NumCOLs ; Local_u8COLsCounter++)
            {
                // Read the current column
                digitalRead(KEYPAD_PORT, KEYPAD_Au8COLsPINs[Local_u8COLsCounter], &Local_u8PinVal);

                if(Local_u8PinVal == DIO_PIN_LOW)
                {
                    _delay_ms(20); // Debounce delay
                    
                    // Read the COLUMN pin again to verify (NOT the ROW pin)
                    digitalRead(KEYPAD_PORT, KEYPAD_Au8COLsPINs[Local_u8COLsCounter], &Local_u8PinVal);
                    if(Local_u8PinVal == DIO_PIN_LOW)
                    {
                        // Register the valid key press
                        *Copy_puReturnKEY = KEYPAD_Au8KEYs[Local_u8ROWsCounter][Local_u8COLsCounter];
                        
                        // Wait for the user to release the key by polling the COLUMN pin
                        while(Local_u8PinVal == DIO_PIN_LOW)
                        {
                            digitalRead(KEYPAD_PORT, KEYPAD_Au8COLsPINs[Local_u8COLsCounter], &Local_u8PinVal);
                        }

                        Local_u8Flag = 1;
                        break; // Break column loop
                    }
                }   
            }

            // Deactivate the current row by pulling it HIGH again
            digitalWrite(KEYPAD_PORT, KEYPAD_Au8ROWsPINs[Local_u8ROWsCounter], DIO_PIN_HIGH);

            if(Local_u8Flag == 1)
            {
                break; // Break row loop if a key was found
            }
        }
    }
    else
    {
        KEYPAD_enumGetKeyValidation = INVALID;
    }

    return KEYPAD_enumGetKeyValidation;
}

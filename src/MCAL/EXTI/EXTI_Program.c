#include "../../LIBs/STD_TYPES.h"
#include "../../LIBs/BIT_MATH.h"
#include "../DIO/DIO_Interface.h"

#include "EXTI_Interface.h"
#include "EXTI_Config.h"
#include "EXTI_Private.h"

static void (*pointerToFunction[3]) (void) = {NULL, NULL, NULL};

ErrorCode EXTI_enumEnable(u8 Copy_u8EXTINo, u8 Copy_u8EXTISense)
{
    ErrorCode EXTI_enumvoidEnableValidationCheck = VALID;

    if( (Copy_u8EXTINo > EXTI_u8EXTI2) || (Copy_u8EXTISense > EXTI_RISING_EDGE) )
    {
        EXTI_enumvoidEnableValidationCheck = INVALID;
    }
    else if((Copy_u8EXTINo == EXTI_u8EXTI2) && (Copy_u8EXTISense < EXTI_u8EXTI2 ))
    {
        EXTI_enumvoidEnableValidationCheck = INVALID;
    }
    else
    {
        /* Open General Interrupt Register */
        SET_BIT(INTERRUPT_SREG_REG, INTERRUPT_GIE_BIT);

         switch (Copy_u8EXTINo)
        {
            case EXTI_u8EXTI0: SET_BIT(EXTI_GICR_REG, EXTI_GICR_INT0);
                switch (Copy_u8EXTISense)
                    {
                        case EXTI_LOW_LEVEL: 
                                CLEAR_BIT(EXTI_MCUCR_REG, EXTI_MCUCR_ISC00);
                                CLEAR_BIT(EXTI_MCUCR_REG, EXTI_MCUCR_ISC01);
                                break;
                        case EXTI_ANY_LOGICAL_CHANGE:
                                SET_BIT(EXTI_MCUCR_REG, EXTI_MCUCR_ISC00);
                                CLEAR_BIT(EXTI_MCUCR_REG, EXTI_MCUCR_ISC01);
                                break;
                        case EXTI_FALLING_EDGE:
                                CLEAR_BIT(EXTI_MCUCR_REG, EXTI_MCUCR_ISC00);
                                SET_BIT(EXTI_MCUCR_REG, EXTI_MCUCR_ISC01);
                                break;
                        case EXTI_RISING_EDGE:
                                SET_BIT(EXTI_MCUCR_REG, EXTI_MCUCR_ISC00);
                                SET_BIT(EXTI_MCUCR_REG, EXTI_MCUCR_ISC01);
                                break;
                        default:
                        break;
                    }
                break;

            case EXTI_u8EXTI1: SET_BIT(EXTI_GICR_REG, EXTI_GICR_INT1);
                switch (Copy_u8EXTISense)
                {
                        case EXTI_LOW_LEVEL: 
                                CLEAR_BIT(EXTI_MCUCR_REG, EXTI_MCUCR_ISC10);
                                CLEAR_BIT(EXTI_MCUCR_REG, EXTI_MCUCR_ISC11);
                                break;
                        case EXTI_ANY_LOGICAL_CHANGE:
                                SET_BIT(EXTI_MCUCR_REG, EXTI_MCUCR_ISC10);
                                CLEAR_BIT(EXTI_MCUCR_REG, EXTI_MCUCR_ISC11);
                                break;
                        case EXTI_FALLING_EDGE:
                                CLEAR_BIT(EXTI_MCUCR_REG, EXTI_MCUCR_ISC10);
                                SET_BIT(EXTI_MCUCR_REG, EXTI_MCUCR_ISC11);
                                break;
                        case EXTI_RISING_EDGE:
                                SET_BIT(EXTI_MCUCR_REG, EXTI_MCUCR_ISC10);
                                SET_BIT(EXTI_MCUCR_REG, EXTI_MCUCR_ISC11);
                                break;
                        default:
                        break;
                }
                break;
            
            case EXTI_u8EXTI2: SET_BIT(EXTI_GICR_REG, EXTI_GICR_INT2);
                switch (Copy_u8EXTISense)
                {
                        case EXTI_FALLING_EDGE:
                                CLEAR_BIT(EXTI_MCUCSR_REG, EXTI_MCUCSR_ISC2);
                                break;
                        case EXTI_RISING_EDGE:
                                SET_BIT(EXTI_MCUCSR_REG, EXTI_MCUCSR_ISC2);                            
                                break;
                        default:
                        break;
                }
                break;
        }
    }
        return EXTI_enumvoidEnableValidationCheck;
}

ErrorCode EXTI_enumDisable(u8 Copy_u8EXTINo)
{
        ErrorCode EXTI_enumvoidDisableValidationCheck = VALID;

    if (Copy_u8EXTINo > EXTI_u8EXTI2 )
    {
        EXTI_enumvoidDisableValidationCheck = INVALID;
    }
    else
    {
        switch (Copy_u8EXTINo)
        {
        case EXTI_u8EXTI0: CLEAR_BIT(EXTI_GICR_REG, EXTI_GICR_INT0);     break;
        case EXTI_u8EXTI1: CLEAR_BIT(EXTI_GICR_REG, EXTI_GICR_INT1);    break;
        case EXTI_u8EXTI2: CLEAR_BIT(EXTI_GICR_REG, EXTI_GICR_INT2);    break;
        default:
            EXTI_enumvoidDisableValidationCheck = INVALID;
            break;
        }
    }
    
    return EXTI_enumvoidDisableValidationCheck;
}

ErrorCode EXTI_enumSetSignalLatch(u8 Copy_u8EXTINo, u8 Copy_u8EXTISense)
{
    ErrorCode EXTI_enumvoidSetSignalLatchValidationCheck = VALID;

    if( (Copy_u8EXTINo > EXTI_u8EXTI2 && Copy_u8EXTINo < EXTI_u8EXTI0) || (Copy_u8EXTISense > EXTI_RISING_EDGE) )
    {
        EXTI_enumvoidSetSignalLatchValidationCheck = INVALID;
    }
    else if((Copy_u8EXTINo == EXTI_u8EXTI2) && (Copy_u8EXTISense < EXTI_u8EXTI2 ))
    { 
        EXTI_enumvoidSetSignalLatchValidationCheck = INVALID;
    }
    else
    {
         switch (Copy_u8EXTINo)
        {
            case EXTI_u8EXTI0:
                switch (Copy_u8EXTISense)
                    {
                        case EXTI_LOW_LEVEL: 
                                CLEAR_BIT(EXTI_MCUCR_REG, EXTI_MCUCR_ISC00);
                                CLEAR_BIT(EXTI_MCUCR_REG, EXTI_MCUCR_ISC01);
                                break;
                        case EXTI_ANY_LOGICAL_CHANGE:
                                SET_BIT(EXTI_MCUCR_REG, EXTI_MCUCR_ISC00);
                                CLEAR_BIT(EXTI_MCUCR_REG, EXTI_MCUCR_ISC01);
                                break;
                        case EXTI_FALLING_EDGE:
                                CLEAR_BIT(EXTI_MCUCR_REG, EXTI_MCUCR_ISC00);
                                SET_BIT(EXTI_MCUCR_REG, EXTI_MCUCR_ISC01);
                                break;
                        case EXTI_RISING_EDGE:
                                SET_BIT(EXTI_MCUCR_REG, EXTI_MCUCR_ISC00);
                                SET_BIT(EXTI_MCUCR_REG, EXTI_MCUCR_ISC01);
                                break;
                        default:
                        break;
                    }
                break;

            case EXTI_u8EXTI1:
                switch (Copy_u8EXTISense)
                {
                        case EXTI_LOW_LEVEL: 
                                CLEAR_BIT(EXTI_MCUCR_REG, EXTI_MCUCR_ISC10);
                                CLEAR_BIT(EXTI_MCUCR_REG, EXTI_MCUCR_ISC11);
                                break;
                        case EXTI_ANY_LOGICAL_CHANGE:
                                SET_BIT(EXTI_MCUCR_REG, EXTI_MCUCR_ISC10);
                                CLEAR_BIT(EXTI_MCUCR_REG, EXTI_MCUCR_ISC11);
                                break;
                        case EXTI_FALLING_EDGE:
                                CLEAR_BIT(EXTI_MCUCR_REG, EXTI_MCUCR_ISC10);
                                SET_BIT(EXTI_MCUCR_REG, EXTI_MCUCR_ISC11);
                                break;
                        case EXTI_RISING_EDGE:
                                SET_BIT(EXTI_MCUCR_REG, EXTI_MCUCR_ISC10);
                                SET_BIT(EXTI_MCUCR_REG, EXTI_MCUCR_ISC11);
                                break;
                        default:
                        break;
                    }
                break;

            case EXTI_u8EXTI2:
                switch (Copy_u8EXTISense)
                {
                        case EXTI_FALLING_EDGE:
                                CLEAR_BIT(EXTI_MCUCSR_REG, EXTI_MCUCSR_ISC2);
                                break;
                        case EXTI_RISING_EDGE:
                                SET_BIT(EXTI_MCUCSR_REG, EXTI_MCUCSR_ISC2);                            
                                break;
                        default:
                        break;
                }
                break;
                
                default:
                break;
        }
    }
    return EXTI_enumvoidSetSignalLatchValidationCheck;
}

void EXTI_voidSetCallBack(void(*Ptr_pToFunc)(void), u8 Copy_u8EXTIIndex)
{
    pointerToFunction[Copy_u8EXTIIndex] = Ptr_pToFunc;
}


/*******************************************EXTI0 ISR****************************************************/

void __vector_1(void)   __attribute__((signal));
void __vector_1(void)
{
	if(pointerToFunction[EXTI_u8EXTI0] != NULL)
	{
		pointerToFunction[EXTI_u8EXTI0]();
	}
}



/*******************************************EXTI1 ISR****************************************************/



void __vector_2(void)   __attribute__((signal));
void __vector_2(void)
{
	if(pointerToFunction[EXTI_u8EXTI1] != NULL)
	{
		pointerToFunction[EXTI_u8EXTI1]();
	}
}



/*******************************************EXTI2 ISR****************************************************/



void __vector_3(void)   __attribute__((signal));
void __vector_3(void)
{
	if(pointerToFunction[EXTI_u8EXTI2] != NULL)
	{
		pointerToFunction[EXTI_u8EXTI2]();
	}
}







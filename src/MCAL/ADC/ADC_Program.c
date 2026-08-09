/*
 *<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<   ADC_Program.c   >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
 *
 *  Author : Muhannad Mahfouz El-Shahiedy
 *  Layer  : MCAL
 *  SWC    : ADC
 *  DATE   : 7 AUG 2026
 */ 


#include "../../LIBs/STD_TYPES.h"
#include "../../LIBs/BIT_MATH.h"

#include "../DIO/DIO_Interface.h"
#include "ADC_Interface.h"
#include "ADC_Private.h"
#include "ADC_Config.h"

static volatile ADC_u8BusyFlag = NOTBUSY;
static volatile u16 *ADC_pu16AsynchConversionResult = NULL;
static volatile void (*ADC_pvNotificationFUNC)(void) = NULL;

void ADC_voidInit(void)
{
    /* Reference Voltage Selection */
    #if ADC_REFVOLT == ADC_AREF
        CLEAR_BIT(ADC_ADMUX_REG, ADC_ADMUX_REFS0);
        CLEAR_BIT(ADC_ADMUX_REG, ADC_ADMUX_REFS1);
    #elif ADC_REFVOLT == ADC_AVCC
        SET_BIT(ADC_ADMUX_REG, ADC_ADMUX_REFS0);
        CLEAR_BIT(ADC_ADMUX_REG, ADC_ADMUX_REFS1);
    #elif ADC_REFVOLT == ADC_INTERNAL
        SET_BIT(ADC_ADMUX_REG, ADC_ADMUX_REFS0);
        SET_BIT(ADC_ADMUX_REG, ADC_ADMUX_REFS1);
    #else
        #error "Error In ADC_REFVOLT Configuration"
    #endif

    /* Result Adjustment Selection */
    #if ADC_ADJUST_CTRL == ADC_LEFT
        SET_BIT(ADC_ADMUX_REG, ADC_ADMUX_ADLAR);
    #elif ADC_ADJUST_CTRL == ADC_RIGHT
        CLEAR_BIT(ADC_ADMUX_REG, ADC_ADMUX_ADLAR);
    #else
        #error "Error In ADC_ADJUST_CTRL Configuration"
    #endif

    #if ADC_TRIGGER_SOURCE != ADC_SINGLE_CONVERSION
    /* Select Auto Trigger Source in SFIOR (Bits 7..5)
     * NOTE: ADC_TRIGGER_SOURCE values are 1..8 (ADC_SINGLE_CONVERSION=0 is a
     * software-only sentinel, not a hardware ADTS code), while the ADTS2:0
     * field itself is 0..7 (0=Free Running .. 7=Timer1 Capture Event).
     * Subtract 1 to translate the enum value into the real hardware code. */
    ADC_SFIOR_REG &= ~(0b11100000);
    ADC_SFIOR_REG |= ((ADC_TRIGGER_SOURCE - 1) << ADC_SFIOR_ADTS0);
    /* Enable Auto Triggering */
    SET_BIT(ADC_ADCSRA_REG, ADC_ADCSRA_ADATE);
    #endif

    #if ADC_INTERRUPT == INT_ENABLE
            SET_BIT(INTERRUPT_SREG_REG, INTERRUPT_GIE_BIT);
    #elif ADC_INTERRUPT == INT_DISABLE
        /* Leave global interrupts as the application configures them */
    #else
        #error "Error In ADC_INTERRUPT Configuration"
    #endif

    /* Prescaler Selection */
    ADC_ADCSRA_REG &= ADC_PRES_MASK;
    ADC_ADCSRA_REG |= ADC_PRESCALAR;

    /* Enable ADC Module */
    SET_BIT(ADC_ADCSRA_REG, ADC_ADCSRA_ADEN);
}

ErrorCode ADC_enumReadSynchronus(u8 Copy_u8ChannelNum, u16 *pu16Result)
{
    ErrorCode ADC_ErrorCode_ReadSynchronus = VALID;
    u32 ReadSync_u32TimeoutCounter = 0;

    if (Copy_u8ChannelNum > 7)
    {
        return INVALID;   
    }

    if (pu16Result != NULL)
    {
        if (ADC_u8BusyFlag == NOTBUSY)
        {
            ADC_u8BusyFlag = BUSY;

            /* Configure Pin Direction as Input */
            pinMode(DIO_PORTA, Copy_u8ChannelNum, DIO_PIN_INPUT);

            /* Channel Selection */
            ADC_ADMUX_REG &= ADC_CHANNEL_MASK;
            ADC_ADMUX_REG |= Copy_u8ChannelNum;

            /* Start Conversion */
            SET_BIT(ADC_ADCSRA_REG, ADC_ADCSRA_ADSC);

            /* Wait for conversion completion or timeout */
            while ((READ_BIT(ADC_ADCSRA_REG, ADC_ADCSRA_ADIF) == 0) && (ReadSync_u32TimeoutCounter < ADC_TIMEOUT))
            {
                ReadSync_u32TimeoutCounter++;
            }

            if (ReadSync_u32TimeoutCounter == ADC_TIMEOUT)
            {
                ADC_ErrorCode_ReadSynchronus = TIMEOUT;
            }
            else
            {
                /* Clear Interrupt Flag by writing 1 */
                SET_BIT(ADC_ADCSRA_REG, ADC_ADCSRA_ADIF);

                /* Read Result (Guaranteed Sequence: ADCL then ADCH) */
                #if ADC_ADJUST_CTRL == ADC_RIGHT
                    u16 Local_u16Low = ADC_ADCL_REG;
                    *pu16Result = Local_u16Low | ((u16)ADC_ADCH_REG << 8);
                #elif ADC_ADJUST_CTRL == ADC_LEFT
                    *pu16Result = ADC_ADCH_REG;
                #else
                    #error "Wrong ADC_ADJUSTMENT config"
                #endif
            }

            ADC_u8BusyFlag = NOTBUSY;
        }
        else
        {
            ADC_ErrorCode_ReadSynchronus = BUSY_STATE;
        }
    }
    else
    {
        ADC_ErrorCode_ReadSynchronus = NULL_POINTER;
    }

    return ADC_ErrorCode_ReadSynchronus;
}

ErrorCode ADC_enumReadAsynchronus(u8 Copy_u8ChannelNum, u16 *pu16Result, void (*pvCallBackFuncPtr)(void))
{
    ErrorCode ADC_ErrorCode_ReadAsynchronus = VALID;

    if (Copy_u8ChannelNum > 7)
    {
        return INVALID;
    }

    if ((pu16Result != NULL) && (pvCallBackFuncPtr != NULL))
    {
        if (ADC_u8BusyFlag == NOTBUSY)
        {
            ADC_u8BusyFlag = BUSY;

            ADC_pu16AsynchConversionResult = pu16Result;
            ADC_pvNotificationFUNC = pvCallBackFuncPtr;

            /* Configure Pin Direction as Input */
            pinMode(DIO_PORTA, Copy_u8ChannelNum, DIO_PIN_INPUT);

            /* Channel Selection */
            ADC_ADMUX_REG &= ADC_CHANNEL_MASK;
            ADC_ADMUX_REG |= Copy_u8ChannelNum;

            /* Clear any pending interrupt flag */
            SET_BIT(ADC_ADCSRA_REG, ADC_ADCSRA_ADIF);

            /* Enable ADC Interrupt */
            SET_BIT(ADC_ADCSRA_REG, ADC_ADCSRA_ADIE);

            /* Start Conversion */
            SET_BIT(ADC_ADCSRA_REG, ADC_ADCSRA_ADSC);
        }
        else
        {
            ADC_ErrorCode_ReadAsynchronus = BUSY_STATE;
        }
    }
    else
    {
        ADC_ErrorCode_ReadAsynchronus = NULL_POINTER;
    }

    return ADC_ErrorCode_ReadAsynchronus;
}

void __vector_16 (void) __attribute__((signal));
void __vector_16 (void)
{
    /* Read Result */
    #if ADC_ADJUST_CTRL == ADC_RIGHT
        /* Guaranteed Sequence: ADCL then ADCH witout getting Register Lock */
        u16 Local_u16Low = ADC_ADCL_REG;
        *ADC_pu16AsynchConversionResult = Local_u16Low | ((u16)ADC_ADCH_REG << 8);
    #elif ADC_ADJUST_CTRL == ADC_LEFT
        *ADC_pu16AsynchConversionResult = ADC_ADCH_REG;
    #endif

    /* Disable Interrupt BEFORE Notification Callback to avoid Async Chaining Override */
    CLEAR_BIT(ADC_ADCSRA_REG, ADC_ADCSRA_ADIE);

    ADC_u8BusyFlag = NOTBUSY;

    if (ADC_pvNotificationFUNC != NULL)
    {
        ADC_pvNotificationFUNC();
    }
}
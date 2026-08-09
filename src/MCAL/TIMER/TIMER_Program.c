/*
 *<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<   TIMER_Program.c   >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
 *
 *  Author : Muhannad Mahfouz El-Shahiedy
 *  Layer  : MCAL
 *  SWC    : TIMER
 *
 */ 

#include "../../LIBs/STD_TYPES.h"
#include "../../LIBs/BIT_MATH.h"

#include "../DIO/DIO_Interface.h"
#include "TIMER_Interface.h"
#include "TIMER_Private.h"
#include "TIMER_Config.h"

/* Global pointers to functions to hold the callback addresses for ISRs */
static void (*TIMER0_pvCallBackOVF)(void)  = NULL;
static void (*TIMER0_pvCallBackCTC)(void)  = NULL;

static void (*TIMER1_pvCallBackOVF)(void)   = NULL;
static void (*TIMER1_pvCallBackCompA)(void) = NULL;
static void (*TIMER1_pvCallBackCompB)(void) = NULL;
static void (*TIMER1_pvCallBackICU)(void)   = NULL;

static void (*TIMER2_pvCallBackOVF)(void)  = NULL;
static void (*TIMER2_pvCallBackCTC)(void)  = NULL;

/* ==================================================================== */
/*                              Timer0                                   */
/* ==================================================================== */

void TIMER0_voidInit(void)
{
    /* 1. Configure Timer Mode */
#if TIMER0_MODE == TIMER0_NORMAL_MODE
    CLEAR_BIT(TCCR0, WGM00);
    CLEAR_BIT(TCCR0, WGM01);

#elif TIMER0_MODE == TIMER0_CTC_MODE
    CLEAR_BIT(TCCR0, WGM00);
    SET_BIT(TCCR0, WGM01);

#elif TIMER0_MODE == TIMER0_FAST_PWM_MODE
    SET_BIT(TCCR0, WGM00);
    SET_BIT(TCCR0, WGM01);

    /* Non-inverting: clear OC0 on compare match, set at BOTTOM */
    CLEAR_BIT(TCCR0, COM00);
    SET_BIT(TCCR0, COM01);

    /* OC0 = PB3, must be output for the hardware waveform to appear on the pin */
    pinMode(DIO_PORTB, DIO_PIN3, DIO_PIN_OUTPUT);

#elif TIMER0_MODE == TIMER0_PHASE_PWM_MODE
    SET_BIT(TCCR0, WGM00);
    CLEAR_BIT(TCCR0, WGM01);

    /* Non-inverting: clear OC0 on compare match while up-counting */
    CLEAR_BIT(TCCR0, COM00);
    SET_BIT(TCCR0, COM01);

    pinMode(DIO_PORTB, DIO_PIN3, DIO_PIN_OUTPUT);

#else
    #error "Error In TIMER0_MODE Configuration"
#endif

    /* 2. Configure Clock & Prescaler */
    TCCR0 &= 0xF8;
    TCCR0 |= TIMER0_PRESCALER;

    /* NOTE: TOIE0/OCIE0 are intentionally NOT enabled here.
     * They are enabled automatically inside TIMER0_voidSetCallBackOVF() /
     * TIMER0_voidSetCallBackCTC() so the timer doesn't take interrupt
     * overhead you never asked for (e.g. pure hardware PWM, or polling TOV0
     * yourself). */
}

void TIMER0_voidSetPreload(u8 Copy_u8Preload)
{
    TCNT0 = Copy_u8Preload;
}

void TIMER0_voidSetCompareMatch(u8 Copy_u8CompareValue)
{
    OCR0 = Copy_u8CompareValue;
}

void TIMER0_voidSetPWMDutyCycle(u8 Copy_u8DutyCycle)
{
    u16 Local_u16OCR;

    if (Copy_u8DutyCycle > 100)
    {
        Copy_u8DutyCycle = 100;
    }

    /* Timer0 is 8-bit with a fixed TOP of 0xFF in both PWM modes */
    Local_u16OCR = (u16)(((u32)Copy_u8DutyCycle * 255U) / 100U);
    OCR0 = (u8)Local_u16OCR;
}

void TIMER0_voidSetCallBackOVF(void (*Copy_pvCallBackFunc)(void))
{
    if (Copy_pvCallBackFunc != NULL)
    {
        TIMER0_pvCallBackOVF = Copy_pvCallBackFunc;
        SET_BIT(TIMSK, TOIE0);
    }
}

void TIMER0_voidSetCallBackCTC(void (*Copy_pvCallBackFunc)(void))
{
    if (Copy_pvCallBackFunc != NULL)
    {
        TIMER0_pvCallBackCTC = Copy_pvCallBackFunc;
        SET_BIT(TIMSK, OCIE0);
    }
}

/* ==================================================================== */
/*                              Timer1                                   */
/* ==================================================================== */

void TIMER1_voidInit(void)
{
#if TIMER1_MODE == TIMER1_NORMAL_MODE
    CLEAR_BIT(TCCR1A, WGM10); CLEAR_BIT(TCCR1A, WGM11);
    CLEAR_BIT(TCCR1B, WGM12); CLEAR_BIT(TCCR1B, WGM13);

#elif TIMER1_MODE == TIMER1_CTC_MODE
    /* TOP = OCR1A */
    CLEAR_BIT(TCCR1A, WGM10); CLEAR_BIT(TCCR1A, WGM11);
    SET_BIT(TCCR1B, WGM12);   CLEAR_BIT(TCCR1B, WGM13);

#elif TIMER1_MODE == TIMER1_FAST_PWM_8BIT
    SET_BIT(TCCR1A, WGM10); CLEAR_BIT(TCCR1A, WGM11);
    SET_BIT(TCCR1B, WGM12); CLEAR_BIT(TCCR1B, WGM13);
    SET_BIT(TCCR1A, COM1A1); CLEAR_BIT(TCCR1A, COM1A0);
    SET_BIT(TCCR1A, COM1B1); CLEAR_BIT(TCCR1A, COM1B0);
    pinMode(DIO_PORTD, DIO_PIN5, DIO_PIN_OUTPUT); /* OC1A */
    pinMode(DIO_PORTD, DIO_PIN4, DIO_PIN_OUTPUT); /* OC1B */

#elif TIMER1_MODE == TIMER1_FAST_PWM_9BIT
    CLEAR_BIT(TCCR1A, WGM10); SET_BIT(TCCR1A, WGM11);
    SET_BIT(TCCR1B, WGM12);   CLEAR_BIT(TCCR1B, WGM13);
    SET_BIT(TCCR1A, COM1A1); CLEAR_BIT(TCCR1A, COM1A0);
    SET_BIT(TCCR1A, COM1B1); CLEAR_BIT(TCCR1A, COM1B0);
    pinMode(DIO_PORTD, DIO_PIN5, DIO_PIN_OUTPUT);
    pinMode(DIO_PORTD, DIO_PIN4, DIO_PIN_OUTPUT);

#elif TIMER1_MODE == TIMER1_FAST_PWM_10BIT
    SET_BIT(TCCR1A, WGM10); SET_BIT(TCCR1A, WGM11);
    SET_BIT(TCCR1B, WGM12); CLEAR_BIT(TCCR1B, WGM13);
    SET_BIT(TCCR1A, COM1A1); CLEAR_BIT(TCCR1A, COM1A0);
    SET_BIT(TCCR1A, COM1B1); CLEAR_BIT(TCCR1A, COM1B0);
    pinMode(DIO_PORTD, DIO_PIN5, DIO_PIN_OUTPUT);
    pinMode(DIO_PORTD, DIO_PIN4, DIO_PIN_OUTPUT);

#elif TIMER1_MODE == TIMER1_PHASE_PWM_8BIT
    SET_BIT(TCCR1A, WGM10); CLEAR_BIT(TCCR1A, WGM11);
    CLEAR_BIT(TCCR1B, WGM12); CLEAR_BIT(TCCR1B, WGM13);
    SET_BIT(TCCR1A, COM1A1); CLEAR_BIT(TCCR1A, COM1A0);
    SET_BIT(TCCR1A, COM1B1); CLEAR_BIT(TCCR1A, COM1B0);
    pinMode(DIO_PORTD, DIO_PIN5, DIO_PIN_OUTPUT);
    pinMode(DIO_PORTD, DIO_PIN4, DIO_PIN_OUTPUT);

#elif TIMER1_MODE == TIMER1_PHASE_PWM_9BIT
    CLEAR_BIT(TCCR1A, WGM10); SET_BIT(TCCR1A, WGM11);
    CLEAR_BIT(TCCR1B, WGM12); CLEAR_BIT(TCCR1B, WGM13);
    SET_BIT(TCCR1A, COM1A1); CLEAR_BIT(TCCR1A, COM1A0);
    SET_BIT(TCCR1A, COM1B1); CLEAR_BIT(TCCR1A, COM1B0);
    pinMode(DIO_PORTD, DIO_PIN5, DIO_PIN_OUTPUT);
    pinMode(DIO_PORTD, DIO_PIN4, DIO_PIN_OUTPUT);

#elif TIMER1_MODE == TIMER1_PHASE_PWM_10BIT
    SET_BIT(TCCR1A, WGM10); SET_BIT(TCCR1A, WGM11);
    CLEAR_BIT(TCCR1B, WGM12); CLEAR_BIT(TCCR1B, WGM13);
    SET_BIT(TCCR1A, COM1A1); CLEAR_BIT(TCCR1A, COM1A0);
    SET_BIT(TCCR1A, COM1B1); CLEAR_BIT(TCCR1A, COM1B0);
    pinMode(DIO_PORTD, DIO_PIN5, DIO_PIN_OUTPUT);
    pinMode(DIO_PORTD, DIO_PIN4, DIO_PIN_OUTPUT);

#else
    #error "Error In TIMER1_MODE Configuration"
#endif

    /* Prescaler (CS12:10) */
    TCCR1B &= 0xF8;
    TCCR1B |= TIMER1_PRESCALER;
}

void TIMER1_voidSetPreload(u16 Copy_u16Preload)
{
    /* Per datasheet: for 16-bit register writes, high byte must be
     * written before the low byte (uses the shared TEMP register). */
    TCNT1H = (u8)(Copy_u16Preload >> 8);
    TCNT1L = (u8)(Copy_u16Preload);
}

u16 TIMER1_u16GetTimerValue(void)
{
    u8 Local_u8Low;
    u8 Local_u8High;

    /* Per datasheet: for 16-bit register reads, low byte must be
     * read before the high byte. */
    Local_u8Low  = TCNT1L;
    Local_u8High = TCNT1H;

    return (u16)(((u16)Local_u8High << 8) | Local_u8Low);
}

void TIMER1_voidSetCompareMatchA(u16 Copy_u16CompareValue)
{
    OCR1AH = (u8)(Copy_u16CompareValue >> 8);
    OCR1AL = (u8)(Copy_u16CompareValue);
}

void TIMER1_voidSetCompareMatchB(u16 Copy_u16CompareValue)
{
    OCR1BH = (u8)(Copy_u16CompareValue >> 8);
    OCR1BL = (u8)(Copy_u16CompareValue);
}

/* Resolves the fixed TOP value for the currently configured Timer1 PWM mode */
#if (TIMER1_MODE == TIMER1_FAST_PWM_8BIT) || (TIMER1_MODE == TIMER1_PHASE_PWM_8BIT)
    #define TIMER1_PWM_TOP     255U
#elif (TIMER1_MODE == TIMER1_FAST_PWM_9BIT) || (TIMER1_MODE == TIMER1_PHASE_PWM_9BIT)
    #define TIMER1_PWM_TOP     511U
#elif (TIMER1_MODE == TIMER1_FAST_PWM_10BIT) || (TIMER1_MODE == TIMER1_PHASE_PWM_10BIT)
    #define TIMER1_PWM_TOP     1023U
#else
    #define TIMER1_PWM_TOP     0U   /* Not a PWM mode; duty functions are meaningless here */
#endif

void TIMER1_voidSetPWMDutyCycleA(u8 Copy_u8DutyCycle)
{
    u16 Local_u16OCR;

    if (Copy_u8DutyCycle > 100)
    {
        Copy_u8DutyCycle = 100;
    }

    Local_u16OCR = (u16)(((u32)Copy_u8DutyCycle * TIMER1_PWM_TOP) / 100U);
    TIMER1_voidSetCompareMatchA(Local_u16OCR);
}

void TIMER1_voidSetPWMDutyCycleB(u8 Copy_u8DutyCycle)
{
    u16 Local_u16OCR;

    if (Copy_u8DutyCycle > 100)
    {
        Copy_u8DutyCycle = 100;
    }

    Local_u16OCR = (u16)(((u32)Copy_u8DutyCycle * TIMER1_PWM_TOP) / 100U);
    TIMER1_voidSetCompareMatchB(Local_u16OCR);
}

void TIMER1_voidICUSetEdge(u8 Copy_u8Edge)
{
    if (Copy_u8Edge == TIMER1_ICU_RISING_EDGE)
    {
        SET_BIT(TCCR1B, ICES1);
    }
    else
    {
        CLEAR_BIT(TCCR1B, ICES1);
    }
}

void TIMER1_voidICUNoiseCancelerEnable(void)
{
    SET_BIT(TCCR1B, ICNC1);
}

void TIMER1_voidICUNoiseCancelerDisable(void)
{
    CLEAR_BIT(TCCR1B, ICNC1);
}

u16 TIMER1_u16ICUGetCapturedValue(void)
{
    u8 Local_u8Low;
    u8 Local_u8High;

    Local_u8Low  = ICR1L;
    Local_u8High = ICR1H;

    return (u16)(((u16)Local_u8High << 8) | Local_u8Low);
}

void TIMER1_voidSetCallBackOVF(void (*Copy_pvCallBackFunc)(void))
{
    if (Copy_pvCallBackFunc != NULL)
    {
        TIMER1_pvCallBackOVF = Copy_pvCallBackFunc;
        SET_BIT(TIMSK, TOIE1);
    }
}

void TIMER1_voidSetCallBackCompA(void (*Copy_pvCallBackFunc)(void))
{
    if (Copy_pvCallBackFunc != NULL)
    {
        TIMER1_pvCallBackCompA = Copy_pvCallBackFunc;
        SET_BIT(TIMSK, OCIE1A);
    }
}

void TIMER1_voidSetCallBackCompB(void (*Copy_pvCallBackFunc)(void))
{
    if (Copy_pvCallBackFunc != NULL)
    {
        TIMER1_pvCallBackCompB = Copy_pvCallBackFunc;
        SET_BIT(TIMSK, OCIE1B);
    }
}

void TIMER1_voidSetCallBackICU(void (*Copy_pvCallBackFunc)(void))
{
    if (Copy_pvCallBackFunc != NULL)
    {
        TIMER1_pvCallBackICU = Copy_pvCallBackFunc;
        SET_BIT(TIMSK, TICIE1);
    }
}

/* ==================================================================== */
/*                              Timer2                                   */
/* ==================================================================== */

void TIMER2_voidInit(void)
{
#if TIMER2_MODE == TIMER2_NORMAL_MODE
    CLEAR_BIT(TCCR2, WGM20);
    CLEAR_BIT(TCCR2, WGM21);

#elif TIMER2_MODE == TIMER2_CTC_MODE
    CLEAR_BIT(TCCR2, WGM20);
    SET_BIT(TCCR2, WGM21);

#elif TIMER2_MODE == TIMER2_FAST_PWM_MODE
    SET_BIT(TCCR2, WGM20);
    SET_BIT(TCCR2, WGM21);

    /* Non-inverting: clear OC2 on compare match, set at BOTTOM */
    CLEAR_BIT(TCCR2, COM20);
    SET_BIT(TCCR2, COM21);

    /* OC2 = PD7 */
    pinMode(DIO_PORTD, DIO_PIN7, DIO_PIN_OUTPUT);

#elif TIMER2_MODE == TIMER2_PHASE_PWM_MODE
    SET_BIT(TCCR2, WGM20);
    CLEAR_BIT(TCCR2, WGM21);

    CLEAR_BIT(TCCR2, COM20);
    SET_BIT(TCCR2, COM21);

    pinMode(DIO_PORTD, DIO_PIN7, DIO_PIN_OUTPUT);

#else
    #error "Error In TIMER2_MODE Configuration"
#endif

    /* Clock Source: internal system clock, or external 32.768kHz
     * async crystal on TOSC1/TOSC2 for RTC-style use cases */
#if TIMER2_ASYNC == TIMER2_ASYNC_ENABLE
    SET_BIT(ASSR, AS2);
    /* Per datasheet: after switching to async mode, TCNT2/OCR2/TCCR2 must
     * not be written again until these busy flags clear, otherwise the
     * write is dropped. */
    while ((READ_BIT(ASSR, TCN2UB) != 0) ||
           (READ_BIT(ASSR, OCR2UB) != 0) ||
           (READ_BIT(ASSR, TCR2UB) != 0))
    {
        /* wait for oscillator/register sync */
    }
#elif TIMER2_ASYNC == TIMER2_ASYNC_DISABLE
    CLEAR_BIT(ASSR, AS2);
#else
    #error "Error In TIMER2_ASYNC Configuration"
#endif

    /* Prescaler (CS22:20) - Timer2's own unique tap set */
    TCCR2 &= 0xF8;
    TCCR2 |= TIMER2_PRESCALER;
}

void TIMER2_voidSetPreload(u8 Copy_u8Preload)
{
    TCNT2 = Copy_u8Preload;
}

void TIMER2_voidSetCompareMatch(u8 Copy_u8CompareValue)
{
    OCR2 = Copy_u8CompareValue;
}

void TIMER2_voidSetPWMDutyCycle(u8 Copy_u8DutyCycle)
{
    u16 Local_u16OCR;

    if (Copy_u8DutyCycle > 100)
    {
        Copy_u8DutyCycle = 100;
    }

    Local_u16OCR = (u16)(((u32)Copy_u8DutyCycle * 255U) / 100U);
    OCR2 = (u8)Local_u16OCR;
}

void TIMER2_voidSetCallBackOVF(void (*Copy_pvCallBackFunc)(void))
{
    if (Copy_pvCallBackFunc != NULL)
    {
        TIMER2_pvCallBackOVF = Copy_pvCallBackFunc;
        SET_BIT(TIMSK, TOIE2);
    }
}

void TIMER2_voidSetCallBackCTC(void (*Copy_pvCallBackFunc)(void))
{
    if (Copy_pvCallBackFunc != NULL)
    {
        TIMER2_pvCallBackCTC = Copy_pvCallBackFunc;
        SET_BIT(TIMSK, OCIE2);
    }
}

/* ==================================================================== */
/*                      Interrupt Service Routines                      */
/*    (ATmega32 vector numbering: 4=T2_COMP 5=T2_OVF 6=T1_CAPT           */
/*     7=T1_COMPA 8=T1_COMPB 9=T1_OVF 10=T0_COMP 11=T0_OVF)              */
/* ==================================================================== */

/* Timer2 Compare Match ISR (Vector 4) */
void __vector_4(void) __attribute__((signal));
void __vector_4(void)
{
    if (TIMER2_pvCallBackCTC != NULL)
    {
        TIMER2_pvCallBackCTC();
    }
}

/* Timer2 Overflow ISR (Vector 5) */
void __vector_5(void) __attribute__((signal));
void __vector_5(void)
{
    if (TIMER2_pvCallBackOVF != NULL)
    {
        TIMER2_pvCallBackOVF();
    }
}

/* Timer1 Input Capture ISR (Vector 6) */
void __vector_6(void) __attribute__((signal));
void __vector_6(void)
{
    if (TIMER1_pvCallBackICU != NULL)
    {
        TIMER1_pvCallBackICU();
    }
}

/* Timer1 Compare Match A ISR (Vector 7) */
void __vector_7(void) __attribute__((signal));
void __vector_7(void)
{
    if (TIMER1_pvCallBackCompA != NULL)
    {
        TIMER1_pvCallBackCompA();
    }
}

/* Timer1 Compare Match B ISR (Vector 8) */
void __vector_8(void) __attribute__((signal));
void __vector_8(void)
{
    if (TIMER1_pvCallBackCompB != NULL)
    {
        TIMER1_pvCallBackCompB();
    }
}

/* Timer1 Overflow ISR (Vector 9) */
void __vector_9(void) __attribute__((signal));
void __vector_9(void)
{
    if (TIMER1_pvCallBackOVF != NULL)
    {
        TIMER1_pvCallBackOVF();
    }
}

/* Timer0 Compare Match ISR (Vector 10) */
void __vector_10(void) __attribute__((signal));
void __vector_10(void)
{
    if (TIMER0_pvCallBackCTC != NULL)
    {
        TIMER0_pvCallBackCTC();
    }
}

/* Timer0 Overflow ISR (Vector 11) */
void __vector_11(void) __attribute__((signal));
void __vector_11(void)
{
    if (TIMER0_pvCallBackOVF != NULL)
    {
        TIMER0_pvCallBackOVF();
    }
}
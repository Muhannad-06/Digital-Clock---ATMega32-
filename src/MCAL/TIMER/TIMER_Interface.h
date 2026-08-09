/*
 *<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<   TIMER_Interface.h   >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
 *
 *  Author : Muhannad Mahfouz El-Shahiedy
 *  Layer  : MCAL
 *  SWC    : TIMER
 *
 */ 

#ifndef TIMER_INTERFACE_H
#define TIMER_INTERFACE_H

#include "../../LIBs/STD_TYPES.h"

/* Timer1 Input Capture Unit - Edge Selection */
#define TIMER1_ICU_FALLING_EDGE     0
#define TIMER1_ICU_RISING_EDGE      1

/* ============================================================ */
/*                          Timer0                               */
/* ============================================================ */

/* Initializes Timer0 based on the configuration in TIMER_Config.h */
void TIMER0_voidInit(void);

/* Sets the initial value for the timer in Normal/CTC/PWM modes */
void TIMER0_voidSetPreload(u8 Copy_u8Preload);

/* Sets the compare match value (OCR0) for CTC or PWM modes */
void TIMER0_voidSetCompareMatch(u8 Copy_u8CompareValue);

/* Sets the OC0 (PB3) PWM duty cycle as a percentage (0-100).
 * Only meaningful when TIMER0_MODE is a PWM mode. */
void TIMER0_voidSetPWMDutyCycle(u8 Copy_u8DutyCycle);

/* Assigns the callback executed on Overflow Interrupt and enables it */
void TIMER0_voidSetCallBackOVF(void (*Copy_pvCallBackFunc)(void));

/* Assigns the callback executed on Compare Match Interrupt and enables it */
void TIMER0_voidSetCallBackCTC(void (*Copy_pvCallBackFunc)(void));

/* ============================================================ */
/*                          Timer1                               */
/* ============================================================ */

/* Initializes Timer1 based on the configuration in TIMER_Config.h */
void TIMER1_voidInit(void);

/* Sets/reads the 16-bit timer count (TCNT1) */
void TIMER1_voidSetPreload(u16 Copy_u16Preload);
u16  TIMER1_u16GetTimerValue(void);

/* Sets the compare match values (OCR1A/OCR1B) for CTC or PWM modes */
void TIMER1_voidSetCompareMatchA(u16 Copy_u16CompareValue);
void TIMER1_voidSetCompareMatchB(u16 Copy_u16CompareValue);

/* Sets the OC1A (PD5) / OC1B (PD4) PWM duty cycle as a percentage (0-100).
 * Resolution (8/9/10-bit) is whatever TIMER1_MODE selected. */
void TIMER1_voidSetPWMDutyCycleA(u8 Copy_u8DutyCycle);
void TIMER1_voidSetPWMDutyCycleB(u8 Copy_u8DutyCycle);

/* Input Capture Unit (ICU) - captures TCNT1 into ICR1 on the chosen edge */
void TIMER1_voidICUSetEdge(u8 Copy_u8Edge);
void TIMER1_voidICUNoiseCancelerEnable(void);
void TIMER1_voidICUNoiseCancelerDisable(void);
u16  TIMER1_u16ICUGetCapturedValue(void);

/* Assigns callbacks and enables the corresponding interrupt */
void TIMER1_voidSetCallBackOVF(void (*Copy_pvCallBackFunc)(void));
void TIMER1_voidSetCallBackCompA(void (*Copy_pvCallBackFunc)(void));
void TIMER1_voidSetCallBackCompB(void (*Copy_pvCallBackFunc)(void));
void TIMER1_voidSetCallBackICU(void (*Copy_pvCallBackFunc)(void));

/* ============================================================ */
/*                          Timer2                               */
/* ============================================================ */

/* Initializes Timer2 based on the configuration in TIMER_Config.h */
void TIMER2_voidInit(void);

/* Sets the initial value for the timer in Normal/CTC/PWM modes */
void TIMER2_voidSetPreload(u8 Copy_u8Preload);

/* Sets the compare match value (OCR2) for CTC or PWM modes */
void TIMER2_voidSetCompareMatch(u8 Copy_u8CompareValue);

/* Sets the OC2 (PD7) PWM duty cycle as a percentage (0-100).
 * Only meaningful when TIMER2_MODE is a PWM mode. */
void TIMER2_voidSetPWMDutyCycle(u8 Copy_u8DutyCycle);

/* Assigns the callback executed on Overflow Interrupt and enables it */
void TIMER2_voidSetCallBackOVF(void (*Copy_pvCallBackFunc)(void));

/* Assigns the callback executed on Compare Match Interrupt and enables it */
void TIMER2_voidSetCallBackCTC(void (*Copy_pvCallBackFunc)(void));

#endif /* TIMER_INTERFACE_H */
/*
 *<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<   TIMER_Private.h   >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
 *
 *  Author : Muhannad Mahfouz El-Shahiedy
 *  Layer  : MCAL
 *  SWC    : TIMER
 *
 */ 


#ifndef TIMER_PRIVATE_H
#define TIMER_PRIVATE_H

/* ============================================================ */
/*                      Timer0 Registers                        */
/* ============================================================ */
#define TCCR0       *((volatile u8*)0x53)
#define TCNT0       *((volatile u8*)0x52)
#define OCR0        *((volatile u8*)0x5C)

/* TCCR0 Bits */
#define FOC0        7
#define WGM00       6
#define COM01       5
#define COM00       4
#define WGM01       3
#define CS02        2
#define CS01        1
#define CS00        0

/* ============================================================ */
/*                      Timer1 Registers                        */
/* ============================================================ */
#define TCCR1A      *((volatile u8*)0x4F)
#define TCCR1B      *((volatile u8*)0x4E)
#define TCNT1H      *((volatile u8*)0x4D)
#define TCNT1L      *((volatile u8*)0x4C)
#define OCR1AH      *((volatile u8*)0x4B)
#define OCR1AL      *((volatile u8*)0x4A)
#define OCR1BH      *((volatile u8*)0x49)
#define OCR1BL      *((volatile u8*)0x48)
#define ICR1H       *((volatile u8*)0x47)
#define ICR1L       *((volatile u8*)0x46)

/* TCCR1A Bits */
#define COM1A1      7
#define COM1A0      6
#define COM1B1      5
#define COM1B0      4
#define FOC1A       3
#define FOC1B       2
#define WGM11       1
#define WGM10       0

/* TCCR1B Bits */
#define ICNC1       7
#define ICES1       6
/* Bit 5 reserved */
#define WGM13       4
#define WGM12       3
#define CS12        2
#define CS11        1
#define CS10        0

/* ============================================================ */
/*                      Timer2 Registers                        */
/* ============================================================ */
#define TCCR2       *((volatile u8*)0x45)
#define TCNT2       *((volatile u8*)0x44)
#define OCR2        *((volatile u8*)0x43)
#define ASSR        *((volatile u8*)0x42)

/* TCCR2 Bits (same layout as TCCR0) */
#define FOC2        7
#define WGM20       6
#define COM21       5
#define COM20       4
#define WGM21       3
#define CS22        2
#define CS21        1
#define CS20        0

/* ASSR Bits */
#define AS2         3
#define TCN2UB      2
#define OCR2UB      1
#define TCR2UB      0

/* ============================================================ */
/*                Shared Interrupt Registers                    */
/* ============================================================ */
#define TIMSK       *((volatile u8*)0x59)
#define TIFR        *((volatile u8*)0x58)

/* TIMSK Bits */
#define OCIE2       7
#define TOIE2       6
#define TICIE1      5
#define OCIE1A      4
#define OCIE1B      3
#define TOIE1       2
#define OCIE0       1
#define TOIE0       0

/* TIFR Bits (mirror TIMSK positions) */
#define OCF2        7
#define TOV2        6
#define ICF1        5
#define OCF1A       4
#define OCF1B       3
#define TOV1        2
#define OCF0        1
#define TOV0        0

/* ============================================================ */
/*                    Mode / Prescaler Codes                    */
/* ============================================================ */

/* Timer0 Modes */
#define TIMER0_NORMAL_MODE          1
#define TIMER0_PHASE_PWM_MODE       2
#define TIMER0_CTC_MODE             3
#define TIMER0_FAST_PWM_MODE        4

/* Timer0 Prescaler Options (values = CS02:00 hardware code) */
#define TIMER0_NO_CLOCK             0
#define TIMER0_PRESCALER_1          1
#define TIMER0_PRESCALER_8          2
#define TIMER0_PRESCALER_64         3
#define TIMER0_PRESCALER_256        4
#define TIMER0_PRESCALER_1024       5

/* Timer1 Modes (subset commonly used; TOP fixed per mode, no ICR1-as-TOP variants) */
#define TIMER1_NORMAL_MODE          1
#define TIMER1_CTC_MODE             2   /* TOP = OCR1A */
#define TIMER1_FAST_PWM_8BIT        3   /* TOP = 0x00FF */
#define TIMER1_FAST_PWM_9BIT        4   /* TOP = 0x01FF */
#define TIMER1_FAST_PWM_10BIT       5   /* TOP = 0x03FF */
#define TIMER1_PHASE_PWM_8BIT       6   /* TOP = 0x00FF */
#define TIMER1_PHASE_PWM_9BIT       7   /* TOP = 0x01FF */
#define TIMER1_PHASE_PWM_10BIT      8   /* TOP = 0x03FF */

/* Timer1 Prescaler Options (values = CS12:10 hardware code, internal clock only) */
#define TIMER1_NO_CLOCK             0
#define TIMER1_PRESCALER_1          1
#define TIMER1_PRESCALER_8          2
#define TIMER1_PRESCALER_64         3
#define TIMER1_PRESCALER_256        4
#define TIMER1_PRESCALER_1024       5

/* Timer2 Modes (same layout as Timer0) */
#define TIMER2_NORMAL_MODE          1
#define TIMER2_PHASE_PWM_MODE       2
#define TIMER2_CTC_MODE             3
#define TIMER2_FAST_PWM_MODE        4

/* Timer2 Prescaler Options (values = CS22:20 hardware code)
 * NOTE: Timer2's prescaler taps are DIFFERENT from Timer0/Timer1
 * (32 and 128 exist here but not on Timer0/1). Do not reuse Timer0/1
 * prescaler macros for Timer2. */
#define TIMER2_NO_CLOCK             0
#define TIMER2_PRESCALER_1          1
#define TIMER2_PRESCALER_8          2
#define TIMER2_PRESCALER_32         3
#define TIMER2_PRESCALER_64         4
#define TIMER2_PRESCALER_128        5
#define TIMER2_PRESCALER_256        6
#define TIMER2_PRESCALER_1024       7

/* Timer2 Async Clock Source (TOSC1/TOSC2 external 32.768kHz crystal) */
#define TIMER2_ASYNC_DISABLE        0
#define TIMER2_ASYNC_ENABLE         1

#endif /* TIMER_PRIVATE_H */
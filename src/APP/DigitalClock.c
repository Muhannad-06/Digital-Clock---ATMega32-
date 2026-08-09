#include "STD_TYPES.h"
#include "BIT_MATH.h"

#include "DIO_Interface.h"
#include "TIMER_Interface.h"
#include "ADC_Interface.h"
#include "EXTI_Interface.h"
#include "EXTI_Private.h"

#include "LCD_Interface.h"
#include "APP.h"

/* Global State Variables */
volatile ClockMode_t CurrentMode = MODE_CLOCK;
volatile Time_t CurrentTime = {0, 0, 12}; // Start at 12:00:00
volatile Time_t AlarmTime   = {0, 0, 0};
volatile u8 TimerMinutes = 0;
volatile u8 TimerSeconds = 0;
volatile u8 TimerActiveFlag = 0;
volatile u8 AlarmActiveFlag = 1; 

/* Mode Switch Button ISR */
void ISR_ModeSwitch(void) {
    CurrentMode++;
    if (CurrentMode > MODE_DATE) {
        CurrentMode = MODE_CLOCK;
    }
    LCD_voidClearScreen(); // Clear screen whenever mode changes
}

/* 1-Second Tick ISR (Ideally from Timer2 Async Overflow) */
void ISR_OneSecondTick(void) {
    /* 1. Update Clock */
    CurrentTime.seconds++;
    if (CurrentTime.seconds >= 60) {
        CurrentTime.seconds = 0;
        CurrentTime.minutes++;
        if (CurrentTime.minutes >= 60) {
            CurrentTime.minutes = 0;
            CurrentTime.hours++;
            if (CurrentTime.hours >= 24) CurrentTime.hours = 0;
        }
    }

    /* 2. Check Alarm */
    if (AlarmActiveFlag && 
        CurrentTime.hours == AlarmTime.hours && 
        CurrentTime.minutes == AlarmTime.minutes && 
        CurrentTime.seconds == 0) {
        // TRIGGER ALARM BUZZER HERE
    }

    /* 3. Update Background Timer (Mode 2) */
    if (TimerActiveFlag) {
        if (TimerSeconds == 0) {
            if (TimerMinutes > 0) {
                TimerMinutes--;
                TimerSeconds = 59;
            } else {
                /* Timer Finished! */
                TimerActiveFlag = 0;
                // TRIGGER TIMER RINGTONE HERE
            }
        } else {
            TimerSeconds--;
        }
        
        /* 9-Second Countdown Logic for 7-Segment */
        if (TimerMinutes == 0 && TimerSeconds <= 9 && TimerSeconds > 0) {
            // CALL 7-SEGMENT DRIVER TO DISPLAY 'TimerSeconds'
        }
    }
}

int main(void) {
    /* Initialize HAL and MCAL */
    LCD_voidInit();
    ADC_voidInit();
    
    /* Initialize EXTI for Mode Button */
    EXTI_voidSetCallBack(ISR_ModeSwitch, EXTI_u8EXTI0);
    EXTI_enumEnable(EXTI_u8EXTI0, EXTI_FALLING_EDGE);
    
    /* Initialize Timer for 1-second Tick (Assuming Timer2 RTC setup) */
    TIMER2_voidSetCallBackOVF(ISR_OneSecondTick);
    TIMER2_voidInit(); 

    /* Load Custom Calendar Char to CGRAM */
    u8 calendarIcon[8] = {0x1F, 0x15, 0x1F, 0x11, 0x15, 0x11, 0x1F, 0x00}; // Example
    LCD_voidCreateCustomChar(calendarIcon, 0);

    while (1) {
        /* State Machine */
        switch (CurrentMode) {
            case MODE_CLOCK:
                APP_voidDisplayClock();
                break;
                
            case MODE_TIMER:
                APP_voidHandleTimerMode();
                break;
                
            case MODE_ALARM:
                APP_voidHandleAlarmMode();
                break;
                
            case MODE_DATE:
                APP_voidDisplayDate();
                break;
        }
    }
    return 0;
}
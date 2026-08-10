#include "../LIBs/STD_TYPES.h"
#include "../LIBs/BIT_MATH.h"
#include "APP.h"

#include "../MCAL/DIO/DIO_Interface.h"
#include "../MCAL/TIMER/TIMER_Interface.h"
#include "../MCAL/ADC/ADC_Interface.h"
#include "../MCAL/EXTI/EXTI_Interface.h"

#include "../HAL/LCD/LCD_Interface.h"
#include "../HAL/7Segment/Seg_Interface.h"

/*
 * ============================================================================
 *  Pin map (from the schematic, ATmega32 U1):
 * ----------------------------------------------------------------------------
 *  LCD   D4..D7        -> PA4..PA7           (LCD_Config.h: LCD_DATA_PORT)
 *  LCD   RS/RW/EN      -> PC0/PC1/PC2        (LCD_Config.h: LCD_CTRL_PORT)
 *  POT   (RV1)         -> PA0 / ADC0
 *  7-SEG SEGA..SEGG    -> PB0..PB6           (Direct connection, no BCD)
 *  BUZZER              -> PC5
 *  ModeSwitch          -> PD2 / INT0
 *  TimerSet            -> PD3 / INT1
 *  TimerStart          -> PD4 (polled digital input)
 *  TimerToggle         -> PD6 (polled digital input)
 * ============================================================================
 */

#define BUZZ_PORT       DIO_PORTC
#define BUZZ_PIN        DIO_PIN5

#define TIMERSTART_PORT DIO_PORTD
#define TIMERSTART_PIN  DIO_PIN4

#define TIMERTOGGLE_PORT DIO_PORTD
#define TIMERTOGGLE_PIN  DIO_PIN6

#define SEG_PORT        DIO_PORTB
#define SEG_TYPE        COMMON_CATHODE

/* How many seconds the buzzer rings for once triggered */
#define BUZZER_BEEP_SECONDS 5

/* Timer1 CTC compare value for a 1.000s tick @ F_CPU=8MHz, prescaler 1024 */
#define TIMER1_1HZ_COMPARE   7812

/* Global State Variables */
volatile ClockMode_t CurrentMode = MODE_CLOCK;
volatile Time_t CurrentTime = {0, 0, 12}; // Start at 12:00:00
volatile Time_t AlarmTime   = {0, 0, 0};
volatile u8 TimerMinutes = 0;
volatile u8 TimerSeconds = 0;
volatile u8 TimerActiveFlag = 0;
volatile u8 AlarmActiveFlag = 1;
volatile u8 BuzzerBeepCounter = 0;

/* ---> FIX: This flag prevents LCD noise by throttling updates <--- */
volatile u8 UpdateDisplayFlag = 1;

/* Date is only tracked/displayed in MODE_DATE */
static volatile Date_t CurrentDate = {1, 1, 2026};

/* Small helper: print a 0..59 value as two digits (zero-padded) */
static void APP_voidPrintTwoDigits(u8 Copy_u8Value) {
    if (Copy_u8Value < 10) {
        LCD_voidSendData('0');
    }
    LCD_voidSendNumber(Copy_u8Value);
}

/* Drives the physical buzzer pin from BuzzerBeepCounter's state */
static void APP_voidUpdateBuzzer(void) {
    digitalWrite(BUZZ_PORT, BUZZ_PIN, (BuzzerBeepCounter > 0) ? DIO_PIN_HIGH : DIO_PIN_LOW);
}

/* Refreshes the single 7-segment display with the last-9-seconds
 * countdown of the background timer; blanked the rest of the time. */
static void APP_voidUpdateSevenSegment(void) {
    if (TimerActiveFlag && TimerMinutes == 0 && TimerSeconds >= 1 && TimerSeconds <= 9) {
        Seg_enumDisplay(SEG_PORT, TimerSeconds, SEG_TYPE);
    } else {
        digitalWritePORT(SEG_PORT, DIO_PORT_LOW);
    }
}

/* Mode Switch Button ISR (EXTI0 / PD2) */
void ISR_ModeSwitch(void) {
    CurrentMode++;
    if (CurrentMode > MODE_DATE) {
        CurrentMode = MODE_CLOCK;
    }
    LCD_voidClearScreen();
    UpdateDisplayFlag = 1; /* Flag for display redraw */
}

/* TimerSet Button ISR (EXTI1 / PD3) */
void ISR_TimerSet(void) {
    if (CurrentMode == MODE_TIMER) {
        if (!TimerActiveFlag) {
            TimerMinutes = (TimerMinutes + 1) % 60;
            TimerSeconds = 0;
            UpdateDisplayFlag = 1; /* Flag for display redraw */
        }
    } else if (CurrentMode == MODE_ALARM) {
        AlarmTime.minutes = (AlarmTime.minutes + 1) % 60;
        UpdateDisplayFlag = 1; /* Flag for display redraw */
    }
}

/* 1-Second Tick, called by the Timer1 Compare-Match-A ISR */
void APP_voidTickOneSecond(void) {
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
        BuzzerBeepCounter = BUZZER_BEEP_SECONDS;
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
                BuzzerBeepCounter = BUZZER_BEEP_SECONDS;
            }
        } else {
            TimerSeconds--;
        }
    }

    /* 4. Count down any active buzzer ring */
    if (BuzzerBeepCounter > 0) {
        BuzzerBeepCounter--;
    }

    APP_voidUpdateBuzzer();
    UpdateDisplayFlag = 1; /* Redraw the clock/timer every 1 second */
}

/* Polls the two buttons that aren't wired to an external-interrupt pin */
static void APP_voidPollTimerButtons(void) {
    static u8 Prev_u8TimerStart  = DIO_PIN_HIGH;
    static u8 Prev_u8TimerToggle = DIO_PIN_HIGH;
    u8 Local_u8Current;

    /* TimerStart */
    digitalRead(TIMERSTART_PORT, TIMERSTART_PIN, &Local_u8Current);
    if (Prev_u8TimerStart == DIO_PIN_HIGH && Local_u8Current == DIO_PIN_LOW) {
        if (CurrentMode == MODE_TIMER && (TimerMinutes > 0 || TimerSeconds > 0)) {
            TimerActiveFlag = 1;
            UpdateDisplayFlag = 1; /* Request LCD redraw */
        }
    }
    Prev_u8TimerStart = Local_u8Current;

    /* TimerToggle */
    digitalRead(TIMERTOGGLE_PORT, TIMERTOGGLE_PIN, &Local_u8Current);
    if (Prev_u8TimerToggle == DIO_PIN_HIGH && Local_u8Current == DIO_PIN_LOW) {
        if (CurrentMode == MODE_TIMER) {
            if (TimerMinutes > 0 || TimerSeconds > 0 || TimerActiveFlag) {
                TimerActiveFlag = !TimerActiveFlag;
                UpdateDisplayFlag = 1; /* Request LCD redraw */
            }
        } else if (CurrentMode == MODE_ALARM) {
            AlarmActiveFlag = !AlarmActiveFlag;
            UpdateDisplayFlag = 1; /* Request LCD redraw */
        }
    }
    Prev_u8TimerToggle = Local_u8Current;
}

/* Reads the POT (ADC0) to set the alarm hour */
static void APP_voidPollPot(void) {
    u16 Local_u16AdcValue = 0;
    static u8 Last_u8AlarmHours = 255; /* Force initial draw */

    if (CurrentMode == MODE_ALARM) {
        if (ADC_enumReadSynchronus(ADC_CHANNEL_0, &Local_u16AdcValue) == VALID) {

            /* Assuming Left Adjust is on: Local_u16AdcValue is 0-255 */
            AlarmTime.hours = (u8)(((u32)Local_u16AdcValue * 23U) / 255U);

            if (AlarmTime.hours > 23) {
                AlarmTime.hours = 23;
            }

            /* Only redraw the screen if the potentiometer moved enough to change the hour */
            if (AlarmTime.hours != Last_u8AlarmHours) {
                Last_u8AlarmHours = AlarmTime.hours;
                UpdateDisplayFlag = 1;
            }
        }
    }
}

/* Centralized HAL/MCAL + application init */
void APP_voidInit(void) {
    /* Initialize HAL and MCAL */
    LCD_voidInit();
    ADC_voidInit();
    Seg_enumInit(SEG_PORT);

    /* Buzzer output */
    pinMode(BUZZ_PORT, BUZZ_PIN, DIO_PIN_OUTPUT);
    digitalWrite(BUZZ_PORT, BUZZ_PIN, DIO_PIN_LOW);

    /* Push-buttons using internal pull-ups */
    pinMode(DIO_PORTD, DIO_PIN2, DIO_PIN_INPUT_PULLUP); /* ModeSwitch  */
    pinMode(DIO_PORTD, DIO_PIN3, DIO_PIN_INPUT_PULLUP); /* TimerSet    */
    pinMode(TIMERSTART_PORT, TIMERSTART_PIN, DIO_PIN_INPUT_PULLUP);  /* TimerStart  */
    pinMode(TIMERTOGGLE_PORT, TIMERTOGGLE_PIN, DIO_PIN_INPUT_PULLUP); /* TimerToggle */

    /* Initialize EXTI */
    EXTI_voidSetCallBack(ISR_ModeSwitch, EXTI_u8EXTI0);
    EXTI_enumEnable(EXTI_u8EXTI0, EXTI_FALLING_EDGE);

    EXTI_voidSetCallBack(ISR_TimerSet, EXTI_u8EXTI1);
    EXTI_enumEnable(EXTI_u8EXTI1, EXTI_FALLING_EDGE);

    /* Initialize Timer1 */
    TIMER1_voidInit();
    TIMER1_voidSetCallBackCompA(APP_voidTickOneSecond);
    TIMER1_voidSetCompareMatchA(TIMER1_1HZ_COMPARE);

    /* Load Custom Calendar Char to CGRAM */
    u8 calendarIcon[8] = {0x1F, 0x15, 0x1F, 0x11, 0x15, 0x11, 0x1F, 0x00};
    LCD_voidCreateCustomChar(calendarIcon, 0);
}

void APP_voidDisplayClock(void) {
    LCD_voidSetPosition(LCD_ROW_1, LCD_COL_1);
    LCD_voidSendString((const u8*)"Clock:          ");

    LCD_voidSetPosition(LCD_ROW_2, LCD_COL_1);
    APP_voidPrintTwoDigits(CurrentTime.hours);
    LCD_voidSendData(':');
    APP_voidPrintTwoDigits(CurrentTime.minutes);
    LCD_voidSendData(':');
    APP_voidPrintTwoDigits(CurrentTime.seconds);
}

void APP_voidHandleTimerMode(void) {
    LCD_voidSetPosition(LCD_ROW_1, LCD_COL_1);
    LCD_voidSendString((const u8*)(TimerActiveFlag ? "Timer (running) " : "Timer (stopped) "));

    LCD_voidSetPosition(LCD_ROW_2, LCD_COL_1);
    APP_voidPrintTwoDigits(TimerMinutes);
    LCD_voidSendData(':');
    APP_voidPrintTwoDigits(TimerSeconds);
    LCD_voidSendString((const u8*)"       "); /* Clear leftover chars */

    APP_voidUpdateSevenSegment();
}

void APP_voidHandleAlarmMode(void) {
    LCD_voidSetPosition(LCD_ROW_1, LCD_COL_1);
    LCD_voidSendString((const u8*)(AlarmActiveFlag ? "Alarm (on)      " : "Alarm (off)     "));

    LCD_voidSetPosition(LCD_ROW_2, LCD_COL_1);
    APP_voidPrintTwoDigits(AlarmTime.hours);
    LCD_voidSendData(':');
    APP_voidPrintTwoDigits(AlarmTime.minutes);
    LCD_voidSendString((const u8*)"       "); /* Clear leftover chars */
}

void APP_voidDisplayDate(void) {
    LCD_voidSetPosition(LCD_ROW_1, LCD_COL_1);
    LCD_voidSendData(0);
    LCD_voidSendData(' ');
    LCD_voidSendString((const u8*)"Date:           ");

    LCD_voidSetPosition(LCD_ROW_2, LCD_COL_1);
    APP_voidPrintTwoDigits(CurrentDate.day);
    LCD_voidSendData('/');
    APP_voidPrintTwoDigits(CurrentDate.month);
    LCD_voidSendData('/');
    LCD_voidSendNumber(CurrentDate.year);
    LCD_voidSendString((const u8*)"   ");
}

int main(void) {
    APP_voidInit();

    while (1) {
        /* Poll the buttons/POT that aren't interrupt-driven */
        APP_voidPollTimerButtons();
        APP_voidPollPot();

        /* Only push new data to the LCD if the flag was set! */
        if (UpdateDisplayFlag == 1) {
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

            /* Clear flag so we don't spam the LCD on the next loop iteration */
            UpdateDisplayFlag = 0;
        }
    }
    return 0;
}

#ifndef APP_H
#define APP_H

#include "../LIBs/STD_TYPES.h"

/* State Machine Enums */
typedef enum {
    MODE_CLOCK = 0,
    MODE_TIMER,
    MODE_ALARM,
    MODE_DATE
} ClockMode_t;

/* Time Structure */
typedef struct {
    u8 seconds;
    u8 minutes;
    u8 hours;
} Time_t;

/* Date Structure */
typedef struct {
    u8 day;
    u8 month;
    u16 year;
} Date_t;

/* Global Variables exported for ISRs and APP modules */
extern volatile ClockMode_t CurrentMode;
extern volatile Time_t CurrentTime;
extern volatile Time_t AlarmTime;
extern volatile u8 TimerMinutes;
extern volatile u8 TimerSeconds;
extern volatile u8 TimerActiveFlag;

/* Add this line right here! */
extern volatile u8 AlarmActiveFlag;

/* Seconds the buzzer keeps beeping after an Alarm/Timer trigger */
extern volatile u8 BuzzerBeepCounter;

/* Application Function Prototypes */
void APP_voidInit(void);
void APP_voidDisplayClock(void);
void APP_voidHandleTimerMode(void);
void APP_voidHandleAlarmMode(void);
void APP_voidDisplayDate(void);
void APP_voidTickOneSecond(void); /* Called by Timer ISR */

#endif /* APP_H */

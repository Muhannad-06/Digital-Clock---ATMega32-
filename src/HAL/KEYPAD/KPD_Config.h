#ifndef KPD_CONFIG_H_
#define KPD_CONFIG_H_



#define KEYPAD_PORT     DIO_PORTA

#define KEYPAD_ROW1     DIO_PIN0
#define KEYPAD_ROW2     DIO_PIN1
#define KEYPAD_ROW3     DIO_PIN2

#define KEYPAD_COL1     DIO_PIN4
#define KEYPAD_COL2     DIO_PIN5
#define KEYPAD_COL3     DIO_PIN6


#define KEYPAD_KEYS        { \
                           {'1', '2', '3'},\
                           {'4', '5', '6'},\
                           {'7', '8', '9'} \
                        }

#define NumROWs 3
#define NumCOLs 3

#endif /* KPD_CONFIG_H_ */

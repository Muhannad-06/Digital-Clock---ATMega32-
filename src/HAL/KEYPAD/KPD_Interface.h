#ifndef KPD_INTERFACE_H_
#define KPD_INTERFACE_H_

#define KEYPAD_KEYs_notPressed 0xff

void KEYPAD_voidInit(void);
ErrorCode KEYPAD_enumGetKeyState(u8* Copy_puReturnKEY);

#endif /* KPD_INTERFACE_H_ */

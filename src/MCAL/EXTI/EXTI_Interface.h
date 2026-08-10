#ifndef EXTI_INTERFACE_H_
#define EXTI_INTERFACE_H_

/* EXTI Line Numbers */
#define EXTI_u8EXTI0    0
#define EXTI_u8EXTI1    1
#define EXTI_u8EXTI2    2

/* EXTI Sense (edge/level) options */
#define EXTI_LOW_LEVEL              0
#define EXTI_ANY_LOGICAL_CHANGE     1
#define EXTI_FALLING_EDGE           2
#define EXTI_RISING_EDGE            3

ErrorCode EXTI_enumEnable(u8 Copy_u8EXTINo, u8 Copy_u8EXTISense);

ErrorCode EXTI_enumDisable(u8 Copy_u8EXTINo);

ErrorCode EXTI_enumSetSignalLatch(u8 Copy_u8EXTINo, u8 Copy_u8EXTISense);

void EXTI_voidSetCallBack(void(*Ptr_pToFunc)(void), u8 Copy_u8EXTIIndex);

#endif /* EXTI_INTERFACE_H_ */

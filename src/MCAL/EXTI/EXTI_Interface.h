#ifndef EXTI_INTERFACE_H_
#define EXTI_INTERFACE_H_

ErrorCode EXTI_enumEnable(u8 Copy_u8EXTINo, u8 Copy_u8EXTISense);

ErrorCode EXTI_enumDisable(u8 Copy_u8EXTINo);

ErrorCode EXTI_enumSetSignalLatch(u8 Copy_u8EXTINo, u8 Copy_u8EXTISense);

void EXTI_voidSetCallBack(void(*Ptr_pToFunc)(void), u8 Copy_u8EXTIIndex);

#endif /* EXTI_INTERFACE_H_ */

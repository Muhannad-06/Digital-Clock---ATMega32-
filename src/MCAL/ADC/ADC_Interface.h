/*
 *<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<   ADC_Interface.h   >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
 *
 *  Author : Muhannad Mahfouz El-Shahiedy
 *  Layer  : MCAL
 *  SWC    : ADC
 *
 */ 


#ifndef ADC_INTERFACE_H
#define ADC_INTERFACE_H

/*  ADC Prescalars */
#define ADC_PRESCALAR_by2           1
#define ADC_PRESCALAR_by4           2
#define ADC_PRESCALAR_by8           3
#define ADC_PRESCALAR_by16          4
#define ADC_PRESCALAR_by32          5
#define ADC_PRESCALAR_by64          6
#define ADC_PRESCALAR_by128         7
/*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/

/*  ADC Always PORT A */
#define ADC_CHANNEL_0               DIO_PIN0
#define ADC_CHANNEL_1               DIO_PIN1
#define ADC_CHANNEL_2               DIO_PIN2
#define ADC_CHANNEL_3               DIO_PIN3
#define ADC_CHANNEL_4               DIO_PIN4
#define ADC_CHANNEL_5               DIO_PIN5
#define ADC_CHANNEL_6               DIO_PIN6
#define ADC_CHANNEL_7               DIO_PIN7
/*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/

/*  ADC Trigger Sources */
#define ADC_SINGLE_CONVERSION   0
#define ADC_FREE_RUNNING        1
#define ADC_ANALOG_COMPARATOR   2
#define ADC_EXTI0               3
#define ADC_TIM0_CTC            4
#define ADC_TIM0_OVF            5
#define ADC_TIM1_CTC_CHANNEL_B  6
#define ADC_TIM1_OVF            7
#define ADC_TIM1_ICU            8
/*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/

/* ADC Adjustment */
#define ADC_LEFT        0
#define ADC_RIGHT       1
/*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/

/* ADC Reference Voltage */
#define ADC_AREF                  0
#define ADC_AVCC                  1
#define ADC_INTERNAL              2
/*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/

/* ADC Interrupt Status */
#define INT_ENABLE    1
#define INT_DISABLE   0
/*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/

void ADC_voidInit();

/* THIS FUNV READS A SPECIFIC ADC CHANNEL*/
ErrorCode ADC_enumReadSynchronus(u8 Copy_u8ChannelNum, u16 *pu16Result);

/*  This FUNC READS A SPECIFIC ADC CHANNEL
    Copy_u8ChannelNum: 0 ----> 7
    *pu16Result: Pointer to store the conversion result
    *pvCallBackFuncPtr: Address of the Function to be Executed */
ErrorCode ADC_enumReadAsynchronus(u8 Copy_u8ChannelNum, u16 *pu16Result, void (*pvCallBackFuncPtr)(void));

#endif /*ADC_INTERFACE_H*/

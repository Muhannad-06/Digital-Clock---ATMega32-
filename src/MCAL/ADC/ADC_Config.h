/*
 *<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<   ADC_Config.h   >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
 *
 *  Author : Muhannad Mahfouz El-Shahiedy
 *  Layer  : MCAL
 *  SWC    : ADC
 *
 */ 


#ifndef ADC_CONFIG_H
#define ADC_CONFIG_H

/*  Options: 
            ADC_AREF        AREF Pin
            ADC_AVCC        AVCC Pin
            ADC_INTERNAL   2.56 Ref Voltage
*/
#define ADC_REFVOLT         ADC_AVCC

/*  Options:
            ADC_RIGHT       Right Adjusted
            ADC_LEFT        Left Adjusted
*/
#define ADC_ADJUST_CTRL     ADC_LEFT  

/*
 Options:-
 	 1-ADC_CHANNEL_0
 	 2-ADC_CHANNEL_1
 	 3-ADC_CHANNEL_2
 	 4-ADC_CHANNEL_3
 	 5-ADC_CHANNEL_4
 	 6-ADC_CHANNEL_5
 	 7-ADC_CHANNEL_6
 	 8-ADC_CHANNEL_7


 */
#define ADC_CHANNEL  ADC_CHANNEL_0


/*  Options:
            ADC_PRESCALAR_by2
            ADC_PRESCALAR_by4
            ADC_PRESCALAR_by8
            ADC_PRESCALAR_by16
            ADC_PRESCALAR_by32
            ADC_PRESCALAR_by64
            ADC_PRESCALAR_by128
*/
#define ADC_PRESCALAR   ADC_PRESCALAR_by128

/*  Options:
            ADC_SINGLE_CONVERSION
            ADC_FREE_RUNNING
            ADC_ANALOG_COMPARATOR
            ADC_EXTERNAL_INT0
            ADC_TIMER0_COMPARE_MATCH
            ADC_TIMER0_OVERFLOW
            ADC_TIMER1_COMPARE_MATCH_B
            ADC_TIMER1_OVERFLOW
            ADC_TIMER1_CAPTURE_EVENT
*/
#define ADC_TRIGGER_SOURCE   ADC_FREE_RUNNING

/*
 Options:-
 	 1-INT_ENABLE
 	 2-INT_DISABLE

 */
#define ADC_INTERRUPT   INT_DISABLE




#define  ADC_TIMEOUT      50000



#endif /*ADC_CONFIG_H*/
/*
 *<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<   ADC_Private.h   >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
 *
 *  Author : Muhannad Mahfouz El-Shahiedy
 *  Layer  : MCAL
 *  SWC    : ADC
 *
 */ 


#ifndef ADC_PRIVATE_H
#define ADC_PRIVATE_H

/*************************** ADMUX Register **************************************/
#define ADC_ADMUX_REG         *((volatile u8*) 0x27)

#define ADC_ADMUX_ADLAR       5
#define ADC_ADMUX_REFS0       6
#define ADC_ADMUX_REFS1       7

/*************************** ADCSRA Register ************************************/
#define ADC_ADCSRA_REG        *((volatile u8*) 0x26)

#define ADC_ADCSRA_ADPS0      0
#define ADC_ADCSRA_ADPS1      1
#define ADC_ADCSRA_ADPS2      2
#define ADC_ADCSRA_ADIE       3
#define ADC_ADCSRA_ADIF       4
#define ADC_ADCSRA_ADATE      5
#define ADC_ADCSRA_ADSC       6
#define ADC_ADCSRA_ADEN       7

/*************************** ADC Data Registers *********************************/
#define ADC_ADCH_REG          *((volatile u8*) 0x25)
#define ADC_ADCL_REG          *((volatile u8*) 0x24)
#define ADC_REG               *((volatile u16*) 0x24)

/*************************** SFIOR Register *************************************/
#define ADC_SFIOR_REG         *((volatile u8*) 0x50)

#define ADC_SFIOR_ADTS0       5
#define ADC_SFIOR_ADTS1       6
#define ADC_SFIOR_ADTS2       7

/* ADC Utility Masks */
#define ADC_PRES_MASK         0b11111000
#define ADC_CHANNEL_MASK      0b11100000

#define NOTBUSY               0
#define BUSY                  1

/* Global Interrupt Enable Register (SREG) */
#define INTERRUPT_SREG_REG  			 *((volatile u8*) 0x5F)
#define INTERRUPT_GIE_BIT   		        	7

#endif /*ADC_PRIVATE_H*/
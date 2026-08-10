#ifndef EXTI_PRIVATE_H_
#define EXTI_PRIVATE_H_


/* Private Macros */
/* NOTE: EXTI line numbers (EXTI_u8EXTI0..2) and sense options
 * (EXTI_LOW_LEVEL..EXTI_RISING_EDGE) are public API and now live in
 * EXTI_Interface.h so the APP layer can use them without including
 * this private header. */

/********************************************************** */
/* Address Of Register MCUCR */
#define EXTI_MCUCR_REG      *((volatile u8*) 0x55)

/* MCUCR PINs Macros */
#define EXTI_MCUCR_ISC00    0
#define EXTI_MCUCR_ISC01    1
#define EXTI_MCUCR_ISC10    2
#define EXTI_MCUCR_ISC11    3
/*---------------------------------------------------------*/


/********************************************************* */
/* Address Of Register MCUCSR */
#define EXTI_MCUCSR_REG      *((volatile u8*) 0x54)

/* MCUCSR PINs Macros */
#define EXTI_MCUCSR_ISC2    6
/*---------------------------------------------------------*/


/********************************************************* */
/* Address Of Register GICR */
#define EXTI_GICR_REG        *((volatile u8*) 0x5B)

/* GICR PINs Macros */
#define EXTI_GICR_INT2      5
#define EXTI_GICR_INT0      6
#define EXTI_GICR_INT1      7
/*---------------------------------------------------------*/


/********************************************************* */
/* Address Of Register GIFR */
#define EXTI_GIFR_REG        *((volatile u8*) 0x5A)

/*---------------------------------------------------------*/


/***********************************************************/
/* Global Interrupt Enable Register (SREG) */
#define INTERRUPT_SREG_REG  			 *((volatile u8*) 0x5F)
#define INTERRUPT_GIE_BIT   			7





#endif /* EXTI_PRIVATE_H_ */

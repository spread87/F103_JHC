/*
 * peri_NVIC.h
 *
 * Created on: 2014-10-27
 * Author: LvSL
 */

#ifndef PERI_NVIC_H_
#define PERI_NVIC_H_

#define  	PwmTrap_Isr					CCU80_3_IRQHandler
//#define  	ZPhase_Isr					CCU40_0_IRQHandler
//#define  	VelocityCalc_Isr			CCU40_2_IRQHandler
//#define  	EncoderError_Isr			POSIF0_0_IRQHandler
#define  	PulseInputPeriod_Isr		CCU41_0_IRQHandler
#define	    PulseOutputPeriodA_Isr		CCU42_0_IRQHandler
#define     PulseOutputPeriodB_Isr      CCU42_1_IRQHandler
#define     Zphase_ISR                  CCU40_1_IRQHandler
#define     Zphase_ISR1                 CCU40_3_IRQHandler

extern void NVICInit(void);

#endif /* PERI_NVIC_H_ */

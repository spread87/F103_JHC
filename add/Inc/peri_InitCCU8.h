/*
 * peri_InitCCU8.h
 *
 *  Created on: 2014-10-27
 *      Author: LvSL
 */

#ifndef PERI_INITCCU8_H_
#define PERI_INITCCU8_H_

extern void CCU8Init(void);

extern void CCU8GetPeriod(uint8_t );

extern void PwmOutputEnable(void);

extern void PwmOutputDisable(void);

extern uint8_t ResetTrapIsr(void);

extern void PwmIOInit(void);

#endif /* PERI_INITCCU8_H_ */

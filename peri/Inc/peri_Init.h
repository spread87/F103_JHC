/*
 * peri_Init.h
 *
 *  Created on: 2014-2-19
 *      Author: JH-LvSL
 */

#ifndef PERI_INIT_H_
#define PERI_INIT_H_

extern void InitModule(void);
extern void PanelInit(void);
extern void InitScanDiSwitch(void);
extern void VelocityFilInit(void);
extern void Encoder_Lineless(void);
extern void FuncCodeF6015Init(void);
extern void PIParaInit(void);
extern void InitAdcFilter(void);
extern void MotorInitAngleCalc(void);
extern void MotorParaRefresh(void);
extern void LimitPISet(void);
extern void CurrentPIKpCalc(void);
extern void CurrentPIKiCalc(void);
extern void PanelInitDisplay(void);
extern void PIDInit(void);
extern void AOInitValue(void);
extern void PosiPLCInit(void);
extern void AlmRcdRead(void);
extern void AlmRcdClr(void);
extern void UARTInit(void);
extern void CANInit(void);//20170418 CYL

#endif /* PERI_INIT_H_ */

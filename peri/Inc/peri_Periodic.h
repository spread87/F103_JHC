/*
 * peri_Periodic.h
 *
 *  Created on: 2014-2-24
 *      Author: JH-LvSL
 */

#ifndef PERI_PERIODIC_H_
#define PERI_PERIODIC_H_

typedef void (*PERIODIC_FUNC)(void);

/*System Main Loop*/
extern void SysMainLoop(void);
extern void LPFFilterTimeCalc(void);
extern void TorqueLimitStepCalc(void);
extern void CtrlModeSelect(void);
extern void AlmClr(void);
extern void PosiDeviationClr(void);
extern void ElecGearDivisorCalc(void);
extern void TerminalAIHandle(void);
extern void TerminalAORatedCalc(void);
extern void FanCtrl(void);
extern void RelayCtrl(void);
extern void PulseOutDenominatorCalc(void);
extern void FuncCodeF6015Handle(void);
extern void VeloInputRatedCalc(void);
extern void PulInSrcSelect(void);
extern void ProtectPUValueCalc(void);
extern void AlmRcdDisplay(void);
extern void AlmRcdRefresh(void);
extern void AlmRcdStorage(void);
extern void TerminalIOSts(void);
extern void MonitorParaHandle(void);

extern void PeriPidHandle(void);
extern void PeriPidPara(void);
extern void TerminalDOSetHandle(void);
extern void DcbusPUValueCalc(void);


/*100us timer treat*/
extern void SysCore100usLoop(void);
extern void EncCalc(void);
extern void PmsmFOCHandle(void);
extern void PulInSrcHandle(void);
extern void AdcReadResult(void);
extern void CurInSrcHandle(void);
extern void VeloInputSrcHandle(void);
extern void PulseOutHandle(void);
extern void AdcFilter100usHandle(void);
extern void AlmCheckOC(void);
extern void AlmCheckOV(void);

/*1ms timer treat*/
extern void SysCore1msLoop(void);
extern void TimerControlThread(void);
extern void PanelLEDButtonHandle(void);
extern void TorqueLimitCalc(void);
extern void PIParaCalc(void);
extern void FuncDIHandle(void);
extern void TerminalDOHandle(void);
extern void TerminalAOHandle(void);
extern void AdcFilter1msHandle(void);
extern void CanHandle(void);
extern int32_t VeloCMDCalc(int32_t,int32_t);
extern void VeloStepCalc(void);
extern void CurInVeloCalc(void);
extern void VeloCtrlTorqueLimit(void);
extern void TorqueLimit_Output(void);
extern void TimeSequProcess(void);
extern void Dcbus_Break(void);
extern void IrmsCalc(void);
extern void VrmsCalc(void);
extern void PrmsCalc(void);
extern void TempCalc(void);
extern void UART_Test_Handle(void);
extern void ModbusHandle(void);

extern void GetVelocity(void);//CYL
//extern void EncVelocityCalc(void);//

/*2ms timer treat*/
extern void AlmCheck2ms(void);
extern void AdcCalibration(void);
extern void TerminalAIProcess(void);
extern void AOPosiCMDSpeedCalc(void);
extern void UART_Test_Handle(void);

/*10ms timer treat*/
extern void AlmCheck10ms(void);
extern void RunTimeStatistics(void);

/*100ms timer treat*/
extern void AlmCheck100ms(void);

/*500ms timer treat*/
extern void AlmCheck500ms(void);

extern void AlmCheckEnc(void);

#endif /* PERI_PERIODIC_H_ */

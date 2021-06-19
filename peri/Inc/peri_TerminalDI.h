/*
 * peri_TerminalDI.h
 *
 *  Created on: 2014-5-23
 *      Author: JH-LvSL
 */

#ifndef PERI_TERMINALDI_H_
#define PERI_TERMINALDI_H_

typedef void (*TERMINALDI_FUNC)(void);

#define	NumDI1		(uint8_t)1
#define	NumDI2		(uint8_t)2
#define	NumDI3		(uint8_t)3
#define	NumDI4		(uint8_t)4

extern SW_SCAN_FUN DI1;
extern SW_SCAN_FUN DI2;
extern SW_SCAN_FUN DI3;
extern SW_SCAN_FUN DI4;

extern DICtrl	DICtrlFlg1;
extern DICtrl	DICtrlFlg2;
extern DICtrl	DICtrlFlg3;
extern DICtrl	DICtrlFlg4;

extern  void TerminalDIHandle(void);
extern  void TerminalDI1Handle(void);
extern  void TerminalDI2Handle(void);
extern  void TerminalDI3Handle(void);
extern  void TerminalDI4Handle(void);

extern void TerminalDIServoOn(uint8_t DIPortInfo,uint8_t TerminalNum);
extern void TerminalDIPOT(uint8_t DIPortInfo,uint8_t TerminalNum);
extern void TerminalDINOT(uint8_t DIPortInfo,uint8_t TerminalNum);
extern void TerminalDIAlmClr(uint8_t DIPortInfo,uint8_t TerminalNum);
extern void TerminalDICtrlMode(uint8_t DIPortInfo,uint8_t TerminalNum);
extern void TerminalDIGain(uint8_t DIPortInfo,uint8_t TerminalNum);
extern void TerminalDICL(uint8_t DIPortInfo,uint8_t TerminalNum);
extern void TerminalDIINH(uint8_t DIPortInfo,uint8_t TerminalNum);
extern void TerminalDITLS(uint8_t DIPortInfo,uint8_t TerminalNum);
extern void TerminalDIZeroSPD(uint8_t DIPortInfo,uint8_t TerminalNum);
extern void TerminalDIEStop(uint8_t DIPortInfo,uint8_t TerminalNum);
extern void TerminalDIDIV0(uint8_t DIPortInfo,uint8_t TerminalNum);
extern void TerminalDIDIV1(uint8_t DIPortInfo,uint8_t TerminalNum);
extern void TerminalDIINTSPD0(uint8_t DIPortInfo,uint8_t TerminalNum);
extern void TerminalDIINTSPD1(uint8_t DIPortInfo,uint8_t TerminalNum);
extern void TerminalDIINTSPD2(uint8_t DIPortInfo,uint8_t TerminalNum);
extern void TerminalDIINTSPD3(uint8_t DIPortInfo,uint8_t TerminalNum);
extern void TerminalDIINTPOS0(uint8_t DIPortInfo,uint8_t TerminalNum);
extern void TerminalDIINTPOS1(uint8_t DIPortInfo,uint8_t TerminalNum);
extern void TerminalDIINTPOS2(uint8_t DIPortInfo,uint8_t TerminalNum);
extern void TerminalDIINTPOS3(uint8_t DIPortInfo,uint8_t TerminalNum);
extern void TerminalDISOR(uint8_t DIPortInfo,uint8_t TerminalNum);
extern void TerminalDISORPOS(uint8_t DIPortInfo,uint8_t TerminalNum);
extern void TerminalDIPJOG(uint8_t DIPortInfo,uint8_t TerminalNum);
extern void TerminalDINJOG(uint8_t DIPortInfo,uint8_t TerminalNum);
extern void TerminalDIMTRDIR(uint8_t DIPortInfo,uint8_t TerminalNum);

extern void TerminalDIPLICH1(uint8_t DIPortInfo,uint8_t TerminalNum);
extern void TerminalDIPLICH2(uint8_t DIPortInfo,uint8_t TerminalNum);
extern void TerminalDIPLICH3(uint8_t DIPortInfo,uint8_t TerminalNum);
extern void TerminalPIHLSWITCH(uint8_t DIPortInfo,uint8_t TerminalNum);

extern void TerminalDIINER1(uint8_t DIPortInfo,uint8_t TerminalNum);
extern void TerminalDIINER2(uint8_t DIPortInfo,uint8_t TerminalNum);
extern void TerminalDIINER3(uint8_t DIPortInfo,uint8_t TerminalNum);
extern void TerminalDIECAMEN(uint8_t DIPortInfo,uint8_t TerminalNum);
extern void TerminalDIECAMSEL0(uint8_t DIPortInfo,uint8_t TerminalNum);
extern void TerminalDIECAMSEL1(uint8_t DIPortInfo,uint8_t TerminalNum);
extern void TerminalDIECAMSEL2(uint8_t DIPortInfo,uint8_t TerminalNum);
extern void TerminalDIECUTFISH(uint8_t DIPortInfo,uint8_t TerminalNum);
extern void TerminalDIECAMRSVD35(uint8_t DIPortInfo,uint8_t TerminalNum);
extern void TerminalDIECAMRSVD36(uint8_t DIPortInfo,uint8_t TerminalNum);
extern void TerminalDIECAMRSVD37(uint8_t DIPortInfo,uint8_t TerminalNum);
extern void TerminalDIECAMRSVD38(uint8_t DIPortInfo,uint8_t TerminalNum);
extern void TerminalDIECAMRSVD39(uint8_t DIPortInfo,uint8_t TerminalNum);
extern void TerminalDIECAMRSVD40(uint8_t DIPortInfo,uint8_t TerminalNum);

extern void TerminalFuncServoOn(void);
extern void TerminalFuncPOT(void);
extern void TerminalFuncNOT(void);
extern void TerminalFuncAlmClr(void);
extern void TerminalFuncCtrlMode(void);
extern void TerminalFuncGain(void);
extern void TerminalFuncCL(void);
extern void TerminalFuncINH(void);
extern void TerminalFuncTLS(void);
extern void TerminalFuncZeroSPD(void);
extern void TerminalFuncEStop(void);
extern void TerminalFuncDIV0(void);
extern void TerminalFuncDIV1(void);
extern void TerminalFuncINTSPD0(void);
extern void TerminalFuncINTSPD1(void);
extern void TerminalFuncINTSPD2(void);
extern void TerminalFuncINTSPD3(void);
extern void TerminalFuncINTPOS0(void);
extern void TerminalFuncINTPOS1(void);
extern void TerminalFuncINTPOS2(void);
extern void TerminalFuncINTPOS3(void);
extern void TerminalFuncSOR(void);
extern void TerminalFuncSORPOS(void);
extern void TerminalFuncPJOG(void);
extern void TerminalFuncNJOG(void);
extern void TerminalFuncMTRDIR(void);

extern void TerminalFuncPLICH1(void);
extern void TerminalFuncPLICH2(void);
extern void TerminalFuncPLICH3(void);
extern void TerminalFuncPIHLSWITCH(void);

extern void TerminalFuncINER1(void);
extern void TerminalFuncINER2(void);
extern void TerminalFuncINER3(void);
extern void TerminalFuncECAMEN(void);
extern void TerminalFuncECAMSEL0(void);
extern void TerminalFuncECAMSEL1(void);
extern void TerminalFuncECAMSEL2(void);
extern void TerminalFuncECUTFISH(void);
extern void TerminalFuncECAMRSVD35(void);
extern void TerminalFuncECAMRSVD36(void);
extern void TerminalFuncECAMRSVD37(void);
extern void TerminalFuncECAMRSVD38(void);
extern void TerminalFuncECAMRSVD39(void);
extern void TerminalFuncECAMRSVD40(void);


#endif /* PERI_TERMINALDI_H_ */

/*
 * peri_TerminalDIFunc.c
 *
 * Created on: 2014-5-23
 * Author: JH-LvSL
 */
#include <main.h>
#include <peri_GlobalVariablesExtern.h>

DICtrl DICtrlFlg1; //9个输入端子
DICtrl DICtrlFlg2;
DICtrl DICtrlFlg3;
DICtrl DICtrlFlg4;

static DICtrl *DITable[] =
{
  &DICtrlFlg1,
  &DICtrlFlg2,
  &DICtrlFlg3,
  &DICtrlFlg4,
};

/*************************************************
 Function: Terminal DI: Servo On  Handle
 Description:Period:1ms
 Input: No
 Return: No
 Others: Be Called TerminalDIFuncHandle[]
 *************************************************/
void TerminalDIServoOn(uint8_t DIPortInfo, uint8_t TerminalNum)
{
  DICtrl *DITemp;
  DITemp = DITable[TerminalNum - 1];
//	DITemp->all = 0;
  if (DIPortInfo)
    DITemp->bit.ServoOn = OK;
  else
    DITemp->bit.ServoOn = NG;
}
/*************************************************
 Function: Terminal DI: Positive direction over-travel inhibition input
 Description:Period:1ms
 Input: No
 Return: No
 Others: Be Called TerminalDIFuncHandle[]
 *************************************************/
void TerminalDIPOT(uint8_t DIPortInfo, uint8_t TerminalNum)
{
  DICtrl *DITemp;
  DITemp = DITable[TerminalNum - 1];
  if (DIPortInfo)
    DITemp->bit.POT = OK;
  else
    DITemp->bit.POT = NG;
}
/*************************************************
 Function: Terminal DI: Negative direction over-travel inhibition input
 Description:Period:1ms
 Input: No
 Return: No
 Others: Be Called TerminalDIFuncHandle[]
 *************************************************/
void TerminalDINOT(uint8_t DIPortInfo, uint8_t TerminalNum)
{
  DICtrl *DITemp;
  DITemp = DITable[TerminalNum - 1];
  if (DIPortInfo)
    DITemp->bit.NOT = OK;
  else
    DITemp->bit.NOT = NG;
}
/*************************************************
 Function: Terminal DI: Alarm clear input
 Description:Period:1ms
 Input: No
 Return: No
 Others: Be Called TerminalDIFuncHandle[]
 *************************************************/
void TerminalDIAlmClr(uint8_t DIPortInfo, uint8_t TerminalNum)
{
  DICtrl *DITemp;
  DITemp = DITable[TerminalNum - 1];
  if (DIPortInfo)
    DITemp->bit.AlmClr = OK;
  else
    DITemp->bit.AlmClr = NG;
}
/*************************************************
 Function: Terminal DI: Control mode switching input
 Description:Period:1ms
 Input: No
 Return: No
 Others: Be Called TerminalDIFuncHandle[]
 *************************************************/
void TerminalDICtrlMode(uint8_t DIPortInfo, uint8_t TerminalNum)
{
  DICtrl *DITemp;
  DITemp = DITable[TerminalNum - 1];
  if (DIPortInfo)
    DITemp->bit.CtrlMode = OK;
  else
    DITemp->bit.CtrlMode = NG;
}
/*************************************************
 Function: Terminal DI: Gain switching input
 Description:Period:1ms
 Input: No
 Return: No
 Others: Be Called TerminalDIFuncHandle[]
 *************************************************/
void TerminalDIGain(uint8_t DIPortInfo, uint8_t TerminalNum)
{
  DICtrl *DITemp;
  DITemp = DITable[TerminalNum - 1];
  if (DIPortInfo)
    DITemp->bit.Gain = OK;
  else
    DITemp->bit.Gain = NG;
}
/*************************************************
 Function: Terminal DI: Deviation counter clear input
 Description:Period:1ms
 Input: No
 Return: No
 Others: Be Called TerminalDIFuncHandle[]
 *************************************************/
void TerminalDICL(uint8_t DIPortInfo, uint8_t TerminalNum)
{
  DICtrl *DITemp;
  DITemp = DITable[TerminalNum - 1];
  if (DIPortInfo)
    DITemp->bit.CL = OK;
  else
    DITemp->bit.CL = NG;
}
/*************************************************
 Function: Terminal DI: Command pulse inhibition input
 Description:Period:1ms
 Input: No
 Return: No
 Others: Be Called TerminalDIFuncHandle[]
 *************************************************/
void TerminalDIINH(uint8_t DIPortInfo, uint8_t TerminalNum)
{
  DICtrl *DITemp;
  DITemp = DITable[TerminalNum - 1];
  if (DIPortInfo)
    DITemp->bit.INH = OK;
  else
    DITemp->bit.INH = NG;
}
/*************************************************
 Function: Terminal DI: Torque limit switching input
 Description:Period:1ms
 Input: No
 Return: No
 Others: Be Called TerminalDIFuncHandle[]
 *************************************************/
void TerminalDITLS(uint8_t DIPortInfo, uint8_t TerminalNum)
{
  DICtrl *DITemp;
  DITemp = DITable[TerminalNum - 1];
  if (DIPortInfo)
    DITemp->bit.TLS = OK;
  else
    DITemp->bit.TLS = NG;
}
/*************************************************
 Function: Terminal DI: Speed zero clamp input
 Description:Period:1ms
 Input: No
 Return: No
 Others: Be Called TerminalDIFuncHandle[]
 *************************************************/
void TerminalDIZeroSPD(uint8_t DIPortInfo, uint8_t TerminalNum)
{
  DICtrl *DITemp;
  DITemp = DITable[TerminalNum - 1];
  if (DIPortInfo)
    DITemp->bit.ZeroSPD = OK;
  else
    DITemp->bit.ZeroSPD = NG;
}
/*************************************************
 Function: Terminal DI: Emergency stop
 Description:Period:1ms
 Input: No
 Return: No
 Others: Be Called TerminalDIFuncHandle[]
 *************************************************/
void TerminalDIEStop(uint8_t DIPortInfo, uint8_t TerminalNum)
{
  DICtrl *DITemp;
  DITemp = DITable[TerminalNum - 1];
  if (DIPortInfo)
    DITemp->bit.EStop = OK;
  else
    DITemp->bit.EStop = NG;
}
/*************************************************
 Function: Terminal DI: Electronic gear(division/multiplication) switching input0
 Description:Period:1ms
 Input: No
 Return: No
 Others: Be Called TerminalDIFuncHandle[]
 *************************************************/
void TerminalDIDIV0(uint8_t DIPortInfo, uint8_t TerminalNum)
{
  DICtrl *DITemp;
  DITemp = DITable[TerminalNum - 1];
  if (DIPortInfo)
    DITemp->bit.DIV0 = OK;
  else
    DITemp->bit.DIV0 = NG;
}
/*************************************************
 Function: Terminal DI:Electronic gear(division/multiplication) switching input1
 Description:Period:1ms
 Input: No
 Return: No
 Others: Be Called TerminalDIFuncHandle[]
 *************************************************/
void TerminalDIDIV1(uint8_t DIPortInfo, uint8_t TerminalNum)
{
  DICtrl *DITemp;
  DITemp = DITable[TerminalNum - 1];
  if (DIPortInfo)
    DITemp->bit.DIV1 = OK;
  else
    DITemp->bit.DIV1 = NG;
}
/*************************************************
 Function: Terminal DI:Selection input of internal command speed0
 Description:Period:1ms
 Input: No
 Return: No
 Others: Be Called TerminalDIFuncHandle[]
 *************************************************/
void TerminalDIINTSPD0(uint8_t DIPortInfo, uint8_t TerminalNum)
{
  DICtrl *DITemp;
  DITemp = DITable[TerminalNum - 1];
  if (DIPortInfo)
    DITemp->bit.INTSPD0 = OK;
  else
    DITemp->bit.INTSPD0 = NG;
}
/*************************************************
 Function: Terminal DI:Selection input of internal command speed1
 Description:Period:1ms
 Input: No
 Return: No
 Others: Be Called TerminalDIFuncHandle[]
 *************************************************/
void TerminalDIINTSPD1(uint8_t DIPortInfo, uint8_t TerminalNum)
{
  DICtrl *DITemp;
  DITemp = DITable[TerminalNum - 1];
  if (DIPortInfo)
    DITemp->bit.INTSPD1 = OK;
  else
    DITemp->bit.INTSPD1 = NG;
}
/*************************************************
 Function: Terminal DI:Selection input of internal command speed2
 Description:Period:1ms
 Input: No
 Return: No
 Others: Be Called TerminalDIFuncHandle[]
 *************************************************/
void TerminalDIINTSPD2(uint8_t DIPortInfo, uint8_t TerminalNum)
{
  DICtrl *DITemp;
  DITemp = DITable[TerminalNum - 1];
  if (DIPortInfo)
    DITemp->bit.INTSPD2 = OK;
  else
    DITemp->bit.INTSPD2 = NG;
}
/*************************************************
 Function: Terminal DI:Selection input of internal command speed3
 Description:Period:1ms
 Input: No
 Return: No
 Others: Be Called TerminalDIFuncHandle[]
 *************************************************/
void TerminalDIINTSPD3(uint8_t DIPortInfo, uint8_t TerminalNum)
{
  DICtrl *DITemp;
  DITemp = DITable[TerminalNum - 1];
  if (DIPortInfo)
    DITemp->bit.INTSPD3 = OK;
  else
    DITemp->bit.INTSPD3 = NG;
}
/*************************************************
 Function: Terminal DI:Selection input of internal command position0
 Description:Period:1ms
 Input: No
 Return: No
 Others: Be Called TerminalDIFuncHandle[]
 *************************************************/
void TerminalDIINTPOS0(uint8_t DIPortInfo, uint8_t TerminalNum)
{
  DICtrl *DITemp;
  DITemp = DITable[TerminalNum - 1];
  if (DIPortInfo)
    DITemp->bit.INTPOS0 = OK;
  else
    DITemp->bit.INTPOS0 = NG;
}
/*************************************************
 Function: Terminal DI:Selection input of internal command position1
 Description:Period:1ms
 Input: No
 Return: No
 Others: Be Called TerminalDIFuncHandle[]
 *************************************************/
void TerminalDIINTPOS1(uint8_t DIPortInfo, uint8_t TerminalNum)
{
  DICtrl *DITemp;
  DITemp = DITable[TerminalNum - 1];
  if (DIPortInfo)
    DITemp->bit.INTPOS1 = OK;
  else
    DITemp->bit.INTPOS1 = NG;
}
/*************************************************
 Function: Terminal DI:Selection input of internal command position2
 Description:Period:1ms
 Input: No
 Return: No
 Others: Be Called TerminalDIFuncHandle[]
 *************************************************/
void TerminalDIINTPOS2(uint8_t DIPortInfo, uint8_t TerminalNum)
{
  DICtrl *DITemp;
  DITemp = DITable[TerminalNum - 1];
  if (DIPortInfo)
    DITemp->bit.INTPOS2 = OK;
  else
    DITemp->bit.INTPOS2 = NG;
}
/*************************************************
 Function: Terminal DI:Selection input of internal command position3
 Description:Period:1ms
 Input: No
 Return: No
 Others: Be Called TerminalDIFuncHandle[]
 *************************************************/
void TerminalDIINTPOS3(uint8_t DIPortInfo, uint8_t TerminalNum)
{
  DICtrl *DITemp;
  DITemp = DITable[TerminalNum - 1];
  if (DIPortInfo)
    DITemp->bit.INTPOS3 = OK;
  else
    DITemp->bit.INTPOS3 = NG;
}
/*************************************************
 Function: Terminal DI:Start Origin homing mode
 Description:Period:1ms
 Input: No
 Return: No
 Others: Be Called TerminalDIFuncHandle[]
 *************************************************/
void TerminalDISOR(uint8_t DIPortInfo, uint8_t TerminalNum)
{
  DICtrl *DITemp;
  DITemp = DITable[TerminalNum - 1];
  if (DIPortInfo)
    DITemp->bit.SOR = OK;
  else
    DITemp->bit.SOR = NG;
}
/*************************************************
 Function: Terminal DI:Ensure Origin homing mode position
 Description:Period:1ms
 Input: No
 Return: No
 Others: Be Called TerminalDIFuncHandle[]
 *************************************************/
void TerminalDISORPOS(uint8_t DIPortInfo, uint8_t TerminalNum)
{
  DICtrl *DITemp;
  DITemp = DITable[TerminalNum - 1];
  if (DIPortInfo)
    DITemp->bit.SORPOS = OK;
  else
    DITemp->bit.SORPOS = NG;
}
/*************************************************
 Function: Terminal DI:Positive JOG
 Description:Period:1ms
 Input: No
 Return: No
 Others: Be Called TerminalDIFuncHandle[]
 *************************************************/
void TerminalDIPJOG(uint8_t DIPortInfo, uint8_t TerminalNum)
{
  DICtrl *DITemp;
  DITemp = DITable[TerminalNum - 1];
  if (DIPortInfo)
    DITemp->bit.PJOG = OK;
  else
    DITemp->bit.PJOG = NG;
}
/*************************************************
 Function: Terminal DI:Negative JOG
 Description:Period:1ms
 Input: No
 Return: No
 Others: Be Called TerminalDIFuncHandle[]
 *************************************************/
void TerminalDINJOG(uint8_t DIPortInfo, uint8_t TerminalNum)
{
  DICtrl *DITemp;
  DITemp = DITable[TerminalNum - 1];
  if (DIPortInfo)
    DITemp->bit.NJOG = OK;
  else
    DITemp->bit.NJOG = NG;
}
/*************************************************
 Function: Terminal DI: Motor rotation direction
 Description:Period:1ms
 Input: No
 Return: No
 Others: Be Called TerminalDIFuncHandle[]
 *************************************************/
void TerminalDIMTRDIR(uint8_t DIPortInfo, uint8_t TerminalNum)
{
  DICtrl *DITemp;
  DITemp = DITable[TerminalNum - 1];
  if (DIPortInfo)
    DITemp->bit.MTRDIR = OK;
  else
    DITemp->bit.MTRDIR = NG;
}

/*************************************************
 Function: Terminal DI: Motor rotation direction
 Description:Period:1ms
 Input: No
 Return: No
 Others: Be Called TerminalDIFuncHandle[]
 *************************************************/
void TerminalDIINER1(uint8_t DIPortInfo, uint8_t TerminalNum)
{
  DICtrl *DITemp;
  DITemp = DITable[TerminalNum - 1];
  if (DIPortInfo)
    DITemp->bit.INERSWH1 = OK;
  else
    DITemp->bit.INERSWH1 = NG;
}

void TerminalDIINER2(uint8_t DIPortInfo, uint8_t TerminalNum)
{
  DICtrl *DITemp;
  DITemp = DITable[TerminalNum - 1];
  if (DIPortInfo)
    DITemp->bit.INERSWH2 = OK;
  else
    DITemp->bit.INERSWH2 = NG;
}

void TerminalDIINER3(uint8_t DIPortInfo, uint8_t TerminalNum)
{
  DICtrl *DITemp;
  DITemp = DITable[TerminalNum - 1];
  if (DIPortInfo)
    DITemp->bit.INERSWH3 = OK;
  else
    DITemp->bit.INERSWH3 = NG;
}

void TerminalDIECAMEN(uint8_t DIPortInfo, uint8_t TerminalNum)
{
}

void TerminalDIECAMSEL0(uint8_t DIPortInfo, uint8_t TerminalNum)
{
}

void TerminalDIECAMSEL1(uint8_t DIPortInfo, uint8_t TerminalNum)
{
}

void TerminalDIECAMSEL2(uint8_t DIPortInfo, uint8_t TerminalNum)
{
}

void TerminalDIECUTFISH(uint8_t DIPortInfo, uint8_t TerminalNum)
{
}

void TerminalDIECAMRSVD35(uint8_t DIPortInfo, uint8_t TerminalNum)
{
}
void TerminalDIECAMRSVD36(uint8_t DIPortInfo, uint8_t TerminalNum)
{
}

void TerminalDIECAMRSVD37(uint8_t DIPortInfo, uint8_t TerminalNum)
{
}

void TerminalDIECAMRSVD38(uint8_t DIPortInfo, uint8_t TerminalNum)
{
}

void TerminalDIECAMRSVD39(uint8_t DIPortInfo, uint8_t TerminalNum)
{
}

void TerminalDIECAMRSVD40(uint8_t DIPortInfo, uint8_t TerminalNum)
{
}

void TerminalDIPLICH1(uint8_t DIPortInfo, uint8_t TerminalNum)
{
  DICtrl *DITemp;
  DITemp = DITable[TerminalNum - 1];
  if (DIPortInfo)
    DITemp->bit.PullInCh1 = OK;
  else
    DITemp->bit.PullInCh1 = NG;
}

void TerminalDIPLICH2(uint8_t DIPortInfo, uint8_t TerminalNum)
{
  DICtrl *DITemp;
  DITemp = DITable[TerminalNum - 1];
  if (DIPortInfo)
    DITemp->bit.PullInCh2 = OK;
  else
    DITemp->bit.PullInCh2 = NG;
}

void TerminalDIPLICH3(uint8_t DIPortInfo, uint8_t TerminalNum)
{
  DICtrl *DITemp;
  DITemp = DITable[TerminalNum - 1];
  if (DIPortInfo)
    DITemp->bit.PullInCh3 = OK;
  else
    DITemp->bit.PullInCh3 = NG;
}

void TerminalPIHLSWITCH(uint8_t DIPortInfo, uint8_t TerminalNum)
{
  DICtrl *DITemp;
  DITemp = DITable[TerminalNum - 1];
  if (DIPortInfo)
    DITemp->bit.PullInSwth = OK;
  else
    DITemp->bit.PullInSwth = NG;
}

/*************************************************
 Function: Terminal Function Synthesis: Servo On Input
 Description:Period:1ms
 Input: No
 Return: No
 Others: Be Called TerminalSynthesis[]
 *************************************************/
void TerminalFuncServoOn(void)
{
  if (DICtrlFlg1.bit.ServoOn || DICtrlFlg2.bit.ServoOn || DICtrlFlg3.bit.ServoOn || DICtrlFlg4.bit.ServoOn)
    TerminalDICtrlFlg.bit.ServoOn = OK;
  else
    TerminalDICtrlFlg.bit.ServoOn = NG;
}

void TerminalFuncPOT(void) //20190415
{
  static int64_t POT_Position = 0;
  if (DICtrlFlg1.bit.POT || DICtrlFlg2.bit.POT || DICtrlFlg3.bit.POT || DICtrlFlg4.bit.POT)
  {
    if (TerminalDICtrlFlg.bit.POT == NG)
    {
      POT_Position = g_PosiFeedback;
      TerminalDICtrlFlg.bit.POT = OK;
    }
  }
  else if (g_PosiFeedback <= POT_Position)
    TerminalDICtrlFlg.bit.POT = NG;

}

void TerminalFuncNOT(void) //20190415
{
  static int64_t NOT_Position = 0;
  if (DICtrlFlg1.bit.NOT || DICtrlFlg2.bit.NOT || DICtrlFlg3.bit.NOT || DICtrlFlg4.bit.NOT)
  {
    if (TerminalDICtrlFlg.bit.NOT == NG)
    {
      NOT_Position = g_PosiFeedback;
      TerminalDICtrlFlg.bit.NOT = OK;
    }
  }
  else if (g_PosiFeedback >= NOT_Position)
    TerminalDICtrlFlg.bit.NOT = NG;
}
/*************************************************
 Function: Terminal Function Synthesis: Alarm clear input
 Description:Period:1ms
 Input: No
 Return: No
 Others: Be Called TerminalSynthesis[]
 *************************************************/
void TerminalFuncAlmClr(void)
{
  if (DICtrlFlg1.bit.AlmClr || DICtrlFlg2.bit.AlmClr || DICtrlFlg3.bit.AlmClr || DICtrlFlg4.bit.AlmClr)
    TerminalDICtrlFlg.bit.AlmClr = OK;
  else
    TerminalDICtrlFlg.bit.AlmClr = NG;
}
/*************************************************
 Function: Terminal Function Synthesis: Control mode switching input
 Description:Period:1ms
 Input: No
 Return: No
 Others: Be Called TerminalSynthesis[]
 *************************************************/
void TerminalFuncCtrlMode(void)
{
  if (DICtrlFlg1.bit.CtrlMode || DICtrlFlg2.bit.CtrlMode || DICtrlFlg3.bit.CtrlMode || DICtrlFlg4.bit.CtrlMode)
    TerminalDICtrlFlg.bit.CtrlMode = OK;
  else
    TerminalDICtrlFlg.bit.CtrlMode = NG;
}
/*************************************************
 Function: Terminal Function Synthesis: Gain switching input
 Description:Period:1ms
 Input: No
 Return: No
 Others: Be Called TerminalSynthesis[]
 *************************************************/
void TerminalFuncGain(void)
{
  if (DICtrlFlg1.bit.Gain || DICtrlFlg2.bit.Gain || DICtrlFlg3.bit.Gain || DICtrlFlg4.bit.Gain)
    TerminalDICtrlFlg.bit.Gain = OK;
  else
    TerminalDICtrlFlg.bit.Gain = NG;
}
/*************************************************
 Function: Terminal Function Synthesis: Deviation counter clear input
 Description:Period:1ms
 Input: No
 Return: No
 Others: Be Called TerminalSynthesis[]
 *************************************************/
void TerminalFuncCL(void)
{
  uint8_t PosiClr; // = 0;
  static uint8_t PosiClr_old = 0;

  uint8_t PosiClr_Mode; // = 0;

  if (DICtrlFlg1.bit.CL || DICtrlFlg2.bit.CL || DICtrlFlg3.bit.CL || DICtrlFlg4.bit.CL)
    PosiClr = OK;
  else
    PosiClr = NG;

  PosiClr_Mode = (FuncCode_Handle.Ram.F50.F5059 & 0x0F);
  if (PosiClr_Mode == 1) //rise active
  {
    if ((PosiClr == 1) && (PosiClr_old == 0)) //rise
    {
      TerminalDICtrlFlg.bit.CL_R = OK;
    }
    TerminalDICtrlFlg.bit.CL = NG;
  }
  else if (PosiClr_Mode == 2) //Low active
  {
    TerminalDICtrlFlg.bit.CL = ~PosiClr;
  }
  else if (PosiClr_Mode == 3) //fall active
  {
    if ((PosiClr == 0) && (PosiClr_old == 1)) //fall
    {
      TerminalDICtrlFlg.bit.CL_F = OK;
    }
    TerminalDICtrlFlg.bit.CL = NG;
  }
  else //High active
  {
    TerminalDICtrlFlg.bit.CL = PosiClr;
  }

  PosiClr_old = PosiClr;
}

/*************************************************
 Function: Terminal Function Synthesis: Command pulse inhibition input
 Description:Period:1ms
 Input: No
 Return: No
 Others: Be Called TerminalSynthesis[]
 *************************************************/
void TerminalFuncINH(void)
{
  if (DICtrlFlg1.bit.INH || DICtrlFlg2.bit.INH || DICtrlFlg3.bit.INH || DICtrlFlg4.bit.INH)
    TerminalDICtrlFlg.bit.INH = OK;
  else
    TerminalDICtrlFlg.bit.INH = NG;
}
/*************************************************
 Function: Terminal Function Synthesis: Torque limit switching input
 Description:Period:1ms
 Input: No
 Return: No
 Others: Be Called TerminalSynthesis[]
 *************************************************/
void TerminalFuncTLS(void)
{
  if (DICtrlFlg1.bit.TLS || DICtrlFlg2.bit.TLS || DICtrlFlg3.bit.TLS || DICtrlFlg4.bit.TLS)
    TerminalDICtrlFlg.bit.TLS = OK;
  else
    TerminalDICtrlFlg.bit.TLS = NG;
}
/*************************************************
 Function: Terminal Function Synthesis: Speed zero clamp input
 Description:Period:1ms
 Input: No
 Return: No
 Others: Be Called TerminalSynthesis[]
 *************************************************/
void TerminalFuncZeroSPD(void)
{
  if (DICtrlFlg1.bit.ZeroSPD || DICtrlFlg2.bit.ZeroSPD || DICtrlFlg3.bit.ZeroSPD || DICtrlFlg4.bit.ZeroSPD)
    TerminalDICtrlFlg.bit.ZeroSPD = OK;
  else
    TerminalDICtrlFlg.bit.ZeroSPD = NG;
}
/*************************************************
 Function: Terminal Function Synthesis: Emergency stop
 Description:Period:1ms
 Input: No
 Return: No
 Others: Be Called TerminalSynthesis[]
 *************************************************/
void TerminalFuncEStop(void)
{
  if (DICtrlFlg1.bit.EStop || DICtrlFlg2.bit.EStop || DICtrlFlg3.bit.EStop || DICtrlFlg4.bit.EStop)
    TerminalDICtrlFlg.bit.EStop = OK;
  else
    TerminalDICtrlFlg.bit.EStop = NG;
}
/*************************************************
 Function: Terminal Function Synthesis: Electronic gear(division/multiplication) switching input0
 Description:Period:1ms
 Input: No
 Return: No
 Others: Be Called TerminalSynthesis[]
 *************************************************/
void TerminalFuncDIV0(void)
{
  if (DICtrlFlg1.bit.DIV0 || DICtrlFlg2.bit.DIV0 || DICtrlFlg3.bit.DIV0 || DICtrlFlg4.bit.DIV0)
    TerminalDICtrlFlg.bit.DIV0 = OK;
  else
    TerminalDICtrlFlg.bit.DIV0 = NG;
}
/*************************************************
 Function: Terminal Function Synthesis: Electronic gear(division/multiplication) switching input1
 Description:Period:1ms
 Input: No
 Return: No
 Others: Be Called TerminalSynthesis[]
 *************************************************/
void TerminalFuncDIV1(void)
{
  if (DICtrlFlg1.bit.DIV1 || DICtrlFlg2.bit.DIV1 || DICtrlFlg3.bit.DIV1 || DICtrlFlg4.bit.DIV1)
    TerminalDICtrlFlg.bit.DIV1 = OK;
  else
    TerminalDICtrlFlg.bit.DIV1 = NG;
}
/*************************************************
 Function: Terminal Function Synthesis: Selection input of internal command speed0
 Description:Period:1ms
 Input: No
 Return: No
 Others: Be Called TerminalSynthesis[]
 *************************************************/
void TerminalFuncINTSPD0(void)
{
  static uint8_t Postouchof1_old = 0;

  if (DICtrlFlg1.bit.INTSPD0 || DICtrlFlg2.bit.INTSPD0 || DICtrlFlg3.bit.INTSPD0 || DICtrlFlg4.bit.INTSPD0)
    TerminalDICtrlFlg.bit.INTSPD0 = OK;
  else
    TerminalDICtrlFlg.bit.INTSPD0 = NG;

  if ((TerminalDICtrlFlg.bit.INTSPD0 == 1) && (Postouchof1_old == 0)) //20171009 mxj
  {
    TerminalDICtrlFlg.bit.Spdtouchof1 = OK;
  }

  Postouchof1_old = TerminalDICtrlFlg.bit.INTSPD0;
}
/*************************************************
 Function: Terminal Function Synthesis: Selection input of internal command speed1
 Description:Period:1ms
 Input: No
 Return: No
 Others: Be Called TerminalSynthesis[]
 *************************************************/
void TerminalFuncINTSPD1(void)
{
  static uint8_t Postouchof2_old = 0;

  if (DICtrlFlg1.bit.INTSPD1 || DICtrlFlg2.bit.INTSPD1 || DICtrlFlg3.bit.INTSPD1 || DICtrlFlg4.bit.INTSPD1)
    TerminalDICtrlFlg.bit.INTSPD1 = OK;
  else
    TerminalDICtrlFlg.bit.INTSPD1 = NG;

  if ((TerminalDICtrlFlg.bit.INTSPD1 == 1) && (Postouchof2_old == 0)) //20171009 mxj
  {
    TerminalDICtrlFlg.bit.Spdtouchof2 = OK;
  }

  Postouchof2_old = TerminalDICtrlFlg.bit.INTSPD1;
}
/*************************************************
 Function: Terminal Function Synthesis: Selection input of internal command speed2
 Description:Period:1ms
 Input: No
 Return: No
 Others: Be Called TerminalSynthesis[]
 *************************************************/
void TerminalFuncINTSPD2(void)
{
  static uint8_t Postouchof3_old = 0;

  if (DICtrlFlg1.bit.INTSPD2 || DICtrlFlg2.bit.INTSPD2 || DICtrlFlg3.bit.INTSPD2 || DICtrlFlg4.bit.INTSPD2)
    TerminalDICtrlFlg.bit.INTSPD2 = OK;
  else
    TerminalDICtrlFlg.bit.INTSPD2 = NG;

  if ((TerminalDICtrlFlg.bit.INTSPD2 == 1) && (Postouchof3_old == 0)) //20171009 mxj
  {
    TerminalDICtrlFlg.bit.Spdtouchof3 = OK;
  }

  Postouchof3_old = TerminalDICtrlFlg.bit.INTSPD2;
}
/*************************************************
 Function: Terminal Function Synthesis: Selection input of internal command speed3
 Description:Period:1ms
 Input: No
 Return: No
 Others: Be Called TerminalSynthesis[]
 *************************************************/
void TerminalFuncINTSPD3(void)
{
  static uint8_t Postouchof4_old = 0;

  if (DICtrlFlg1.bit.INTSPD3 || DICtrlFlg2.bit.INTSPD3 || DICtrlFlg3.bit.INTSPD3 || DICtrlFlg4.bit.INTSPD3)
    TerminalDICtrlFlg.bit.INTSPD3 = OK;
  else
    TerminalDICtrlFlg.bit.INTSPD3 = NG;

  if ((TerminalDICtrlFlg.bit.INTSPD3 == 1) && (Postouchof4_old == 0)) //20171009 mxj
  {
    TerminalDICtrlFlg.bit.Spdtouchof4 = OK;
  }

  Postouchof4_old = TerminalDICtrlFlg.bit.INTSPD3;
}
/*************************************************
 Function: Terminal Function Synthesis: Selection input of internal command position0
 Description:Period:1ms
 Input: No
 Return: No
 Others: Be Called TerminalSynthesis[]
 *************************************************/
void TerminalFuncINTPOS0(void)
{
  static uint8_t Postouchof1_old = 0;

  if (DICtrlFlg1.bit.INTPOS0 || DICtrlFlg2.bit.INTPOS0 || DICtrlFlg3.bit.INTPOS0 || DICtrlFlg4.bit.INTPOS0)
    TerminalDICtrlFlg.bit.INTPOS0 = OK;
  else
    TerminalDICtrlFlg.bit.INTPOS0 = NG;

  if ((TerminalDICtrlFlg.bit.INTPOS0 == 1) && (Postouchof1_old == 0)) //20171009 mxj
  {
    TerminalDICtrlFlg.bit.Postouchof1 = OK;
  }

  Postouchof1_old = TerminalDICtrlFlg.bit.INTPOS0;
}
/*************************************************
 Function: Terminal Function Synthesis: Selection input of internal command position1
 Description:Period:1ms
 Input: No
 Return: No
 Others: Be Called TerminalSynthesis[]
 *************************************************/
void TerminalFuncINTPOS1(void)
{
  static uint8_t Postouchof2_old = 0;

  if (DICtrlFlg1.bit.INTPOS1 || DICtrlFlg2.bit.INTPOS1 || DICtrlFlg3.bit.INTPOS1 || DICtrlFlg4.bit.INTPOS1)
    TerminalDICtrlFlg.bit.INTPOS1 = OK;
  else
    TerminalDICtrlFlg.bit.INTPOS1 = NG;

  if ((TerminalDICtrlFlg.bit.INTPOS1 == 1) && (Postouchof2_old == 0)) //20171009 mxj
  {
    TerminalDICtrlFlg.bit.Postouchof2 = OK;
  }

  Postouchof2_old = TerminalDICtrlFlg.bit.INTPOS1;
}
/*************************************************
 Function: Terminal Function Synthesis: Selection input of internal command position2
 Description:Period:1ms
 Input: No
 Return: No
 Others: Be Called TerminalSynthesis[]
 *************************************************/
void TerminalFuncINTPOS2(void)
{
  static uint8_t Postouchof3_old = 0;

  if (DICtrlFlg1.bit.INTPOS2 || DICtrlFlg2.bit.INTPOS2 || DICtrlFlg3.bit.INTPOS2 || DICtrlFlg4.bit.INTPOS2)
    TerminalDICtrlFlg.bit.INTPOS2 = OK;
  else
    TerminalDICtrlFlg.bit.INTPOS2 = NG;

  if ((TerminalDICtrlFlg.bit.INTPOS2 == 1) && (Postouchof3_old == 0)) //20171009 mxj
  {
    TerminalDICtrlFlg.bit.Postouchof3 = OK;
  }

  Postouchof3_old = TerminalDICtrlFlg.bit.INTPOS2;
}
/*************************************************
 Function: Terminal Function Synthesis: Selection input of internal command position3
 Description:Period:1ms
 Input: No
 Return: No
 Others: Be Called TerminalSynthesis[]
 *************************************************/
void TerminalFuncINTPOS3(void)
{
  static uint8_t Postouchof4_old = 0;

  if (DICtrlFlg1.bit.INTPOS3 || DICtrlFlg2.bit.INTPOS3 || DICtrlFlg3.bit.INTPOS3 || DICtrlFlg4.bit.INTPOS3)
    TerminalDICtrlFlg.bit.INTPOS3 = OK;
  else
    TerminalDICtrlFlg.bit.INTPOS3 = NG;

  if ((TerminalDICtrlFlg.bit.INTPOS3 == 1) && (Postouchof4_old == 0))
  {
    TerminalDICtrlFlg.bit.Postouchof4 = OK;
  }

  Postouchof4_old = TerminalDICtrlFlg.bit.INTPOS3;
}
/*************************************************
 Function: Terminal Function Synthesis: Start Origin homing mode
 Description:Period:1ms
 Input: No
 Return: No
 Others: Be Called TerminalSynthesis[]
 *************************************************/
void TerminalFuncSOR(void)
{
  if (DICtrlFlg1.bit.SOR || DICtrlFlg2.bit.SOR || DICtrlFlg3.bit.SOR || DICtrlFlg4.bit.SOR)
    TerminalDICtrlFlg.bit.SOR = OK;
  else
    TerminalDICtrlFlg.bit.SOR = NG;
}
/*************************************************
 Function: Terminal Function Synthesis: Ensure Origin homing mode position
 Description:Period:1ms
 Input: No
 Return: No
 Others: Be Called TerminalSynthesis[]
 *************************************************/
void TerminalFuncSORPOS(void)
{
  if (DICtrlFlg1.bit.SORPOS || DICtrlFlg2.bit.SORPOS || DICtrlFlg3.bit.SORPOS || DICtrlFlg4.bit.SORPOS)
    TerminalDICtrlFlg.bit.SORPOS = OK;
  else
    TerminalDICtrlFlg.bit.SORPOS = NG;
}
/*************************************************
 Function: Terminal Function Synthesis:Positive JOG
 Description:Period:1ms
 Input: No
 Return: No
 Others: Be Called TerminalSynthesis[]
 *************************************************/
void TerminalFuncPJOG(void)
{
  if (DICtrlFlg1.bit.PJOG || DICtrlFlg2.bit.PJOG || DICtrlFlg3.bit.PJOG || DICtrlFlg4.bit.PJOG)
    TerminalDICtrlFlg.bit.PJOG = OK;
  else
    TerminalDICtrlFlg.bit.PJOG = NG;
}
/*************************************************
 Function: Terminal Function Negative JOG
 Description:Period:1ms
 Input: No
 Return: No
 Others: Be Called TerminalSynthesis[]
 *************************************************/
void TerminalFuncNJOG(void)
{
  if (DICtrlFlg1.bit.NJOG || DICtrlFlg2.bit.NJOG || DICtrlFlg3.bit.NJOG || DICtrlFlg4.bit.NJOG)
    TerminalDICtrlFlg.bit.NJOG = OK;
  else
    TerminalDICtrlFlg.bit.NJOG = NG;
}
/*************************************************
 Function: Terminal Function :Motor rotation direction
 Description:Period:1ms
 Input: No
 Return: No
 Others: Be Called TerminalSynthesis[]
 *************************************************/
void TerminalFuncMTRDIR(void)
{
  if (DICtrlFlg1.bit.MTRDIR || DICtrlFlg2.bit.MTRDIR || DICtrlFlg3.bit.MTRDIR || DICtrlFlg4.bit.MTRDIR)
    TerminalDICtrlFlg.bit.MTRDIR = OK;
  else
    TerminalDICtrlFlg.bit.MTRDIR = NG;
}

void TerminalFuncINER1(void)
{
  TerminalDICtrlFlg.bit.INERSWH1 = DICtrlFlg1.bit.INERSWH1 || DICtrlFlg2.bit.INERSWH1 || DICtrlFlg3.bit.INERSWH1
      || DICtrlFlg4.bit.INERSWH1;
}

void TerminalFuncINER2(void)
{
  TerminalDICtrlFlg.bit.INERSWH2 = DICtrlFlg1.bit.INERSWH2 || DICtrlFlg2.bit.INERSWH2 || DICtrlFlg3.bit.INERSWH2
      || DICtrlFlg4.bit.INERSWH2;
}

void TerminalFuncINER3(void)
{
  TerminalDICtrlFlg.bit.INERSWH3 = DICtrlFlg1.bit.INERSWH3 || DICtrlFlg2.bit.INERSWH3 || DICtrlFlg3.bit.INERSWH3
      || DICtrlFlg4.bit.INERSWH3;
}

void TerminalFuncECAMEN(void)
{
}
void TerminalFuncECAMSEL0(void)
{
}
void TerminalFuncECAMSEL1(void)
{
}
void TerminalFuncECAMSEL2(void)
{
}
void TerminalFuncECUTFISH(void)
{
}
void TerminalFuncECAMRSVD35(void)
{
}
void TerminalFuncECAMRSVD36(void)
{
}
void TerminalFuncECAMRSVD37(void)
{
}
void TerminalFuncECAMRSVD38(void)
{
}
void TerminalFuncECAMRSVD39(void)
{
}
void TerminalFuncECAMRSVD40(void)
{
}

void TerminalFuncPLICH1(void)
{
  TerminalDICtrlFlg.bit.PullInCh1 = DICtrlFlg1.bit.PullInCh1 || DICtrlFlg2.bit.PullInCh1 || DICtrlFlg3.bit.PullInCh1
      || DICtrlFlg4.bit.PullInCh1;
}

void TerminalFuncPLICH2(void)
{
  TerminalDICtrlFlg.bit.PullInCh2 = DICtrlFlg1.bit.PullInCh2 || DICtrlFlg2.bit.PullInCh2 || DICtrlFlg3.bit.PullInCh2
      || DICtrlFlg4.bit.PullInCh2;
}

void TerminalFuncPLICH3(void)
{
  TerminalDICtrlFlg.bit.PullInCh3 = DICtrlFlg1.bit.PullInCh3 || DICtrlFlg2.bit.PullInCh3 || DICtrlFlg3.bit.PullInCh3
      || DICtrlFlg4.bit.PullInCh3;
}

void TerminalFuncPIHLSWITCH(void)
{
  TerminalDICtrlFlg.bit.PullInSwth = DICtrlFlg1.bit.PullInSwth || DICtrlFlg2.bit.PullInSwth || DICtrlFlg3.bit.PullInSwth
      || DICtrlFlg4.bit.PullInSwth;
}

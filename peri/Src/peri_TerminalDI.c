/*
 * peri_TerminalDI.c
 *
 * Created on: 2014-5-22
 * Author: JH-LvSL
 */
#include <main.h>
#include <peri_GlobalVariablesExtern.h>
#include <peri_SwScan.h>
#include <peri_TerminalDI.h>

void TerminalDIStartFuncHandle(const TERMINALDI_FUNC *func);
void TerminalDIFuncSynthesis(const TERMINALDI_FUNC *func);
uint8_t ScanVirtualDISwitch(uint8_t TerminalDISta, uint8_t TerminalNum);

static const TERMINALDI_FUNC TerminalDIProcessTable[] =
{
  TerminalDI1Handle,
  TerminalDI2Handle,
  TerminalDI3Handle,
  TerminalDI4Handle,
  NULL_FUNC
};

void (*const TerminalDIFuncHandle[])(uint8_t DIPortInfo, uint8_t TerminalNum) =
  {
  TerminalDIServoOn,
  TerminalDIPOT,
  TerminalDINOT,
  TerminalDIAlmClr,
  TerminalDICtrlMode,
  TerminalDIGain,
  TerminalDICL,
  TerminalDIINH,
  TerminalDITLS,
  TerminalDIZeroSPD,	//10
  TerminalDIEStop,//11
  TerminalDIDIV0,
  TerminalDIDIV1,
  TerminalDIINTSPD0,
  TerminalDIINTSPD1,
  TerminalDIINTSPD2,
  TerminalDIINTSPD3,
  TerminalDIINTPOS0,
  TerminalDIINTPOS1,
  TerminalDIINTPOS2,
  TerminalDIINTPOS3,//21
  TerminalDISOR,//22
  TerminalDISORPOS,//23
  TerminalDIPJOG,//24
  TerminalDINJOG,//25
  TerminalDIMTRDIR,	//26	电机转向输入
  TerminalDIINER1,//27
  TerminalDIINER2,//28
  TerminalDIINER3,//29
  TerminalDIECAMEN,//30
  TerminalDIECAMSEL0,//31
  TerminalDIECAMSEL1,//32
  TerminalDIECAMSEL2,//33
  TerminalDIECUTFISH,//34
  TerminalDIECAMRSVD35,//35
  TerminalDIECAMRSVD36,//36
  TerminalDIECAMRSVD37,//37
  TerminalDIECAMRSVD38,//38
  TerminalDIECAMRSVD39,//39
  TerminalDIECAMRSVD40,//40
  TerminalDIPLICH1,		//41	指令切换输入1
  TerminalDIPLICH2,		//42	指令切换输入2
  TerminalDIPLICH3,		//43	指令切换输入3
  TerminalPIHLSWITCH,		//44	脉冲口切换输入
  NULL_FUNC
};

static const TERMINALDI_FUNC TerminalSynthesis[] =
{
  TerminalFuncServoOn,	//1
  TerminalFuncPOT,		//2
  TerminalFuncNOT,		//3
  TerminalFuncAlmClr,		//4
  TerminalFuncCtrlMode,	//5
  TerminalFuncGain,		//6
  TerminalFuncCL,			//7
  TerminalFuncINH,		//8
  TerminalFuncTLS,		//9
  TerminalFuncZeroSPD,	//10
  TerminalFuncEStop,		//11
  TerminalFuncDIV0,		//12
  TerminalFuncDIV1,		//13
  TerminalFuncINTSPD0,	//14
  TerminalFuncINTSPD1,	//15
  TerminalFuncINTSPD2,	//16
  TerminalFuncINTSPD3,	//17
  TerminalFuncINTPOS0,	//18
  TerminalFuncINTPOS1,	//19
  TerminalFuncINTPOS2,	//20
  TerminalFuncINTPOS3,	//21
  TerminalFuncSOR,		//22
  TerminalFuncSORPOS,		//23
  TerminalFuncPJOG,		//24
  TerminalFuncNJOG,		//25
  TerminalFuncMTRDIR,		//26
  TerminalFuncINER1,       //27
  //TerminalFuncINER2,       //28
  //TerminalFuncINER3,       //29
  //TerminalFuncECAMEN,      //30
  //TerminalFuncECAMSEL0,    //31
  //TerminalFuncECAMSEL1,    //32
  //TerminalFuncECAMSEL2,    //33
  //TerminalFuncECUTFISH,    //34
  //TerminalFuncECAMRSVD35,  //35
  //TerminalFuncECAMRSVD36,  //36
  //TerminalFuncECAMRSVD37,  //37
  //TerminalFuncECAMRSVD38,  //38
  //TerminalFuncECAMRSVD39,  //39
  //TerminalFuncECAMRSVD40,  //40
  TerminalFuncPLICH1,		//41
  TerminalFuncPLICH2,		//42
  TerminalFuncPLICH3,		//43
  TerminalFuncPIHLSWITCH,		//44 TerminalPIHLSWITCH
  NULL_FUNC
};

/*************************************************
 Function: Terminal DI Handle
 Description:Period:1ms
 Input: No
 Return: No
 Others: Be Called TerminalDIProcessTable[]
 *************************************************/
void TerminalDIHandle(void)
{
  TerminalDIStartFuncHandle_New();		//20190922
  TerminalDIStartFuncHandle(TerminalDIProcessTable);		//输入IO口赋值处理
  TerminalDIFuncSynthesis(TerminalSynthesis);		//标志位生成
}
/*************************************************
 Function: Terminal DI Function Handle
 Description:Period:1ms
 Input: No
 Return: No
 Others: Be Called TerminalDIHandle()
 *************************************************/
void TerminalDIStartFuncHandle(const TERMINALDI_FUNC *func)
{
  while ((*func) != NULL_FUNC )
  {
    (*func)();
    func++;
  }
}
/*************************************************
 Function: Terminal Function Synthesis
 Description:Period:1ms
 Input: No
 Return: No
 Others: Be Called TerminalDIHandle()
 *************************************************/
void TerminalDIFuncSynthesis(const TERMINALDI_FUNC *func)
{
  while ((*func) != NULL_FUNC )
  {
    (*func)();
    func++;
  }
}

/*************************************************
 Function: Terminal DI0 Handle
 Description:Period:1ms
 Input: No
 Return: No
 Others: Be Called TerminalDIProcessTable[]
 *************************************************/
void TerminalDI1Handle(void)
{
  uint32_t FcodeValue = 0;
  void (*TerminalFunc)(uint8_t, uint8_t);
  //uint8_t tempDISta=ScanVirtualDISwitch((uint8_t)DI1.flg.bit.DI_sta_F,NumDI1);

  uint8_t tempDISta = g_DIAddVir_sts & 0x01;

  DICtrlFlg1.all = 0;

  FcodeValue = FuncCode_Handle.Ram.F50.F5000;

  if (FcodeValue != 0)
  {
    TerminalFunc = TerminalDIFuncHandle[(FcodeValue - 1)];		//决定调用哪个功能
    (*TerminalFunc)(tempDISta, NumDI1);		//按照特定功能给第一个输入结构体赋值
  }
}
/*************************************************
 Function: Terminal DI1 Handle
 Description:Period:1ms
 Input: No
 Return: No
 Others: Be Called TerminalDIProcessTable[]
 *************************************************/
void TerminalDI2Handle(void)
{
  uint32_t FcodeValue = 0;
  void (*TerminalFunc)(uint8_t, uint8_t);
  //uint8_t tempDISta=ScanVirtualDISwitch((uint8_t)DI2.flg.bit.DI_sta_F,NumDI2);

  uint8_t tempDISta = (g_DIAddVir_sts >> 1) & 0x01;

  DICtrlFlg2.all = 0;

  FcodeValue = FuncCode_Handle.Ram.F50.F5001;

  if (FcodeValue != 0)
  {
    TerminalFunc = TerminalDIFuncHandle[(FcodeValue - 1)];		//决定调用哪个功能
    (*TerminalFunc)(tempDISta, NumDI2);		//按照特定功能给第一个输入结构体赋值
  }
}
/*************************************************
 Function: Terminal DI2 Handle
 Description:Period:1ms
 Input: No
 Return: No
 Others: Be Called TerminalDIProcessTable[]
 *************************************************/
void TerminalDI3Handle(void)
{
  uint32_t FcodeValue = 0;
  void (*TerminalFunc)(uint8_t, uint8_t);
  //uint8_t tempDISta=ScanVirtualDISwitch((uint8_t)DI3.flg.bit.DI_sta_F,NumDI3);

  uint8_t tempDISta = (g_DIAddVir_sts >> 2) & 0x01;

  DICtrlFlg3.all = 0;

  FcodeValue = FuncCode_Handle.Ram.F50.F5002;

  if (FcodeValue != 0)
  {
    TerminalFunc = TerminalDIFuncHandle[(FcodeValue - 1)];		//决定调用哪个功能
    (*TerminalFunc)(tempDISta, NumDI3);		//按照特定功能给第一个输入结构体赋值
  }
}
/*************************************************
 Function: Terminal DI3 Handle
 Description:Period:1ms
 Input: No
 Return: No
 Others: Be Called TerminalDIProcessTable[]
 *************************************************/
void TerminalDI4Handle(void)
{
  uint32_t FcodeValue = 0;
  void (*TerminalFunc)(uint8_t, uint8_t);
  //uint8_t tempDISta=ScanVirtualDISwitch((uint8_t)DI4.flg.bit.DI_sta_F,NumDI4);

  uint8_t tempDISta = (g_DIAddVir_sts >> 3) & 0x01;

  DICtrlFlg4.all = 0;

  FcodeValue = FuncCode_Handle.Ram.F50.F5003;

  if (FcodeValue != 0)
  {
    TerminalFunc = TerminalDIFuncHandle[(FcodeValue - 1)];		//决定调用哪个功能
    (*TerminalFunc)(tempDISta, NumDI4);		//按照特定功能给第一个输入结构体赋值
  }
}

void TerminalDIStartFuncHandle_New(void)		//20191024
{
  uint16_t tempVirDIForeON;
  uint16_t tempVirDIForeOFF;
  int32_t tempDIsts = g_DIsts;

  uint32_t tempMasker = FuncCode_Handle.Ram.F50.F5009 >> 8;

  tempVirDIForeON = (uint16_t) (ForceDIOnEn.all); //虚拟DI ON
  tempVirDIForeOFF = (uint16_t) (ForceDIOffEn.all); //虚拟DI OFF

  g_DIAddVir_sts = tempVirDIForeON | ((~tempVirDIForeON) & (~tempVirDIForeOFF) & (tempDIsts ^ tempMasker)); //d=a+(/a)(/b)(c^d)
}


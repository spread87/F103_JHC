/*
 * peri_MonitorHandle.c
 *
 * Created on: 2015-3-16
 * Author: JH-LvSL
 */
#include <main.h>
#include <peri_GlobalVariablesExtern.h>
#include <peri_SwScan.h>
#include <peri_TerminalDI.h>

int32_t MonParaMotorVeloCalc(void);
int32_t MonParaVeloCMDCalc(void);
int32_t MonParaIrmsCalc(void);
int32_t MonParaVrmsCalc(void);
int32_t MonParaCurCMDCalc(void);
int32_t MonParaAvgLoadRateCalc(void);
int32_t MonParaAvgVbusCalc(void);
int32_t MonParaToutCalc(void);
int32_t MonParaAI1Calc(void);
int32_t MonParaAI2Calc(void);
int32_t MonParaAI3Calc(void);
int32_t MonParaAI1ADCalc(int32_t AdcResult);
int32_t MonParaAI2ADCalc(int32_t AdcResult);
int32_t MonParaAI3ADCalc(int32_t AdcResult);

/*************************************************
 Function: Monitor Parameter Handle
 Description: period:System Loop
 Input: No
 Return: No
 Others: Called by SysMainLoop()
 *************************************************/
void MonitorParaHandle(void)
{
  FuncCode_Handle.Ram.FA0.FA000 = MonParaMotorVeloCalc();
  FuncCode_Handle.Ram.FA0.FA001 = g_Vrms_Cmd; //0.1V设定电压   MonParaVeloCMDCalc();
  FuncCode_Handle.Ram.FA0.FA002 = g_VrmsCalc; //0.1V实际输出电压();
  FuncCode_Handle.Ram.FA0.FA003 = g_Irms_Cmd_A; //0.1A设定电流   MonParaIrmsCalc();;
  FuncCode_Handle.Ram.FA0.FA004 = g_IrmsCalc; //0.1A实际输出电流   MonParaAvgLoadRateCalc();
  FuncCode_Handle.Ram.FA0.FA005 = g_PrmsCalc / 1000; //0.1kW实际输出功率    MonParaVrmsCalc();
  FuncCode_Handle.Ram.FA0.FA006 = MonParaAvgVbusCalc();
  FuncCode_Handle.Ram.FA0.FA007 = ((int32_t) FuncCode_Handle.Ram.FA0.FA007 * 15 + (int32_t) g_PulInAllFWD) >> 4;
  FuncCode_Handle.Ram.FA0.FA008 = (int32_t) g_PulInAllCount;
  FuncCode_Handle.Ram.FA0.FA009 = g_PosiFeedback; //20190409
  FuncCode_Handle.Ram.FA0.FA010 = (int32_t) g_PosDevtRegCount;
  FuncCode_Handle.Ram.FA0.FA011 = (int32_t) g_Temp10msFilQ24;
  FuncCode_Handle.Ram.FA0.FA012 = g_DIAddVir_sts; //20190922
  FuncCode_Handle.Ram.FA0.FA013 = g_DOsts; //20190328
  FuncCode_Handle.Ram.FA0.FA015 = 0;
  FuncCode_Handle.Ram.FA0.FA016 = MonParaAI1ADCalc(g_AI1FilQ24); //AI1 AI输入电压值 20190704
  FuncCode_Handle.Ram.FA0.FA017 = MonParaAI2ADCalc(g_AI2FilQ24); //AI2
  FuncCode_Handle.Ram.FA0.FA018 = MonParaAI3ADCalc(g_AI3FilQ24); //AI2
  FuncCode_Handle.Ram.FA0.FA019 = g_PowerUpTime;
  FuncCode_Handle.Ram.FA0.FA020 = g_RunTime;
  FuncCode_Handle.Ram.FA0.FA021 = g_ComPulse; // 0x8002 8003
  FuncCode_Handle.Ram.FA0.FA022 = g_ComSpeed; //8006 8007
  FuncCode_Handle.Ram.FA0.FA023 = g_ComTorque; //8008
  FuncCode_Handle.Ram.FA0.FA024 = g_ComPidRef; //8009
  FuncCode_Handle.Ram.FA0.FA025 = g_ComAO1; //800A
  FuncCode_Handle.Ram.FA0.FA026 = g_ComAO2; //800B
  FuncCode_Handle.Ram.FA0.FA028 = SeriEncoder.Angle;
  FuncCode_Handle.Ram.FA0.FA029 = g_PositionCount; //20190220
  FuncCode_Handle.Ram.FA0.FA030 = SeriEncoder.CycleCnt;
  FuncCode_Handle.Ram.FA0.FA031 = SeriEncoder.Tempture;
  FuncCode_Handle.Ram.FA0.FA033 = ComCtrlFlg.all; //控制命令字
  FuncCode_Handle.Ram.FA0.FA034 = CoreStatusFlag.all; //状态字
  FuncCode_Handle.Ram.FA0.FA040 = g_PulInPosition; //脉冲输入频率，电子齿轮比后，单位kHz
  //FuncCode_Handle.Ram.FA0.FA041 = (g_EncoderUVWQ24 >> 22) * 10000 + CCU40_CC40->TIMER; //Encoder cnt
  //FuncCode_Handle.Ram.FA0.FA045 = MX_GPIO_ReadPin(IO004_Handle26) + g_RXDL * 1000 + g_TXDataLength * 10; //个位表示REDE状态
  //g_canview15 = ((int32_t) MX_GPIO_ReadPin(IO004_Handle26) << 21) * (g_TXDataLength + 1) + ((int32_t) g_RXDL << 21); //test
  FuncCode_Handle.Ram.FA0.FA048 = AdcResult[7]; //1.65V
  FuncCode_Handle.Ram.FA0.FA049 = MonParaAI1Calc(); //ExAI.AI1Q24 经过曲线变换后的0.1%  20190704
  FuncCode_Handle.Ram.FA0.FA050 = MonParaAI2Calc(); //ExAI.AI2Q24
  FuncCode_Handle.Ram.FA0.FA051 = AdcResult[6] * 10; //AI3
  FuncCode_Handle.Ram.FA0.FA052 = AdcResult[8]; //1.414V
}

/*************************************************
 Function: Motor Velocity Calc
 Description: period:System Loop
 Input: No
 Return: No
 Others: Called by SysMainLoop()
 *************************************************/
int32_t MonParaMotorVeloCalc(void)
{
  static int64_t AvgVelo = 0;

  uint16_t Temp_V;
  uint16_t Temp_I;

  if (CoreStatusFlag.bit.RUN_OK_F) //RUN
  {
    Temp_V = g_VrmsCalc / 10;
    Temp_I = g_IrmsCalc / 10;
  }
  else //STOP
  {
    Temp_V = g_Vrms_Cmd / 10;
    Temp_I = g_Irms_Cmd / 10;
  }

  if (FuncCode_Handle.Ram.F00.F0009 <= 999) //电压上限低于100V电流显示三位
  {
    if (Temp_I > 999)
      Temp_I = 999;

    AvgVelo = Temp_V * 1000 + Temp_I;
  }
  else //高于100V电流显示两位
  {
    if (Temp_I > 99)
      Temp_I = 99;

    AvgVelo = Temp_V * 100 + Temp_I;
  }

  return (int32_t) (AvgVelo);
}

/*************************************************
 Function:Velocity Command  Calc
 Description: period:System Loop
 Input: No
 Return: No
 Others: Called by SysMainLoop()
 *************************************************/
int32_t MonParaVeloCMDCalc(void)
{
  static uint16_t i = 0;
  static int64_t Sum = 0;
  //static int32_t AvgVeloCMDQ24 = 0;
  static int64_t AvgVeloCMD = 0;

  if (i < 128)
  {
    i++;
    Sum += (int64_t) g_VelocityRefQ24;
  }
  else
  {
    AvgVeloCMD = (int32_t) ((Sum * 220) >> 31);
    i = 0;
    Sum = 0;
  }

  return AvgVeloCMD;
}

/*************************************************
 Function: Output Current Calc
 Description: period:System Loop
 Input: No
 Return: No
 Others: Called by SysMainLoop()
 *************************************************/
int32_t MonParaIrmsCalc(void)
{
  static uint16_t i = 0;
  static int64_t Sum = 0;
  static int32_t AvgIrmsQ24 = 0;
  static int64_t AvgIrms = 0;

  if (i < 128)
  {
    i++;
    Sum += (int64_t) g_IrmsCalc;
  }
  else
  {
    AvgIrms = (int32_t) ((Sum) >> 7);

    i = 0;
    Sum = 0;
  }

  return AvgIrms;
}

/*************************************************
 Function: Output Voltage Calc
 Description: period:System Loop
 Input: No
 Return: No
 Others: Called by SysMainLoop()
 *************************************************/
int32_t MonParaVrmsCalc(void)
{
  static uint16_t i = 0;
  static int64_t Sum = 0;
  static int64_t AvgVrms = 0;

  if (i < 128)
  {
    i++;
    Sum += (int64_t) g_VrmsCalc;
  }
  else
  {
    //AvgVrms = (int32_t)((Sum * DriverPara.RatedVol * 10) >> 31);//20181212
    AvgVrms = (int32_t) ((Sum) >> 7); //20191226
    i = 0;
    Sum = 0;
  }

  return AvgVrms;
}

/*************************************************
 Function:  Current  CMD Calc
 Description: period:System Loop
 Input: No
 Return: No
 Others: Called by SysMainLoop()
 *************************************************/
int32_t MonParaCurCMDCalc(void)
{
  static uint16_t i = 0;
  static int64_t Sum = 0;
//	static int32_t AvgCurCMDQ24 = 0;
  static int64_t AvgCurCMD = 0;

  if (i < 128)
  {
    i++;
    Sum += (int64_t) g_CurrentRefQ24;
  }
  else
  {
    //AvgCurCMDQ24 = (int32_t)(Sum >> 6);
    AvgCurCMD = (int32_t) ((Sum * 100) >> 31);
    i = 0;
    Sum = 0;
  }

  //AvgCurCMD = (int64_t)AvgCurCMDQ24 * 100;

  //return (int32_t)(AvgCurCMD >> 24);
  return AvgCurCMD;
}

/*************************************************
 Function:  Current  CMD Calc
 Description: period:System Loop
 Input: No
 Return: No
 Others: Called by SysMainLoop()
 *************************************************/
int32_t MonParaAvgLoadRateCalc(void)
{
  static uint16_t i = 0;
  static int64_t Sum = 0;
  static int64_t AvgLoadRate = 0;

  if (i < 128)
  {
    i++;
    Sum += (int64_t) g_PrmsCalc;
    //Sum += (int64_t)abs(g_PrmsCalc);
  }
  else
  {
    //AvgLoadRate = (Sum * (750>>10))>>37;
    AvgLoadRate = (Sum / 100) >> 7;
    i = 0;
    Sum = 0;
  }
  return (int32_t) (AvgLoadRate);
}

/*************************************************
 Function: Vbus Calc
 Description: period:System Loop
 Input: No
 Return: No
 Others: Called by SysMainLoop()
 *************************************************/
int32_t MonParaAvgVbusCalc(void)
{
  static uint16_t i = 0;
  static int64_t Sum = 0;
  //static int32_t AvgVbusQ24 = 0;
  static int64_t AvgVbus = 0;

  if (i < 128)
  {
    i++;
    Sum += (int64_t) g_PowerVbus10msFilQ24;
  }
  else
  {
    //AvgVbusQ24 = (int32_t)(Sum >> 6);
    //AvgVbus = (Sum * 3111)>>31;
    if (DriverPara.RatedVol == 220)
      AvgVbus = (Sum * 3111) >> 31;
    else if (DriverPara.RatedVol == 380)
      AvgVbus = (Sum * 5374) >> 31;
    else
      AvgVbus = (Sum * 3111) >> 34;

    i = 0;
    Sum = 0;
  }

  //AvgVbus = (int64_t)AvgVbusQ24 * 3111;

  return (int32_t) (AvgVbus);
}
/*************************************************
 Function: DI/DO Status Monitor
 Description: period:System Loop
 Input: No
 Return: No
 Others: Called by SysMainLoop()
 *************************************************/
void TerminalIOSts(void)
{
  int32_t DISts = 0, DOSts = 0;
/*
  DOSts = MX_GPIO_ReadPin(IO004_Handle18) | (MX_GPIO_ReadPin(IO004_Handle19) << 1)
      | (MX_GPIO_ReadPin(IO004_Handle20) << 2) | (MX_GPIO_ReadPin(IO004_Handle21) << 3)
      | (MX_GPIO_ReadPin(IO004_Handle22) << 4) | (MX_GPIO_ReadPin(IO004_Handle23) << 5);
*/
  DISts = g_DIAddVir_sts;		//20190922

  DOSts ^= 0x03F;
  g_DOsts = DOSts;		//20180328

  g_DIDOsts = (int32_t) ((DOSts << 9) | DISts);
}
/*************************************************
 Function: Output Torque Calc
 Description: period:System Loop
 Input: No
 Return: No
 Others: Called by SysMainLoop()
 *************************************************/
int32_t MonParaToutCalc(void)
{
  static uint16_t i = 0;
  static int64_t Sum = 0;
  //static int32_t AvgIrmsQ24 = 0;
  static int64_t AvgTorque = 0;

  if (i < 128)		//20180904
  {
    i++;
    //Sum += (int64_t)g_Irms_Motor;
  }
  else
  {
    //AvgIrmsQ24 = (int32_t)(Sum >> 6);

    AvgTorque = ((Sum * 100) >> 31);
    i = 0;
    Sum = 0;
  }

  //AvgTorque = (int64_t)AvgIrmsQ24 * DriverPara.RatedCur * MotorPara.Kt;

  return (int32_t) (AvgTorque);
}
/*************************************************
 Function: AI1 Calc
 Description: period:System Loop
 Input: No
 Return: No
 Others: Called by SysMainLoop()
 *************************************************/
int32_t MonParaAI1Calc(void)
{
  static uint16_t i = 0;
  static int64_t Sum = 0;
  //static int32_t AvgAI1Q24 = 0;
  static int64_t AvgAI1 = 0;

  if (i < 128)
  {
    i++;
    Sum += (int64_t) ExAI.AI1Q24;
  }
  else
  {
    //AvgAI1Q24 = (int32_t)(Sum >> 6);
    AvgAI1 = (Sum * 1000) >> 31;
    i = 0;
    Sum = 0;
  }

  //AvgAI1 = (int64_t)AvgAI1Q24 * 1000;

  return (int32_t) (AvgAI1);
}
/*************************************************
 Function: AI1 Calc
 Description: period:System Loop
 Input: No
 Return: No
 Others: Called by SysMainLoop()
 *************************************************/
int32_t MonParaAI2Calc(void)
{
  static uint16_t i = 0;
  static int64_t Sum = 0;
  //static int32_t AvgAI2Q24 = 0;
  static int64_t AvgAI2 = 0;

  if (i < 128)
  {
    i++;
    Sum += (int64_t) ExAI.AI2Q24;
  }
  else
  {
    //AvgAI2Q24 = (int32_t)(Sum >> 6);
    AvgAI2 = (Sum * 1000) >> 31;
    i = 0;
    Sum = 0;
  }

  //AvgAI2 = (int64_t)AvgAI2Q24 * 1000;

  return (int32_t) (AvgAI2);
}

/*************************************************
 Function: AI1 Calc
 Description: period:System Loop
 Input: No
 Return: No
 Others: Called by SysMainLoop()
 *************************************************/
int32_t MonParaAI3Calc(void)
{
  static uint16_t i = 0;
  static int64_t Sum = 0;
  //static int32_t AvgAI3Q24 = 0;
  static int64_t AvgAI3 = 0;

  if (i < 128)
  {
    i++;
    Sum += (int64_t) ExAI.AI3Q24;
  }
  else
  {
    //AvgAI3Q24 = (int32_t)(Sum >> 6);
    AvgAI3 = (Sum * 1000) >> 31;
    i = 0;
    Sum = 0;
  }

  //AvgAI3 = (int64_t)AvgAI3Q24 * 1000;

  return (int32_t) (AvgAI3);
}

int32_t MonParaAI1ADCalc(int32_t AdcResult)
{
  static uint16_t i = 0;
  static int64_t Sum = 0;
  static int64_t AvgAI1AD = 0;

  if (i < 1024)
  {
    i++;
    Sum += (int64_t) AdcResult;
  }
  else
  {
    //AvgAI1AD = (Sum*10)>>10;
    AvgAI1AD = (Sum * 1000) >> 34;		//20190704
    i = 0;
    Sum = 0;
  }

  return (int32_t) (AvgAI1AD);
}

int32_t MonParaAI2ADCalc(int32_t AdcResult)
{
  static uint16_t i = 0;
  static int64_t Sum = 0;
  static int64_t AvgAI2AD = 0;

  if (i < 512)
  {
    i++;
    Sum += (int64_t) AdcResult;
  }
  else
  {
    //AvgAI2AD = (Sum*10)>>9;
    AvgAI2AD = (Sum * 1000) >> 33;		//20190704
    i = 0;
    Sum = 0;
  }

  return (int32_t) (AvgAI2AD);
}
int32_t MonParaAI3ADCalc(int32_t AdcResult)
{
  static uint16_t i = 0;
  static int64_t Sum = 0;
  static int64_t AvgAI3AD = 0;

  if (i < 512)
  {
    i++;
    Sum += (int64_t) AdcResult;
  }
  else
  {
    //AvgAI2AD = (Sum*10)>>9;
    AvgAI3AD = (Sum * 1000) >> 33;		//20190704
    i = 0;
    Sum = 0;
  }

  return (int32_t) (AvgAI3AD);
}


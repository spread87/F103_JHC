/*
 * peri_AlmCheck.c
 *
 * Created on: 2014-3-4
 * Author: JH-LvSL
 */

#include <main.h>			//Declarations from DAVE3 Code Generation (includes SFR declaration)
#include <math.h>
#include <peri_AlmWarnMacro.h>
#include <peri_GlobalVariablesExtern.h>
#include <stdlib.h>

uint16_t g_AlmNum = 0;		//Alarm Number
uint32_t g_OVPUQ24 = 10249746;		//OV Protect PU Value
uint32_t g_LVPUQ24 = 10249746;		//LV Protect PU Value  0.61 =190 or 328V
uint32_t g_DOCPUQ24 = 0;	//Driver OC Protect PU Value
uint32_t g_MOCPUQ24 = 0;	//Motor OC Protect PU Value
uint16_t Bus_Error_Cnt = 0;	//mxj
uint16_t g_Bus_Error_Cnt = 0;	//mxj
//int32_t g_Motor_Power = 0;	//mxj
//static void AlmCheckOV(void);
static void AlmCheckLV(void);
static void AlmCheckOL(void);
static void AlmCheckOS(void);	//过速
static void AlmCheckPD(void);
static void AlmCheckOT(void);
static void AlmCheckDCOF(void);
static void AlmCheckAIE(void);
static void AlmCheckES(void);
static void WarnCheckPOT(void);
static void WarnCheckNOT(void);

//static void AlmCheckFO(void);
static void AlmLockedRotor(void);	//2ms
static void AlmNegRunaway(void);	//2ms
static void AlmCheckSD(void); //10ms
static void EncDisconnect(void);
static void ABErr(void);
static void ZErr(void);
static void AlmCheckAV(void);
static void AlmCheckOLF(void);
void AlmCheckEnc(void);
void ResetDelayHandle(uint16_t id);
void AlmCheckBreakFO(void);

uint8_t g_reset_flag = 0;

/*************************************************
 Function: check alm
 Description: period:2ms
 Input: No
 Return: No
 Others: No
 *************************************************/
void AlmCheck2ms(void)
{
  if (!SysTimeSequ.Start_Flg)
    return;
  //AlmCheckOV();//20180530   move to 1ms
  AlmCheckOLF(); //20180810
  //AlmCheckLV();
  //AlmCheckAV();
//	AlmCheckOS();
  AlmCheckES();
//	AlmCheckEnc();//编码器检测1.6US
  //  AlmLockedRotor();
  //  AlmNegRunaway();

  WarnCheckPOT();  //20161128 CYL
  WarnCheckNOT();  //20161128 CYL

}

/*************************************************
 Function: check alm
 Description: period:10ms
 Input: No
 Return: No
 Others: No
 *************************************************/
void AlmCheck10ms(void)
{
  if (!SysTimeSequ.Start_Flg)
    return;
  //AlmCheckPD();
  AlmCheckSD();
}

/*************************************************
 Function: check alm
 Description: period:100ms
 Input: No
 Return: No
 Others: No
 *************************************************/
void AlmCheck100ms(void)
{
  if (!SysTimeSequ.Start_Flg)
    return;
  AlmCheckOL();
  AlmCheckOT();
  //AlmCheckDCOF();  //None
  AlmCheckPD();

}

/*************************************************
 Function: check alm
 Description: period:500ms
 Input: No
 Return: No
 Others: No
 *************************************************/
void AlmCheck500ms(void)
{
  if (!SysTimeSequ.Start_Flg)
    return;
  AlmCheckAIE();
}

void AlmCheckBreakFO(void)
{
  uint8_t BreakFo;
  static uint8_t BreakFo_Cnt = 0;

  if (DriverPara.RatedPow < 2300)
    return;

  //BreakFo = (uint8_t) MX_GPIO_ReadPin(IO001_Handle18);  //P15.9@SCC1501C
  if (BreakFo == 0)
  {
    BreakFo_Cnt++;
    if (BreakFo_Cnt >= 20)
      AlarmStatusFlag.bit.Driver.BRKFO = OK;
  }
  else
  {
    BreakFo_Cnt = 0;
    AlarmStatusFlag.bit.Driver.BRKFO = NG;
  }
}

/*************************************************
 Function:check drive over-current
 Description: period:1ms
 Input: No
 Return: No
 Others: be called:AlmCheckOC()
 *************************************************/
static void AlmCheckDrvOC(void)
{
  static uint16_t I_count = 0;

  if (AlarmStatusFlag.bit.Driver.DOC)
  {
    I_count = 0;
    return;
  }

  if (CoreStatusFlag.bit.RUN_OK_F == NG)	//check when stopping
  {
    if (g_IrmsCalc >= 20132659)		//IQ(1.2)  20180807
    {
      if (abs(g_VelocityQ24Fil) < 503316)		//0.03  20180807
        I_count++;
    }
    else
    {
      I_count = 0;
    }

    if (I_count >= 3)		//2->3
    {
      AlarmStatusFlag.bit.Driver.DOC = OK;
      //MX_GPIO_ReadPin(IO004_Handle27, 0);		//拉低DRIVE信号
    }
  }
  else		//check when running
  {
    if (AlarmStatusFlag.bit.Motor.SEDC)		//20180713
      return;

    if (g_IrmsCalc > g_DOCPUQ24)
    {
      I_count++;
    }
    else
    {
      I_count = 0;
    }

    //Continued Time: 2ms//
    if (I_count >= 3)		//2->3
    {
      AlarmStatusFlag.bit.Driver.DOC = OK;
    }
    else
    {
      AlarmStatusFlag.bit.Driver.DOC = NG;
    }
  }
}
/*************************************************
 Function:check motor over-current
 Description: period:100us
 Input: No
 Return: No
 Others: be called:AlmCheckOC()
 *************************************************/
static void AlmCheckMotOC(void)		//电机电流
{
  static uint16_t I_count = 0;

  if (AlarmStatusFlag.bit.Motor.MOC)
  {
    I_count = 0;
    return;
  }
  if (!SysTimeSequ.Start_Flg)
  {
    I_count = 0;
    return;
  }

  if (AlarmStatusFlag.bit.Motor.SEDC)		//20180713
    return;

  if (g_IrmsCalc > g_MOCPUQ24)
  {
    I_count++;
  }
  else
  {
    I_count = 0;
  }

  //Continued Time: 2ms//
  if (I_count >= 5)	//2->5  20171201
  {
    AlarmStatusFlag.bit.Motor.MOC = OK;
  }
  else
  {
    AlarmStatusFlag.bit.Motor.MOC = NG;
  }
}

/*************************************************
 Function:Check Motor&Driver Over-current
 Description: Period:1ms
 Input: No
 Return: No
 Others: be called: 1ms-interrupt
 *************************************************/
void AlmCheckOC(void)
{
  AlmCheckDrvOC();	//超过2.8倍，1ms，立马保护
  AlmCheckMotOC();	//超过3.0倍，1ms, 立马保护
}

/*************************************************
 Function:Check Power Over-Voltage
 Description: Period:1ms
 Input: No
 Return: No
 Others: be called: AlmCheckOV()
 *************************************************/
static void AlmCheckPowerOV(void)	//20180530
{
  static uint16_t OV_happen_cnt = 0;

  if (SysTimeSequ.Start_Flg == NG)
  {
    return;
  }

  if (AlarmStatusFlag.bit.Driver.POV == NG)
  {
    if (g_PowerVbus1msFilQ24 >= g_OVPUQ24)
    {
      OV_happen_cnt++;
    }
    else
    {
      if (OV_happen_cnt > 0)
        OV_happen_cnt--;
    }

    if (OV_happen_cnt >= 6)	//if(OV_happen_cnt >= 10)//20190831
    {
      AlarmStatusFlag.bit.Driver.POV = OK;
    }
  }
  else
  {
    OV_happen_cnt = 0;
  }
}

/*************************************************
 Function:Check Over-Voltage
 Description: Period:2ms->1ms
 Input: No
 Return: No
 Others: be called: AlmCheck2ms()
 *************************************************/
void AlmCheckOV(void)
{
  AlmCheckPowerOV();
}

/*************************************************
 Function:Check Power Under-Voltage
 Description: Period:2ms
 Input: No
 Return: No
 Others: be called: AlmCheckLV()
 *************************************************/
static void AlmCheckPowerLV(void)
{
  static uint16_t LV_scan_cnt = 0;
  static uint16_t LV_happen_cnt = 0;
  static uint16_t LV_scan_cnt1 = 0;
  static uint16_t LV_happen_cnt1 = 0;

  if (CoreStatusFlag.bit.EEPROMINIT_OK_F == NG)
  {
    return;
  }

  if (AlarmStatusFlag.bit.Driver.PLV == NG)		//非欠压
  {
    LV_scan_cnt1 = 0;
    LV_happen_cnt1 = 0;

    if (LV_scan_cnt > 2000)
    {
      /*Vbus scan 10 times (7 times effect),when Vbus<110V*/
      if (LV_happen_cnt > 1750)
      {
        AlarmStatusFlag.bit.Driver.PLV = OK;   //暂时屏蔽掉
      }
      LV_scan_cnt = 0;
      LV_happen_cnt = 0;
    }
    else
    {
      LV_scan_cnt++;
      /*Judge Power Board Vbus*/
      if (g_PowerVbus10msFilQ24 < (uint32_t) g_LVPUQ24)
      {
        LV_happen_cnt++;
      }
    }
  }

  else
  {
    LV_scan_cnt = 0;
    LV_happen_cnt = 0;
    if (LV_scan_cnt1 > 2000)
    {
      /*Vbus scan 10 times (7 times effect),when Vbus<110V*/
      if (LV_happen_cnt1 > 1750)
      {
        AlarmStatusFlag.bit.Driver.PLV = NG;   //暂时屏蔽掉
      }
      LV_scan_cnt1 = 0;
      LV_happen_cnt1 = 0;
    }
    else
    {
      LV_scan_cnt1++;
      /*Judge Power Board Vbus*/
      if (g_PowerVbus10msFilQ24 > (uint32_t) (g_LVPUQ24 + 20 * g_VpnVoltGain))
      {
        LV_happen_cnt1++;
      }
    }
  }
}

/*************************************************
 Function:Check Power&Control Board Under-Voltage
 Description: Period:2ms
 Input: No
 Return: No
 Others: be called: AlmCheck2ms()
 *************************************************/
static void AlmCheckLV(void)
{
  AlmCheckPowerLV();
  //AlmCheckCtrlLV();
}
/*************************************************
 Function:Check Driver Over-Load
 Description: Period:100ms
 Input: No
 Return: No
 Others: be called: AlmCheckOL()
 *************************************************/
static void AlmCheckDrvOL(void)
{
  static int32_t I_PerTherm = 0;   //Load-Ratio
  static int32_t C_ThermCountM = 0;
  static int64_t Temp = 0;
  static int32_t DeltaLR = 0;

  if (!CoreStatusFlag.bit.RUN_OK_F)
  {
    I_PerTherm = 0;
    C_ThermCountM = 0;
    DeltaLR = 0;

    return;
  }

  if (AlarmStatusFlag.bit.Driver.DOL)
  {
    I_PerTherm = 0;
    C_ThermCountM = 0;
    DeltaLR = 0;

    return;
  }

  /* Calc Driver Load-Ratio */
  Temp = (int64_t) g_Irms10msFilQ24 * 100;   //
  /* Calc the Deviation between Load-Ratio and 100(zoom:100) */
  I_PerTherm = Temp >> 24;
  I_PerTherm -= FuncCode_Handle.Ram.F00.F0097;    //20181027

  /* Calc the Deviation Square;Notice the sign */
  if (I_PerTherm > 0)
    DeltaLR = I_PerTherm * I_PerTherm;    //超过驱动器电流
  else
    DeltaLR = -(I_PerTherm * I_PerTherm);    //低于驱动器电流

  /*Accumulate the Counts*/
  C_ThermCountM += DeltaLR;

  /*Judge Over-Load Protection*/
  if (C_ThermCountM > (int32_t) OL_THRED)    //门槛值到底对不对？
  {
    AlarmStatusFlag.bit.Driver.DOL = OK;
  }

  if (C_ThermCountM < 0)
  {
    C_ThermCountM = 0;
  }
}

/*************************************************
 Function:Check Motor Over-Load
 Description: Period:100ms
 Input: No
 Return: No
 Others: be called: AlmCheckOL()
 *************************************************/
static void AlmCheckMotOL(void)    //有问题,已经修正
{
  static int32_t I_PerTherm = 0;    //Load-Ratio
  static int32_t C_ThermCountM = 0;
  static int64_t Temp = 0;
  static int32_t DeltaLR = 0;

  if (!CoreStatusFlag.bit.RUN_OK_F)
  {
    I_PerTherm = 0;
    C_ThermCountM = 0;
    DeltaLR = 0;

    return;
  }

  if (AlarmStatusFlag.bit.Motor.MOL)
  {
    I_PerTherm = 0;
    C_ThermCountM = 0;
    DeltaLR = 0;

    return;
  }

  /* Calc Motor Load-Ratio*/
  Temp = (int64_t) g_Irms10msFilQ24;    //反馈电流
  Temp = Temp * g_MotorCurGain;
  Temp = Temp >> 24;    //转换成马达电流

  Temp = Temp * 100;
  /* Calc the Deviation between Load-Ratio and 100(zoom:100) */
  I_PerTherm = Temp >> 24;
  I_PerTherm -= FuncCode_Handle.Ram.F00.F0098;    //20180516

  /* Calc the Deviation Square;Notice the sign*/
  if (I_PerTherm > 0)
    DeltaLR = I_PerTherm * I_PerTherm;    //超过驱动器电流
  else
    DeltaLR = -(I_PerTherm * I_PerTherm);    //低于驱动器电流

  /*Accumulate the Counts*/
  C_ThermCountM += DeltaLR;

  /*Judge Motor Over-Load Protection*/
  if (C_ThermCountM > (int32_t) OL_THRED)
  {
    AlarmStatusFlag.bit.Motor.MOL = OK;
  }

  if (C_ThermCountM < 0)
  {
    C_ThermCountM = 0;
  }

  FuncCode_Handle.Ram.FA0.FA058 = C_ThermCountM >> 6;    //20170312
}

/*************************************************
 Function:Check Driver&Motor Over-Load
 Description: Period:100ms;Formula:y=100/x, x=square([(LoadRatio)-100%])
 Input: No
 Return: No
 Others: be called: AlmCheck100ms()
 *************************************************/
static void AlmCheckOL(void)
{
  //AlmCheckDrvOL();
  //AlmCheckMotOL();
}
/*************************************************
 Function:Check Motor Over-Speed
 Description: Period:2ms;
 Input: Function Code: F6014
 Return: No
 Others: be called: AlmCheck2ms()
 *************************************************/
static void AlmCheckOS(void)    //10次有7次速度超过最大值，过速
{
  static uint16_t OS_scan_cnt = 0;
  static uint16_t OS_happen_cnt = 0;
  uint64_t Temp = 0;
  uint32_t SetVelocity = 0;

  /* Calc OS Protection Threshold*/
  if (FuncCode_Handle.Ram.F10.F1030 == 0)    //过速等级设定
  {
    /*OS Threshold Equal Max Velocity Zoom 1.2 IQ24(1.2) = 20132659*/
    Temp = (uint64_t) g_VelocityMaxQ24;
    SetVelocity = (uint32_t) (Temp * 20132659) >> 24;    //最高速度1.2倍
  }
  else
  {
    /*OS Threshold Equal F6014*/
    SetVelocity = FuncCode_Handle.Ram.F10.F1030;
  }

  if (AlarmStatusFlag.bit.Driver.OS == NG)
  {
    if (OS_scan_cnt > 9)
    {
      /* Scan 10 times (7 times effect)*/
      if (OS_happen_cnt > 6)
      {
        AlarmStatusFlag.bit.Driver.OS = OK;
      }
      else
      {
        AlarmStatusFlag.bit.Driver.OS = NG;
      }
      OS_scan_cnt = 0;
      OS_happen_cnt = 0;
    }
    else
    {
      OS_scan_cnt++;
      /*Judge Speed */
      if (g_EncSpeed1msFilQ16 > SetVelocity)
      {
        OS_happen_cnt++;
      }
    }
  }
  else
  {
    OS_scan_cnt = 0;
    OS_happen_cnt = 0;
  }
}

/*************************************************
 Function:Check Position Deviation Excess Protection
 Description: Period:100ms;
 Input: Function Code: F1011
 Return: No
 Others: be called: AlmCheck10ms()-100ms()
 *************************************************/
static void AlmCheckPD(void)
{
  static uint16_t PD_scan_cnt = 0;
  static uint16_t PD_happen_cnt = 0;
  uint32_t Temp = 0;

  if ((!CoreStatusFlag.bit.RUN_OK_F) || (g_UltimateCtrlMode != POSITIONCTRL))	//20171028 mxj
  {
    PD_scan_cnt = 0;
    PD_happen_cnt = 0;
    return;	//不运行的时候 不用管
  }

  Temp = FuncCode_Handle.Ram.F10.F1029;

  if (Temp == 0)
  {
    PD_scan_cnt = 0;
    PD_happen_cnt = 0;
    return;
  }

  if (AlarmStatusFlag.bit.Driver.PD == NG)
  {
    if (PD_scan_cnt > 9)
    {
      /* Scan 10 times (7 times effect)*/
      if (PD_happen_cnt > 6)
      {
        AlarmStatusFlag.bit.Driver.PD = OK;
      }
      else
      {
        AlarmStatusFlag.bit.Driver.PD = NG;
      }
      PD_scan_cnt = 0;
      PD_happen_cnt = 0;
    }
    else
    {
      PD_scan_cnt++;
      /*Judge Position Deviation */
      if (abs(g_PosDevtRegCount) > Temp)
      {
        PD_happen_cnt++;
      }
    }
  }
  else
  {
    PD_scan_cnt = 0;
    PD_happen_cnt = 0;
  }
}
/*************************************************
 Function:Check Driver Over-Temperature Protection
 Description: Period:100ms;
 Input: No
 Return: No
 Others: be called: AlmCheckOT()
 *************************************************/
static void AlmCheckDrvOT(void)
{
  static uint16_t OT_scan_cnt = 0;
  static uint16_t OT_happen_cnt = 0;
  uint32_t FcodeValue = 0;

  FcodeValue = FuncCode_Handle.Ram.F00.F0055;	//Temperature Protection Value

  if (SysTimeSequ.Start_Flg == NG)
  {
    return;
  }

}
/*************************************************
 Function:Check Motor Over-Temperature Protection
 Description: Period:100ms;
 Input: No
 Return: No
 Others: be called: AlmCheckOT()
 *************************************************/
static void AlmCheckMotOT(void)
{
  static uint16_t OT_scan_cnt = 0;
  static uint16_t OT_happen_cnt = 0;

  if (AlarmStatusFlag.bit.Motor.MOT == NG)
  {
    if (OT_scan_cnt > 9)
    {
      /* Scan 10 times (7 times effect) */
      if (OT_happen_cnt > 6)
      {
        AlarmStatusFlag.bit.Motor.MOT = OK;
      }
      else
      {
        AlarmStatusFlag.bit.Motor.MOT = NG;
      }
      OT_scan_cnt = 0;
      OT_happen_cnt = 0;
    }
    else
    {
      OT_scan_cnt++;
      /*Judge the Sample Temperature*/
      if (g_PTC10msFilQ24 > (uint32_t) MOT_THRED)
      {
        OT_happen_cnt++;
      }
    }
  }
  else
  {
    OT_scan_cnt = 0;
    OT_happen_cnt = 0;
  }
}

/*************************************************
 Function:Check Driver&Motor Temperature
 Description: Period:100ms;
 Input: Function Code: F1011
 Return: No
 Others: be called: AlmCheck100ms()
 *************************************************/
static void AlmCheckOT(void)
{
  AlmCheckDrvOT();
  AlmCheckMotOT();
}

/*************************************************
 Function:Check Deviation counter overflow protection
 Description: Period:100ms
 Input: Function Code:Max.F1011
 Return: No
 Others: be called: AlmCheck100ms()
 *************************************************/
static void AlmCheckDCOF(void)	//no used
{
  static uint16_t DCOF_scan_cnt = 0;
  static uint16_t DCOF_happen_cnt = 0;
  uint32_t Temp = 0;

  Temp = FuncCode_Handle.Max.F10.F1011;	//这个单元没用啊 奇怪了

  if (AlarmStatusFlag.bit.Driver.DCOF == NG)
  {
    if (DCOF_scan_cnt > 9)
    {
      /* Scan 10 times (7 times effect) */
      if (DCOF_happen_cnt > 6)
      {
        //			AlarmStatusFlag.bit.Driver.DCOF = OK;
      }
      else
      {
        AlarmStatusFlag.bit.Driver.DCOF = NG;
      }
      DCOF_scan_cnt = 0;
      DCOF_happen_cnt = 0;
    }
    else
    {
      DCOF_scan_cnt++;
      /*Judge the Position Deviation Register*/
      if (abs(g_PosDevtRegCount) > Temp)	//偏差长时间大于一个值，和PD故障重复
      {
        DCOF_happen_cnt++;
      }
    }
  }
  else
  {
    DCOF_scan_cnt = 0;
    DCOF_happen_cnt = 0;
  }
}

/*************************************************
 Function:Check Analog Input Excess Protection
 Description: Period:500ms
 Input: No
 Return: No
 Others: be called: AlmCheck500ms()
 *************************************************/
static void AlmCheckAIE(void)
{
  static uint16_t AIE_scan_cnt = 0;
  static uint16_t AI1E_happen_cnt = 0;
  static uint16_t AI2E_happen_cnt = 0;
  static uint16_t AI3E_happen_cnt = 0;

  if (AlarmStatusFlag.bit.Driver.AI1E || AlarmStatusFlag.bit.Driver.AI2E || AlarmStatusFlag.bit.Driver.AI3E)
  {
    AIE_scan_cnt = 0;
    AI1E_happen_cnt = 0;
    AI2E_happen_cnt = 0;
    AI3E_happen_cnt = 0;

    return;
  }

  if (AIE_scan_cnt > 9)
  {
    /* Scan 10 times (7 times effect) */
    if (AI1E_happen_cnt > 6)
    {
      AlarmStatusFlag.bit.Driver.AI1E = OK;
    }
    else
    {
      AlarmStatusFlag.bit.Driver.AI1E = NG;
    }

    if (AI2E_happen_cnt > 6)
    {
      AlarmStatusFlag.bit.Driver.AI2E = OK;
    }
    else
    {
      AlarmStatusFlag.bit.Driver.AI2E = NG;
    }

    if (AI3E_happen_cnt > 6)
    {
      AlarmStatusFlag.bit.Driver.AI3E = OK;
    }
    else
    {
      AlarmStatusFlag.bit.Driver.AI3E = NG;
    }

    AIE_scan_cnt = 0;
    AI1E_happen_cnt = 0;
    AI2E_happen_cnt = 0;
    AI3E_happen_cnt = 0;
  }
  else
  {
    AIE_scan_cnt++;
    /*Judge the AD Sample*/
    if ((g_AI1FilQ24 > AIE_THRED) || (g_AI1FilQ24 < (-AIE_THRED)))
      AI1E_happen_cnt++;
    if ((g_AI2FilQ24 > AIE_THRED) || (g_AI2FilQ24 < (-AIE_THRED)))
      AI2E_happen_cnt++;
    if ((g_AI3FilQ24 > AIE_THRED) || (g_AI3FilQ24 < (-AIE_THRED)))
      AI3E_happen_cnt++;
  }
}
/*************************************************
 Function:Emergency Stop
 Description: Period:2ms
 Input: No
 Return: No
 Others: be called: AlmCheck2ms()
 *************************************************/
static void AlmCheckES(void)
{
  if (DICtrlFlg.bit.EStop)
    AlarmStatusFlag.bit.Driver.ES = OK;
}

/*************************************************
 Function:正转极限限位
 Description: Period:2ms
 Input: No
 Return: No
 Others: be called: AlmCheck2ms()
 *************************************************/
static void WarnCheckPOT(void)
{
  if (DICtrlFlg.bit.POT && SHomeParameter.LimitMode == 0)	//20161212
    WarnStatusFlag.bit.POT = OK;
  else
    WarnStatusFlag.bit.POT = NG;
}

/*************************************************
 Function:反转极限限位
 Description: Period:2ms
 Input: No
 Return: No
 Others: be called: AlmCheck2ms()
 *************************************************/
static void WarnCheckNOT(void)
{
  if (DICtrlFlg.bit.NOT && SHomeParameter.LimitMode == 0)
    WarnStatusFlag.bit.NOT = OK;
  else
    WarnStatusFlag.bit.NOT = NG;
}

/*************************************************
 Function:Read Alarm Number
 Description: Period:100ms->mainloop
 Input: No
 Return: No
 Others: be called: AlmCheck100ms()
 *************************************************/
void AlmReadNum(void)
{
  CoreStatusFlag.bit.AlmNum = (uint8_t) g_AlmNum;
  if ((!AlarmStatusFlag.all) && (WarnStatusFlag.all == 0))	//错误标志位 20161128
  {
    /*No Alarm*/
    g_AlmNum = 0;
    return;
  }

  if (AlarmStatusFlag.bit.Motor.MDP)	//电机或驱动器代码错误  20180906
    g_AlmNum = ALM_CODE_MDP;	//38
  else if (AlarmStatusFlag.bit.Driver.FO)
    g_AlmNum = ALM_CODE_FO;
  else if (AlarmStatusFlag.bit.Motor.AEMO)
    g_AlmNum = ALM_CODE_AEMO;
  else if (AlarmStatusFlag.bit.Motor.AEBE)
    g_AlmNum = ALM_CODE_AEBE;	//E.SE17
  else if (AlarmStatusFlag.bit.Motor.AECF)
    g_AlmNum = ALM_CODE_AECF;
  else if (AlarmStatusFlag.bit.Motor.SERL)
    g_AlmNum = ALM_CODE_SERL;	//SE12
  else if (AlarmStatusFlag.bit.Motor.SEOT)
    g_AlmNum = ALM_CODE_SEOT;	//E.SE10
  else if (AlarmStatusFlag.bit.Motor.SEOS)
    g_AlmNum = ALM_CODE_SEOS;	//SE11
  else if (AlarmStatusFlag.bit.Motor.SECE)
    g_AlmNum = ALM_CODE_SECE;	//SE13
  else if (AlarmStatusFlag.bit.Motor.SETE)	//编码器种类（位数）与驱动器设定不同
    g_AlmNum = ALM_CODE_SETE;	//52 E.SE18
  else if (AlarmStatusFlag.bit.Motor.SEDC)
    g_AlmNum = ALM_CODE_SEDC;	//54 E.SE20
  else if (AlarmStatusFlag.bit.Motor.SEDE)
    g_AlmNum = ALM_CODE_SEDE;	//53 E.SE19
  else if (AlarmStatusFlag.bit.Motor.SEWE)
    g_AlmNum = ALM_CODE_SEWE;	//55 E.SE21
  else if (AlarmStatusFlag.bit.Driver.DOC)
    g_AlmNum = ALM_CODE_DOC;
  else if (AlarmStatusFlag.bit.Motor.BRP)	//制动功率不足  20190831
    g_AlmNum = ALM_CODE_BRP1;	//57
  else if (AlarmStatusFlag.bit.Driver.POV)
    g_AlmNum = ALM_CODE_POV;
  else if (AlarmStatusFlag.bit.Driver.COV)
    g_AlmNum = ALM_CODE_COV;
  else if (AlarmStatusFlag.bit.Driver.PLV)
    g_AlmNum = ALM_CODE_PLV;
  else if (AlarmStatusFlag.bit.Driver.CLV)
    g_AlmNum = ALM_CODE_CLV;
  else if (AlarmStatusFlag.bit.Driver.DOL)
    g_AlmNum = ALM_CODE_DOL;
  else if (AlarmStatusFlag.bit.Driver.OS)
    g_AlmNum = ALM_CODE_OS;
  else if (AlarmStatusFlag.bit.Driver.PD)	//位置偏差过大
    g_AlmNum = ALM_CODE_PD;
  else if (AlarmStatusFlag.bit.Driver.SD)	//速度偏差过大
    g_AlmNum = ALM_CODE_SD;
  else if (AlarmStatusFlag.bit.Driver.ES)
    g_AlmNum = ALM_CODE_ES;
  else if (AlarmStatusFlag.bit.Driver.DOT)
    g_AlmNum = ALM_CODE_DOT;
  else if (AlarmStatusFlag.bit.Driver.EE)
    g_AlmNum = ALM_CODE_EE;
  else if (AlarmStatusFlag.bit.Driver.EC)	//待添加
    g_AlmNum = ALM_CODE_EC;
  else if (AlarmStatusFlag.bit.Driver.BUS)	//母线浮动过大，缺相判断
    g_AlmNum = ALM_CODE_BUS;
  else if (AlarmStatusFlag.bit.Driver.ECE)	//中断里直接送过来
    g_AlmNum = ALM_CODE_ECE;
  else if (AlarmStatusFlag.bit.Driver.ORL)
    g_AlmNum = ALM_CODE_ORL;
  else if (AlarmStatusFlag.bit.Driver.PIFE)
    g_AlmNum = ALM_CODE_PIFE;
  else if (AlarmStatusFlag.bit.Driver.DCOF)
    g_AlmNum = ALM_CODE_DCOF;
  else if (AlarmStatusFlag.bit.Driver.AI1E)
    g_AlmNum = ALM_CODE_AI1E;
  else if (AlarmStatusFlag.bit.Driver.AI2E)
    g_AlmNum = ALM_CODE_AI2E;
  else if (AlarmStatusFlag.bit.Driver.AI3E)
    g_AlmNum = ALM_CODE_AI3E;
  else if (AlarmStatusFlag.bit.Driver.EPE)
    g_AlmNum = ALM_CODE_EPE;
  else if (AlarmStatusFlag.bit.Driver.RLE)
    g_AlmNum = ALM_CODE_RLE;
  else if (AlarmStatusFlag.bit.Driver.ECE1)	//进Z中断时，编码器脉冲值不对
    g_AlmNum = ALM_CODE_ECE1;
  else if (AlarmStatusFlag.bit.Driver.ECE2)	//编码器顺向溢出时，一直未进Z中断
    g_AlmNum = ALM_CODE_ECE2;
  else if (AlarmStatusFlag.bit.Driver.ECE3)	//断编码器
    g_AlmNum = ALM_CODE_ECE3;
  else if (AlarmStatusFlag.bit.Driver.EE1)	//第一次上电
    g_AlmNum = ALM_CODE_EE1;
  else if (AlarmStatusFlag.bit.Driver.EE2)	//ack出错
    g_AlmNum = ALM_CODE_EE2;
  else if (AlarmStatusFlag.bit.Driver.EGE)	//电子齿轮比出错
    g_AlmNum = ALM_CODE_EGE;
  else if (AlarmStatusFlag.bit.Driver.POSE)	//脉冲输出设置出错
    g_AlmNum = ALM_CODE_POSE;	//31
  else if (AlarmStatusFlag.bit.Motor.PHS)	//相序错误
    g_AlmNum = ALM_CODE_PHS;	//32
  else if (AlarmStatusFlag.bit.Motor.MOC)
    g_AlmNum = ALM_CODE_MOC;	//33
  else if (AlarmStatusFlag.bit.Motor.MOL)
    g_AlmNum = ALM_CODE_MOL;	//34
  else if (AlarmStatusFlag.bit.Motor.MOT)
    g_AlmNum = ALM_CODE_MOT;	//35
  else if (AlarmStatusFlag.bit.Motor.MC1)	//堵转锁定
    g_AlmNum = ALM_CODE_MC1;	//36
  else if (AlarmStatusFlag.bit.Motor.MC2)	//飞车
    g_AlmNum = ALM_CODE_MC2;	//37
  else if (WarnStatusFlag.bit.POT) //正向极限限位  20161128
    g_AlmNum = WARN_CODE_POT; //39
  else if (WarnStatusFlag.bit.NOT) //负向极限限位
    g_AlmNum = WARN_CODE_NOT; //40
  else if (WarnStatusFlag.bit.OL) //over-load warn
    g_AlmNum = WARN_CODE_OL; //41
  else if (WarnStatusFlag.bit.OT) //over-temperature warn
    g_AlmNum = WARN_CODE_OT; //42
  else if (WarnStatusFlag.bit.POOS) //脉冲输出过速
    g_AlmNum = WARN_CODE_POOS; //43
  else if (WarnStatusFlag.bit.AELV) //绝对值编码器电池欠压
    g_AlmNum = WARN_CODE_AELV; //44
  else if (AlarmStatusFlag.bit.Motor.CUr) //电机或驱动器代码错误
    g_AlmNum = ALM_CODE_CUR1; //56
  else if (AlarmStatusFlag.bit.Motor.OLF)		//
    g_AlmNum = ALM_CODE_OLF1;		//58
  else if (AlarmStatusFlag.bit.Motor.HOME)		//
    g_AlmNum = ALM_CODE_HOME;		//59

  if (AlarmStatusFlag.all)		//有故障发生
  {
    AlmWarnRecord(); //记录故障参数
  }
}
/*************************************************
 Function:  Alarm Clear
 Description: Period:System Loop
 Input: No
 Return: No
 Others: be called: SysMainLoop()
 *************************************************/
void AlmClr(void)
{
  if (DICtrlFlg.bit.AlmClr || ComCtrlFlg.bit.ResetFlg) //20180822
  {
    struct TimerConfig tc;

    if (AlarmStatusFlag.all)
    {
      g_reset_flag = 0xff; //复位过程中
      tc.init_ms = 250;	//延时500ms
      tc.mode = TIMER_MODE_ONESHOT;
      tc.func = ResetDelayHandle;
      TimerInit(RESETDELAY, &tc);
      TimerStart(RESETDELAY);

      if ((SeriEncoder.Operation & 0xF000) == 0
          && ((FuncCode_Handle.Ram.F10.F1040 & 0x05) == 0 || ((FuncCode_Handle.Ram.F10.F1040 & 0x0F00) >> 8) == 1))	//清除串行编码器故障
      {
        SeriEncoder.Operation = 0x6000 + (SeriEncoder.Operation & 0x0FFF);	//0、2 can    1、3、4can't
      }

      SHomeFlag.all = 0;	//20181128

      AlarmStatusFlag.all &= 0x0001;	//20180822 not clear the FO
      WarnStatusFlag.all = 0;	//CYL 20161206
    }

    ComCtrlFlg.all &= 0x02;	//除了正反转维持原来状态，其他全部置零。20181122

    CoreStatusFlag.bit.ERR_ALL_STOP_F = NG;
    AlmRecord.AlmRcdFlg = OK;	//清除故障后
  }
}

void ResetDelayHandle(uint16_t id)
{
  g_reset_flag = 0;
}

/*************************************************
 Function:Protect PU Value Calculate
 Description: Period:System Loop
 Input: No
 Return: No
 Others: be called: SysMainLoop()
 *************************************************/
void ProtectPUValueCalc(void)
{
  uint32_t FcodeValue = 0;

  /*OV PU Value Calc*/
  FcodeValue = FuncCode_Handle.Ram.F00.F0056;
  g_OVPUQ24 = FcodeValue * g_VpnVoltGain;	//53924 = 2^24/(220*sqrt(2))

  /*LV PU Value Calc*/
  FcodeValue = FuncCode_Handle.Ram.F00.F0057;
  g_LVPUQ24 = FcodeValue * g_VpnVoltGain;	//53924 = 2^24/(220*sqrt(2))

  /*Driver OC PU Value Calc*/
  FcodeValue = FuncCode_Handle.Ram.F00.F0058;
  g_DOCPUQ24 = FcodeValue * 167772;	//167772 = 2^24/100

  /*Motor OC PU Value Calc*/
  FcodeValue = FuncCode_Handle.Ram.F00.F0059;
  g_MOCPUQ24 = ((int64_t) FcodeValue * 167772 * g_MotorCurGain_1) >> 24;	//mxj
}

/*************************************************
 Function:Check SPEED Deviation Excess Protection
 Description: Period:10ms;
 Return: No
 Others: be called: AlmCheck10ms()
 *************************************************/
static void AlmCheckSD(void)	//200ms 有16个周期差值过大
{
}

/*************************************************
 Function:Protect encoder
 Description: Period:2ms
 Input: No
 Return: No
 Others: be called: 2msLoop()
 *************************************************/
void AlmCheckEnc(void)
{
  if (g_CtrlMode == 11)	//VF Control mode
    return;

  //if(MotorPara.EncMode <= 1)//20170302 mxj
  {
    EncDisconnect();	//E.ECE3
    ABErr();	//E.ECE
    ZErr();	//E.ECE2
  }
  //E.ECE1 is in the Zphase_ISR1
}

/*编码器连接断开*/
static void EncDisconnect(void)
{
  uint8_t tempUVW = 0;
//		uint32_t tempAng = 0;
  uint8_t Temp = 0;
  static uint8_t counter = 0;
  static uint8_t counter1 = 0;

  tempUVW = g_tempUVW;

  if (AlarmStatusFlag.bit.Driver.ECE3 == NG)
  {
    /* Read Encoder UVW Initial Angle */

    switch (tempUVW)
    //Detect the middle of Section
    {
    case 0:
    {
      counter++;
      if (counter >= 15) //50->15  20180719
        AlarmStatusFlag.bit.Driver.ECE3 = OK;
      break;
    }
    case 7:
    {
      counter1++;
      if (counter1 >= 15) //50->15
        AlarmStatusFlag.bit.Driver.ECE3 = OK;
      break;
    }
    default:
    {
      counter = 0;
      counter1 = 0;
      break;
    }
    }
  }
  else
  {
    switch (tempUVW)
    {
    case 0:
      break;
    case 7:
      break;
    default:
    {
      counter = 0;
      counter1 = 0;
      //AlarmStatusFlag.bit.Driver.ECE3 = NG;  //20170223 not restore
      break;
    }
    }

  }
}

/*AB相位出错*/
static void ABErr(void)
{
  static uint16_t i;
  if (i >= 500) // 1s种
  {

    if (g_ABErrCount >= 100)
    {
      AlarmStatusFlag.bit.Driver.ECE = OK;
    }
    g_ABErrCount = 0;
    i = 0;
  }
  else
    i++;

  if (!CoreStatusFlag.bit.ZPHASE_F)	//Z never detected
    return;

  if (FuncCode_Handle.Ram.F00.F0073 & 0x0002)	//bit1屏蔽E.ECE
    return;

  int32_t temp;
  static uint16_t g_ABUVWErrCount = 0;
  temp = ((g_Uvw_Theta + g_MotorInitAngleQ24) & 0x00FFFFFF) - g_EleThetaQ24;
  if (temp > 0x07FFFFF)
    temp -= 0x01000000;
  else if (temp < -0x07FFFFF)
    temp += 0x01000000;

  if (temp > 3728270)	//80 IQ(80/360)=IQ(0.2222) = 3728270
    g_ABUVWErrCount += 3;
  else if (temp < -3728270)
    g_ABUVWErrCount += 3;
  else if (g_ABUVWErrCount > 1)
    g_ABUVWErrCount -= 1;

  if (g_ABUVWErrCount > 100)	//66ms
  {
    AlarmStatusFlag.bit.Driver.ECE = OK;
    g_ABUVWErrCount = 0;
  }
}

/*Z丢失*/
static void ZErr(void)
{
  static uint32_t z_counter_old = 0;
  static int64_t pulse_start = 0;
  int64_t pulse_now = 0;
  int32_t pulse_total = 0;
  uint32_t temp;
  uint32_t FcodeValue;

  FcodeValue = FuncCode_Handle.Ram.F00.F0074 * 10000;

  if (FcodeValue <= 9999)
  {
    return;
  }

  if (!CoreStatusFlag.bit.RUN_OK_F)	//静止的时候
  {
    pulse_start = g_EncPulAllCount;
    pulse_now = g_EncPulAllCount;
    z_counter_old = g_index_counter;	//取当前的Z脉冲计数器
  }
  else
  {
    if (AlarmStatusFlag.bit.Driver.ECE2 == NG)
    {
      pulse_now = g_EncPulAllCount;
      pulse_total = pulse_now - pulse_start;
      if (abs(pulse_total) > FcodeValue)  //走了8圈
      {
        temp = g_index_counter - z_counter_old;
        if (temp == 0)
        {
          AlarmStatusFlag.bit.Driver.ECE2 = OK;
        }
        else
        {
          pulse_start = g_EncPulAllCount;
          z_counter_old = g_index_counter;
        }
      }
    }
  }
}

/*母线电压检测*/
static void AlmCheckAV()
{

}

static void AlmCheckOLF(void)
{
  static uint16_t OLF_count = 0;
  int64_t temp;

  if ((AlarmStatusFlag.bit.Motor.OLF) || (CoreStatusFlag.bit.RUN_OK_F == NG)
      || (FuncCode_Handle.Ram.F00.F0073 & 0x0001))
  {
    OLF_count = 0;
    return;
  }

  if (g_VrmsCalc > 8388608)  //IQ(0.5)
  {
    temp = g_IrmsCalc;
    temp = (temp * g_LqValueQ24) >> 24;
    //g_canview25 = temp;//20180810

    temp = (((int64_t) (abs(g_VelocityQ24Fdb)) * (g_FluxfQ24 + temp)) >> 24) + 5033164;  //IQ(0.3)

    g_canview25 = temp;  //20180810

    if (temp < g_VrmsCalc)
    {
      OLF_count++;
      if (OLF_count > 10)  //2*20=40ms->20ms
        AlarmStatusFlag.bit.Motor.OLF = OK;
    }
    else
    {
      if (OLF_count > 0)
        OLF_count--;
    }
  }
  else
  {
    OLF_count = 0;
  }

  FuncCode_Handle.Ram.FA0.FA064 = OLF_count;
}

/*************************************************
 Function:Protect encoder
 Description: Period:SysMainLoop
 Input: No
 Return: No
 Others: be called: SysMainLoop()
 *************************************************/
void AlmCheckMotorandDriver(void)
{
  uint32_t Charger_type = 0;
  //drive_level = FuncCode_Handle.Ram.F00.F0004;//驱动功率
  Charger_type = FuncCode_Handle.Ram.F00.F0002;  //电机类型

  if (Charger_type < 100)
    AlarmStatusFlag.bit.Motor.MDP = OK;
  else
    AlarmStatusFlag.bit.Motor.MDP = NG;
}

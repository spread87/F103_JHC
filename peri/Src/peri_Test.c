/*
 * peri_Test.c
 *
 * Created on: 2019-12-25
 * Author: JH-MXJ
 */
#include <main.h>			//Declarations from DAVE3 Code Generation (includes SFR declaration)
#include <peri_AlmWarnMacro.h>
#include <peri_GlobalVariablesExtern.h>

extern LPF CurCmdFil;
extern uint8_t g_reset_flag;
int32_t test0;
uint32_t test1 = 0;
uint8_t g_serve_ready = 0;
uint32_t Cnt_Stop_1ms = 0;
uint32_t Cnt_Stop_Status = 0;
void status_clr(void);
void AlmWarnRecord(void); //20161128CYL

void Test(void)
{
  static uint8_t AlmRecord_Time = 0;
  static uint16_t sWarnStatusFlagOld = 0; //20161128CYL
  uint32_t FcodeValue = 0;
  uint8_t serve_start = 0; //启动信号，来自JOG,IO口，上位机等

  FcodeValue = FuncCode_Handle.Ram.F30.F3000; //马达测试用

  if (AlarmStatusFlag.all != 0)
  {
    CoreStatusFlag.bit.ERR_ALL_STOP_F = OK; //故障输出
  }
  else
  {
    CoreStatusFlag.bit.ERR_ALL_STOP_F = NG;
  }

  if (WarnStatusFlag.all)						//20161128CYL
  {
    CoreStatusFlag.bit.WARN_ALL_F = OK;						//有警告时，仍可以运行
    if (sWarnStatusFlagOld == 0)
      ; //AlmWarnRecord(); //记录故障参数
  }
  else
  {
    CoreStatusFlag.bit.WARN_ALL_F = NG;
  }
  sWarnStatusFlagOld = WarnStatusFlag.all;

  serve_start = (FcodeValue || DICtrlFlg.bit.ServoOn || ComCtrlFlg.bit.RunFlg);						//并上控制命令字 mxj

  if (AlarmStatusFlag.all)						//故障
  {
    //IO004_SetOutputValue(IO004_Handle27,0);//拉低DRIVE信号  20180717
    CoreStatusFlag.bit.RUN0_OK_F = NG;
    g_VeloRefQ24 = 0;
    g_VelocityRefQ24 = 0;

    if (Cnt_Stop_1ms >= FuncCode_Handle.Ram.F50.F5056)						//10->20 20171108 mxj
    {
      //CoreStatusFlag.bit.RUN_OK_F = NG;
      status_clr();

      AlmRecord_Time = 0;						//mxj
      Cnt_Stop_1ms = 0;						//mxj
    }
    else
    {
      Cnt_Stop_1ms++;
    }
  }
  else						//没故障
  {
    AlmRecord.AlmRcdFlg = OK;						//没有 故障 允许更新
    if (serve_start)						//有启动信号
    {
      //IO004_SetOutputValue(IO004_Handle27,0);//拉低DRIVE信号  20180717
      CoreStatusFlag.bit.RUN0_OK_F = OK;
      //  CoreStatusFlag.bit.RUN_OK_F = OK;
      Cnt_Stop_1ms = 0;						//mxj
    }
    else						//没有启动信号 ，减速关
    {
      CoreStatusFlag.bit.RUN0_OK_F = NG;

      if (Cnt_Stop_1ms >= FuncCode_Handle.Ram.F50.F5056)						//10->20 20171112 mxj
      {
        status_clr();
        AlmRecord_Time = 0;						//mxj
        Cnt_Stop_1ms = 0;						//mxj
      }
      else
      {
        Cnt_Stop_1ms++;
      }
    }
  }
}

/*************************************************
 Function: 故障记录
 Description: 出现故障或警告时记录参数
 Input: No
 Return: No
 Others:
 *************************************************/
void AlmWarnRecord(void)						//出现故障时被调用
{
  if (AlmRecord.AlmRcdFlg)						//记录故障
  {
    AlmRecord.AlmRcdFlg = NG;						//初始化以后可以进行存储 ，或者清除故障后
    AlmRecord.AlmRefreshFlg = OK;
    AlmRecord.AlmNum = g_AlmNum;						//故障来源
    AlmRecord.AlmVbus = g_PowerVbus1msFilQ24;						//g_PowerVbus10msFilQ24->g_PowerVbus1msFilQ24
    AlmRecord.AlmIrms = g_IrmsCalc;						//g_Irms10msFilQ24;
    AlmRecord.AlmIu = g_Iu100usFilQ24;
    AlmRecord.AlmIv = g_Iv100usFilQ24;
    //AlmRecord.AlmVelo = g_Velocity10msFilQ24;
    //AlmRecord.AlmVelo = g_VelocityQ24Fil;//20190908
    AlmRecord.AlmVelo = g_VrmsCalc;
    AlmRecord.AlmIOSts = g_DIDOsts;
    AlmRecord.AlmTemp = g_Temp10msFilQ24;
    AlmRecord.AlmCoreFlg = CoreStatusFlag.all;

    AlmRecord.FA41 = FuncCode_Handle.Ram.FA0.FA041;		 //F9-04
    //AlmRecord.FA42 = FuncCode_Handle.Ram.FA0.FA042;//20190628
  }
}

void status_clr(void)		 //清除状态
{
  g_VeloRefQ24 = 0;		 //速度模式的给定值
  g_JogRefQ24 = 0;
  g_InertiaRefQ24 = 0;
  g_InertiaQ24 = 0;
  g_CurInVelocityQ24 = 0; //电流模式的速度给定值
  g_CurInVeloQ24 = 0; //电流模式的速度给定值

  g_CurrentRefQ24 = 0; //IQ给定
}

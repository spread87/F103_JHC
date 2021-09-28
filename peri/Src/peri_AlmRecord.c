/*
 * peri_AlmRecord.c
 *
 * Created on: 2015-2-25
 * Author: JH-LvSL
 */
#include <main.h>			//Declarations from DAVE3 Code Generation (includes SFR declaration)
#include <peri_GlobalVariablesExtern.h>
#include <peri_E2promHandle.h>
#include <stdlib.h>

int32_t AlarmRcdTable[90];
AlmRcd AlmRecord;

/*************************************************
 Function: Alarm Record Display
 Description: period:System Loop  10ms ->main loop
 Input: No
 Return: No
 Others: Called by SysMainLoop() 10ms
 *************************************************/
void AlmRcdDisplay(void) //根据F9-00的值更新显示RAM区
{
#if 0
	int32_t FcodeValue = 0;
//	uint16_t temp;
//    static uint16_t max =0;
	FcodeValue = (int32_t)FuncCode_Handle.Ram.F90.F9000;//应该用AlarmRcdTable【0】;

	FuncCode_Handle.Ram.F90.F9001 = AlarmRcdTable[FcodeValue * 9];
	FuncCode_Handle.Ram.F90.F9002 = AlarmRcdTable[FcodeValue * 9 + 1];
	FuncCode_Handle.Ram.F90.F9003 = AlarmRcdTable[FcodeValue * 9 + 2];
	FuncCode_Handle.Ram.F90.F9004 = AlarmRcdTable[FcodeValue * 9 + 3];
	FuncCode_Handle.Ram.F90.F9005 = AlarmRcdTable[FcodeValue * 9 + 4];
	FuncCode_Handle.Ram.F90.F9006 = AlarmRcdTable[FcodeValue * 9 + 5];
	FuncCode_Handle.Ram.F90.F9007 = AlarmRcdTable[FcodeValue * 9 + 6];
	FuncCode_Handle.Ram.F90.F9008 = AlarmRcdTable[FcodeValue * 9 + 7];
	FuncCode_Handle.Ram.F90.F9009 = AlarmRcdTable[FcodeValue * 9 + 8];
	FuncCode_Handle.Ram.F90.F9021 = AlmRecord.MaxTemp;
	FuncCode_Handle.Ram.F90.F9022 = AlmRecord.MaxVbus;
	FuncCode_Handle.Ram.F90.F9023 = AlmRecord.MaxIrms;
	FuncCode_Handle.Ram.F90.F9024 = AlmRecord.MaxVelo;

//	FuncCode_Handle.Ram.F00.F0000 = max;
#endif
}

/*************************************************
 Function: Alarm Record Refresh
 Description: period:System Loop
 Input: No
 Return: No
 Others: Called by SysMainLoop()
 *************************************************/
void AlmRcdRefresh(void)
{
  uint8_t i = 9;
  int64_t Temp = 0;
  int32_t Temp1 = 0;

  if (!SysTimeSequ.Start_Flg)
    return;

  if (g_PowerVbus1msFilQ24 < 11382612) //mxj  211V/311V = IQ(0.678)=11382612 = 364V/537V    20180427
    return;

  if (g_PowerVbus1msFilQ24 < (g_LVPUQ24)) //
    return;

  if (g_Temp10msFilQ24 > AlmRecord.MaxTemp)
  {
    AlmRecord.MaxTemp = g_Temp10msFilQ24;
    EepromWriteFcode(1590, AlmRecord.MaxTemp);
  } //记录最大温度，并存取到E2PROM 1590;

  //Temp = (int64_t)g_PowerVbus10msFilQ24 * 3111;
  if (DriverPara.RatedVol == 220)			//20180427
    Temp = (int64_t) g_PowerVbus1msFilQ24 * 3111;			//20190908 g_PowerVbus10msFilQ24 - >g_PowerVbus1msFilQ24
  else
    Temp = (int64_t) g_PowerVbus1msFilQ24 * 5374;

  Temp = Temp >> 24;
  if ((int32_t) Temp > AlmRecord.MaxVbus)
  {
    AlmRecord.MaxVbus = (int32_t) Temp;
    EepromWriteFcode(1591, AlmRecord.MaxVbus);
  }			//记录最大母线电压，并存取到E2PROM 1591;

  //Temp = abs(g_Irms10msFilQ24);
  Temp = abs(g_IrmsCalc);			//20190908
  Temp = Temp * DriverPara.RatedCur;
  Temp = Temp >> 24;
  if (Temp > AlmRecord.MaxIrms)
  {
    AlmRecord.MaxIrms = Temp;			//记录最大值
    EepromWriteFcode(1592, AlmRecord.MaxIrms);
  }

  //Temp = (int64_t)g_Velocity10msFilQ24 * MotorPara.RatedVel;
  Temp = Temp >> 24;
  if (abs(Temp) > abs(AlmRecord.MaxVelo))
  {
    AlmRecord.MaxVelo = Temp;
    EepromWriteFcode(1593, AlmRecord.MaxVelo);
  }			//记录最大速度，并存取到E2PROM 1593;

  if (AlmRecord.AlmRefreshFlg == NG)
  {
    return;
  }
  //有故障刷新标志
  Temp1 = (DriverPara.RatedCur * 1414) / 1000;
  //故障数据后移
  for (i = 9; i >= 1; i--)
  {
    AlarmRcdTable[i * 9] = AlarmRcdTable[(i - 1) * 9];
    AlarmRcdTable[i * 9 + 1] = AlarmRcdTable[(i - 1) * 9 + 1];
    AlarmRcdTable[i * 9 + 2] = AlarmRcdTable[(i - 1) * 9 + 2];
    AlarmRcdTable[i * 9 + 3] = AlarmRcdTable[(i - 1) * 9 + 3];
    AlarmRcdTable[i * 9 + 4] = AlarmRcdTable[(i - 1) * 9 + 4];
    AlarmRcdTable[i * 9 + 5] = AlarmRcdTable[(i - 1) * 9 + 5];
    AlarmRcdTable[i * 9 + 6] = AlarmRcdTable[(i - 1) * 9 + 6];
    AlarmRcdTable[i * 9 + 7] = AlarmRcdTable[(i - 1) * 9 + 7];
    AlarmRcdTable[i * 9 + 8] = AlarmRcdTable[(i - 1) * 9 + 8];
  }
  //存取当前故障
  AlarmRcdTable[0] = AlmRecord.AlmNum;			//故障代码
  if (DriverPara.RatedVol == 220)			//20180427
    Temp = (int64_t) AlmRecord.AlmVbus * 3111;
  else
    Temp = (int64_t) AlmRecord.AlmVbus * 5374;
  AlarmRcdTable[1] = (int32_t) (Temp >> 24);			//F9-02  母线电压
  Temp = (int64_t) AlmRecord.AlmIrms * DriverPara.RatedCur;
  AlarmRcdTable[2] = (int32_t) (Temp >> 24);			//F9-03实际电流

  if (AlarmStatusFlag.bit.Driver.ECE1 || AlarmStatusFlag.bit.Motor.MC1)
  {
    AlarmRcdTable[3] = AlmRecord.FA41;			//F9-04
    AlarmRcdTable[4] = AlmRecord.FA42;			//20190628
  }
  else
  {
    Temp = (int64_t) AlmRecord.AlmIu * Temp1;
    AlarmRcdTable[3] = (int32_t) (Temp >> 24);			//F9-04
    Temp = (int64_t) AlmRecord.AlmIv * Temp1;
    AlarmRcdTable[4] = (int32_t) (Temp >> 24);			//F9-05
  }
  Temp = (int64_t) AlmRecord.AlmVelo * 220;
  AlarmRcdTable[5] = (int32_t) (Temp >> 24);			//F9-06
  AlarmRcdTable[6] = AlmRecord.AlmIOSts;
  AlarmRcdTable[7] = AlmRecord.AlmTemp;
  AlarmRcdTable[8] = AlmRecord.AlmCoreFlg;

  FuncCode_Handle.E2prom.F00.F0099++;			//20161212  test
  FuncCode_Handle.Ram.F00.F0099 = FuncCode_Handle.E2prom.F00.F0099;

  FuncCode_Handle.Att.F00.F0099 |= 0x0100;
  g_FcodeChangeFlag++;

  AlmRecord.AlmRcdStgFlg = OK;			//开启存储标志
  AlmRecord.AlmRefreshFlg = NG;
}

/*************************************************
 Function: Alarm Record Storage
 Description: period:System Loop
 Input: No
 Return: No
 Others: Called by SysMainLoop()
 *************************************************/
void AlmRcdStorage(void)
{
  uint16_t i = 0;

  if (AlmRecord.AlmRcdStgFlg == NG)
  {
    return;
  }
//有存储标志
  for (i = 1500; i < 1590; i++)			//全部写进E2PROM 在中断外面无所谓
  {
    EepromWriteFcode(i, AlarmRcdTable[i - 1500]);
  }
//清除存储标志
  AlmRecord.AlmRcdStgFlg = NG;
}

/*************************************************
 Function: Alarm Record Read
 Description: period:System Init
 Input: No
 Return: No
 Others: Called by InitModule()
 *************************************************/
void AlmRcdRead(void)			//初始化读取
{
  uint16_t i;

  for (i = 1500; i < 1590; i++)
  {
    AlarmRcdTable[i - 1500] = EepromReadFcode(i);
  }

  AlmRecord.MaxTemp = EepromReadFcode(1590);
  AlmRecord.MaxVbus = EepromReadFcode(1591);
  AlmRecord.MaxIrms = EepromReadFcode(1592);
  AlmRecord.MaxVelo = EepromReadFcode(1593);
  AlmRecord.AlmRcdFlg = OK;			//允许存储标志
}

/*************************************************
 Function: Clear Alarm Record
 Description: period:System Function Code Default
 Input: No
 Return: No
 Others:
 *************************************************/
void AlmRcdClr(void)
{
  uint16_t i;

  //FuncCode_Handle.Ram.F90 = FuncCode_Handle.Init.F90;//struct value directly set,the same struct
  //FuncCode_Handle.E2prom.F90 =FuncCode_Handle.Init.F90;

  for (i = 0; i < 90; i++)
  {
    AlarmRcdTable[i] = 0;
  }

  for (i = 1500; i < 1594; i++)	//清除故障记录和最大值记录
  {
    EepromWriteFcode(i, 0);
  }

  FuncCode_Handle.E2prom.F00.F0099 = 0;	//20170223
  FuncCode_Handle.Ram.F00.F0099 = FuncCode_Handle.E2prom.F00.F0099;

  FuncCode_Handle.Att.F00.F0099 |= 0x0100;
  g_FcodeChangeFlag++;

  AlmRecord.MaxIrms = 0;	//1592
  AlmRecord.MaxTemp = 0;	//1590
  AlmRecord.MaxVbus = 0;	//1591
  AlmRecord.MaxVelo = 0;	//1593
}

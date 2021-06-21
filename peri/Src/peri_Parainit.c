/*
 * peri_Parainit.c
 *
 *  Created on: 2019-12-23
 *      Author: HeLZ
 */

#include <main.h>
#include <peri_GlobalVariablesExtern.h>
#include <stdlib.h>
#include <peri_E2promHandle.h>
#include <peri_init.h>

#define CG_Software_VERSION 1038//20200711 by mxj

void ParameterF1toF8Restore(void)
{
  uint32_t i;
  uint32_t value;

  FuncCode_Handle.Ram.F10 = FuncCode_Handle.Init.F10; //struct value directly set,the same struct
  FuncCode_Handle.E2prom.F10 = FuncCode_Handle.Init.F10;

  FuncCode_Handle.Ram.F20 = FuncCode_Handle.Init.F20; //struct value directly set,the same struct
  FuncCode_Handle.E2prom.F20 = FuncCode_Handle.Init.F20;

  FuncCode_Handle.Ram.F30 = FuncCode_Handle.Init.F30; //struct value directly set,the same struct
  FuncCode_Handle.E2prom.F30 = FuncCode_Handle.Init.F30;

  FuncCode_Handle.Ram.F50 = FuncCode_Handle.Init.F50; //struct value directly set,the same struct
  FuncCode_Handle.E2prom.F50 = FuncCode_Handle.Init.F50;

  FuncCode_Handle.Ram.F60 = FuncCode_Handle.Init.F60; //struct value directly set,the same struct
  FuncCode_Handle.E2prom.F60 = FuncCode_Handle.Init.F60;

  for (i = 1 + FCODE_F00_Num; i <= FCODE_F00ToF60_Num; i++) //F0、F9、FA组功能码不需要初始化
  {
    value = FuncCodeRead(FcodeTable[i - 1][1]); //read value from funccode_handle.e2prom
    EepromWriteFcode(i, value);
  }
}

void CGParameterRefresh(uint32_t Charger_type)
{
  uint32_t num = 0;

  uint32_t temp[32];
  uint32_t i;

  const ChargerParameter *p = &Charger_lever[0]; //20170620 mxj

  if (AlarmStatusFlag.bit.Motor.MDP) //20180906
    return;

  num = Charger_type - 101;
  if (num < 0)
    num = 0;
  else if (num > 29)
    num = 29;

  p = &Charger_lever[num]; //20170620 mxj
  temp[0] = p->RatedPow; //F0-04
  temp[1] = p->RatedCur; //F0-05
  temp[2] = p->RatedVol; //F0-06
  temp[3] = p->InputVolt; //F0-07
  temp[4] = p->Volt_L; //F0-08
  temp[5] = p->Volt_H; //F0-09
  temp[6] = p->Curr_L; //F0-10
  temp[7] = p->Curr_H; //F0-11

  for (i = 0; i < 8; i++)
  {
    EepromWriteFcode(i + 5, temp[i]);		//将功率参数写进E2PROM
    FuncCodeChange(FcodeTable[i + 4][1], temp[i], &FuncCode_Handle, NG);		//写进sram,但不写进 e2prom,只读参数此函数不能修改
  }

  EepromWriteFcode(3, Charger_type);			//存储充电机类型  F0-02
  FuncCodeChange(FcodeTable[2][1], Charger_type, &FuncCode_Handle, NG);			//写进sram,但不写进 e2prom

  FuncCode_Handle.Ram.F20.F2000 = p->Volt_DF;		//20200304 mxj
  FuncCode_Handle.E2prom.F20.F2000 = FuncCode_Handle.Ram.F20.F2000;
  FuncCode_Handle.Att.F20.F2000 |= 0x0100;
  g_FcodeChangeFlag++;

  FuncCode_Handle.Ram.F20.F2001 = p->Curr_DF;		//20200304 mxj
  FuncCode_Handle.E2prom.F20.F2001 = FuncCode_Handle.Ram.F20.F2001;
  FuncCode_Handle.Att.F20.F2001 |= 0x0100;
  g_FcodeChangeFlag++;

  FuncCode_Handle.Ram.F10.F1006 = p->Curr_LCMD;		//20200516 mxj
  FuncCode_Handle.E2prom.F10.F1006 = FuncCode_Handle.Ram.F10.F1006;
  FuncCode_Handle.Att.F10.F1006 |= 0x0100;
  g_FcodeChangeFlag++;

  FuncCode_Handle.Ram.F10.F1008 = p->Volt_H;		//20200516 mxj
  FuncCode_Handle.E2prom.F10.F1008 = FuncCode_Handle.Ram.F10.F1008;
  FuncCode_Handle.Att.F10.F1008 |= 0x0100;
  g_FcodeChangeFlag++;
}

uint8_t CGParameterLimitCheck(uint32_t Charger_type)
{
  uint32_t num = 0;

  const ChargerParameter *p = &Charger_lever[0];		//

  num = Charger_type - 101;
  if (num < 0)
    AlarmStatusFlag.bit.Motor.SETE = OK;
  else if (num > 29)
    AlarmStatusFlag.bit.Motor.SETE = OK;
  else
  {
    p = &Charger_lever[num];		//20170620 mxj
    if (FuncCode_Handle.Ram.F00.F0008 != p->Volt_L)
      AlarmStatusFlag.bit.Motor.SETE = OK;
    else if (FuncCode_Handle.Ram.F00.F0009 != p->Volt_H)
      AlarmStatusFlag.bit.Motor.SETE = OK;
    else if (FuncCode_Handle.Ram.F00.F0010 != p->Curr_L)
      AlarmStatusFlag.bit.Motor.SETE = OK;
    else if (FuncCode_Handle.Ram.F00.F0011 != p->Curr_H)
      AlarmStatusFlag.bit.Motor.SETE = OK;
  }
  return AlarmStatusFlag.bit.Motor.SETE;
}

void CGParameterInit(void)
{
  uint32_t i;
  uint32_t Charger_type;
  uint32_t value;

  FuncCode_Handle.Max.F20.F2000 = FuncCode_Handle.Ram.F10.F1008;		//输出电压上限
  FuncCode_Handle.Ram.F00.F0000 = CG_Software_VERSION;
  FuncCode_Handle.E2prom.F00.F0000 = CG_Software_VERSION;

  switch (g_SpecialSave)
  {
  case Normal:
    break;
  case ParameterInit1:
  {
    g_SpecialSave = 0;
    break;
  }
  case ParameterInit2:		//用户参数初始化，包括故障参数
  {
    ParameterF1toF8Restore();

    AlmRcdClr();		//清除

    FuncCode_Handle.Ram.F10.F1001 = 0;
    FuncCode_Handle.E2prom.F10.F1001 = 0;

    g_SpecialSave = 0;
    break;
  }
  case ParameterInit3:		//用户参数初始化，不包括故障参数
  {
    ParameterF1toF8Restore();

    FuncCode_Handle.Ram.F10.F1001 = 0;
    FuncCode_Handle.E2prom.F10.F1001 = 0;

    g_SpecialSave = 0;
    break;
  }
  case ParameterInit4:		//故障参数初始化
  {
    AlmRcdClr();		//清除

    FuncCode_Handle.Ram.F10.F1001 = 0;
    FuncCode_Handle.E2prom.F10.F1001 = 0;

    g_SpecialSave = 0;
    break;
  }
//全部参数初始化
  case ParameterInit5:		//全部参数初始化
  {
    //drive_level = FuncCode_Handle.Ram.F00.F0004;//功率
    Charger_type = FuncCode_Handle.Ram.F00.F0002;		//充电机代码 100

    FuncCode_Handle.Ram = FuncCode_Handle.Init;		//struct value directly set,the same struct
    FuncCode_Handle.E2prom = FuncCode_Handle.Init;
    HAL_Delay(500);
    //for(i=1;i<=FCODE_TOTAL_Num;i++)//FCODE_F00ToF80_Num
    for (i = 1; i <= FCODE_F00ToF60_Num; i++)		//F9、FA组功能码不需要初始化
    {
      value = FuncCodeRead(FcodeTable[i - 1][1]);		//read value from funccode_handle.e2prom
      EepromWriteFcode(i, value);
      HAL_Delay(500);
    }

    AlmRcdClr();		//清除

    CGParameterRefresh(Charger_type);				//20170105 mxj

    FuncCode_Handle.Ram.F10.F1001 = 0;
    FuncCode_Handle.E2prom.F10.F1001 = 0;

    srand(g_165AD + g_141AD);
    FuncCode_Handle.Ram.F00.F0087 = (uint32_t) rand() & 0x0000FFFF;				//20170104 mxj
    FuncCode_Handle.E2prom.F00.F0087 = FuncCode_Handle.Ram.F00.F0087;
    FuncCode_Handle.Att.F00.F0087 |= 0x0100;
    g_FcodeChangeFlag++;

    HAL_Delay(500);
    g_SpecialSave = 0;
    break;
  }
  case ParameterInit6:
  {
    FuncCode_Handle.Ram.F10.F1014 = 1;
    FuncCode_Handle.E2prom.F10.F1014 = 1;

    FuncCode_Handle.Ram.F50.F5010 = 20;
    FuncCode_Handle.E2prom.F50.F5010 = 20;
    FuncCode_Handle.Ram.F50.F5011 = 20;
    FuncCode_Handle.E2prom.F50.F5011 = 20;
    FuncCode_Handle.Ram.F50.F5012 = 20;
    FuncCode_Handle.E2prom.F50.F5012 = 20;
    FuncCode_Handle.Ram.F50.F5013 = 20;
    FuncCode_Handle.E2prom.F50.F5013 = 20;
    FuncCode_Handle.Ram.F50.F5014 = 21;
    FuncCode_Handle.E2prom.F50.F5014 = 21;
    FuncCode_Handle.Ram.F50.F5015 = 22;
    FuncCode_Handle.E2prom.F50.F5015 = 22;

    FuncCode_Handle.Ram.F50.F5032 = 20;
    FuncCode_Handle.E2prom.F50.F5032 = 20;

    FuncCode_Handle.Ram.F50.F5037 = 21;
    FuncCode_Handle.E2prom.F50.F5037 = 21;

    FuncCode_Handle.Ram.F10.F1001 = 0;
    FuncCode_Handle.E2prom.F10.F1001 = 0;
    g_SpecialSave = 0;
    break;
  }
  case ParameterInit7:
  {
    FuncCode_Handle.Ram.F50.F5010 = 0;
    FuncCode_Handle.E2prom.F50.F5010 = 0;
    FuncCode_Handle.Ram.F50.F5011 = 0;
    FuncCode_Handle.E2prom.F50.F5011 = 0;
    FuncCode_Handle.Ram.F50.F5012 = 0;
    FuncCode_Handle.E2prom.F50.F5012 = 0;
    FuncCode_Handle.Ram.F50.F5013 = 0;
    FuncCode_Handle.E2prom.F50.F5013 = 0;
    FuncCode_Handle.Ram.F50.F5014 = 0;
    FuncCode_Handle.E2prom.F50.F5014 = 0;
    FuncCode_Handle.Ram.F50.F5015 = 0;
    FuncCode_Handle.E2prom.F50.F5015 = 0;

    FuncCode_Handle.Ram.F10.F1001 = 0;
    FuncCode_Handle.E2prom.F10.F1001 = 0;
    g_SpecialSave = 0;
    break;
  }
  case ParameterInit8:
  {
    FuncCode_Handle.Ram.F50.F5010 = 20;
    FuncCode_Handle.E2prom.F50.F5010 = 20;
    FuncCode_Handle.Ram.F50.F5011 = 20;
    FuncCode_Handle.E2prom.F50.F5011 = 20;
    FuncCode_Handle.Ram.F50.F5012 = 20;
    FuncCode_Handle.E2prom.F50.F5012 = 20;
    FuncCode_Handle.Ram.F50.F5013 = 20;
    FuncCode_Handle.E2prom.F50.F5013 = 20;
    FuncCode_Handle.Ram.F50.F5014 = 20;
    FuncCode_Handle.E2prom.F50.F5014 = 20;
    FuncCode_Handle.Ram.F50.F5015 = 20;
    FuncCode_Handle.E2prom.F50.F5015 = 20;

    FuncCode_Handle.Ram.F10.F1001 = 0;
    FuncCode_Handle.E2prom.F10.F1001 = 0;
    g_SpecialSave = 0;
    break;
  }
//只改参数
  case ParameterInit10:
  {
    //drive_level = FuncCode_Handle.Ram.F00.F0004;//驱动功率
    Charger_type = FuncCode_Handle.Ram.F00.F0002;				//充电机代码

    CGParameterRefresh(Charger_type);				//20170105 mxj

    FuncCode_Handle.Ram.F10.F1001 = 0;
    FuncCode_Handle.E2prom.F10.F1001 = 0;
    g_SpecialSave = 0;
    break;
  }
  case InflexibilityChange:
  {
    g_SpecialSave = 0;
    break;
  }
  case InertiaChange:
  {
    g_SpecialSave = 0;
    break;
  }
  default:
  {
    g_SpecialSave = 0;
    break;
  }
  }
}


/**********************************************************
 Copyright (c) 2014 JIANGSU JOINHOPE ELECTRIC CO.,LTD
 FileName: peri_E2promHandle.c
 Author: LvSL
 Version :
 Date: 2014.02.17
 Description: E2prom read or write handle,include i2c communication using IO simulation
 History:
 1. Date: 2014.02.17
 Author: LvSL
 Modification:  1st Version
 2...
 ***********************************************************/
//wo plan to use p2.8 as clk;use p2.9 as data in/out
//Now we use p1.0 and p1.1 as clk line and data line Correspondingly
//And in future ,wo will use p2.8 and p2.9.
#include <main.h>
#include <peri_FuncCodeHandle.h>
#include <peri_GlobalVariablesExtern.h>
#include <stdbool.h>
#include "i2c.h"

uint32_t g_SpecialSave = 0;

/*************************************************
 Function:EEPROM write byte
 Description:
 Input: No
 Return: No
 Others: No
 *************************************************/
void EepromWriteByte(uint8_t AddH, uint8_t AddL, uint8_t Data)
{
  uint8_t data[4];

  data[0] = 0xA0;	//write mode
  data[1] = AddH;
  data[2] = AddL;
  data[3] = Data;

  MX_I2cwriteByte(data, 4);
}

/*************************************************
 Function:EEPROM write byte per page continouly
 Description:
 Input: No
 Return: No
 Others: No
 *************************************************/
void EepromPageWriteByte(uint8_t AddH, uint8_t AddL, uint8_t *table, uint8_t Length)
{
  uint8_t data[35];
  uint8_t i;

  data[0] = 0xA0; //write mode
  data[1] = AddH;
  data[2] = AddL;

  for (i = 0; i < Length; i++)
  {
    data[3 + i] = table[i];
  }

  MX_I2cwriteByte(data, Length + 3);
}

/*************************************************
 Function:EEPROM read byte
 Description:
 Input: No
 Return: No
 Others: No
 *************************************************/
uint8_t EepromReadByte(uint8_t AddH, uint8_t AddL)
{
  uint8_t data[4];

  data[0] = 0xA0; //write mode
  data[1] = AddH;
  data[2] = AddL;
  data[3] = 0xA1;	//read mode
  MX_I2cwriteByte(data, 4);
  MX_I2cReadByte(data, 1);

  return data[0];
}

/*************************************************
 Function:EEPROM read one function code (32bit)
 Description:
 Input: FcodeNum
 Return: No
 Others: No
 *************************************************/
uint32_t EepromReadFcode(uint16_t FcodeNum)
{
  uint8_t data[4];
  uint32_t value = 0;
  uint8_t AddH, AddL;

  FcodeNum = FcodeNum * 4 - 4;
  AddH = (uint8_t) ((FcodeNum >> 8) & 0xFF);
  AddL = (uint8_t) (FcodeNum & 0xFF);

  data[0] = 0xA0; //write mode
  data[1] = AddH;
  data[2] = AddL;
  data[3] = 0xA1; //read mode
  MX_I2cwriteByte(data, 4);
  MX_I2cReadByte(data, 4);

  value = (((uint32_t) data[3] << 24) & 0xff000000) | (((uint32_t) data[2] << 16) & 0xff0000)
      | (((uint32_t) data[1] << 8) & 0xff00) | ((uint32_t) data[0] & 0xff);

  return value;
}

/*************************************************
 Function:EEPROM write one function code (32bit)
 Description:
 Input: FcodeNum,FcodeValue
 Return: No
 Others: No
 *************************************************/
void EepromWriteFcode(uint16_t FcodeNum, uint32_t value)
{
  uint8_t AddH, AddL;
  uint8_t data[4];

  FcodeNum = FcodeNum * 4 - 4;
  AddH = (uint8_t) ((FcodeNum >> 8) & 0xFF);
  AddL = (uint8_t) (FcodeNum & 0xFF);

  data[0] = 0xA0; //write mode
  data[1] = AddH;
  data[2] = AddL;
  data[3] = (uint8_t) ((value >> 24) & 0xff);
  data[4] = (uint8_t) ((value >> 16) & 0xff);
  data[5] = (uint8_t) ((value >> 8) & 0xff);
  data[6] = (uint8_t) (value & 0xff);

  MX_I2cwriteByte(data, 7);
}

/*************************************************
 Function:EEPROM init
 Description:
 Input: no
 Return: No
 Others: No
 *************************************************/
void EepromInit(void)
{
  uint16_t i, tempdata;
  uint8_t Test = 0;
  uint8_t counter = 0;
  int32_t value;
  static uint8_t ErrReadValueCount = 0;

  for (counter = 0; counter < 5; counter++)	//the last address 8K e2prom,最好多读几遍，防止误操作
  {
    if (207 == EepromReadByte(31, 255))
    {
      Test = 207;
      break;
    }
  }
  g_out_counter = counter;

  /*IF eeprom is used first,need to write the FCode default value to  it*/
  if (Test != 207)
  {
    FuncCode_Handle.Ram = FuncCode_Handle.Init;	//struct value directly set,the same struct
    FuncCode_Handle.E2prom = FuncCode_Handle.Init;

    for (i = 1; i <= FCODE_F00ToF60_Num; i++)
    {
      value = FuncCodeRead(FcodeTable[i - 1][1]);	//read value from funccode_handle.e2prom
      EepromWriteFcode(i, value);
    }

    EepromWriteByte(31, 255, 207);
    AlarmStatusFlag.bit.Driver.EE1 = OK;
  }
  else
  {
    tempdata = FCODE_F00ToF60_Num;		//20161209mxj FA,F9组功能码不用读取
    for (i = 1; i <= tempdata; i++)		//FuncCodeReadDetect
    {
      value = EepromReadFcode(i);		//8ms一个，500个菜单 4s

      /*detect the function code */
      Test = FuncCodeReadDetect(FcodeTable[i - 1][1], value);
      ErrReadValueCount += Test;
      if (Test)
      {
        FuncCode_Handle.Ram.FA0.FA098 = ErrReadValueCount * 10000 + FcodeTable[i - 1][1];
        FuncCode_Handle.Ram.FA0.FA099 = value;
      }

      if (ErrReadValueCount >= 5)		//限止值比较
      {
        /* err counts >3,all function code reset default value*/
        FuncCode_Handle.Ram = FuncCode_Handle.Init;
        //FuncCode_Handle.E2prom =FuncCode_Handle.Init;//mxj 20170118 EE故障查找
        AlarmStatusFlag.bit.Driver.EE = OK;

        FuncCode_Handle.Ram.FA0.FA098 = ErrReadValueCount * 10000 + FcodeTable[i - 1][1];
        FuncCode_Handle.Ram.FA0.FA099 = value;

        break;
      }
      FuncCodeChange(FcodeTable[i - 1][1], value, &FuncCode_Handle, NG);		//the value from EEPROM give to FuncCode

      if (i <= 12)
      {
        if (i == 9)
        {
          FuncCode_Handle.Ram.F00.F0008 = value;			//20200311 mxj
          FuncCode_Handle.E2prom.F00.F0008 = FuncCode_Handle.Ram.F00.F0008;
        }
        else if (i == 10)
        {
          FuncCode_Handle.Ram.F00.F0009 = value;			//20200311 mxj
          FuncCode_Handle.E2prom.F00.F0009 = FuncCode_Handle.Ram.F00.F0009;
        }
        else if (i == 11)
        {
          FuncCode_Handle.Ram.F00.F0010 = value;			//20200311 mxj
          FuncCode_Handle.E2prom.F00.F0010 = FuncCode_Handle.Ram.F00.F0010;
        }
        else if (i == 12)
        {
          FuncCode_Handle.Ram.F00.F0011 = value;			//20200311 mxj
          FuncCode_Handle.E2prom.F00.F0011 = FuncCode_Handle.Ram.F00.F0011;
        }
      }
    }
  }
  //FuncCode_Handle.Ram.F90.F9029 = FuncCode_Handle.E2prom.F00.F0099;//20170223 mxj

  if (CGParameterLimitCheck(FuncCode_Handle.Ram.F00.F0002))			//20200311判断上限限与充电机代码是否一致。
  { //Error
    FuncCode_Handle.Min.F20.F2000 = 300;			//输出电压下限
    FuncCode_Handle.Max.F20.F2000 = 300;			//输出电压上限

    FuncCode_Handle.Min.F20.F2001 = 100;			//输出电流下限
    FuncCode_Handle.Max.F20.F2001 = 100;			//输出电流上限
  }
  else
  {
    FuncCode_Handle.Min.F20.F2000 = FuncCode_Handle.Ram.F00.F0008;			//输出电压下限
    FuncCode_Handle.Max.F20.F2000 = FuncCode_Handle.Ram.F00.F0009;			//输出电压上限

    FuncCode_Handle.Min.F20.F2001 = FuncCode_Handle.Ram.F00.F0010;			//输出电流下限
    FuncCode_Handle.Max.F20.F2001 = FuncCode_Handle.Ram.F00.F0011;			//输出电流上限

    FuncCode_Handle.Min.F10.F1006 = FuncCode_Handle.Ram.F00.F0010;			//下限区电流指令下限
    FuncCode_Handle.Max.F10.F1006 = FuncCode_Handle.Ram.F00.F0011;			//下限区电流指令上限

    FuncCode_Handle.Min.F10.F1008 = FuncCode_Handle.Ram.F00.F0008;			//电压指令上限的下限
    FuncCode_Handle.Max.F10.F1008 = FuncCode_Handle.Ram.F00.F0009;			//电压指令上限的上限
  }

  if (FuncCode_Handle.Ram.F00.F0009 <= 999)			//电压上限高于100V电流显示两位
    FuncCode_Handle.Att.FA0.FA000 |= 0x040;			//电流用三位显示  80-》40

  CoreStatusFlag.bit.EEPROMINIT_OK_F = OK;
  CoreStatusFlag.bit.ZPHASE_F = NG;
  //IO004_SetOutputValue(IO004_Handle0,0);
}

/*************************************************
 Function:Function Code to EEprom
 Description: Period: system loop
 Input: no
 Return: No
 Others: No
 *************************************************/
void FcodeValueToEEPROM(void)			//write value to e2prom
{
  uint16_t i = 0;
  uint32_t FC_Att = 0;
  uint32_t FcodeValue = 0;
  uint32_t temp;
  static uint8_t counter;

  if (g_FcodeChangeFlag == 0)
    return;

  for (i = 1; i <= FCODE_F00ToF60_Num; i++)	//FuncCode Storage FA,F9 组不保存
  {
    FC_Att = FuncCodeAttRead(FcodeTable[i - 1][1]);	//读
    if ((FC_Att & 0x100) == 0x100)
    {
      if (i == FCODE_F00_Num + 2) //F1001
      {
        g_SpecialSave = FuncCodeRead(FcodeTable[i - 1][1]); //上位机 修改 也要阻截
      }
      else //一般情况的E2PROM的更新
      {
        if (i == FCODE_F00ToF20_Num + 5) //F3004
          g_SpecialSave = InflexibilityChange;

        FcodeValue = FuncCodeRead(FcodeTable[i - 1][1]); //读功能码的值EEPROM
        temp = EepromReadFcode(i); //读出E2PROM的值
        if (FcodeValue != temp) //不相等则写
        {
          FuncCode_Handle.Ram.FA0.FA035 = 0;
          FuncCode_Handle.Ram.FA0.FA036 = 0;

          EepromWriteFcode(i, FcodeValue); //写值

          while (EepromReadFcode(i) != FcodeValue)
          {
            EepromWriteFcode(i, FcodeValue);

            counter++;
            if (counter >= 30)						//3->30
            {
              AlarmStatusFlag.bit.Driver.EE1 = OK;
            }
          }
          FuncCode_Handle.Ram.FA0.FA032 = counter;
          counter = 0;
        }

      }
      FuncCodeChangeAttClear(FcodeTable[i - 1][1]);
    }
  }
  //g_FcodeChangeFlag = 0;
  g_FcodeChangeFlag--;
}

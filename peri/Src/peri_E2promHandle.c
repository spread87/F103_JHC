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
#include "eeprom_26l64.h"
#include "i2c_bsp.h"

uint32_t g_SpecialSave = 0;

static int EepromWriteByte(uint8_t AddH, uint8_t AddL, uint8_t data);
static int EepromReadByte(uint8_t AddH, uint8_t AddL, uint16_t len, uint8_t *rx_buf);

/*************************************************
 Function:EEPROM read one function code (32bit)
 Description:
 Input: FcodeNum
 Return: No
 Others: No
 *************************************************/
uint32_t EepromReadFcode(uint16_t FcodeNum)
{
  uint32_t value = 0;
  uint8_t AddH, AddL;

  FcodeNum = FcodeNum * 4 - 4;
  AddH = (uint8_t) ((FcodeNum >> 8) & 0xFF);
  AddL = (uint8_t) (FcodeNum & 0xFF);

  EepromReadByte(AddH, AddL, 4, &value);

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
  data[0]=(uint8_t)(value&0x000000FF);
  data[1]=(uint8_t)((value>>8)&0x000000FF);
  data[2]=(uint8_t)((value>>16)&0x000000FF);
  data[3]=(uint8_t)((value>>24)&0x000000FF);

  EepromWriteByte(AddH,AddL,data[0]);
  EepromWriteByte(AddH,AddL,data[1]);
  EepromWriteByte(AddH,AddL,data[2]);
  EepromWriteByte(AddH,AddL,data[3]);
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
  int n = 0;

  IIC_init( );

  while (!Isok_i2c_Device())
    if (n++ > 500)
      udelay(2000);

  for (counter = 0; counter < 5; counter++)	//the last address 8K e2prom,????????????????????????????????????
  {
    EepromReadByte(31, 255, 1, &Test);

    if (207 == Test)
    {
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
    tempdata = FCODE_F00ToF60_Num;		//20161209mxj FA,F9????????????????????????
    for (i = 1; i <= tempdata; i++)		//FuncCodeReadDetect
    {
      value = EepromReadFcode(i);		//8ms?????????500????????? 4s

      /*detect the function code */
      Test = FuncCodeReadDetect(FcodeTable[i - 1][1], value);
      ErrReadValueCount += Test;
      if (Test)
      {
        FuncCode_Handle.Ram.FA0.FA098 = ErrReadValueCount * 10000 + FcodeTable[i - 1][1];
        FuncCode_Handle.Ram.FA0.FA099 = value;
      }

      if (ErrReadValueCount >= 5)		//???????????????
      {
        /* err counts >3,all function code reset default value*/
        FuncCode_Handle.Ram = FuncCode_Handle.Init;
        //FuncCode_Handle.E2prom =FuncCode_Handle.Init;//mxj 20170118 EE????????????
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

  if (CGParameterLimitCheck(FuncCode_Handle.Ram.F00.F0002))			//20200311????????????????????????????????????????????????
  { //Error
    FuncCode_Handle.Min.F20.F2000 = 300;			//??????????????????
    FuncCode_Handle.Max.F20.F2000 = 300;			//??????????????????

    FuncCode_Handle.Min.F20.F2001 = 100;			//??????????????????
    FuncCode_Handle.Max.F20.F2001 = 100;			//??????????????????
  }
  else
  {
    FuncCode_Handle.Min.F20.F2000 = FuncCode_Handle.Ram.F00.F0008;			//??????????????????
    FuncCode_Handle.Max.F20.F2000 = FuncCode_Handle.Ram.F00.F0009;			//??????????????????

    FuncCode_Handle.Min.F20.F2001 = FuncCode_Handle.Ram.F00.F0010;			//??????????????????
    FuncCode_Handle.Max.F20.F2001 = FuncCode_Handle.Ram.F00.F0011;			//??????????????????

    FuncCode_Handle.Min.F10.F1006 = FuncCode_Handle.Ram.F00.F0010;			//???????????????????????????
    FuncCode_Handle.Max.F10.F1006 = FuncCode_Handle.Ram.F00.F0011;			//???????????????????????????

    FuncCode_Handle.Min.F10.F1008 = FuncCode_Handle.Ram.F00.F0008;			//???????????????????????????
    FuncCode_Handle.Max.F10.F1008 = FuncCode_Handle.Ram.F00.F0009;			//???????????????????????????
  }

  if (FuncCode_Handle.Ram.F00.F0009 <= 999)			//??????????????????100V??????????????????
    FuncCode_Handle.Att.FA0.FA000 |= 0x040;			//?????????????????????  80-???40

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

  for (i = 1; i <= FCODE_F00ToF60_Num; i++)	//FuncCode Storage FA,F9 ????????????
  {
    FC_Att = FuncCodeAttRead(FcodeTable[i - 1][1]);	//???
    if ((FC_Att & 0x100) == 0x100)
    {
      if (i == FCODE_F00_Num + 2) //F1001
      {
        g_SpecialSave = FuncCodeRead(FcodeTable[i - 1][1]); //????????? ?????? ????????????
      }
      else //???????????????E2PROM?????????
      {
        if (i == FCODE_F00ToF20_Num + 5) //F3004
          g_SpecialSave = InflexibilityChange;

        FcodeValue = FuncCodeRead(FcodeTable[i - 1][1]); //??????????????????EEPROM
        temp = EepromReadFcode(i); //??????E2PROM??????
        if (FcodeValue != temp) //???????????????
        {
          FuncCode_Handle.Ram.FA0.FA035 = 0;
          FuncCode_Handle.Ram.FA0.FA036 = 0;

          EepromWriteFcode(i, FcodeValue); //??????

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

static int EepromWriteByte(uint8_t AddH, uint8_t AddL, uint8_t data)
{
  uint8_t tx_buf[3];
  int ret;
  uint8_t temp = 0;

  tx_buf[0] = AddH;
  tx_buf[1] = AddL;
  tx_buf[2] = data;

  /* setp 1. read current addr value */
  {
    IIC_Read_Buffer(2, tx_buf, 1, &temp);
    if (temp == data)
      ret = 1;
    else
      ret = IIC_Write_Buffer(3, tx_buf);
    IIC_Read_Buffer(2, tx_buf, 1, &temp);
    if (temp == data)
      ret = 1;
    else
      ret = 0;
  }

  return !ret;
}

static int EepromReadByte(uint8_t AddH, uint8_t AddL, uint16_t len, uint8_t *rx_buf)
{
  uint8_t tx_buf[2];
  int ret = 0;

  tx_buf[0] = AddH;
  tx_buf[1] = AddL;

  ret = IIC_Read_Buffer(2, tx_buf, len, rx_buf);

  return !ret;
}

/*
 *  peri_Init.c
 *
 *  Created on: 2014-2-19
 *  Author: JH-LvSL
 */

#include <main.h>			//Declarations from DAVE3 Code Generation (includes SFR declaration)
//#include <peri_E2promHandle.h>
#include <peri_FuncCodeHandle.h>
#include <peri_GlobalVariablesExtern.h>
#include <peri_Init.h>
//#include "USBVC001.h"

void InitModule(void)
{
  //IO004_SetOutputValue(IO004_Handle26,0);//�ø�  test

  //IO004_SetOutputValue(IO004_Handle27,0);//mxj 20170120  disable pwm
  //IO004_SetOutputValue(IO004_Handle27,1);//mxj 20170120  enable pwm
  //IO004_SetOutputValue(IO004_Handle27,0);//mxj 20170120  disable pwm
  //IO004_SetOutputValue(IO004_Handle26,!(MX_GPIO_ReadPin(IO004_Handle26)));//*********************

  PanelInit();		//panel shift clk1,clk2

  FuncCodeInit();		//code init��min,max,att

//		NVIC_DisableIRQ(64);

  //EepromInit();//FuncCode init ;from table or e2prom

//		NVIC_EnableIRQ(64);

  InitScanDiSwitch();

  //InitAdcFilter();//IU,IV,BUS gain and filter time

  //ADCInit();

  //CCU8Init();//CCU80 PWM,output,PINS:p0.0-p0.5

  //CCU4Init();//ccu41,input p2.4-p2.5    ccu42:output p3.3-p3.5

  //IO004_SetOutputValue(IO004_Handle27,1);//mxj 20180713  enable pwm

  //AlmRcdRead();//read alarm

  //IO004_SetOutputValue(IO004_Handle27,1);//mxj 20180713  enable pwm

  //UartEncoderInit();//23Bit/17Bit ENCODER

  UARTInit();//uart configure

  //CANInit(); //20170418 CYL

  //IO004_SetOutputValue(IO004_Handle26,0);//�ø�  test

  //IO004_SetOutputValue(IO004_Handle27,1);//mxj 20170120  enable pwm

  //NVICInit();//have changed some,�ж�ʹ��

  g_LEDErrDisplayMode = (FuncCode_Handle.Ram.F10.F1049 & 0x0F00) >> 8;
  SysTimeSequ.Sec10_Flg = OK;		//
}



/*
 *  peri_Init.c
 *
 *  Created on: 2014-2-19
 *  Author: JH-LvSL
 */

#include <main.h>			//Declarations from DAVE3 Code Generation (includes SFR declaration)
#include <peri_E2promHandle.h>
#include <peri_FuncCodeHandle.h>
#include <peri_GlobalVariablesExtern.h>
#include <peri_Init.h>
//#include "USBVC001.h"

void InitModule(void)
{
  PanelInit();		//panel shift clk1,clk2
  FuncCodeInit();		//code init，min,max,att
  //EepromInit();		//FuncCode init ;from table or e2prom
  InitScanDiSwitch();

  AlmRcdRead();		//read alarm
  UARTInit();		//uart configure
  CANInit(); //20170418 CYL

  g_LEDErrDisplayMode = (FuncCode_Handle.Ram.F10.F1049 & 0x0F00) >> 8;
  SysTimeSequ.Sec10_Flg = OK;		//
}

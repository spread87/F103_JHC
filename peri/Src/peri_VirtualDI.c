/*
 * peri_VirtualDI.c
 *
 *  Created on: 2015-3-25
 *      Author: JH-LvSL
 */
#include <main.h>			//Declarations from DAVE3 Code Generation (includes SFR declaration)
#include <peri_GlobalVariablesExtern.h>
#include <peri_SwScan.h>
#include <peri_TerminalDI.h>
#include <peri_VirtualDI.h>

/*************************************************
  Function:  DI Function Handle
  Description:Period:1ms
  Input: No
  Return: No
  Others: Be Called sys1msHandle()
*************************************************/
void FuncDIHandle(void)
{
  //DIFuncHandle(VirtualDISynthesis);
  g_DIsts = DI1.flg.bit.DI_sta_F | (DI2.flg.bit.DI_sta_F << 1) | (DI3.flg.bit.DI_sta_F << 2)
      | (DI4.flg.bit.DI_sta_F << 3);

  TerminalDIHandle();//IO口输入处理和各功能标志位生成
  VirtualDIHandle();//根据DI1-DI9来确定上位机功能 ，不能支持新的功能
}

void VirtualDIHandle(void)
{
    DICtrlFlg.all=TerminalDICtrlFlg.all;//temp;
}

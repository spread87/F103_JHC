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
    g_DIsts = DI1.flg.bit.DI_sta_F | (DI2.flg.bit.DI_sta_F << 1) |(DI3.flg.bit.DI_sta_F << 2) |(DI4.flg.bit.DI_sta_F << 3) |	\
    			(DI5.flg.bit.DI_sta_F << 4) |(DI6.flg.bit.DI_sta_F << 5) |(DI7.flg.bit.DI_sta_F << 6) |(DI8.flg.bit.DI_sta_F << 7) | \
    			(DI9.flg.bit.DI_sta_F << 8);//20190922

	TerminalDIHandle();//IO�����봦��͸����ܱ�־λ����
	VirtualDIHandle();//����DI1-DI9��ȷ����λ������ ������֧���µĹ���
}

void VirtualDIHandle(void)
{
    DICtrlFlg.all=TerminalDICtrlFlg.all;//temp;
}






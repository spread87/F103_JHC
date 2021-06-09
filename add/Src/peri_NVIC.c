/*
 * peri_NVIC.c
 *
 * Created on: 2014-10-27
 * Author: LvSL
 *
 */
//#include <XMC4400.h>		//SFR declarations of the selected device
#include <main.h>			//Declarations from DAVE3 Code Generation (includes SFR declaration)



/*************************************************
  Function: Setup Interrupt Module Register
  Description:Period:Init
  Input: No
  Return: No
  Others: Be Called InitModule()
*************************************************/
void NVICInit(void)
{
	NVIC_SetPriority(63, 10);	//IRQ 63,level 10,	Trap_Protection_INT     CCU80_3_IRQHandler
	NVIC_EnableIRQ(63);
	//IO004_SetOutputValue(IO004_Handle26,1);//�ø�  test

	//NVIC002_EnableIRQ(&NVIC002_Handle0);//100us  ID=57 Priority = 30  ���� EEPROM���׳�����

	//NVIC002_EnableIRQ(&NVIC002_Handle11);//uart 485�����ж�  20161227 mxj
	//NVIC002_EnableIRQ(&NVIC002_Handle9);//phase_err ID=69 Priority = 34

}



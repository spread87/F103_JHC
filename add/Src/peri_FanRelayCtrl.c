/*
 * peri_FanRelayCtrl.c
 *
 *  Created on: 2014-7-10
 *      Author: JH-LvSL
 */
#include <main.h>			//Declarations from DAVE3 Code Generation (includes SFR declaration)
#include <peri_GlobalVariablesExtern.h>

//IO004_SetOutputValue(Handle,Value)
//Fan,P5.7: IO004_Handle24
//Relay,P5.0: IO004_Handle25

//#define 	RELAYRUN				150L
//#define		RELAYRUNQ24			8091905//(RELAYRUN << 24)
//#define 	RELAYSTOP				130L
//#define		RELAYSTOPQ24		7012984//(RELAYSTOP << 24)


/*************************************************
  Function: Fan Control
  Description:Period:Main Loop
  Input: No
  Return: No
  Others: Be Called SysMainLoop()
*************************************************/
void FanCtrl(void)
{
	int32_t FcodeValue = 0;

	FcodeValue = (int32_t)FuncCode_Handle.Ram.F00.F0054;

	if(g_Temp10msFilQ24 >= FcodeValue)
		IO004_SetOutputValue(IO004_Handle24,1);//High Level Valid
	else if(g_Temp10msFilQ24 < (FcodeValue-5))
		IO004_SetOutputValue(IO004_Handle24,0);//Low Level Invalid
}

void RelayCtrl(void)
{
	static uint32_t Relay_OK_Timer = 0;

	//if(g_PowerVbus10msFilQ24 > RELAYRUNQ24)
	if(g_PowerVbus10msFilQ24 > g_RelayrunQ24)//20170620 mxj
	{
		//if(Relay_OK_Timer > 100)
		if((Relay_OK_Timer >= 200)&&(SysTimeSequ.Sec10_Flg == OK))
		{
			IO004_SetOutputValue(IO004_Handle25,1);//High Level Valid
		}
		else
		{
			if(Relay_OK_Timer <= 200)
				Relay_OK_Timer ++;
			return;
		}

		//if(Relay_OK_Timer > 200)
		if(Relay_OK_Timer > 600)
		{
			//Relay_OK_Timer = 0;
			//CoreStatusFlag.bit.RELAY_OK_F = SeriEncoder.NoFRCnt==0?OK:NG;//CYL 20170405
			CoreStatusFlag.bit.RELAY_OK_F = (SeriEncoder.NoFRCnt<=1)?OK:NG;//20180912
		}
		else
		{
			Relay_OK_Timer ++;
		}
	}//else if(g_PowerVbus10msFilQ24 < RELAYSTOPQ24)
	else if(g_PowerVbus10msFilQ24 < g_RelaystopQ24)//20170620 mxj
	{
		IO004_SetOutputValue(IO004_Handle25,0);//Low Level Invalid
		Relay_OK_Timer = 0;
		CoreStatusFlag.bit.RELAY_OK_F = NG;
	}
}







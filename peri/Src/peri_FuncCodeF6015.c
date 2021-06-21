/*
 * peri_FuncCodeF6015.c
 *
 *  Created on: 2014-7-24
 *      Author: JH-LvSL
 */
#include <main.h>			//Declarations from DAVE3 Code Generation (includes SFR declaration)
#include <peri_GlobalVariablesExtern.h>
#include <stdlib.h>

int32_t g_LEDVelocityCmd;
int32_t g_LEDPositionCmd;
int32_t g_LEDTorqueCmd;
int32_t g_LEDPosiDev;
int32_t g_LEDLoadRated;
int32_t g_LEDTem;
int32_t g_LEDEncDev;
int32_t g_LEDAI1;
int32_t g_LEDAI2;
int32_t g_LEDAI3;
int32_t g_LEDVbus;
int32_t g_LEDCurrent;
int32_t g_LEDRunTime;

/*************************************************
  Function: Function Code F6015 Handle
  Description:Period:Main Loop
  Input: No
  Return: No
  Others: Be Called SysMainLoop()
*************************************************/
void FuncCodeF6015Init(void)
{
	//g_RunTime = FuncCode_Handle.Ram.F00.F0013;//这不是额定力矩吗
}

/*************************************************
  Function: Function Code F6015 Velocity Calculate
  Description:Period:Main Loop
  Input: No
  Return: No
  Others: Be Called FuncCodeF6015Handle()
*************************************************/
int32_t FuncCodeF6015VeloCalc(void)
{
	static uint16_t i = 0;
	static int64_t Sum = 0;
	static int32_t VelocityValue = 0;
	static int32_t VelocityValue_Led = 0;

	if(i<256)//256->512
	{
		i++;
		//Sum += (int64_t)g_Velocity10msFilQ24 * MotorPara.RatedVel;;
		Sum += (int64_t)g_Velocity10msFilQ24;
	}
	else
	{
		i = 0;

		//VelocityValue_Led = (((int64_t)Sum* MotorPara.RatedVel)>>32);
		Sum = 0;
		if(abs(VelocityValue_Led) <= 4)//2->4 20161201
			VelocityValue_Led = 0;
	}

	return VelocityValue_Led;

}

/*************************************************
  Function: Function Code F6015 Vbus Calculate
  Description:Period:Main Loop
  Input: No
  Return: No
  Others: Be Called FuncCodeF6015Handle()
*************************************************/
int32_t FuncCodeF6015VbusCalc(void)
{
	static uint8_t i = 0;
	static int64_t Sum = 0;
	static int32_t VbusValue = 0;

	if(i<256)
	{
		i++;
		//Sum += (int64_t)g_PowerVbus10msFilQ24 * 3110;
		Sum += (int64_t)g_PowerVbus10msFilQ24;
	}
	else
	{
		//VbusValue = (int32_t)(Sum >> 30);
		//VbusValue = (int32_t)((Sum * 3110) >> 30);
		VbusValue = (int32_t)((Sum * 3110) >> 32);
		i = 0;
		Sum = 0;
	}

	return VbusValue;

}
/*************************************************
  Function: Function Code F6015 Handle
  Description:Period:Main Loop
  Input: No
  Return: No
  Others: Be Called SysMainLoop()
*************************************************/
void FuncCodeF6015Handle(void)
{
	int64_t Temp = 0;

	/*LED Display: Motor Velocity Calc*/
	//g_Velocity10times = FuncCodeF6015VeloCalc();//F6-15 = 0
	//g_Velocity10times = FuncCode_Handle.Ram.FA0.FA000;

	if(abs((int32_t)FuncCode_Handle.Ram.FA0.FA000) <= 4)//2->4 20161201
		g_Velocity10times = 0;
	else
		g_Velocity10times = FuncCode_Handle.Ram.FA0.FA000;

	/*LED Display: Velocity Command Calc*/
	//Temp = (int64_t)g_VelocityRefQ24 * MotorPara.RatedVel;
//	Temp = Temp * 10;
	//g_LEDVelocityCmd = (int32_t)(Temp >> 24);//F6-15 = 1
	g_LEDVelocityCmd = FuncCode_Handle.Ram.FA0.FA001;

	/*LED Display: Position Command Calc*/
	g_LEDPositionCmd = g_PulInPosition;		//F6-15 = 2

	/*LED Display:Torque Command Calc*/
	//Temp = (int64_t)g_CurrentRefQ24 * 1000;
	//g_LEDTorqueCmd = (int32_t)(Temp >> 24);//F6-15 = 3
	g_LEDTorqueCmd = FuncCode_Handle.Ram.FA0.FA003;

	/*LED Display: Position Command Deviation*/
	g_LEDPosiDev = abs(g_PosDevtRegCount);//F6-15 = 4

	/*LED Display: Load Rated*/
	//g_IrmsAvgFilZoom10/g_LEDLoadRated
	//Temp = (int64_t)g_IrmsAvgFilZoom10 * 1000;
	//g_LEDLoadRated =(int32_t)(Temp >> 24);//F6-15 = 5
	g_LEDLoadRated = FuncCode_Handle.Ram.FA0.FA004;

	/*LED Display:  Temperature*/
	//g_LEDTem = g_Temp10msFilQ24;//F6-15 = 6
	g_LEDTem = FuncCode_Handle.Ram.FA0.FA011;

	/*LED Display: Encoder Position Deviation*/
	g_LEDEncDev = abs(g_PosDevtRegCount);//F6-15 = 7

	/*LED Display: Analog Output 1*/
	//Temp = (int64_t)ExAI.AI1Q24 * 1000;
	//g_LEDAI1 = (int32_t)(Temp >> 24);//F6-15 = 8
	g_LEDAI1 = FuncCode_Handle.Ram.FA0.FA016;

	/*LED Display: Analog Output 2*/
	//Temp = (int64_t)ExAI.AI2Q24 * 1000;
	//g_LEDAI2 = (int32_t)(Temp >> 24);//F6-15 = 9
	g_LEDAI2 = FuncCode_Handle.Ram.FA0.FA017;

	/*LED Display: Analog Output 3*/
	//Temp = (int64_t)ExAI.AI3Q24 * 1000;
	//g_LEDAI3 = (int32_t)(Temp >> 24);//F6-15 = 10
	g_LEDAI3 = FuncCode_Handle.Ram.FA0.FA018;

	/*LED Display: Vbus*/
	//g_LEDVbus = FuncCodeF6015VbusCalc();//F6-15 = 11
	g_LEDVbus = FuncCode_Handle.Ram.FA0.FA006;

	/*LED Display: Current Output*/
	//g_LEDCurrent = g_IrmsAvgFilZoom10;//F6-15 = 12
	g_LEDCurrent =FuncCode_Handle.Ram.FA0.FA002;

	/*LED Display: Run Time*/
	//Temp = (int64_t)g_RunTime;
	//g_LEDRunTime = (int32_t)(Temp/10);//F6-15 = 13
	g_LEDRunTime = FuncCode_Handle.Ram.FA0.FA020;
}
/*************************************************
  Function: Run Time Statistics
  Description:Period:10ms
  Input: No
  Return: No
  Others: Be Called peri_10ms_periodic_func_table[]
*************************************************/
void RunTimeStatistics(void)
{
	static uint16_t RunTime10msCount = 0;
	static uint16_t PowerTime10msCount = 0;

	if(CoreStatusFlag.bit.RUN_OK_F)
	{
		RunTime10msCount++;

		if(RunTime10msCount >= 36000)	//0.1hour
		{
			RunTime10msCount = 0;
			g_RunTime++;
		}
	}

	PowerTime10msCount++;
	//if(PowerTime10msCount >= 36000)	//0.1hour
	if(PowerTime10msCount >= 6000)	//1mim
	{
		PowerTime10msCount = 0;
		g_PowerUpTime++;
	}
}

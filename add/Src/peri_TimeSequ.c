/*
 * peri_TimeSequ.c
 *
 *  Created on: 2014-4-28
 *      Author: JH-LvSL
 */
#include <main.h>			//Declarations from DAVE3 Code Generation (includes SFR declaration)
#include <peri_GlobalVariablesExtern.h>

// IO004_Handle10 p2.2
// IO004_SetOutputValue(Handle,Value)

int32_t g_VdcBrakeQ24 = 0;

#define 		EnableBrake			IO004_SetOutputValue(IO004_Handle10,1)
#define		DisableBrake		IO004_SetOutputValue(IO004_Handle10,0)
#define 		VDCBRAKEQ24			(uint32_t)19412645	//360v (311<=> 16777216)
#define		VDCBRAKEHYSQ24		(uint32_t)18334164	//340v (311<=> 16777216)

void TimeSequServoOffProcess(void);
static void TimeSequServoOffMode0(int32_t *PosDevtReg,DICtrl *p,CoreStatusFlg *v);
static void TimeSequServoOffMode1(int32_t *PosDevtReg,DICtrl  *p,CoreStatusFlg *v);

void (*const  ServoOffTimeSequ[])(int32_t *PosDevtReg,DICtrl *p,CoreStatusFlg *v)=
{
		TimeSequServoOffMode0,
		TimeSequServoOffMode1,
		NULL_FUNC
};


void Dcbus_Break_Calc(void)//20171129 mxj
{
	//static uint32_t Breakon_time = 0;
	//static uint32_t Breakoff_time = 0;
	int32_t FcodeValue = 0;
	int32_t FcodeValue1 = 0;
	//uint32_t Breakon_time_Inc_Step = 0;

	FcodeValue = (int32_t)FuncCode_Handle.Ram.F10.F1037;
	if(FcodeValue == 3)//�����ƶ�������Ч���ƶ�������F1-38����ó�
	{
		FcodeValue1 = (int32_t)FuncCode_Handle.Ram.F10.F1038;//�����ƶ����蹦��
		FcodeValue1 += (FcodeValue1>>1);//�����ƶ����蹦��ԣ������0.5��  20190831
		FcodeValue1 *= (int32_t)FuncCode_Handle.Ram.F10.F1039;//�����ƶ�������ֵ

		FcodeValue = (int32_t)FuncCode_Handle.Ram.F00.F0061;//�ƶ���ѹ
		FcodeValue *= FcodeValue;

		FcodeValue = FcodeValue/(FcodeValue1+1);

		FuncCode_Handle.Ram.F00.F0062 = (uint32_t)FcodeValue;
		FuncCode_Handle.E2prom.F00.F0062 = FuncCode_Handle.Ram.F00.F0062;
	}
	else if(FcodeValue == 0)
	{
		if(DriverPara.RatedPow < 700)//Size A��׼���ƶ����裬������Ϊ0���ݶ�Ϊ10
			FuncCode_Handle.Ram.F00.F0062 = 1;//20180905 10->1
		else if(DriverPara.RatedPow < 2300)//Size C
			//FuncCode_Handle.Ram.F00.F0062 = 50;
			FuncCode_Handle.Ram.F00.F0062 = 54;//20181105
		else//Size E
			FuncCode_Handle.Ram.F00.F0062 = 100;

		FuncCode_Handle.E2prom.F00.F0062 = FuncCode_Handle.Ram.F00.F0062;
	}
}
/*************************************************
  Function: Time Sequence Process
  Description: Period:1ms
  Input: No
  Return: No
  Others: Be Called peri_1ms_periodic_func_table[];
*************************************************/
void Dcbus_Break(void)
{
	static uint32_t Breakon_time = 0;
	static uint32_t Breakoff_time = 0;
	int32_t FcodeValue = 0;
	uint32_t Breakon_time_Inc_Step = 0;

	Breakon_time_Inc_Step = (uint32_t)FuncCode_Handle.Ram.F00.F0062;

	FcodeValue = (int32_t)FuncCode_Handle.Ram.F00.F0061;

	//g_VdcBrakeQ24 = FcodeValue * 53946;//53946 = 2^24/311
	g_VdcBrakeQ24 = FcodeValue * g_VpnVoltGain;//53946 = 2^24/311

	if(g_PowerVbus1msFilQ24 > g_VdcBrakeQ24)
	{
		if((Breakon_time <= 19800))//20181102  0.2s
		{
			EnableBrake;
			CoreStatusFlag.bit.EX_BRAKE_F = OK;
		}
	}
	else if(g_PowerVbus1msFilQ24 < (g_VdcBrakeQ24 - g_VpnVoltGain*12)) //370V�ر�  20181107
	{
		DisableBrake;
		CoreStatusFlag.bit.EX_BRAKE_F = NG;
	}

	if(CoreStatusFlag.bit.EX_BRAKE_F == OK)
	{
		if(Breakon_time < 400000)//<100S->10S
		{
			Breakon_time += Breakon_time_Inc_Step;//20170310 mxj
		}

		//if(Breakon_time >= 30000)// >=10S
		if(Breakon_time >= 20002)// >=5S  20170109  ->0.5S
		{
			AlarmStatusFlag.bit.Motor.BRP = OK;//20171129
			DisableBrake;
			CoreStatusFlag.bit.EX_BRAKE_F = NG;
			//Breakoff_time = 0;
		}

		Breakoff_time = 0;

	}
	else
	{
		if(Breakon_time > 0)
		{
			Breakon_time --;
		}

		if(Breakoff_time <= 99998)
		{
			Breakoff_time++;
		}
	}

	g_canview16 = Breakon_time * 839;//16777216*2/40000=839
	//g_canview14 = Breakoff_time * 839;

	FuncCode_Handle.Ram.FA0.FA062 = Breakon_time>>1;//20170312
	//FuncCode_Handle.Ram.FA0.FA062 = Breakon_time;//20170312
	FuncCode_Handle.Ram.FA0.FA063 = Breakoff_time;//20170312

}
/*************************************************
  Function: Time Sequence Process
  Description: Period:1ms
  Input: No
  Return: No
  Others: Be Called peri_1ms_periodic_func_table[];
*************************************************/
void TimeSequProcess(void)
{
	TimeSequServoOffProcess();
}
/*************************************************
  Function: Time Sequence On Servo Off Process
  Description: Period:1ms
  Input: F6011
  Return: No
  Others: Be Called peri_1ms_periodic_func_table[];
*************************************************/
void TimeSequServoOffProcess(void)
{
	uint32_t temp;
	void (*TimeSequServoOff)(int32_t *,DICtrl *,CoreStatusFlg *);

	temp = FuncCode_Handle.Ram.F60.F6011;

	TimeSequServoOff = ServoOffTimeSequ[temp];

	(*TimeSequServoOff)(&g_PosDevtReg,&DICtrlFlg,&CoreStatusFlag);

}
/*************************************************
  Function: Time Sequence On Servo-Off Process When Mode 0
  Description: Period:1ms;F6011 = 0
  Input: g_PosDevtReg,&DICtrlFlg,&CoreStatusFlag,g_PowerVbus1msFilQ24
  Return: No
  Others: Be Called TimeSequServoOffProcess();
*************************************************/
static void TimeSequServoOffMode0(int32_t *PosDevtReg,DICtrl  *p,CoreStatusFlg *v)
{
}

/*************************************************
  Function: Time Sequence On Servo-Off Process When Mode 0
  Description: Period:1ms;F6011 = 1
  Input: g_PosDevtReg,&DICtrlFlg,&CoreStatusFlag,g_PowerVbus1msFilQ24
  Return: No
  Others: Be Called TimeSequServoOffProcess();
*************************************************/
static void TimeSequServoOffMode1(int32_t *PosDevtReg,DICtrl  *p,CoreStatusFlg *v)
{

}






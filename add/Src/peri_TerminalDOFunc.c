/*
 * peri_TerminalDOFunc.c
 *
 *  Created on: 2014-6-16
 *      Author: JH-LvSL
 */
#include <main.h>			//Declarations from DAVE3 Code Generation (includes SFR declaration)
#include "peri_PosiEasyPLC.h"
#include <peri_GlobalVariablesExtern.h>
#include <peri_PosiEasyPLC.h>
#include <peri_TerminalDO.h>
extern PosiPLC PosiPLCSeg;

uint32_t DOAlarmSelect(uint32_t Fcode);

/*************************************************
  Function: Terminal Do: Servo Alarm Output
  Description:Period:1ms
  Input: No
  Return: No
  Others: Be Called TerminalDOFuncTable[]
*************************************************/
void TerminalDOServoAlm(void)
{
	DOCtrlFlg.bit.ServoAlm 	= CoreStatusFlag.bit.ERR_ALL_STOP_F;//20161206 mxj
	DOCtrlFlg.bit.Warnning 	= CoreStatusFlag.bit.WARN_ALL_F;
	DOCtrlFlg.bit.Run 		= CoreStatusFlag.bit.RUN_OK_F;

}
/*************************************************
  Function: Terminal Do: Servo Ready Output
  Description:Period:1ms
  Input: No
  Return: No
  Others: Be Called TerminalDOFuncTable[]
*************************************************/
void TerminalDOServoRdy(void)
{
	/*Servo Alarm*/
	if(CoreStatusFlag.bit.ERR_ALL_STOP_F)
	{
		DOCtrlFlg.bit.ServoRdy = NG;//�й����ŷ�δ׼����
		return;
	}

	if(CoreStatusFlag.bit.RELAY_OK_F == NG)
	{
		DOCtrlFlg.bit.ServoRdy = NG;//�̵���δ׼����ʱ�ŷ�δ׼����
		return;
	}

	DOCtrlFlg.bit.ServoRdy = OK;
}
/*************************************************
  Function: Terminal Do: External Brake Release Signal
  Description:Period:1ms
  Input: No
  Return: No
  Others: Be Called TerminalDOFuncTable[]
*************************************************/
void TerminalDOExBrake(void)
{
	if(CoreStatusFlag.bit.EX_BRAKE_F)//�ƶ������
	{
		DOCtrlFlg.bit.ExBrake = OK;
	}
	else
	{
		DOCtrlFlg.bit.ExBrake = NG;
	}
}
/*************************************************
  Function: Terminal Do: Position Complete
  Description:Period:1ms
  Input: No
  Return: No
  Others: Be Called TerminalDOFuncTable[]
*************************************************/
void PosiOKHoldTimeHandle(uint16_t id)
{
	DOCtrlFlg.bit.PosiOK = NG;
}
/*************************************************
  Function: Terminal Do: Position Complete
  Description:Period:1ms
  Input: No
  Return: No
  Others: Be Called TerminalDOFuncTable[]
*************************************************/
void TerminalDOPosiOK(void)//��λ���������ź�
{

}
/*************************************************
  Function: Terminal Do: Speed Arrival Output
  Description:Period:1ms
  Input: No
  Return: No
  Others: Be Called TerminalDOFuncTable[]
*************************************************/
void TerminalDOSPDArvl(void)//5 �ٶȴﵽ����ֵ
{
	int32_t VelocityTemp = 0;

	VelocityTemp = abs(g_VelocityQ24Fil);

	if(VelocityTemp >= g_VelocityQ24_Arv1 )
	{
		DOCtrlFlg.bit.SPDArvl = OK;
	}
	//else if(VelocityTemp < (g_VelocityQ24_Arv1 - 167772))//30rpm = 0.01 = 167772
	else if(VelocityTemp < (g_VelocityQ24_Arv1 - 33555))// 6rpm  0.002 = 33555  20180627
	{
		DOCtrlFlg.bit.SPDArvl = NG;
	}
}
/*************************************************
  Function: Terminal Do: Torque in-limit Signal Output
  Description:Period:1ms
  Input: No
  Return: No
  Others: Be Called TerminalDOFuncTable[]
*************************************************/
void TerminalDOTLimit(void)//������������ֵ
{
	if(CoreStatusFlag.bit.TLIMIT_F)
	{
		DOCtrlFlg.bit.TLimit = OK;
	}
	else
	{
		DOCtrlFlg.bit.TLimit = NG;
	}
}
/*************************************************
  Function: Terminal Do: Zero-Speed Status Output
  Description:Period:1ms
  Input: No
  Return: No
  Others: Be Called TerminalDOFuncTable[]
*************************************************/
void TerminalDOZSPD(void)//7
{
	int32_t VelocityTemp = 0;

	VelocityTemp = abs(g_VelocityQ24Fil);

	if(VelocityTemp <= g_VelocityQ24_Zero )//20
	{
		DOCtrlFlg.bit.ZSPD = OK;
	}
	else if(VelocityTemp > (g_VelocityQ24_Zero + 55924))//10rpm = 0.00333 = 55924
	{
		DOCtrlFlg.bit.ZSPD = NG;
	}
}
/*************************************************
  Function: Terminal Do: Speed Coincidence Output
  Description:Period:1ms
  Input: No
  Return: No
  Others: Be Called TerminalDOFuncTable[]
*************************************************/
void TerminalDOSPDCO(void)//�ٶ�  8
{
	uint32_t FcodeValue = 0;
	int64_t VeloRefTemp = 0;
	int64_t VeloEncoderTemp = 0;

	/* Work in Velocity and Torque Control Mode a*/
	//if(g_UltimateCtrlMode == POSITIONCTRL)

	if((g_UltimateCtrlMode != VELOCITYCTRL)||(!CoreStatusFlag.bit.RUN0_OK_F))
	//if((g_UltimateCtrlMode != VELOCITYCTRL)||(!CoreStatusFlag.bit.RUN_OK_F))
	{
		DOCtrlFlg.bit.SPDCO = NG;
		return;
	}

	FcodeValue = FuncCode_Handle.Ram.F50.F5053;//�ٶ�һ�·�Χ
	FcodeValue = FcodeValue * 10;
	/*Velocity Target*/
	//VeloRefTemp = (int64_t)g_VeloTargetQ24 * MotorPara.RatedVel ;
	VeloRefTemp = (int64_t)g_VeloTargetQ24 * 220 ;
	VeloRefTemp = VeloRefTemp >> 24;
	/*Velocity Feedback */
	//VeloEncoderTemp = (int64_t)g_VelocityQ24 * MotorPara.RatedVel ;
	VeloEncoderTemp = (int64_t)g_VelocityQ24 * 220;
	VeloEncoderTemp = VeloEncoderTemp >> 24;

	if((VeloEncoderTemp > (VeloRefTemp - FcodeValue)) && (VeloEncoderTemp < (VeloRefTemp + FcodeValue)))
	{
		DOCtrlFlg.bit.SPDCO = OK;
	}
	else
	{
		DOCtrlFlg.bit.SPDCO = NG;
	}
}
/*************************************************
  Function: Terminal Do: Speed in-limit Signal Output
  Description:Period:1ms
  Input: No
  Return: No
  Others: Be Called TerminalDOFuncTable[]
*************************************************/
void TerminalDOSPDLimit(void)//ת��ģʽ������ֵ
{
	DOCtrlFlg.bit.SPDLimit = CoreStatusFlag.bit.SPDLIMIT_F;
}
/*************************************************
  Function: Terminal Do: Position Arrival  Output
  Description:Period:1ms
  Input: No
  Return: No
  Others: Be Called TerminalDOFuncTable[]
*************************************************/
void TerminalDOPosiArvl(void)
{
	int32_t PosiArrCount = 0;
	if(g_UltimateCtrlMode != POSITIONCTRL)
	{
		DOCtrlFlg.bit.PosiArvl = NG;
		return;
	}

	PosiArrCount = g_PulInAllCount - g_EncPulAllCount;

	if(abs(PosiArrCount) < 20 )
	{
		DOCtrlFlg.bit.PosiArvl = OK;
	}
	else
	{
		DOCtrlFlg.bit.PosiArvl = NG;
	}

}
/*************************************************
  Function: Terminal Do: Alarm Output1
  Description:Period:1ms
  Input: No
  Return: No
  Others: Be Called TerminalDOFuncTable[]
*************************************************/
void TerminalDOAlmOut1(void)//�������ѡ��1
{
	uint32_t FcodeValue = 0;

	FcodeValue = FuncCode_Handle.Ram.F50.F5057;//�������ѡ��1

//	FcodeValue = 3;//test

	if(DOAlarmSelect(FcodeValue))
	{
		DOCtrlFlg.bit.AlmOut1 = OK;
	}
	else
	{
		DOCtrlFlg.bit.AlmOut1 = NG;
	}
}
/*************************************************
  Function: Terminal Do: Alarm Output2
  Description:Period:1ms
  Input: No
  Return: No
  Others: Be Called TerminalDOFuncTable[]
*************************************************/
void TerminalDOAlmOut2(void)
{
	uint32_t FcodeValue = 0;

	FcodeValue = FuncCode_Handle.Ram.F50.F5058;//�������ѡ��2

	if(DOAlarmSelect(FcodeValue))
	{
		DOCtrlFlg.bit.AlmOut2 = OK;
	}
	else
	{
		DOCtrlFlg.bit.AlmOut2 = NG;
	}
}
/*************************************************
  Function: Terminal Do: Origin Homing Mode Complete
  Description:Period:1ms
  Input: No
  Return: No
  Others: Be Called TerminalDOFuncTable[]
*************************************************/
void TerminalDOORZ(void)
{
	if(SHomeFlag.bit.SHomeSUCC_F)//20161128CYL
	//if(SHomeFlag.bit.SHomeSUCC_F & CoreStatusFlag.bit.RUN_OK_F)//mxj 20170112
	{
		DOCtrlFlg.bit.ORZ = OK;
	}
	else
	{
		DOCtrlFlg.bit.ORZ = NG;
	}

}
/*************************************************
  Function: Terminal Do:  Motor Break Output
  Description:Period:1ms
  Input: No
  Return: No
  Author: MXJ
  Others: Be Called TerminalDOFuncTable[]
*************************************************/
extern void PeriMotorBreakHandle(void);
void TerminalDOMtBrake(void)
{
	//��������ֹͣ��ʱ����,δtest
	//DOCtrlFlg.bit.Run
	PeriMotorBreakHandle();

	if(CoreStatusFlag.bit.MOTOR_BREAK_F )
	{
		DOCtrlFlg.bit.MotorBreak = OK;
	}
	else
	{
		DOCtrlFlg.bit.MotorBreak = NG;
	}
}

/*************************************************
  Function: Terminal Do:  PulseOutput
  Description:Period:1ms
  Input: No
  Return: No
  Author: MXJ
  Others: Be Called TerminalDOFuncTable[]
*************************************************/
void TerminalDOPulseOutput(void)
{
	static int32_t Pulse_Out_Cnt=0;
	int32_t Pulse_Out=0;

	Pulse_Out = (int32_t)FuncCode_Handle.Ram.F10.F1013;//20000

	Pulse_Out_Cnt ++;
	if(Pulse_Out_Cnt <=Pulse_Out)
	{
		DOCtrlFlg.bit.Pulses = (Pulse_Out_Cnt & 0x01);
		DOCtrlFlg.bit.Dir = 0;
	}
	else if(Pulse_Out_Cnt <=(Pulse_Out + 10))
	{
		DOCtrlFlg.bit.Pulses = 0;
		DOCtrlFlg.bit.Dir = 0;
	}
	else if(Pulse_Out_Cnt <=(Pulse_Out + 1020))
	{
		DOCtrlFlg.bit.Dir = 1;
	}
	else if(Pulse_Out_Cnt <=(Pulse_Out * 2 + 1020))
	{
		DOCtrlFlg.bit.Pulses = (Pulse_Out_Cnt & 0x01);
		DOCtrlFlg.bit.Dir = 1;
	}
	else if(Pulse_Out_Cnt <=(Pulse_Out * 2 + 1030))
	{
		DOCtrlFlg.bit.Pulses = 0;
		DOCtrlFlg.bit.Dir = 1;
	}
	else if(Pulse_Out_Cnt <=(Pulse_Out * 2 + 2030))
	{
		DOCtrlFlg.bit.Pulses = 0;
		DOCtrlFlg.bit.Dir = 0;
	}
	else
	{
		Pulse_Out_Cnt = 0;
	}
}



void TerminalDOTORQArvl(void)//29 ת�شﵽ����ֵ
{

}


/*************************************************
  Function: Terminal Do:Alarm or Warn Select
  Description:Period:1ms
  Input: No
  Return: No
  Others: Be Called TerminalDOAlmOut1/TerminalDOAlmOut2
*************************************************/
uint32_t DOAlarmSelect(uint32_t Fcode)
{
	uint32_t StsValue=0;
	switch(Fcode)
	{
	case NOOUTPUT:
		StsValue = 0;
		break;
	case ALMDRVFO:
		StsValue = AlarmStatusFlag.bit.Driver.FO;//1
		break;
	case ALMDRVDOC:
		StsValue = AlarmStatusFlag.bit.Driver.DOC;//2
		break;
	case ALMDRVPOV:
		StsValue = AlarmStatusFlag.bit.Driver.POV;//3
		break;
	case ALMDRVCOV:
		StsValue = AlarmStatusFlag.bit.Driver.COV;//4
		break;
	case ALMDRVPLV:
		StsValue = AlarmStatusFlag.bit.Driver.PLV;//5
		break;
	case ALMDRVCLV:
		StsValue = AlarmStatusFlag.bit.Driver.CLV;//6
		break;
	case ALMDRVDOL:
		StsValue = AlarmStatusFlag.bit.Driver.DOL;//7
		break;
	case ALMDRVOS:
		StsValue = AlarmStatusFlag.bit.Driver.OS;//8
		break;
	case ALMDRVPD:
		StsValue = AlarmStatusFlag.bit.Driver.PD;//9
		break;
	case ALMDRVSD:
		StsValue = AlarmStatusFlag.bit.Driver.SD;//10
		break;
	case ALMDRVES:
		StsValue = AlarmStatusFlag.bit.Driver.ES;//11
		break;
	case ALMDRVDOT:
		StsValue = AlarmStatusFlag.bit.Driver.DOT;//12
		break;
	case ALMDRVEE:
		StsValue = AlarmStatusFlag.bit.Driver.EE;//13
		break;
	case ALMDRVEC:
		StsValue = AlarmStatusFlag.bit.Driver.EC;//14
		break;
	case ALMDRVBUS:
		StsValue = AlarmStatusFlag.bit.Driver.BUS;//15
		break;
	case ALMDRVECE:
		StsValue = AlarmStatusFlag.bit.Driver.ECE;//16
		break;
	case ALMDRVORL:
		StsValue = AlarmStatusFlag.bit.Driver.ORL;//17
		break;
	case ALMDRVPIFE:
		StsValue = AlarmStatusFlag.bit.Driver.PIFE;//18
		break;
	case ALMDRVDCOF:
		StsValue = AlarmStatusFlag.bit.Driver.DCOF;//19
		break;
	case ALMDRVAI1E:
		StsValue = AlarmStatusFlag.bit.Driver.AI1E;//20
		break;
	case ALMDRVAI2E:
		StsValue = AlarmStatusFlag.bit.Driver.AI2E;//21
		break;
	case ALMDRVAI3E:
		StsValue = AlarmStatusFlag.bit.Driver.AI3E;//22
		break;
	case ALMDRVEPE:
		StsValue = AlarmStatusFlag.bit.Driver.EPE;//23
		break;
	case ALMDRVRLE:
		StsValue = AlarmStatusFlag.bit.Driver.RLE;//24
		break;
	case ALMDRVECE1:
		StsValue = AlarmStatusFlag.bit.Driver.ECE1;//25
		break;
	case ALMDRVECE2:
		StsValue = AlarmStatusFlag.bit.Driver.ECE2;//26
		break;
	case ALMDRVECE3:
		StsValue = AlarmStatusFlag.bit.Driver.ECE3;//27
		break;
	case ALMDRVEE1:
		StsValue = AlarmStatusFlag.bit.Driver.EE1;//28
		break;
	case ALMDRVEE2:
		StsValue = AlarmStatusFlag.bit.Driver.EE2;//29
		break;
	case ALMDRVEGE:
		StsValue = AlarmStatusFlag.bit.Driver.EGE;//30
		break;
	case ALMDRVPOSE:
		StsValue = AlarmStatusFlag.bit.Driver.POSE;//31
		break;
	case ALMDRVPHS:
		StsValue = AlarmStatusFlag.bit.Motor.PHS;//32
		break;
	case ALMMTRMOC:
		StsValue = AlarmStatusFlag.bit.Motor.MOC;//33
		break;
	case ALMMTRMOL:
		StsValue = AlarmStatusFlag.bit.Motor.MOL;//34
		break;
	case ALMMTRMOT:
		StsValue = AlarmStatusFlag.bit.Motor.MOT;//35
		break;
	case ALMMTRMC1:
		StsValue = AlarmStatusFlag.bit.Motor.MC1;//36
		break;
	case ALMMTRMC2:
		StsValue = AlarmStatusFlag.bit.Motor.MC2;//37
		break;
	case WARNPOT://20161206 mxj
		StsValue = WarnStatusFlag.bit.POT;//39
		break;
	case WARNNOT://20161206 mxj
		StsValue = WarnStatusFlag.bit.NOT;//40
		break;
	case WARNOL:
		StsValue = WarnStatusFlag.bit.OL;//41
		break;
	case WARNOT:
		StsValue = WarnStatusFlag.bit.OT;//42
		break;
	case WARNPOOS:
		StsValue = WarnStatusFlag.bit.POOS;//43
		break;
	case WARNAELV:
		StsValue = WarnStatusFlag.bit.AELV;//44
		break;
	case ALMMTRSEOT:
		StsValue = AlarmStatusFlag.bit.Motor.SEOT;//45
		break;
	case ALMMTRSEOS:
		StsValue = AlarmStatusFlag.bit.Motor.SEOS;//46
		break;
	case ALMMTRSERL:
		StsValue = AlarmStatusFlag.bit.Motor.SERL;//47
		break;
	case ALMMTRSECE:
		StsValue = AlarmStatusFlag.bit.Motor.SECE;//48
		break;
	case ALMMTRAEMO:
		StsValue = AlarmStatusFlag.bit.Motor.AEMO;//49
		break;
	case ALMMTRAECF:
		StsValue = AlarmStatusFlag.bit.Motor.AECF;//50
		break;
	case ALMMTRAEBE:
		StsValue = AlarmStatusFlag.bit.Motor.AEBE;//51
		break;
	case ALMMTRSETE:
		StsValue = AlarmStatusFlag.bit.Motor.SETE;//52
		break;
	case ALMMTRSEDE:
		StsValue = AlarmStatusFlag.bit.Motor.SEDE;//53
		break;
	case ALMMTRSEDC:
		StsValue = AlarmStatusFlag.bit.Motor.SEDC;//54
		break;
	case ALMMTRSEWE:
		StsValue = AlarmStatusFlag.bit.Motor.SEWE;//55
		break;
	case ALMMTRCUR:
		StsValue = AlarmStatusFlag.bit.Motor.CUr;//56
		break;
	case ALMMTRBRP:
		StsValue = AlarmStatusFlag.bit.Motor.BRP;//57
		break;
	case ALMMTROLF:
		StsValue = AlarmStatusFlag.bit.Motor.OLF;//58
		break;
	case ALMMTRHOME:
		StsValue = AlarmStatusFlag.bit.Motor.HOME;//59
		break;
	default:
		StsValue = 0;
		break;
	}
	return StsValue;
}

void TerminalDOSpdUpdate(void)//7
{
	uint32_t FcodeValue = 0;

	FcodeValue = FuncCode_Handle.Ram.F50.F5052 * 10;//���ٶ��趨
	//g_VelocityQ24_Zero = FcodeValue * MotorParaPU.RatedVel;
	g_VelocityQ24_Zero = FcodeValue * 220;

	FcodeValue = FuncCode_Handle.Ram.F50.F5054 *10;//�����ٶ�
	//g_VelocityQ24_Arv1 = FcodeValue * MotorParaPU.RatedVel;
	g_VelocityQ24_Arv1 = FcodeValue * 220;

	//FcodeValue = FuncCode_Handle.Ram.F40.F4065;//��������      20180904
	//g_TorqueQ24_Arv1 = FcodeValue * 16777;//1000 ->IQ(1.0)

	FcodeValue = FuncCode_Handle.Ram.F40.F4065 * 16777;//��������      20181008
	g_TorqueQ24_Arv1 = (int32_t)(((int64_t)FcodeValue * g_MotorCurGain_1)>>24);;//1000 ->IQ(1.0)

	g_TorqueArv1_Time = FuncCode_Handle.Ram.F40.F4066;//�������ؼ��ʱ��
	g_TorqueLock_Time = FuncCode_Handle.Ram.F40.F4067;//���ص����������ʱ��

}











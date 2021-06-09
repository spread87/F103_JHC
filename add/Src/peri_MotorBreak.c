/*=============================================================
 * peri_MotorBreak.c
 *
 *  Created on: 2016-6-23
 *      Author: MXJ
 ============================================================*/

#include <main.h>			//Declarations from DAVE3 Code Generation (includes SFR declaration)
#include <peri_GlobalVariablesExtern.h>
//#include "peri_MotorBreak.h"

void PeriMotorBreakHandle(void)
{
	uint32_t FcodeValue = 0;
	uint32_t Break_Stop_Spd = 0;
	static uint32_t Motor_Break_Start_Delay_time = 0;
	static uint32_t Motor_Break_Stop_Delay_time = 0;
	static uint32_t Motor_Break_Stop_Check_time = 0;

	//if(g_UltimateCtrlMode == POSITIONCTRL)	{	return;	}
	FcodeValue = FuncCode_Handle.Ram.F50.F5047;//������ʱʱ��
	if(CoreStatusFlag.bit.RUN0_OK_F)//�����ź���Ч
	{
		if(Motor_Break_Start_Delay_time < FcodeValue)
			Motor_Break_Start_Delay_time ++;
		else if(CoreStatusFlag.bit.RUN_OK_F)//�������
		{
			CoreStatusFlag.bit.MOTOR_BREAK_F = OK;
			Motor_Break_Stop_Delay_time = 0;
			Motor_Break_Stop_Check_time = 0;
		}
	}
	else//ͣ���ź���Ч�����й���ʱ
	{
		FcodeValue = FuncCode_Handle.Ram.F50.F5052;//�����趨��Χ
		//Break_Stop_Spd = FcodeValue * 10 * MotorParaPU.RatedVel;
		Break_Stop_Spd = FcodeValue * 10 * 220;

		//if(abs(g_VelocityQ24Fil) <= Break_Stop_Spd)
		if(((abs(g_VelocityQ24Fil) + abs(g_VelocityQ24Fdb))>>1)  <= Break_Stop_Spd)
		{
			Motor_Break_Stop_Check_time++;
			if(Motor_Break_Stop_Check_time >= 3)
			{
				CoreStatusFlag.bit.MOTOR_BREAK_F = NG;
				Motor_Break_Start_Delay_time = 0;
				Motor_Break_Stop_Check_time = 0;
			}
		}
		else
		{
			Motor_Break_Stop_Check_time = 0;
		}

		FcodeValue = FuncCode_Handle.Ram.F50.F5048;//�ر���ʱʱ��
		if(Motor_Break_Stop_Delay_time < FcodeValue)
			Motor_Break_Stop_Delay_time ++;
		else
		{
			CoreStatusFlag.bit.MOTOR_BREAK_F = NG;
			Motor_Break_Start_Delay_time = 0;
		}
	}
}

/*
 * peri_AlmCheck.c
 *
 * Created on: 2014-3-4
 * Author: JH-LvSL
 */


#include <main.h>			//Declarations from DAVE3 Code Generation (includes SFR declaration)
#include <math.h>
#include <peri_AlmWarnMacro.h>
#include <peri_GlobalVariablesExtern.h>
#include <peri_GPIO.h>

uint16_t g_AlmNum = 0;		//Alarm Number
uint32_t g_OVPUQ24 = 10249746;		//OV Protect PU Value
uint32_t g_LVPUQ24 = 10249746;		//LV Protect PU Value  0.61 =190 or 328V
uint32_t g_DOCPUQ24 = 0;	//Driver OC Protect PU Value
uint32_t g_MOCPUQ24 = 0;	//Motor OC Protect PU Value
uint16_t Bus_Error_Cnt = 0;	//mxj
uint16_t g_Bus_Error_Cnt = 0;	//mxj
//int32_t g_Motor_Power = 0;	//mxj
//static void AlmCheckOV(void);
static void AlmCheckLV(void);
static void AlmCheckOL(void);
static void AlmCheckOS(void);//����
static void AlmCheckPD(void);
static void AlmCheckOT(void);
static void AlmCheckDCOF(void);
static void AlmCheckAIE(void);
static void AlmCheckES(void);
static void WarnCheckPOT(void);
static void WarnCheckNOT(void);

//static void AlmCheckFO(void);
static void AlmLockedRotor(void);//2ms
static void AlmNegRunaway(void);//2ms
static void AlmCheckSD(void); //10ms
static void EncDisconnect(void);
static void ABErr(void);
static void ZErr(void);
static void AlmCheckAV(void);
static void AlmCheckOLF(void);
void  AlmCheckEnc(void);
void  ResetDelayHandle(uint16_t id);
void  AlmCheckBreakFO(void);

//extern void AlmWarnRecord(void);//20170420

uint8_t g_reset_flag=0;
/*************************************************
  Function: check alm
  Description: period:2ms
  Input: No
  Return: No
  Others: No
*************************************************/
void AlmCheck2ms(void)
{
	if(!SysTimeSequ.Start_Flg)
	return;
	//AlmCheckOV();//20180530   move to 1ms
	AlmCheckOLF();//20180810
	//AlmCheckLV();
	//AlmCheckAV();
//	AlmCheckOS();
	AlmCheckES();
//	AlmCheckEnc();//���������1.6US
  //  AlmLockedRotor();
  //  AlmNegRunaway();

	WarnCheckPOT();//20161128 CYL
	WarnCheckNOT();//20161128 CYL

}

/*************************************************
  Function: check alm
  Description: period:10ms
  Input: No
  Return: No
  Others: No
*************************************************/
void AlmCheck10ms(void)
{
	if(!SysTimeSequ.Start_Flg)
	return;
	//AlmCheckPD();
	AlmCheckSD();
}

/*************************************************
  Function: check alm
  Description: period:100ms
  Input: No
  Return: No
  Others: No
*************************************************/
void AlmCheck100ms(void)
{
	if(!SysTimeSequ.Start_Flg)
	return;
	AlmCheckOL();
	AlmCheckOT();
	//AlmCheckDCOF();  //None
	AlmCheckPD();

}

/*************************************************
  Function: check alm
  Description: period:500ms
  Input: No
  Return: No
  Others: No
*************************************************/
void AlmCheck500ms(void)
{
	if(!SysTimeSequ.Start_Flg)
	return;
	AlmCheckAIE();
}

void AlmCheckBreakFO(void)
{
	uint8_t BreakFo;
	static uint8_t BreakFo_Cnt=0;

	if(DriverPara.RatedPow < 2300)
		return;

	BreakFo = (uint8_t)MX_GPIO_ReadPin(IO001_Handle18);//P15.9@SCC1501C
	if(BreakFo == 0)
	{
		BreakFo_Cnt++;
		if(BreakFo_Cnt >= 20)
			AlarmStatusFlag.bit.Driver.BRKFO = OK;
	}
	else
	{
		BreakFo_Cnt = 0;
		AlarmStatusFlag.bit.Driver.BRKFO = NG;
	}
}

/*************************************************
  Function:check drive over-current
  Description: period:1ms
  Input: No
  Return: No
  Others: be called:AlmCheckOC()
*************************************************/
static void AlmCheckDrvOC(void)
{
	static uint16_t I_count = 0;
	//int64_t temp;
	//int32_t Curr_temp;

	if(AlarmStatusFlag.bit.Driver.DOC)
	{
		I_count = 0;
		return ;
	}

	if(CoreStatusFlag.bit.RUN_OK_F == NG)//check when stopping
	{
		//if(g_IrmsCalc >= 8388608)//IQ(0.5)
		//if(g_IrmsCalc >= 25165824)//IQ(1.5) 20180804
		if(g_IrmsCalc >= 20132659)//IQ(1.2)  20180807
		{
			//if(  abs(g_VelocityQ24Fil) < 1677721 )//0.1  20180804
			if(  abs(g_VelocityQ24Fil) < 503316 )//0.03  20180807
				I_count++;
		}
		else
		{
			I_count = 0;
		}

		if(I_count >= 3)//2->3
		{
			AlarmStatusFlag.bit.Driver.DOC = OK;
			IO004_SetOutputValue(IO004_Handle27,0);//����DRIVE�ź�
		}
	}
	else//check when running
	{
		//if(!SysTimeSequ.Start_Flg)
		//{
		//	I_count = 0;
		//	return ;
		//}
		if(AlarmStatusFlag.bit.Motor.SEDC)//20180713
			return;

		if(g_IrmsCalc > g_DOCPUQ24)
		{
			I_count++;
		}
		else
		{
			I_count = 0;
		}

		//Continued Time: 2ms//
		if(I_count >= 3)//2->3
		{
			AlarmStatusFlag.bit.Driver.DOC = OK;
		}
		else
		{
			AlarmStatusFlag.bit.Driver.DOC = NG;
		}
	}
}
/*************************************************
  Function:check motor over-current
  Description: period:100us
  Input: No
  Return: No
  Others: be called:AlmCheckOC()
*************************************************/
static void AlmCheckMotOC(void)//�������
{
	static uint16_t I_count = 0;
	//int32_t Curr_temp;

    //int64_t temp;

	if(AlarmStatusFlag.bit.Motor.MOC)
	{
		I_count = 0;
		return ;
	}
	if(!SysTimeSequ.Start_Flg)
	{
		I_count = 0;
		return ;
	}

	if(AlarmStatusFlag.bit.Motor.SEDC)//20180713
		return;

	//Judge U-Phase Instantaneous Current//
	//temp = abs(g_IrmsCalc);
	//temp = temp * g_MotorCurGain;
	//temp = temp>>24;

	if(g_IrmsCalc > g_MOCPUQ24)
	{
		I_count++;
	}
	else
	{
		I_count = 0;
	}

	//Continued Time: 2ms//
	if(I_count >= 5)//2->5  20171201
	{
		AlarmStatusFlag.bit.Motor.MOC = OK;
	}
	else
	{
		AlarmStatusFlag.bit.Motor.MOC = NG;
	}
}

/*************************************************
  Function:Check Motor&Driver Over-current
  Description: Period:1ms
  Input: No
  Return: No
  Others: be called: 1ms-interrupt
*************************************************/
void AlmCheckOC(void)
{
	AlmCheckDrvOC();//����2.8����1ms��������
	AlmCheckMotOC();//����3.0����1ms, ������
}
/*************************************************
  Function:Check Power Over-Voltage
  Description: Period:1ms
  Input: No
  Return: No
  Others: be called: AlmCheckOV()
*************************************************/
static void AlmCheckPowerOV(void)//20180530
{
	static uint16_t  OV_happen_cnt = 0;

	if(SysTimeSequ.Start_Flg == NG)
	{
		return;
	}

	if(AlarmStatusFlag.bit.Driver.POV == NG)
	{
		//if(g_PowerVbus10msFilQ24 >= g_OVPUQ24)
		if(g_PowerVbus1msFilQ24 >= g_OVPUQ24)
		{
			OV_happen_cnt++;
		}
		else
		{
			//OV_happen_cnt = 0;
			if(OV_happen_cnt > 0)
				OV_happen_cnt --;
		}

		if(OV_happen_cnt >= 6)//if(OV_happen_cnt >= 10)//20190831
		{
			AlarmStatusFlag.bit.Driver.POV = OK;
		}
		//else
		//{
		//	AlarmStatusFlag.bit.Driver.POV = NG;
		//}
	}
	else
	{
		OV_happen_cnt = 0;
	}
}

/*************************************************
  Function:Check Over-Voltage
  Description: Period:2ms->1ms
  Input: No
  Return: No
  Others: be called: AlmCheck2ms()
*************************************************/
void AlmCheckOV(void)
{
	AlmCheckPowerOV();
	//AlmCheckCtrlOV();
}

/*************************************************
  Function:Check Power Under-Voltage
  Description: Period:2ms
  Input: No
  Return: No
  Others: be called: AlmCheckLV()
*************************************************/
static void AlmCheckPowerLV(void)
{
	static uint16_t  LV_scan_cnt = 0;
	static uint16_t  LV_happen_cnt = 0;
	static uint16_t  LV_scan_cnt1=0;
	static uint16_t  LV_happen_cnt1=0;

	if(CoreStatusFlag.bit.EEPROMINIT_OK_F == NG)
	{
		return;
	}

	if(AlarmStatusFlag.bit.Driver.PLV == NG)//��Ƿѹ
	{
	    LV_scan_cnt1 = 0;
		LV_happen_cnt1 = 0;

		if(LV_scan_cnt > 2000)
		{
			/*Vbus scan 10 times (7 times effect),when Vbus<110V*/
			if(LV_happen_cnt > 1750)
			{
				AlarmStatusFlag.bit.Driver.PLV = OK;   //��ʱ���ε�
			}
			LV_scan_cnt   = 0;
			LV_happen_cnt = 0;
		}
		else
		{
			LV_scan_cnt++;
			/*Judge Power Board Vbus*/
			//if(g_PowerVbus10msFilQ24 < (uint32_t)PLV_THRED_Q24)
			if(g_PowerVbus10msFilQ24 < (uint32_t)g_LVPUQ24)
			{
				LV_happen_cnt++;
			}
		}
	}

	else
		{
		    LV_scan_cnt = 0;
			LV_happen_cnt = 0;
			if(LV_scan_cnt1 > 2000)
			{
				/*Vbus scan 10 times (7 times effect),when Vbus<110V*/
				if(LV_happen_cnt1 > 1750)
				{
					AlarmStatusFlag.bit.Driver.PLV = NG;   //��ʱ���ε�
				}
				LV_scan_cnt1   = 0;
				LV_happen_cnt1 = 0;
			}
			else
			{
				LV_scan_cnt1++;
				/*Judge Power Board Vbus*/
				//if(g_PowerVbus10msFilQ24 > (uint32_t)PLV_THRED1_Q24)
				if(g_PowerVbus10msFilQ24 > (uint32_t)(g_LVPUQ24 + 20*g_VpnVoltGain))
				{
					LV_happen_cnt1++;
				}
			}
		}
}

/*************************************************
  Function:Check Power&Control Board Under-Voltage
  Description: Period:2ms
  Input: No
  Return: No
  Others: be called: AlmCheck2ms()
*************************************************/
static void AlmCheckLV(void)
{
	AlmCheckPowerLV();
	//AlmCheckCtrlLV();
}
/*************************************************
  Function:Check Driver Over-Load
  Description: Period:100ms
  Input: No
  Return: No
  Others: be called: AlmCheckOL()
*************************************************/
static void AlmCheckDrvOL(void)
{
	static int32_t I_PerTherm = 0;//Load-Ratio
	static int32_t C_ThermCountM = 0;
	static int64_t Temp=0;
	static int32_t DeltaLR=0;

	if(!CoreStatusFlag.bit.RUN_OK_F)
	{
		I_PerTherm = 0;
		C_ThermCountM = 0;
		DeltaLR = 0;

		return;
	}

	if(AlarmStatusFlag.bit.Driver.DOL)
	{
		I_PerTherm = 0;
		C_ThermCountM = 0;
		DeltaLR = 0;

		return;
	}

	/* Calc Driver Load-Ratio */
	Temp = (int64_t)g_Irms10msFilQ24*100;//
	/* Calc the Deviation between Load-Ratio and 100(zoom:100) */
	I_PerTherm = Temp>>24 ;
	//I_PerTherm -=100;	/*100=>Rated Current*/
	//I_PerTherm -=106;	/*100=>Rated Current*/
	I_PerTherm -= FuncCode_Handle.Ram.F00.F0097;    //20181027

	/* Calc the Deviation Square;Notice the sign */
	if(I_PerTherm > 0)
		DeltaLR = I_PerTherm * I_PerTherm;//��������������
	else
		DeltaLR = -(I_PerTherm * I_PerTherm);//��������������

	/*Accumulate the Counts*/
	C_ThermCountM += DeltaLR;

	/*Judge Over-Load Protection*/
	if(C_ThermCountM > (int32_t)OL_THRED)//�ż�ֵ���׶Բ��ԣ�
	{
		AlarmStatusFlag.bit.Driver.DOL = OK;
	}

	if(C_ThermCountM < 0 )
	{
		C_ThermCountM = 0;
	}
}

/*************************************************
  Function:Check Motor Over-Load
  Description: Period:100ms
  Input: No
  Return: No
  Others: be called: AlmCheckOL()
*************************************************/
static void AlmCheckMotOL(void)//������,�Ѿ�����
{
	static int32_t I_PerTherm = 0;//Load-Ratio
	static int32_t C_ThermCountM = 0;
	static int64_t Temp=0;
	static int32_t DeltaLR=0;

	if(!CoreStatusFlag.bit.RUN_OK_F)
	{
		I_PerTherm = 0;
		C_ThermCountM = 0;
		DeltaLR = 0;

		return;
	}

	if(AlarmStatusFlag.bit.Motor.MOL)
	{
		I_PerTherm = 0;
		C_ThermCountM = 0;
		DeltaLR = 0;

		return;
	}

	/* Calc Motor Load-Ratio*/
	Temp = (int64_t)g_Irms10msFilQ24;//��������
	Temp = Temp * g_MotorCurGain;
	Temp = Temp>>24;//ת����������

	Temp = Temp * 100;
	/* Calc the Deviation between Load-Ratio and 100(zoom:100) */
	I_PerTherm = Temp>>24 ;
	//I_PerTherm -=100;	/*100=>Rated Current*/
	//I_PerTherm -=106;	/*100=>Rated Current*/
	I_PerTherm -= FuncCode_Handle.Ram.F00.F0098;    //20180516

	/* Calc the Deviation Square;Notice the sign*/
	if(I_PerTherm > 0)
		DeltaLR = I_PerTherm * I_PerTherm;//��������������
	else
		DeltaLR = -(I_PerTherm * I_PerTherm);//��������������

	/*Accumulate the Counts*/
	C_ThermCountM += DeltaLR;

	/*Judge Motor Over-Load Protection*/
	if(C_ThermCountM > (int32_t)OL_THRED)
	{
		AlarmStatusFlag.bit.Motor.MOL = OK;
	}

	if(C_ThermCountM < 0 )
	{
		C_ThermCountM = 0;
	}

	FuncCode_Handle.Ram.FA0.FA058 = C_ThermCountM>>6;//20170312
}

/*************************************************
  Function:Check Driver&Motor Over-Load
  Description: Period:100ms;Formula:y=100/x, x=square([(LoadRatio)-100%])
  Input: No
  Return: No
  Others: be called: AlmCheck100ms()
*************************************************/
static void AlmCheckOL(void)
{
	AlmCheckDrvOL();
	AlmCheckMotOL();
}
/*************************************************
  Function:Check Motor Over-Speed
  Description: Period:2ms;
  Input: Function Code: F6014
  Return: No
  Others: be called: AlmCheck2ms()
*************************************************/
static void AlmCheckOS(void)//10����7���ٶȳ������ֵ������
{
	static uint16_t  OS_scan_cnt = 0;
	static uint16_t  OS_happen_cnt = 0;
	uint64_t Temp = 0;
	uint32_t SetVelocity =0;

	/* Calc OS Protection Threshold*/
	if(FuncCode_Handle.Ram.F10.F1030 == 0)//���ٵȼ��趨
	{
		/*OS Threshold Equal Max Velocity Zoom 1.2 IQ24(1.2) = 20132659*/
		//Temp = (uint64_t)MotorPara.MaxVel * MotorParaPU.RatedVel;
		Temp = (uint64_t)g_VelocityMaxQ24;
		SetVelocity = (uint32_t)(Temp * 20132659)>>24;//����ٶ�1.2��
	}
	else
	{
		/*OS Threshold Equal F6014*/
		SetVelocity = FuncCode_Handle.Ram.F10.F1030;
		//SetVelocity = SetVelocity * MotorParaPU.RatedVel;//�Լ�����ת��
		//SetVelocity = (uint32_t)((uint64_t)SetVelocity <<24/MotorPara.RatedVel);
	}

	if(AlarmStatusFlag.bit.Driver.OS == NG)
	{
		if(OS_scan_cnt > 9)
		{
			/* Scan 10 times (7 times effect)*/
			if(OS_happen_cnt > 6)
			{
				AlarmStatusFlag.bit.Driver.OS = OK;
			}
			else
			{
				AlarmStatusFlag.bit.Driver.OS = NG;
			}
			OS_scan_cnt   = 0;
			OS_happen_cnt = 0;
		}
		else
		{
			OS_scan_cnt++;
			/*Judge Speed */
			if(g_EncSpeed1msFilQ16 > SetVelocity)
			{
				OS_happen_cnt++;
			}
		}
	}
	else
	{
		OS_scan_cnt = 0;
		OS_happen_cnt = 0;
	}
}

/*************************************************
  Function:Check Position Deviation Excess Protection
  Description: Period:100ms;
  Input: Function Code: F1011
  Return: No
  Others: be called: AlmCheck10ms()-100ms()
*************************************************/
static void AlmCheckPD(void)
{
	static uint16_t  PD_scan_cnt = 0;
	static uint16_t  PD_happen_cnt = 0;
	uint32_t Temp=0;

	//if(g_UltimateCtrlMode != POSITIONCTRL)
	//if(!CoreStatusFlag.bit.RUN_OK_F)
	if((!CoreStatusFlag.bit.RUN_OK_F)||(g_UltimateCtrlMode != POSITIONCTRL))//20171028 mxj
	{
		PD_scan_cnt = 0;
		PD_happen_cnt = 0;
		return ;//�����е�ʱ�� ���ù�
	}

	//if(MotorPara.EncMode == 0)
	//if(MotorPara.EncMode <= 1)
		Temp = FuncCode_Handle.Ram.F10.F1029;
	//else
	//	Temp = ((int64_t)FuncCode_Handle.Ram.F10.F1029 * MotorPara.EncLine)>>11;

	if(Temp == 0)
	{
		PD_scan_cnt = 0;
		PD_happen_cnt = 0;
		return ;
	}

	//if(!CoreStatusFlag.bit.RUN_OK_F)
	//{
	//	PD_scan_cnt = 0;
	//	PD_happen_cnt = 0;
	//	return ;//�����е�ʱ�� ���ù�
	//}

	if(AlarmStatusFlag.bit.Driver.PD == NG)
	{
		if(PD_scan_cnt > 9)
		{
			/* Scan 10 times (7 times effect)*/
			if(PD_happen_cnt > 6)
			{
				AlarmStatusFlag.bit.Driver.PD = OK;
			}
			else
			{
				AlarmStatusFlag.bit.Driver.PD = NG;
			}
			PD_scan_cnt   = 0;
			PD_happen_cnt = 0;
		}
		else
		{
			PD_scan_cnt++;
			/*Judge Position Deviation */
			if(abs(g_PosDevtRegCount) > Temp)
			{
				PD_happen_cnt++;
			}
		}
	}
	else
	{
		PD_scan_cnt = 0;
		PD_happen_cnt = 0;
	}
}
/*************************************************
  Function:Check Driver Over-Temperature Protection
  Description: Period:100ms;
  Input: No
  Return: No
  Others: be called: AlmCheckOT()
*************************************************/
static void AlmCheckDrvOT(void)
{
	static uint16_t  OT_scan_cnt = 0;
	static uint16_t  OT_happen_cnt = 0;
	uint32_t FcodeValue = 0;

	FcodeValue = FuncCode_Handle.Ram.F00.F0055;//Temperature Protection Value

	if(SysTimeSequ.Start_Flg == NG)
	{
		return;
	}

}
/*************************************************
  Function:Check Motor Over-Temperature Protection
  Description: Period:100ms;
  Input: No
  Return: No
  Others: be called: AlmCheckOT()
*************************************************/
static void AlmCheckMotOT(void)
{
	static uint16_t  OT_scan_cnt = 0;
	static uint16_t  OT_happen_cnt = 0;

	if(AlarmStatusFlag.bit.Motor.MOT== NG)
	{
		if(OT_scan_cnt > 9)
		{
			/* Scan 10 times (7 times effect) */
			if(OT_happen_cnt > 6)
			{
				AlarmStatusFlag.bit.Motor.MOT = OK;
			}
			else
			{
				AlarmStatusFlag.bit.Motor.MOT = NG;
			}
			OT_scan_cnt   = 0;
			OT_happen_cnt = 0;
		}
		else
		{
			OT_scan_cnt++;
			/*Judge the Sample Temperature*/
			if(g_PTC10msFilQ24 > (uint32_t)MOT_THRED)
			{
				OT_happen_cnt++;
			}
		}
	}
	else
	{
		OT_scan_cnt = 0;
		OT_happen_cnt = 0;
	}
}

/*************************************************
  Function:Check Driver&Motor Temperature
  Description: Period:100ms;
  Input: Function Code: F1011
  Return: No
  Others: be called: AlmCheck100ms()
*************************************************/
static void AlmCheckOT(void)
{
	AlmCheckDrvOT();
	AlmCheckMotOT();
}

/*************************************************
  Function:Check Deviation counter overflow protection
  Description: Period:100ms
  Input: Function Code:Max.F1011
  Return: No
  Others: be called: AlmCheck100ms()
*************************************************/
static void AlmCheckDCOF(void)//no used
{
	static uint16_t  DCOF_scan_cnt = 0;
	static uint16_t  DCOF_happen_cnt = 0;
	uint32_t Temp = 0;

	Temp = FuncCode_Handle.Max.F10.F1011;//�����Ԫû�ð� �����

	if(AlarmStatusFlag.bit.Driver.DCOF == NG)
	{
		if(DCOF_scan_cnt > 9)
		{
			/* Scan 10 times (7 times effect) */
			if(DCOF_happen_cnt > 6)
			{
	//			AlarmStatusFlag.bit.Driver.DCOF = OK;
			}
			else
			{
				AlarmStatusFlag.bit.Driver.DCOF = NG;
			}
			DCOF_scan_cnt   = 0;
			DCOF_happen_cnt = 0;
		}
		else
		{
			DCOF_scan_cnt++;
			/*Judge the Position Deviation Register*/
			if(abs(g_PosDevtRegCount) > Temp)//ƫ�ʱ�����һ��ֵ����PD�����ظ�
			{
				DCOF_happen_cnt++;
			}
		}
	}
	else
	{
		DCOF_scan_cnt = 0;
		DCOF_happen_cnt = 0;
	}
}

/*************************************************
  Function:Check Analog Input Excess Protection
  Description: Period:500ms
  Input: No
  Return: No
  Others: be called: AlmCheck500ms()
*************************************************/
static void AlmCheckAIE(void)
{
	static uint16_t  AIE_scan_cnt = 0;
	static uint16_t  AI1E_happen_cnt = 0;
	static uint16_t  AI2E_happen_cnt = 0;
	static uint16_t  AI3E_happen_cnt = 0;

	if(AlarmStatusFlag.bit.Driver.AI1E ||
		AlarmStatusFlag.bit.Driver.AI2E ||
		AlarmStatusFlag.bit.Driver.AI3E )
	{
		AIE_scan_cnt = 0;
		AI1E_happen_cnt = 0;
		AI2E_happen_cnt = 0;
		AI3E_happen_cnt = 0;

		return;
	}

	if(AIE_scan_cnt > 9)
	{
		/* Scan 10 times (7 times effect) */
		if(AI1E_happen_cnt > 6)
		{
			AlarmStatusFlag.bit.Driver.AI1E = OK;
		}
		else
		{
			AlarmStatusFlag.bit.Driver.AI1E = NG;
		}

		if(AI2E_happen_cnt > 6)
		{
			AlarmStatusFlag.bit.Driver.AI2E = OK;
		}
		else
		{
			AlarmStatusFlag.bit.Driver.AI2E = NG;
		}

		if(AI3E_happen_cnt > 6)
		{
			AlarmStatusFlag.bit.Driver.AI3E = OK;
		}
		else
		{
			AlarmStatusFlag.bit.Driver.AI3E = NG;
		}

		AIE_scan_cnt = 0;
		AI1E_happen_cnt = 0;
		AI2E_happen_cnt = 0;
		AI3E_happen_cnt = 0;
	}
	else
	{
		AIE_scan_cnt++;
		/*Judge the AD Sample*/
		if((g_AI1FilQ24 > AIE_THRED) || (g_AI1FilQ24 < (-AIE_THRED))) 	AI1E_happen_cnt++;
		if((g_AI2FilQ24 > AIE_THRED) || (g_AI2FilQ24 < (-AIE_THRED))) 	AI2E_happen_cnt++;
		if((g_AI3FilQ24 > AIE_THRED) || (g_AI3FilQ24 < (-AIE_THRED))) 	AI3E_happen_cnt++;
	}
}
/*************************************************
  Function:Emergency Stop
  Description: Period:2ms
  Input: No
  Return: No
  Others: be called: AlmCheck2ms()
*************************************************/
static void AlmCheckES(void)
{
	if(DICtrlFlg.bit.EStop)
		AlarmStatusFlag.bit.Driver.ES = OK;
	//else  20181122
	//	AlarmStatusFlag.bit.Driver.ES = NG;
}

/*************************************************
  Function:��ת������λ
  Description: Period:2ms
  Input: No
  Return: No
  Others: be called: AlmCheck2ms()
*************************************************/
static void WarnCheckPOT(void)
{
	//if(DICtrlFlg.bit.POT)
	if(DICtrlFlg.bit.POT&&SHomeParameter.LimitMode==0)//20161212
		WarnStatusFlag.bit.POT = OK;
	else
		WarnStatusFlag.bit.POT = NG;
}

/*************************************************
  Function:��ת������λ
  Description: Period:2ms
  Input: No
  Return: No
  Others: be called: AlmCheck2ms()
*************************************************/
static void WarnCheckNOT(void)
{
	//if(DICtrlFlg.bit.NOT)
	if(DICtrlFlg.bit.NOT&&SHomeParameter.LimitMode==0)
		WarnStatusFlag.bit.NOT = OK;
	else
		WarnStatusFlag.bit.NOT = NG;
}

/*************************************************
  Function:Read Alarm Number
  Description: Period:100ms->mainloop
  Input: No
  Return: No
  Others: be called: AlmCheck100ms()
*************************************************/
void AlmReadNum(void)
{
	//if(!AlarmStatusFlag.all)//�����־λ
	CoreStatusFlag.bit.AlmNum = (uint8_t)g_AlmNum;
	if((!AlarmStatusFlag.all)&&(WarnStatusFlag.all==0))//�����־λ 20161128
	{
		/*No Alarm*/
		g_AlmNum = 0;
		return ;
	}

	if(AlarmStatusFlag.bit.Motor.MDP)//������������������  20180906
		g_AlmNum = ALM_CODE_MDP;//38
	else if(AlarmStatusFlag.bit.Driver.FO)
		g_AlmNum = ALM_CODE_FO;
	else if(AlarmStatusFlag.bit.Motor.AEMO)
		g_AlmNum = ALM_CODE_AEMO;
	else if(AlarmStatusFlag.bit.Motor.AEBE)
		g_AlmNum = ALM_CODE_AEBE;//E.SE17
	else if(AlarmStatusFlag.bit.Motor.AECF)
		g_AlmNum = ALM_CODE_AECF;
	else if(AlarmStatusFlag.bit.Motor.SERL)
		g_AlmNum = ALM_CODE_SERL;//SE12
	else if(AlarmStatusFlag.bit.Motor.SEOT)
		g_AlmNum = ALM_CODE_SEOT;//E.SE10
	else if(AlarmStatusFlag.bit.Motor.SEOS)
		g_AlmNum = ALM_CODE_SEOS;//SE11
	else if(AlarmStatusFlag.bit.Motor.SECE)
		g_AlmNum = ALM_CODE_SECE;//SE13
	else if(AlarmStatusFlag.bit.Motor.SETE)//���������ࣨλ�������������趨��ͬ
		g_AlmNum = ALM_CODE_SETE;//52 E.SE18
	else if(AlarmStatusFlag.bit.Motor.SEDC)
		g_AlmNum = ALM_CODE_SEDC;//54 E.SE20
	else if(AlarmStatusFlag.bit.Motor.SEDE)
		g_AlmNum = ALM_CODE_SEDE;//53 E.SE19
	else if(AlarmStatusFlag.bit.Motor.SEWE)
		g_AlmNum = ALM_CODE_SEWE;//55 E.SE21
	else if(AlarmStatusFlag.bit.Driver.DOC)
		g_AlmNum = ALM_CODE_DOC;
	else if(AlarmStatusFlag.bit.Motor.BRP)//�ƶ����ʲ���  20190831
		g_AlmNum = ALM_CODE_BRP1;//57
	else if(AlarmStatusFlag.bit.Driver.POV)
		g_AlmNum = ALM_CODE_POV;
	else if(AlarmStatusFlag.bit.Driver.COV)
		g_AlmNum = ALM_CODE_COV;
	else if(AlarmStatusFlag.bit.Driver.PLV)
		g_AlmNum = ALM_CODE_PLV;
	else if(AlarmStatusFlag.bit.Driver.CLV)
		g_AlmNum = ALM_CODE_CLV;
	else if(AlarmStatusFlag.bit.Driver.DOL)
		g_AlmNum = ALM_CODE_DOL;
	else if(AlarmStatusFlag.bit.Driver.OS)
		g_AlmNum = ALM_CODE_OS;
	else if(AlarmStatusFlag.bit.Driver.PD)//λ��ƫ�����
		g_AlmNum = ALM_CODE_PD;
	else if(AlarmStatusFlag.bit.Driver.SD)//�ٶ�ƫ�����
		g_AlmNum = ALM_CODE_SD;
	else if(AlarmStatusFlag.bit.Driver.ES)
		g_AlmNum = ALM_CODE_ES;
	else if(AlarmStatusFlag.bit.Driver.DOT)
		g_AlmNum = ALM_CODE_DOT;
	else if(AlarmStatusFlag.bit.Driver.EE)
		g_AlmNum = ALM_CODE_EE;
	else if(AlarmStatusFlag.bit.Driver.EC)//�����
		g_AlmNum = ALM_CODE_EC;
	else if(AlarmStatusFlag.bit.Driver.BUS)//ĸ�߸�������ȱ���ж�
		g_AlmNum = ALM_CODE_BUS;
	else if(AlarmStatusFlag.bit.Driver.ECE)//�ж���ֱ���͹���
		g_AlmNum = ALM_CODE_ECE;
	else if(AlarmStatusFlag.bit.Driver.ORL)
		g_AlmNum = ALM_CODE_ORL;
	else if(AlarmStatusFlag.bit.Driver.PIFE)
		g_AlmNum = ALM_CODE_PIFE;
	else if(AlarmStatusFlag.bit.Driver.DCOF)
		g_AlmNum = ALM_CODE_DCOF;
	else if(AlarmStatusFlag.bit.Driver.AI1E)
		g_AlmNum = ALM_CODE_AI1E;
	else if(AlarmStatusFlag.bit.Driver.AI2E)
		g_AlmNum = ALM_CODE_AI2E;
	else if(AlarmStatusFlag.bit.Driver.AI3E)
		g_AlmNum = ALM_CODE_AI3E;
	else if(AlarmStatusFlag.bit.Driver.EPE)
		g_AlmNum = ALM_CODE_EPE;
	else if(AlarmStatusFlag.bit.Driver.RLE)
		g_AlmNum = ALM_CODE_RLE;
	else if(AlarmStatusFlag.bit.Driver.ECE1)//��Z�ж�ʱ������������ֵ����
		g_AlmNum = ALM_CODE_ECE1;
	else if(AlarmStatusFlag.bit.Driver.ECE2)//������˳�����ʱ��һֱδ��Z�ж�
		g_AlmNum = ALM_CODE_ECE2;
	else if(AlarmStatusFlag.bit.Driver.ECE3)//�ϱ�����
		g_AlmNum = ALM_CODE_ECE3;
	else if(AlarmStatusFlag.bit.Driver.EE1)//��һ���ϵ�
		g_AlmNum = ALM_CODE_EE1;
	else if(AlarmStatusFlag.bit.Driver.EE2)//ack����
		g_AlmNum = ALM_CODE_EE2;
	else if(AlarmStatusFlag.bit.Driver.EGE)//���ӳ��ֱȳ���
		g_AlmNum = ALM_CODE_EGE;
	else if(AlarmStatusFlag.bit.Driver.POSE)//����������ó���
		g_AlmNum = ALM_CODE_POSE;//31
	else if(AlarmStatusFlag.bit.Motor.PHS)//�������
			g_AlmNum = ALM_CODE_PHS;//32
	else if(AlarmStatusFlag.bit.Motor.MOC)
		g_AlmNum = ALM_CODE_MOC;//33
	else if(AlarmStatusFlag.bit.Motor.MOL)
		g_AlmNum = ALM_CODE_MOL;//34
	else if(AlarmStatusFlag.bit.Motor.MOT)
		g_AlmNum = ALM_CODE_MOT;//35
	else if(AlarmStatusFlag.bit.Motor.MC1)//��ת����
		g_AlmNum = ALM_CODE_MC1;//36
	else if(AlarmStatusFlag.bit.Motor.MC2)//�ɳ�
		g_AlmNum = ALM_CODE_MC2;//37
	else if(WarnStatusFlag.bit.POT) //��������λ  20161128
		g_AlmNum = WARN_CODE_POT;//39
	else if(WarnStatusFlag.bit.NOT) //��������λ
		g_AlmNum = WARN_CODE_NOT;//40
	else if(WarnStatusFlag.bit.OL) //over-load warn
		g_AlmNum = WARN_CODE_OL;//41
	else if(WarnStatusFlag.bit.OT) //over-temperature warn
		g_AlmNum = WARN_CODE_OT;//42
	else if(WarnStatusFlag.bit.POOS) //�����������
		g_AlmNum = WARN_CODE_POOS;//43
	else if(WarnStatusFlag.bit.AELV) //����ֵ���������Ƿѹ
		g_AlmNum = WARN_CODE_AELV;//44
	else if(AlarmStatusFlag.bit.Motor.CUr)//������������������
		g_AlmNum = ALM_CODE_CUR1;//56
	//else if(AlarmStatusFlag.bit.Motor.BRP)//�ƶ����ʲ���
		//g_AlmNum = ALM_CODE_BRP1;//57
	else if(AlarmStatusFlag.bit.Motor.OLF)//
		g_AlmNum = ALM_CODE_OLF1;//58
	else if(AlarmStatusFlag.bit.Motor.HOME)//
			g_AlmNum = ALM_CODE_HOME;//59

//	else
//		g_AlmNum = 0

	if(AlarmStatusFlag.all)//�й��Ϸ���
	{
		AlmWarnRecord(); //��¼���ϲ���
	}
}
/*************************************************
  Function:  Alarm Clear
  Description: Period:System Loop
  Input: No
  Return: No
  Others: be called: SysMainLoop()
*************************************************/
void AlmClr(void)
{
	//if(DICtrlFlg.bit.AlmClr)
	if(DICtrlFlg.bit.AlmClr||ComCtrlFlg.bit.ResetFlg)//20180822
	{
		struct TimerConfig tc;

		if(AlarmStatusFlag.all)
		{
			g_reset_flag=0xff;//��λ������
			tc.init_ms =250;	//��ʱ500ms
			tc.mode = TIMER_MODE_ONESHOT;
			tc.func = ResetDelayHandle;
			TimerInit(RESETDELAY,&tc);
			TimerStart(RESETDELAY);

			if((SeriEncoder.Operation&0xF000)==0&&((FuncCode_Handle.Ram.F10.F1040&0x05)==0||((FuncCode_Handle.Ram.F10.F1040&0x0F00)>>8)==1))//������б���������
			{
				SeriEncoder.Operation=0x6000+(SeriEncoder.Operation&0x0FFF);//0��2 can    1��3��4can't
			}

			SHomeFlag.all = 0;//20181128

			AlarmStatusFlag.all &= 0x0001;//20180822 not clear the FO
			WarnStatusFlag.all=0;//CYL 20161206
		}
		//ComCtrlFlg.bit.ResetFlg=0; //CYL 20170520

		ComCtrlFlg.all &= 0x02;//��������תά��ԭ��״̬������ȫ�����㡣20181122

		CoreStatusFlag.bit.ERR_ALL_STOP_F = NG;
		AlmRecord.AlmRcdFlg = OK;//������Ϻ�
	}
}

void ResetDelayHandle(uint16_t id)
{
	    g_reset_flag=0;
}

/*************************************************
  Function:Protect PU Value Calculate
  Description: Period:System Loop
  Input: No
  Return: No
  Others: be called: SysMainLoop()
*************************************************/
void ProtectPUValueCalc(void)
{
	uint32_t FcodeValue = 0;

	/*OV PU Value Calc*/
	FcodeValue = FuncCode_Handle.Ram.F00.F0056;
//	FcodeValue = FuncCode_Handle->
	//g_OVPUQ24 = FcodeValue * 53924;	//53924 = 2^24/(220*sqrt(2))
	g_OVPUQ24 = FcodeValue * g_VpnVoltGain;	//53924 = 2^24/(220*sqrt(2))

	/*LV PU Value Calc*/
	FcodeValue = FuncCode_Handle.Ram.F00.F0057;
	//g_LVPUQ24 = FcodeValue * 53924;	//53924 = 2^24/(220*sqrt(2))
	g_LVPUQ24 = FcodeValue * g_VpnVoltGain;	//53924 = 2^24/(220*sqrt(2))

	/*Driver OC PU Value Calc*/
	FcodeValue = FuncCode_Handle.Ram.F00.F0058;
	g_DOCPUQ24 = FcodeValue * 167772;//167772 = 2^24/100

	/*Motor OC PU Value Calc*/
	FcodeValue = FuncCode_Handle.Ram.F00.F0059;
	//g_MOCPUQ24 = FcodeValue * 167772;//167772 = 2^24/100
	g_MOCPUQ24 = ((int64_t)FcodeValue * 167772 * g_MotorCurGain_1)>>24;//mxj

}

/*************************************************
  Function:Check SPEED Deviation Excess Protection
  Description: Period:10ms;
  Return: No
  Others: be called: AlmCheck10ms()
*************************************************/
static void AlmCheckSD(void)//200ms ��16�����ڲ�ֵ����
{}

/*************************************************
  Function:Protect encoder
  Description: Period:2ms
  Input: No
  Return: No
  Others: be called: 2msLoop()
*************************************************/
void AlmCheckEnc(void)
{
	if(g_CtrlMode == 11)//VF Control mode
		return;

	//if(MotorPara.EncMode == 0)//20170302 mxj
	//if(MotorPara.EncMode <= 1)//20170302 mxj
	{
		EncDisconnect();//E.ECE3
		ABErr();//E.ECE
		ZErr();//E.ECE2
	}
	//E.ECE1 is in the Zphase_ISR1
}

/*���������ӶϿ�*/
static void EncDisconnect(void)
{
	    uint8_t tempUVW = 0;
//		uint32_t tempAng = 0;
		uint8_t Temp = 0;
        static uint8_t counter = 0;
        static uint8_t counter1 = 0;

		tempUVW = g_tempUVW;

   if( AlarmStatusFlag.bit.Driver.ECE3 == NG)
   {
		/* Read Encoder UVW Initial Angle */

		switch (tempUVW) //Detect the middle of Section
		{
		     case 0:
		     {
		    	    counter++;
		    	    if(counter >= 15)//50->15  20180719
		            AlarmStatusFlag.bit.Driver.ECE3 = OK;
		            break;
		     }
		     case 7:
		     {
		    	    counter1++;
		    	    if(counter1 >= 15)//50->15
		        	AlarmStatusFlag.bit.Driver.ECE3 = OK;
		            break;
		     }
		     default:
		     {
		    	    counter = 0;
		    	    counter1 = 0;
		        	break;
		     }
		}
   }
   else
   {
	   switch (tempUVW)
	   {
	   case 0:
		   break;
	   case 7:
		   break;
	   default:
	       {
		   counter = 0;
		   counter1 = 0;
		   //AlarmStatusFlag.bit.Driver.ECE3 = NG;  //20170223 not restore
		   break;
	       }
	   }

   }
}

/*AB��λ����*/
static void ABErr(void)
{
	static uint16_t i;
	if(i>=500)// 1s��
	{

		if(g_ABErrCount >=100)
		{
		AlarmStatusFlag.bit.Driver.ECE = OK;
		}
		g_ABErrCount = 0;
		i=0;
	}
	else
	i++;

	//if(MotorPara.EncMode == 1)//ʡ��ʽ
	//	return;

	if(!CoreStatusFlag.bit.ZPHASE_F)//Z never detected
		return;

	if(FuncCode_Handle.Ram.F00.F0073&0x0002)//bit1����E.ECE
		return;

	//if(FuncCode_Handle.Ram.F10.F1005 != 0)//�����ѧϰ
	//	return;

	int32_t temp;
	static uint16_t g_ABUVWErrCount = 0;
	temp = ((g_Uvw_Theta + g_MotorInitAngleQ24)&0x00FFFFFF) - g_EleThetaQ24;
	if(temp>0x07FFFFF)
		temp-=0x01000000;
	else if(temp<-0x07FFFFF)
		temp+=0x01000000;

	if(temp > 3728270)//80 IQ(80/360)=IQ(0.2222) = 3728270
		g_ABUVWErrCount+=3;
	else if(temp < -3728270)
		g_ABUVWErrCount+=3;
	else if(g_ABUVWErrCount > 1)
		g_ABUVWErrCount-=1;

	if(g_ABUVWErrCount > 100)//66ms
	{
		AlarmStatusFlag.bit.Driver.ECE = OK;
		g_ABUVWErrCount = 0;
	}
}

/*Z��ʧ*/
static void ZErr(void)
{
	static uint32_t z_counter_old=0;
	static int64_t  pulse_start=0;
	int64_t  	pulse_now=0;
	int32_t  	pulse_total=0;
	uint32_t  	temp;
	uint32_t	FcodeValue;

	FcodeValue = FuncCode_Handle.Ram.F00.F0074 * 10000;

	if(FcodeValue <= 9999)
	{
		return;
	}

	if(!CoreStatusFlag.bit.RUN_OK_F)//��ֹ��ʱ��
	{
		pulse_start = g_EncPulAllCount;
		pulse_now = g_EncPulAllCount;
		z_counter_old = g_index_counter;//ȡ��ǰ��Z���������
	}
	else
	{
	   if(AlarmStatusFlag.bit.Driver.ECE2==NG)
	   {
			pulse_now = g_EncPulAllCount;
			pulse_total = pulse_now - pulse_start;
			//if ( abs(pulse_total) >= 50000)  //����5Ȧ
			//if ( abs(pulse_total) >= 80000)  //����8Ȧ
			if ( abs(pulse_total) > FcodeValue)  //����8Ȧ
			{
				temp = g_index_counter - z_counter_old;
				if( temp == 0)
				{
				   AlarmStatusFlag.bit.Driver.ECE2 = OK ;
				}
				else
				{
					pulse_start = g_EncPulAllCount;
					z_counter_old = g_index_counter;
				}
			}
	   }
	}
}


/*ĸ�ߵ�ѹ���*/
static void AlmCheckAV()
{

}


static void AlmCheckOLF(void)
{
	static uint16_t OLF_count = 0;
	int64_t temp;

	//if((AlarmStatusFlag.bit.Motor.OLF)||(CoreStatusFlag.bit.RUN_OK_F == NG))
	if((AlarmStatusFlag.bit.Motor.OLF)||(CoreStatusFlag.bit.RUN_OK_F == NG)||(FuncCode_Handle.Ram.F00.F0073&0x0001))
	{
		OLF_count = 0;
		return ;
	}

	if(g_VrmsCalc > 8388608)//IQ(0.5)
	{
		temp = g_IrmsCalc;
		temp = (temp * g_LqValueQ24)>>24;
		//g_canview25 = temp;//20180810

		temp = (((int64_t)(abs(g_VelocityQ24Fdb)) * (g_FluxfQ24 + temp))>>24) + 5033164;//IQ(0.3)

		g_canview25 = temp;//20180810

		if(temp < g_VrmsCalc)
		{
			OLF_count++;
			if(OLF_count >10)//2*20=40ms->20ms
				AlarmStatusFlag.bit.Motor.OLF = OK;
		}
		else
		{
			if(OLF_count > 0)
				OLF_count--;
		}
	}
	else
	{
		OLF_count = 0;
	}

	FuncCode_Handle.Ram.FA0.FA064 = OLF_count;
}

/*************************************************
  Function:Protect encoder
  Description: Period:SysMainLoop
  Input: No
  Return: No
  Others: be called: SysMainLoop()
*************************************************/
void AlmCheckMotorandDriver(void)
{
	//uint32_t drive_level=0;
	uint32_t Charger_type=0;
	//drive_level = FuncCode_Handle.Ram.F00.F0004;//��������
	Charger_type =  FuncCode_Handle.Ram.F00.F0002;//�������

	if(Charger_type<100)
		AlarmStatusFlag.bit.Motor.MDP = OK;
	else
		AlarmStatusFlag.bit.Motor.MDP = NG;

	//if(!SysTimeSequ.Sec10_Flg)//δ�����ã�������
	//	return;
}

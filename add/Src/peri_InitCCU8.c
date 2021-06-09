/*
 * peri_InitCCU8.c
 *
 *  Created on: 2014-3-25
 * Author: JH-LvSL
 */
#include <main.h>			//Declarations from DAVE3 Code Generation (includes SFR declaration)
#include <peri_GlobalVariablesExtern.h>
#include <peri_GPIO.h>


/*************************************************
  Function: PWM output Init
  Description:Period:Init
  Input: No
  Return: No
  Others: Be Called InitModule()
*************************************************/
void CCU8Init(void)
{
	uint16_t  PwmCompareValue = 0;

//	IO004_SetOutputValue(IO004_Handle27,0);//mxj 20170120  disable pwm

	/*Init IO*/
	PwmIOInit();//IO�ڳ�ʼ��
	/*Calculate Pwm Period Value according to Carry Frequency*/
	CCU8GetPeriod((uint8_t)g_CarryPwmFreq);
	/*Calculate 50% Duty*/
	//PwmCompareValue = (uint16_t)(g_PwmPeriod>>1) +1;
	PwmCompareValue = (uint16_t)(g_PwmPeriod) +1;//20170221 mxj

	/**********************CCU8 Timer Setup*********************************/
	SCU_RESET->PRSET0|=0x00000080;	//Apply reset to the CCU8, via the specific SCU bit field on the PRSET0/PRSET1 register.
	SCU_RESET->PRCLR0|=0x00000080;	//Release reset of the CCU8, via the specific SCU bit field on the 	PRCLR0/PRCLR1 register.
	SCU_CLK->CLKSET|=0x00000010;	//Enable the CCU8 clock via the specific SCU register, CLKSET.
	/***********************CCU8 Global Setup*******************************/
	CCU80->GIDLC|=0x00000100;  	//	Enable the prescaler block, by writing 1B	to the GIDLC.SPRB field.
	CCU80->GCTRL|=0X00000000;  	//Configure the global CCU8 register GCTRL.

	/*******************************CCU80_CC80����**********************/
	CCU80_CC80->INS=0x61410007;				//�����¼�ѡ���¼���Ч��ƽ�������غ������˲����á� p2367
												//Event2	->	TRAP (LEVEL active)	->	EV2 IN0A Low Level active(P0.7) -Falling edge

	CCU80_CC80->CMC=0x00020001;  	//�ⲿ�¼��������ú� ʱ�Ӽ������� TRAP function connected to Event2

	CCU80_CC80->TC|=0x006E0001;	//CCU8��Ԫ��ʱ��������ʽ������ģʽ�������ⲿ���ƹ��ܵĽ�ֹ��ʹ�� 	p2375
												//��Ϊ ���Ķ��룬ʹ��shadow transfer��TRAP enabled forCC80��3�Խ�
												//TRAP ͬ��PWM��������TRAP

	CCU80_CC80->PSL|=0x00000000;   			//������������,����Ϊ�ߣ�����Ϊ�͡�
												//EV2	LP:11 7fccu8  EV2LM:0 High level

	CCU80_CC80->PSC=0x00000000;      		//Ԥ��Ƶʱ��

	CCU80_CC80->CHC|=0x00000000;    		//�Գ�PWM �Լ����ѡ��
	CCU80_CC80->DTC|=0x0000000D; 			//��������

	if(DriverPara.RatedPow >=2000)//20161227 mxj
	{
		CCU80_CC80->DC1R|=0x0000fcfc;			//ͨ��1����ʱ������  2.1us
		CCU80_CC80->DC2R|=0x0000fcfc;			//ͨ��2����ʱ������ 2.1us
	}
	//else //if(DriverPara.RatedPow >=700)//20170410 mxj
	else if(DriverPara.RatedPow >=750)//201801010
	{
		CCU80_CC80->DC1R|=0x0000bfbf;			//ͨ��1����ʱ������ 1.6us
		CCU80_CC80->DC2R|=0x0000bfbf;			//ͨ��2����ʱ������ 1.6us
	}
	else
	{
		CCU80_CC80->DC1R|=0x00008484;			//ͨ��1����ʱ������ 1.1us
		CCU80_CC80->DC2R|=0x00008484;			//ͨ��2����ʱ������ 1.1us
	}

	CCU80_CC80->INTE|=0x00000400; 			//Trap�ж�ʹ��
	CCU80_CC80->SRS|=0x00003000;  			//�жϷ������������ã�
	//CCU80_CC80->SWS|=0x00000000; 			//�ж�״̬λ����

	//mxj===========================================================================
	//CCU80_CC80->TC      |=
	//CCU80->GCSS         |= 0x01;			// Global Channel Set: Slice 0 shadow transfer set enable
	//CCU80->GIDLC        |= 0x01;			// Global Idle Clear:n CC80 IDLE mode clear, Prescaler Run Bit Set
	//CCU80_CC80->TCSET    = 0x01;			// Run
	//CCU80_CC80->INTE	|=0x00000001; 			//����ƥ��
	CCU80_CC80->INTE	|=0x00000003; 			//����ƥ���1ƥ��  20181028
	CCU80_CC80->SRS		|=0x00000002;  			//10B ����ƥ�䴫�͵�CCU80SR2
	//mxj===========================================================================

	CCU80_CC80->DITS=0x00000000;	  		//����ʱ������
	CCU80_CC80->PRS=g_PwmPeriod;
	CCU80_CC80->CR1S=PwmCompareValue;
	/************************CCU80_CC81����************************************/
	CCU80_CC81->INS=0x61410007;				//�����¼�ѡ���¼���Ч��ƽ�������غ������˲����á� p2367
												//Event2	->	TRAP (LEVEL active)	->	EV2 IN0A Low Level active(P0.7)

	CCU80_CC81->CMC=0x00020001;  			//�ⲿ�¼��������ú� ʱ�Ӽ�������
												//Enable Trap function

	CCU80_CC81->TC|=0x006E0001;	  			//CCU8��Ԫ��ʱ��������ʽ������ģʽ�������ⲿ���ƹ��ܵĽ�ֹ��ʹ�� 	p2375
												//��Ϊ ���Ķ��룬ʹ��shadow transfer��TRAP enabled forCC80���ĸ���
												//TRAP ͬ��PWM��������TRAP
												//External Modulation as outputs gate, sync with PWM

	CCU80_CC81->PSL|=0x00000000;   			//������������,����Ϊ�ߣ�����Ϊ�͡�
												//EV2	LP:11 7fccu8  EV2LM:0 High level

	CCU80_CC81->PSC=0x00000000;      		//Ԥ��Ƶʱ��

	CCU80_CC81->CHC|=0x00000000;    		//�Գ�PWM �Լ����ѡ��
	CCU80_CC81->DTC|=0x0000000D; 			//��������
	//CCU80_CC81->DC1R|=0x0000bfbf;			//ͨ��1����ʱ������
	//CCU80_CC81->DC2R|=0x0000bfbf;			//ͨ��2����ʱ������

	if(DriverPara.RatedPow >=2000)//20161227 mxj
	{
		CCU80_CC81->DC1R|=0x0000fcfc;			//ͨ��1����ʱ������  2.1us
		CCU80_CC81->DC2R|=0x0000fcfc;			//ͨ��2����ʱ������ 2.1us
	}
	//else //if(DriverPara.RatedPow >=700)//20170410 mxj
	else if(DriverPara.RatedPow >=750)//201801010
	{
		CCU80_CC81->DC1R|=0x0000bfbf;			//ͨ��1����ʱ������ 1.6us
		CCU80_CC81->DC2R|=0x0000bfbf;			//ͨ��2����ʱ������ 1.6us
	}
	else
	{
		CCU80_CC81->DC1R|=0x00008484;			//ͨ��1����ʱ������ 1.1us
		CCU80_CC81->DC2R|=0x00008484;			//ͨ��2����ʱ������ 1.1us
	}

	//CCU80_CC81->DC1R|=0x00005f5f;			//ͨ��1����ʱ������
	//CCU80_CC81->DC2R|=0x00005f5f;			//ͨ��2����ʱ������

	CCU80_CC81->INTE|=0x00000400; 			//�ж�ʹ��
	CCU80_CC81->SRS|=0x00003000;  			//�жϷ�������������

	CCU80_CC81->DITS=0x00000000;  			//����ʱ������
	CCU80_CC81->PRS=g_PwmPeriod;
	CCU80_CC81->CR1S=PwmCompareValue;

	/****************************CCU80_CC82***************************************/
	CCU80_CC82->INS=0x61410007;				//�����¼�ѡ���¼���Ч��ƽ�������غ������˲����á� p2367
												//Event0	->	CCU82.IN0H	-> SCU.GLCCST80 -> Rising edge
												//Event0	->	CCU82.IN0H	-> SCU.GLCCST80 -> Low level active
												//Event2	->	TRAP (LEVEL active)	->	EV2 IN0A Low Level active(P0.7)

	CCU80_CC82->CMC=0x00020001;  			//�ⲿ�¼��������ú� ʱ�Ӽ�������
												//Enable Trap function

	CCU80_CC82->TC|=0x006E0001;	  			//CCU8��Ԫ��ʱ��������ʽ������ģʽ�������ⲿ���ƹ��ܵĽ�ֹ��ʹ�� 	p2375
												//��Ϊ ���Ķ��룬ʹ��shadow transfer��TRAP enabled forCC80���ĸ���
												//TRAP ͬ��PWM��������TRAP
												//External Modulation as outputs gate, sync with PWM

	CCU80_CC82->PSL|=0x00000000;   			//������������,����Ϊ�ߣ�����Ϊ�͡�
												//EV2	LP:11 7fccu8  EV2LM:0 High level

	CCU80_CC82->PSC=0x00000000;      		//Ԥ��Ƶʱ��

	CCU80_CC82->CHC|=0x00000000;    		//�Գ�PWM �Լ����ѡ��
	CCU80_CC82->DTC|=0x0000000D; 			//��������

	if(DriverPara.RatedPow >=2000)//20161227 mxj
	{
		CCU80_CC82->DC1R|=0x0000fcfc;			//ͨ��1����ʱ������  2.1us
		CCU80_CC82->DC2R|=0x0000fcfc;			//ͨ��2����ʱ������ 2.1us
	}
	//else //if(DriverPara.RatedPow >=700)//20170410 mxj
	else if(DriverPara.RatedPow >=750)//201801010
	{
		CCU80_CC82->DC1R|=0x0000bfbf;			//ͨ��1����ʱ������ 1.6us
		CCU80_CC82->DC2R|=0x0000bfbf;			//ͨ��2����ʱ������ 1.6us
	}
	else
	{
		CCU80_CC82->DC1R|=0x00008484;			//ͨ��1����ʱ������ 1.1us
		CCU80_CC82->DC2R|=0x00008484;			//ͨ��2����ʱ������ 1.1us
	}

	//CCU80_CC82->DC1R|=0x00005f5f;			//ͨ��1����ʱ������
	//CCU80_CC82->DC2R|=0x00005f5f;			//ͨ��2����ʱ������


	CCU80_CC82->INTE|=0x00000400; 			//�ж�ʹ��
	CCU80_CC82->SRS|=0x00003000;  			//�жϷ�������������

	CCU80_CC82->DITS=0x00000000;  			//����ʱ������
	CCU80_CC82->PRS=g_PwmPeriod;
	CCU80_CC82->CR1S=PwmCompareValue;

	//20180920*****************************************************************************************
	CCU81_CC80->TCCLR |= 0x07;//CCU43_CC43->TIMER set 0  20180920
	//CCU81_CC80->PRS  = 59991;//(7498+1)*8-1
	CCU81_CC80->PRS  = ((CCU43_CC43->PRS + 1)<<3) - 1;//(7498+1)*8-1
	CCU81_CC80->CR1S = CCU81_CC80->PRS>>1;
	CCU81_CC80->TIMER = 0;
	//SCU_GENERAL->CCUCON|=0x00000200;// CCU81 mxj 20190926
	CCU81->GCSS=0x00000001;	//Enable the Shadow Register Transfer
	//20180920*****************************************************************************************

	/*********************************��������******************************************/

	CCU80->GIDLC=0x0000010f; 				//�������״̬ ������ԪԤ��Ƶʱ��
	CCU80->GCSS|=0x00000fff;     			//ʹ����Ӱ����

	CCU80_CC80->SWR|=0x00000c00;
	CCU80_CC81->SWR|=0x00000c00;
	CCU80_CC82->SWR|=0x00000c00;//INTS set 0

	if(DriverPara.RatedVol == 220)
		CCU80_CC82->SWS |= 0x00C00;//bit11  STRPF bit10 SE2A  20180713
	else
		CCU80_CC80->SWS |= 0x00C00;//bit11  STRPF bit10 SE2A  20180713

	CCU43_CC43->TCCLR |= 0x07;//CCU43_CC43->TIMER set 0
	//CCU43_CC43->TIMER = (CCU43_CC43->PR >> 1);//20170730
	CCU43_CC43->TIMER = (CCU43_CC43->PR >> 3)*5;//20170730
	SCU_GENERAL->CCUCON|=0x00000100;//mxj 20160510
	CCU43_CC43->TCSET |= 0x01;

	CCU81_CC80->TCSET |= 0x01;//20180920

//	PwmOutputEnable();
//	IO004_SetOutputValue(IO004_Handle27,1);

}
/*************************************************
  Function: Pwm Period Calc
  Description:Period:Init/1ms;Get Carry-Frequency and Pwm Period Value
  Input: No
  Return: No
  Others: Be Called CCU8Init()
*************************************************/
void CCU8GetPeriod(uint8_t CarryPwmFreq)
{
	float Period;

	/*Limit the Carry-Frequency Min and Max*/
	if(CarryPwmFreq<10)
		CarryPwmFreq=10;	//1khz
	if(CarryPwmFreq>200)
		CarryPwmFreq=200;	//20khz

	Period=(float)1/(float)CarryPwmFreq;
	g_PwmPeriod=((uint16_t)(Period*1200000)>>1)-1;//20180926

	CCU80_CC80->CR1S=(uint16_t)((float)((float)(g_PwmPeriod+1)/(float)CCU80_CC80->PRS)*CCU80_CC80->CR1S);
	CCU80_CC81->CR1S=(uint16_t)((float)((float)(g_PwmPeriod+1)/(float)CCU80_CC81->PRS)*CCU80_CC81->CR1S);
	CCU80_CC82->CR1S=(uint16_t)((float)((float)(g_PwmPeriod+1)/(float)CCU80_CC82->PRS)*CCU80_CC82->CR1S);

	CCU80_CC80->PRS=g_PwmPeriod;
	CCU80_CC81->PRS=g_PwmPeriod;
	CCU80_CC82->PRS=g_PwmPeriod;
	CCU80->GCSS|=0x00000fff;
}

void PwmOutputDisable_W(void)
{
	if(DriverPara.RatedVol == 220)
	{
		if((CCU80_CC82->INTS&0x00000C00) == 0x00000C00)
			return;
		CCU80_CC82->SWS |= 0x00C00;//bit11  STRPF bit10 SE2A
	}
	else
	{
		if((CCU80_CC80->INTS&0x00000C00) == 0x00000C00)
				return;
		CCU80_CC80->SWS |= 0x00C00;//bit11  STRPF bit10 SE2A
	}
}
void PwmOutputEnable_W(void)
{
	//if(PORT0->IN & 0x00000080)				//��TRAP��ƽP0.7����Ϊ�ߵ�ƽ ��ִ��
	if(DriverPara.RatedVol == 220)
	{
		if((PORT0->IN & 0x00000080)&&(CCU80_CC82->INTS&0x00000C00))
		{
			CCU80_CC82->SWR |= 0x00C00;//bit11  RTRPF  bit10 E2AS
		}
	}
	else
	{
		if((PORT0->IN & 0x00000080)&&(CCU80_CC80->INTS&0x00000C00))
		{
			CCU80_CC80->SWR |= 0x00C00;//bit11  RTRPF  bit10 E2AS
		}
	}
}

/*************************************************
  Function: Pwm Output Enable
  Description: Controlled Servo-on Cmd/Motor Run Cmd
  Input: No
  Return: No
  Others:
*************************************************/
void PwmOutputEnable(void)
{
	/*PWM Valid*/
	PwmOutputEnable_W();//20180713
	IO004_SetOutputValue(IO004_Handle27,1);
	//CCU80 External Modulation Inactive, CC8x output Active Level.
	//SCU_GENERAL->CCUCON|=0x00000100;//mxj
}

/*************************************************
  Function: Pwm Output Disable
  Description: Controlled Servo-Off Cmd/Motor Stop Cmd/Alarm Generate
  Input: No
  Return: No
  Others:
*************************************************/
void PwmOutputDisable(void)
{
	int8_t Stop_Type;
	/*PWM Invalid*/

	Stop_Type = FuncCode_Handle.Ram.F10.F1049 & 0x000F;
	if(Stop_Type == 1)
	{
		IO004_SetOutputValue(IO004_Handle27,0);
	}
	else if(Stop_Type == 0)
	{
		PwmOutputDisable_W();//20180713
	}
	else
	{
		PwmOutputEnable_W();//20180713
	}

//IO004_SetOutputValue(IO004_Handle27,1);
}


/*************************************************
  Function: Pwm Trap Interrupt
  Description: Pwm FO Protect
  Input: No
  Return: No
  Others: Interrupt Vector Address: CCU80_3_IRQHandler
*************************************************/
void PwmTrap_Isr(void)
{
	//if((CCU80_CC80->INTS&0x00000C00)||(CCU80_CC81->INTS&0x00000C00)||(CCU80_CC82->INTS&0x00000C00))
	if(DriverPara.RatedVol == 220)
	{
		if((CCU80_CC80->INTS&0x00000C00)||(CCU80_CC81->INTS&0x00000C00))//20180713
		{
			//EVErrProc(ERR_OC_DRIVE);
			AlarmStatusFlag.bit.Driver.FO = OK;
			IO004_SetOutputValue(IO004_Handle27,0);
			//g_canview14 += 1677721;
			//FuncCode_Handle.Ram.FA0.FA059++;
		}
	}
	else
	{
		if((CCU80_CC82->INTS&0x00000C00)||(CCU80_CC81->INTS&0x00000C00))//20180713
		{
			AlarmStatusFlag.bit.Driver.FO = OK;
			IO004_SetOutputValue(IO004_Handle27,0);
		}
	}
	FuncCode_Handle.Ram.FA0.FA059++;//Add 1 when stop
}

/*************************************************
  Function: Clear Pwm Trap Interrupt Flag
  Description:
  Input: No
  Return: No
  Others: Interrupt Vector Address: CCU80_3_IRQHandler
*************************************************/
uint8_t ResetTrapIsr(void)
{
	/*If the Trap Pin(P0.7) is high level, Clear Flag will be executed*/
	if(PORT0->IN & 0x00000080)				//��TRAP��ƽP0.7����Ϊ�ߵ�ƽ ��ִ��
	{
	    CCU80_CC80->SWR|=0x00000c00;
	    CCU80_CC81->SWR|=0x00000c00;
	    CCU80_CC82->SWR|=0x00000c00;

	    return(OK);
	}
	else
	{
		return(NG);
	}
}
/*************************************************
  Function: CCU8 Module
  Description:
  Input: No
  Return: No
  Others: Interrupt Vector Address: CCU80_3_IRQHandler
*************************************************/
void PwmIOInit(void)
{
	//Clear Pin Register Status
	PORT0->OMR|=0x00000000;
	PORT0->PDR0&=0xff000000;
	PORT0->IOCR0|=0x80808080;
	PORT0->IOCR4|=0x00008080;
	PORT0->OUT|=0x0000003f; 	//Set High Level

	/*Pwm Output IO Setup*/
	P0_5_set_mode(OUTPUT_PP_AF3);	//CCU8.out00
	P0_5_set_driver_strength(MEDIUM);

	P0_2_set_mode(OUTPUT_PP_AF3);	//CCU8.out01
	P0_2_set_driver_strength(MEDIUM);

	P0_4_set_mode(OUTPUT_PP_AF3);	//CCU8.out10
	P0_4_set_driver_strength(MEDIUM);

	P0_1_set_mode(OUTPUT_PP_AF3);	//CCU8.out11
	P0_1_set_driver_strength(MEDIUM);

	P0_3_set_mode(OUTPUT_PP_AF3);	//CCU8.out20
	P0_3_set_driver_strength(MEDIUM);

	P0_0_set_mode(OUTPUT_PP_AF3);	//CCU8.out21
	P0_0_set_driver_strength(MEDIUM);

	/*Pwm Trap IO Setup*/
	P0_7_set_mode(INPUT_PU);	//CCU8.out21
	P0_7_set_driver_strength(MEDIUM);
	PORT0->HWSEL	&=0xFFFF3FFF;	//PORT0 hardware select: P0.7-- software control only
}










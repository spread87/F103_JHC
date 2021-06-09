/*
 * peri_InitCCU4.c
 *
 *  Created on: 2014-10-30
 *  Author: LvSL
 *
 */
#include <main.h>			//Declarations from DAVE3 Code Generation (includes SFR declaration)
#include <peri_GlobalVariablesExtern.h>
#include <peri_GPIO.h>


/*************************************************
  Function: CCU4 Init
  Description:Period:Init
  Input: No
  Return: No
  Others: Be Called InitModule()
*************************************************/
void CCU4Init(void)
{
	/*Configure for POSIF0*/
	CCU40Init();//zphase_isr_init

	/*Configure for POSIF1 ��������*/
	CCU41Init();

	/*Configure for Pulse Output �������*/
	CCU42Init();

}
/*************************************************
  Function: CCU40 Init
  Description:Period:Init; For POSIF0
  Input: No
  Return: No
  Others: Be Called CCU4Init()
*************************************************/
void CCU40Init(void)
{
	//CYL==============================================
	SCU_RESET->PRSET0 |= 0x00000004;            //��λCCU40
	SCU_RESET->PRCLR0 |= 0x00000004;            //��λ�ͷ�CCU40
	SCU_CLK->CLKSET |= 0x00000010;              //ʹ��CCU40ʱ��
	CCU40->GIDLC |= 0x00000100;                 //����CCU40
	CCU40->GCTRL |= 0x00000000;                 //��CCU40��Ԫȫ�ּĴ�����������

	CCU40_CC40->PSC |= 0x00000006;              //����PreScaler��ʼֵ
	CCU40_CC40->FPC |= 0x00000600;              //����Ƶ��ΪFccu/64
	CCU40_CC40->INS |= 0x00950654;              //IN0E��Ϊ�¼�0,IN0F��Ϊ�¼�1��IN0G��Ϊ�¼�2
	CCU40_CC40->CMC |= 0x00004830;              //�趨0�¼��������ܣ��¼�1����/�¼������¼�2����0
	CCU40_CC40->TC  |= 0x00001048;              //���ض��룬���������ʱ��
	CCU40_CC40->PRS = 0x270F;                   //��������ֵΪ9999
	CCU40_CC40->CRS |= 0x1;                     //���ñȽ�ֵ
	CCU40_CC40->INTE |= 0x400;					//ʹ��Z�ж� 20170122 ����λ��
	CCU40_CC40->SRS |= 0x3000;					//���ж�ӳ�䵽CC40.SR3

	CCU40_CC41->PSC |= 0x00000000;              //����PreScaler��ʼֵ
	CCU40_CC41->FPC |= 0x00000000;              //����Ƶ��ΪFccu/1

  //CCU40_CC41->INS |= 0x000900FB;              //IN0L��Ϊ�¼�0,IN1P��Ϊ�¼�1����Ϊ��������Ч
	CCU40_CC41->INS |= 0x011905FB;              //IN0L��Ϊ�¼�0,IN1P��Ϊ�¼�1����Ϊ��������Ч,IN1F��Ϊ�¼�2 20160725
 // CCU40_CC41->CMC |= 0x00004020;              //�趨0�¼��������ܣ��¼�1����0
	CCU40_CC41->CMC |= 0x00004C20;              //�趨0�¼��������ܣ��¼�1����0���¼�2�������� 20160725
	CCU40_CC41->TC  |= 0x00001048;              //���ض��룬���������ʱ��
	CCU40_CC41->PRS = 0xFFFF;                   //��������ֵΪ65535
	CCU40_CC41->CRS |= 0x1;                     //���ñȽ�ֵ

	CCU40_CC42->PSC |= 0x00000005;              //����PreScaler��ʼֵ
	CCU40_CC42->FPC |= 0x00000500;              //����Ƶ��ΪFccu/32

	CCU40_CC42->INS |= 0x000900F5;              //IN2F��Ϊ�¼�0,INSP��Ϊ�¼�1����Ϊ��������Ч
	CCU40_CC42->CMC |= 0x00000021;              //�趨0�¼��������¼�1����
	CCU40_CC42->TC  |= 0x00001408;              //���ض��룬������������ʱ�����������
	CCU40_CC42->PRS = 0xFFFF;                   //��������ֵΪ65535
	CCU40_CC42->CRS |= 0x1;                     //���ñȽ�ֵ

	CCU40_CC43->PSC |= 0x00000006;              //����PreScaler��ʼֵ
	CCU40_CC43->FPC |= 0x00000600;              //����Ƶ��ΪFccu/64
	CCU40_CC43->INS |= 0x00010005;              //IN3F��Ϊ�¼�0,��������Ч
	CCU40_CC43->CMC |= 0x00000001;              //�趨0�¼�����
	CCU40_CC43->TC  |= 0x00000000;              //���ض��룬�Ƚ�ģʽ
	CCU40_CC43->PRS = 0x0EA4;                   //��������ֵΪ2ms
	//CCU40_CC43->PRS = 0x0EA4>>1;                   //��������ֵΪ1ms
	CCU40_CC43->CRS |= 0x0752;                  //���ñȽ�ֵ

	CCU40->GCSS |= 0x1111;                      //ʹ��CC40,CC41,CC42,CC43��ӳ��
	CCU40->GIDLC |= 0x0000000F;                 //����CC40,CC41,CC42,CC43
	CCU40_CC40->TCSET |= 0x1;                   //������ʱ��CC40
	CCU40_CC41->TCSET |= 0x1;                   //������ʱ��CC41
	CCU40_CC42->TCSET |= 0x1;                   //������ʱ��CC42
	//CYL==============================================
}
/*************************************************
  Function: CCU41 Init
  Description:Period:Init; For POSIF1
  Input: No
  Return: No
  Others: Be Called CCU4Init()
*************************************************/
void CCU41Init(void)
{
	uint32_t FcodeValue = 0;

	FcodeValue = FuncCode_Handle.Ram.F10.F1005;

    SCU_RESET->PRSET0 |= 0x00000008;         //Apply reset to the CCU41
    SCU_RESET->PRCLR0 |= 0x00000008;        	//Release reset of the CCU41
    SCU_CLK->CLKSET |= 0x00000010;           	//Enable the CCU4 clock
    CCU41->GIDLC |= 0x00000100;               	//Enable the prescaler block
    CCU41->GCTRL |= 0x00000000;              	//Configure the global CCU41

    if(FcodeValue == PULSECCWCW)
    {
        CCU41_CC40->PSC |= 0x00000000; 		 	//Setup PreScaler Value
        CCU41_CC40->FPC |= 0x00000000;
        CCU41_CC40->TC  |= 0x00000000;			//Set count mode
        CCU41_CC40->INS |= 0x06010000;
        CCU41_CC40->CMC |= 0x00004000;
        CCU41_CC40->PRS |= 0xFFFF;
        CCU41_CC40->CRS |= 0x100;
        CCU41_CC40->INTE|= 0x01;	//Period Interrupt Enable
        CCU41_CC40->SRS |= 0x00;	//CCU41_0_IRQHandler

        CCU41_CC41->PSC |= 0x00000000; 		 	//Setup PreScaler Value
        CCU41_CC41->FPC |= 0x00000000;
        CCU41_CC41->TC  |= 0x00000000;			//Set count mode
        CCU41_CC41->INS |= 0x06010000;
        CCU41_CC41->CMC |= 0x00004000;
        CCU41_CC41->PRS |= 0xFFFF;
        CCU41_CC41->INTE|= 0x01;	//Period Interrupt Enable
        CCU41_CC41->SRS |= 0x00;	//CCU41_0_IRQHandler

        CCU41->GCSS |= 0x0011;
        CCU41->GIDLC |= 0x03;
        CCU41_CC40->TCSET |= 0x00000001;
        CCU41_CC41->TCSET |= 0x00000001;

        P2_4_set_mode(INPUT_PU);					//Configure P2.4 as Input Mode
        P2_4_set_driver_strength(MEDIUM);
        P2_5_set_mode(INPUT_PU);					//Configure P2.5 as Input Mode
        P2_5_set_driver_strength(MEDIUM);

    }
    else
    {
    	/*Configure CCU41_CC40*/
    	CCU41_CC40->PSC |= 0x00000001; 		 	//Setup PreScaler Value
    	//CCU41_CC40->INS |= 0x000D0054;	//CYL 20160819
    	CCU41_CC40->INS |= 0x008D0054;		//Input Selector Unit Configuration
    										//Event 0 Signal Selection: CCU41.IN0E(Interconnects:POSIF1.OUT0)
    										//Event 1 Signal Selection: CCU41.IN0F(Interconnects:POSIF1.OUT1)
    	CCU41_CC40->CMC |= 0x00004800;			//External Up/Down Function triggered by Event 1
    										//External Count Function triggered by Event 0
    	CCU41_CC40->TC  |= 0x00000004;			//Setting this bit to 1 enables a shadow transfer
    	CCU41_CC40->PRS |= 0xFFFF;  			//10000 Pulses via Revolution
    	CCU41_CC40->CRS |= 0x100;					//Configure Compare Value

    	CCU41_CC40->INTE |= 0x01;				//Enable Period Match Interrupt
    	CCU41_CC40->SRS |= 0x00;				//Interrupt Pointing IRQ_48(CCU41_0_IRQHandler)
    	/*Configure the startup value for a specific Compare Channel Status*/
    	CCU41->GCSS |= 0x0001;
    	CCU41->GIDLC |= 0x01;//�˳�����ģʽ
    	CCU41_CC40->TCSET |= 0x00000001;//������ʱ��
    }
}
/*************************************************
  Function: CCU42 Init
  Description:Period:Init; For Pulse Output
  Input: No
  Return: No
  Others: Be Called CCU4Init()
*************************************************/
void CCU42Init(void)
{
	SCU_RESET->PRSET0 |= 0x00000010;            //��λCCU42
	SCU_RESET->PRCLR0 |= 0x00000010;            //��λ�ͷ�CCU42
	SCU_CLK->CLKSET |= 0x00000010;              //ʹ��CCU4ʱ��
	CCU42->GIDLC |= 0x00000100;                 //����CCU42
	CCU42->GCTRL |= 0x00000000;                 //��CCU42��Ԫȫ�ּĴ�����������

	CCU42_CC40->PSC |= 0x00000000;              //����PreScaler��ʼֵ
	CCU42_CC40->FPC |= 0x00000000;              //����Ƶ��ΪFccu
	CCU42_CC40->INS |= 0x001F08DF;              //IN0P��Ϊ�¼�0,IN0N��Ϊ�¼�1����Ϊ���±�����Ч��INOI��Ϊ�¼�2��Ϊ��������Ч
	CCU42_CC40->CMC |= 0x00000003;              //�¼�2��Ϊ�ⲿ�����ź�
	CCU42_CC40->TC  |= 0x00000000;              //���ض���
	CCU42_CC40->PRS = 0xFFFF;                   //��������ֵ
	CCU42_CC40->CRS |= 0xFFFF;                     //���ñȽ�ֵ

	CCU42_CC41->PSC |= 0x00000000;              //����PreScaler��ʼֵ
	CCU42_CC41->FPC |= 0x00000000;              //����Ƶ��ΪFccu
	CCU42_CC41->INS |= 0x0011080C;              //IN1M��Ϊ�¼�0,��������Ч,IN1I��Ϊ�¼�2��Ϊ��������Ч
	CCU42_CC41->CMC |= 0x00000007;              //�¼�0��Ϊ�ⲿֹͣ�ź�,�¼�2��Ϊ�ⲿ�����ź�
	CCU42_CC41->TC  |= 0x00000000;              //���ض���
	CCU42_CC41->PRS = 0xFFFF;                   //��������ֵ
	CCU42_CC41->CRS |= 0xFFFF;                     //���ñȽ�ֵ

#if 1
	CCU42_CC42->PSC |= 0x00000000;              //����PreScaler��ʼֵ
	CCU42_CC42->FPC |= 0x00000000;              //����Ƶ��ΪFccu
	CCU42_CC42->INS |= 0x00000000;              //
	CCU42_CC42->CMC |= 0x00000000;              //
	CCU42_CC42->TC  |= 0x00000002;              //���ض���,����ģʽ
	CCU42_CC42->PRS = 0xFFFF;                   //��������ֵ
	CCU42_CC42->CRS |= 0x0000;                     //���ñȽ�ֵ
#endif

	CCU42_CC43->PSC |= 0x00000000;              //����PreScaler��ʼֵ
	CCU42_CC43->FPC |= 0x00000000;              //����Ƶ��ΪFccu
	CCU42_CC43->INS |= 0x0011080C;              //IN3M��Ϊ�¼�0,��������Ч,IN3I��Ϊ�¼�2��Ϊ��������Ч
	CCU42_CC43->CMC |= 0x00000007;              //�¼�0��Ϊ�ⲿֹͣ�ź�,�¼�2��Ϊ�ⲿ�����ź�
	CCU42_CC43->TC  |= 0x00000000;              //���ض���
	CCU42_CC43->PRS = 0xFFFF;                   //��������ֵ
	CCU42_CC43->CRS |= 0xFFFF;                     //���ñȽ�ֵ

	CCU42->GCSS |= 0x1111;                      //ʹ��CC40,CC41,CC42,CC43��ӳ��
	CCU42->GIDLC |= 0x0000000F;                 //����CC40,CC41,CC42,CC43

	P3_3_set_mode(OUTPUT_PP_AF3);
	P3_3_set_driver_strength(MEDIUM);
	P3_4_set_mode(OUTPUT_PP_AF3);
	P3_4_set_driver_strength(MEDIUM);
	P3_5_set_mode(OUTPUT_PP_AF3);
	P3_5_set_driver_strength(MEDIUM);
}

/*************************************************
  Function: Pulse Input Period Interrupt
  Description: For Pulse Input Calculate;
  Input: No
  Return: No
  Others: Interrupt Vector Address: CCU41_0_IRQHandler
*************************************************/
void PulseInputPeriod_Isr(void)
{
	uint32_t FcodeValue = 0;

	FcodeValue = FuncCode_Handle.Ram.F10.F1005;

	if(FcodeValue == PULSECCWCW)
	{
		if(CCU41_CC40->INTS & 0x00000001)
		{
			g_PulInPrdCCWCount++;
		}
		else if(CCU41_CC41->INTS & 0x00000001)
		{
			g_PulInPrdCWCount++;
		}
	}
	else
	{

	}
}








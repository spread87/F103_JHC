/*
 * peri_InitADC.c
 *
 * Created on: 2014-10-28
 * Author: LvSL
 */
#include <main.h>			//Declarations from DAVE3 Code Generation (includes SFR declaration)
#include <peri_GlobalVariablesExtern.h>

/*************************************************
  Function:ADC Init
  Description:Period:Init
  Input: No
  Return: No
  Others: Be Called InitModule()
*************************************************/
void ADCInit0(void)//�Զ�ɨ��ģ���������ͨ����������
{

//CYL 20160713=====================================================
	SCU_RESET->PRSET0 |= 0x01; //��λADC
	SCU_RESET->PRCLR0 |= 0x01; //ADC��λ���
	VADC->CLC	&=	0xfffffffe;//����ģ��ʱ��
	while(VADC->CLC!=0);	   //�ȴ�ʱ������
	VADC->GLOBCFG=0x00008002;  //ʱ��3��Ƶ
	//VADC->GLOBCFG=0x00008006;  //ʱ��7��Ƶ

	VADC_G0->QINR0=0x000000A0; 	//CH0ѹ����У�������䣬�ȴ������¼�,IU  P14.0
	VADC_G0->QINR0=0x00000021; 	//CH1ѹ����У��������,IV            P14.1
	VADC_G0->QINR0=0x00000024; 	//CH4ѹ����У��������,TEMP			p14.4
	VADC_G0->QINR0=0x00000025; 	//CH5ѹ����У��������,AI1			P14.5
	VADC_G0->QINR0=0x00000026; 	//CH6ѹ����У��������,AI2
	VADC_G0->QINR0=0x00000027; 	//CH7ѹ����У��������,AI3
//	VADC_G0->QINR0=0x00000020; 	//CH0ѹ����У�������䣬�ȴ������¼�,IU  20171206
//	VADC_G0->QINR0=0x00000021; 	//CH1ѹ����У��������,IV
	VADC_G1->QINR0=0x000000A4; 	//CH4ѹ����У�������䣬�ȴ������¼�,1.65V
	VADC_G1->QINR0=0x00000025; 	//CH5ѹ����У��������,1.414V
	VADC_G1->QINR0=0x00000026; 	//CH6ѹ����У��������,PTC
	VADC_G3->QINR0=0x000000A0; 	//CH0ѹ����У�������䣬�ȴ������¼�,V_BUS


	VADC_G0->QMR0 =0x00000005; //�����ⲿ����
	VADC_G1->QMR0 =0x00000005; //�����ⲿ����
	VADC_G3->QMR0 =0x00000005; //�����ⲿ����

	VADC_G0->QCTRL0=0x0000C800; //CCU80SR2����ƥ����Ϊ����Դ�������ش���
	VADC_G1->QCTRL0=0x0000C800;
	VADC_G3->QCTRL0=0x0000C800;


	VADC_G0->CHCTR[0]=0x00000000;	//ADת��������������Ĵ���0
	VADC_G0->CHCTR[1]=0x00010000;
	VADC_G0->CHCTR[4]=0x00040000;
	VADC_G0->CHCTR[5]=0x00050000;
	VADC_G0->CHCTR[6]=0x00060000;
	VADC_G0->CHCTR[7]=0x00070000;
	//VADC_G0->CHCTR[2]=0x00000000;	//ADת��������������Ĵ���0  20171206
	//VADC_G0->CHCTR[3]=0x00010000;
	VADC_G1->CHCTR[4]=0x00040000;
	VADC_G1->CHCTR[5]=0x00050000;
	VADC_G1->CHCTR[6]=0x00060000;

	VADC_G3->CHCTR[0]=0x00000000;


	VADC_G0->ARBPR=0x0100000B;	 //����Դ0����������ȼ���ȡ�������ظ�ģʽ
	VADC_G1->ARBPR=0x0100000B;
	VADC_G3->ARBPR=0x0100000B;

	VADC_G0 ->ARBCFG = 0x00000003;         //��ADC
	VADC_G1 ->ARBCFG = 0x00000003;
	VADC_G3 ->ARBCFG = 0x00000003;
	while (VADC_G0 ->ARBCFG & 0x10000000); //�ȴ�У׼���
	while (VADC_G1 ->ARBCFG & 0x10000000);
	while (VADC_G3 ->ARBCFG & 0x10000000);
//CYL 20160713=====================================================

}
void ADC_Sample_Change(void)
{
	//VADC_G0->QCTRL0
}
void ADC_Trigger_Change(void)
{
	SCU_GENERAL->CCUCON|=0x00000100;

	//Define the sequence
	VADC_G0->QINR0	=	0x80;
	//Select the appropriate triggers
	VADC_G0->QCTRL0	= 0x8000C800;
	VADC_G0->QMR0  	= 0x05;
	VADC_G0->ARBCFG = 0x03;
	VADC_G0->ARBPR 	= 0x01000000;
	// wait for startup calibration finished
	while ((VADC_G0->ARBCFG & VADC_G_ARBCFG_CAL_Msk)!=0);

}

void ADCInit(void)//�Զ�ɨ��ģ���������ͨ����������
{
	SCU_RESET->PRSET0 |= 0x01; //��λADC
	SCU_RESET->PRCLR0 |= 0x01; //ADC��λ���
	VADC->CLC	&=	0xfffffffe;//����ģ��ʱ��
	while(VADC->CLC!=0);	   //�ȴ�ʱ������
	VADC->GLOBCFG=0x00008002;  //ʱ��3��Ƶ
	//VADC->GLOBCFG=0x00008006;  //ʱ��7��Ƶ

	VADC_G0->QINR0=0x000000A0; 	//CH0ѹ����У�������䣬�ȴ������¼�,IU  		P14.0
	VADC_G0->QINR0=0x00000021; 	//CH1ѹ����У��������,IV            	P14.1
	//VADC_G0->QINR0=0x00000020; 	//CH0ѹ����У��������,IU  			P14.0  20181028
	//VADC_G0->QINR0=0x00000021; 	//CH1ѹ����У��������,IV            P14.1
	VADC_G0->QINR0=0x00000020; 	//CH0ѹ����У��������,IU  				P14.0  20181028
	VADC_G0->QINR0=0x00000021; 	//CH1ѹ����У��������,IV            	P14.1

	//VADC_G0->QINR0=0x00000024; 	//CH4ѹ����У��������,TEMP			p14.4
	//VADC_G0->QINR0=0x00000025; 	//CH5ѹ����У��������,AI1			P14.5
	VADC_G0->QINR0=0x00000026; 	//CH6ѹ����У��������,AI2			p14.6
	VADC_G0->QINR0=0x00000027; 	//CH7ѹ����У��������,AI3			p14.7

//	VADC_G0->QINR0=0x00000020; 	//CH0ѹ����У�������䣬�ȴ������¼�,IU  20171206
//	VADC_G0->QINR0=0x00000021; 	//CH1ѹ����У��������,IV
	VADC_G1->QINR0=0x000000A4; 	//CH4ѹ����У�������䣬�ȴ������¼�,1.65V	P14.12
	VADC_G1->QINR0=0x00000025; 	//CH5ѹ����У��������,1.414V			P14.13
	VADC_G1->QINR0=0x00000026; 	//CH6ѹ����У��������,PTC				P14.14

	VADC_G2->QINR0=0x000000A0; 	//CH0ѹ����У��������,�ȴ������¼�TEMP		p14.4  20181028
	VADC_G2->QINR0=0x00000021; 	//CH1ѹ����У��������,AI1				P14.5
	VADC_G2->QINR0=0x00000021; 	//CH1ѹ����У��������,AI1				P14.5
	VADC_G2->QINR0=0x00000021; 	//CH1ѹ����У��������,AI1				P14.5

	VADC_G3->QINR0=0x000000A0; 	//CH0ѹ����У�������䣬�ȴ������¼�,V_BUS	P15.8
	VADC_G3->QINR0=0x00000020; 	//CH0ѹ����У�������䣬V_BUS				P15.8  20181028
	VADC_G3->QINR0=0x00000020; 	//CH0ѹ����У�������䣬V_BUS				P15.8

	VADC_G0->QMR0 =0x00000005; //�����ⲿ����
	VADC_G1->QMR0 =0x00000005; //�����ⲿ����
	VADC_G2->QMR0 =0x00000005; //�����ⲿ����  20181028
	VADC_G3->QMR0 =0x00000005; //�����ⲿ����

	VADC_G0->QCTRL0=0x0000C800; //CCU80SR2����ƥ����Ϊ����Դ�������ش���
	VADC_G1->QCTRL0=0x0000C800;
	VADC_G2->QCTRL0=0x0000C800;//20181028
	VADC_G3->QCTRL0=0x0000C800;

	//VADC_G0->RCR[7] =0x001A0000;	//ADת����Ĵ���7 ����FIR �˲� 121 20181028
	//VADC_G0->RCR[15]=0x001A0000;	//ADת������Ĵ���15
	//VADC_G0->RCR[7] =0x001C0000;	//ADת����Ĵ���7 ����FIR �˲� 112 20181028
	//VADC_G0->RCR[15]=0x001C0000;	//ADת������Ĵ���15
	VADC_G0->RCR[7] =0x00160000;	//ADת����Ĵ���7 ����FIR �˲� 220 20181028
	VADC_G0->RCR[15]=0x00160000;	//ADת������Ĵ���15

	VADC_G0->CHCTR[0]=0x00070000;	//ADת��������������Ĵ���7  20181028
	VADC_G0->CHCTR[1]=0x000F0000;	//ADת��������������Ĵ���15
	//VADC_G0->CHCTR[0]=0x00000000;	//ADת��������������Ĵ���0
	//VADC_G0->CHCTR[1]=0x00010000;
	VADC_G0->CHCTR[4]=0x00040000;
	VADC_G0->CHCTR[5]=0x00050000;
	VADC_G0->CHCTR[6]=0x00060000;
	//VADC_G0->CHCTR[7]=0x00070000;
	VADC_G0->CHCTR[7]=0x00080000;//20181028
	//VADC_G0->CHCTR[2]=0x00000000;	//ADת��������������Ĵ���0  20171206
	//VADC_G0->CHCTR[3]=0x00010000;
	VADC_G1->CHCTR[4]=0x00040000;
	VADC_G1->CHCTR[5]=0x00050000;
	VADC_G1->CHCTR[6]=0x00060000;

	VADC_G2->RCR[7] =0x001A0000;	//ADת����Ĵ���7 ����FIR �˲� 121 20181028
	VADC_G2->CHCTR[0]=0x00000000;	//ADת��������������Ĵ���0  20181028
	VADC_G2->CHCTR[1]=0x00070000;

	VADC_G3->RCR[7] =0x001A0000;	//ADת����Ĵ���7 ����FIR �˲� 121 20181028
	//VADC_G3->CHCTR[0]=0x00000000;
	VADC_G3->CHCTR[0]=0x00070000;

	VADC_G0->ARBPR=0x0100000B;	 			//����Դ0����������ȼ���ȡ�������ظ�ģʽ
	VADC_G1->ARBPR=0x0100000B;
	VADC_G2->ARBPR=0x0100000B;				// 20181028
	VADC_G3->ARBPR=0x0100000B;

	VADC_G0 ->ARBCFG = 0x00000003;         	//��ADC
	VADC_G1 ->ARBCFG = 0x00000003;
	VADC_G2 ->ARBCFG = 0x00000003;			// 20181028
	VADC_G3 ->ARBCFG = 0x00000003;
	while (VADC_G0 ->ARBCFG & 0x10000000); 	//�ȴ�У׼���
	while (VADC_G1 ->ARBCFG & 0x10000000);
	while (VADC_G2 ->ARBCFG & 0x10000000);	// 20181028
	while (VADC_G3 ->ARBCFG & 0x10000000);
}

void AdcReadResult(void)//��������12λ��ζ�Ӧ
{
	AdcResult[5] = VADC_G0->RES[6] & 0x00000fff;	//AI2
	//AdcResult[6] = VADC_G0->RES[7] & 0x00000fff;	//AI3
	AdcResult[6] = VADC_G0->RES[8] & 0x00000fff;	//AI3 20181028

	AdcResult[7] = VADC_G1->RES[4] & 0x00000fff;	//1.65V
	AdcResult[8] = VADC_G1->RES[5] & 0x00000fff;	//1.414V
	AdcResult[9] = VADC_G1->RES[6] & 0x00000fff;	//PTC

	AdcResult[3] = VADC_G2->RES[0] & 0x00000fff;	//Temp  20181028
	AdcResult[4] = VADC_G2->RES[7] & 0x00003fff;	//AI1   20181028

	//AdcResult[10] = VADC_G3->RES[0] & 0x00000fff;	//VDC
	AdcResult[10] = VADC_G3->RES[7] & 0x00003fff;	//VDC 20181028

	AdcResult[0] = VADC_G0->RES[7]  & 0x00003fff;	//Iu  20181028
	AdcResult[1] = VADC_G0->RES[15] & 0x00003fff;	//Iv  20181028
}


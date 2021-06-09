/*
 * peri_Parainit.c
 *
 *  Created on: 2019-12-23
 *      Author: HeLZ
 */

#include <main.h>
#include <peri_GlobalVariablesExtern.h>

//#define CG_Software_VERSION 1037//20200516 by mxj
#define CG_Software_VERSION 1038//20200711 by mxj
extern    void EepromWriteFcode(uint16_t ,uint32_t );
extern	  void AlmRcdClr(void);

void    ParameterF1toF8Restore(void)
{
	uint32_t  i;
	uint32_t value;

	FuncCode_Handle.Ram.F10 = FuncCode_Handle.Init.F10;//struct value directly set,the same struct
	FuncCode_Handle.E2prom.F10 =FuncCode_Handle.Init.F10;

	FuncCode_Handle.Ram.F50 = FuncCode_Handle.Init.F50;//struct value directly set,the same struct
	FuncCode_Handle.E2prom.F50 =FuncCode_Handle.Init.F50;

	FuncCode_Handle.Ram.F60 = FuncCode_Handle.Init.F60;//struct value directly set,the same struct
	FuncCode_Handle.E2prom.F60 =FuncCode_Handle.Init.F60;

	HAL_Delay(200);
				//for(i=1;i<=FCODE_TOTAL_Num;i++)//FCODE_F00ToF80_Num
	for(i=1+FCODE_F00_Num;i<=FCODE_F00ToF60_Num;i++)//F0��F9��FA�鹦���벻��Ҫ��ʼ��
	{
		value = FuncCodeRead(FcodeTable[i-1][1]);//read value from funccode_handle.e2prom
		////EepromWriteFcode(i,value);
		HAL_Delay(500);
	}
}

void    CGParameterRefresh(uint32_t  Charger_type)
{
		uint32_t num=0;

		uint32_t temp[32];
		uint32_t  i;

		const ChargerParameter* p=&Charger_lever[0];//20170620 mxj

		if(AlarmStatusFlag.bit.Motor.MDP)//20180906
					return;

		num = Charger_type-101;
		if(num<0)
			num = 0;
		else if(num>29)
			num = 29;

		p = &Charger_lever[num];//20170620 mxj
		temp[0] = p->RatedPow;//F0-04
		temp[1] = p->RatedCur;//F0-05
		temp[2] = p->RatedVol;//F0-06
		temp[3] = p->InputVolt;//F0-07
		temp[4] = p->Volt_L;//F0-08
		temp[5] = p->Volt_H;//F0-09
		temp[6] = p->Curr_L;//F0-10
		temp[7] = p->Curr_H;//F0-11
		//uint32_t *p_Ram = &FuncCode_Handle.Ram.F00.F0004;
		//uint32_t *p_E2prom = &FuncCode_Handle.E2prom.F00.F0004;
		for (i=0; i<8; i++)
		{
			//EepromWriteFcode(i+5,temp[i]);//�����ʲ���д��E2PROM
			FuncCodeChange(FcodeTable[i+4][1],temp[i],&FuncCode_Handle,NG);//д��sram,����д�� e2prom,ֻ�������˺��������޸�
			//*(p_Ram+i)=temp[i];
			//*(p_E2prom+i)=temp[i];
			HAL_Delay(500);
		}

		//EepromWriteFcode(3,Charger_type);//�洢��������  F0-02
		FuncCodeChange(FcodeTable[2][1],Charger_type,&FuncCode_Handle,NG);//д��sram,����д�� e2prom
		HAL_Delay(500);

	//	i=FuncCode_Handle.Ram.F00.F0007;

		//FuncCode_Handle.Ram.F10.F1030 = p->RatedVol;//20170104 mxj
		//FuncCode_Handle.Ram.F10.F1030 = p->Volt_H - 20;//20200209 mxj
		FuncCode_Handle.Ram.F10.F1030 = p->Volt_DF;//20200304 mxj
		FuncCode_Handle.E2prom.F10.F1030 = FuncCode_Handle.Ram.F10.F1030;
		FuncCode_Handle.Att.F10.F1030 |=0x0100;
		g_FcodeChangeFlag ++;

		//FuncCode_Handle.Ram.F10.F1031 = p->RatedCur;//20170104 mxj
		//FuncCode_Handle.Ram.F10.F1031 = p->RatedCur>>1;//20170104 mxj
		FuncCode_Handle.Ram.F10.F1031 = p->Curr_DF;//20200304 mxj
		FuncCode_Handle.E2prom.F10.F1031 = FuncCode_Handle.Ram.F10.F1031;
		FuncCode_Handle.Att.F10.F1031 |=0x0100;
		g_FcodeChangeFlag ++;

		FuncCode_Handle.Ram.F10.F1006 = p->Curr_LCMD;//20200516 mxj
		FuncCode_Handle.E2prom.F10.F1006 = FuncCode_Handle.Ram.F10.F1006;
		FuncCode_Handle.Att.F10.F1006 |=0x0100;
		g_FcodeChangeFlag ++;

		FuncCode_Handle.Ram.F10.F1008 = p->Volt_H;//20200516 mxj
		FuncCode_Handle.E2prom.F10.F1008 = FuncCode_Handle.Ram.F10.F1008;
		FuncCode_Handle.Att.F10.F1008 |=0x0100;
		g_FcodeChangeFlag ++;
}

uint8_t  CGParameterLimitCheck(uint32_t  Charger_type)
{
	uint32_t num=0;

	const ChargerParameter* p=&Charger_lever[0];//

	num = Charger_type-101;
	if(num<0)
		AlarmStatusFlag.bit.Motor.SETE = OK;
	else if(num>29)
		AlarmStatusFlag.bit.Motor.SETE = OK;
	else
	{
		p = &Charger_lever[num];//20170620 mxj
		if(FuncCode_Handle.Ram.F00.F0008 != p->Volt_L)
			AlarmStatusFlag.bit.Motor.SETE = OK;
		else if(FuncCode_Handle.Ram.F00.F0009 != p->Volt_H)
			AlarmStatusFlag.bit.Motor.SETE = OK;
		else if(FuncCode_Handle.Ram.F00.F0010 != p->Curr_L)
			AlarmStatusFlag.bit.Motor.SETE = OK;
		else if(FuncCode_Handle.Ram.F00.F0011 != p->Curr_H)
			AlarmStatusFlag.bit.Motor.SETE = OK;
	}
	return AlarmStatusFlag.bit.Motor.SETE;
}

void    CGParameterInit()
{
//	uint32_t num=0;
//	uint32_t num1=0;
	uint32_t temp[3];
	uint32_t  i;
	uint32_t  Charger_type;
//	uint32_t fresh_flag;
	uint32_t value;
	static uint8_t counter;

	FuncCode_Handle.Max.F10.F1030 = FuncCode_Handle.Ram.F10.F1008;//�����ѹ����

	FuncCode_Handle.Ram.F00.F0000 = CG_Software_VERSION;
	FuncCode_Handle.E2prom.F00.F0000 = CG_Software_VERSION;

	switch  (g_SpecialSave)
	{
	  case Normal:
		    break;
	  case ParameterInit1:
	  {
		    g_SpecialSave=0;
		    break;
	  }
	  case ParameterInit2://�û�������ʼ�����������ϲ���
	  {
		  ParameterF1toF8Restore();

		  ////AlmRcdClr();//���

		  FuncCode_Handle.Ram.F10.F1001 = 0;
		  FuncCode_Handle.E2prom.F10.F1001 = 0;

		  g_SpecialSave=0;
		    break;
	  }
	  case ParameterInit3://�û�������ʼ�������������ϲ���
	  {
		  ParameterF1toF8Restore();

		  FuncCode_Handle.Ram.F10.F1001 = 0;
		  FuncCode_Handle.E2prom.F10.F1001 = 0;

		  g_SpecialSave=0;
		  break;
	  }
	  case ParameterInit4://���ϲ�����ʼ��
	  {
		 ////AlmRcdClr();//���

		 FuncCode_Handle.Ram.F10.F1001 = 0;
		 FuncCode_Handle.E2prom.F10.F1001 = 0;

		  g_SpecialSave=0;
		  break;
	  }
//ȫ��������ʼ��
	  case ParameterInit5://ȫ��������ʼ��
	  {
			//drive_level = FuncCode_Handle.Ram.F00.F0004;//����
			Charger_type =  FuncCode_Handle.Ram.F00.F0002;//�������� 100

			FuncCode_Handle.Ram = FuncCode_Handle.Init;//struct value directly set,the same struct
			FuncCode_Handle.E2prom =FuncCode_Handle.Init;
			HAL_Delay(500);
			//for(i=1;i<=FCODE_TOTAL_Num;i++)//FCODE_F00ToF80_Num
			for(i=1;i<=FCODE_F00ToF60_Num;i++)//F9��FA�鹦���벻��Ҫ��ʼ��
			{
				value = FuncCodeRead(FcodeTable[i-1][1]);//read value from funccode_handle.e2prom
				//EepromWriteFcode(i,value);
				HAL_Delay(500);
			}

			//AlmRcdClr();//���

			CGParameterRefresh(Charger_type);//20170105 mxj

			FuncCode_Handle.Ram.F10.F1001 = 0;
			FuncCode_Handle.E2prom.F10.F1001 = 0;

			srand(g_165AD + g_141AD);
			FuncCode_Handle.Ram.F00.F0087 = (uint32_t)rand()&0x0000FFFF;//20170104 mxj
			FuncCode_Handle.E2prom.F00.F0087 = FuncCode_Handle.Ram.F00.F0087;
			FuncCode_Handle.Att.F00.F0087 |=0x0100;
			g_FcodeChangeFlag ++;

			HAL_Delay(500);
			g_SpecialSave=0;
		    break;
        }
	    case ParameterInit6:
	 	  {
	 		FuncCode_Handle.Ram.F10.F1014 =1;
	 		FuncCode_Handle.E2prom.F10.F1014 =1;

	 		FuncCode_Handle.Ram.F50.F5010 =20;
	 		FuncCode_Handle.E2prom.F50.F5010 =20;
	 		FuncCode_Handle.Ram.F50.F5011 =20;
	 		FuncCode_Handle.E2prom.F50.F5011 =20;
	 		FuncCode_Handle.Ram.F50.F5012 =20;
	 	    FuncCode_Handle.E2prom.F50.F5012 =20;
			FuncCode_Handle.Ram.F50.F5013 =20;
		 	FuncCode_Handle.E2prom.F50.F5013 =20;
	 		FuncCode_Handle.Ram.F50.F5014 =21;
	 		FuncCode_Handle.E2prom.F50.F5014 =21;
	 		FuncCode_Handle.Ram.F50.F5015 =22;
	 		FuncCode_Handle.E2prom.F50.F5015 =22;

	 		FuncCode_Handle.Ram.F50.F5032 =20;
	 		FuncCode_Handle.E2prom.F50.F5032 =20;

	 		FuncCode_Handle.Ram.F50.F5037 =21;
	 		FuncCode_Handle.E2prom.F50.F5037 =21;

	 		FuncCode_Handle.Ram.F10.F1001 = 0;
	 		FuncCode_Handle.E2prom.F10.F1001 = 0;
	 		g_SpecialSave=0;
	 		break;
	 	  }
	 	case ParameterInit7:
		  {
			  FuncCode_Handle.Ram.F50.F5010 =0;
			  FuncCode_Handle.E2prom.F50.F5010 =0;
			  FuncCode_Handle.Ram.F50.F5011 =0;
			  FuncCode_Handle.E2prom.F50.F5011 =0;
			  FuncCode_Handle.Ram.F50.F5012 =0;
			  FuncCode_Handle.E2prom.F50.F5012 =0;
			  FuncCode_Handle.Ram.F50.F5013 =0;
			  FuncCode_Handle.E2prom.F50.F5013 =0;
			  FuncCode_Handle.Ram.F50.F5014 =0;
			  FuncCode_Handle.E2prom.F50.F5014 =0;
			  FuncCode_Handle.Ram.F50.F5015 =0;
			  FuncCode_Handle.E2prom.F50.F5015 =0;

			  FuncCode_Handle.Ram.F10.F1001 = 0;
			  FuncCode_Handle.E2prom.F10.F1001 = 0;
			  g_SpecialSave=0;
			  break;
		  }
	 	case ParameterInit8:
	 	  {
	 		 FuncCode_Handle.Ram.F50.F5010 =20;
	 		 FuncCode_Handle.E2prom.F50.F5010 =20;
	 		 FuncCode_Handle.Ram.F50.F5011 =20;
	 		 FuncCode_Handle.E2prom.F50.F5011 =20;
	 	     FuncCode_Handle.Ram.F50.F5012 =20;
	 		 FuncCode_Handle.E2prom.F50.F5012 =20;
	 		 FuncCode_Handle.Ram.F50.F5013 =20;
	 		 FuncCode_Handle.E2prom.F50.F5013 =20;
	 		 FuncCode_Handle.Ram.F50.F5014 =20;
	 		 FuncCode_Handle.E2prom.F50.F5014 =20;
	 		 FuncCode_Handle.Ram.F50.F5015 =20;
	 		 FuncCode_Handle.E2prom.F50.F5015 =20;

	 		 FuncCode_Handle.Ram.F10.F1001 = 0;
	 		 FuncCode_Handle.E2prom.F10.F1001 = 0;
	 		  g_SpecialSave=0;
	 		  break;
	 	  }
//ֻ�Ĳ���
	    case ParameterInit10:
	    {
	 		 //drive_level = FuncCode_Handle.Ram.F00.F0004;//��������
	 		 Charger_type =  FuncCode_Handle.Ram.F00.F0002;//��������

	 		CGParameterRefresh(Charger_type);//20170105 mxj

	 		FuncCode_Handle.Ram.F10.F1001 = 0;
	 		FuncCode_Handle.E2prom.F10.F1001 = 0;
	 		g_SpecialSave=0;
	 		break;
	 	}
	    case InflexibilityChange:
	    {
	    	g_SpecialSave=0;
	    	break;
	    }
	    case InertiaChange:
	    {
	    	g_SpecialSave=0;
	    	break;
	    }
	    default:
	    {
	    	g_SpecialSave=0;
	    	break;
	    }
	}
}



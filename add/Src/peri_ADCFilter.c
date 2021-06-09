/*
 * peri_ADCFilter.c
 *
 * Created on: 2014-6-20
 * Author: JH-LvSL
 */
#include <main.h>			//Declarations from DAVE3 Code Generation (includes SFR declaration)
#include <math.h>
#include <peri_AdcHandle.h>
#include <peri_GlobalVariablesExtern.h>
#include <peri_GPIO.h>

#include "core_Com.h"
const uint16_t TempTable[109]=
{
	1974,1970,1967,1963,1960,1956,1952,1948,1944,1940,//20�ȿ�ʼ
	1935,1930,1926,1921,1916,1910,1905,1899,1894,1888,//30
	1882,1875,1869,1862,1856,1849,1841,1834,1827,1819,//40
	1811,1803,1794,1786,1777,1768,1759,1750,1740,1730,//50
	1720,1710,1700,1689,1678,1667,1656,1645,1633,1622,//60
	1610,1598,1585,1573,1560,1548,1535,1521,1508,1495,//70
	1481,1468,1454,1440,1426,1411,1397,1383,1368,1353,//80
	1339,1324,1309,1294,1279,1264,1249,1234,1219,1204,//90
	1188,1173,1158,1143,1128,1112,1097,1082,1067,1052,//100
	1037,1022,1007, 992, 978, 963, 949, 934, 920, 905,//110
	891, 877, 863,  849, 836, 822, 809, 795, 782//120
};

const uint16_t TempTable_E[109]=//20170717 mxj
{
	1476,1460,1443,1427,1410,1393,1376,1359,1342,1325,//20�ȿ�ʼ
	1307,1290,1272,1255,1237,1220,1202,1185,1167,1150,//30
	1132,1115,1098,1080,1063,1046,1029,1012,995,978,//40
	962,945,929,913,897,881,865,850,835,820,//50
	805,790,775,761,747,733,719,705,692,679,//60
	666,653,640,628,616,604,592,580,569,558,//70
	547,536,526,515,505,495,485,476,466,457,//80
	448,439,430,422,414,405,397,389,382,374,//90
	367,360,353,346,339,332,326,319,313,307,//100
	301,295,289,284,278,273,268,263,258,253,//110
	248,243,239,234,230,225,221,217,213//120
};

//int32_t g_IrmsCalc = 0;

ADCCorrection Iu;
ADCCorrection Iv;
ADCCorrection Vbus;
ADCCorrection Temperature;
ADCCorrection PTC;
ADCCorrection AI1;
ADCCorrection AI2;
ADCCorrection AI3;
LPF Iu100usFil = LPF_DEFAULTS;
LPF Iu1msFil = LPF_DEFAULTS;
LPF Iu10msFil = LPF_DEFAULTS;
LPF Iv100usFil = LPF_DEFAULTS;
LPF Iv1msFil = LPF_DEFAULTS;
LPF Iv10msFil = LPF_DEFAULTS;
LPF PVbus1msFil = LPF_DEFAULTS;
LPF PVbus10msFil = LPF_DEFAULTS;
LPF Temp10msFil = LPF_DEFAULTS;
LPF PTC10msFil = LPF_DEFAULTS;
LPF AI1Fil = LPF_DEFAULTS;
LPF AI2Fil = LPF_DEFAULTS;
LPF AI3Fil = LPF_DEFAULTS;
LPF Irms10msFil = LPF_DEFAULTS;

int32_t ADCCorrectionHandle(ADCCorrection *,uint16_t);
uint8_t AdcIuIvOffsetCalc(void);
extern void ADC_Trigger_Change(void);
/*************************************************
  Function: Init ADC Gain/Offset
  Description: Period:Init
  Input: No
  Return: No
  Others: Be Called InitModule();
*************************************************/
void InitAdcFilter(void)
{
	uint32_t drive_level=0,num=0;

	const ChargerParameter* p=&Charger_lever[0];

	//FuncCode_Handle.Ram.F00.F0004 = 101;//test

	//drive_level =
	//p = &Drive_lever1[num];
	//Iu.Gain = p->CurrPara;
	//Iv.Gain = Iu.Gain;

	//Iu.Gain = Drive_lever1[num][3];
	Iu.Offset = 2048;

	//Iv.Gain = Drive_lever1[num][3];
	Iv.Offset = 2048;

	//Vbus.Gain = 6777;//After Calculate,the value is P.u.:220*sqrt(2) = IQ24(1.0)
	//Vbus.Gain = 6856;//by mxj,6777*1.01=6844,6777*1.0116=6855.6
	if(drive_level<2300)//220V
	{
		//Vbus.Gain = 6865;//by mxj,6777*1.01=6844,6781*1.0122=6865
		Vbus.Gain = 1716;//by mxj,6777*1.01=6844,6781*1.0122=6865 /4=1716.25  20181028
		g_VpnVoltGain = 53946;	//53924 = 2^24/(220*sqrt(2))

		g_RelayrunQ24  = 8091905;//150V/311 =IQ(0.4823)=8091904.82
		g_RelaystopQ24 = 7012984;//130V/311 =IQ(0.41800643)=7012984.18
	}
	else//380V
	{
		//Vbus.Gain = 7463;//by mxj,7373*1.0122=7463
		Vbus.Gain = 1866;//by mxj,7373*1.0122=7463  /4=1865.75 20181028
		g_VpnVoltGain = 31342;	//31342 = 2^24/(380*sqrt(2))

		g_RelayrunQ24  = 9997596;//320V/537 =IQ(0.5959)=9997596.12
		g_RelaystopQ24 = 9372746;//300V/537 =IQ(0.55865921)=9372746.36
	}

	Vbus.Offset = 0;

	Temperature.Gain = 1;
	Temperature.Offset = 0;

	PTC.Gain = 0;
	PTC.Offset = 0;

	//AI1.Gain = 11354;
	AI1.Gain = 2839;//  11354/4=2838.5 20181028
	//AI1.Offset = 2048;
	//AI1.Offset = FuncCode_Handle.Ram.F00.F0049;
	AI1.Offset = FuncCode_Handle.Ram.F00.F0049<<2;

	AI2.Gain = 11354;
	//AI2.Offset = 2048;
	AI2.Offset = FuncCode_Handle.Ram.F00.F0050;

	AI3.Gain = 11354;
	//AI3.Offset = 2048;
	AI3.Offset = FuncCode_Handle.Ram.F00.F0051;

	Iu100usFil.Tm_Ts = CONST_100US_TmTs;
	Iu1msFil.Tm_Ts = CONST_1msTm_1msTs;
	Iu10msFil.Tm_Ts = CONST_10msTm_1msTs;

	Iv100usFil.Tm_Ts = CONST_100US_TmTs;
	Iv1msFil.Tm_Ts = CONST_1msTm_1msTs;
	Iv10msFil.Tm_Ts = CONST_10msTm_1msTs;

	Irms10msFil.Tm_Ts = CONST_10msTm_1msTs;

	PVbus1msFil.Tm_Ts = CONST_1msTm_1msTs;
	PVbus10msFil.Tm_Ts = CONST_10msTm_1msTs;

	Temp10msFil.Tm_Ts = CONST_10msTm_1msTs;
	Temp10msFil.Y 	= 2000;//mxj 4096->2000
	g_Temp100usQ24 	= 2000;//mxj20171204
	PTC10msFil.Tm_Ts =  CONST_10msTm_1msTs;
}
/*************************************************
  Function: ADC Sample Handle
  Description: Period:100us
  Input: No
  Return: No
  Others: Be Called AdcFilter100usHandle();
*************************************************/
uint16_t AdcSampleHandle(uint16_t AdcValue)
{
	int32_t Temp = 0;
	static uint16_t AdcValueOld = 0;
	uint32_t FcodeValue  = 0;

	FcodeValue = FuncCode_Handle.Ram.F00.F0042;//sample point error

	if(FcodeValue)
	{
		Temp = abs(g_VelocityQ24Fil) * 170;//170 <=> Rate Velocity Only For 8 poles

		if(Temp < 5)
			Temp = 5;

		if(AdcValue > (AdcValueOld + (uint16_t)Temp))
		{
			AdcValue = AdcValueOld + (uint16_t)Temp;
		}
		else if(AdcValue < (AdcValueOld - (uint16_t)Temp))
		{
			AdcValue = AdcValueOld - (uint16_t)Temp;
		}

		AdcValueOld = AdcValue;
	}

	return AdcValue;
}

/*************************************************
  Function: ADC Cuurent Calebrate
  Description: Period:100us
  Input: No
  Return: No
  Others: Be Called AdcFilter100usHandle();
*************************************************/
int32_t AdcICalebrate(int32_t IValue)
{
	int32_t FcodeValue;
	int64_t Temp;

	FcodeValue = (int32_t)FuncCode_Handle.Ram.F00.F0041;//��������У��ֵ

	Temp = (int64_t)FcodeValue * 16777;//16777 = 2^24/1000

	Temp = Temp * IValue;

	return (int32_t)(Temp >> 24);
}
/*************************************************
  Function: ADC Result LPF Filter Handle
  Description: Period:100us
  Input: No
  Return: No
  Others: Be Called SysCore100usLoop();
*************************************************/
void AdcFilter100usHandle(void)
{
	static  uint8_t AD_Count = 0;
	static  int32_t AI1_ADResult[8];
	static  int32_t AI2_ADResult[8];

	//uint8_t Drive_State = 0;
	//static  uint8_t Drive_State_Old = 0;
	static  uint16_t Drive_State_Cnt = 0;

	if(AD_Count <= 7)
	{
		AI1_ADResult[AD_Count] = AdcResult[4];
		AI2_ADResult[AD_Count] = AdcResult[5];
		AD_Count ++;
	}
	else
	{
		g_AI1AD = (AI1_ADResult[0] + AI1_ADResult[1] + AI1_ADResult[2]+ AI1_ADResult[3]+ AI1_ADResult[4]+ AI1_ADResult[5]+ AI1_ADResult[6]+ AI1_ADResult[7])>>3;
		//g_AI1AD = (AI1_ADResult[0] + AI1_ADResult[1] + AI1_ADResult[2]+ AI1_ADResult[3]+ AI1_ADResult[4]+ AI1_ADResult[5]+ AI1_ADResult[6]+ AI1_ADResult[7])>>5;//20181028
		g_AI2AD = (AI2_ADResult[0] + AI2_ADResult[1] + AI2_ADResult[2]+ AI2_ADResult[3]+ AI2_ADResult[4]+ AI2_ADResult[5]+ AI2_ADResult[6]+ AI2_ADResult[7])>>3;
		AD_Count = 0;
	}

	if(AdcIuIvOffsetCalc())
	{
		return;
	}//��ʱ����

	/********ADC Dynamic Calibration********/
/*    temp = P5_2_read();
    temp >>2;
	if( temp !=0 )
	{
	  g_IuAD = AdcCalibrateValue(AdcResult[0]);//û�κδ���
	  g_IvAD = AdcCalibrateValue(AdcResult[1]);//û�κδ���
	}
	else
	{
	  g_IuAD = 2048;
	  g_IvAD = 2048;
	}*/

	 g_IuAD = AdcResult[0];//
	 g_IvAD = AdcResult[1];//

	/********ADC Gain/Offset Handle********/
	//IO004_SetOutputValue(IO004_Handle27,1);
	//MX_GPIO_ReadPin(IO004_Handle27)
	// if(MX_GPIO_ReadPin(IO004_Handle27)&&(	AlmRecord_Time>10))
	 /*
	 if(MX_GPIO_ReadPin(IO004_Handle27))
	 	{
	 		//g_Iu100usQ24 = AdcICalebrate(ADCCorrectionHandle(&Iu,AdcSampleHandle(g_IuAD)));
	 		//g_Iv100usQ24 = AdcICalebrate(ADCCorrectionHandle(&Iv,AdcSampleHandle(g_IvAD)));
	 		//g_Iu100usQ24 = ADCCorrectionHandle(&Iu,AdcSampleHandle(g_IuAD));
	 		//g_Iv100usQ24 = ADCCorrectionHandle(&Iv,AdcSampleHandle(g_IvAD));
	 		g_Iu100usQ24 = ADCCorrectionHandle(&Iu,g_IuAD);
	 		g_Iv100usQ24 = ADCCorrectionHandle(&Iv,g_IvAD);

	 		//g_Iu100usQ24 = g_IuAD<<12;//test
	 		//g_Iv100usQ24 = g_IvAD<<12;
	 	}
	 	else
	 	{
	 		g_Iu100usQ24 = 0;
	 		g_Iv100usQ24 = 0;
	 	}*/

	if(MX_GPIO_ReadPin(IO004_Handle27))
	{
		if(Drive_State_Cnt >= 48)//6ms
		{
			g_Iu100usQ24 = ADCCorrectionHandle(&Iu,g_IuAD);
			g_Iv100usQ24 = ADCCorrectionHandle(&Iv,g_IvAD);
		}
		else
			Drive_State_Cnt++;
	}
	else
	{
		g_Iu100usQ24 = 0;
		g_Iv100usQ24 = 0;
		Drive_State_Cnt = 0;
	}

	/********ADC LPF Calc*********/
/*//to 1ms
	Iu100usFil.X = g_Iu100usQ24;
	Iu100usFil.FilterCalc(&Iu100usFil);
	g_Iu100usFilQ24 = Iu100usFil.Y;

	Iv100usFil.X = g_Iv100usQ24;
	Iv100usFil.FilterCalc(&Iv100usFil);
	g_Iv100usFilQ24 = Iv100usFil.Y;//after LPF

	g_Iw100usFilQ24	= -g_Iu100usFilQ24-g_Iv100usFilQ24;
//	Current1msQ24.Iu = g_Iu100usFilQ24;//
//	Current1msQ24.Iv = g_Iv100usFilQ24;
//	Current1msQ24.Iw = g_Iw100usFilQ24;
*///to 1ms
	if(DriverPara.RatedPow < 2300)
	//if(DriverPara.RatedVol == 220)
	{
		Current1msQ24.Iu = g_Iu100usQ24;//
		Current1msQ24.Iv = g_Iv100usQ24;
//		Current1msQ24.Iw = -g_Iu100usQ24 - g_Iv100usQ24;//to 1ms
	}
	else
	{
		//Current1msQ24.Iw = g_Iu100usQ24;//
		Current1msQ24.Iv = g_Iv100usQ24;
		Current1msQ24.Iu = -g_Iu100usQ24 - g_Iv100usQ24;//to 1ms
	}

//	g_165AD = ((int32_t)AdcResult[7])<<12;//1.65V
//	g_141AD = ((int32_t)AdcResult[8])<<12;//1.414V

	g_PowerVbusAD 	= AdcResult[10];
	/********ADC Gain/Offset Handle********/
	g_PowerVbus100usQ24 = ADCCorrectionHandle(&Vbus,g_PowerVbusAD)+1;//20170628 mxj
	//if(g_PowerVbus100usQ24 >= 8388608)
	g_PowerVbus100usQ24_PU = 0x0001000000000000/g_PowerVbus100usQ24;//20170628
}
/*************************************************
  Function: ADC Result LPF Filter Handle
  Description: Period:1ms
  Input: No
  Return: No
  Others: Be Called SysCore1msLoop();
*************************************************/
void AdcFilter1msHandle(void)
{
	uint8_t i = 0;
  //  int64_t Temp;
	if(CoreStatusFlag.bit.EEPROMINIT_OK_F == NG)
	{
		return;
	}

	/********ADC Dynamic Calibration********/
	//g_CtrlVbusAD = AdcCalibrateValue(AdcResult[1]);
	/*g_PowerVbusAD = AdcCalibrateValue(AdcResult[10]);
	g_TempAD = AdcCalibrateValue(AdcResult[3]);
	g_PTCAD =  AdcCalibrateValue(AdcResult[9]);

	g_AI1AD = AdcCalibrateValue(AdcResult[4]);
	g_AI2AD = AdcCalibrateValue(AdcResult[5]);
	g_AI3AD = AdcCalibrateValue(AdcResult[6]);*/

	//g_PowerVbusAD 	= AdcResult[10];//20170628 mxj
	g_TempAD		= AdcResult[3];
	g_PTCAD = AdcResult[9];

//	g_AI1AD = AdcResult[4];
//	g_AI2AD = AdcResult[5];
	g_AI3AD = AdcResult[6];

	g_165AD = ((int32_t)AdcResult[7])<<12;//1.65V
	g_141AD = ((int32_t)AdcResult[8])<<12;//1.414V

	/********ADC Gain/Offset Handle********/
	//g_PowerVbus100usQ24 = ADCCorrectionHandle(&Vbus,g_PowerVbusAD)+1;//20170628 mxj

	//if(g_PowerVbus100usQ24 >= 8388608)
	//	g_PowerVbus100usQ24_PU = 0x0001000000000000/g_PowerVbus100usQ24;//20170628

	g_Temp100usQ24 = ADCCorrectionHandle(&Temperature,g_TempAD);
	g_PTC100usQ24 = ADCCorrectionHandle(&PTC,g_PTCAD);

	g_AI1Q24 = ADCCorrectionHandle(&AI1,g_AI1AD);//(g_AI1Q24*10)/2E24=ģ�������ѹ
	g_AI2Q24 = ADCCorrectionHandle(&AI2,g_AI2AD);
	g_AI3Q24 = ADCCorrectionHandle(&AI3,g_AI3AD);

	/********ADC LPF Calc*********/
	Iu1msFil.X = g_Iu100usQ24;
	Iu1msFil.FilterCalc(&Iu1msFil);
	g_Iu1msFilQ24 = Iu1msFil.Y;

	Iv1msFil.X = g_Iv100usQ24;
	Iv1msFil.FilterCalc(&Iv1msFil);
	g_Iv1msFilQ24 = Iv1msFil.Y;

	Iu10msFil.X = g_Iu100usQ24;
	Iu10msFil.FilterCalc(&Iu10msFil);
	g_Iu10msFilQ24 = Iu10msFil.Y;

	Iv10msFil.X = g_Iv100usQ24;
	Iv10msFil.FilterCalc(&Iv10msFil);
	g_Iv10msFilQ24 = Iv10msFil.Y;

	Irms10msFil.X = g_IrmsCalc;
	Irms10msFil.FilterCalc(&Irms10msFil);
	g_Irms10msFilQ24 = (uint32_t)Irms10msFil.Y;

/*    Temp = abs(g_Irms10msFilQ24);
    Temp = Temp * DriverPara.RatedCur;
    Temp = Temp >> 24;
    if(Temp > AlmRecord.MaxIrms)
    {
    	AlmRecord.MaxIrms = Temp;//��¼���ֵ
    }*/

	PVbus1msFil.X = g_PowerVbus100usQ24;
	PVbus1msFil.FilterCalc(&PVbus1msFil);
	g_PowerVbus1msFilQ24 = PVbus1msFil.Y;

	PVbus10msFil.X = g_PowerVbus100usQ24;
	PVbus10msFil.FilterCalc(&PVbus10msFil);
	g_PowerVbus10msFilQ24 = PVbus10msFil.Y;

	// be moved to mainloop mxj

	PTC10msFil.X = g_PTC100usQ24;
	PTC10msFil.FilterCalc(&PTC10msFil);
	g_PTC10msFilQ24 =  PTC10msFil.Y;

	AI1Fil.X = g_AI1Q24;
	AI1Fil.FilterCalc(&AI1Fil);
	g_AI1FilQ24 = AI1Fil.Y;
//	ExAI.AI1Q24 =AI1Fil.Y;

	AI2Fil.X = g_AI2Q24;
	AI2Fil.FilterCalc(&AI2Fil);
	g_AI2FilQ24 = AI2Fil.Y;
//	ExAI.AI2Q24 =AI2Fil.Y;

	AI3Fil.X = g_AI3Q24;
	AI3Fil.FilterCalc(&AI3Fil);
	g_AI3FilQ24 = AI3Fil.Y;
//	ExAI.AI3Q24 =AI3Fil.Y;

//100us to 1ms======================================================
	Iu100usFil.X = g_Iu100usQ24;
	Iu100usFil.FilterCalc(&Iu100usFil);
	g_Iu100usFilQ24 = Iu100usFil.Y;

	Iv100usFil.X = g_Iv100usQ24;
	Iv100usFil.FilterCalc(&Iv100usFil);
	g_Iv100usFilQ24 = Iv100usFil.Y;//after LPF
	g_Iw100usFilQ24	= -g_Iu100usFilQ24-g_Iv100usFilQ24;

	//Current1msQ24.Iw = -g_Iu100usQ24 - g_Iv100usQ24;
	Current1msQ24.Iw = -Current1msQ24.Iu - Current1msQ24.Iv;
//100us to 1ms==============================================================

}
/*************************************************
  Function: ADC Result Correction Handle
  Description: Period:100us/1ms
  Input: No
  Return: No
  Others: Be Called AdcFilter100usHandle()/AdcFilter1msHandle();
*************************************************/
int32_t ADCCorrectionHandle(ADCCorrection *p,uint16_t ADResult)// calibrate rated current 2^24
{
	int32_t Result;

	Result = p->Gain * ((int32_t)ADResult - p->Offset);

	return Result;
}
/*************************************************
  Function: AI Related Calc
  Description: Period:System Loop
  Input: FuncCode  F5016/F5021/F5026
  Return: No
  Others: Be Called LPFFilterTimeCalc();
*************************************************/
void AIRelatedCalc(void)
{
	int64_t temp = 0;
	int32_t temp1 = 0;

	/*AI1 Filter Time Calc*/
	temp = (int64_t)CONST_TS_1MS_Q48;
	temp1 = (int32_t)FuncCode_Handle.Ram.F50.F5016 * 1678 + (int32_t)CONST_TS_1MS_Q24;
	AI1Fil.Tm_Ts = (int32_t)(temp/temp1);

	/*AI2 Filter Time Calc*/
	temp = (int64_t)CONST_TS_1MS_Q48;
	temp1 = (int32_t)FuncCode_Handle.Ram.F50.F5021 * 1678 + (int32_t)CONST_TS_1MS_Q24;
	AI2Fil.Tm_Ts = (int32_t)(temp/temp1);

	/*AI3Filter Time Calc*/
	temp = (int64_t)CONST_TS_1MS_Q48;
	temp1 = (int32_t)FuncCode_Handle.Ram.F50.F5026 * 1678 + (int32_t)CONST_TS_1MS_Q24;
	AI3Fil.Tm_Ts = (int32_t)(temp/temp1);
}
/*************************************************
  Function: Adc Sample Related Calc
  Description: Period:100us
  Input: FuncCode
  Return: No
  Others: Be Called AdcFilter100usHandle();
*************************************************/
uint8_t AdcIuIvOffsetCalc(void)
{
	static uint32_t SumU = 0;
	static uint32_t SumV = 0;
	static uint16_t i = 0;
	static uint8_t CalcResult = 1;

	static uint32_t AD_ErrCnt = 0;
	int16_t  AdcResultU;
	int16_t  AdcResultV;

	static int16_t AdcResultU_Max=0;
	//static int16_t AdcResultU_Min=4000;//
	static int16_t AdcResultU_Min=17000;//20181028
	static int16_t AdcResultV_Max=0;
	//static int16_t AdcResultV_Min=4000;
	static int16_t AdcResultV_Min=17000;

	if(CalcResult)
	{
		if(i < 4096)
		{
			//SumU += AdcResult[0];
			//SumV += AdcResult[1];

			AdcResultU = AdcResult[0];
			AdcResultV = AdcResult[1];
			SumU += AdcResultU;
			SumV += AdcResultV;
			i++;

			//AdcResultU = abs(AdcResultU - 2048);
			//AdcResultV = abs(AdcResultV - 2048);
			//if((AdcResultU >= 200)||(AdcResultU >= 200))
			//AdcResultU = (AdcResultU - 2048);
		  //  AdcResultV = (AdcResultV - 2048);
			//if((AdcResultU >= 100)||(AdcResultV >= 100)||(AdcResultU <= -100)||(AdcResultV <= -100))

			if(i&0x007F)
			{
				if(AdcResultU > AdcResultU_Max)
					AdcResultU_Max = AdcResultU;
				else if (AdcResultU < AdcResultU_Min)
					AdcResultU_Min = AdcResultU;

				if(AdcResultV > AdcResultV_Max)
					AdcResultV_Max = AdcResultV;
				else if (AdcResultV < AdcResultV_Min)
					AdcResultV_Min = AdcResultV;
			}
			else
			{
				//if((AdcResultU_Max - AdcResultU_Min > 35)||(AdcResultV_Max - AdcResultV_Min > 35))
				if((AdcResultU_Max - AdcResultU_Min > 140)||(AdcResultV_Max - AdcResultV_Min > 140))//20181028
				{
					AD_ErrCnt ++;
					//FuncCode_Handle.Ram.FA0.FA088 = AdcResultU_Max - AdcResultU_Min;
					//FuncCode_Handle.Ram.FA0.FA089 = AdcResultV_Max - AdcResultV_Min;
				}
				FuncCode_Handle.Ram.FA0.FA088 += (AdcResultU_Max - AdcResultU_Min);
				FuncCode_Handle.Ram.FA0.FA089 += (AdcResultV_Max - AdcResultV_Min);
				//FuncCode_Handle.Ram.FA0.FA090 = AD_ErrCnt;
				FuncCode_Handle.Ram.FA0.FA090 ++;

				AdcResultU_Max=0;
				//AdcResultU_Min=4000;
				AdcResultU_Min=17000;//20181028
				AdcResultV_Max=0;
				//AdcResultV_Min=4000;
				AdcResultV_Min=17000;
			}

			if(AD_ErrCnt >= 10)
			{
				//AlarmStatusFlag.bit.Driver.FO = OK;
				IO004_SetOutputValue(IO004_Handle27,0);
				FuncCode_Handle.Ram.FA0.FA059 = 1000;//20180807
				i=4096;
			}
		}
		else
		{
			Iu.Offset = (int32_t)(SumU >> 12);
			Iv.Offset = (int32_t)(SumV >> 12);
//			Iu.Offset = 4096 - Iu.Offset;
//			Iv.Offset = 4096 - Iv.Offset;
			FuncCode_Handle.Ram.F00.F0043 = Iu.Offset;
			FuncCode_Handle.Ram.F00.F0045 = Iv.Offset;
			FuncCode_Handle.E2prom.F00.F0043 = Iu.Offset;
			FuncCode_Handle.E2prom.F00.F0045 = Iv.Offset;

			/*
			if(AD_ErrCnt >= 10)
				AlarmStatusFlag.bit.Driver.FO = OK;
			//else if((FuncCode_Handle.Ram.F00.F0043 <1900)||(FuncCode_Handle.Ram.F00.F0043 >2200))//1700->1900
			else if((FuncCode_Handle.Ram.F00.F0043 <7600)||(FuncCode_Handle.Ram.F00.F0043 >8800))//20181028
				AlarmStatusFlag.bit.Motor.CUr = OK;
			//else if((FuncCode_Handle.Ram.F00.F0045 <1900)||(FuncCode_Handle.Ram.F00.F0045 >2200))//2400->2200
			else if((FuncCode_Handle.Ram.F00.F0045 <7600)||(FuncCode_Handle.Ram.F00.F0045 >8800))//20181028
				AlarmStatusFlag.bit.Motor.CUr = OK;
			else
				SysTimeSequ.Sec10_Flg = OK;
			*/

			CalcResult = 0;

			SysTimeSequ.Sec10_Flg = OK;//test CAN
		}
	}
	return CalcResult;
}

void TempCalc(void)
{
	uint8_t i = 0;
	//====================================================================
	//may be moved to mainloop mxj
	Temp10msFil.X = g_Temp100usQ24;
	Temp10msFil.FilterCalc(&Temp10msFil);

	if(FuncCode_Handle.Ram.F00.F0004 < 2300)//220V
	{
		if(Temp10msFil.Y >= 2036)//�¶Ȳ�������1.64V
		{
			g_Temp10msFilQ24 = 0;
		}
		else if(Temp10msFil.Y > TempTable[0])
		{
			g_Temp10msFilQ24 = 20;
		}
		else
		{
			for(i = 0;(TempTable[i] > Temp10msFil.Y)&&(i<109);i++)
			{
			}
			g_Temp10msFilQ24 = i + 20;
		}
	}
	else//E  380V
	{
		if(Temp10msFil.Y >= 2036)//�¶Ȳ�������1.64V
		{
			g_Temp10msFilQ24 = 0;
		}
		else if(Temp10msFil.Y > TempTable_E[0])
		{
			g_Temp10msFilQ24 = 20;
		}
		else
		{
			for(i = 0;(TempTable_E[i] > Temp10msFil.Y)&&(i<109);i++)
			{
			}
			g_Temp10msFilQ24 = i + 20;
		}
	}

//	g_Temp10msFilQ24 = Temp10msFil.Y;
	g_TempQ24= g_Temp10msFilQ24 * 167772;//100�ȶ�ӦIQ24��1.0��mxj
	//=================================================================
}













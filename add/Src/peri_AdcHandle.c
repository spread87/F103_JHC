/**********************************************************
Copyright (c) 2014 JIANGSU JOINHOPE ELECTRIC CO.,LTD
FileName: main.c
Author: LvSL
Version :
Date: 2014.02.26
Description: Handle relactive ADC
History:
    1. Date: 2014.02.26
       Author: LvSL
       Modification:  1st Version
    2...
***********************************************************/

#include <main.h>			//Declarations from DAVE3 Code Generation (includes SFR declaration)
#include <peri_GlobalVariablesExtern.h>
//#include <XMC4400.h>

#define    ADC_STANDARD_POWER1       2733          //ADCRESULT(G1CH4) 2728 = 1.9987*4096/3 theory value
#define    ADC_STANDARD_POWER1_Q16   (ADC_STANDARD_POWER1*65536)    //ADCRESULT(G1CH4) ideal Q16
#define    ADC_STANDARD_POWER2       1366          //ADCRESULT(G1CH5) 1364 = 0.9993*4096/3 theory value
#define    ADC_STANDARD_POWER2_Q16   (ADC_STANDARD_POWER2*65536)    //ADCRESULT(G1CH5) ideal Q16

int32_t  Adc_CalGain;
int32_t  Adc_CalOffset;
static uint32_t adcINA3Add;
static uint32_t adcINA5Add;

uint32_t ComTcFilter(uint16_t in, uint16_t fback, uint32_t add);
/*************************************************
  Function: Adc calibration,calculate AdcGain & AdcOffset
  Description:
  Input: No
  Return: No
  Others: period:2ms
*************************************************/
void AdcCalibration(void)
{
	static uint16_t Adcina3;
	static uint16_t Adcina5;
	static uint32_t Adcina3_value;
	static uint32_t Adcina5_value;
	static int16_t Init=1;
	static int32_t value;
	static uint16_t Adcina3_1;
	static uint16_t Adcina5_1;


	Adcina3 = AdcResult[7];    //G1CH4 actual value
	Adcina5 = AdcResult[8];    //G1CH5 actual value
	/*init:ideal value;LPF_TIME:2.048s*/
	if(Init)
	{
		adcINA3Add = ADC_STANDARD_POWER1_Q16; 	//2V
		adcINA5Add = ADC_STANDARD_POWER2_Q16;	//1V

		Adcina3_value = ComTcFilter(Adcina3,ADC_STANDARD_POWER1,adcINA3Add);
		Adcina5_value = ComTcFilter(Adcina5,ADC_STANDARD_POWER2,adcINA5Add);
		Init=0;
	}
	else
	{
		Adcina3_value = ComTcFilter(Adcina3,(uint16_t)(Adcina3_value>>16),Adcina3_value);
		Adcina5_value = ComTcFilter(Adcina5,(uint16_t)(Adcina5_value>>16),Adcina5_value);
	}

	Adcina3_1 = (uint16_t)(Adcina3_value>>16);
	Adcina5_1 = (uint16_t)(Adcina5_value>>16);
	if((Adcina3_value-Adcina5_value)==0)
	{
		return;
	}

	value = (int32_t)Adcina5_1*Adc_CalGain;
	value = value>>16;

	Adc_CalGain = (int32_t)((ADC_STANDARD_POWER1-ADC_STANDARD_POWER2)*65536)/(int32_t)(Adcina3_1-Adcina5_1);
	Adc_CalOffset = (int32_t)ADC_STANDARD_POWER2-value;

}

/*************************************************
  Function: Adc calibration handle
  Description:
  Input:  AdcResult[7]/AdcResult[9];
  Return: No
  Others: No
*************************************************/
uint16_t AdcCalibrateValue(uint16_t adc_value)
{
#if 0
	int32_t adc_out;
	static int32_t value;

	value = (int32_t)adc_value*Adc_CalGain;
	value = value>>16;

	adc_out = value + Adc_CalOffset;

	return (uint16_t)adc_out;
#endif
	int16_t ADTemp = 0;
	uint16_t adc_out = 0;

	ADTemp = 0;//(int16_t)2048 - (int16_t)AdcResult[7] ;

	adc_out = adc_value - ADTemp;

	return adc_out;
}

/*************************************************
  Function:unsigned 16bit LPF 2ms
  Description:LPF
  Input: in: sample actual value  n :filter time   n=1:10ms  fback: last out value
  Return: *add  (last value)<<16
  Others: No
*************************************************/
uint32_t ComTcFilter(uint16_t in, uint16_t fback, uint32_t add)
{
    int32_t  delta_in;

    delta_in = ((int32_t)in - (int32_t)fback) << 6;
    delta_in = (int32_t)add + delta_in;
    add      = (uint32_t)delta_in;

    return  add;
}
#if 0
/*************************************************
  Function: Read one Adc Value
  Description:
  Input: No
  Return: No
  Others: Be called: AdcReadResult()
*************************************************/
void AdcReadOneChannel(ADC001_ResultHandleType* ResultPtr)
{
	switch(ResultPtr->GroupNo)
	{
		case 0:
			switch(ResultPtr->ChannelNo)
			{
				case 0: AdcResult[0]=ResultPtr->Result;
				break;
				case 1: AdcResult[1]=ResultPtr->Result;
				break;
				case 2: AdcResult[2]=ResultPtr->Result;
				break;
				case 3: //AdcResult[3]=ResultPtr->Result;
				break;
				case 4: AdcResult[3]=ResultPtr->Result;
				break;
				case 5: AdcResult[4]=ResultPtr->Result;
				break;
				case 6: AdcResult[5]=ResultPtr->Result;
				break;
				case 7: AdcResult[6]=ResultPtr->Result;
				break;
				default:
				break;
			}
			break;

		case 1:
			switch(ResultPtr->ChannelNo)
			{
				case 4:	 AdcResult[7]=ResultPtr->Result;
				break;
				case 5: AdcResult[8]=ResultPtr->Result;
				break;
				case 6: AdcResult[9]=ResultPtr->Result;
				break;
			}
			break;

		case 2:
			break;

		case 3:
			if(ResultPtr->ChannelNo == 0)
			AdcResult[10]=ResultPtr->Result;
			break;

		default:
			break;
	}
}
#endif





















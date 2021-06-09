//############################################################################
// FILE:	  peri_lpfhpf.c
// CONTENT:	  low pass filter/high pass filter
// AUTHOR:    LvSL
// DATE:      2014/03/28
//############################################################################
#include <main.h>			//Declarations from DAVE3 Code Generation (includes SFR declaration)
#include <peri_GlobalVariablesExtern.h>

/**
 * @brief: period:0.1ms loop/Sample Freq:10K
 * @param[in]:low pass filter calc
 * @return: none
 *
 */
void LPF_Calc(LPF *p)// (Ts/(T+Ts))(Xn-Yn-1)+Yn-1=Yn  Tm_Ts=(Ts/(T+Ts))
{
	int32_t Temp;
	int64_t Temp0;
	Temp = p->X - p->Y;
	Temp0 = (int64_t)Temp * p->Tm_Ts;//* means multiply
	p->Y +=(int32_t)(Temp0 >> 24);
}

void LPF64_Calc_Pulse(LPF64 *p)// (Ts/(T+Ts))(Xn-Yn-1)+Yn-1=Yn  Tm_Ts=(T/(T+Ts))
{
	int32_t Temp;
	int64_t Temp0;
	Temp = (int32_t)(p->Y - p->X);
	Temp0 = (int64_t)Temp * p->Tm_Ts;//* means multiply
	if(Temp0>=0)
		p->Y = p->X + (Temp0 >> 24);
	else
		p->Y = p->X - ((-Temp0) >> 24);

}

void LPF_Calc_Pulse(LPF *p)// (Ts/(T+Ts))(Xn-Yn-1)+Yn-1=Yn  Tm_Ts=(T/(T+Ts))  CYL170308
{
	int32_t Temp;
	int64_t Temp0;
	Temp = p->Y - p->X;
	Temp0 = (int64_t)Temp * p->Tm_Ts;//* means multiply
	if(Temp0>=0)
		p->Y = p->X + (int32_t)(Temp0 >> 24);
	else
		p->Y = p->X - (int32_t)((-Temp0) >> 24);

}

/**
 * @brief: period:0.1ms loop/Sample Freq:10K
 * @param[in]:high pass filter calc
 * @return: none
 *
 */
void HPF_Calc(HPF *p)
{
	int32_t Temp;
	int64_t Temp0;
	Temp = p->X - p->X_n + p->Y;
	Temp0 = (int64_t)Temp * p->Tm_Ts;
	p->Y =(int32_t)(Temp0 >> 24);
	p->X_n = p->X;
}



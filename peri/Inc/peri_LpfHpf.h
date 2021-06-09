//############################################################################
// FILE:	  peri_lpfhpf.h
// CONTENT:	  low pass filter/high pass filter
// AUTHOR:    LvSL
// DATE:      2013/10/19
//############################################################################

#ifndef PERI_LPFHPF_H_
#define PERI_LPFHPF_H_

//low pass filter
typedef struct
{
	int32_t 	X;					//input
	int32_t 	Y;					//output
	int32_t 	Tm_Ts;				//filter time
	void 	(*FilterCalc)();
}LPF;

typedef struct
{
	int64_t 	X;					//input
	int64_t 	Y;					//output
	int32_t 	Tm_Ts;				//filter time
	void 	(*FilterCalc)();
}LPF64;

typedef LPF *LPF_HANDLE;
typedef LPF64 *LPF64_HANDLE;

#define LPF_DEFAULTS {0,1974,0,(void(*)(uint32_t))LPF_Calc}
#define LPF_P_DEFAULTS {0,0,0,(void(*)(uint32_t))LPF_Calc_Pulse}
#define LPF64_P_DEFAULTS {0,0,0,(void(*)(uint32_t))LPF64_Calc_Pulse}
#define LPF1_DEFAULTS {0,0,6710886,(void(*)(uint32_t))LPF_Calc}
//high pass filter
typedef struct
{
	int32_t		X;
	int32_t		X_n;
	int32_t		Y;
	int32_t		Tm_Ts;
	void 	(*FilterCalc)();
}HPF;

typedef HPF *HPF_HANDLE;

#define HPF_DEFAULTS {0,0,0,0,(void(*)(uint32_t))HPF_Calc}

void LPF_Calc(LPF_HANDLE);
void LPF_Calc_Pulse(LPF_HANDLE);
void LPF64_Calc_Pulse(LPF64_HANDLE);
void HPF_Calc(HPF_HANDLE);

#endif /* PERI_LPFHPF_H_ */

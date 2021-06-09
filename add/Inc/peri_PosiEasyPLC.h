/*
 * peri_PosiEasyPLC.h
 *
 *  Created on: 2015-2-5
 *      Author: JH-LvSL
 */

#ifndef PERI_POSIEASYPLC_H_
#define PERI_POSIEASYPLC_H_

typedef void (*POSIPLC_FUNC)(void);

typedef struct
{
	int32_t PosiSegNum;			//input Value
	int32_t PulCount;			//input Value
	int32_t PosiAllCounts;		//input Value
	int32_t AllCounts;
	int32_t PosiIncCounts;		//input Value
	int32_t PosiLimit;
	int32_t MaxVelo;			//input Value
	int32_t	AccTime;			//input Value
	int32_t	WaitTime;			//input Value
	int32_t VeloMaxQ24;			//Inner Value
	int32_t VeloAccDeltaQ24;	//Inner Value
	int32_t RunFlg;				//Inner Value
	int32_t	WaitRunFlg;			//Inner Value
	int32_t VeloLimitQ24;		//Output Value
	int32_t PosiDelta;			//Output Value
	int32_t PosiAllCounts_Err; //Output Value  20170914 mxj
	int16_t PosiOK_Restart;		//Output Value  20180816
	int16_t Step;		//Output Value  20190809

	void 	(*Calc)();

}PosiPLC;

typedef PosiPLC *PosiPLC_HANDLE;

#define POSIPLC_DEFAULTS   { 0,1000,0,0,0,0,0,0,0,0,0,0,0,128,0,0,0,0,(void(*)(uint32_t))PosiPLC_Calc}

void PosiPLC_Calc(PosiPLC_HANDLE);


#endif /* PERI_POSIEASYPLC_H_ */





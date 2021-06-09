/*
 * peri_Pid.h
 *
 *  Created on: 2016-6-2
 *      Author: MXJ
 */

#ifndef PERI_PID_H_
#define PERI_PID_H_


typedef struct
{
	int32_t Ref_Channel;			//input Value
	int32_t Fdb_Channel;			//input Value
	int32_t Dig_Ref;					//input Value
	int32_t Fdb;					//input Value
	int32_t	MaxLimit_Channel;		//input Value
	int32_t	MinLimit_Channel;		//input Value
	int32_t Dig_MaxLimit;			//Inner Value
	int32_t Dig_MinLimit;			//Inner Value
	int32_t Calc_time;				//Inner Value
	int32_t	Calc_time_Cnt;			//Inner Value
	int32_t Com_Ref;			//Output Value
	int32_t PosiDelta;			//Output Value

}PeriPid;

typedef PeriPid *PeriPid_HANDLE;

#define PERIPID_DEFAULTS   { 0,1000,0,0,0,0,0,0,1,1,0,0}

//extern void PeriPidHandle(void);
//extern void PeriPidPara(void);

#endif /* PERI_PID_H_ */

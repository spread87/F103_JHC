/*
 * peri_VeloEasyPLC.h
 *
 * Created on: 2015-2-9
 * Author: JH-LvSL
 */

#ifndef PERI_VELOEASYPLC_H_
#define PERI_VELOEASYPLC_H_

typedef void (*VELOPLC_FUNC)(void);

typedef struct
{
	int32_t VelocityCMD;		//Input Value
	int32_t RunTime;			//Input Value
	int32_t AccTime;			//Input Value
	int32_t DecTime;			//Input Value
	int32_t VeloACCQ24;			//Output Value
	int32_t VeloDECQ24;			//Output Value
	int32_t VeloTargetQ24;		//Output Value
	int32_t RunFlg;				//Inner Value
	int32_t VeloNumSeg;			//Inner Value
	int32_t RunTimeFlg;			//Inner Value
	int32_t RunOverFlg;			//Inner Value

	void (*Calc)();
}VelocityPLC;

typedef VelocityPLC *VeloPLC_Handle;

#define	VELOCITYPLC_DEFAULTS	{0,0,0,0,0,0,0,0,0,0,0,(void(*)(uint32_t))VeloPLC_Calc}

void VeloPLC_Calc(VeloPLC_Handle);
#endif /* PERI_VELOEASYPLC_H_ */

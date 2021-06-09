/*
 * peri_TerminalAI.c
 *
 *  Created on: 2014-6-19
 *      Author: JH-LvSL
 */
#include <main.h>			//Declarations from DAVE3 Code Generation (includes SFR declaration)
#include <peri_GlobalVariablesExtern.h>
#include <peri_TerminalAI.h>

#define  			RECIPROCAL1000        16777 //IQ24(1/10000)

AICorrection AI1Q24;
AICorrection AI2Q24;
AICorrection AI3Q24;

static void TerminalAI1Calc(void);
static void TerminalAI2Calc(void);
static void TerminalAI3Calc(void);
void AIGainOffsetCalc(AICorrection *);
//static void TerminalAIProcess(void);

/*************************************************
  Function: Terminal AI Handle
  Description:Period:Main Loop
  Input: No
  Return: No
  Others: Be Called SysMainLoop()
*************************************************/
void TerminalAIHandle(void)
{
	TerminalAI1Calc();
	TerminalAI2Calc();
	TerminalAI3Calc();

//	TerminalAIProcess();
}
/*************************************************
  Function: Terminal AI1 Handle
  Description:Period:Main Loop
  Input: F5017/F5018/F5019/F5020/F5016
  Return: No
  Others: Be Called TerminalAIHandle()
*************************************************/
static void TerminalAI1Calc(void)
{
	AI1Q24.LLimitVol = (int32_t)FuncCode_Handle.Ram.F50.F5017;
	AI1Q24.LlimitRatio = (int32_t)FuncCode_Handle.Ram.F50.F5018;
	AI1Q24.HLimitVol = (int32_t)FuncCode_Handle.Ram.F50.F5019;
	AI1Q24.HlimitRatio = (int32_t)FuncCode_Handle.Ram.F50.F5020;

	//AI1Q24.Omit = (int32_t)FuncCode_Handle.Ram.F50.F5044*16777216/1000;
	AI1Q24.Omit = (int32_t)FuncCode_Handle.Ram.F50.F5044 * 1677;

	AIGainOffsetCalc(&AI1Q24);
}
/*************************************************
  Function: Terminal AI2 Handle
  Description:Period:Main Loop
  Input: F5022/F5023/F5024/F5025
  Return: No
  Others: Be Called TerminalAIHandle()
*************************************************/
static void TerminalAI2Calc(void)
{
	AI2Q24.LLimitVol = (int32_t)FuncCode_Handle.Ram.F50.F5022;
	AI2Q24.LlimitRatio = (int32_t)FuncCode_Handle.Ram.F50.F5023;
	AI2Q24.HLimitVol = (int32_t)FuncCode_Handle.Ram.F50.F5024;
	AI2Q24.HlimitRatio = (int32_t)FuncCode_Handle.Ram.F50.F5025;

	AI2Q24.Omit = (int32_t)FuncCode_Handle.Ram.F50.F5045 * 1677;

	AIGainOffsetCalc(&AI2Q24);
}
/*************************************************
  Function: Terminal AI3 Handle
  Description:Period:Main Loop
  Input: F5027/F5028/F5029/F5030
  Return: No
  Others: Be Called TerminalAIHandle()
*************************************************/
static void TerminalAI3Calc(void)
{
	AI3Q24.LLimitVol = (int32_t)FuncCode_Handle.Ram.F50.F5027;
	AI3Q24.LlimitRatio = (int32_t)FuncCode_Handle.Ram.F50.F5028;
	AI3Q24.HLimitVol = (int32_t)FuncCode_Handle.Ram.F50.F5029;
	AI3Q24.HlimitRatio = (int32_t)FuncCode_Handle.Ram.F50.F5030;

	AI3Q24.Omit = (int32_t)FuncCode_Handle.Ram.F50.F5046 * 1677;

	AIGainOffsetCalc(&AI3Q24);
}

void AIGainOffsetCalc(AICorrection *p)
{
	int64_t Y0,Y1,TempY;
	int64_t X0,X1,TempX;

	Y0 = (int64_t)p->LlimitRatio;
	Y1 = (int64_t)p->HlimitRatio;
	X0 = (int64_t)p->LLimitVol;
	X1 = (int64_t)p->HLimitVol;

	//Y1 = Y1 * RECIPROCAL1000;//1000*167772
	//Y0 = Y0 * RECIPROCAL1000;//-1000*167772

	Y1 = Y1 * 16777216;//1000*167772160
	Y0 = Y0 * 16777216;//-1000*167772160

	TempY = Y1 - Y0;//2e24
	TempX = X1 - X0;//10.00-(-10.00)=20.00

	if(TempX)
	{
		p->Gain = (int32_t)(TempY/TempX);
		p->Offset = (Y1 - X1 * p->Gain)/1000;//����b=Y0-X0*p->gain;
	}
	else
	{
		p->Gain = 0;
		p->Offset = 0;
	}
}


void TerminalAIProcess(void)//ģ����������Դ
{
	int32_t TempAI1,TempAI2,TempAI3;
	int64_t Temp;

	Temp = (int64_t)g_AI1FilQ24 * 1000;
	TempAI1 = (int32_t)(Temp >> 24);//����λС����ʵ�ʵ�ѹ

	Temp = (int64_t)g_AI2FilQ24 * 1000;
	TempAI2 = (int32_t)(Temp >> 24);

	Temp = (int64_t)g_AI3FilQ24 * 1000;
	TempAI3 = (int32_t)(Temp >> 24);

	//mxj===========================================================================
	if(TempAI1 >= AI1Q24.HLimitVol)
		ExAI.AI1Q24 = AI1Q24.HlimitRatio * RECIPROCAL1000;//1000*167772
	else if(TempAI1 <= AI1Q24.LLimitVol)
		ExAI.AI1Q24 = AI1Q24.LlimitRatio * RECIPROCAL1000;//1000*167772
	else
		ExAI.AI1Q24 = ((AI1Q24.Gain * (int64_t)g_AI1FilQ24)>>24) + AI1Q24.Offset;

	if(abs(ExAI.AI1Q24)<AI1Q24.Omit)
		ExAI.AI1Q24 = 0;

	if(TempAI2 >= AI2Q24.HLimitVol)
		ExAI.AI2Q24 = AI2Q24.HlimitRatio * RECIPROCAL1000;//1000*167772
	else if(TempAI2 <= AI2Q24.LLimitVol)
		ExAI.AI2Q24 = AI2Q24.LlimitRatio * RECIPROCAL1000;//1000*167772
	else
		ExAI.AI2Q24 = ((AI2Q24.Gain * (int64_t)g_AI2FilQ24)>>24) + AI2Q24.Offset;

	if(abs(ExAI.AI2Q24)<AI2Q24.Omit)
			ExAI.AI2Q24 = 0;

	if(TempAI3 >= AI3Q24.HLimitVol)
		ExAI.AI3Q24 = AI3Q24.HlimitRatio * RECIPROCAL1000;//1000*167772
	else if(TempAI3 <= AI3Q24.LLimitVol)
		ExAI.AI3Q24 = AI3Q24.LlimitRatio * RECIPROCAL1000;//1000*167772
	else
		ExAI.AI3Q24 = ((AI3Q24.Gain * (int64_t)g_AI3FilQ24)>>24) + AI3Q24.Offset;

	if(abs(ExAI.AI3Q24)<AI3Q24.Omit)
			ExAI.AI3Q24 = 0;
	//mxj==========================================================================
}


















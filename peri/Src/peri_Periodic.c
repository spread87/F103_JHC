
/*
 * peri_Periodic.c
 *
 * Created on: 2014-2-24
 * Author: JH-LvSL
 */
#define   PULSE_OUTPUT_TEST  1
#include <main.h>			//Declarations from DAVE3 Code Generation (includes SFR declaration)
//#include <peri_USBHandle.h>
#include <peri_Periodic.h>
#include <peri_GlobalVariablesExtern.h>
//#include <peri_AdcHandle.h>
#include <peri_SwScan.h>
//#include <peri_E2promHandle.h>
#include <peri_PanelLEDButton.h>

extern void InertiaIqRead();
extern void Test(void);	//for test
extern void CurrentOffsetCalc(void);
extern void AlmCheckMotorandDriver(void);
extern void CanUpdateHandle(void);
extern void VFErrClr(void);
extern void AlmReadNum(void);
extern void CurrentPIKpUpdate(void);
extern void VFCalc(int32_t );
extern void InitialAngle(void);
extern void TerminalDOSpdUpdate(void);
extern void Dcbus_Break_Calc(void);

//1ms timer treat
static const PERIODIC_FUNC peri_1ms_periodic_func_table[] =
{
  TimerControlThread, /* general timer */
  ModbusHandle,
  UpdateAllDiPort,//按键滤波,DI滤波
  FuncDIHandle,//9个IO输入口和虚拟输入口按照功能赋值
  //TerminalDOHandle,//6个IO输出口，根据配置的功能码进行输出
  //TerminalAOHandle,//3中模拟输出模式，2个AD输出口
  //AdcFilter1msHandle,
  //TerminalAIProcess,//mxj
  //CanHandle,
  //Dcbus_Break,//制动管开关
  //AlmCheckOC,//过流保护
  //AlmCheckOV,//moved from 2ms
  NULL_FUNC
};

//2ms timer treat
static const PERIODIC_FUNC peri_2ms_periodic_func_table[] =
{
	//AlmClr,
	//AlmCheck2ms,
	NULL_FUNC
};

//10ms timer treat
static const PERIODIC_FUNC peri_10ms_periodic_func_table[] =
{
	//AlmCheck10ms,
	//RunTimeStatistics,
	NULL_FUNC
};

//100ms timer treat
static const PERIODIC_FUNC peri_100ms_periodic_func_table[] =
{
	//AlmCheck100ms,
	NULL_FUNC
};

//500ms timer treat
static const PERIODIC_FUNC peri_500ms_periodic_func_table[] =
{
	//AlmCheck500ms,
	NULL_FUNC
};

//Function declaration
 void peri_periodic_func_start(const PERIODIC_FUNC *);

/**
 * @brief 1ms loop
 * @param[in] none
 * @return none
 *
 */
void Peri1msPrdSwi(void)
{
    static uint16_t count_2ms = 2;
    static uint16_t count_10ms = 1;
    static uint16_t count_100ms = 1;
    static uint16_t count_500ms = 1;

   // ServiceDog(); 记住要控制喂狗
    //peri_periodic_func_start(peri_1ms_periodic_func_table);

    if(count_2ms >= 2)
    {
        peri_periodic_func_start(peri_2ms_periodic_func_table);
        count_2ms = 0;
    }

    if(count_10ms >= 11)
    {
        peri_periodic_func_start(peri_10ms_periodic_func_table);
        count_10ms = 1;
    }

    if(count_100ms >= 101)
    {
        peri_periodic_func_start(peri_100ms_periodic_func_table);
        count_100ms = 1;
        SeriEncoder._100msCounter++;
    }

    if(count_500ms >= 501)
    {
        peri_periodic_func_start(peri_500ms_periodic_func_table);
        count_500ms = 1;
    }

    count_2ms++;
    count_10ms++;
    count_100ms++;
    count_500ms++;

    peri_periodic_func_start(peri_1ms_periodic_func_table);
}

/**
 * @brief treat timing program
 * @param[in] Timing handler
 * @return none
 *
 */
void peri_periodic_func_start(const PERIODIC_FUNC *func)
{
    while((*func) != NULL_FUNC)
    {
        (*func)();
        func++;
    }
}


void SysCore1msLoop(void)
{
	Timer1msCount++;
	SWI_Tick = 1;

	if( (!SysTimeSequ.Start_Flg) || (!SysTimeSequ.Sec10_Flg))//未启动好，返回先
	{
		g_PulInAllCount = 0;//位置给定滤波
		g_EncPulAllCount = 0;//
		FuncCode_Handle.Ram.FA0.FA087 = 0;

		PanelLEDButtonHandle();
		//UART_Test_Handle();//20180419 ���ݽ���
		//ModbusHandle();//20180419
		//CanHandle();//20180716������  20180725
		//Wave_dis();//20181229

		return;
	}

	//USBSendHandle();
//	AlmReadNum();//20170222  无法记录故障代码，更改回来
	//Test();//Control Driver Run/Stop

	PanelLEDButtonHandle();
	//UART_Test_Handle();//20180419
	//ModbusHandle();//20180419
	//CanHandle();//20180716������  20180725
	//Wave_dis();//20181229
}

/**
 * @brief 100us interrupt function
 * @param[in] none
 * @return none
 */
//#pragma CODE_SECTION(SysCore100usLoop, "ramfuncs");
void SysCore100usLoop(void)//静止不动最长100US
{
	//PanelLEDDisplay();//最终的输出最长17.6
//	IO004_SetOutputValue(IO004_Handle7,0);
	//AdcReadResult();//Run time:2.3us
	//AdcFilter100usHandle();	//Run Time:7.8us
	//Delay(1);//12us
	//Delay(2);//24us
//	IO004_SetOutputValue(IO004_Handle7,1);
}

/**
 * @brief system main loop
 * @param[in] none
 * @return none
 *
 */
void SysMainLoop(void)
{
//	static uint32_t BaseTimer = 0;
	int64_t temp;

	static uint8_t MainLoopCnt = 0;
	MainLoopCnt++;

	//if(MainLoopCnt>=8)
	//	MainLoopCnt=0;
	switch(MainLoopCnt)
	{
		case 1:
			//AlmCheckMotorandDriver();//20170420 mxj
			//CanUpdateHandle();//20170420 mxj
			break;
		case 2:
			//TerminalDOSetHandle();//20170420 mxj
			break;
		case 3:
			//TerminalDOSpdUpdate();//20171030 mxj
			break;
		case 4:
			//TerminalAIHandle();//AI1Q24,AI2Q24,AI3Q24
			break;
		case 5:
			//ProtectPUValueCalc();//保护限止值的计算
			break;
		case 6:
			//AlmRcdDisplay();//故障记录显示 10ms ->main loop 20170223
			//MotorParameterInit();//马达参数初始化
			CGParameterInit();
			//TempCalc();//20171204
			break;
		case 7:
			//Dcbus_Break_Calc();//20171129 制动能力参数更新
			break;
		case 8:
			SysTimeSequ.Start_Flg = OK;//ȫ����һ���� �ٽ��й��ϵļ��
			MainLoopCnt=0;
			break;
		default:
			MainLoopCnt=0;
			break;
	}

//	USBReceiveHandle();
//	USBSendHandle();

	//AlmReadNum();//20170106,moved from 1msloop

	/*Terminal DI Filter Handle*/
	UpdateAllDiPort();//1ms中断程序中已有，此处是否可以不要？

	/*Fan Control*/
	//FanCtrl();
	/*Relay Control*/
	//RelayCtrl();

	/*Function Code F6015 Handle*/
	FuncCodeF6015Handle();//F6015是用LED表示  待考虑？

	/*Function Code Storage*/
	//FcodeValueToEEPROM();//功能码存进E2PROM

	/*Alarm Record Refresh*/
	//AlmRcdRefresh();//故障记录刷新
	/*Alarm Record Storage*/
	//AlmRcdStorage();//故障记录存储

	/*运行状态记录*/
	/*DI/DO Status Monitor*/
	//TerminalIOSts();//输入输出口
	/*Monitor Parameter Refresh*/
	//MonitorParaHandle();//监控参数处理   待考虑？

	//SysTimeSequ.Start_Flg = OK;//全部走一编了 再进行故障的检测

	g_canview13+=1677;

	if(SWI_Tick==1)
  {
    SWI_Tick = 0;
    Peri1msPrdSwi();
  }
}

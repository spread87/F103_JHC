
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
int64_t g_testcount=0;
uint32_t Timer_Temp0 = 0;
uint32_t Timer_Temp1 = 0;

//1ms timer treat
static const PERIODIC_FUNC peri_1ms_periodic_func_table[] =
{
    TimerControlThread, /* general timer */

    UpdateAllDiPort,//�����˲�,DI�˲�
    FuncDIHandle,//9��IO����ں���������ڰ��չ��ܸ�ֵ
    //TerminalDOHandle,//6��IO����ڣ��������õĹ�����������
    //TerminalAOHandle,//3��ģ�����ģʽ��2��AD�����
    //AdcFilter1msHandle,
    //TerminalAIProcess,//mxj
    //CanHandle,
    //Dcbus_Break,//�ƶ��ܿ���
    //AlmCheckOC,//��������
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

   // ServiceDog(); ��סҪ����ι��
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
	//PulseOutputRead();//mxj 20160511
	//PanelLEDButtonHandle();/20180925   moved to down
	//UART_Test_Handle();//20180419
	//ModbusHandle();//20180419
	//CanHandle();//20180716������  20180725

	if( (!SysTimeSequ.Start_Flg) || (!SysTimeSequ.Sec10_Flg))//δ�����ã�������
	{
		g_PulInAllCount = 0;//λ�ø����˲�
		g_EncPulAllCount = 0;//
		FuncCode_Handle.Ram.FA0.FA087 = 0;

		PanelLEDButtonHandle();
		//UART_Test_Handle();//20180419 ���ݽ���
		//ModbusHandle();//20180419
		//CanHandle();//20180716������  20180725
		//Wave_dis();//20181229

		return;
	}
	Peri1msPrdSwi();//����1ms�ֱ�ִ��,�632US

	//USBSendHandle();
//	AlmReadNum();//20170222  �޷���¼���ϴ��룬���Ļ���
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
void SysCore100usLoop(void)//��ֹ�����100US
{
	//PanelLEDDisplay();//���յ�����17.6
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
			//ProtectPUValueCalc();//������ֵֹ�ļ���
			break;
		case 6:
			//AlmRcdDisplay();//���ϼ�¼��ʾ 10ms ->main loop 20170223
			//MotorParameterInit();//��������ʼ��
			CGParameterInit();
			//TempCalc();//20171204
			break;
		case 7:
			//Dcbus_Break_Calc();//20171129 �ƶ�������������
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
	//UpdateAllDiPort();//1ms�жϳ��������У��˴��Ƿ���Բ�Ҫ��

	/*Fan Control*/
	//FanCtrl();
	/*Relay Control*/
	//RelayCtrl();

	/*Function Code F6015 Handle*/
	FuncCodeF6015Handle();//F6015����LED��ʾ  �����ǣ�

	/*Function Code Storage*/
	//FcodeValueToEEPROM();//��������E2PROM

	/*Alarm Record Refresh*/
	//AlmRcdRefresh();//���ϼ�¼ˢ��
	/*Alarm Record Storage*/
	//AlmRcdStorage();//���ϼ�¼�洢

	/*����״̬��¼*/
	/*DI/DO Status Monitor*/
	//TerminalIOSts();//���������
	/*Monitor Parameter Refresh*/
	//MonitorParaHandle();//��ز�������   �����ǣ�

	//SysTimeSequ.Start_Flg = OK;//ȫ����һ���� �ٽ��й��ϵļ��

	g_canview13+=1677;
}






#include "gpio.h"

void SysCore1msLoop(void)
{
	/*
	Timer1msCount++;
	//PulseOutputRead();//mxj 20160511
	//PanelLEDButtonHandle();/20180925   moved to down
	//UART_Test_Handle();//20180419
	//ModbusHandle();//20180419
	//CanHandle();//20180716调试用  20180725

	if( (!SysTimeSequ.Start_Flg) || (!SysTimeSequ.Sec10_Flg))//未启动好，返回先
	{
		g_PulInAllCount = 0;//位置给定滤波
		g_EncPulAllCount = 0;//
		FuncCode_Handle.Ram.FA0.FA087 = 0;

		PanelLEDButtonHandle();
		UART_Test_Handle();//20180419 数据接收
		ModbusHandle();//20180419
		CanHandle();//20180716调试用  20180725
		Wave_dis();//20181229

		return;
	}
	Peri1msPrdSwi();//按照1ms分别执行,最长632US

	USBSendHandle();
//	AlmReadNum();//20170222  无法记录故障代码，更改回来
	Test();//Control Driver Run/Stop

	PanelLEDButtonHandle();
	UART_Test_Handle();//20180419
	ModbusHandle();//20180419
	CanHandle();//20180716调试用  20180725
	Wave_dis();//20181229
	*/
	int pin_val;
	HAL_GPIO_WritePin(CLK1_GPIO_Port, CLK1_Pin, GPIO_PIN_RESET); // GPIO_PIN_SET -- bit 1  GPIO_PIN_RESET -- bit0
	pin_val = HAL_GPIO_ReadPin(CLK1_GPIO_Port,CLK1_Pin);

}

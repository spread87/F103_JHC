/*
 * peri_UARTHandle.h
 *
 *  Created on: 2015-3-9
 *      Author: JH-LvSL
 */

#ifndef PERI_UARTHANDLE_H_
#define PERI_UARTHANDLE_H_


typedef struct
{
	uint32_t FrmStrt_F      :1;  //数据帧开始标志位
	uint32_t FrmFish_F      :1;  //数据帧接收完成标志位，可以开始处理
	uint32_t Inited_F       :1;  //参数已初始化
//	uint32_t RXOKFSTFLG		:1;
//	uint32_t RXFLG 			:1;   //每接收到一个字节置位一次，帧接收完成后清零
//	uint32_t RXFLGOK		:1;
	uint32_t TXFLG 			:1;//0:无数据发送，1：有数据要发送
	uint32_t TXFLGOK		:1;
	uint32_t CRCERRFLG		:1;	//0:Normal;1:CRC Parity Error

}UARTCOMFLG;

/*typedef struct
{
	uint32_t RXOKFSTFLG		:1;
	uint32_t RXFLG 			:1;
	uint32_t RXFLGOK		:1;
	uint32_t TXFLG 			:1;
	uint32_t TXFLGOK		:1;
	uint32_t CRCERRFLG		:1;	//0:Normal;1:CRC Parity Error

}UARTCOMFLG;*/

typedef union
{
	uint32_t all;
	UARTCOMFLG bit;
}UARTCOM;
extern uint16_t g_TXDataLength;
extern uint16_t g_RXDataLength;
extern uint8_t UartRxData[32];
extern uint8_t UartTxData[200];
extern UARTCOM UartCmnFlg;
//extern void UARTTXHandle(void);
extern void UARTTXDataHandle(uint8_t );
//extern uint32_t UART001_ClearReadbuf(const UART001_HandleType* Handle,uint32_t Count);
void UartRceiveCheck(uint16_t* Data);
#endif /* PERI_UARTHANDLE_H_ */

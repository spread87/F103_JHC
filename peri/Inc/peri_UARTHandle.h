/*
 * peri_UARTHandle.h
 *
 *  Created on: 2015-3-9
 *      Author: JH-LvSL
 */

#ifndef PERI_UARTHANDLE_H_
#define PERI_UARTHANDLE_H_

#define RX_MSG_SIZE 32
#define TX_MSG_SIZE 100

typedef struct
{
  uint16_t Len;
  uint8_t Buff[ RX_MSG_SIZE ];
} UART_MSG_t;

typedef struct
{
  uint32_t FrmFish_F :1;  //数据帧接收完成标志位，可以开始处理
  uint32_t Inited_F :1;   //参数已初始化
  uint32_t TXFLG :1;      //0:无数据发送，1：有数据要发送
  uint32_t CRCERRFLG :1;  //0:Normal;1:CRC Parity Error
}UARTCOMFLG;

typedef union
{
  uint32_t all;
  UARTCOMFLG bit;
} UARTCOM;

extern uint16_t g_TXDataLength;
extern uint16_t g_RXDataLength;
extern uint8_t UartRxData[32];
extern uint8_t UartTxData[200];
extern UARTCOM UartCmnFlg;

void UART_Tx_DMA(void);
void UART_Rx_TO(void);

#endif /* PERI_UARTHANDLE_H_ */

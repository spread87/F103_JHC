/*
 *peri_UARTHandle.c
 *
 *Created on: 2015-3-6
 *Author: JH-LvSL
 */

#include "usart.h"
#include "peri_GlobalVariablesExtern.h"
#include "peri_UARTHandle.h"
#include "tim.h"

#define RE_ENABLE() MX_GPIO_WritePin(REDE_GPIO_Port,REDE_Pin,0)
#define DE_ENABLE() MX_GPIO_WritePin(REDE_GPIO_Port,REDE_Pin,1)

uint16_t g_RXDataLength = 0;
uint16_t g_TXDataLength = 0;
UARTCOM UartCmnFlg;
uint8_t UartRxData[32]; //200->32
uint8_t UartTxData[200];
uint8_t FrmInterval = 0;

const uint32_t BaudRateTable[] =
{
  2400, //0
  4800,
  9600, //2
  19200,
  38400, //4
  57600,
  115200, //6
  230400,
  460800, //8
  921600,
  1843200 //10
};

const uint32_t ParityTable[] =
{
  /** No parity selected */
  UART_PARITY_NONE,
  /** Odd parity selected */
  UART_PARITY_ODD,
  /** Even parity selected */
  UART_PARITY_EVEN
};

const uint32_t StopBitsTable[] =
{
  /** One stop bit selected */
  UART_STOPBITS_1,
  /** Two stop bit selected */
  UART_STOPBITS_2
};

/*************************************************
 Function: UART Init
 Description:Setup BaudRate\Parity\Stop Bits
 Input: No
 Return: No
 Others:NO
 *************************************************/
void UARTInit(void)
{
  uint32_t BaudRate = 0;
  uint32_t Parity;
  uint32_t StopBits;
  uint32_t FcodeValue = 0;

  uint8_t FrmIntl[] = { 17, 8, 5, 3, 2, 1, 1, 1, 0, 0, 0 }; //帧间隔，3.5T,如9600,1000/9600*11*3.5=4.01ms

  FcodeValue = FuncCode_Handle.Ram.F60.F6020; //波特率
  if (FcodeValue > 10)
    FcodeValue = 2;	//9600
  BaudRate = BaudRateTable[FcodeValue];
  FrmInterval = FrmIntl[FcodeValue];

  FcodeValue = FuncCode_Handle.Ram.F60.F6021;	//校验位
  if (FcodeValue > 2)	//20190105
    FcodeValue = 0;	//无校验
  Parity = ParityTable[FcodeValue];

  FcodeValue = FuncCode_Handle.Ram.F60.F6022;	//停止位
  if (FcodeValue > 1)
    FcodeValue = 0;	//1位停止位
  StopBits = StopBitsTable[FcodeValue];

  //UART001_Configure(&UART001_Handle0, BaudRate, Parity, StopBits);
  UartCmnFlg.bit.Inited_F = 1; //初始化完成

  g_RXDataLength = 0;
  RE_ENABLE();
  MX_UART_Receive_IT(UartRxData, 1);
}

/*************************************************
 Function: UART send
 Description:
 Input: No
 Return: No
 Others:NO
 *************************************************/
void UART_Tx_DMA(void)
{
  DE_ENABLE();
  MX_UART_Transmit_DMA(UartTxData, g_TXDataLength);
}

/*************************************************
 Function: UART receive time out
 Description:
 Input: No
 Return: No
 Others:NO
 *************************************************/
void UART_Rx_TO(void)
{
  UartCmnFlg.bit.FrmFish_F = 1;
}

/*************************************************
 Function: UART tx done
 Description:
 Input: No
 Return: No
 Others:NO
 *************************************************/
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
  RE_ENABLE();
}

/*************************************************
 Function: UART receive isr callback
 Description:
 Input: No
 Return: No
 Others:NO
 *************************************************/
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  g_RXDataLength++;
  if (g_RXDataLength > 31)
  {
    g_RXDataLength = 0;
  }

  MX_UART_Receive_IT(&UartRxData[g_RXDataLength], 1);
  MX_TIM2_Start();
}

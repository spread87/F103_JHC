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
	uint32_t FrmStrt_F      :1;  //����֡��ʼ��־λ
	uint32_t FrmFish_F      :1;  //����֡������ɱ�־λ�����Կ�ʼ����
	uint32_t Inited_F       :1;  //�����ѳ�ʼ��
//	uint32_t RXOKFSTFLG		:1;
//	uint32_t RXFLG 			:1;   //ÿ���յ�һ���ֽ���λһ�Σ�֡������ɺ�����
//	uint32_t RXFLGOK		:1;
	uint32_t TXFLG 			:1;//0:�����ݷ��ͣ�1��������Ҫ����
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

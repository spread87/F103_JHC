/*
 * peri_CanHandle.h
 *
 *  Created on: 2014-12-12
 *      Author: JH-LvSL
 */

#ifndef PERI_CANHANDLE_H_
#define PERI_CANHANDLE_H_

typedef struct
{
	//uint32_t channel0;
	uint32_t channel1;
	uint32_t channel2;
	uint32_t channel3;
	uint32_t channel4;

	uint16_t tempCanMonEn; //CAN上位机监控功能使能 兼具发送时间间隔功能
	uint16_t tempCanMSEn; //CAN主从通信功能使能	 兼具发送时间间隔功能
	uint16_t tempCanMREn; //CAN主从通信功能使能	 兼具接收时间间隔功能

	uint8_t  Can_Step;
	uint8_t  Can_Number;
	uint16_t Can_ReceiveNo_Cnt;
}CanCmn;

typedef struct
{
	int32_t *add;
	int32_t	Att;
}CanValue;

#endif /* PERI_CANHANDLE_H_ */

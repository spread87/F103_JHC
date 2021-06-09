/*
 * peri_USBHandle.c
 *
 *  Created on: 2014-2-24
 *      Author: JH-LIUCL
 */
#include <main.h>			//Declarations from DAVE3 Code Generation (includes SFR declaration)
#include <XMC4400.h>
#include <arm_math.h>
#include <peri_GlobalVariablesExtern.h>
#include <peri_USBHandle.h>
//#include "peri_GlobalVariablesExtern.h"

uint8_t USBFlag = 0;
uint16_t USBLength = 0;
uint8_t USBRxBuffer[100] = {0};
uint8_t USBDataType1=0,USBDataType2=0,USBDataType3=0,USBDataType4=0;
uint32_t USBErrCount = 0;

/*************************************************
  Function: delay time
  Description: use as delay time
  Input: counts(Unit:us)
  Return: No
  Others: No
*************************************************/
uint32_t USBReadRam(uint32_t *address,uint8_t datalength)
{
	uint32_t RamData = 0;
	RamData = *address;
	switch(datalength)
	{
	case 1:
		RamData = (uint32_t)(*address);
		break;

	case 2:
		RamData = __PACKq7((*address),(*(address+1)),0,0);
		break;

	case 3:
		RamData = __PACKq7((*address),(*(address+1)),(*(address+2)),0);
		break;

	case 4:
		RamData = __PACKq7((*address),(*(address+1)),(*(address+2)),(*(address+3)));
		break;

	default:
		RamData = 0xFFFFFFFF;
	break;
	}

	return RamData;
}
/*************************************************
  Function: delay time
  Description: use as delay time
  Input: counts(Unit:us)
  Return: No
  Others: No
*************************************************/
void USBRamToTxbuffer(uint32_t data,uint8_t channel)
{
	switch(channel)
	{
	case 1:
		TxBuffer[4] = (uint8_t)(data & 0xFF);
		TxBuffer[3] = (uint8_t)((data>>8) & 0xFF);
		TxBuffer[2] = (uint8_t)((data>>16) & 0xFF);
		TxBuffer[1] = (uint8_t)((data>>24) & 0xFF);
		break;

	case 2:
		TxBuffer[8] = (uint8_t)(data & 0xFF);
		TxBuffer[7] = (uint8_t)((data>>8) & 0xFF);
		TxBuffer[6] = (uint8_t)((data>>16) & 0xFF);
		TxBuffer[5] = (uint8_t)((data>>24) & 0xFF);
		break;

	case 3:
		TxBuffer[12] = (uint8_t)(data & 0xFF);
		TxBuffer[11] = (uint8_t)((data>>8) & 0xFF);
		TxBuffer[10] = (uint8_t)((data>>16) & 0xFF);
		TxBuffer[9] = (uint8_t)((data>>24) & 0xFF);
		break;

	case 4:
		TxBuffer[16] = (uint8_t)(data & 0xFF);
		TxBuffer[15] = (uint8_t)((data>>8) & 0xFF);
		TxBuffer[14] = (uint8_t)((data>>16) & 0xFF);
		TxBuffer[13] = (uint8_t)((data>>24) & 0xFF);
		break;

	default:
		break;
	}
}



/*************************************************
  Function: delay time
  Description: use as delay time
  Input: counts(Unit:us)
  Return: No
  Others: No
*************************************************/
void USBReceiveHandle(void)
{
//	uint32_t USBTemp=0;

	USBLength = USBD_WinUSB_BytesReceived(WinUSBInfo.Config.OutEndpoint.Address);

	if(USBLength)
	{
		if(USBLength == 22)
		{
			/* Receive data */
			USBD_WinUSB_ReadData(WinUSBInfo.Config.OutEndpoint.Address,&RxBuffer,USBLength);
			memcpy(TxBuffer, RxBuffer, USBLength);

			/* Send data back to the host */
			USBD_WinUSB_WriteData(WinUSBInfo.Config.InEndpoint.Address,&TxBuffer,USBLength);

			USBDataType1 = RxBuffer[1];
			USBDataType2 = RxBuffer[6];
			USBDataType3 = RxBuffer[11];
			USBDataType4 = RxBuffer[16];
			ChannelDataAdd1 = (uint32_t)__PACKq7(RxBuffer[2],RxBuffer[3],RxBuffer[4],RxBuffer[5]);
			ChannelDataAdd2 = (uint32_t)__PACKq7(RxBuffer[7],RxBuffer[8],RxBuffer[9],RxBuffer[10]);
			ChannelDataAdd3 = (uint32_t)__PACKq7(RxBuffer[12],RxBuffer[13],RxBuffer[14],RxBuffer[15]);
			ChannelDataAdd4 = (uint32_t)__PACKq7(RxBuffer[17],RxBuffer[18],RxBuffer[19],RxBuffer[20]);
		}
		else if(USBLength == 3)
		{
			/* Receive data */
			USBD_WinUSB_ReadData(WinUSBInfo.Config.OutEndpoint.Address,&RxBuffer,USBLength);

			if((RxBuffer[0] == 0x55) && (RxBuffer[USBLength-1] == 0x55))
			{
				memcpy(TxBuffer, RxBuffer, USBLength);
				/* Send data back to the host */
				USBD_WinUSB_WriteData(WinUSBInfo.Config.InEndpoint.Address,&TxBuffer,USBLength);
			}
			else if((RxBuffer[0] == 0x0B) && (RxBuffer[USBLength-1] == 0x0B))
			{
				if(RxBuffer[1] == 0)
				{
					USBFlag = 0;
					TxBuffer[0] = 0x0B;
					USBRamToTxbuffer(*ChannelDataAdd1,1);//USBRamToTxbuffer(USBReadRam(ChannelDataAdd1,USBDataType1),1);
					USBRamToTxbuffer(*ChannelDataAdd2,2);//USBRamToTxbuffer(USBReadRam(ChannelDataAdd2,USBDataType2),2);
					USBRamToTxbuffer(*ChannelDataAdd3,3);//USBRamToTxbuffer(USBReadRam(ChannelDataAdd3,USBDataType3),3);
					USBRamToTxbuffer(*ChannelDataAdd4,4);//USBRamToTxbuffer(USBReadRam(ChannelDataAdd4,USBDataType4),4);
					TxBuffer[17] = 0x0B;
				}
				else if(RxBuffer[1] == 0x80)
				{
					USBFlag = 0x80;
				}
			}
		}
	}
}
/*************************************************
  Function: delay time
  Description: use as delay time
  Input: counts(Unit:us)
  Return: No
  Others: No
*************************************************/
void USBSendHandle(void)
{
	uint8_t USB_Status = 0;
	if(USBFlag == 0x80)
	{
		TxBuffer[0] = 0x0B;
		USBRamToTxbuffer(*ChannelDataAdd1,1);//USBRamToTxbuffer(USBReadRam(ChannelDataAdd1,USBDataType1),1);
		USBRamToTxbuffer(*ChannelDataAdd2,2);//USBRamToTxbuffer(USBReadRam(ChannelDataAdd2,USBDataType2),2);
		USBRamToTxbuffer(*ChannelDataAdd3,3);//USBRamToTxbuffer(USBReadRam(ChannelDataAdd3,USBDataType3),3);
		USBRamToTxbuffer(*ChannelDataAdd4,4);//USBRamToTxbuffer(USBReadRam(ChannelDataAdd4,USBDataType4),4);
		TxBuffer[17] = 0x0B;

		USB_Status = USBD_WinUSB_WriteData(WinUSBInfo.Config.InEndpoint.Address,&TxBuffer,18);
		//USBFlag = 0;
		if(USB_Status != USBD_WinUSB_OK)
		{
			USBErrCount++;
		}
		else
		{
			USBErrCount = 0;
		}

		if(USBErrCount > 5000)
		{
			USBFlag =0x0;
//			USBErrCount = 0;
		}
	}
}




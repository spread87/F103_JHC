/*
 * peri_ModbusHandle.c
 *
 * Created on: 2015-3-11
 * Author: JH-LvSL
 */
#include <main.h>			//Declarations from DAVE3 Code Generation (includes SFR declaration)
#include <peri_FuncCodeHandle.h>
#include <peri_GlobalVariablesExtern.h>
#include <peri_ModbusHandle.h>
#include <peri_UARTHandle.h>
void ModbusCMD03HandleV20(void);
void ModbusCMD06HandleV20(void);
void ModbusCMD10HandleV20(void);
void ModbusCMD22HandleV20(void);

void ModbusCMDIlgFuncV20(void);
void FrameByteLoad(uint16_t *DataLength,uint8_t byte1,uint8_t byte2);

extern int16_t Wave13[5];

extern int16_t WaveA[1000];
extern int16_t WaveB[1000];
extern int16_t WaveC[1000];
extern int16_t WaveD[1000];
//extern int16_t WaveE[1000];
extern void Wave_dis(void);

const uint8_t FrameErrTable[]=
{
		0x01,		/*0:Illegal Function Code*/
		0x02,		/*1:Illegal Data Address*/
		0x03,		/*2:Illegal Data, Out of range(Min to Max)*/
		0x04,		/*3:Data Invalid*/
		0x05,		/*4:Cmd Valid and System Busy*/
		0x06,		/*5:Slave Driver Busy*/
		0x20,		/*6:Frame Error(CRC Error or Length Error)*/
		0x21,		/*7:*/
		0x22,		/*8:*/
		0x23,		/*9:*/
		0x24		/*10:*/
};
const uint8_t WriteResultTable[]=
{
		0,			/*Write OK*/
		1,			/*Don't write for running*/
		2,			/*Valid Reset Power*/
		3			/*Only Read*/
};

const uint16_t UartTXWaitTimeTable[]=	//ms
{
		10,			//0:2400
		5,			//1:4800
		4,			//2:9600
		3,			//3:19200
		2,			//4:38400
		1,			//5:57600
		1,			//6:115200
		1,			//7:230400
		0,			//8:460800
		0,			//9:921600
		0,			//10:1843200
};

/*************************************************
  Function: Modbus Handle
  Description: Period:1ms
  Input: No
  Return: No
  Others:NO
*************************************************/
void ModbusHandle(void)
{
	ModbusRxHandle();

	ModbusTXHandle();
}
/*************************************************
  Function: CRC Parity Calculate
  Description:Period:1ms
  Input: No
  Return: No
  Others:NO
*************************************************/
uint16_t usCRC16( uint8_t * pucFrame, uint16_t usLen )
{
	uint8_t 	*p = pucFrame;
	uint16_t	ucCRCHi = 0xFF;
	uint16_t	ucCRCLo = 0xFF;
	uint16_t	iIndex;

    while( usLen-- )
    {
        iIndex = ucCRCLo ^ *( p++ );
        ucCRCLo = ( uint8_t )( ucCRCHi ^ aucCRCHi[iIndex] );
        ucCRCHi = aucCRCLo[iIndex];
    }
    //return ( ucCRCHi << 8 | ucCRCLo );
    return ( ucCRCLo << 8 | ucCRCHi );
}
/*************************************************
  Function: UART TX Handle
  Description: Period:1ms
  Input: No
  Return: No
  Others:NO
*************************************************/
void ModbusTXHandle(void)//CYL
{
	uint8_t i = 0;
	uint32_t FcodeValue = 0;
	uint16_t temp;
	static uint16_t DelayTime = 0;
	static uint16_t DelayTime0 = 0;

	if(UartCmnFlg.bit.TXFLG == NG )
	{
		return;
	}

	FcodeValue = FuncCode_Handle.Ram.F60.F6020;

	if(UartCmnFlg.bit.TXFLGOK == NG )
	{
		IO004_SetOutputValue(IO004_Handle26,1);
		UART001_WriteDataBytesM(&UART001_Handle0, &UartTxData[0], g_TXDataLength);//20190103
		for (i = 0; i < g_TXDataLength; i++)
			UartTxData[i] = 0;
		UartCmnFlg.bit.TXFLGOK = OK;
		DelayTime = 0;
		DelayTime0 = 0;
	}
	else
	{
		//if(((UART001_Handle0.UartRegs->TRBSR & 0x800) >> 11))
		if(((UART001_Handle0.UartRegs->TRBSR & 0x800) >> 11)||(DelayTime0 >=2000))//��2000ms��û�з�����ϣ�ֱ��ֹͣ����
		{
			//if(DelayTime > UartTXWaitTimeTable[FcodeValue] )
			if((DelayTime >= UartTXWaitTimeTable[FcodeValue] )||(DelayTime>=50))
			{
				IO004_SetOutputValue(IO004_Handle26,0);
				UartCmnFlg.bit.TXFLG = NG;
				UartCmnFlg.bit.TXFLGOK = NG;
				g_TXDataLength = 0;
			}
			else
			{
				DelayTime++;
			}
		}
		else
		{
			DelayTime0++;
		}
	}
}

/*************************************************
  Function: Modbus Receive Data Handle
  Description:Period:1ms
  Input: No
  Return: No
  Others:NO
*************************************************/
void ModbusRxHandle(void)
{
	uint32_t FcodeValue = 0;
	uint16_t RxDataLength = 0;
	uint16_t TxDataLength = 0;
	uint16_t CRCRXValue = 0;
	uint16_t CRCTXValue = 0;
	uint16_t CRCCalcValue = 0;
	uint8_t i=0;

	if(UartCmnFlg.bit.FrmFish_F==0)//(UartCmnFlg.bit.RXOKFSTFLG != NG)||(UartCmnFlg.bit.RXFLGOK != OK))
	{
		return;
	}
	UartCmnFlg.bit.FrmFish_F=0;

	RxDataLength = g_RXDataLength;//sizeof(UartRxData)/sizeof(UartRxData[0]);

	FuncCode_Handle.Ram.FA0.FA044 =  (uint32_t)(UartRxData[1] << 16) + (UartRxData[2] << 8) + UartRxData[3];//mxj test

	//if(FuncCode_Handle.Ram.F60.F6018==1 ) //V1.0 �Ǳ�Э��
	if((FuncCode_Handle.Ram.F60.F6018 & 0x000F) == 1 ) //V1.0 �Ǳ�Э��
	{
		if((RxDataLength != 0x08)&&(RxDataLength != 0x0A))
		{
			FuncCode_Handle.Ram.FA0.FA043 += 2 ;//���ݳ��ȴ���
			return;
		}
		else if((UartRxData[1] != 0x06)&&(UartRxData[1] != 0x03))
		{
			FuncCode_Handle.Ram.FA0.FA043 += 3 ;//��дָ�����
			return;
		}
	}
	else  //V2.0��׼Э��
	{
		if(UartRxData[1]==0x03||UartRxData[1]==0x06)
		{
			if(RxDataLength!=8)
			{
				FuncCode_Handle.Ram.FA0.FA043 += 2;//���ݳ��ȴ���
				return;
			}
		}
		else if(UartRxData[1]==0x10)
		{
			if(RxDataLength!=9+UartRxData[6])
			{
				FuncCode_Handle.Ram.FA0.FA043 += 2;//���ݳ��ȴ���
				return;
			}
		}
		//else if(UartRxData[1]==0x13)//����Э�� 20181229
		//{
		//	if(RxDataLength!=4)
		//	{
		//		FuncCode_Handle.Ram.FA0.FA043 += 2;//���ݳ��ȴ���
		//		return;
		//	}
		//}
		else if(UartRxData[1]==0x22)//����Э�� 20181229
		{
			if(RxDataLength!=6)
			{
				FuncCode_Handle.Ram.FA0.FA043 += 2;//���ݳ��ȴ���
				return;
			}
		}
		else
		{
			FuncCode_Handle.Ram.FA0.FA043 += 3 ;//��дָ�����
			//return; //�������쳣ʱ����Ҫ�����쳣����
		}
	}
	FcodeValue = FuncCode_Handle.Ram.F60.F6019;//Communication Address
	//if((FcodeValue != UartRxData[0]) && (UartRxData[0] != 0))//Address Error
	if((FcodeValue != UartRxData[0]) && (UartRxData[0] != 0) && (UartRxData[0] != 248))//Address Error 20181229
	{
		return;
	}

	/*CRC Parity Judge*/
	if((RxDataLength < 4)||(RxDataLength > 32))//20180419
	{
		CRCRXValue = 0;
		CRCCalcValue = 1;
	}
	else
	{
		CRCRXValue = (uint16_t)UartRxData[RxDataLength-2]<<8 | UartRxData[RxDataLength-1];
		CRCCalcValue =usCRC16((uint8_t *)UartRxData,(RxDataLength-2));
	}

	if(CRCRXValue == CRCCalcValue)
	{
		//if(FuncCode_Handle.Ram.F60.F6018==1 ) //V1.0 �Ǳ�Э��
		if((FuncCode_Handle.Ram.F60.F6018 & 0x000F) == 1 ) //V1.0 �Ǳ�Э��
		{
			ModbusCMD03Handle();
			ModbusCMD06Handle();
		}
		else  //V2.0��׼Э��
		{
			ModbusCMD03HandleV20();
			ModbusCMD06HandleV20();
			ModbusCMD10HandleV20();
			ModbusCMD22HandleV20();
			ModbusCMDIlgFuncV20();
		}

		UartCmnFlg.bit.CRCERRFLG = NG;
	}
	else
	{
		FuncCode_Handle.Ram.FA0.FA043 ++ ;//CRCУ�����

		UartCmnFlg.bit.CRCERRFLG = OK;
		//UartCmnFlg.bit.TXFLG = OK; OK �ᷢ������
	}

//	for(i=0;i<32;i++)
//	{
//		UartRxData[i] = 0;
//	}
}
/*************************************************
  Function: Modbus Receive Data Handle(CMD:0x03)
  Description:Period:1ms
  Input: No
  Return: No
  Others:No
*************************************************/
void ModbusCMD03Handle(void)//��
{
	uint16_t DataAddress = 0;
	uint16_t DataNum = 0;
	uint16_t TxDataLength = 0;
	uint16_t CRCTXValue = 0;
	uint32_t DataValue = 0;
	uint8_t  i=0;

	if(UartRxData[1] != 0x03||UartRxData[0] == 0)  //CYL
	{
		return;
	}
//	else if(RxDataLength != 0x08)
//	{
//		return;
//	}

	UartCmnFlg.bit.TXFLG = OK;
	DataAddress = (uint16_t)UartRxData[2]<<8 | UartRxData[3];
	DataNum = (uint16_t)UartRxData[4]<<8 | UartRxData[5];

	UartTxData[TxDataLength] = UartRxData[TxDataLength];//0xf8
	TxDataLength++;
	UartTxData[TxDataLength] = UartRxData[TxDataLength];//0x03
	TxDataLength++;
	UartTxData[TxDataLength] = (uint8_t)((DataNum>>6)&0xFF);//����2��8�η�  �ٳ���4
	TxDataLength++;
	UartTxData[TxDataLength] = (uint8_t)((DataNum<<2)&0xFF);//ֱ�ӳ���4
	TxDataLength++;

	switch((DataAddress>>8)&0xFF)
	{
		case 0x10://������Ϣ�洢��  CYL��Ϊ����ֵ��������Ȧ���͵�Ȧλ��
			if(DataAddress==0x1014) //��Ȧλ�õ�16λ
			{
				UartTxData[2]=(uint8_t)(DataNum*2);
				if(DataNum==1)
				{
					UartTxData[3]=(uint8_t)((SeriEncoder._1RevPuls&0x0000FF00)>>8);
					UartTxData[4]=(uint8_t)(SeriEncoder._1RevPuls&0x000000FF);
					TxDataLength=5;
				}
				else if(DataNum==2)
				{
					UartTxData[3]=(uint8_t)((SeriEncoder._1RevPuls&0x0000FF00)>>8);
					UartTxData[4]=(uint8_t)(SeriEncoder._1RevPuls&0x000000FF);
					UartTxData[5]=(uint8_t)((SeriEncoder._1RevPuls&0xFF000000)>>24);
					UartTxData[6]=(uint8_t)((SeriEncoder._1RevPuls&0x00FF0000)>>16);
					TxDataLength=7;
				}
				else
				{
					UartTxData[1]=0x83;
					UartTxData[2]=FrameErrTable[1]; //�Ƿ����ݵ�ַ
					TxDataLength=3;
				}
			}
			else if(DataAddress==0x1015) //��Ȧλ�ø�16λ
			{
				UartTxData[2]=2;
				if(DataNum==1)
				{
					UartTxData[3]=(uint8_t)((SeriEncoder._1RevPuls&0xFF000000)>>24);
					UartTxData[4]=(uint8_t)((SeriEncoder._1RevPuls&0x00FF0000)>>16);
					TxDataLength=5;
				}
				else
				{
					UartTxData[1]=0x83;
					UartTxData[2]=FrameErrTable[1]; //�Ƿ����ݵ�ַ
					TxDataLength=3;
				}
			}
			else if(DataAddress==0x101C) //Ȧ��
			{
				UartTxData[2]=2;
				if(DataNum==1)
				{
					UartTxData[3]=(uint8_t)((SeriEncoder.CycleCnt&0xFF00)>>8);
					UartTxData[4]=(uint8_t)(SeriEncoder.CycleCnt&0x00FF);
					TxDataLength=5;
				}
				else
				{
					UartTxData[1]=0x83;
					UartTxData[2]=FrameErrTable[1]; //�Ƿ����ݵ�ַ
					TxDataLength=3;
				}
			}
			else
			{
				UartTxData[1]=0x83;
				UartTxData[2]=FrameErrTable[1]; //�Ƿ����ݵ�ַ
				TxDataLength=3;
			}
/*			for(i=0;i<DataNum;i++)
			{
				DataValue = AlarmRcdTable[UartRxData[3] + i];
				UartTxData[TxDataLength] = (uint8_t)((DataValue>>24)&0xFF);
				TxDataLength++;
				UartTxData[TxDataLength] = (uint8_t)((DataValue>>16)&0xFF);
				TxDataLength++;
				UartTxData[TxDataLength] = (uint8_t)((DataValue>>8)&0xFF);
				TxDataLength++;
				UartTxData[TxDataLength] = (uint8_t)(DataValue&0xFF);
				TxDataLength++;
			}*/
		break;

		case 0x80://ComCtrlFlg �����ּĴ���
			DataValue = ComCtrlFlg.all;
			UartTxData[TxDataLength] = (uint8_t)((DataValue>>24)&0xFF);
			TxDataLength++;
			UartTxData[TxDataLength] = (uint8_t)((DataValue>>16)&0xFF);
			TxDataLength++;
			UartTxData[TxDataLength] = (uint8_t)((DataValue>>8)&0xFF);
			TxDataLength++;
			UartTxData[TxDataLength] = (uint8_t)(DataValue&0xFF);
			TxDataLength++;
		break;

		case 0x81://״̬��
			DataValue = CoreStatusFlag.all;
			UartTxData[TxDataLength] = (uint8_t)((DataValue>>24)&0xFF);
			TxDataLength++;
			UartTxData[TxDataLength] = (uint8_t)((DataValue>>16)&0xFF);
			TxDataLength++;
			UartTxData[TxDataLength] = (uint8_t)((DataValue>>8)&0xFF);
			TxDataLength++;
			UartTxData[TxDataLength] = (uint8_t)(DataValue&0xFF);
			TxDataLength++;
		break;

		case 0x82://�����������
			if( (DataAddress&0xFF) <= 8 )
			DataValue = *(&COMVirtualDI.DI1 + (DataAddress&0xFF));
			else if ( (DataAddress&0xFF)== 9 )//DIǿ��ʹ��
			DataValue = ForceDIOnEn.all;//ONʹ��״̬λ
			else if ( (DataAddress&0xFF)== 0xA )
			DataValue = ForceDIOffEn.all;//OFFʹ��״̬λ
			UartTxData[TxDataLength] = (uint8_t)((DataValue>>24)&0xFF);
			TxDataLength++;
			UartTxData[TxDataLength] = (uint8_t)((DataValue>>16)&0xFF);
			TxDataLength++;
			UartTxData[TxDataLength] = (uint8_t)((DataValue>>8)&0xFF);
			TxDataLength++;
			UartTxData[TxDataLength] = (uint8_t)(DataValue&0xFF);
			TxDataLength++;
		break;

		case 0x83://��������˿�
			if ( (DataAddress&0xFF)== 0 )//DIǿ��ʹ��
			DataValue = ForceDOOnEn.all;//ONʹ��״̬λ
			else if ( (DataAddress&0xFF)== 1 )
			DataValue = ForceDOOffEn.all;//OFFʹ��״̬λ
			UartTxData[TxDataLength] = (uint8_t)((DataValue>>24)&0xFF);
			TxDataLength++;
			UartTxData[TxDataLength] = (uint8_t)((DataValue>>16)&0xFF);
			TxDataLength++;
			UartTxData[TxDataLength] = (uint8_t)((DataValue>>8)&0xFF);
			TxDataLength++;
			UartTxData[TxDataLength] = (uint8_t)(DataValue&0xFF);
			TxDataLength++;
		break;

		case 0x84://����
			DataValue = (Inertia_status.study_times<<16) | (g_InertiaTotal & 0xffff);
			UartTxData[TxDataLength] = (uint8_t)((DataValue>>24)&0xFF);
			TxDataLength++;
			UartTxData[TxDataLength] = (uint8_t)((DataValue>>16)&0xFF);
			TxDataLength++;
			UartTxData[TxDataLength] = (uint8_t)((DataValue>>8)&0xFF);
			TxDataLength++;
			UartTxData[TxDataLength] = (uint8_t)(DataValue&0xFF);
			TxDataLength++;

		break;

		case 0x85://ԭ��ع�  20161212 CYL
			DataValue =  SHomeFlag.all;
			UartTxData[TxDataLength] = (uint8_t)((DataValue>>24)&0xFF);
			TxDataLength++;
			UartTxData[TxDataLength] = (uint8_t)((DataValue>>16)&0xFF);
			TxDataLength++;
			UartTxData[TxDataLength] = (uint8_t)((DataValue>>8)&0xFF);
			TxDataLength++;
			UartTxData[TxDataLength] = (uint8_t)(DataValue&0xFF);
			TxDataLength++;
		break;

		default:
			for(i=0;i<DataNum;i++)
			{
				DataValue = FuncCodeReadRAMValue((uint16_t)UartRxData[2] * 1000 + UartRxData[3] + i);
				UartTxData[TxDataLength] = (uint8_t)((DataValue>>24)&0xFF);
				TxDataLength++;
				UartTxData[TxDataLength] = (uint8_t)((DataValue>>16)&0xFF);
				TxDataLength++;
				UartTxData[TxDataLength] = (uint8_t)((DataValue>>8)&0xFF);
				TxDataLength++;
				UartTxData[TxDataLength] = (uint8_t)(DataValue&0xFF);
				TxDataLength++;
			}
		break;
	}

	CRCTXValue = usCRC16((uint8_t *)UartTxData,TxDataLength);
	UartTxData[TxDataLength] = (uint8_t)((CRCTXValue>>8) & 0xFF);
	TxDataLength++;
	UartTxData[TxDataLength] = (uint8_t)(CRCTXValue & 0xFF);
	TxDataLength++;
	g_TXDataLength = TxDataLength;

}
inline void FrameByteLoad(uint16_t *DataLength,uint8_t byte1,uint8_t byte2)
{
	UartTxData[(*DataLength)]=byte1;
	(*DataLength)++;
	UartTxData[(*DataLength)]=byte2;
	(*DataLength)++;
}
/*************************************************
  Function: Modbus Receive Data Handle(CMD:0x03)
  Description:Period:1ms
  Input: No
  Return: No
  Others:No
*************************************************/
void ModbusCMD03HandleV20(void)//��
{
	uint16_t DataAddress = 0;
	uint16_t tempStrtAddr=0;
	uint16_t DataNum = 0;
	uint16_t TxDataLength = 0;
	uint16_t CRCTXValue = 0;
	uint32_t DataValue = 0;
	uint8_t  i=0;
	uint8_t tempRegNo=0;
	if(UartRxData[1] != 0x03||UartRxData[0] == 0)  //CYL
	{
		return;
	}

	UartCmnFlg.bit.TXFLG = OK;
	DataAddress = (uint16_t)UartRxData[2]<<8 | UartRxData[3];
	DataNum = (uint16_t)UartRxData[4]<<8 | UartRxData[5];

	UartTxData[TxDataLength] = UartRxData[TxDataLength];//0xf8
	TxDataLength++;
	UartTxData[TxDataLength] = UartRxData[TxDataLength];//0x03
	TxDataLength++;
	UartTxData[TxDataLength] = (uint8_t)(DataNum<<1);//�����ֽ���
	TxDataLength++;
	if(DataNum>10) //���ζ�ȡ�ļĴ������������޶�ֵ
	{
		UartTxData[1]=0x83;
		UartTxData[2]=FrameErrTable[3]; //�ӻ������Ʊ�
		TxDataLength=3;
	}
	else
	{
		switch((DataAddress>>8)&0xFF)
		{
			case 0x10://������Ϣ�洢��  CYL��Ϊ����ֵ��������Ȧ���͵�Ȧλ��
				switch(DataAddress)
				{
				case 0x1014: //��Ȧλ��L
					FrameByteLoad((&TxDataLength),(uint8_t)((SeriEncoder._1RevPuls&0x0000FF00)>>8),(uint8_t)(SeriEncoder._1RevPuls&0x000000FF));
					if((++tempRegNo)==DataNum)
						break;
				case 0x1015://��Ȧλ��H
					FrameByteLoad(&TxDataLength,(uint8_t)((SeriEncoder._1RevPuls&0xFF000000)>>24),(uint8_t)((SeriEncoder._1RevPuls&0x00FF0000)>>16));
					if((++tempRegNo)==DataNum)
						break;
				case 0x1016://�Ƿ���ַ
					UartTxData[1]=0x83;
					UartTxData[2]=FrameErrTable[1]; //�Ƿ����ݵ�ַ
					TxDataLength=3;
					break;
				case 0x101C://��Ȧλ��
					FrameByteLoad(&TxDataLength,(uint8_t)((SeriEncoder.CycleCnt&0xFF00)>>8),(uint8_t)(SeriEncoder.CycleCnt&0x00FF));
					if((++tempRegNo)==DataNum)
						break;
				default://�Ƿ���ַ
					UartTxData[1]=0x83;
					UartTxData[2]=FrameErrTable[1]; //�Ƿ����ݵ�ַ
					TxDataLength=3;
					break;
				}
			break;

			case 0x80:
				switch(DataAddress)
				{
				case 0x8000: //���������ֵ�16λ
					FrameByteLoad(&TxDataLength,(uint8_t)(ComCtrlFlg.all>>8),(uint8_t)(ComCtrlFlg.all));
					if((++tempRegNo)==DataNum)
						break;
				case 0x8001: //���������ָ�16λ
					FrameByteLoad(&TxDataLength,(uint8_t)(ComCtrlFlg.all>>24),(uint8_t)(ComCtrlFlg.all>>16));
					if((++tempRegNo)==DataNum)
						break;
				case 0x8002: //λ�����������16λ
					FrameByteLoad(&TxDataLength,(uint8_t)(g_ComPulse>>8),(uint8_t)(g_ComPulse));
					if((++tempRegNo)==DataNum)
						break;
				case 0x8003: //λ�����������16λ
					FrameByteLoad(&TxDataLength,(uint8_t)(g_ComPulse>>24),(uint8_t)(g_ComPulse>>16));
					if((++tempRegNo)==DataNum)
						break;
				case 0x8004: //��������
					FrameByteLoad(&TxDataLength,(uint8_t)(0),(uint8_t)(0));
					if((++tempRegNo)==DataNum)
						break;
				case 0x8005://��������
					FrameByteLoad(&TxDataLength,(uint8_t)(0),(uint8_t)(0));
					if((++tempRegNo)==DataNum)
						break;
				case 0x8006://�ٶȸ�����16λ (��λ0.1rpm)��������
					FrameByteLoad(&TxDataLength,(uint8_t)(g_ComSpeed>>8),(uint8_t)(g_ComSpeed));
					if((++tempRegNo)==DataNum)
						break;
				case 0x8007://�ٶȸ�����16λ (��λ0.1rpm)��������
					FrameByteLoad(&TxDataLength,(uint8_t)(g_ComSpeed>>24),(uint8_t)(g_ComSpeed>>16));
					if((++tempRegNo)==DataNum)
						break;
				case 0x8008://ת�ظ��� (��λ0.1%)
					FrameByteLoad(&TxDataLength,(uint8_t)(g_ComTorque>>8),(uint8_t)(g_ComTorque));
					if((++tempRegNo)==DataNum)
						break;
				case 0x8009://PID���� (��λ0.1%)��������
					FrameByteLoad(&TxDataLength,(uint8_t)(g_ComPidRef>>8),(uint8_t)(g_ComPidRef));
					if((++tempRegNo)==DataNum)
						break;
				case 0x800A://AO1�����λ���ٷֱ� (��λ0.1%)��������
					FrameByteLoad(&TxDataLength,(uint8_t)(g_ComAO1>>8),(uint8_t)(g_ComAO1));
					if((++tempRegNo)==DataNum)
						break;
				case 0x800B://AO2�����λ���ٷֱ� (��λ0.1%)��������
					FrameByteLoad(&TxDataLength,(uint8_t)(g_ComAO2>>8),(uint8_t)(g_ComAO2));
					if((++tempRegNo)==DataNum)
						break;
				case 0x800C://DO�����λ������,��0λ��ӦDO1,�������ƣ�������
					FrameByteLoad(&TxDataLength,(uint8_t)(g_ComDO>>8),(uint8_t)(g_ComDO));
					if((++tempRegNo)==DataNum)
						break;
				case 0x800D://��λ������ģʽѡ�񣨱�����
					FrameByteLoad(&TxDataLength,(uint8_t)(g_ComCmdModel>>8),(uint8_t)(g_ComCmdModel));
					if((++tempRegNo)==DataNum)
						break;
				default://�Ƿ���ַ
					UartTxData[1]=0x83;
					UartTxData[2]=FrameErrTable[1]; //�Ƿ����ݵ�ַ
					TxDataLength=3;
					break;
				}
			break;

			case 0x81://״̬��
				switch(DataAddress)
				{
				case 0x810B:
					FrameByteLoad(&TxDataLength,(uint8_t)(CoreStatusFlag.all>>8),(uint8_t)(CoreStatusFlag.all));
					if((++tempRegNo)==DataNum)
						break;
				case 0x810C:
					FrameByteLoad(&TxDataLength,(uint8_t)(CoreStatusFlag.all>>24),(uint8_t)(CoreStatusFlag.all>>16));
					if((++tempRegNo)==DataNum)
						break;
				//case 0x810D://20181229
				//	FrameByteLoad(&TxDataLength,(uint8_t)(CoreStatusFlag.all>>40),(uint8_t)(CoreStatusFlag.all>>32));
				//	if((++tempRegNo)==DataNum)
				//		break;
				case 0x810D://20181229
					if((!CoreStatusFlag.bit.WAVE_DATA_F)&&(!CoreStatusFlag.bit.WAVE_DATA_S))
						CoreStatusFlag.bit.WAVE_DATA_S = OK;
					FrameByteLoad(&TxDataLength,(uint8_t)(CoreStatusFlag.all>>40),(uint8_t)(CoreStatusFlag.all>>32));
					if((++tempRegNo)==DataNum)
						break;

				case 0x8120://20181229
					Wave13_dis();		//	������   ����5·
					DataValue = Wave13[0];//16bit
					FrameByteLoad(&TxDataLength,(uint8_t)(DataValue>>8),(uint8_t)(DataValue));
					if((++tempRegNo)==DataNum)
						break;
				//case 0x8121://20181229
					DataValue = Wave13[1];//16bit
					FrameByteLoad(&TxDataLength,(uint8_t)(DataValue>>8),(uint8_t)(DataValue));
					if((++tempRegNo)==DataNum)
						break;
				//case 0x8122://20181229
					DataValue = Wave13[2];//16bit
					FrameByteLoad(&TxDataLength,(uint8_t)(DataValue>>8),(uint8_t)(DataValue));
					if((++tempRegNo)==DataNum)
						break;
				//case 0x8123://20181229
					DataValue = Wave13[3];//16bit
					FrameByteLoad(&TxDataLength,(uint8_t)(DataValue>>8),(uint8_t)(DataValue));
					if((++tempRegNo)==DataNum)
						break;
				//case 0x8124://20181229
					DataValue = Wave13[4];//16bit
					FrameByteLoad(&TxDataLength,(uint8_t)(DataValue>>8),(uint8_t)(DataValue));
					if((++tempRegNo)==DataNum)
						break;
				case 0x8125://20181229
					DataValue = 0;//16bit  reserve
					FrameByteLoad(&TxDataLength,(uint8_t)(DataValue>>8),(uint8_t)(DataValue));
					if((++tempRegNo)==DataNum)
						break;

				default://�Ƿ���ַ
					UartTxData[1]=0x83;
					UartTxData[2]=FrameErrTable[1]; //�Ƿ����ݵ�ַ
					TxDataLength=3;
					break;
				}
			break;

			case 0x82://�����������
				switch(DataAddress)
				{
				case 0x8200:
					FrameByteLoad(&TxDataLength,(uint8_t)(ForceDIOnEn.all>>8),(uint8_t)(ForceDIOnEn.all));
					if((++tempRegNo)==DataNum)
						break;
				case 0x8201:
					FrameByteLoad(&TxDataLength,(uint8_t)(ForceDIOffEn.all>>8),(uint8_t)(ForceDIOffEn.all));
					if((++tempRegNo)==DataNum)
						break;
				default:
					UartTxData[1]=0x83;
					UartTxData[2]=FrameErrTable[1]; //�Ƿ����ݵ�ַ
					TxDataLength=3;
					break;
				}
			break;

			case 0x83://��������˿�
				switch(DataAddress)
				{
				case 0x8300:
					FrameByteLoad(&TxDataLength,(uint8_t)(ForceDOOnEn.all>>8),(uint8_t)(ForceDOOnEn.all));
					if((++tempRegNo)==DataNum)
						break;
				case 0x8301:
					FrameByteLoad(&TxDataLength,(uint8_t)(ForceDOOffEn.all>>8),(uint8_t)(ForceDOOffEn.all));
					if((++tempRegNo)==DataNum)
						break;
				default:
					UartTxData[1]=0x83;
					UartTxData[2]=FrameErrTable[1]; //�Ƿ����ݵ�ַ
					TxDataLength=3;
					break;
				}
			break;

			case 0x84://����
				switch(DataAddress)
				{
				case 0x8400:
					FrameByteLoad(&TxDataLength,(uint8_t)(g_InertiaTotal>>8),(uint8_t)(g_InertiaTotal));
					if((++tempRegNo)==DataNum)
						break;
				case 0x8401:
					FrameByteLoad(&TxDataLength,(uint8_t)(Inertia_status.study_times>>8),(uint8_t)(Inertia_status.study_times));
					if((++tempRegNo)==DataNum)
						break;
				default:
					UartTxData[1]=0x83;
					UartTxData[2]=FrameErrTable[1]; //�Ƿ����ݵ�ַ
					TxDataLength=3;
					break;
				}
			break;

			case 0x85://ԭ��ع�  20161212 CYL
				switch(DataAddress)
				{
				case 0x8500:
					FrameByteLoad(&TxDataLength,(uint8_t)(SHomeFlag.all>>8),(uint8_t)(SHomeFlag.all));
					if((++tempRegNo)==DataNum)
						break;
				case 0x8501:
					FrameByteLoad(&TxDataLength,(uint8_t)(SHomeFlag.all>>24),(uint8_t)(SHomeFlag.all>>16));
					if((++tempRegNo)==DataNum)
						break;
				default:
					UartTxData[1]=0x83;
					UartTxData[2]=FrameErrTable[1]; //�Ƿ����ݵ�ַ
					TxDataLength=3;
					break;
				}
			break;

			default:
				//if((UartRxData[3]>199)||(UartRxData[2]>0x0A)||(UartRxData[2] * 1000+UartRxData[3]+DataNum)>10199)
				if((UartRxData[3]>199)||(UartRxData[2]>0x0A)||(UartRxData[2] * 1000+UartRxData[3]+DataNum)>10200)//20180124 CYL
				{
					UartTxData[1]=0x83;
					UartTxData[2]=FrameErrTable[1]; //�Ƿ����ݵ�ַ
					TxDataLength=3;
				}
				else
				{
					tempStrtAddr=(uint16_t)UartRxData[2] * 1000+UartRxData[3]/2;
					DataValue = FuncCodeReadRAMValue(tempStrtAddr + i/2);
					if(UartRxData[3]%2!=0)
					{
						FrameByteLoad(&TxDataLength,(uint8_t)(DataValue>>24),(uint8_t)(DataValue>>16));
						tempStrtAddr++;
						DataNum--;
					}
					for(i=0;i<DataNum;i+=2)
					{
						DataValue = FuncCodeReadRAMValue(tempStrtAddr + i/2);
						FrameByteLoad(&TxDataLength,(uint8_t)(DataValue>>8),(uint8_t)(DataValue));
						if((i+1)<DataNum)
							FrameByteLoad(&TxDataLength,(uint8_t)(DataValue>>24),(uint8_t)(DataValue>>16));
					}
				}
			break;
		}
	}
	CRCTXValue = usCRC16((uint8_t *)UartTxData,TxDataLength);
	UartTxData[TxDataLength] = (uint8_t)((CRCTXValue>>8) & 0xFF);
	TxDataLength++;
	UartTxData[TxDataLength] = (uint8_t)(CRCTXValue & 0xFF);
	TxDataLength++;
	g_TXDataLength = TxDataLength;

}


void ModbusCMD22HandleV20(void)//������   ����5· 20181229  �̳�
{
	uint16_t ask_deal =0;
	uint16_t ask_count =0;

	uint16_t DataAddress = 0;
	uint16_t tempStrtAddr=0;
	uint16_t DataNum = 0;
	uint16_t TxDataLength = 0;
	uint16_t CRCTXValue = 0;
	uint32_t DataValue = 0;
	uint8_t  i=0;
	uint8_t tempRegNo=0;
	if(UartRxData[1] != 0x22||UartRxData[0] == 0)  //CYL
	{
		return;
	}

	ask_count=(UartRxData[2]<<8);//��ȡ���������
	ask_count+=(UartRxData[3]);

	UartCmnFlg.bit.TXFLG = OK;
	//DataAddress = (uint16_t)UartRxData[2]<<8 | UartRxData[3];
	//DataNum = (uint16_t)UartRxData[4]<<8 | UartRxData[5];
	//DataNum = 5000;
	//DataNum = 10;

	UartTxData[TxDataLength] = UartRxData[TxDataLength];//0xf8
	TxDataLength++;
	UartTxData[TxDataLength] = UartRxData[TxDataLength];//0x22
	TxDataLength++;
	//UartTxData[TxDataLength] = (uint8_t)(DataNum<<1);//�����ֽ���
	//TxDataLength++;

	if(ask_count<1000) //waveA
	{
		 for(ask_deal =0;ask_deal <25;ask_deal++)
		{
			// *pucFrameCur++ =  (Uint8)(WaveA[ask_count]>>8);  //ȡ��8λ
			//*pucFrameCur++ = (Uint8)(WaveA[ask_count] & 0x00FF); //ȡ��8λ

			DataValue = WaveA[ask_count];
			FrameByteLoad(&TxDataLength,(uint8_t)(DataValue>>8),(uint8_t)(DataValue));
			//if((i+1)<DataNum)
			//	FrameByteLoad(&TxDataLength,(uint8_t)(DataValue>>24),(uint8_t)(DataValue>>16));

			ask_count++;
		}
	}
	else if(ask_count<2000) //waveB
	{
		 for(ask_deal =0;ask_deal <25;ask_deal++)
		{
			// *pucFrameCur++ =  (Uint8)(WaveB[ask_count-1000]>>8);  //ȡ��8λ
			//*pucFrameCur++ = (Uint8)(WaveB[ask_count-1000] & 0x00FF); //ȡ��8λ

			 DataValue = WaveB[ask_count-1000];
			 FrameByteLoad(&TxDataLength,(uint8_t)(DataValue>>8),(uint8_t)(DataValue));
			ask_count++;
		}
	}
	else if(ask_count<3000) //waveC
	{
		 for(ask_deal =0;ask_deal <25;ask_deal++)
		{
			// *pucFrameCur++ =  (Uint8)(WaveC[ask_count-2000]>>8);  //ȡ��8λ
			//*pucFrameCur++ = (Uint8)(WaveC[ask_count-2000] & 0x00FF); //ȡ��8λ

			 DataValue = WaveC[ask_count-2000];
			 FrameByteLoad(&TxDataLength,(uint8_t)(DataValue>>8),(uint8_t)(DataValue));
			ask_count++;
		}
	}
	else if(ask_count<4000) //waveD
	{
		 for(ask_deal =0;ask_deal <25;ask_deal++)
		{
			// *pucFrameCur++ =  (Uint8)(WaveD[ask_count-3000]>>8);  //ȡ��8λ
			//*pucFrameCur++ = (Uint8)(WaveD[ask_count-3000] & 0x00FF); //ȡ��8λ

			DataValue = WaveD[ask_count-3000];
			//DataValue = (ask_count-3000)*164;//0-1000   0-IQ14��10.009��
			FrameByteLoad(&TxDataLength,(uint8_t)(DataValue>>8),(uint8_t)(DataValue));
			ask_count++;
		}
	}
	else if(ask_count<5000) //waveE
	{
		 for(ask_deal =0;ask_deal <25;ask_deal++)
		{
			// *pucFrameCur++ =  (Uint8)(WaveE[ask_count-4000]>>8);  //ȡ��8λ
			//*pucFrameCur++ = (Uint8)(WaveE[ask_count-4000] & 0x00FF); //ȡ��8λ

			//DataValue = WaveE[ask_count-4000];
			//DataValue = (ask_count-4000)*131;//0-1000   0-IQ14��8.0��
			 //DataValue = (ask_count-4000)*164;//0-1000   0-IQ14��10.009��
			DataValue = ((ask_count-4000)*33)&0x3FFF;//0-1000   0-IQ14��10.009��
			//DataValue = (ask_count-4000)*16;//0-1000   0-IQ14��10.009��
			FrameByteLoad(&TxDataLength,(uint8_t)(DataValue>>8),(uint8_t)(DataValue));
			ask_count++;
		}
	}

	//TxDataLength += 50;
	if(ask_count>4998) // �ϴ�����
	    CoreStatusFlag.bit.WAVE_DATA_F = NG;

	CRCTXValue = usCRC16((uint8_t *)UartTxData,TxDataLength);
	UartTxData[TxDataLength] = (uint8_t)((CRCTXValue>>8) & 0xFF);
	TxDataLength++;
	UartTxData[TxDataLength] = (uint8_t)(CRCTXValue & 0xFF);
	TxDataLength++;
	g_TXDataLength = TxDataLength;
}

/*************************************************
  Function: Modbus Receive Data Handle(CMD:0x06)
  Description:Period:1ms
  Input: No
  Return: No
  Others:NO
*************************************************/
void ModbusCMD06HandleV20(void)
{
	uint16_t DataAddress = 0;
	uint16_t tempStrtAddr=0;
	uint8_t  WriteResult = 0;
	uint16_t TxDataLength = 0;
	uint16_t CRCTXValue = 0;
	uint16_t DataValue = 0;
	uint32_t tempData=0;

	if(UartRxData[1] != 0x06)
	{
		return;
	}

	if(UartRxData[0] != 0)  //CYL
		UartCmnFlg.bit.TXFLG = OK;
	DataAddress = (uint16_t)UartRxData[2]<<8 | UartRxData[3];
	DataValue =	((uint16_t)(UartRxData[4]&0xFF)<<8)|((uint16_t)(UartRxData[5]&0xFF));

	UartTxData[TxDataLength] = UartRxData[TxDataLength];
	TxDataLength++;
	UartTxData[TxDataLength] = UartRxData[TxDataLength];
	TxDataLength++;
	UartTxData[TxDataLength] = UartRxData[TxDataLength];
	TxDataLength++;
	UartTxData[TxDataLength] = UartRxData[TxDataLength];
	TxDataLength++;
	UartTxData[TxDataLength] = UartRxData[TxDataLength];
	TxDataLength++;
	UartTxData[TxDataLength] = UartRxData[TxDataLength];
	TxDataLength++;

	switch((DataAddress>>8)&0xFF)
	{
		case 0x10 : //ֻ������
			UartTxData[1]=0x86;
			UartTxData[2]=FrameErrTable[3];
			TxDataLength=3;
			break;

		case 0x80://д������
			switch(DataAddress)
			{
			case 0x8000:
				ComCtrlFlg.all =(ComCtrlFlg.all&0xFFFF0000)+ DataValue;
				break;
			case 0x8001:
				ComCtrlFlg.all=(int32_t)((ComCtrlFlg.all&0x0000FFFF)+ ((uint32_t)DataValue<<16));
				break;
			case 0x8002:
				g_ComPulse=(g_ComPulse&0xFFFF0000)+ DataValue;
				break;
			case 0x8003:
				g_ComPulse=(int32_t)((g_ComPulse&0x0000FFFF)+ ((uint32_t)DataValue<<16));
				break;
			case 0x8004:
				break;
			case 0x8005:
				break;
			case 0x8006:
				g_ComSpeed=(g_ComSpeed&0xFFFF0000)+ DataValue;
				g_ComSpeed=g_ComSpeed>30000?30000:g_ComSpeed;
				g_ComSpeed=g_ComSpeed<-30000?-30000:g_ComSpeed;
				break;
			case 0x8007:
				g_ComSpeed=(int32_t)((g_ComSpeed&0x0000FFFF)+ ((uint32_t)DataValue<<16));
				g_ComSpeed=g_ComSpeed>30000?30000:g_ComSpeed;
				g_ComSpeed=g_ComSpeed<-30000?-30000:g_ComSpeed;
				break;
			case 0x8008:
				g_ComTorque=(int32_t)DataValue;
				g_ComTorque=g_ComTorque>3000?3000:g_ComTorque;
				g_ComTorque=g_ComTorque<-3000?-3000:g_ComTorque;
				break;
			case 0x8009:
				g_ComPidRef=(int32_t)DataValue;
				g_ComPidRef=g_ComPidRef>1000?1000:g_ComPidRef;
				g_ComPidRef=g_ComPidRef<-1000?-1000:g_ComPidRef;
				break;
			case 0x800A:
				g_ComAO1=(int32_t)DataValue;
				g_ComAO1=g_ComAO1>1000?1000:g_ComAO1;
				g_ComAO1=g_ComAO1<-1000?-1000:g_ComAO1;
				break;
			case 0x800B:
				g_ComAO2=(int32_t)DataValue;
				g_ComAO2=g_ComAO1>1000?1000:g_ComAO2;
				g_ComAO2=g_ComAO1<-1000?-1000:g_ComAO2;
				break;
			case 0x800C:
				g_ComDO=(int32_t)DataValue;
				break;
			case 0x800D:
				g_ComCmdModel=(int32_t)DataValue;
				break;
			default:
				UartTxData[1]=0x86;
				UartTxData[2]=FrameErrTable[1];
				TxDataLength=3;
				break;
			}
			break;

		case 0x81: //ֻ������
			UartTxData[1]=0x86;
			UartTxData[2]=FrameErrTable[3];
			TxDataLength=3;
			break;

		case 0x82://COMVirtualDI��������һ��DI�ڸ�ֵ
			switch(DataAddress)
			{
			case 0x8200:
				ForceDIOnEn.all=(uint32_t)DataValue;
				break;
			case 0x8201:
				ForceDIOffEn.all=(uint32_t)DataValue;
				break;
			default:
				UartTxData[1]=0x86;
				UartTxData[2]=FrameErrTable[1];
				TxDataLength=3;
				break;
			}
			break;

		case 0x83:
			switch(DataAddress)
			{
			case 0x8300:
				ForceDOOnEn.all=(uint32_t)DataValue;
				break;
			case 0x8301:
				ForceDOOffEn.all=(uint32_t)DataValue;
				break;
			default:
				UartTxData[1]=0x86;
				UartTxData[2]=FrameErrTable[1];
				TxDataLength=3;
				break;
			}
			break;
		case 0x84:
			UartTxData[1]=0x86;
			UartTxData[2]=FrameErrTable[3];
			TxDataLength=3;
			break;
		case 0x85:
			UartTxData[1]=0x86;
			UartTxData[2]=FrameErrTable[3];
			TxDataLength=3;
			break;
		default://F0-FA
			tempStrtAddr = (uint16_t) UartRxData[2] * 1000 + UartRxData[3] / 2;
			tempData = FuncCodeReadRAMValue(tempStrtAddr);
			if (UartRxData[3] % 2 != 0)
			{
				WriteResult = (uint8_t) FuncCodeChange(tempStrtAddr,
						(tempData & 0x0000FFFF) + ((uint32_t) DataValue << 16),
						&FuncCode_Handle, OK);
			}
			else
			{
				WriteResult = (uint8_t) FuncCodeChange(tempStrtAddr,
						(tempData & 0xFFFF0000) + DataValue, &FuncCode_Handle, OK);
			}
			switch (WriteResult) //�޸Ĳ��ɹ�
			{
			case 1: //ͣ���޸�
				UartTxData[1] = 0x86;
				UartTxData[2] = FrameErrTable[3];
				TxDataLength = 3;
				break;
			case 3: //ֻ��
				UartTxData[1] = 0x86;
				UartTxData[2] = FrameErrTable[3];
				TxDataLength = 3;
				break;
			case 4: //���������
				UartTxData[1] = 0x86;
				UartTxData[2] = FrameErrTable[1];
				TxDataLength = 3;
				break;
			case 5: //������ֵ����������
				UartTxData[1] = 0x86;
				UartTxData[2] = FrameErrTable[2];
				TxDataLength = 3;
				break;
			default:
				break;
			}
			break;
	}

	CRCTXValue = usCRC16((uint8_t *)UartTxData,TxDataLength);
	UartTxData[TxDataLength] = (uint8_t)((CRCTXValue>>8) & 0xFF);
	TxDataLength++;
	UartTxData[TxDataLength] = (uint8_t)(CRCTXValue & 0xFF);
	TxDataLength++;
	g_TXDataLength = TxDataLength;
}

/*************************************************
  Function: Modbus Receive Data Handle(CMD:0x06)
  Description:Period:1ms
  Input: No
  Return: No
  Others:NO
*************************************************/
void ModbusCMD06Handle(void)
{
	uint16_t DataAddress = 0;
	uint8_t  WriteResult = 0;
	uint16_t TxDataLength = 0;
	uint16_t CRCTXValue = 0;
	uint32_t DataValue = 0;

	if(UartRxData[1] != 0x06)
	{
		return;
	}
//	else if(RxDataLength != 0x0A)
//	{
//		return;
//	}


	if(UartRxData[0] != 0)  //CYL
		UartCmnFlg.bit.TXFLG = OK;
	DataAddress = (uint16_t)UartRxData[2]<<8 | UartRxData[3];
	DataValue = ((uint32_t)(UartRxData[4]&0xFF)<<24)|((uint32_t)(UartRxData[5]&0xFF)<<16)| \
				((uint32_t)(UartRxData[6]&0xFF)<<8)|((uint32_t)(UartRxData[7]&0xFF));




	UartTxData[TxDataLength] = UartRxData[TxDataLength];
	TxDataLength++;
	UartTxData[TxDataLength] = UartRxData[TxDataLength];
	TxDataLength++;
	UartTxData[TxDataLength] = UartRxData[TxDataLength];
	TxDataLength++;
	UartTxData[TxDataLength] = UartRxData[TxDataLength];
	TxDataLength++;

	switch((DataAddress>>8)&0xFF)
	{
		case 0x10 :
			WriteResult = WriteResultTable[3];
			UartTxData[TxDataLength] = 0;
			TxDataLength++;
			UartTxData[TxDataLength] = WriteResult;
			TxDataLength++;
			break;

		case 0x80://д������
			WriteResult = WriteResultTable[0];//CYL
			if( (DataAddress&0xFF) == 0 )	//0x8000
			{
				ComCtrlFlg.all = DataValue;
			}
			else if( (DataAddress&0xFF) == 1 )//0x8001
			{
				g_ComPulse = DataValue;
			}
			else if( (DataAddress&0xFF) == 2 )//0x8002
			{
				g_ComSpeed = DataValue;

				if(g_ComSpeed > 30000)//mxj
					g_ComSpeed = 30000;
				else if(g_ComSpeed < -30000)
					g_ComSpeed = -30000;
			}
			else if( (DataAddress&0xFF) == 3 )//0x8003
			{
				g_ComTorque = DataValue;

				if(g_ComTorque > 3000)//mxj
					g_ComTorque = 3000;
				else if(g_ComTorque < -3000)
					g_ComTorque = -3000;
			}
			else if( (DataAddress&0xFF) == 4 )//0x8004
			{
				g_ComPidRef = DataValue;

				if(g_ComPidRef > 1000)//mxj
					g_ComPidRef = 1000;
				else if(g_ComPidRef < -1000)
					g_ComPidRef = -1000;

				//PeriPid1.Com_Ref = g_ComPidRef * 16777216/1000;
			}
			else if( (DataAddress&0xFF) == 5 )
			{
				g_ComAO1 = DataValue;

				if(g_ComAO1 > 1000)//mxj
					g_ComAO1 = 1000;
				else if(g_ComAO1 < -1000)
					g_ComAO1 = -1000;
			}
			else if( (DataAddress&0xFF) == 6 )
			{
				g_ComAO2 = DataValue;

				if(g_ComAO2 > 1000)//mxj
					g_ComAO2 = 1000;
				else if(g_ComAO2 < -1000)
					g_ComAO2 = -1000;
			}
			else if( (DataAddress&0xFF) == 7 )
			{
				g_ComDO = DataValue;
			}
			else if( (DataAddress&0xFF) == 8)//0x8008
			{
				g_ComCmdModel = DataValue;
			}
			else
			{
				WriteResult = WriteResultTable[3];//3
			}
			UartTxData[TxDataLength] = 0;
			TxDataLength++;
			UartTxData[TxDataLength] = WriteResult;
			TxDataLength++;
			break;

		case 0x81:
			WriteResult = WriteResultTable[3];
			UartTxData[TxDataLength] = 0;
			TxDataLength++;
			UartTxData[TxDataLength] = WriteResult;
			TxDataLength++;
			break;

		case 0x82://COMVirtualDI��������һ��DI�ڸ�ֵ
			WriteResult = WriteResultTable[0];

			if( (DataAddress&0xFF) <= 8 )
				*(&COMVirtualDI.DI1 + (DataAddress&0xFF)) = DataValue;
			else if ( (DataAddress&0xFF)== 9 )//DIǿ��ʹ��
				ForceDIOnEn.all = DataValue;//ONʹ��״̬λ
			else if ( (DataAddress&0xFF)== 0xA )
				ForceDIOffEn.all = DataValue;//OFFʹ��״̬λ
			else
				WriteResult = FCODE_ERROR;//�Ƿ����ݵ�ַ

			//WriteResult = WriteResultTable[0];
			UartTxData[TxDataLength] = 0;
			TxDataLength++;
			UartTxData[TxDataLength] = WriteResult;
			TxDataLength++;
			break;

		case 0x83:
			WriteResult = WriteResultTable[0];

			if ( (DataAddress&0xFF)== 0 )//DOǿ��ʹ��
				ForceDOOnEn.all = DataValue;//ONʹ��״̬λ
			else if ( (DataAddress&0xFF)== 1 )//DOǿ��ʹ��
				ForceDOOffEn.all = DataValue;//Offʹ��״̬λ
			else
				WriteResult = FCODE_ERROR;//�Ƿ����ݵ�ַ

			UartTxData[TxDataLength] = 0;
			TxDataLength++;
			UartTxData[TxDataLength] = WriteResult;
			TxDataLength++;
			break;

		default://F0-FA
			if((UartRxData[3]>99)||(UartRxData[2]>0x0A))
			{
				WriteResult = FCODE_ERROR;//�Ƿ����ݵ�ַ
			}
			else
			{
				DataAddress = (uint16_t)UartRxData[2]*1000 + UartRxData[3];
				WriteResult = (uint8_t)FuncCodeChange(DataAddress,DataValue,&FuncCode_Handle,OK);
			}
			UartTxData[TxDataLength] = 0;
			TxDataLength++;
			UartTxData[TxDataLength] = WriteResult;
			TxDataLength++;
			if(  (UartRxData[2] == 0x9) && (UartRxData[3]==0)  )
			{
				g_timer_count = timer_count; //��ֵ
				g_timer_flag =0xff;
			}
			break;
	}

	CRCTXValue = usCRC16((uint8_t *)UartTxData,TxDataLength);
	UartTxData[TxDataLength] = (uint8_t)((CRCTXValue>>8) & 0xFF);
	TxDataLength++;
	UartTxData[TxDataLength] = (uint8_t)(CRCTXValue & 0xFF);
	TxDataLength++;
	g_TXDataLength = TxDataLength;

}

/*************************************************
  Function: Modbus Receive Data Handle(CMD:0x10)
  Description:Period:1ms
  Input: No
  Return: No
  Others:NO
*************************************************/
void ModbusCMD10HandleV20(void)
{
	uint16_t DataAddress = 0;
	uint16_t tempAddr=0;
	uint16_t tempStrtAddr=0;
	uint8_t  WriteResult = 0;
	uint16_t TxDataLength = 0;
	uint16_t CRCTXValue = 0;
	uint16_t DataValue = 0;
	uint32_t tempData=0;
	uint32_t tempData1=0;
	uint8_t tempRegNo=0;
	uint16_t tempTtlRegNo=0;
	uint16_t i=0;
	if(UartRxData[1] != 0x10)
	{
		return;
	}

	if(UartRxData[0] != 0)  //CYL
		UartCmnFlg.bit.TXFLG = OK;
	DataAddress = (uint16_t)UartRxData[2]<<8 | UartRxData[3];
	tempTtlRegNo = (uint16_t)UartRxData[4]<<8 | UartRxData[5];

	UartTxData[TxDataLength] = UartRxData[TxDataLength];
	TxDataLength++;
	UartTxData[TxDataLength] = UartRxData[TxDataLength];
	TxDataLength++;
	UartTxData[TxDataLength] = UartRxData[TxDataLength];
	TxDataLength++;
	UartTxData[TxDataLength] = UartRxData[TxDataLength];
	TxDataLength++;
	UartTxData[TxDataLength] = UartRxData[TxDataLength];
	TxDataLength++;
	UartTxData[TxDataLength] = UartRxData[TxDataLength];
	TxDataLength++;

	switch((DataAddress>>8)&0xFF)
	{
		case 0x10 : //ֻ������
			UartTxData[1]=0x90;
			UartTxData[2]=FrameErrTable[3];
			TxDataLength=3;
			break;

		case 0x80://д������
			switch(DataAddress)
			{
			case 0x8000:
				DataValue =	((uint16_t)(UartRxData[tempRegNo+7]&0xFF)<<8)|((uint16_t)(UartRxData[tempRegNo+8]&0xFF));
				ComCtrlFlg.all =(ComCtrlFlg.all&0xFFFF0000)+ DataValue;
				if((++tempRegNo)==tempTtlRegNo)
					break;
			case 0x8001:
				DataValue =	((uint16_t)(UartRxData[tempRegNo*2+7]&0xFF)<<8)|((uint16_t)(UartRxData[tempRegNo*2+8]&0xFF));
				ComCtrlFlg.all=(int32_t)((ComCtrlFlg.all&0x0000FFFF)+ ((uint32_t)DataValue<<16));
				if((++tempRegNo)==tempTtlRegNo)
					break;
			case 0x8002:
				DataValue =	((uint16_t)(UartRxData[tempRegNo*2+7]&0xFF)<<8)|((uint16_t)(UartRxData[tempRegNo*2+8]&0xFF));
				g_ComPulse=(g_ComPulse&0xFFFF0000)+ DataValue;
				if((++tempRegNo)==tempTtlRegNo)
					break;
			case 0x8003:
				DataValue =	((uint16_t)(UartRxData[tempRegNo*2+7]&0xFF)<<8)|((uint16_t)(UartRxData[tempRegNo*2+8]&0xFF));
				g_ComPulse=(int32_t)((g_ComPulse&0x0000FFFF)+ ((uint32_t)DataValue<<16));
				if((++tempRegNo)==tempTtlRegNo)
					break;
			case 0x8004:
				DataValue =	((uint16_t)(UartRxData[tempRegNo*2+7]&0xFF)<<8)|((uint16_t)(UartRxData[tempRegNo*2+8]&0xFF));
				if((++tempRegNo)==tempTtlRegNo)
					break;
			case 0x8005:
				DataValue =	((uint16_t)(UartRxData[tempRegNo*2+7]&0xFF)<<8)|((uint16_t)(UartRxData[tempRegNo*2+8]&0xFF));
				if((++tempRegNo)==tempTtlRegNo)
					break;
			case 0x8006:
				DataValue =	((uint16_t)(UartRxData[tempRegNo*2+7]&0xFF)<<8)|((uint16_t)(UartRxData[tempRegNo*2+8]&0xFF));
				g_ComSpeed=(g_ComSpeed&0xFFFF0000)+ DataValue;
				g_ComSpeed=g_ComSpeed>30000?30000:g_ComSpeed;
				g_ComSpeed=g_ComSpeed<-30000?-30000:g_ComSpeed;
				if((++tempRegNo)==tempTtlRegNo)
					break;
			case 0x8007:
				DataValue =	((uint16_t)(UartRxData[tempRegNo*2+7]&0xFF)<<8)|((uint16_t)(UartRxData[tempRegNo*2+8]&0xFF));
				g_ComSpeed=(int32_t)((g_ComSpeed&0x0000FFFF)+ ((uint32_t)DataValue<<16));
				g_ComSpeed=g_ComSpeed>30000?30000:g_ComSpeed;
				g_ComSpeed=g_ComSpeed<-30000?-30000:g_ComSpeed;
				if((++tempRegNo)==tempTtlRegNo)
					break;
			case 0x8008:
				DataValue =	((uint16_t)(UartRxData[tempRegNo*2+7]&0xFF)<<8)|((uint16_t)(UartRxData[tempRegNo*2+8]&0xFF));
				g_ComTorque=(int32_t)DataValue;
				g_ComTorque=g_ComTorque>3000?3000:g_ComTorque;
				g_ComTorque=g_ComTorque<-3000?-3000:g_ComTorque;
				if((++tempRegNo)==tempTtlRegNo)
					break;
			case 0x8009:
				DataValue =	((uint16_t)(UartRxData[tempRegNo*2+7]&0xFF)<<8)|((uint16_t)(UartRxData[tempRegNo*2+8]&0xFF));
				g_ComPidRef=(int32_t)DataValue;
				g_ComPidRef=g_ComPidRef>1000?1000:g_ComPidRef;
				g_ComPidRef=g_ComPidRef<-1000?-1000:g_ComPidRef;
				if((++tempRegNo)==tempTtlRegNo)
					break;
			case 0x800A:
				DataValue =	((uint16_t)(UartRxData[tempRegNo*2+7]&0xFF)<<8)|((uint16_t)(UartRxData[tempRegNo*2+8]&0xFF));
				g_ComAO1=(int32_t)DataValue;
				g_ComAO1=g_ComAO1>1000?1000:g_ComAO1;
				g_ComAO1=g_ComAO1<-1000?-1000:g_ComAO1;
				if((++tempRegNo)==tempTtlRegNo)
					break;
			case 0x800B:
				DataValue =	((uint16_t)(UartRxData[tempRegNo*2+7]&0xFF)<<8)|((uint16_t)(UartRxData[tempRegNo*2+8]&0xFF));
				g_ComAO2=(int32_t)DataValue;
				g_ComAO2=g_ComAO1>1000?1000:g_ComAO2;
				g_ComAO2=g_ComAO1<-1000?-1000:g_ComAO2;
				if((++tempRegNo)==tempTtlRegNo)
					break;
			case 0x800C:
				DataValue =	((uint16_t)(UartRxData[tempRegNo*2+7]&0xFF)<<8)|((uint16_t)(UartRxData[tempRegNo*2+8]&0xFF));
				g_ComDO=(int32_t)DataValue;
				if((++tempRegNo)==tempTtlRegNo)
					break;
			case 0x800D:
				DataValue =	((uint16_t)(UartRxData[tempRegNo*2+7]&0xFF)<<8)|((uint16_t)(UartRxData[tempRegNo*2+8]&0xFF));
				g_ComCmdModel=(int32_t)DataValue;
				if((++tempRegNo)==tempTtlRegNo)
					break;
			default:
				UartTxData[1]=0x90;
				UartTxData[2]=FrameErrTable[1];
				TxDataLength=3;
				break;
			}
			break;

		case 0x81: //ֻ������
			UartTxData[1]=0x90;
			UartTxData[2]=FrameErrTable[3];
			TxDataLength=3;
			break;

		case 0x82://COMVirtualDI��������һ��DI�ڸ�ֵ
			switch(DataAddress)
			{
			case 0x8200:
				DataValue =	((uint16_t)(UartRxData[tempRegNo+7]&0xFF)<<8)|((uint16_t)(UartRxData[tempRegNo+8]&0xFF));
				ForceDIOnEn.all=(uint32_t)DataValue;
				if((++tempRegNo)==tempTtlRegNo)
					break;
			case 0x8201:
				DataValue =	((uint16_t)(UartRxData[tempRegNo*2+7]&0xFF)<<8)|((uint16_t)(UartRxData[tempRegNo*2+8]&0xFF));
				ForceDIOffEn.all=(uint32_t)DataValue;
				if((++tempRegNo)==tempTtlRegNo)
					break;
			default:
				UartTxData[1]=0x90;
				UartTxData[2]=FrameErrTable[1];
				TxDataLength=3;
				break;
			}
			break;

		case 0x83:
			switch(DataAddress)
			{
			case 0x8300:
				DataValue =	((uint16_t)(UartRxData[tempRegNo*2+7]&0xFF)<<8)|((uint16_t)(UartRxData[tempRegNo*2+8]&0xFF));
				ForceDOOnEn.all=(uint32_t)DataValue;
				if((++tempRegNo)==tempTtlRegNo)
					break;
			case 0x8301:
				DataValue =	((uint16_t)(UartRxData[tempRegNo*2+7]&0xFF)<<8)|((uint16_t)(UartRxData[tempRegNo*2+8]&0xFF));
				ForceDOOffEn.all=(uint32_t)DataValue;
				if((++tempRegNo)==tempTtlRegNo)
					break;
			default:
				UartTxData[1]=0x90;
				UartTxData[2]=FrameErrTable[1];
				TxDataLength=3;
				break;
			}
			break;
		case 0x84:
			UartTxData[1]=0x90;
			UartTxData[2]=FrameErrTable[3];
			TxDataLength=3;
			break;
		case 0x85:
			UartTxData[1]=0x90;
			UartTxData[2]=FrameErrTable[3];
			TxDataLength=3;
			break;
		default: //F0-FA
			//=========================================����ָ���==========================================
			if((DataAddress == 0x03F0)&&(tempTtlRegNo == 0x0003)&&(UartRxData[6]==0x06))
			{
				if((UartRxData[7]==0xFF)&&(UartRxData[8]==0xFF))//ON
				{
					DataValue =	(((uint16_t)(UartRxData[11]&0xFF)<<8)|((uint16_t)(UartRxData[12]&0xFF)))/10  ;

					g_ComPulse = ((uint32_t)DataValue<<16)+(((uint16_t)(UartRxData[9]&0xFF)<<8)|((uint16_t)(UartRxData[10]&0xFF)))/10;

					ComCtrlFlg.all|=0x01;
				}
				else if((UartRxData[7]==0x00)&&(UartRxData[8]==0x00))//OFF
				{
					ComCtrlFlg.all&=0xFFFE;
				}
				else
				{
					UartTxData[1] = 0x90;
					UartTxData[2] = FrameErrTable[2];
					TxDataLength = 3;
				}
				break;
			}
			//=========================================����ָ���==========================================

			tempStrtAddr = (uint16_t) UartRxData[2] * 1000 + UartRxData[3] / 2;
			tempData = FuncCodeReadRAMValue(tempStrtAddr + i / 2);
			if (UartRxData[3] % 2 != 0)
			{
				tempData1 = ((uint16_t) (UartRxData[7]) << 8)
						| ((uint16_t) (UartRxData[8]));
				WriteResult = (uint8_t) FuncCodeChange(tempStrtAddr,
						(tempData & 0x0000FFFF) + ((uint32_t) tempData1 << 16),
						&FuncCode_Handle, OK);
				i++;
				tempStrtAddr++;
			}
			for (; i < tempTtlRegNo;)
			{
				switch (WriteResult) //�޸Ĳ��ɹ�
				{
				case 1: //ͣ���޸�
					UartTxData[1] = 0x90;
					UartTxData[2] = FrameErrTable[3];
					TxDataLength = 3;
					i = tempTtlRegNo;
					continue;
					break;
				case 3: //ֻ��
					UartTxData[1] = 0x90;
					UartTxData[2] = FrameErrTable[3];
					TxDataLength = 3;
					i = tempTtlRegNo;
					continue;
					break;
				case 4: //���������
					UartTxData[1] = 0x90;
					UartTxData[2] = FrameErrTable[1];
					TxDataLength = 3;
					i = tempTtlRegNo;
					continue;
					break;
				case 5: //������ֵ����������
					UartTxData[1] = 0x90;
					UartTxData[2] = FrameErrTable[2];
					TxDataLength = 3;
					i = tempTtlRegNo;
					continue;
					break;
				default:
					break;
				}
				tempAddr =tempStrtAddr+ (i / 2);
				tempData = FuncCodeReadRAMValue(tempAddr);
				tempData1 = ((uint16_t) (UartRxData[i * 2 + 7]) << 8)
						| ((uint16_t) (UartRxData[i * 2 + 8]));
				if (i + 1 < tempTtlRegNo)
					tempData1 += ((uint16_t) (UartRxData[i * 2 + 9]) << 24)
							| ((uint16_t) (UartRxData[i * 2 + 10]) << 16);
				else
					tempData1 = (tempData & 0xFFFF0000) + tempData1;
				WriteResult = (uint8_t) FuncCodeChange(tempAddr, tempData1,
						&FuncCode_Handle, OK);
				i = (WriteResult != 0 && WriteResult != 2) ? 0 : i + 2; //��д��ʧ�ܣ���ǿ����ѭ��һ��
			}
			break;
	}

	CRCTXValue = usCRC16((uint8_t *)UartTxData,TxDataLength);
	UartTxData[TxDataLength] = (uint8_t)((CRCTXValue>>8) & 0xFF);
	TxDataLength++;
	UartTxData[TxDataLength] = (uint8_t)(CRCTXValue & 0xFF);
	TxDataLength++;
	g_TXDataLength = TxDataLength;
}

/*************************************************
  Function: �Ƿ������봦��
  Description:Period:1ms
  Input: No
  Return: No
  Others:NO
*************************************************/
void ModbusCMDIlgFuncV20()
{
	uint16_t TxDataLength = 0;
	uint16_t CRCTXValue = 0;

	//if(UartRxData[1] == 0x03||UartRxData[1] == 0x06||UartRxData[1] == 0x10||UartRxData[0]==0)
	if(UartRxData[1] == 0x03||UartRxData[1] == 0x06||UartRxData[1] == 0x10||UartRxData[1] == 0x22||UartRxData[0]==0)
	{
		return;
	}
	UartCmnFlg.bit.TXFLG = OK;
	UartTxData[TxDataLength] = UartRxData[TxDataLength];
	TxDataLength++;
	UartTxData[TxDataLength] = UartRxData[TxDataLength]+0x80;
	TxDataLength++;
	UartTxData[TxDataLength] = FrameErrTable[0];
	TxDataLength++;

	CRCTXValue = usCRC16((uint8_t *)UartTxData,TxDataLength);
	UartTxData[TxDataLength] = (uint8_t)((CRCTXValue>>8) & 0xFF);
	TxDataLength++;
	UartTxData[TxDataLength] = (uint8_t)(CRCTXValue & 0xFF);
	TxDataLength++;
	g_TXDataLength = TxDataLength;
}


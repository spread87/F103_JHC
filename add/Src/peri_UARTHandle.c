/*
 *peri_UARTHandle.c
 *
 *Created on: 2015-3-6
 *Author: JH-LvSL
 */

#include <main.h>			//Declarations from DAVE3 Code Generation (includes SFR declaration)
#include <peri_GlobalVariablesExtern.h>
#include <peri_UARTHandle.h>
//static uint8_t counterA=0,counterB=0;
uint8_t g_UARTFlg = 0;
uint8_t g_UARTData = 0;
uint16_t g_RXDataLength = 0;
uint16_t g_TXDataLength = 0;
UARTCOM UartCmnFlg;
uint8_t UartRxData[32];//200->32
uint8_t UartTxData[200];
uint8_t g_FrameIntervalTime = 0;

uint8_t FrmInterval=0;
uint16_t g_RXDL;

void ModbusRXJudge(void);

uint32_t UART001_ClearReadbuf(
  const UART001_HandleType* Handle,
  uint32_t Count
)//������ж�
{
  USIC_CH_TypeDef* UartRegs = Handle->UartRegs;
  uint32_t temp;
  if(Handle->RxFifoEn)
  {
	  while(! USIC_ubIsRxFIFOempty(UartRegs) && Count)
	  {
		temp = (uint16_t)UartRegs->OUTR;
		Count--;
	  }
  }
  return 0; //
}

const uint32_t BaudRateTable[]=
{
		2400,//0
		4800,
		9600,//2
		19200,
		38400,//4
		57600,
		115200,//6
		230400,
		460800,//8
		921600,
		1843200//10
};

const uint8_t ParityTable[]=
{
		 /** No parity selected */
		 UART_PARITY_NONE,
		 /** Odd parity selected */
		 UART_PARITY_ODD,
		 /** Even parity selected */
		 UART_PARITY_EVEN
};

const uint8_t StopBitsTable[]=
{
		  /** One stop bit selected */
		  UART_ONE_STOPBIT,
		  /** Two stop bit selected */
		  UART_TWO_STOPBIT
};

/*************************************************
  Function: UART Init
  Description:MODBUSͨ�������ʼ��
  Input: No
  Return: No
  Others:20161216 CYL�ӣ�20170215 CYL��ΪUSIC0_CH0
*************************************************/
void RS485Init()//
{
	SCU_CLK->CGATCLR0|=0x00000800;  //���USIC0��ʱ���ſ�
	SCU_RESET->PRCLR0|=0x00000800;  //��λ�ͷ�USIC0
	USIC0_CH0->CCR&=0xFFFFFFF0;     //������ģʽ��Ϊ0
	USIC0_CH0->KSCFG|=0x00000003;   //ʹ��ģ�鹤��
	USIC0_CH0->FDR|=0x000081A1;     //ѡ�������Ƶģʽ��STEP=0X1A1
	USIC0_CH0->BRG|=0x01FC2400;     //PDIV=508,DCTQ=9,PCTQ=0  ������=9600BPS
	USIC0_CH0->SCTR|=0x07070102;    //������ƽ��Ϊ1������ģʽ��Ϊ0,֡�����ֳ�=7
	USIC0_CH0->TCSR|=0x00000500;    //TBUF�������ã�TBUF���ݵ���ģʽ
	USIC0_CH0->PCR|=0x00000601;     //SMD=1,SP=6
	USIC0_CH0->TBCTR|=0x05000000;   //����FIFO SIZE=32
	USIC0_CH0->RBCTR|=0X05000000;   //����FIFO SIZE=32
	USIC0_CH0->DX0CR|=0X00000001;   //DSEL=1 ��������DX0B
	USIC0_CH0->DX0CR|=0X00000260;   //���������˲�������ͬ�����ܣ�EFEN=1��DSEN=1��SFSEL=0(�����˲�����Ƶ��ΪFPB) 1(����Ƶ��ΪFFD ���˳����������)
	USIC0_CH0->CCR|=0x00000002;     //ѡ��UARTЭ��
}

/*************************************************
  Function: UART Init
  Description:���б����������ʼ��
  Input: No
  Return: No
  Others:20170215 CYL��ΪUSIC1_CH0
*************************************************/
void UartEncoderInit()
{
	SCU_CLK->CGATCLR1|=0x00000080;  //���USIC1��ʱ���ſ�
	SCU_RESET->PRCLR1|=0x00000080;  //��λ�ͷ�USIC1
	USIC1_CH0->CCR&=0xFFFFFFF0;     //������ģʽ��Ϊ0
	USIC1_CH0->KSCFG|=0x00000003;   //ʹ��ģ�鹤��
	USIC1_CH0->FDR|=0x00008280;     //ѡ�������Ƶģʽ��STEP=0X280
	USIC1_CH0->BRG|=0x00022400;     //PDIV=2,DCTQ=9,PCTQ=0  ������=2.5MBPS
	USIC1_CH0->SCTR|=0x07070102;    //������ƽ��Ϊ1������ģʽ��Ϊ0,֡�����ֳ�=7
	USIC1_CH0->TCSR|=0x00000500;    //TBUF�������ã�TBUF���ݵ���ģʽ
	USIC1_CH0->PCR|=0x00000601;     //SMD=1,SP=6
	USIC1_CH0->TBCTR|=0x00000100;   //����FIFO LIMIT=1,STBINP=0,SIZE=0,LOF=0,STBIEN=1
	USIC1_CH0->RBCTR|=0X50000A00;   //����FIFO LIMIT=10��SRBINP=0,SIZE=0,LOF=1,SRBIEN=1
	USIC1_CH0->DX0CR|=0X00000002;   //DSEL=1 ��������DX0C
	USIC1_CH0->DX0CR|=0X00000260;   //���������˲�������ͬ�����ܣ�EFEN=1��DSEN=1��SFSEL=0(�����˲�����Ƶ��ΪFPB) 1(����Ƶ��ΪFFD ���˳����������)
	USIC1_CH0->CCR|=0x00001000;     //TSIEN=1 ������λ�ж�ʹ��
	USIC1_CH0->INPR|=0X00000004;    //SR4��Ϊ������λ�жϽڵ�,TSINP=4
	USIC1_CH0->TBCTR|=0x04000000;   //SIZE=4
	USIC1_CH0->RBCTR|=0X04000000;   //SIZE=4
	USIC1_CH0->CCR|=0x00000002;     //ѡ��UARTЭ��
	//InitSEncPara();                 //��ʼ�����б�������ص�ȫ�ֱ���
}
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
//	uint32_t BaudRateBRG = 0;  //CYL20170215
	UART_ParityType Parity;
	UART_StopBitType StopBits;
	uint32_t FcodeValue = 0;

  //uint8_t FrmIntl[]={17,8,5,3,2,2,2}; //֡�����3.5T,��9600,1000/9600*11*3.5=4.01ms
	uint8_t FrmIntl[]={17,8,5,3,2,1,1,1,0,0,0}; //֡�����3.5T,��9600,1000/9600*11*3.5=4.01ms  20190107
	RS485Init(); //�����ʼ��

	FcodeValue = FuncCode_Handle.Ram.F60.F6020;//������
	//==mxj==160825===========================================
	//if(FcodeValue>6)
	if(FcodeValue>10)
		FcodeValue = 2;//9600
	//==mxj==160825===========================================
	BaudRate = BaudRateTable[FcodeValue];
	FrmInterval=FrmIntl[FcodeValue];
	//BaudRateBRG = BaudRateBRGTable[FcodeValue];

	FcodeValue = FuncCode_Handle.Ram.F60.F6021;//У��λ
	//==mxj==160825===========================================
	if(FcodeValue>2)//20190105
		FcodeValue = 0;//��У��
	//==mxj==160825===========================================
	Parity = ParityTable[FcodeValue];

	FcodeValue = FuncCode_Handle.Ram.F60.F6022;//ֹͣλ
	//==mxj==160825===========================================
	if(FcodeValue>1)
		FcodeValue = 0;//1λֹͣλ
	//==mxj==160825===========================================
	StopBits = StopBitsTable[FcodeValue];

	UART001_Configure(&UART001_Handle0,BaudRate,Parity,StopBits);
	//USIC0_CH0->BRG| = BaudRateBRG;     //PDIV=2,DCTQ=9,PCTQ=0  ������=2.5MBPS

	UartCmnFlg.bit.Inited_F=1; //��ʼ�����
//	UART001_Handle1.UartRegs->TBCTR |= 0x10000000;

}

/*************************************************
  Function: UART receive Fifo Interrupt
  Description: Period:1ms
  Input: No
  Return: No
  Others:NO
*************************************************/
void UARTRXFIFO_ISR(void)//uart receive isr  485�����жϣ�Ŀǰ�յ�һ�����ݾͲ���һ���жϣ���һ������
{

}

/*************************************************
  Function: UART RX End Judge
  Description: Period:1ms
  Input: No
  Return: No
  Others:NO
*************************************************/
void ModbusRXJudge(void)
{
	static uint8_t FIFOLthOld=0; //������FIFO�������ֽ���
	uint8_t FIFOLth=0;           //������FIFO�������ֽ���

	if(UartCmnFlg.bit.Inited_F==0)
		return;

	g_FrameIntervalTime+=g_FrameIntervalTime<200?1:0;
	FIFOLth=(USIC0_CH0->TRBSR&0XFF0000)>>16;

	g_RXDL = FIFOLth;

	if(FIFOLth!=0&&FIFOLthOld==0) //֡��ʼ
	{
		if(g_FrameIntervalTime > FrmInterval) //��һ֡�Ѿ���������֡��ʼ
		{
			g_RXDataLength = 0;
			UartCmnFlg.bit.FrmStrt_F=1; //֡��ʼ��־λ��λ
			UartCmnFlg.bit.FrmFish_F=0;
			g_FrameIntervalTime = 0;
		}
	}
	else if(FIFOLth>FIFOLthOld) //�������ݹ���
	{
		 g_FrameIntervalTime = 0;
	}
	else if(FIFOLth==FIFOLthOld && FIFOLth!=0) //֡�������
	{
/*		if(FIFOLth>10) //�����涨�ֽ���,03��06�����8\10���ֽ�   CYL20170405Э������V2.0
		{
			UART001_ClearReadbuf(&UART001_Handle0,32);
			UartCmnFlg.bit.FrmStrt_F=0;
		}
		else*/
		{
			if((g_FrameIntervalTime > FrmInterval)) //֡����
			{
				if(UartCmnFlg.bit.FrmStrt_F==1)//(UartCmnFlg.bit.RXFLGOK == NG) && (UartCmnFlg.bit.RXFLG == OK))
				{
					UART001_ReadDataBytes(&UART001_Handle0,UartRxData,FIFOLth);
					g_RXDataLength=FIFOLth;
					g_RXDL = 0;
					UartCmnFlg.bit.FrmFish_F=1; //֡������ɣ����Կ�ʼ����
				}
				else
				{
					UART001_ClearReadbuf(&UART001_Handle0,32);//????????????????????????????????????????????????????????????????????????????????????????????????
				}
				UartCmnFlg.bit.FrmStrt_F=0;
			}
		}
	}
	else //����֡����ȡ�����γ���Ϊ0��ǰһ�β�Ϊ0
	{

	}
	FIFOLthOld=FIFOLth;

	if((USIC0_CH0->PSR_ASCMode&0x78)!=0)//ͨѶ������
	{
		USIC0_CH0->PSCR|=0x78;
	}
	if((USIC0_CH0->TRBSR&0x202)!=0)//FIFO������
	{
		USIC0_CH0->TRBSCR|=0x202;
	}
}

/*************************************************
  Function: UART RX End Judge
  Description: Period:1ms
  Input: No
  Return: No
  Others:NO
*************************************************/
/*void UARTRXEndJudge(void)
{
	g_FrameIntervalTime++;

	if((g_FrameIntervalTime > 20) && (UartCmnFlg.bit.RXFLGOK == NG) && (UartCmnFlg.bit.RXFLG == OK))
	{
		UartCmnFlg.bit.RXFLG = NG;
		UartCmnFlg.bit.RXFLGOK = OK;//��ʼ�������ݣ�����20msû�������ݹ�����ʾ�������
		UartCmnFlg.bit.RXOKFSTFLG = NG;
	}

	if(  ( (  (UART001_Handle0.UartRegs->TRBSR & 0x01) == 0x01) ||  ( (UART001_Handle0.UartRegs->TRBSR & 0x7f0000) == 0x200000)) && (g_FrameIntervalTime > 500)   )	//Clear Communication Error
	{
		UART001_ClearReadbuf(&UART001_Handle0,32);
		//UART001_Handle0.UartRegs->PSR_ASCMode | 0x10;
	}

//	if(UART001_Handle0.UartRegs->PSR_ASCMode | 0x10)//����
//		UART001_Handle0.UartRegs->PSR_ASCMode &= 0xFFFFFFEF;
//	if(UART001_Handle0.UartRegs->PSR_ASCMode | 0x20)//ֹͣλ����
//		UART001_Handle0.UartRegs->PSR_ASCMode &= 0xFFFFFFDF;

}*/
/*************************************************
  Function: UART TX Handle
  Description: Period:1ms
  Input: No
  Return: No
  Others:NO
*************************************************/
void UARTTXDataHandle(uint8_t Data)//�����жϣ�����û��
{
	UART001_Handle0.UartRegs->PSCR     |= 0x1000;   //  clear transmit buffer indication flag
	UART001_Handle0.UartRegs->TBUF[0]    = Data;   //  load transmit buffer register

    while(!(UART001_Handle0.UartRegs->PSR_ASCMode & 0x1000));   //  wait until tx buffer indication flag is set
}
/*************************************************
  Function: UART RX/TX Handle
  Description: Period:1ms
  Input: No
  Return: No
  Others:NO
*************************************************/
void UART_Test_Handle(void)
{
	//UARTRXEndJudge();
	ModbusRXJudge();

	if(g_UARTFlg)
	{
		g_UARTFlg = 0;
	}
}

/*�����������ж�*/
void UART1_RE_ISR(void)
{
}


/*�����ͣƣɣƣ������ݳ����ٽ�ֵ���������������ж�*/
void UART1_TX_ISR(void)
{
	if(UART001_GetFlagStatus(&UART001_Handle1,UART001_FIFO_STD_TRANSMIT_BUF_FLAG) == UART001_SET)
	{
		/*	    uint8_t i;
				if (UART001_Handle1.UartRegs->PSR_ASCMode & 0x1000)
				{

					for(i=0;i<5;i++)
					{;}
					IO004_SetOutputValue(IO004_Handle26,0);//���ý���״̬1
				}*/
		UART001_ClearFlag(&UART001_Handle1,UART001_FIFO_STD_TRANSMIT_BUF_FLAG);

	}
}
/*������λ�жϣ�������ͽ������ɽ���״̬ */
void UART1_TX1_ISR(void)//������λ�ж�  88���ж�
{
    uint8_t i;
    SeriEncoder.ByteBeSent--; //ÿ����һ���ֽڻ��һ���ж�
	if (((UART001_Handle1.UartRegs->PSR_ASCMode & 0x1000)!=0)&&(SeriEncoder.ByteBeSent==0))
	{
		for(i=0;i<5;i++)//10->5
			;
		IO004_SetOutputValue(IO004_Handle28,0);//���ý���״̬1
	}
	UART001_Handle1.UartRegs->PSCR |= 0x1000;
}



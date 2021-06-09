/**********************************************************
Copyright (c) 2014 JIANGSU JOINHOPE ELECTRIC CO.,LTD
FileName: peri_E2promHandle.c
Author: LvSL
Version :
Date: 2014.02.17
Description: E2prom read or write handle,include i2c communication using IO simulation
History:
    1. Date: 2014.02.17
       Author: LvSL
       Modification:  1st Version
    2...
***********************************************************/
//wo plan to use p2.8 as clk;use p2.9 as data in/out
//Now we use p1.0 and p1.1 as clk line and data line Correspondingly
//And in future ,wo will use p2.8 and p2.9.
#include <main.h>
#include <peri_FuncCodeHandle.h>
#include <peri_GlobalVariablesExtern.h>

//clk line macro define
#define 	SETCLK			IO004_SetOutputValue(IO004_Handle4,1)
#define 	CLRCLK			IO004_SetOutputValue(IO004_Handle4,0)

//data line macro define
#define	SETDATAINMODE	IO004_DisableOutputDriver(&IO004_Handle5,IO004_PULL_UP_DEVICE)
#define	READDATA		MX_GPIO_ReadPin(IO004_Handle5)
#define	SETDATAOUTMODE	IO004_EnableOutputDriver(&IO004_Handle5,IO004_PUSHPULL)
#define	SETDATA			IO004_SetOutputValue(IO004_Handle5,1)
#define	CLRDATA			IO004_SetOutputValue(IO004_Handle5,0)

//#define	Ack_Count  100

#define	Ack_Count  2000

extern uint8_t  CGParameterLimitCheck(uint32_t  );

uint8_t g_I2cAck=0;

uint8_t g_I2cAck_cnt=0;
/*************************************************
  Function: delay time
  Description: use as delay time
  Input: counts(Unit:us)
  Return: No
  Others: No
*************************************************/
void Delay(uint16_t counts)//12us@1
{
	uint16_t i=0,j=0;

	for(j=0;j<counts;j++)
	for(i=0;i<120;i++)//120
	{
		asm("NOP");
	}
}

void Delayus(uint16_t counts)//1us@1
{
	uint16_t i=0,j=0;

	for(j=0;j<counts;j++)
	for(i=0;i<10;i++)//10
	{
		asm("NOP");
	}
}

uint8_t Delay1000(void)
{
	uint16_t i=0,j=0;

	for(j=0;j<1000;j++)
	for(i=0;i<120;i++)
	{
		asm("NOP");
	}
	return TRUE;
}



/*************************************************
  Function: start i2c communication
  Description:
  Input: No
  Return: No
  Others: No
*************************************************/
void I2cStart(void)
{
//	SETDATAOUTMODE;
	SETDATA;
	Delay(1);
	SETCLK;
	Delay(1);//5->1 CYL timer 100K->400K
	CLRDATA;
	Delay(1);//5->1CYL
	CLRCLK;
	Delay(1);//2->1CYL
}
/*************************************************
  Function: STOP i2c communication
  Description:
  Input: No
  Return: No
  Others: No
*************************************************/
void I2cStop(void)
{
//	SETDATAOUTMODE;
	CLRDATA;
	Delay(1);
	SETCLK;
	Delay(1);//5->1CYL
	SETDATA;
	Delay(1);//4->1CYL
}
/*************************************************
  Function:I2c communication send byte
  Description:
  Input: No
  Return: No
  Others: No
*************************************************/
void I2cwriteByte(uint8_t Byte)
{
	uint8_t i=0;

	/*set data line to output mode*/
//	SETDATAOUTMODE;

	/*8 bit data*/
	for(i=0;i<8;i++)
	{
		/*judge the bit that need to be sended*/
		if((Byte<<i)&0x80)
		SETDATA;
		else
		CLRDATA;

		Delay(1);//1
		/*set clk line to 1 in order to notice E2prom to receive the bit of data*/
		SETCLK;
		/*ensure high-level of clk >4us */
		Delay(1);//5->1CYL
		CLRCLK;
		Delay(1);//1
	}
}
/*************************************************
  Function:I2c communication receive byte
  Description:
  Input: No
  Return: No
  Others: No
*************************************************/
uint8_t I2cReadByte(void)
{
	uint8_t data = 0;
	uint8_t i = 0;

	//set data line to input mode
	SETDATAINMODE;
	Delay(1);//CYL
	for(i=0;i<8;i++)
	{
		//Delay(1);CYL
		/*set clk to low(0),and be ready for receiving data bit*/
		CLRCLK;
		Delay(2);//5->2 �͵�ƽʱ�䳤CYL
		/*set clk to high(1),in order to enable data line sigal*/
		SETCLK;
		Delay(1);//2->1 �ߵ�ƽʱ���CYL
		data = data << 1;
		/*read data line bit,and */
		if(READDATA)
		data += 1;
		//Delay(2);CYL
	}

	CLRCLK;
	Delay(1);//2->1CYL
	/*set data line to output mode*/
	SETDATAOUTMODE;

	return data;
}
/*************************************************
  Function:I2c communication receive byte
  Description:
  Input: No
  Return: No
  Others: No
*************************************************/
#if 1
uint8_t I2cReadAck(void)
{
	uint8_t Ack;
	uint16_t  time_counter;
	SETDATAINMODE;

	//set data line to input mode
	//Delay(1);
	Delay(2);//20200107  1->2
	SETCLK;
	Delay(2);//5->1
//	IO004_SetOutputValue(IO004_Handle7,0);
	//for(time_counter=0;time_counter<=20;time_counter++)//����������ѭ����1~2US���ܷ��ص�
	IO004_SetOutputValue(IO004_Handle27,1);//mxj 20170120  disable pwm
	for(time_counter=0;time_counter<=Ack_Count;time_counter++)//����������ѭ����1~2US���ܷ��ص�
	{
	  Ack = READDATA;
	  if(Ack == 0)
	  break;
	  else
      Delay(3);//5->1
	}
	IO004_SetOutputValue(IO004_Handle27,0);//mxj 20170120  disable pwm
	//if(time_counter >=20)
	if(time_counter >=Ack_Count)
	{
		AlarmStatusFlag.bit.Driver.EE2 = OK;
		FuncCode_Handle.Ram.FA0.FA035 = g_I2cAck_cnt;
	}

	if(FuncCode_Handle.Ram.FA0.FA036 < time_counter)
	{
		FuncCode_Handle.Ram.FA0.FA036 = time_counter;
	}

//	IO004_SetOutputValue(IO004_Handle7,1);
	CLRCLK;
	Delay(1);//2->1
	/*set data line to output mode*/
	//������0���˳�
	SETDATAOUTMODE;
	//Delay(2);//20200107
	return 0;
}
#else
uint8_t I2cReadAck(void)//��ѭ��������
{
	uint8_t Ack;
	//set data line to input mode
	SETDATAINMODE;
	Delay(1);
	SETCLK;
	Delay(5);
	Ack = READDATA;
	CLRCLK;
	Delay(2);
	/*set data line to output mode*/
	SETDATAOUTMODE;
	//������0���˳�
	return Ack;
}

#endif



/*************************************************
  Function:I2c communication acknowledge bit
  Description:
  Input: No
  Return: No
  Others: No
*************************************************/
void I2cWriteAck(void)
{
	Delay(2);//2->1
	/*after send data,release data line to be ready for receive ACK bit*/
	CLRDATA;

	Delay(2);//2->1
	SETCLK;
	Delay(3);//3->1

	CLRCLK;
	Delay(2);//2->1
}
/*************************************************
  Function:I2c communication acknowledge bit
  Description:
  Input: No
  Return: No
  Others: No
*************************************************/
void I2cNoAck(void)
{
	SETDATA;
	Delay(2);//2->1
	SETCLK;
	Delay(2);//2->1
	CLRCLK;
	Delay(2);
}
/*************************************************
  Function:EEPROM write byte
  Description:
  Input: No
  Return: No
  Others: No
*************************************************/
void EepromWriteByte(uint8_t AddH,uint8_t AddL,uint8_t data)
{
	uint8_t CtrByte=0xA0;//write mode
//	AddH = AddH <<1;
//	CtrByte = CtrByte | AddH;

	I2cStart();
	I2cwriteByte(CtrByte);
	while(I2cReadAck());
	I2cwriteByte(AddH);
	while(I2cReadAck());
	I2cwriteByte(AddL);
	while(I2cReadAck());
	I2cwriteByte(data);
	while(I2cReadAck());
	I2cStop();
}
/*************************************************
  Function:EEPROM write byte per page continouly
  Description:
  Input: No
  Return: No
  Others: No
*************************************************/
void EepromPageWriteByte(uint8_t AddH,uint8_t AddL,uint8_t*table,uint8_t Length)
{
	uint8_t CtrByte=0xA0;//write mode
	uint8_t i;
	AddH = AddH <<1;
	CtrByte = CtrByte | AddH;
	I2cStart();
	I2cwriteByte(CtrByte);
	while(I2cReadAck());
	I2cwriteByte(AddL);
	while(I2cReadAck());
	for(i=0;i<Length;i++)
	{
		I2cwriteByte(*table);
		while(I2cReadAck());
		table++;
	}
	I2cStop();
}
/*************************************************
  Function:EEPROM read byte
  Description:
  Input: No
  Return: No
  Others: No
*************************************************/
uint8_t EepromReadByte(uint8_t AddH,uint8_t AddL)
{
	uint8_t data = 0;
	uint8_t CtrByte0 = 0xA0;//write mode
	uint8_t CtrByte1 = 0xA1;//read mode
//	AddH = AddH <<1;
//	CtrByte0 = CtrByte0 | AddH;
//	CtrByte1 = CtrByte1 | AddH;

	I2cStart();
	I2cwriteByte(CtrByte0);//write mode
	while(I2cReadAck());
	I2cwriteByte(AddH);
	while(I2cReadAck());
	I2cwriteByte(AddL);
	while(I2cReadAck());
	I2cStart();
	I2cwriteByte(CtrByte1);//read mode
	while(I2cReadAck());
	data = I2cReadByte();
	I2cNoAck();//��ʾ������
	I2cStop();

	return data;
}
/*************************************************
  Function:EEPROM read one function code (32bit)
  Description:
  Input: FcodeNum
  Return: No
  Others: No
*************************************************/
uint32_t EepromReadFcode(uint16_t FcodeNum)
{
	uint32_t value=0;
	uint8_t CtrByte0 = 0xA0;//write mode	uint8_t CtrByte1 = 0xA1;//read mode
	uint8_t CtrByte1 = 0xA1;//write mode	uint8_t CtrByte1 = 0xA1;//read mode
	uint8_t AddH,AddL,Temp0,Temp1,Temp2,Temp3;

	FcodeNum = FcodeNum * 4 - 4;

	AddH = (uint8_t)((FcodeNum >> 8)&0xFF);
	AddL = (uint8_t)(FcodeNum & 0xFF);
#if 0
	Temp0 = EepromReadByte(AddH,AddL);
	Delay(1000);
	Temp1 = EepromReadByte(AddH,(AddL+1));
	Delay(1000);
	Temp2 = EepromReadByte(AddH,(AddL+2));
	Delay(1000);
	Temp3 = EepromReadByte(AddH,(AddL+3));
	Delay(1000);

#else
//	AddH = AddH <<1;
//	CtrByte0 = CtrByte0 | AddH;
//	CtrByte1 = CtrByte1 | AddH;

	I2cStart();
	I2cwriteByte(CtrByte0);//write mode
	while(I2cReadAck());
	I2cwriteByte(AddH);
	while(I2cReadAck());
	I2cwriteByte(AddL);
	while(I2cReadAck());
	I2cStart();
	I2cwriteByte(CtrByte1);//read mode
	while(I2cReadAck());
	Temp3 = I2cReadByte();
//	while(I2cReadAck());
	I2cWriteAck();//������
	Temp2 = I2cReadByte();
//	while(I2cReadAck());
	I2cWriteAck();//������
	Temp1 = I2cReadByte();
//	while(I2cReadAck());
	I2cWriteAck();//������
	Temp0 = I2cReadByte();
	I2cNoAck();//������
	I2cStop();
#endif
	value =(((uint32_t)Temp3<<24) & 0xff000000) | \
			(((uint32_t)Temp2<<16) & 0xff0000) | \
			(((uint32_t)Temp1<<8) & 0xff00) | \
			((uint32_t)Temp0 & 0xff);

	return value;
}
/*************************************************
  Function:EEPROM write one function code (32bit)
  Description:
  Input: FcodeNum,FcodeValue
  Return: No
  Others: No
*************************************************/
void EepromWriteFcode(uint16_t FcodeNum,uint32_t value)
{
	uint8_t CtrByte0 = 0xA0;//write mode
	uint8_t AddH,AddL,Temp0,Temp1,Temp2,Temp3;

	if(FcodeNum <= FCODE_TOTAL_Num)
		FuncCode_Handle.Ram.F90.F9027 = FcodeTable[FcodeNum-1][1];//mxj
	else
		FuncCode_Handle.Ram.F90.F9027 = 20000 + FcodeNum;

	FuncCode_Handle.Ram.F90.F9028 ++ ;//mxj

//	FuncCode_Handle.E2prom.F90.F9027 = FuncCode_Handle.Ram.F90.F9027;
//	FuncCode_Handle.E2prom.F90.F9028 = FuncCode_Handle.Ram.F90.F9028;

	FcodeNum = FcodeNum * 4 - 4;

	AddH = (uint8_t)((FcodeNum >> 8)&0xFF);
	AddL = (uint8_t)(FcodeNum & 0xFF);
//	AddH = AddH <<1;
//	CtrByte0 = CtrByte0 | AddH;

	Temp3 = (uint8_t)((value >> 24) & 0xff);
	Temp2 = (uint8_t)((value >> 16) & 0xff);
	Temp1 = (uint8_t)((value >> 8) & 0xff);
	Temp0 = (uint8_t)(value& 0xff);
#if 0
	EepromWriteByte(AddH,AddL,Temp0);
	Delay(1000);
	EepromWriteByte(AddH,(AddL+1),Temp1);
	Delay(1000);
	EepromWriteByte(AddH,(AddL+2),Temp2);
	Delay(1000);
	EepromWriteByte(AddH,(AddL+3),Temp3);
	Delay(1000);

#else
	I2cStart();
	I2cwriteByte(CtrByte0);//write_mode
	g_I2cAck_cnt++;//+1
	while(I2cReadAck());
	I2cwriteByte(AddH);
	g_I2cAck_cnt++;//+2
	while(I2cReadAck());
	I2cwriteByte(AddL);
	g_I2cAck_cnt++;//3
	while(I2cReadAck());
	I2cwriteByte(Temp3);
	g_I2cAck_cnt++;//4
	while(I2cReadAck());
	I2cwriteByte(Temp2);
	g_I2cAck_cnt++;//5
	while(I2cReadAck());
	I2cwriteByte(Temp1);
	g_I2cAck_cnt++;//6
	while(I2cReadAck());
	I2cwriteByte(Temp0);
	g_I2cAck_cnt++;//7
	while(I2cReadAck());
	I2cStop();
#endif

}
/*************************************************
  Function:EEPROM init
  Description:
  Input: no
  Return: No
  Others: No
*************************************************/
void EepromInit(void)
{
	uint16_t i,tempdata;
	uint8_t Test=0;
	uint8_t counter=0;
	int32_t value,value0;
	static uint8_t ErrReadValueCount=0;

	for(counter=0;counter<5;counter++)//the last address 8K e2prom,��ö�����飬��ֹ�����
	{
		if(207==EepromReadByte(31,255))
		{
		Test = 207;
		break;
		}
		Delay(200);
	}
    g_out_counter = counter;
	/*IF eeprom is used first,need to write the FCode default value to  it*/
	if(Test != 207)
	{
		//EepromWriteByte(31,255,207);

		FuncCode_Handle.Ram = FuncCode_Handle.Init;//struct value directly set,the same struct
		FuncCode_Handle.E2prom =FuncCode_Handle.Init;
		Delay(200);
		//for(i=1;i<=FCODE_TOTAL_Num;i++)
		for(i=1;i<=FCODE_F00ToF80_Num;i++)
		{
			value = FuncCodeRead(FcodeTable[i-1][1]);//read value from funccode_handle.e2prom
			EepromWriteFcode(i,value);
			Delay(500);
			//value0 = EepromReadFcode(i);
			//Delay(100);
		}

		EepromWriteByte(31,255,207);
		AlarmStatusFlag.bit.Driver.EE1 = OK ;
	}
	else
	{
//		value = EepromReadFcode(64);
		//tempdata = FCODE_TOTAL_Num-FCODE_FA0_Num;//CYL FA�鹦���벻�ö�ȡ
		tempdata = FCODE_F00ToF80_Num;//20161209mxj FA,F9�鹦���벻�ö�ȡ
		for(i=1;i<=tempdata;i++)//FuncCodeReadDetect
		{
			Delay(2);//100delay=1.24ms ,1delay=12.4us

			value = EepromReadFcode(i);//8msһ����500���˵� 4s

			/*detect the function code */
			//ErrReadValueCount +=FuncCodeReadDetect(FcodeTable[i-1][1],value);
			Test = FuncCodeReadDetect(FcodeTable[i-1][1],value);
			ErrReadValueCount += Test;
			if(Test)
			{
				FuncCode_Handle.Ram.FA0.FA098 = ErrReadValueCount*10000 + FcodeTable[i-1][1];
				FuncCode_Handle.Ram.FA0.FA099 = value;
			}

			if(ErrReadValueCount>=5 )//��ֵֹ�Ƚ�
			{
				/* err counts >3,all function code reset default value*/
				FuncCode_Handle.Ram = FuncCode_Handle.Init;
				//FuncCode_Handle.E2prom =FuncCode_Handle.Init;//mxj 20170118 EE���ϲ���
				AlarmStatusFlag.bit.Driver.EE = OK;

				FuncCode_Handle.Ram.FA0.FA098 = ErrReadValueCount*10000 + FcodeTable[i-1][1];
				FuncCode_Handle.Ram.FA0.FA099 = value;

				break;
			}
			FuncCodeChange(FcodeTable[i-1][1],value,&FuncCode_Handle,NG);//the value from EEPROM give to FuncCode
			//*******************************************************************
			if(i<=12)
			{
				if(i==9)
				{
					FuncCode_Handle.Ram.F00.F0008 = value;//20200311 mxj
					FuncCode_Handle.E2prom.F00.F0008 = FuncCode_Handle.Ram.F00.F0008;
				}
				else if(i==10)
				{
					FuncCode_Handle.Ram.F00.F0009 = value;//20200311 mxj
					FuncCode_Handle.E2prom.F00.F0009 = FuncCode_Handle.Ram.F00.F0009;
				}
				else if(i==11)
				{
					FuncCode_Handle.Ram.F00.F0010 = value;//20200311 mxj
					FuncCode_Handle.E2prom.F00.F0010 = FuncCode_Handle.Ram.F00.F0010;
				}
				else if(i==12)
				{
					FuncCode_Handle.Ram.F00.F0011 = value;//20200311 mxj
					FuncCode_Handle.E2prom.F00.F0011 = FuncCode_Handle.Ram.F00.F0011;
				}
			}
			//*******************************************************************
		}
	}

	FuncCode_Handle.Ram.F90.F9029 = FuncCode_Handle.E2prom.F00.F0099;//20170223 mxj

	if(CGParameterLimitCheck(FuncCode_Handle.Ram.F00.F0002))//20200311�ж�����������������Ƿ�һ�¡�
	{//Error
		FuncCode_Handle.Min.F10.F1030 = 300;//�����ѹ����
		FuncCode_Handle.Max.F10.F1030 = 300;//�����ѹ����

		FuncCode_Handle.Min.F10.F1031 = 100;//�����������
		FuncCode_Handle.Max.F10.F1031 = 100;//�����������
	}
	else
	{
		FuncCode_Handle.Min.F10.F1030 = FuncCode_Handle.Ram.F00.F0008;//�����ѹ����
		FuncCode_Handle.Max.F10.F1030 = FuncCode_Handle.Ram.F00.F0009;//�����ѹ����

		FuncCode_Handle.Min.F10.F1031 = FuncCode_Handle.Ram.F00.F0010;//�����������
		FuncCode_Handle.Max.F10.F1031 = FuncCode_Handle.Ram.F00.F0011;//�����������

		FuncCode_Handle.Min.F10.F1006 = FuncCode_Handle.Ram.F00.F0010;//����������ָ������
		FuncCode_Handle.Max.F10.F1006 = FuncCode_Handle.Ram.F00.F0011;//����������ָ������

		FuncCode_Handle.Min.F10.F1008 = FuncCode_Handle.Ram.F00.F0008;//��ѹָ�����޵�����
		FuncCode_Handle.Max.F10.F1008 = FuncCode_Handle.Ram.F00.F0009;//��ѹָ�����޵�����
	}

	//if(FuncCode_Handle.Ram.F00.F0006 <= 999)//����100V������ʾ��λ
	if(FuncCode_Handle.Ram.F00.F0009 <= 999)//��ѹ���޸���100V������ʾ��λ
		FuncCode_Handle.Att.FA0.FA000 |=0x040;//��������λ��ʾ  80-��40

	CoreStatusFlag.bit.EEPROMINIT_OK_F = OK;
	CoreStatusFlag.bit.ZPHASE_F = NG;
	IO004_SetOutputValue(IO004_Handle0,0);
}

/*************************************************
  Function:Function Code to EEprom
  Description: Period: system loop
  Input: no
  Return: No
  Others: No
*************************************************/
void FcodeValueToEEPROM(void)//write value to e2prom
{
	uint16_t i = 0;
	uint32_t FC_Att = 0;
	uint32_t FcodeValue = 0;
	uint32_t temp;
    static uint8_t counter;

    if(g_FcodeChangeFlag == 0)
    	return;

	//for(i=1;i<=FCODE_TOTAL_Num;i++)//FuncCode Storage
	//for(i=1;i<=FCODE_F00ToF90_Num;i++)//FuncCode Storage FA �鲻����FCODE_F00ToF80_Num
	for(i=1;i<=FCODE_F00ToF80_Num;i++)//FuncCode Storage FA,F9 �鲻����
	{
		FC_Att = FuncCodeAttRead(FcodeTable[i-1][1]);//��
		if((FC_Att & 0x100) == 0x100)
		{
			if( i == FCODE_F00_Num+2) //F1001
			{
			    g_SpecialSave = FuncCodeRead(FcodeTable[i-1][1]); //��λ�� �޸� ҲҪ���
			}
			else//һ�������E2PROM�ĸ���
			{
				if(i == FCODE_F00ToF20_Num+5)//F1004
				g_SpecialSave = InflexibilityChange;

				FcodeValue = FuncCodeRead(FcodeTable[i-1][1]);//���������ֵEEPROM
				temp = EepromReadFcode(i);//����E2PROM��ֵ
				if(  FcodeValue != temp )//�������д
				{
					g_I2cAck_cnt = 0;
					FuncCode_Handle.Ram.FA0.FA035 = 0;
					FuncCode_Handle.Ram.FA0.FA036 = 0;
					//IO004_SetOutputValue(IO004_Handle27,1);//mxj 20170120  disable pwm
					 EepromWriteFcode(i,FcodeValue);//дֵ
					 Delay(1000);//200->1000
					// IO004_SetOutputValue(IO004_Handle27,0);//mxj 20170120  disable pwm
					 while ( EepromReadFcode(i) !=FcodeValue  )
					{
						 g_I2cAck_cnt = 10+counter*10;
						// IO004_SetOutputValue(IO004_Handle27,1);//mxj 20170120  disable pwm
						EepromWriteFcode(i,FcodeValue);
						Delay(1000);//200->1000
						//IO004_SetOutputValue(IO004_Handle27,0);//mxj 20170120  disable pwm
						counter++;
						if( counter >= 30)//3->30
						{
						  AlarmStatusFlag.bit.Driver.EE1 = OK ;
						}
					}
					 FuncCode_Handle.Ram.FA0.FA032 = counter;
					 counter =0;
				}

			}
			FuncCodeChangeAttClear(FcodeTable[i-1][1]);
		}
	}
	//g_FcodeChangeFlag = 0;
	g_FcodeChangeFlag--;
}











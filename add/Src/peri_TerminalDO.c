/*
 * peri_TerminalDO.c
 *
 * Created on: 2014-6-11
 * Author: JH-LvSL
 */
#include <main.h>			//Declarations from DAVE3 Code Generation (includes SFR declaration)
#include <peri_GlobalVariablesExtern.h>
#include <peri_TerminalDO.h>

void TerminalDOFuncHandle(const TERMINALDO_FUNC *func);
uint8_t TerminalDOSts(uint32_t Fcode);

static const TERMINALDO_FUNC TerminalDOProcessTable[] =
{
		TerminalDO1Handle,
		TerminalDO2Handle,
		TerminalDO3Handle,
		TerminalDO4Handle,
		TerminalDO5Handle,
		TerminalDO6Handle,
		NULL_FUNC
};
static const TERMINALDO_FUNC TerminalDOFuncTable[] =
{

		TerminalDOServoAlm,	//1��14��36
		TerminalDOServoRdy,	//2
		TerminalDOExBrake,	//3
		TerminalDOPosiOK,	//4
		TerminalDOSPDArvl,	//5
		TerminalDOTLimit,	//6
		TerminalDOZSPD,		//7
		TerminalDOSPDCO,	//8
		TerminalDOSPDLimit,	//9
		TerminalDOAlmOut1,	//10
		TerminalDOAlmOut2,	//11
		TerminalDOORZ,		//12
		TerminalDOPosiArvl,	//13
		//TerminalDOMtBrake,	//15
		//TerminalDOPulseOutput,//21,22  mxj 20160623
		//TerminalDOTORQArvl,	//29 20180904
	/*	TerminalDOPosiArv1,
		TerminalDOReserve1,
		TerminalDOReserve2,
		TerminalDOReserve3,
		TerminalDOReserve4,
		TerminalDOReserve5,
		TerminalDOReserve6,
		TerminalDOHiLevel,
		TerminalDOPulses,
		TerminalDODir,*/
		NULL_FUNC
};

/*************************************************
  Function: Terminal DO Handle
  Description:Period:main
  Input: No
  Return: No
  Others: Be Called
*************************************************/
void TerminalDOSetHandle(void)
{
	uint32_t FcodeValue = 0;

	FcodeValue = FuncCode_Handle.Ram.F50.F5009;//DO�����Чѡ��

	DOActiveSet.DO1 =  FcodeValue  & 0x0001;
	DOActiveSet.DO2 = (FcodeValue  & 0x0002)>>1;
	DOActiveSet.DO3 = (FcodeValue  & 0x0004)>>2;
	DOActiveSet.DO4 = (FcodeValue  & 0x0008)>>3;
	DOActiveSet.DO5 = (FcodeValue  & 0x0010)>>4;
	DOActiveSet.DO6 = (FcodeValue  & 0x0020)>>5;

	//FcodeValue = FuncCode_Handle.Ram.F50.F5010;//ѡȡ������
	DOFuncSet.DO1 = FuncCode_Handle.Ram.F50.F5010;//ѡȡ������
	DOFuncSet.DO2 = FuncCode_Handle.Ram.F50.F5011;//ѡȡ������
	DOFuncSet.DO3 = FuncCode_Handle.Ram.F50.F5012;//ѡȡ������
	DOFuncSet.DO4 = FuncCode_Handle.Ram.F50.F5013;//ѡȡ������
	DOFuncSet.DO5 = FuncCode_Handle.Ram.F50.F5014;//ѡȡ������
	DOFuncSet.DO6 = FuncCode_Handle.Ram.F50.F5015;//ѡȡ������
}

/*************************************************
  Function: Terminal DO Handle
  Description:Period:1ms
  Input: No
  Return: No
  Others: Be Called
*************************************************/
void TerminalDOHandle(void)
{
	TerminalDOFuncHandle(TerminalDOProcessTable);//��־λ�Ķ�ȡ
	TerminalDOFuncHandle(TerminalDOFuncTable);//��־λ������
}
/*************************************************
  Function: Terminal DO Fucntion Handle
  Description:Period:1ms
  Input: No
  Return: No
  Others: Be Called TerminalDOHandle()
*************************************************/
void TerminalDOFuncHandle(const TERMINALDO_FUNC *func)
{
    while((*func) != NULL_FUNC)
    {
        (*func)();
        func++;
    }
}

/*************************************************
  Function: Terminal DO1 Fucntion Handle
  Description:Period:1ms
  Input: F5010/IO004_SetOutputValue(IO004_Handle18,STS);
  Return: No
  Others: Be Called TerminalDOProcessTable()
*************************************************/
void  TerminalDO1Handle(void)
{
	//uint32_t FcodeValue = 0;

	//FcodeValue = FuncCode_Handle.Ram.F50.F5010;//ѡȡ������
    if( ForceDOOnEn.bit.ON1 | ForceDOOffEn.bit.OFF1)
    {
    	if(ForceDOOnEn.bit.ON1)				//ͨ����λ��ǿ��DO�����Ч
    	IO004_SetOutputValue(IO004_Handle18,0);
    	else								//ͨ����λ��ǿ��DO�����Ч
    	IO004_SetOutputValue(IO004_Handle18,1);
    }
    else
    {
//		if( TerminalDOSts(FcodeValue) )//����DOCtrlFlg�ı�־λ�������
//			IO004_SetOutputValue(IO004_Handle18,0);
//		else
//			IO004_SetOutputValue(IO004_Handle18,1);

		if( TerminalDOSts(DOFuncSet.DO1) )//����DOCtrlFlg�ı�־λ�������
			IO004_SetOutputValue(IO004_Handle18,DOActiveSet.DO1);
		else
			IO004_SetOutputValue(IO004_Handle18,!DOActiveSet.DO1);
    }
}
/*************************************************
  Function: Terminal DO2 Fucntion Handle
  Description:Period:1ms
  Input: F5011/IO004_SetOutputValue(IO004_Handle19,STS);
  Return: No
  Others: Be Called TerminalDOProcessTable()
*************************************************/
void  TerminalDO2Handle(void)
{
	//uint32_t FcodeValue = 0;

	//FcodeValue = FuncCode_Handle.Ram.F50.F5011;
    if( ForceDOOnEn.bit.ON2 | ForceDOOffEn.bit.OFF2)
    {
    	if(ForceDOOnEn.bit.ON2)
    	IO004_SetOutputValue(IO004_Handle19,0);
    	else
    	IO004_SetOutputValue(IO004_Handle19,1);
    }
    else
    {
		if(TerminalDOSts(DOFuncSet.DO2))
			IO004_SetOutputValue(IO004_Handle19,DOActiveSet.DO2);
		else
			IO004_SetOutputValue(IO004_Handle19,!DOActiveSet.DO2);
    }
}
/*************************************************
  Function: Terminal DO3 Fucntion Handle
  Description:Period:1ms
  Input: F5012/IO004_SetOutputValue(IO004_Handle20,STS);
  Return: No
  Others: Be Called TerminalDOProcessTable()
*************************************************/
void  TerminalDO3Handle(void)
{
	//uint32_t FcodeValue = 0;

	//FcodeValue = FuncCode_Handle.Ram.F50.F5012;
    if( ForceDOOnEn.bit.ON3 | ForceDOOffEn.bit.OFF3)
    {
    	if(ForceDOOnEn.bit.ON3)
    	IO004_SetOutputValue(IO004_Handle20,0);
    	else
    	IO004_SetOutputValue(IO004_Handle20,1);
    }
    else
    {
		if(TerminalDOSts(DOFuncSet.DO3))
			IO004_SetOutputValue(IO004_Handle20,DOActiveSet.DO3);
		else
			IO004_SetOutputValue(IO004_Handle20,!DOActiveSet.DO3);
    }
}
/*************************************************
  Function: Terminal DO4 Fucntion Handle
  Description:Period:1ms
  Input: F5013/IO004_SetOutputValue(IO004_Handle21,STS);
  Return: No
  Others: Be Called TerminalDOProcessTable()
*************************************************/
void  TerminalDO4Handle(void)
{
	//uint32_t FcodeValue = 0;

	//FcodeValue = FuncCode_Handle.Ram.F50.F5013;
    if( ForceDOOnEn.bit.ON4 | ForceDOOffEn.bit.OFF4)
    {
    	if(ForceDOOnEn.bit.ON4)
    	IO004_SetOutputValue(IO004_Handle21,0);
    	else
    	IO004_SetOutputValue(IO004_Handle21,1);
    }
    else
    {
		if(TerminalDOSts(DOFuncSet.DO4))
			IO004_SetOutputValue(IO004_Handle21,DOActiveSet.DO4);
		else
			IO004_SetOutputValue(IO004_Handle21,!DOActiveSet.DO4);
    }
}
/*************************************************
  Function: Terminal DO1 Fucntion Handle
  Description:Period:1ms
  Input: F5010/IO004_SetOutputValue(IO004_Handle18,STS);
  Return: No
  Others: Be Called TerminalDOProcessTable()
*************************************************/
void  TerminalDO5Handle(void)
{
	//uint32_t FcodeValue = 0;

	//FcodeValue = FuncCode_Handle.Ram.F50.F5014;
    if( ForceDOOnEn.bit.ON5 | ForceDOOffEn.bit.OFF5)
    {
    	if(ForceDOOnEn.bit.ON5)
    	IO004_SetOutputValue(IO004_Handle22,0);
    	else
    	IO004_SetOutputValue(IO004_Handle22,1);
    }
    else
    {
		if(TerminalDOSts(DOFuncSet.DO5))
			IO004_SetOutputValue(IO004_Handle22,DOActiveSet.DO5);
		else
			IO004_SetOutputValue(IO004_Handle22,!DOActiveSet.DO5);
    }
}
/*************************************************
  Function: Terminal DO1 Fucntion Handle
  Description:Period:1ms
  Input: F5010/IO004_SetOutputValue(IO004_Handle18,STS);
  Return: No
  Others: Be Called TerminalDOProcessTable()
*************************************************/
void  TerminalDO6Handle(void)
{
	//uint32_t FcodeValue = 0;

	//FcodeValue = FuncCode_Handle.Ram.F50.F5015;
    if( ForceDOOnEn.bit.ON6 | ForceDOOffEn.bit.OFF6)
    {
    	if(ForceDOOnEn.bit.ON6)
    		IO004_SetOutputValue(IO004_Handle23,0);
    	else
    		IO004_SetOutputValue(IO004_Handle23,1);
    }
    else
    {
    	if( TerminalDOSts(DOFuncSet.DO6) )//����DOCtrlFlg�ı�־λ�������
    		IO004_SetOutputValue(IO004_Handle23,DOActiveSet.DO6);
    	else
    		IO004_SetOutputValue(IO004_Handle23,!DOActiveSet.DO6);
    }
}

/*************************************************
  Function: Terminal DO Status
  Description:Period:1ms
  Input: No
  Return: No
  Others: Be Called TerminalDO1Handle()/TerminalDO2Handle/...
*************************************************/
uint8_t TerminalDOSts(uint32_t Fcode)
{
	uint8_t DOFlg = 0;

	switch(Fcode)
	{
	case DISABLE:
		break;
	case SERVOALMOUTPUT:
		DOFlg = DOCtrlFlg.bit.ServoAlm;//1
		break;
	case SERVORDYOUTPUT:
		DOFlg = DOCtrlFlg.bit.ServoRdy;//2
		break;
	case EXBRAKESIGNAL:
		DOFlg = DOCtrlFlg.bit.ExBrake;//3
		break;
	case POSITIONCOMPLETE:
		DOFlg = DOCtrlFlg.bit.PosiOK;//4
		break;
	case SPDARRIVALOUTPUT:
		DOFlg = DOCtrlFlg.bit.SPDArvl;//5
		break;
	case TORQUELIMITOUTPUT:
		DOFlg = DOCtrlFlg.bit.TLimit;//6
		break;
	case ZSPDSTSOUTPUT:
		DOFlg = DOCtrlFlg.bit.ZSPD;//7
		break;
	case SPDCCOUTPUT:
		DOFlg = DOCtrlFlg.bit.SPDCO;//8
		break;
	case SPDLIMITOUTPUT:
		DOFlg = DOCtrlFlg.bit.SPDLimit;//9
		break;
	//case POSITIONARRIVAL:
	//	DOFlg = DOCtrlFlg.bit.PosiArvl;//
	//	break;
	case ALARMOUTPUT1:
		DOFlg = DOCtrlFlg.bit.AlmOut1;//10
		break;
	case ALARMOUTPUT2:
		DOFlg = DOCtrlFlg.bit.AlmOut2;//11
		break;
	case ORIGINHOMECOMPLETE:
		DOFlg = DOCtrlFlg.bit.ORZ;//12
		break;
	case POSITIONARRIVAL://13
		DOFlg = DOCtrlFlg.bit.PosiArvl;//13
		break;
	case RUNFLG:					//14
		//DOFlg = CoreStatusFlag.bit.RUN_OK_F;
		DOFlg = DOCtrlFlg.bit.Run;//14
		break;
	case MOTORBRAKESIGNAL://15
		DOFlg = DOCtrlFlg.bit.MotorBreak;//15
		break;
	case DORESERVE4://17
		break;
	case DORESERVE5:
		break;
	case DORESERVE6:
		break;
	case DOHILEVEL://20
		DOFlg =1;
		break;
	case DOPULSES://21
		DOFlg = DOCtrlFlg.bit.Pulses;
		break;
	case DODIR://22
		DOFlg = DOCtrlFlg.bit.Dir;
		break;
	case TORQARRIVALOUTPUT: //29 ת�ص������  20180904
		DOFlg = DOCtrlFlg.bit.TorqArvl;
		break;
	case SERVOWARNNINGOUTPUT://16 ->36
		//DOFlg = CoreStatusFlag.bit.WARN_ALL_F;//16
		DOFlg = DOCtrlFlg.bit.Warnning;//16 ->36
		break;
	}
	return DOFlg;
}






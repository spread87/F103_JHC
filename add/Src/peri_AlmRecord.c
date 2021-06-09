/*
 * peri_AlmRecord.c
 *
 * Created on: 2015-2-25
 * Author: JH-LvSL
 */
#include <main.h>			//Declarations from DAVE3 Code Generation (includes SFR declaration)
#include <peri_GlobalVariablesExtern.h>

int32_t AlarmRcdTable[90];
AlmRcd AlmRecord;

extern void Delay(uint16_t );
extern void EepromWriteFcode(uint16_t ,uint32_t );
extern uint32_t EepromReadFcode(uint16_t );

/*************************************************
  Function: Alarm Record Display
  Description: period:System Loop  10ms ->main loop
  Input: No
  Return: No
  Others: Called by SysMainLoop() 10ms
*************************************************/
void AlmRcdDisplay(void)//����F9-00��ֵ������ʾRAM��
{
	int32_t FcodeValue = 0;
//	uint16_t temp;
//    static uint16_t max =0;
	FcodeValue = (int32_t)FuncCode_Handle.Ram.F90.F9000;//Ӧ����AlarmRcdTable��0��;

	FuncCode_Handle.Ram.F90.F9001 = AlarmRcdTable[FcodeValue * 9];
	FuncCode_Handle.Ram.F90.F9002 = AlarmRcdTable[FcodeValue * 9 + 1];
	FuncCode_Handle.Ram.F90.F9003 = AlarmRcdTable[FcodeValue * 9 + 2];
	FuncCode_Handle.Ram.F90.F9004 = AlarmRcdTable[FcodeValue * 9 + 3];
	FuncCode_Handle.Ram.F90.F9005 = AlarmRcdTable[FcodeValue * 9 + 4];
	FuncCode_Handle.Ram.F90.F9006 = AlarmRcdTable[FcodeValue * 9 + 5];
	FuncCode_Handle.Ram.F90.F9007 = AlarmRcdTable[FcodeValue * 9 + 6];
	FuncCode_Handle.Ram.F90.F9008 = AlarmRcdTable[FcodeValue * 9 + 7];
	FuncCode_Handle.Ram.F90.F9009 = AlarmRcdTable[FcodeValue * 9 + 8];
	FuncCode_Handle.Ram.F90.F9021 = AlmRecord.MaxTemp;
	FuncCode_Handle.Ram.F90.F9022 = AlmRecord.MaxVbus;
	FuncCode_Handle.Ram.F90.F9023 = AlmRecord.MaxIrms;
	FuncCode_Handle.Ram.F90.F9024 = AlmRecord.MaxVelo;

//	FuncCode_Handle.Ram.F00.F0000 = max;

}
/*************************************************
  Function: Alarm Record Refresh
  Description: period:System Loop
  Input: No
  Return: No
  Others: Called by SysMainLoop()
*************************************************/
void AlmRcdRefresh(void)
{
	uint8_t i = 9;
	int64_t Temp = 0;
	int32_t Temp1 = 0;

	if(!SysTimeSequ.Start_Flg)
	return;

	//if(g_PowerVbus1msFilQ24 < 8388608 )//mxj  155V/311V = IQ(0.5)=8388608
	if(g_PowerVbus1msFilQ24 < 11382612 )//mxj  211V/311V = IQ(0.678)=11382612 = 364V/537V    20180427
		return;

	if(g_PowerVbus1msFilQ24 < (g_LVPUQ24))//
		return;

	if(g_Temp10msFilQ24 > AlmRecord.MaxTemp)
	{
		AlmRecord.MaxTemp = g_Temp10msFilQ24;
		EepromWriteFcode(1590,AlmRecord.MaxTemp);
		Delay(500);
	}//��¼����¶ȣ�����ȡ��E2PROM 1590;

	//Temp = (int64_t)g_PowerVbus10msFilQ24 * 3111;
	if(DriverPara.RatedVol == 220)			//20180427
		Temp = (int64_t)g_PowerVbus1msFilQ24 * 3111;//20190908 g_PowerVbus10msFilQ24 - >g_PowerVbus1msFilQ24
	else
		Temp = (int64_t)g_PowerVbus1msFilQ24 * 5374;

	Temp = Temp >> 24;
	if((int32_t)Temp > AlmRecord.MaxVbus)
	{
		AlmRecord.MaxVbus = (int32_t)Temp;
		EepromWriteFcode(1591,AlmRecord.MaxVbus);
		Delay(500);
	}//��¼���ĸ�ߵ�ѹ������ȡ��E2PROM 1591;

    //Temp = abs(g_Irms10msFilQ24);
    Temp = abs(g_IrmsCalc);//20190908
    Temp = Temp * DriverPara.RatedCur;
    Temp = Temp >> 24;
    if(Temp > AlmRecord.MaxIrms)
    {
    	AlmRecord.MaxIrms = Temp;//��¼���ֵ
    	EepromWriteFcode(1592,AlmRecord.MaxIrms);
    	Delay(500);
    }

	//Temp = (int64_t)g_Velocity10msFilQ24 * MotorPara.RatedVel;
	Temp = Temp >> 24;
	if(abs(Temp) > abs(AlmRecord.MaxVelo))
	{
		AlmRecord.MaxVelo = Temp;
		EepromWriteFcode(1593,AlmRecord.MaxVelo);
		Delay(500);
	}//��¼����ٶȣ�����ȡ��E2PROM 1593;

	if(AlmRecord.AlmRefreshFlg == NG)
	{
		return;
	}
    //�й���ˢ�±�־
	Temp1 = (DriverPara.RatedCur * 1414)/1000;
    //�������ݺ���
	for(i=9;i>=1;i--)
	{
		AlarmRcdTable[i * 9] = AlarmRcdTable[(i-1) * 9];
		AlarmRcdTable[i * 9 + 1] = AlarmRcdTable[(i-1) * 9 + 1];
		AlarmRcdTable[i * 9 + 2] = AlarmRcdTable[(i-1) * 9 + 2];
		AlarmRcdTable[i * 9 + 3] = AlarmRcdTable[(i-1) * 9 + 3];
		AlarmRcdTable[i * 9 + 4] = AlarmRcdTable[(i-1) * 9 + 4];
		AlarmRcdTable[i * 9 + 5] = AlarmRcdTable[(i-1) * 9 + 5];
		AlarmRcdTable[i * 9 + 6] = AlarmRcdTable[(i-1) * 9 + 6];
		AlarmRcdTable[i * 9 + 7] = AlarmRcdTable[(i-1) * 9 + 7];
		AlarmRcdTable[i * 9 + 8] = AlarmRcdTable[(i-1) * 9 + 8];
	}
   //��ȡ��ǰ����
	AlarmRcdTable[0] = AlmRecord.AlmNum;//���ϴ���
	//Temp = (int64_t)AlmRecord.AlmVbus * 3111;
	if(DriverPara.RatedVol == 220)			//20180427
		Temp = (int64_t)AlmRecord.AlmVbus * 3111;
	else
		Temp = (int64_t)AlmRecord.AlmVbus * 5374;
	AlarmRcdTable[1] = (int32_t)(Temp >> 24);//F9-02  ĸ�ߵ�ѹ
	Temp = (int64_t)AlmRecord.AlmIrms * DriverPara.RatedCur;
	AlarmRcdTable[2] = (int32_t)(Temp >> 24);//F9-03ʵ�ʵ���

	if(AlarmStatusFlag.bit.Driver.ECE1||AlarmStatusFlag.bit.Motor.MC1)
	{
		AlarmRcdTable[3] = AlmRecord.FA41;//F9-04
		AlarmRcdTable[4] = AlmRecord.FA42;//20190628
	}
	else
	{
		Temp = (int64_t)AlmRecord.AlmIu * Temp1;
		AlarmRcdTable[3] = (int32_t)(Temp >> 24);//F9-04
		Temp = (int64_t)AlmRecord.AlmIv * Temp1;
		AlarmRcdTable[4] = (int32_t)(Temp >> 24);//F9-05
	}
	//Temp = (int64_t)AlmRecord.AlmVelo * MotorPara.RatedVel;
	Temp = (int64_t)AlmRecord.AlmVelo * 220;
	AlarmRcdTable[5] = (int32_t)(Temp >> 24);//F9-06
	AlarmRcdTable[6] = AlmRecord.AlmIOSts;
	AlarmRcdTable[7] = AlmRecord.AlmTemp;
	AlarmRcdTable[8] = AlmRecord.AlmCoreFlg;

	FuncCode_Handle.E2prom.F00.F0099++;//20161212  test
	FuncCode_Handle.Ram.F00.F0099 = FuncCode_Handle.E2prom.F00.F0099;
	FuncCode_Handle.Ram.F90.F9029 = FuncCode_Handle.E2prom.F00.F0099;

	FuncCode_Handle.Att.F00.F0099 |=0x0100;
	g_FcodeChangeFlag ++;

	AlmRecord.AlmRcdStgFlg = OK;//�����洢��־
	AlmRecord.AlmRefreshFlg = NG;

}
/*************************************************
  Function: Alarm Record Storage
  Description: period:System Loop
  Input: No
  Return: No
  Others: Called by SysMainLoop()
*************************************************/
void AlmRcdStorage(void)
{
	uint16_t i = 0;

	if(AlmRecord.AlmRcdStgFlg == NG)
	{
		return;
	}
//�д洢��־
	for(i=1500;i<1590;i++)//ȫ��д��E2PROM ���ж���������ν
	{
		EepromWriteFcode(i,AlarmRcdTable[i-1500]);
		Delay(500);//200->500
	}
//����洢��־
	AlmRecord.AlmRcdStgFlg = NG;
}
/*************************************************
  Function: Alarm Record Read
  Description: period:System Init
  Input: No
  Return: No
  Others: Called by InitModule()
*************************************************/
void AlmRcdRead(void)//��ʼ����ȡ
{
	uint16_t i;

	for(i=1500;i<1590;i++)
	{
		AlarmRcdTable[i-1500] = EepromReadFcode(i);
		Delay(100);//2.48ms  200->20 0.248ms  20180713 ->200 20180803
	}

	AlmRecord.MaxTemp = EepromReadFcode(1590);
	Delay(100);
	AlmRecord.MaxVbus = EepromReadFcode(1591);
	Delay(100);
	AlmRecord.MaxIrms = EepromReadFcode(1592);
	Delay(100);
	AlmRecord.MaxVelo = EepromReadFcode(1593);
	Delay(100);
	AlmRecord.AlmRcdFlg = OK;//����洢��־

}
/*************************************************
  Function: Clear Alarm Record
  Description: period:System Function Code Default
  Input: No
  Return: No
  Others:
*************************************************/
void AlmRcdClr(void)
{
	uint16_t i;

	FuncCode_Handle.Ram.F90 = FuncCode_Handle.Init.F90;//struct value directly set,the same struct
	FuncCode_Handle.E2prom.F90 =FuncCode_Handle.Init.F90;

	for(i=0;i<90;i++)
	{
		AlarmRcdTable[i] = 0;
	}

	for(i=1500;i<1594;i++)//������ϼ�¼�����ֵ��¼
	{
		EepromWriteFcode(i,0);
		Delay(500);//200->500
	}

	FuncCode_Handle.E2prom.F00.F0099 = 0;//20170223
	FuncCode_Handle.Ram.F00.F0099 = FuncCode_Handle.E2prom.F00.F0099;

	FuncCode_Handle.Att.F00.F0099 |=0x0100;
	g_FcodeChangeFlag ++;

	 AlmRecord.MaxIrms = 0;//1592
	 AlmRecord.MaxTemp = 0;//1590
	 AlmRecord.MaxVbus = 0;//1591
	 AlmRecord.MaxVelo = 0;//1593
}







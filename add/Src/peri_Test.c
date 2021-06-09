/*
 * peri_Test.c
 *
 * Created on: 2019-12-25
 * Author: JH-MXJ
 */
#include <main.h>			//Declarations from DAVE3 Code Generation (includes SFR declaration)
#include <peri_AlmWarnMacro.h>
#include <peri_GlobalVariablesExtern.h>
#include <XMC4400.h>
extern LPF CurCmdFil;
extern uint8_t g_reset_flag;
int32_t test0;
uint32_t test1=0;
uint8_t g_serve_ready = 0;
uint32_t Cnt_Stop_1ms = 0;
uint32_t Cnt_Stop_Status = 0;
void status_clr(void);
void AlmWarnRecord(void);//20161128CYL

void Test(void)
{
	static uint8_t AlmRecord_Time = 0;
	static uint16_t sWarnStatusFlagOld=0;//20161128CYL
	uint32_t FcodeValue = 0;
    uint8_t serve_start=0;//�����źţ�����JOG,IO�ڣ���λ����

	FcodeValue = FuncCode_Handle.Ram.F10.F1000;//��������

	if(AlarmStatusFlag.all != 0 )
	{
		CoreStatusFlag.bit.ERR_ALL_STOP_F = OK;//�������
	}
	else
	{
		CoreStatusFlag.bit.ERR_ALL_STOP_F = NG;
	}

	if(WarnStatusFlag.all)						//20161128CYL
	{
		CoreStatusFlag.bit.WARN_ALL_F =OK;//�о���ʱ���Կ�������
		if(sWarnStatusFlagOld==0)
			;//AlmWarnRecord(); //��¼���ϲ���
	}
	else
	{
		CoreStatusFlag.bit.WARN_ALL_F =NG;
	}
	sWarnStatusFlagOld=WarnStatusFlag.all;

	serve_start =  (FcodeValue||DICtrlFlg.bit.ServoOn||ComCtrlFlg.bit.RunFlg);//���Ͽ��������� mxj

	if(AlarmStatusFlag.all)//����
	{
		//IO004_SetOutputValue(IO004_Handle27,0);//����DRIVE�ź�  20180717
		CoreStatusFlag.bit.RUN0_OK_F = NG;
		g_VeloRefQ24 = 0;
		g_VelocityRefQ24 = 0;

		if(Cnt_Stop_1ms >= FuncCode_Handle.Ram.F50.F5056)//10->20 20171108 mxj
		{
			//CoreStatusFlag.bit.RUN_OK_F = NG;
			status_clr();

			AlmRecord_Time = 0;//mxj
			Cnt_Stop_1ms = 0;//mxj
		}
		else
		{
			Cnt_Stop_1ms ++ ;
		}
	}
	else//û����
	{
	  AlmRecord.AlmRcdFlg = OK;//û�� ���� �������
	  if(serve_start)//�������ź�
	  {
			//IO004_SetOutputValue(IO004_Handle27,0);//����DRIVE�ź�  20180717
			CoreStatusFlag.bit.RUN0_OK_F = OK;
			//  CoreStatusFlag.bit.RUN_OK_F = OK;
			Cnt_Stop_1ms = 0;//mxj
	  }
	  else//û�������ź� �����ٹ�
	  {
		   CoreStatusFlag.bit.RUN0_OK_F = NG;

				   if(Cnt_Stop_1ms >= FuncCode_Handle.Ram.F50.F5056)//10->20 20171112 mxj
				   {
						status_clr();
						AlmRecord_Time = 0;//mxj
						Cnt_Stop_1ms = 0;//mxj
				   }
				   else
				   {
					   Cnt_Stop_1ms ++ ;
				   }
	   }
     }
}

/*************************************************
  Function: ���ϼ�¼
  Description: ���ֹ��ϻ򾯸�ʱ��¼����
  Input: No
  Return: No
  Others:
*************************************************/
void AlmWarnRecord(void)//���ֹ���ʱ������
{
	if(AlmRecord.AlmRcdFlg)//��¼����
    {
		 AlmRecord.AlmRcdFlg = NG;//��ʼ���Ժ���Խ��д洢 ������������Ϻ�
		 AlmRecord.AlmRefreshFlg = OK;
		 AlmRecord.AlmNum = g_AlmNum;//������Դ
		 AlmRecord.AlmVbus = g_PowerVbus1msFilQ24;//g_PowerVbus10msFilQ24->g_PowerVbus1msFilQ24
		 AlmRecord.AlmIrms = g_IrmsCalc;//g_Irms10msFilQ24;
		 AlmRecord.AlmIu = g_Iu100usFilQ24;
		 AlmRecord.AlmIv = g_Iv100usFilQ24;
		 //AlmRecord.AlmVelo = g_Velocity10msFilQ24;
		 //AlmRecord.AlmVelo = g_VelocityQ24Fil;//20190908
		 AlmRecord.AlmVelo =  g_VrmsCalc;
		 AlmRecord.AlmIOSts = g_DIDOsts;
		 AlmRecord.AlmTemp = g_Temp10msFilQ24;
		 AlmRecord.AlmCoreFlg = CoreStatusFlag.all;

		 AlmRecord.FA41 = FuncCode_Handle.Ram.FA0.FA041;//F9-04
		 //AlmRecord.FA42 = FuncCode_Handle.Ram.FA0.FA042;//20190628
	}
}

void status_clr(void)//���״̬
{
	g_VeloRefQ24 = 0;//�ٶ�ģʽ�ĸ���ֵ
	g_JogRefQ24 = 0;
	g_InertiaRefQ24 =0;
	g_InertiaQ24 =0;
	g_CurInVelocityQ24 = 0; //����ģʽ���ٶȸ���ֵ
	g_CurInVeloQ24 = 0;//����ģʽ���ٶȸ���ֵ

	g_CurrentRefQ24 = 0;//IQ����
}

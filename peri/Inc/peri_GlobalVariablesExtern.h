/*
 *  peri_GlobalVariablesExtern.h
 *
 *  Created on: 2014-2-12
 *  Author: JH-LvSL
 */

#ifndef PERI_GLOBALVARIABLESEXTERN_H_
#define PERI_GLOBALVARIABLESEXTERN_H_

//#include "arm_math.h"
//#include "arm_common_tables.h"
#include <peri_AlmWarnStruct.h>
#include <peri_FuncCodeEnum.h>
#include <peri_FuncCodeHandle.h>
#include <peri_FuncCodeStruct.h>
#include <peri_Inertia.h>
//#include <peri_InitADC.h>
//#include <peri_InitCCU4.h>
//#include <peri_InitCCU8.h>
//#include <peri_InitPOSIF.h>
#include <peri_LpfHpf.h>
//#include <peri_NVIC.h>
//#include <peri_PulseInputProcess.h>
//#include <peri_PulseOutputProcess.h>
//#include <peri_SeriEncoder.h>
#include <peri_TimerCtl.h>
//#include <peri_UARTHandle.h>

#include "core_StructDef.h"
#ifndef ARM_MATH_CM4
#define ARM_MATH_CM4
#endif

#ifndef ARM_MATH_BIG_ENDIAN
#define ARM_MATH_BIG_ENDIAN
#endif

#define    OUTPUT_ABZ        0
#define    OUTPUT_DIR        1
#define    OUTPUT_CCW        0

//#define    g_CurrentLoopTc   100  //100us ��������
#define    g_CurrentLoopTc   125  //125us ��������
#define    g_VelocityloopTc  0x2800000 //0.25ms*10  �ٶȻ�����
//#define    g_VelocityloopTc  0x5000000 //0.5ms*10 �ٶȻ�����
#define    VelocityLPFTime   2000//2000us �ݶ���ô��
#define 	OK 	 1
#define 	NG	 0
#define  POSITIONCTRL    0
#define  VELOCITYCTRL    1
#define  CURRENTCTRL    2

/*Pulse Counts per Revolution*/
#define		 	PULCNTREVO			(int16_t)10000
/*1/Pulse Counts  (reciprocal) value of Q24 Data type.*/
#define			RECIPPULCNTQ24	(int32_t)1678 //Q24(1/10000)

#ifndef NULL_FUNC
    #define NULL_FUNC (void *)0
#endif

extern uint16_t 			Timer1msCount;
extern FuncCode	 			FuncCode_Handle;
extern CoreStatusFlg 		CoreStatusFlag;
extern AlarmStatusFlg		AlarmStatusFlag;
extern WarnStatusFlg		WarnStatusFlag;
extern AngleFunc 			AngleFC;
extern CURRENT	  			Current1msQ24;

extern LPF64   				PositionCmdFil;
extern LPF 					VeloFeedFwdFil;

extern LPF 					VeloFeedBackFil ;

extern LPF 					CurFeedFwdFil ;

extern LPF 					Velocity1msFil ;

extern LPF 					Velocity10msFil ;

extern DICtrl				DICtrlFlg;

extern DICtrl				VirtualDICtrlFlg;

extern DICtrl				TerminalDICtrlFlg;

extern DOCtrl	 			DOCtrlFlg;

extern DOActive				DOActiveSet;//20170329  mxj

extern DOFunc				DOFuncSet;//20170329  mxj

extern AnalogInput 			ExAI;

extern TqLimit 				TorqueLimit;

extern DriverParameter 		DriverPara;

extern SystemTimeSequ		SysTimeSequ;

extern MotorSts 			MotorStatus;

extern AlmRcd 				AlmRecord;

extern ComCMDFlg			ComCtrlFlg;

extern COMVirlDI			COMVirtualDI;	//Communication Virtual DI Value

extern uint32_t 			SysTickCount;

extern uint32_t 			g_FcodeChangeFlag;



/********Function Code Global Block*********/
extern const uint16_t FcodeTable[][2];
extern const uint16_t FcodeGroupNum[];
extern const int32_t SINTable1024Q24[];
extern const int32_t TANTable1024Q24[];

extern const ChargerParameter Charger_lever[];
extern int32_t AlarmRcdTable[90];

/*********USB Communication****************/
/* Buffer to receive data */
extern uint8_t RxBuffer[100];
/* Buffer to transmit data */
extern uint8_t TxBuffer[100];
/*channel data address*/
extern uint32_t *ChannelDataAdd1;
extern uint32_t *ChannelDataAdd2;
extern uint32_t *ChannelDataAdd3;
extern uint32_t *ChannelDataAdd4;

extern int32_t CK_Value0;
extern int32_t CK_Value1;
extern int32_t CK_Value2;
extern int32_t CK_Value3;
extern int32_t g_NoValue;
/**********Core Variable Define************/
extern uint16_t  	g_CarryPwmFreq;//0.1kHz
extern uint32_t 	g_EncSpeedQ16;	// calc speed from encoder
extern uint32_t 	g_EncSpeed1msFilQ16;//after LPF(1ms)
extern int32_t 		g_PosDevtReg;//Position Deviation Register
extern int32_t 		g_PosDevtRegCount;//Position Deviation Pulse Count(Total Deviation)

extern int64_t 		g_PulInCan;
extern int32_t		g_SpdInCan;
extern int32_t		g_IqInCan;
extern int32_t		g_SendcntCan;
extern int32_t		g_CmdInCan;
extern int64_t 		g_PulInAllFWD;
extern int64_t		g_PulInAllCount;
extern int64_t		g_PulInAllCountOld;
extern int64_t		g_PulInAllCountErr;
extern int64_t		g_PulInAllCount_Filter;
extern int64_t		g_EncPulAllCount;
extern int64_t 		g_PosiFeedback;
extern int64_t 		g_SHome_Finish_g_EncPulAllCount;
extern int64_t		g_EncPulAllCountOld;
extern int32_t 		g_EncDirChgCount;
extern int32_t 		g_EncPulCountsOld;

extern uint32_t		g_PulInPrdCCWCount;		//Pulse Input Dir CCW + Period
extern uint32_t		g_PulInPrdCWCount;		//Pulse Input Dir CW + Period
//extern uint32_t 	g_PulInCCWTimer;		//Pulse Input CCW Timer
//extern uint32_t 	g_PulInCWTimer;			//Pulse Input CW Timer
extern uint32_t 	g_PositionCount;//position Pulse
extern int32_t 		g_PosCmd;	//Position Command
extern int32_t 		g_PosCmdFil;//Position Command After LPF
extern uint16_t 	g_PulInPOSCountOld;//Pulse Input Count before 100us
extern uint16_t 	g_PulInPOSCount;	//Pulse Input Count
extern int16_t 		g_PulInPOSDelta;	//Pulse Input Delta Count  Between now and 100us Forward
extern uint32_t 	g_PulInInnerRef;	//Inner Pulse Input Ref
extern int32_t 		g_PulInTarget;//Inner Pulse Input Target
extern int32_t		g_VdQ24;
extern int32_t 		g_RevolutionCount;//revolution from app

extern float 		g_Angle;//motor angle from app
extern float 		g_Velocity;//motor velocity from app
extern uint32_t 	g_AngleQ24;//Motor Angle Q24
extern uint32_t 	g_MecThetaQ24;//0~2^24 <=> 0~2PI
extern uint32_t		g_EleThetaQ24;//0~2^24 <=> 0~2PI
extern uint32_t		g_EncoderUVWQ24;
extern int32_t    	g_Uvw_Theta;

extern uint32_t		g_EncoderZcheck;
extern int32_t 		g_VelocityQ24abs;
extern int32_t 		g_VelocityQ24;
extern int32_t 		g_VelocityQ24_1;
extern int32_t 		g_VelocityQ24Fdb;
extern int32_t 		g_VelocityQ24Fdb_Acc;
extern int32_t 		g_VelocityQ24Fil;
extern int32_t 		g_VelocityQ24_Arv1;
extern int32_t 		g_VelocityQ24_Zero;
extern int32_t		g_TorqueQ24_Arv1;
extern int32_t		g_TorqueArv1_Time;//
extern int32_t		g_TorqueLock_Time;//
extern int32_t	 	g_Velocity10msFilQ24;
extern int32_t 		g_Velocity10times;	//Display 10 Times Zoom
extern int32_t 		g_PulInPosition; //Pulse Input Position
extern int32_t 		g_PulInPosition_Old; //Pulse Input Position Ref
extern int32_t 		g_PulInPosi_Spd_Acc; //Pulse Input Position Ref
extern int32_t 		g_PulInPosi_Spd_Acc_K;
extern int32_t 		g_VelocityRefQ24;//Complex Value, include Position Mode and Velocity Mode
extern int32_t		g_VelocityMaxQ24;
extern int32_t		g_VelocityMaxQ24_17single;
//extern int32_t		g_VelocityRefQ24_Acc;
extern int32_t 		g_VeloRefQ24;//Only in the Velocity Mode
extern int32_t 		g_VeloTargetQ24;//Velocity Target Value
extern int32_t 		g_VeloTargetInitQ24;//Velocity Value Before Changed
extern uint32_t    	g_VelocityKp;
extern uint32_t    	g_VelocityKi;
extern uint16_t 	g_VeloFBF_Type;

extern int32_t 		g_CurrentRefQ24;
extern int32_t		g_CurrentRefQ24_InitialAngle;
extern int32_t 		g_CurRefQ24;//External Set Value
extern int32_t 		g_CurRefQ24_Foc;//External Set Value
extern int32_t 		g_TorquePositiveLimit;	/*Torque Limit in the Positive Direction*/
extern int32_t 		g_TorqueNegativeLimit;	/*Torque Limit in the Negative Direction*/
extern int32_t 		g_VeloACCStepValueQ24;//Velocity Acc Step Value per 1ms
extern int32_t 		g_VeloDECStepValueQ24;//Velocity Dec Step Value per 1ms
extern int32_t 		g_CurInVeloQ24 ;//Velocity Value in Torque Control Mode
extern int32_t 		g_CurInVelocityQ24 ;//Velocity Value in Torque Control Mode
extern int32_t 		g_IrmsCalc;
extern int32_t 		g_Irms_Cmd;
extern int32_t 		g_Irms_Cmd_A;
extern int32_t 		g_Vrms_Cmd;
extern int32_t 		g_VrmsCalc;
extern int32_t		g_PrmsCalc;
extern int64_t 		g_filterVd;
extern int64_t 		g_filterVq;

extern uint32_t 	g_PwmPeriod;//Pwm Period Reg
extern uint32_t    g_LdValueQ24;//Ld�ı�������
extern uint32_t    g_LqValueQ24;//Lq�ı�����;
extern uint32_t    g_FluxfQ24;//Fluxf�ı�������
extern uint32_t    g_OverLoadFactor;//����ϵ��
extern uint64_t    g_index_counter;
extern uint16_t    timer_count;
extern uint16_t    g_timer_count;
extern uint16_t    g_timer_flag;
extern uint8_t     run_ready;
extern int32_t     g_out_counter;
extern int32_t     g_out_counter1;
extern uint32_t    g_PulsesPerRevolution;
extern uint32_t    g_VelocityKpCalc;
extern uint32_t    g_SpecialSave;
extern uint32_t    g_VelocityKpAdjust;
extern int32_t     g_JogRefQ24;//�㶯�ٶȸ���
extern int32_t     g_InertiaRefQ24;
extern uint64_t    g_InertiaQ24;
extern uint8_t     g_JogRunDw;
extern uint8_t     g_JogRunUp;
extern uint8_t     g_JogRunUpButton;
extern uint8_t     g_JogRunDwButton;
extern int32_t     g_JogTargetQ24;
extern int32_t     g_JogACCStepValueQ24;
extern int32_t     g_JogDECStepValueQ24;
extern int32_t     g_OutputPulses;//��������������ƽ
extern int32_t     g_OutputDir;//��������ķ����ƽ
extern const  uint32_t    DriveCurrentGain[];
extern int32_t     g_MotorCurGain;
extern int32_t     g_MotorCurGain_1;
extern uint8_t     g_ZeroSpdMode;
extern uint8_t     g_ZeroSpdLockMode;
extern uint16_t    g_ABErrCount;
extern INERTIA_STATUS Inertia_status;
extern int32_t     g_acciq[];
extern int32_t     g_deciq[];
extern int32_t     g_InertiaGain;
extern int32_t     g_InertiaTotal;

extern uint32_t	g_MotorInitAngleQ24;

//extern uint8_t		AlmRecord_Time;

extern FORCEDIONEN          ForceDIOnEn;//ONʹ��
extern FORCEDIOFFEN         ForceDIOffEn;//OFFʹ��
extern uint32_t             g_DIForceOn;
extern uint32_t             g_DIForceOff;

extern FORCEDOONEN          ForceDOOnEn;//ONʹ��
extern FORCEDOOFFEN         ForceDOOffEn;//OFFʹ��




/******Function Variable Define*****/
extern uint16_t 	g_CtrlMode;	//From External Set
extern uint16_t 	g_UltimateCtrlMode; //Ultimate Control Mode
extern uint32_t   	g_RunTime;//Run Time
extern uint32_t 	g_PowerUpTime;//Power Time
extern uint32_t 	g_ComCtrlCMD;//Communication Control Command

/* LED Display */
extern int32_t 		g_LEDVelocityCmd;
extern int32_t 		g_LEDPositionCmd;
extern int32_t 		g_LEDTorqueCmd;
extern int32_t 		g_LEDPosiDev;
extern int32_t 		g_LEDLoadRated;
extern int32_t 		g_LEDTem;
extern int32_t 		g_LEDEncDev;
extern int32_t 		g_LEDAI1;
extern int32_t 		g_LEDAI2;
extern int32_t 		g_LEDAI3;
extern int32_t 		g_LEDVbus;
extern int32_t 		g_LEDCurrent;
extern int32_t 		g_LEDRunTime;


/************AO Value**************/
extern  int32_t 	g_PosiCMDSpeedQ24;	// Position Command Speed
extern int32_t 		g_PosiCMDDev;//Position Command Deviation
extern int32_t 		g_EncCMDDev;//Encoder Command Deviation

/**********Adc handle**********************/
//extern ADC001_ResultHandleType Result[11];
extern uint16_t 	AdcResult[15];

/**********AD-Sample Variable*************/
extern int64_t 		AD_Test;
extern uint32_t 	g_IuAD;	// AD sample value
extern uint32_t 	g_IvAD;
extern uint32_t 	g_IwAD;

extern int32_t 	g_Iu100usQ24; //100us Instantaneous value
extern int32_t 	g_Iv100usQ24;
extern int32_t 	g_Iw100usQ24;

extern int32_t 	g_Iu100usFilQ24; // value after LPF
extern int32_t 	g_Iv100usFilQ24;
extern int32_t 	g_Iw100usFilQ24;

extern int32_t 	g_Iu1msFilQ24; // value after LPF
extern int32_t 	g_Iv1msFilQ24;
extern int32_t 	g_Iw1msFilQ24;

extern int32_t 	g_Iu10msFilQ24; // value after LPF
extern int32_t 	g_Iv10msFilQ24;
extern int32_t 	g_Iw10msFilQ24;

extern uint32_t 	g_Irms1msFilQ24;//Irms1ms is used to OL
extern uint32_t 	g_Irms10msFilQ24;//
extern uint32_t 	g_IrmsAvgFilZoom10;//Irms average filter and be zoomed 10

//Vbus Voltage Sample
extern uint16_t 	g_PowerVbusAD;
extern uint16_t 	g_CtrlVbusAD;
extern int32_t 		g_PowerVbus100usQ24;
extern uint32_t 	g_CtrlVbus100usQ24;
extern int32_t 		g_PowerVbus1msFilQ24;
//extern int32_t 		g_PowerVbus1msFilQ24_PU;
extern int32_t 		g_PowerVbus100usQ24_PU;
extern int32_t 		g_CtrlVbus1msFilQ24;
extern int32_t 		g_PowerVbus10msFilQ24;
extern int32_t 		g_CtrlVbus10msFilQ24;
extern int32_t 		g_PowerVbusAvgFil;
extern int32_t 		g_CtrlVbusAvgFil;

//Temperature Sample
extern uint16_t 	g_TempAD;
extern int32_t 		g_Temp100usQ24;
extern int32_t 		g_Temp10msFilQ24;
extern int32_t 		g_TempQ24;
extern int32_t 		g_TempAvg;

//Motor Temperature Sample
extern uint16_t 	g_PTCAD;
extern int32_t		g_PTC100usQ24;
extern int32_t 		g_PTC10msFilQ24;
extern int32_t 		g_PTCAvg;

//Analog Input Sample
extern uint16_t 	g_AI1AD;
extern int32_t	 	g_AI1Q24;
extern int32_t 		g_AI1FilQ24;

extern uint16_t 	g_AI2AD;
extern int32_t	 	g_AI2Q24;
extern int32_t	 	g_AI2FilQ24;

extern uint16_t 	g_AI3AD;
extern int32_t	 	g_AI3Q24;
extern int32_t 		g_AI3FilQ24;

extern int32_t 		g_165AD;
extern int32_t 		g_141AD;

/*********Function Variable********************/
extern uint16_t 	g_AlmNum;	//Alarm Number
extern uint32_t 	g_OVPUQ24;
extern uint32_t 	g_LVPUQ24;
extern uint32_t 	g_DOCPUQ24;	//Driver OC Protect PU Value
extern uint32_t 	g_MOCPUQ24;	//Motor OC Protect PU Value
extern int32_t 		g_DIsts;
extern int32_t 		g_DIAddVir_sts;
extern int32_t 		g_DOsts;
extern int32_t 		g_DIDOsts;

extern int16_t 		g_LEDErrDisplayMode;

//mxj============================================================================
extern int32_t	 	g_CurrentRefLpf;			/*Current Settings After LPF */
extern int32_t	 	g_EThelta;
extern int32_t 		g_CurrentId_ref;
extern int32_t 		g_CurrentIq_ref;
extern int32_t 		g_CurrentId;			/*Current Settings After LPF */
extern int32_t 		g_CurrentIq;			/*Current Settings After LPF */
extern int32_t		g_CurrentId_Out;			/*Current Settings After LPF */
extern int32_t 		g_CurrentIq_Out;			/*Current Settings After LPF */
extern int32_t 		g_Motor_Power;			//mxj
//mxj============================================================================

/*test view*/
extern int32_t  g_canview1;
extern int32_t  g_canview2;
extern int32_t  g_canview3;
extern int32_t  g_canview4;
extern int32_t  g_canview5;
extern int32_t  g_canview6;
extern int32_t  g_canview7;
extern int32_t  g_canview8;
extern int32_t  g_canview9;
extern int32_t  g_canview10;
extern int32_t  g_canview11;
extern int32_t  g_canview12;
extern int32_t  g_canview13;
extern int32_t  g_canview14;
extern int32_t  g_canview15;
extern int32_t  g_canview16;
extern int32_t  g_canview17;
extern int32_t  g_canview18;
extern int32_t  g_canview19;
extern int32_t  g_canview20;
extern int32_t  g_canview21;
extern int32_t  g_canview22;
extern int32_t  g_canview23;
extern int32_t  g_canview24;
extern int32_t  g_canview25;
extern int32_t  g_canview26;
extern int32_t  g_canview27;
extern int32_t  g_canview28;
extern int32_t  g_canview29;
extern int32_t  g_canview30;
extern int32_t  g_canview31;
extern int32_t  g_canview32;
extern int32_t  g_canview33;
extern int32_t  g_canview34;

extern int32_t g_ComPulse;
extern int32_t g_ComSpeed;
extern int32_t g_ComTorque;
extern int32_t g_ComPidRef;
extern int32_t g_ComAO1;
extern int32_t g_ComAO2;
extern int32_t g_ComDO;
extern int32_t g_ComCmdModel;

extern int32_t g_ABSThetaQ24;
extern uint16_t g_ABSCycNum;

extern int64_t  gMotorPosi;
extern int64_t  gEncPosi;
extern int64_t gPosiHome;
extern int64_t gSHomePullIN;
extern  int32_t g_FLAG;
extern  int32_t g_FLAG1;
extern  int32_t g_FLAG2;
extern SHomeFlg SHomeFlag; //ԭ��ع��־λ
extern  SHomePara SHomeParameter;
extern SeriEnc SeriEncoder;

extern int32_t g_VpnVoltGain;
extern int32_t g_RelayrunQ24;
extern int32_t g_RelaystopQ24;

extern int32_t g_EasyPLCPosiAllCounts_old;
extern int32_t g_CurrentRefAddLim;
extern uint8_t g_tempUVW;

//extern int16_t WaveA[1000];
//extern int16_t WaveB[1000];
//extern int16_t WaveC[1000];
//extern int16_t WaveD[1000];

#endif /* PERI_GLOBALVARIABLESEXTERN_H_ */

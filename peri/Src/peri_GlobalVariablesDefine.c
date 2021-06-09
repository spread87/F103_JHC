/*
 * peri_GlobalVariablesDefine.c
 *
 * Created on: 2019-12-23
 * Author: JH-LvSL
 */

#include <main.h>
#include <peri_GlobalVariablesExtern.h>

FuncCode 			FuncCode_Handle;// __attribute__((section("FUNCTION_RAM")));		//Function Code define and handle
CoreStatusFlg 		CoreStatusFlag;
AlarmStatusFlg 		AlarmStatusFlag;
WarnStatusFlg		WarnStatusFlag;
AngleFunc 			AngleFC;
CURRENT				Current1msQ24;

DICtrl				DICtrlFlg;
DICtrl				VirtualDICtrlFlg;
DICtrl				TerminalDICtrlFlg;
DOCtrl 				DOCtrlFlg;
DOActive			DOActiveSet;//20170329  mxj
DOFunc				DOFuncSet;//20170329  mxj
AnalogInput 		ExAI;/*External Analog Input*/

DriverParameter 	DriverPara;
SystemTimeSequ		SysTimeSequ;
ComCMDFlg			ComCtrlFlg;
COMVirlDI			COMVirtualDI={0};	//Communication Virtual DI Value
INERTIA_STATUS      Inertia_status=INERTIA_DEFAULTS;
/********bmef_cal****************/

uint16_t            Timer1msCount = 0;

/**********USB Communication*******/
/* Buffer to receive data */
uint8_t 	RxBuffer[100] = { 0 };
/* Buffer to transmit data */
uint8_t 	TxBuffer[100] = { 0 };
/*channel data address*/
uint32_t *ChannelDataAdd1;
uint32_t *ChannelDataAdd2;
uint32_t *ChannelDataAdd3;
uint32_t *ChannelDataAdd4;

int32_t CK_Value0 = 0;
int32_t CK_Value1 = 0;
int32_t CK_Value2 = 0;
int32_t CK_Value3 = 0;

/*****Core Variable Define********/
uint16_t  	g_CarryPwmFreq = 100;//0.1kHz
uint32_t 	g_EncSpeedQ16 = 0;	// calc speed from encoder
uint32_t 	g_EncSpeed1msFilQ16 = 0;//LPF(1ms)

int32_t 	g_EncPulCountsOld = 0;
//uint16_t 	g_PositionCount = 0;//Encoder Position
uint32_t 	g_PositionCount = 0;//Encoder Position CYL170306  ���Ӵ��б����� uint16_t->uint32_t
int32_t 	g_RevolutionCount = 0;//
float 		g_Angle = 0;
float 		g_Velocity = 0;
uint32_t 	g_AngleQ24 = 0;	//0~2^24 <=> 0~2PI
uint32_t 	g_MecThetaQ24 = 0;//Mechanical Theta:0~2^24 <=> 0~2PI
uint32_t	g_EleThetaQ24 = 0;//Electrical Theta:0~2^24 <=> 0~2PI
 int32_t    g_Uvw_Theta;
uint32_t	g_EncoderUVWQ24 = 0;//
uint32_t	g_EncoderZcheck = 0;
int32_t 	g_VelocityQ24abs = 0;
int32_t 	g_VelocityQ24 = 0;	//0~2^24 <=> 0~ Motor Rated Velocity
int32_t 	g_VelocityQ24_1=0;
int32_t 	g_VelocityQ24Fdb = 0;
int32_t 	g_VelocityQ24Fdb_Acc = 0;
int32_t 	g_VelocityQ24Fil = 0;//1ms LPF
int32_t 	g_VelocityQ24_Arv1 = 0;
int32_t 	g_VelocityQ24_Zero = 0;
int32_t		g_TorqueQ24_Arv1 = 0;
int32_t		g_TorqueArv1_Time = 0;//
int32_t		g_TorqueLock_Time = 0;//
int32_t		g_Velocity10msFilQ24 = 0;
int32_t 	g_Velocity10times = 0;	//Display 10 Times Zoom
uint32_t    g_VelocityKp;
uint32_t    g_VelocityKi;
uint16_t 	g_VeloFBF_Type;

int64_t 	g_PulInCan = 0;
int32_t		g_SpdInCan = 0;
int32_t		g_IqInCan = 0;
int32_t		g_SendcntCan = 0;
int32_t		g_CmdInCan = 0;

int64_t 	g_PulInAllFWD;
int64_t		g_PulInAllCount;
int64_t		g_PulInAllCountOld;
//int64_t		g_PulInAllCountErr;
int64_t		g_PulInAllCount_Filter;
int64_t		g_EncPulAllCount;
int64_t 	g_PosiFeedback;
int64_t 	g_SHome_Finish_g_EncPulAllCount;
int64_t		g_EncPulAllCountOld;
int32_t 	g_EncDirChgCount;

uint32_t	g_PulInPrdCCWCount;		//Pulse Input Dir CCW
uint32_t	g_PulInPrdCWCount;		//Pulse Input Dir CW
//uint32_t 	g_PulInCCWTimer;		//Pulse Input CCW Timer
//uint32_t 	g_PulInCWTimer;			//Pulse Input CW Timer
int32_t 	g_PulInPosition = 0; //Pulse Input Position Ref
int32_t 	g_PulInPosition_Old = 0; //Pulse Input Position Ref
int32_t 	g_PulInPosi_Spd_Acc = 0; //Pulse Input Position Ref
int32_t  	g_PulInPosi_Spd_Acc_K = 0;
uint16_t 	g_PulInPOSCountOld = 0;//Pulse Input Count before 100us
uint16_t 	g_PulInPOSCount = 0;	//Pulse Input Count
int16_t   	g_PulInPOSDelta = 0;	//Pulse Input Delta Count  Between now and 100us Forward
uint32_t 	g_PulInInnerRef = 0;	//Inner Pulse Input Ref
//int32_t 	g_PulInTarget = 0;//Inner Pulse Input Target
int32_t   	g_PosCmd = 0;	//Position Command
int32_t   	g_PosCmdFil = 0;//Position Command After Delay Filter

int32_t 	g_PosDevtReg = 0;//Position Deviation Register
int32_t 	g_PosDevtRegCount = 0;//Position Deviation Pulse Count(Total Deviation)
int32_t 	g_VelocityRefQ24 = 0;//Complex Value, include Position Mode and Velocity Mode
int32_t		g_VelocityMaxQ24 = 0;
int32_t		g_VelocityMaxQ24_17single = 0;
//int32_t 	g_VelocityRefQ24_Acc = 0;
int32_t 	g_VeloRefQ24 = 0;//Only in the Velocity Mode
int32_t 	g_VeloTargetQ24 = 0;//Velocity Target Value
int32_t 	g_VeloTargetInitQ24 = 0;//Velocity Value Before Changed
int32_t 	g_CurrentRefQ24 = 0;//0~2^24 <=> 0~ Motor Rated Current
int32_t		g_CurrentRefQ24_InitialAngle = 0;
int32_t 	g_CurRefQ24 = 0;//External Set Value
int32_t 	g_CurRefQ24_Foc = 0;//External Set Value
int32_t 	g_CurInVeloQ24 = 0;//Velocity Value in Torque Control Mode
int32_t 	g_CurInVelocityQ24 = 0;//Velocity Value in Torque Control Mode
int32_t 	g_IrmsCalc = 0;
int32_t 	g_Irms_Cmd = 0;
int32_t 	g_Irms_Cmd_A = 0;
int32_t 	g_Vrms_Cmd = 0;
int32_t 	g_VrmsCalc = 0;
int32_t		g_PrmsCalc=0;

int32_t 	g_Volt_L = 0;
int32_t 	g_Curr_L = 0;

int64_t 	g_filterVq = 8088608;
int64_t 	g_filterVd = 8088608;

uint32_t 	g_PwmPeriod = 0;//Pwm Period Reg

int32_t 	g_VeloACCStepValueQ24 = 0;//Velocity Acc Step Value per 1ms
int32_t 	g_VeloDECStepValueQ24 = 0;//Velocity Dec Step Value per 1ms

uint32_t    g_LdValueQ24=0x100000;//Ld�ı�������
uint32_t    g_LqValueQ24=0X100000;//Lq�ı�����;
uint32_t    g_FluxfQ24=0XF00000;//Fluxf�ı�����
uint32_t    g_PulsesPerRevolution=1677;
uint32_t    g_VelocityKpCalc;
uint32_t    g_VelocityKpAdjust = 0x1000000;

uint32_t    g_OverLoadFactor=0x2000000;
uint64_t    g_index_counter=0;
uint16_t    timer_count = 0x0;
uint16_t    g_timer_count = 0;
uint16_t    g_timer_flag=0;
uint8_t     run_ready = 0;
int32_t     g_out_counter = 0;
int32_t     g_out_counter1 =0;
int32_t     g_JogRefQ24 =0;
int32_t     g_InertiaRefQ24=0;//ת��ģʽ
uint64_t    g_InertiaQ24=0;//���巽ʽ��ʾ
uint8_t     g_JogRunDw=0;
uint8_t     g_JogRunUp=0;
uint8_t     g_JogRunUpButton=0;
uint8_t     g_JogRunDwButton=0;
int32_t     g_JogTargetQ24=0;
int32_t     g_JogACCStepValueQ24=0;
int32_t     g_JogDECStepValueQ24=0;
int32_t     g_OutputPulses=0;//��������������ƽ
int32_t     g_OutputDir=0;//��������ķ����ƽ
int32_t     g_MotorCurGain=0x1000000;
int32_t     g_MotorCurGain_1=0x1000000;//by mxj
uint8_t     g_ZeroSpdMode=0;
uint8_t     g_ZeroSpdLockMode=0;
uint16_t    g_ABErrCount=0;
int32_t     g_acciq[128]={0};
int32_t     g_deciq[128]={0};
int32_t     g_InertiaGain=0x1000000;
int32_t     g_InertiaTotal=0;

uint32_t	g_MotorInitAngleQ24 = 0;

//uint8_t 	AlmRecord_Time = 0;

FORCEDIONEN         ForceDIOnEn={0};//ONʹ��
FORCEDIOFFEN        ForceDIOffEn={0};//OFFʹ��
uint32_t            g_DIForceOn=0;
uint32_t            g_DIForceOff=0;


FORCEDOONEN         ForceDOOnEn={0};//ONʹ��
FORCEDOOFFEN        ForceDOOffEn={0};//OFFʹ��


/* ���綯�ƹ۲���*/
int32_t     g_canview1=0;
int32_t     g_canview2=0;
int32_t     g_canview3=0;
int32_t     g_canview4=0;
int32_t     g_canview5=0;
int32_t     g_canview6=0;
int32_t     g_canview7=0;
int32_t     g_canview8=0;
int32_t     g_canview9=0;
int32_t     g_canview10=0;
int32_t     g_canview11=0;
int32_t     g_canview12=0;
int32_t     g_canview13=0;
int32_t     g_canview14=0;
int32_t     g_canview15=0;
int32_t     g_canview16=0;
int32_t     g_canview17=0;
int32_t     g_canview18=0;
int32_t     g_canview19=0;

int32_t     g_canview20=0;
int32_t     g_canview21=0;

int32_t     g_canview22=0;
int32_t     g_canview23=0;
int32_t     g_canview24=0;
int32_t     g_canview25=0;
int32_t     g_canview26=0;
int32_t     g_canview27=0;
int32_t     g_canview28=0;
int32_t     g_canview29=0;
int32_t     g_canview30=0;
int32_t     g_canview31=0;
int32_t     g_canview32=0;
int32_t     g_canview33=0;
int32_t     g_canview34=0;

/******Function Variable Define*****/
uint16_t 	g_CtrlMode = 0;	//From External Set
uint16_t 	g_UltimateCtrlMode = 0; //Ultimate Control Mode: 0:position;1:velocity;2:Current
uint32_t   	g_RunTime = 0;//Run Time
uint32_t 	g_PowerUpTime = 0;//Power Time
uint32_t 	g_ComCtrlCMD = 0;//Communication Control Command
int32_t 	g_DIsts = 0;
int32_t 	g_DIAddVir_sts = 0;
int32_t 	g_DOsts = 0;
int32_t 	g_DIDOsts = 0;

int16_t		g_LEDErrDisplayMode=0;

/************AO Value**************/
int32_t 	g_PosiCMDSpeedQ24 = 0;	// Position Command Speed
int32_t 	g_PosiCMDDev = 0;//Position Command Deviation
int32_t 	g_EncCMDDev = 0;//Encoder Command Deviation

/**********Adc Handle*************/
//ADC001_ResultHandleType Result[11];

uint16_t AdcResult[15];//instruction:  0=>VADC.G0CH0(14.0/Iu); 1=>VADC.G0CH1(14.1/Iv); 2=>VADC.G0CH2; 3=>VADC.G0CH4(14.4/Tem)
					   //4=>VADC.G0CH5(14.5/AI1); 5=>VADC.G0CH6(14.6/AI2); 6=>VADC.G0CH7(14.7/AI3); 7=>VADC.G1CH4(14.12/Reference voltage1);
						//8=>VADC.G1CH5(14.13/Reference voltage2);9=>VADC.G1CH6(14.14/PTC); 10=>VADC.G3CH0(15.8/Vdc)
/*****AD-Sample Variable *****************/
int64_t 	AD_Test = 0;
//electrical current sample
uint32_t 	g_IuAD=0;	// AD sample value
uint32_t 	g_IvAD=0;
uint32_t 	g_IwAD=0;

int32_t 	g_Iu100usQ24=0; //100us Instantaneous value
int32_t 	g_Iv100usQ24=0;
int32_t 	g_Iw100usQ24=0;

int32_t	 	g_Iu100usFilQ24=0; // value after LPF
int32_t 	g_Iv100usFilQ24=0;
int32_t 	g_Iw100usFilQ24=0;

int32_t 	g_Iu1msFilQ24=0; // value after LPF
int32_t 	g_Iv1msFilQ24=0;
int32_t 	g_Iw1msFilQ24=0;


int32_t 	g_Iu10msFilQ24=0; // value after LPF
int32_t 	g_Iv10msFilQ24=0;
int32_t 	g_Iw10msFilQ24=0;

uint32_t 	g_Irms1msFilQ24=0;//Irms1ms is used to OL&OC
uint32_t 	g_Irms10msFilQ24=0;//

uint32_t 	g_IrmsAvgFilZoom10=0;//Irms average filter and be zoomed 10

//bus voltage sample 800V = IQ24(1)
uint16_t 	g_PowerVbusAD=0;
uint16_t 	g_CtrlVbusAD=0;

int32_t 	g_PowerVbus100usQ24=0;
uint32_t 	g_CtrlVbus100usQ24=0;

int32_t 	g_PowerVbus1msFilQ24=0;
//int32_t 	g_PowerVbus1msFilQ24_PU=16777216;
int32_t		g_PowerVbus100usQ24_PU =16777216;
int32_t 	g_CtrlVbus1msFilQ24=0;

int32_t 	g_PowerVbus10msFilQ24=0;
int32_t 	g_CtrlVbus10msFilQ24=0;

int32_t 	g_PowerVbusAvgFil=0;
int32_t 	g_CtrlVbusAvgFil=0;

//Temperature Sample
uint16_t 	g_TempAD=0;
int32_t 	g_Temp100usQ24 = 0;
int32_t 	g_Temp10msFilQ24=0;
int32_t 	g_TempQ24=0;
int32_t 	g_TempAvg=0;

//Motor Temperature Sample
uint16_t 	g_PTCAD = 0;
int32_t		g_PTC100usQ24 = 0;
int32_t 	g_PTC10msFilQ24=0;
int32_t 	g_PTCAvg=0;

//Analog Input Sample
uint16_t 	g_AI1AD = 0;
int32_t		g_AI1Q24 = 0;
int32_t 	g_AI1FilQ24 = 0;

uint16_t 	g_AI2AD = 0;
int32_t		g_AI2Q24 = 0;
int32_t 	g_AI2FilQ24 = 0;

uint16_t 	g_AI3AD = 0;
int32_t		g_AI3Q24 = 0;
int32_t 	g_AI3FilQ24 = 0;

int32_t 	g_165AD = 0;
int32_t 	g_141AD = 0;

int32_t 	g_ComPulse = 0;
int32_t 	g_ComSpeed = 0;
int32_t 	g_ComTorque = 0;
int32_t 	g_ComPidRef = 0;
int32_t 	g_ComAO1 = 0;
int32_t 	g_ComAO2 = 0;
int32_t 	g_ComDO = 0;
int32_t 	g_ComCmdModel = 0;
uint32_t 	g_FcodeChangeFlag = 0;

int32_t     g_ABSThetaQ24=0;
uint16_t    g_ABSCycNum=0;

int32_t g_FLAG=0;
int32_t g_FLAG1=0;
int32_t g_FLAG2=0;

int64_t  gEncPosi=0;
int64_t gPosiHome=0;

SHomeFlg SHomeFlag={0}; //ԭ��ع��־λ

SHomePara SHomeParameter;

SeriEnc SeriEncoder;

int32_t g_VpnVoltGain;
int32_t g_RelayrunQ24;
int32_t g_RelaystopQ24;

int32_t g_EasyPLCPosiAllCounts_old = 0;

int32_t g_CurrentRefAddLim;
uint8_t g_tempUVW;


uint32_t  g_SpecialSave=0;
uint16_t g_AlmNum = 0;		//Alarm Number








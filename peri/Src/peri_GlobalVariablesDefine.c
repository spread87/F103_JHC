/*
 * peri_GlobalVariablesDefine.c
 *
 * Created on: 2019-12-23
 * Author: JH-LvSL
 */

#include <main.h>
#include <peri_GlobalVariablesExtern.h>

FuncCode FuncCode_Handle; // __attribute__((section("FUNCTION_RAM")));		//Function Code define and handle
CoreStatusFlg CoreStatusFlag;
AlarmStatusFlg AlarmStatusFlag;
WarnStatusFlg WarnStatusFlag;

DICtrl DICtrlFlg;
DICtrl VirtualDICtrlFlg;
DICtrl TerminalDICtrlFlg;
DOCtrl DOCtrlFlg;
DOActive DOActiveSet; //20170329  mxj
DOFunc DOFuncSet; //20170329  mxj
AnalogInput ExAI;/*External Analog Input*/

DriverParameter DriverPara;
SystemTimeSequ SysTimeSequ;
ComCMDFlg ComCtrlFlg;
COMVirlDI COMVirtualDI = { 0 };	//Communication Virtual DI Value
INERTIA_STATUS Inertia_status = INERTIA_DEFAULTS;
/********bmef_cal****************/

uint16_t Timer1msCount = 0;
uint16_t SWI_Tick = 0;

/*****Core Variable Define********/
uint32_t g_EncSpeed1msFilQ16 = 0;	//LPF(1ms)

uint32_t g_PositionCount = 0;	//Encoder Position CYL170306  增加串行编码器 uint16_t->uint32_t
int32_t g_RevolutionCount = 0;	//
uint32_t g_EleThetaQ24 = 0;	//Electrical Theta:0~2^24 <=> 0~2PI
int32_t g_Uvw_Theta;
uint32_t g_EncoderUVWQ24 = 0;	//
uint32_t g_EncoderZcheck = 0;
int32_t g_VelocityQ24abs = 0;
int32_t g_VelocityQ24 = 0;	//0~2^24 <=> 0~ Motor Rated Velocity
int32_t g_VelocityQ24_1 = 0;
int32_t g_VelocityQ24Fdb = 0;
int32_t g_VelocityQ24Fdb_Acc = 0;
int32_t g_VelocityQ24Fil = 0;	//1ms LPF
int32_t g_VelocityQ24_Arv1 = 0;
int32_t g_VelocityQ24_Zero = 0;
int32_t g_Velocity10msFilQ24 = 0;
int32_t g_Velocity10times = 0;	//Display 10 Times Zoom

int64_t g_PulInAllCount;
int64_t g_EncPulAllCount;
int64_t g_PosiFeedback;
int32_t g_PulInPosition = 0; //Pulse Input Position Ref
int32_t g_PosDevtRegCount = 0; //Position Deviation Pulse Count(Total Deviation)
int32_t g_VelocityRefQ24 = 0; //Complex Value, include Position Mode and Velocity Mode
int32_t g_VelocityMaxQ24 = 0;
int32_t g_VeloRefQ24 = 0; //Only in the Velocity Mode
int32_t g_VeloTargetQ24 = 0; //Velocity Target Value
int32_t 	g_VeloTargetInitQ24 = 0;//Velocity Value Before Changed
int32_t 	g_CurrentRefQ24 = 0;//0~2^24 <=> 0~ Motor Rated Current
int32_t		g_CurrentRefQ24_InitialAngle = 0;
int32_t 	g_CurRefQ24 = 0;//External Set Value
int32_t 	g_CurRefQ24_Foc = 0;//External Set Value
int32_t 	g_CurInVeloQ24 = 0;//Velocity Value in Torque Control Mode
int32_t g_CurInVelocityQ24 = 0; //Velocity Value in Torque Control Mode
int32_t g_IrmsCalc = 0;
int32_t g_VrmsCalc = 0;

uint32_t g_LdValueQ24 = 0x100000; //Ld的标幺量；
uint32_t g_LqValueQ24 = 0X100000; //Lq的标幺量;
uint32_t g_FluxfQ24 = 0XF00000; //Fluxf的标幺量

uint64_t g_index_counter = 0;
uint16_t timer_count = 0x0;
uint16_t g_timer_count = 0;
uint16_t g_timer_flag = 0;
int32_t g_out_counter = 0;
int32_t g_JogRefQ24 = 0;
int32_t g_InertiaRefQ24 = 0; //转速模式
uint64_t g_InertiaQ24 = 0; //脉冲方式表示
uint8_t g_JogRunDw = 0;
uint8_t g_JogRunUp = 0;
uint8_t g_JogRunUpButton = 0;
uint8_t g_JogRunDwButton = 0;
int32_t g_MotorCurGain = 0x1000000;
int32_t g_MotorCurGain_1 = 0x1000000; //by mxj
uint16_t g_ABErrCount = 0;
int32_t g_InertiaTotal = 0;

uint32_t g_MotorInitAngleQ24 = 0;

//uint8_t 	AlmRecord_Time = 0;

FORCEDIONEN ForceDIOnEn = { 0 }; //ON使能
FORCEDIOFFEN ForceDIOffEn = { 0 }; //OFF使能
uint32_t g_DIForceOn = 0;
uint32_t g_DIForceOff = 0;
FORCEDOONEN ForceDOOnEn = { 0 }; //ON使能
FORCEDOOFFEN ForceDOOffEn = { 0 }; //OFF使能

/* 反电动势观测量*/
int32_t g_canview1 = 0;
int32_t g_canview2 = 0;
int32_t g_canview3 = 0;
int32_t g_canview4 = 0;
int32_t g_canview5 = 0;
int32_t g_canview6 = 0;
int32_t g_canview7 = 0;
int32_t g_canview8 = 0;
int32_t g_canview9 = 0;
int32_t g_canview10 = 0;
int32_t g_canview11 = 0;
int32_t g_canview12 = 0;
int32_t g_canview13 = 0;
int32_t g_canview14 = 0;
int32_t g_canview15 = 0;
int32_t g_canview16 = 0;
int32_t g_canview17 = 0;
int32_t g_canview18 = 0;
int32_t g_canview19 = 0;
int32_t g_canview20 = 0;
int32_t g_canview21 = 0;
int32_t g_canview22 = 0;
int32_t g_canview23 = 0;
int32_t g_canview24 = 0;
int32_t g_canview25 = 0;
int32_t g_canview26 = 0;
int32_t g_canview27 = 0;
int32_t g_canview28 = 0;
int32_t g_canview29 = 0;
int32_t g_canview30 = 0;
int32_t g_canview31 = 0;
int32_t g_canview32 = 0;
int32_t g_canview33 = 0;
int32_t g_canview34 = 0;

/******Function Variable Define*****/
uint16_t g_CtrlMode = 0;	//From External Set
uint16_t g_UltimateCtrlMode = 0; //Ultimate Control Mode: 0:position;1:velocity;2:Current
uint32_t g_RunTime = 0; //Run Time
uint32_t g_PowerUpTime = 0; //Power Time
uint32_t g_ComCtrlCMD = 0; //Communication Control Command
int32_t g_DIsts = 0;
int32_t g_DIAddVir_sts = 0;
int32_t g_DOsts = 0;
int32_t g_DIDOsts = 0;

int16_t g_LEDErrDisplayMode = 0;

/************AO Value**************/
int32_t g_PosiCMDSpeedQ24 = 0;	// Position Command Speed
int32_t g_PosiCMDDev = 0;	//Position Command Deviation
int32_t g_EncCMDDev = 0;	//Encoder Command Deviation

int32_t	 	g_Iu100usFilQ24=0; // value after LPF
int32_t 	g_Iv100usFilQ24=0;
int32_t 	g_Iw100usFilQ24=0;
//bus voltage sample 800V = IQ24(1)
uint16_t g_PowerVbusAD = 0;
uint16_t g_CtrlVbusAD = 0;

int32_t g_PowerVbus100usQ24 = 0;
uint32_t g_CtrlVbus100usQ24 = 0;

int32_t g_PowerVbus1msFilQ24 = 0;
int32_t g_PowerVbus100usQ24_PU = 16777216;
int32_t g_CtrlVbus1msFilQ24 = 0;

int32_t g_PowerVbus10msFilQ24 = 0;
int32_t g_CtrlVbus10msFilQ24 = 0;

int32_t g_PowerVbusAvgFil = 0;
int32_t g_CtrlVbusAvgFil = 0;

//Temperature Sample
uint16_t g_TempAD = 0;
int32_t g_Temp100usQ24 = 0;
int32_t g_Temp10msFilQ24 = 0;
int32_t g_TempQ24 = 0;
int32_t g_TempAvg = 0;

//Motor Temperature Sample
uint16_t g_PTCAD = 0;
int32_t g_PTC100usQ24 = 0;
int32_t g_PTC10msFilQ24 = 0;
int32_t g_PTCAvg = 0;

//Analog Input Sample
uint16_t g_AI1AD = 0;
int32_t g_AI1Q24 = 0;
int32_t g_AI1FilQ24 = 0;

uint16_t g_AI2AD = 0;
int32_t g_AI2Q24 = 0;
int32_t g_AI2FilQ24 = 0;

uint16_t g_AI3AD = 0;
int32_t g_AI3Q24 = 0;
int32_t g_AI3FilQ24 = 0;

int32_t g_165AD = 0;
int32_t g_141AD = 0;

int32_t g_ComPulse = 0;
int32_t g_ComSpeed = 0;
int32_t g_ComTorque = 0;
int32_t g_ComPidRef = 0;
int32_t g_ComAO1 = 0;
int32_t g_ComAO2 = 0;
int32_t g_ComDO = 0;
int32_t g_ComCmdModel = 0;
uint32_t g_FcodeChangeFlag = 0;

SHomeFlg SHomeFlag = { 0 }; //原点回归标志位
SHomePara SHomeParameter;

SeriEnc SeriEncoder;
int32_t g_VpnVoltGain;
uint8_t g_tempUVW;

/*
 * core_StructDef.h
 *
 *  Created on: 2014-2-14
 *  Author: JH-LvSL
 */

#ifndef CORE_STRUCTDEF_H_
#define CORE_STRUCTDEF_H_

struct CoreStatusFlgBits//CORE_STATUS_FLAG_BITS
{
	uint64_t  	RUN_OK_F				: 1;   	/*bit[0] Run Flag: 0 = no run;1 = run flag */
	uint64_t  	ERR_ALL_STOP_F  		: 1;   	/*bit[1] Error Flag: 0 = no err;1 = err */
	uint64_t	RELAY_OK_F				: 1;   	/*bit[2] Relay Detect*/
	uint64_t	ZPHASE_F				: 1;		/*bit[3] Z Phase detection 0:Never Detected;1:Detected*/
	uint64_t  	RUN_DIR_F				: 1;		/*bit[4] Motor Run Direction,1:FWD/0:REV*/
	uint64_t	ACC_SPD_F				: 1;		/*bit[5] Accelerate Status*/
	uint64_t	DEC_SPD_F				: 1;		/*bit[6] Decelerate Status*/
	uint64_t	CST_SPD_F				: 1;		/*bit[7] Constant Status*/
	uint64_t   	EX_BRAKE_F				: 1;		/*bit[8] External Brake Signal : 0:Open 1:Close*/
	uint64_t	TLIMIT_F				: 1;		/*bit[9] Torque Limit Status */
	uint32_t	SPDLIMIT_F				: 1;		/*bit[10] Speed Limit Status */
	uint64_t  	ZSPD_F					: 1;		/*bit[11] Zero Speed Status*/
	uint64_t 	SHomeSUCC_F				: 1;		/*bit[12]:原点回归成功完成20190403*/
	uint64_t    RUN0_OK_F               : 1;    	/*bit[13]伺服使能信号，与run信号有区别，下降沿与serve_on相同，较run_ok_f信号提前*/
	uint64_t    MOTOR_BREAK_F           : 1;		//bit[14]
	uint64_t    WARN_ALL_F              : 1;        /*bit[15]:警告标志位，用于出现警告时，不需要停机的情况*/
	uint64_t	AlmNum					: 8;        /*bit[16+0-7]:故障码 读取*/  //20180705
	uint64_t    EEPROMINIT_OK_F        	: 1;        /*bit[16+8]:EEPROM Init OK Flag*/
	uint64_t    RESVD2              	: 1;        /*bit[16+9]:Reserved*/
	uint64_t    RESVD3              	: 1;        /*bit[16+10]:Reserved*/
	uint64_t    RESVD4              	: 1;        /*bit[16+11]:Reserved*/
	uint64_t    RESVD5              	: 1;        /*bit[16+12]:Reserved*/
	uint64_t    RESVD6              	: 1;        /*bit[16+13]:Reserved*/
	uint64_t    RESVD7              	: 1;        /*bit[16+14]:Reserved*/
	uint64_t    RESVD8              	: 1;        /*bit[16+15]:Reserved*/
	uint64_t    WAVE_DATA_F             : 1;        /*bit[32+0]:485波形读取标识*/  //20180622
	uint64_t    WAVE_DATA_S             : 1;        /*bit[32+1]:485波形更新标识*/  //20180622
	uint64_t	RESVD9					: 5;        /*bit[32+2-6]*/  //
	uint64_t    RESVD10            		: 1;        /*bit[32+7]:Reserved*/
	uint64_t    RESVD11            		: 1;        /*bit[32+8]:Reserved*/
	uint64_t    RESVD12              	: 1;        /*bit[32+9]:Reserved*/
	uint64_t    RESVD13              	: 1;        /*bit[32+10]:Reserved*/
	uint64_t    RESVD14              	: 1;        /*bit[32+11]:Reserved*/
	uint64_t    RESVD15              	: 1;        /*bit[32+12]:Reserved*/
	uint64_t    RESVD16              	: 1;        /*bit[32+13]:Reserved*/
	uint64_t    RESVD17              	: 1;        /*bit[32+14]:Reserved*/
	uint64_t    RESVD18              	: 1;        /*bit[32+15]:Reserved*/
};

typedef union
{
	uint64_t  all;//20181229
    struct  CoreStatusFlgBits   bit;
}CoreStatusFlg;//CORE_STATUS_FLG;

typedef struct
{
	uint32_t RatedCur;
	uint32_t RatedVol ;
	uint32_t RatedPow;
}DriverParameter;

typedef struct
{
	uint32_t RatedVol;
	uint32_t RatedCur;
	uint32_t RatedPow;
	uint32_t InputVolt;
	uint32_t Volt_L;
	uint32_t Volt_H;
	uint32_t Curr_L;
	uint32_t Curr_H;
	uint32_t Volt_DF;
	uint32_t Curr_DF;
	uint32_t Curr_LCMD;
}ChargerParameter;

typedef struct
{
	int32_t Dir;
	int32_t PositionOld;
	int32_t Position;
	int32_t MThelta;
	int32_t EThelta;
	int32_t Velocity;
}MotorSts;



typedef struct
{
	int32_t Iu;
	int32_t Iv;
	int32_t Iw;
}CURRENT;

typedef struct
{
	int32_t sin;
	int32_t cos;
}AngleFunc;

typedef struct
{
	int32_t Vu;
	int32_t Vv;
	int32_t Vw;
}PwmGenerate;

typedef struct
{
	uint64_t 	ServoOn			: 1;  /* 1 :Servo On Input*/
	uint64_t 	POT				: 1;  /* 2 :Positive direction over-travel inhibition input*/
	uint64_t   	NOT				: 1;  /* 3 :Negative direction over-travel inhibition input*/
	uint64_t   	AlmClr			: 1;  /* 4: Alarm clear input*/
	uint64_t  	CtrlMode		: 1;  /* 5:Control mode switching input*/
	uint64_t 	Gain 			: 1;  /* 6:Gain switching input*/
	uint64_t	CL				: 1;  /* 7:Deviation counter clear input*/
	uint64_t  	INH				: 1;	/* 8:Command pulse inhibition input*/
	uint64_t	TLS				: 1; 	/* 9:Torque limit switching input*/
	uint64_t	ZeroSPD			: 1;	/* 10:Speed zero clamp input*/
	uint64_t 	EStop			: 1;	/* 11:Emergency stop*/
	uint64_t  	DIV0			: 1;	/* 12:Electronic gear(division/multiplication) switching input0*/
	uint64_t  	DIV1			: 1;	/* 13:Electronic gear(division/multiplication) switching input1*/
	uint64_t 	INTSPD0			: 1;	/* 14:Selection input of internal command speed0*/
	uint64_t 	INTSPD1			: 1;	/* 15:Selection input of internal command speed1*/
	uint64_t 	INTSPD2			: 1;	/* 16:Selection input of internal command speed2*/
	uint64_t 	INTSPD3			: 1;	/* 17:Selection input of internal command speed3*/
	uint64_t  	INTPOS0			: 1;	/* 18:Selection input of internal command position0*/
	uint64_t  	INTPOS1			: 1;	/* 19:Selection input of internal command position1*/
	uint64_t  	INTPOS2			: 1;	/* 20:Selection input of internal command position2*/
	uint64_t  	INTPOS3			: 1;	/* 21:Selection input of internal command position3*/
	uint64_t	SOR				: 1;	/* 22:Start Origin homing mode*/
	uint64_t	SORPOS			: 1;	/* 23:Ensure Origin homing mode position*/
	uint64_t	PJOG			: 1;	/* 24:Positive JOG*/
	uint64_t 	NJOG			: 1;	/* 25:Negative JOG*/
	uint64_t	MTRDIR			: 1;	/* 26:Motor rotation direction*/
	uint64_t    INERSWH1        : 1;    //27 惯量切换1
	uint64_t    INERSWH2        : 1;    //28 惯量切换2
	uint64_t    INERSWH3        : 1;    //29 惯量切换3
	uint64_t    ECAMEN          : 1;    //30 电子凸轮使能
	uint64_t    ECAMSEL0        : 1;    //31 电子凸轮曲线选择0
	uint64_t    ECAMSEL1        : 1;    //32 电子凸轮曲线选择1
	uint64_t    ECAMSEL2        : 1;    //33 电子凸轮曲线选择2
	uint64_t    ECUTFISH        : 1;    //34 追剪剪切完成
	uint64_t    ECAMRSVD35      : 1;    //35 电子凸轮预留
	uint64_t    ECAMRSVD36      : 1;    //36 电子凸轮预留
	uint64_t    ECAMRSVD37      : 1;    //37 电子凸轮预留
	uint64_t    ECAMRSVD38      : 1;    //38 电子凸轮预留
	uint64_t    ECAMRSVD39      : 1;    //39 电子凸轮预留
	uint64_t    ECAMRSVD40      : 1;    //40 电子凸轮预留
	uint64_t	PullInCh1		: 1;	/* 41:Pulse input direction change 1*/
	uint64_t	PullInCh2		: 1;	/* 42:Pulse input direction change 2*/
	uint64_t	PullInCh3		: 1;	/* 43:Pulse input direction change 3*/
	uint64_t	PullInSwth		: 1;	/* 44:Pulse input HL speed switch*/
	uint64_t	Rsvd45			: 1;	/* 45:Pulse input HL speed switch*/
	uint64_t	Rsvd46			: 1;	/* 46:Pulse input HL speed switch*/
	uint64_t	Stall			: 1;	/* 47:Pulse input HL speed switch*/
	uint64_t	Postouchof1		: 1;	/* 48:Pulse input HL speed switch*/
	uint64_t	Postouchof2		: 1;	/* 49:Pulse input HL speed switch*/
	uint64_t	Postouchof3		: 1;	/* 50:Pulse input HL speed switch*/
	uint64_t	Postouchof4		: 1;	/* 51:Pulse input HL speed switch*/
	uint64_t	Spdtouchof1		: 1;	/* 52:Pulse input HL speed switch*/
	uint64_t	Spdtouchof2		: 1;	/* 53:Pulse input HL speed switch*/
	uint64_t	Spdtouchof3		: 1;	/* 54:Pulse input HL speed switch*/
	uint64_t	Spdtouchof4		: 1;	/* 55:Pulse input HL speed switch*/
	uint64_t	CL_R			: 1;  	/* 56:Deviation counter clear input Rise active*/
	uint64_t	CL_F			: 1;  	/* 57:Deviation counter clear input Fall active*/
}DICtrlFLG;

typedef union
{
	uint64_t all;
	DICtrlFLG bit;
}DICtrl;

typedef struct
{
	uint32_t ServoAlm		: 1; /*Servo Alarm Output*/
	uint32_t ServoRdy		: 1; /*Servo Ready Output*/
	uint32_t ExBrake		: 1; /*External Brake Release Signal*/
	uint32_t PosiOK			: 1; /*Position Complete*/
	uint32_t SPDArvl		: 1; /*Speed Arrival Output*/
	uint32_t TLimit			: 1; /*Torque in-limit Signal Output*/
	uint32_t ZSPD			: 1; /*Zero-Speed Status Output*/
	uint32_t SPDCO			: 1; /*Speed Coincidence Output*/
	uint32_t SPDLimit		: 1; /*Speed in-limit Signal Output*/
	uint32_t PosiArvl		: 1; /*Position Arrival  Output*/
	uint32_t AlmOut1		: 1; /*Alarm Output1*/
	uint32_t AlmOut2		: 1; /*Alarm Output2*/
	uint32_t ORZ			: 1; /*Origin Homing Mode Complete*/
	uint32_t Run            :1;
	uint32_t MotorBreak     :1;
	uint32_t Warnning      	:1;
	uint32_t Pulses         :1;//脉冲输出脉冲电平
	uint32_t Dir            :1;//脉冲输出方向电平
	uint64_t TorqArvl       : 1; //20180904 转矩到达输出
}DOCtrlFLAG;

typedef union
{
	uint32_t 	all;
	DOCtrlFLAG 	bit;
}DOCtrl;

typedef struct
{
	int32_t Positive;		/* Torque Positive Limit */
	int32_t Negative;	/*	Torque Negative Limit*/
}TqLimit;

typedef struct
{
	int32_t AI1Q24;
	int32_t AI2Q24;
	int32_t AI3Q24;
}AnalogInput;

typedef struct
{
	uint32_t Start_Flg			:1;
	uint32_t Start_Flg1         :1;
	uint32_t Sec10_Flg			:1;
	uint32_t Sec20_Flg          :1;
}SystemTimeSequ;

typedef struct
{
	int32_t AlmRcdFlg;
	int32_t AlmRcdStgFlg;	//Alarm Record Storage Flag
	int32_t AlmRefreshFlg;
	int32_t AlmNum;
	int32_t AlmVbus;
	int32_t AlmIrms;
	int32_t AlmIu;
	int32_t AlmIv;
	int32_t AlmVelo;
	int32_t AlmIOSts;
	int32_t AlmTemp;
	int32_t AlmCoreFlg;
	int32_t MaxTemp;
	int32_t MaxVbus;
	int32_t MaxIrms;
	int32_t MaxVelo;
	int32_t	FA41;
	int32_t	FA42;
}AlmRcd;//Alarm Record

typedef struct
{
	uint32_t RunFlg			:1;
	uint32_t DirFlg			:1;
	uint32_t ResetFlg		:1;
	uint32_t ClrAlmFlg		:1;
	uint32_t JOGValidFlg	:1;
	uint32_t JOGCCWFlg		:1;
	uint32_t JOGCWFlg		:1;
	uint32_t SHomeEnFlg		:1;  //bit7 原点回归使能
}ComCMDFlag;

typedef union
{
	uint32_t all;
	ComCMDFlag bit;
}ComCMDFlg;


typedef struct
{
	uint32_t DI1;
	uint32_t DI2;
	uint32_t DI3;
	uint32_t DI4;
	uint32_t DI5;
	uint32_t DI6;
	uint32_t DI7;
	uint32_t DI8;
	uint32_t DI9;
}COMVirlDI;
typedef struct
{
	uint32_t DO1;
	uint32_t DO2;
	uint32_t DO3;
	uint32_t DO4;
	uint32_t DO5;
	uint32_t DO6;
}COMVirlDO;

typedef struct//mxj 20170329
{
	uint32_t DO1:1;
	uint32_t DO2:1;
	uint32_t DO3:1;
	uint32_t DO4:1;
	uint32_t DO5:1;
	uint32_t DO6:1;
}DOActive;

typedef struct//mxj 20170329
{
	uint32_t DO1:8;
	uint32_t DO2:8;
	uint32_t DO3:8;
	uint32_t DO4:8;
	uint32_t DO5:8;
	uint32_t DO6:8;
}DOFunc;

typedef struct
{
	uint32_t ON1:1;
	uint32_t ON2:1;
	uint32_t ON3:1;
	uint32_t ON4:1;
	uint32_t ON5:1;
	uint32_t ON6:1;
	uint32_t ON7:1;
	uint32_t ON8:1;
	uint32_t ON9:1;
}DIEnableFlag;

typedef union
{
	uint32_t all;
	DIEnableFlag bit;
}FORCEDIONEN;//ON 使能

typedef union
{
	uint32_t all;
	DIEnableFlag bit;
}FORCEDOONEN;//ON 使能


typedef struct
{
	uint32_t OFF1:1;
	uint32_t OFF2:1;
	uint32_t OFF3:1;
	uint32_t OFF4:1;
	uint32_t OFF5:1;
	uint32_t OFF6:1;
	uint32_t OFF7:1;
	uint32_t OFF8:1;
	uint32_t OFF9:1;
}DIEnableFag;

typedef union
{
	uint32_t all;
	DIEnableFag bit;
}FORCEDIOFFEN;//OFF 使能

typedef union
{
	uint32_t all;
	DIEnableFag bit;
}FORCEDOOFFEN;//OFF 使能

//==========================20161128 CYL==============================================
struct SHomeFlgBits
{
	uint16_t  	SHomeCMD_F			: 1;   	/*bit[0] 原点复归开始命令标志 */
	uint16_t  	SHomeSTRT_F  	    : 1;   	/*bit[1] 原点复归开始标志位 */
	uint16_t	SHomeSUCC_F			: 1;   	/*bit[2] 原点复归成功标志位*/
	uint16_t  	SHomeFISH_F			: 1;	/*bit[3] 原点复归完成标志位*/
	uint16_t	SHomeZFound_F		: 1;	/*bit[4] 原点复归过程中发生Z中断标志位*/
	uint16_t	SHomeFindZCmd_F		: 1;	/*bit[5] 原点复归过程中找Z命令字*/
	uint16_t    Reserved0           : 10;   //bit[6-15] 保留
	uint16_t    FailReason          : 4;    //bit[16-19]失败原因
	uint16_t    Process             : 4;    //bit[20-23]当前过程阶段
	uint16_t    FailProc            : 4;    //bit[24-27]失败时处于哪个阶段
	uint16_t    Reserved1           : 4;    //bit[28-31] 保留
};

typedef union
{
    uint32_t  all;
    struct  SHomeFlgBits   bit;
}SHomeFlg;//原点复归状态标志位

typedef struct
{
	int32_t  	Speed	;		  	/*速度*/
	uint8_t  	Signal	;		  	/*检测信号*/
	uint8_t  	SigState;		  	/*信号状态*/
}SHomeProc;  //原点回归参数

typedef struct
{
    uint8_t  SORPOS     :1;
    uint8_t  NOT     :1;
    uint8_t  POT  :1;
    uint8_t  STALL :1;//20190401
}PortState;

typedef union
{
    uint8_t  all;
    PortState   bit;
}PortSta;//原点复归状态标志位

typedef struct
{
	int32_t  	HSpeed	;		  	/*高速速度 Q16*/
	int32_t  	LSpeed	;		  	/*低速速度 Q16**/
	int32_t     TimeOut ;           /*超时时间*/
	int32_t     TimeCount ;         /*计时*/
	int32_t  	Speed	;		  	/*运行速度 */
	int32_t	    Acc		; 	        /*加速度*/
	int32_t     Offset;             /*原点偏移量*/
	int64_t     MotorPosiOrig;      /*启动原点回归时电机的位置*/
	int64_t     MotorPosi;          /*电机的位置*/
	int64_t     SHomePullIN;        /*脉冲指令*/
	int64_t     HomePosi;           /*原点位置*/
	int8_t      LimitMode;          /*遇到极限处理方式*/
	uint16_t    Code;               /*代码*/
	uint8_t     FindZ;              /*找Z点的方式*/
	uint8_t     ShiftBits;          /*移位数*/
	uint8_t     Process;            /*找减速点的阶段*/
	PortSta     PortStateOld;       /*端口原始状态*/
	int64_t     SHomePullCMD;       /*指令脉冲*/
	SHomeProc   SHomeProc0;         /*第一阶段*/
	SHomeProc   SHomeProc1;         /*第二阶段*/
	SHomeProc   SHomeProc2;         /*第三阶段*/
	SHomeProc   SHomeProc3;         /*第四阶段*/
	SHomeProc   SHomeProc4;         /*第五阶段*/
	//uint8_t     FailReason;         /*原点回归失败原因*/

}SHomePara;  //原点回归参数

typedef struct
{
	uint8_t ByteBeSent; //向编码器发送的字节数
	uint8_t ByteBeRead; //从编码器读取的字节数
	uint16_t Operation;    //操作功能
	int8_t  CurrentPage;  //当前EEPROM页
	int8_t  Tempture;  //当前温度
	int8_t  TempThre;    //温度保护值
	uint8_t BlanKTime;   //消隐时间
	uint8_t StatusFlg;   //状态标识位
	uint16_t MotorType;  //电机类型
	uint8_t NoDataCnt; //没有接收到数据的次数
	uint8_t ErrDataCnt; //数据错误的次数
	uint8_t ErrTypeCnt; //型号错误的次数
	uint32_t _1RevPuls; //编码器单圈位置
	int16_t CycleCnt; //圈数计数值
	uint16_t NoFRCnt;  //编码器非全精度状态计数
	uint16_t Angle;   //编码器角度值
	uint8_t WrtConf; //EEPROM写入确认
	uint8_t ErrCheckEn; //开始错误检测
	uint8_t _100msCounter;  //100ms计数
	uint8_t ReadData[11]; //由编码器回传的数据
	uint8_t EncMode;//20180911
	uint8_t WrtMtrCodeCnt;
	uint8_t WrtMtrCodeSta;
	uint8_t ClrSglPosCnt;
	uint8_t ClrSglPosSta;

	uint8_t Receive_State0;//数据接收状态
	uint8_t Receive_State1;//数据接收状态
	uint8_t Receive_State2;//数据接收状态
	uint8_t Receive_State3;//数据接收状态
}SeriEnc;


#endif /* CORE_STRUCTDEF_H_ */

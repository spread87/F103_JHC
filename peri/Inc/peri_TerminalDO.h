/*
 * peri_TerminalDO.h
 *
 * Created on: 2014-6-11
 * Author: JH-LvSL
 */

#ifndef PERI_TERMINALDO_H_
#define PERI_TERMINALDO_H_

typedef void (*TERMINALDO_FUNC)(void);

#define 		DOPOWERVOLTAGE			5452595UL   //IQ24(260/800)
#define		DOCTRLVOLTAGE				5872026UL	//IQ24(280/800)

extern void TerminalDO1Handle(void);
extern void TerminalDO2Handle(void);
extern void TerminalDO3Handle(void);
extern void TerminalDO4Handle(void);
extern void TerminalDO5Handle(void);
extern void TerminalDO6Handle(void);

extern void TerminalDOServoAlm(void);
extern void TerminalDOServoRdy(void);
extern void TerminalDOExBrake(void);
extern void TerminalDOPosiOK(void);
extern void TerminalDOSPDArvl(void);
extern void TerminalDOTLimit(void);
extern void TerminalDOZSPD(void);
extern void TerminalDOSPDCO(void);
extern void TerminalDOSPDLimit(void);
extern void TerminalDOPosiArvl(void);
extern void TerminalDOAlmOut1(void);
extern void TerminalDOAlmOut2(void);
extern void TerminalDOORZ(void);
extern void TerminalDOMtBrake(void);
extern void TerminalDOPulseOutput(void);
extern void TerminalDOTORQArvl(void);

enum DOFUNC
{
  DO_DISABLE = 0,
	SERVOALMOUTPUT, /*1Servo Alarm Output*/
	SERVORDYOUTPUT, /*2Servo Ready Output*/
	EXBRAKESIGNAL,/*3External Brake Release Signal*/
	POSITIONCOMPLETE, /*4Position Complete*/
	SPDARRIVALOUTPUT,/*5Speed Arrival Output*/
	TORQUELIMITOUTPUT, /*6Torque in-limit Signal Output*/
	ZSPDSTSOUTPUT,/*7Zero-Speed Status Output*/
	SPDCCOUTPUT,/*8Speed Coincidence Output*/
	SPDLIMITOUTPUT,/*9Speed in-limit Signal Output*/
	ALARMOUTPUT1,/*10Alarm Output1*/
	ALARMOUTPUT2,/*11Alarm Output2*/
	ORIGINHOMECOMPLETE,/*12Origin Homing Mode Complete*/
	POSITIONARRIVAL,/*13Position Arrival  Output*/
	RUNFLG,//14 mxj
	MOTORBRAKESIGNAL,//15���ɲ�� ��բ���
	ECAMSYNC,//16  //SERVOWARNNINGOUTPUT,//16�������  20180519
	DORESERVE4,//17
	DORESERVE5,//18
	DORESERVE6,//19
	DOHILEVEL,//20
	DOPULSES,//21
	DODIR,//22
	ULTSNCCTRLC1,//23
	ULTSNCCTRLC2,//24
	ULTSNCCTRLC3,//25
	CAM1,//26
	CAM2,//27
	DORESERVE28,//28
	TORQARRIVALOUTPUT,//29   20180904
	DORESERVE30,//30
	DORESERVE31,//31
	DORESERVE32,//32
	DORESERVE33,//33
	DORESERVE34,//34
	DORESERVE35,//35
	SERVOWARNNINGOUTPUT,//36�������

};

enum DOALMOUTPUT
{
	NOOUTPUT = 0,
	ALMDRVFO,				//1:IPM error protection
	ALMDRVDOC,			//2:driver over-current protection
	ALMDRVPOV,			//3:power over-voltage protection
	ALMDRVCOV,			//4:Control board over-voltage protection
	ALMDRVPLV,			//5:power under-voltage protection
	ALMDRVCLV,			//6:ctrl board under-voltage protection
	ALMDRVDOL,			//7:driver over-load protection
	ALMDRVOS,				//8:over-speed protection
	ALMDRVPD,				//9:position deviation excess protection
	ALMDRVSD,				//10:(Reserved)speed deviation excess protection
	ALMDRVES,				//11:Emergency Stop
	ALMDRVDOT,			//12:driver IGBT over-temperature protection
	ALMDRVEE,				//13:EEPROM error
	ALMDRVEC,				//14:(Reserved)232/485 communication error
	ALMDRVBUS,			//15:(Reserved)encoder communication disconnect error
	ALMDRVECE,			//16:(Reserved)encoder communication error
	ALMDRVORL,			//17:(Reserved)over-regeneration load protection
	ALMDRVPIFE,			//18:(Reserved)command pulse input frequency error protection
	ALMDRVDCOF,		//19:Deviation counter overflow protection
	ALMDRVAI1E,			//20:analog input1 excess protection
	ALMDRVAI2E,			//21:analog input2 excess protection
	ALMDRVAI3E,			//22:analog input3 excess protection
	ALMDRVEPE,			//23:analog input3 excess protection
	ALMDRVRLE,			//24:(Reserved)relay error
	ALMDRVECE1,          //25
	ALMDRVECE2,          //26
	ALMDRVECE3,          //27
	ALMDRVEE1,           //28
	ALMDRVEE2,           //29
	ALMDRVEGE,         //30
	ALMDRVPOSE,        //31
	ALMDRVPHS,         //32
	ALMMTRMOC,			//33: motor over-current protection
	ALMMTRMOL,			//34: motor over-load protection
	ALMMTRMOT,			//35: motor over-temperature protection
	ALMMTRMC1,			//36: motor-line( U V W ) connection error
	ALMMTRMC2,			//37:
	ALM_CODE_RSV9,		//38
	WARNPOT,       		//39
	WARNNOT,       		//40
	WARNOL,             //41  over-load warn
	WARNOT,				//42 over-temperature warn
	WARNPOOS,	     	//43 �����������
	WARNAELV,      		//44 ����ֵ���������Ƿѹ
	ALMMTRSEOT,      //45
	ALMMTRSEOS,      //46
	ALMMTRSERL,      //47
	ALMMTRSECE,      //48
	ALMMTRAEMO,      //49
	ALMMTRAECF,      //50
	ALMMTRAEBE,      //51
	ALMMTRSETE,      //52
	ALMMTRSEDE,      //53
	ALMMTRSEDC,      //54
	ALMMTRSEWE,      //55
	ALMMTRCUR,      //56
	ALMMTRBRP,      //57
	ALMMTROLF,      //58
	ALMMTRHOME,      //59
};


#endif /* PERI_TERMINALDO_H_ */



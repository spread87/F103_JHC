/*
 * peri_TerminalDO.c
 *
 * Created on: 2014-6-11
 * Author: JH-LvSL
 */
#include <main.h>
#include "peri_GlobalVariablesExtern.h"
#include "peri_TerminalDO.h"
#include "gpio.h"

void TerminalDOFuncHandle(const TERMINALDO_FUNC *func);
uint8_t TerminalDOSts(uint32_t Fcode);

static const TERMINALDO_FUNC TerminalDOProcessTable[] =
{
  TerminalDO1Handle,
  TerminalDO2Handle,
  TerminalDO3Handle,
  TerminalDO4Handle,
  NULL_FUNC
};

static const TERMINALDO_FUNC TerminalDOFuncTable[] =
{
  TerminalDOServoAlm,	//1、14、36
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

  FcodeValue = FuncCode_Handle.Ram.F50.F5009;		//DO输出有效选择

  DOActiveSet.DO1 = FcodeValue & 0x0001;
  DOActiveSet.DO2 = (FcodeValue & 0x0002) >> 1;
  DOActiveSet.DO3 = (FcodeValue & 0x0004) >> 2;
  DOActiveSet.DO4 = (FcodeValue & 0x0008) >> 3;

  //FcodeValue = FuncCode_Handle.Ram.F50.F5010;//选取功能码
  DOFuncSet.DO1 = FuncCode_Handle.Ram.F50.F5010;		//选取功能码
  DOFuncSet.DO2 = FuncCode_Handle.Ram.F50.F5011;		//选取功能码
  DOFuncSet.DO3 = FuncCode_Handle.Ram.F50.F5012;		//选取功能码
  DOFuncSet.DO4 = FuncCode_Handle.Ram.F50.F5013;		//选取功能码
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
  TerminalDOFuncHandle(TerminalDOProcessTable);		//标志位的读取
  TerminalDOFuncHandle(TerminalDOFuncTable);		//标志位的设置
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
  while ((*func) != NULL_FUNC )
  {
    (*func)();
    func++;
  }
}

/*************************************************
 Function: Terminal DO1 Fucntion Handle
 Description:Period:1ms
 Input: F5010/MX_GPIO_WritePin(IO004_Handle18,STS);
 Return: No
 Others: Be Called TerminalDOProcessTable()
 *************************************************/
void TerminalDO1Handle(void)
{
  if (ForceDOOnEn.bit.ON1 | ForceDOOffEn.bit.OFF1)
  {
    if (ForceDOOnEn.bit.ON1)				//通过上位机强制DO输出有效
      MX_GPIO_WritePin(DO1_GPIO_Port, DO1_Pin, 0);
    else
      //通过上位机强制DO输出无效
      MX_GPIO_WritePin(DO1_GPIO_Port, DO1_Pin, 1);
  }
  else
  {
    if (TerminalDOSts(DOFuncSet.DO1))								//根据DOCtrlFlg的标志位进行输出
      MX_GPIO_WritePin(DO1_GPIO_Port, DO1_Pin, DOActiveSet.DO1);
    else
      MX_GPIO_WritePin(DO1_GPIO_Port, DO1_Pin, !DOActiveSet.DO1);
  }
}

/*************************************************
 Function: Terminal DO2 Fucntion Handle
 Description:Period:1ms
 Input: F5011/MX_GPIO_WritePin(IO004_Handle19,STS);
 Return: No
 Others: Be Called TerminalDOProcessTable()
 *************************************************/
void TerminalDO2Handle(void)
{
  if (ForceDOOnEn.bit.ON2 | ForceDOOffEn.bit.OFF2)
  {
    if (ForceDOOnEn.bit.ON2)
      MX_GPIO_WritePin(DO2_GPIO_Port, DO2_Pin, 0);
    else
      MX_GPIO_WritePin(DO2_GPIO_Port, DO2_Pin, 1);
  }
  else
  {
    if (TerminalDOSts(DOFuncSet.DO2))
      MX_GPIO_WritePin(DO2_GPIO_Port, DO2_Pin, DOActiveSet.DO2);
    else
      MX_GPIO_WritePin(DO2_GPIO_Port, DO2_Pin, !DOActiveSet.DO2);
  }
}

/*************************************************
 Function: Terminal DO3 Fucntion Handle
 Description:Period:1ms
 Input: F5012/MX_GPIO_WritePin(IO004_Handle20,STS);
 Return: No
 Others: Be Called TerminalDOProcessTable()
 *************************************************/
void TerminalDO3Handle(void)
{
  if (ForceDOOnEn.bit.ON3 | ForceDOOffEn.bit.OFF3)
  {
    if (ForceDOOnEn.bit.ON3)
      MX_GPIO_WritePin(DO3_GPIO_Port, DO3_Pin, 0);
    else
      MX_GPIO_WritePin(DO3_GPIO_Port, DO3_Pin, 1);
  }
  else
  {
    if (TerminalDOSts(DOFuncSet.DO3))
      MX_GPIO_WritePin(DO3_GPIO_Port, DO3_Pin, DOActiveSet.DO3);
    else
      MX_GPIO_WritePin(DO3_GPIO_Port, DO3_Pin, !DOActiveSet.DO3);
  }
}

/*************************************************
 Function: Terminal DO4 Fucntion Handle
 Description:Period:1ms
 Input: F5013/MX_GPIO_WritePin(IO004_Handle21,STS);
 Return: No
 Others: Be Called TerminalDOProcessTable()
 *************************************************/
void TerminalDO4Handle(void)
{
  if (ForceDOOnEn.bit.ON4 | ForceDOOffEn.bit.OFF4)
  {
    if (ForceDOOnEn.bit.ON4)
      MX_GPIO_WritePin(DO4_GPIO_Port, DO4_Pin, 0);
    else
      MX_GPIO_WritePin(DO4_GPIO_Port, DO4_Pin, 1);
  }
  else
  {
    if (TerminalDOSts(DOFuncSet.DO4))
      MX_GPIO_WritePin(DO4_GPIO_Port, DO4_Pin, DOActiveSet.DO4);
    else
      MX_GPIO_WritePin(DO4_GPIO_Port, DO4_Pin, !DOActiveSet.DO4);
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

  switch (Fcode)
  {
  case DO_DISABLE:
    break;
  case SERVOALMOUTPUT:
    DOFlg = DOCtrlFlg.bit.ServoAlm;								//1
    break;
  case SERVORDYOUTPUT:
    DOFlg = DOCtrlFlg.bit.ServoRdy;								//2
    break;
  case EXBRAKESIGNAL:
    DOFlg = DOCtrlFlg.bit.ExBrake;								//3
    break;
  case POSITIONCOMPLETE:
    DOFlg = DOCtrlFlg.bit.PosiOK;								//4
    break;
  case SPDARRIVALOUTPUT:
    DOFlg = DOCtrlFlg.bit.SPDArvl;								//5
    break;
  case TORQUELIMITOUTPUT:
    DOFlg = DOCtrlFlg.bit.TLimit;								//6
    break;
  case ZSPDSTSOUTPUT:
    DOFlg = DOCtrlFlg.bit.ZSPD;								//7
    break;
  case SPDCCOUTPUT:
    DOFlg = DOCtrlFlg.bit.SPDCO;								//8
    break;
  case SPDLIMITOUTPUT:
    DOFlg = DOCtrlFlg.bit.SPDLimit;								//9
    break;
  //case POSITIONARRIVAL:
  //	DOFlg = DOCtrlFlg.bit.PosiArvl;//
  //	break;
  case ALARMOUTPUT1:
    DOFlg = DOCtrlFlg.bit.AlmOut1;	//10
    break;
  case ALARMOUTPUT2:
    DOFlg = DOCtrlFlg.bit.AlmOut2;	//11
    break;
  case ORIGINHOMECOMPLETE:
    DOFlg = DOCtrlFlg.bit.ORZ;	//12
    break;
  case POSITIONARRIVAL:	//13
    DOFlg = DOCtrlFlg.bit.PosiArvl;	//13
    break;
  case RUNFLG:					//14
    //DOFlg = CoreStatusFlag.bit.RUN_OK_F;
    DOFlg = DOCtrlFlg.bit.Run;		//14
    break;
  case MOTORBRAKESIGNAL:		//15
    DOFlg = DOCtrlFlg.bit.MotorBreak;		//15
    break;
  case DORESERVE4:		//17
    break;
  case DORESERVE5:
    break;
  case DORESERVE6:
    break;
  case DOHILEVEL:		//20
    DOFlg = 1;
    break;
  case DOPULSES:		//21
    DOFlg = DOCtrlFlg.bit.Pulses;
    break;
  case DODIR:		//22
    DOFlg = DOCtrlFlg.bit.Dir;
    break;
  case TORQARRIVALOUTPUT: //29 转矩到达输出  20180904
    DOFlg = DOCtrlFlg.bit.TorqArvl;
    break;
  case SERVOWARNNINGOUTPUT: //16 ->36
    //DOFlg = CoreStatusFlag.bit.WARN_ALL_F;//16
    DOFlg = DOCtrlFlg.bit.Warnning;		//16 ->36
    break;
  }
  return DOFlg;
}

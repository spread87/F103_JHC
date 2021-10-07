/*
 * peri_CanHandle.c
 *
 *  Created on: 2014-12-12
 *      Author: JH-LvSL
 */
#include <main.h>			//Declarations from DAVE3 Code Generation (includes SFR declaration)
#include "peri_GlobalVariablesExtern.h"
#include "peri_CanHandle.h"
#include "peri_VeloEasyPLC.h"

extern CAN_HandleTypeDef hcan;

uint8_t CanData[8] = { 11, 22, 33, 44, 55, 66, 77, 88 };
uint32_t Cnt_Can_1ms = 0;
CanCmn CanTransmitData;
CAN_TxHeaderTypeDef   TxHeader;
CAN_RxHeaderTypeDef   RxHeader;
uint8_t               TxData[8];
uint8_t               RxData[8];
uint32_t              TxMailbox;

static void CanChannelHandle(void);

static const CanValue CanValueTable[] =
{
  { &g_canview1, 0 }, //0
  { &g_canview2, 0 }, //1
  { &g_canview3, 0 }, //2
  { &g_canview4, 0 }, //3
  { &g_canview1, 0 }, //4  Vds
  { &g_canview1, 0 }, //5  Vqs
  { &g_canview1, 0 }, //6  Vde
  { &g_canview1, 0 }, //7  Vqe
  { &g_canview1, 0 }, //8  Id_given
  { &g_canview1, 0 }, //9  Id_feedback
};

void CANInit(void)
{
  CAN_FilterTypeDef  sFilterConfig;

  /* Configure the CAN Filter */
  sFilterConfig.FilterBank = 0;
  sFilterConfig.FilterMode = CAN_FILTERMODE_IDMASK;
  sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;
  sFilterConfig.FilterIdHigh = 0x0000;
  sFilterConfig.FilterIdLow = 0x0000;
  sFilterConfig.FilterMaskIdHigh = 0x0000;
  sFilterConfig.FilterMaskIdLow = 0x0000;
  sFilterConfig.FilterFIFOAssignment = CAN_RX_FIFO0;
  sFilterConfig.FilterActivation = ENABLE;
  sFilterConfig.SlaveStartFilterBank = 14;

  if (HAL_CAN_ConfigFilter(&hcan, &sFilterConfig) != HAL_OK)
  {
    /* Filter configuration Error */
    Error_Handler();
  }


  /* Start the CAN peripheral */
  if (HAL_CAN_Start(&hcan) != HAL_OK)
  {
    /* Start Error */
    Error_Handler();
  }

  /* Activate CAN RX notification */
  if (HAL_CAN_ActivateNotification(&hcan, CAN_IT_RX_FIFO0_MSG_PENDING) != HAL_OK)
  {
    /* Notification Error */
    Error_Handler();
  }

  /* Configure Transmission process */
  TxHeader.StdId = 0x321;
  TxHeader.ExtId = 0x01;
  TxHeader.RTR = CAN_RTR_DATA;
  TxHeader.IDE = CAN_ID_STD;
  TxHeader.DLC = 2;
  TxHeader.TransmitGlobalTime = DISABLE;

#if 0
  CAN001_MessageHandle0_1.Identifier = 0X100 + FuncCode_Handle.Ram.F60.F6019;	//CAN监控发送ID 借用时间报文
  //充电机CAN测试
  CAN001_MessageHandle0_2.Identifier = 0X1307C080 + FuncCode_Handle.Ram.F60.F6024;	//CAN主发送ID
  CAN001_MessageHandle0_3.Identifier = 0X1207C080 + FuncCode_Handle.Ram.F60.F6024;	//CAN主接收ID
#endif
  //CAN_Handle1_NODE();

  CanTransmitData.Can_Step = 0;
  CanTransmitData.Can_ReceiveNo_Cnt = 0; //
  CanTransmitData.Can_Number = 1;
}

/**
  * @brief  Rx Fifo 0 message pending callback in non blocking mode
  * @param  CanHandle: pointer to a CAN_HandleTypeDef structure that contains
  *         the configuration information for the specified CAN.
  * @retval None
  */
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *CanHandle)
{
  /* Get RX message */
  if (HAL_CAN_GetRxMessage(CanHandle, CAN_RX_FIFO0, &RxHeader, RxData) != HAL_OK)
  {
    /* Reception Error */
    Error_Handler();
  }

  /* Display LEDx */
  if ((RxHeader.StdId == 0x321) && (RxHeader.IDE == CAN_ID_STD) && (RxHeader.DLC == 2))
  {
    //ubKeyNumber = RxData[0];
  }
}

static void CanSend(void)
{
  uint8_t tempCanData[8];
  int64_t Temp;

  uint8_t Volt_Select = 0;
  uint8_t Curr_Select = 0;

  static uint16_t RUN_OK_F_Old = 1;
  static uint16_t sendcnt = 0;
  sendcnt++;

  static uint32_t Volt_Cmd = 0;
  static uint32_t Curr_Cmd = 0;

  Volt_Select = (FuncCode_Handle.Ram.F10.F1003 & 0x00F); //电压电流指令来源
  Curr_Select = (FuncCode_Handle.Ram.F10.F1003 & 0x0F0) >> 4; //电流电流指令来源

  if (Volt_Select == 1)
  {
    Temp = (g_ComPulse & 0x0000FFFF); //电压设定
  }
  else
  {
    Temp = FuncCode_Handle.Ram.F20.F2000; //电压设定
  }
  if (Temp > FuncCode_Handle.Ram.F10.F1008)
    Temp = FuncCode_Handle.Ram.F10.F1008; //电压限定
  else if (Temp < FuncCode_Handle.Ram.F00.F0008) //the min volt
    Temp = FuncCode_Handle.Ram.F00.F0008; //the min volt

  g_Vrms_Cmd = Temp;

  if (Curr_Select == 1)
  {
    Temp = (g_ComPulse & 0xFFFF0000) >> 16; //电流设定
  }
  else
  {
    Temp = FuncCode_Handle.Ram.F20.F2001; //电流设定
  }

  if (Temp < FuncCode_Handle.Ram.F00.F0010) //the min Currt
    Temp = FuncCode_Handle.Ram.F00.F0010; //the min Currt

  g_Irms_Cmd = Temp;

  if ((CoreStatusFlag.bit.RUN0_OK_F == 1)) //开机，先设定电压电流在开机
  {
    Temp = g_Vrms_Cmd; //电压设定
    Temp = (uint32_t) Temp * 100;

    if (Temp != Volt_Cmd)
    {
      Volt_Cmd = Temp;
      CanTransmitData.Can_Step = 10;
    }

    //*********************************************************************20200601
    if ((g_VrmsCalc - g_Vrms_Cmd) >= (int32_t) (FuncCode_Handle.Ram.F00.F0006 >> 5))		//the  3.125% of Rated volt
    {
      Temp = FuncCode_Handle.Ram.F00.F0010;		////the min current
    }
    else if (g_VrmsCalc < FuncCode_Handle.Ram.F00.F0008)		//the min volt
    {
      Temp = (FuncCode_Handle.Ram.F10.F1006);
    }
    else if (g_IrmsCalc < (FuncCode_Handle.Ram.F00.F0010))		//the min current
    {
      Temp = (FuncCode_Handle.Ram.F00.F0005 >> 1);
    }
    else
    {
      Temp = g_Irms_Cmd;		//电流设定
    }

    if (Temp > g_Irms_Cmd)
      Temp = g_Irms_Cmd;		//电流设定
    g_Irms_Cmd_A = Temp;
    Temp = (uint32_t) Temp * 100;
    if (Temp > 220000)		//20200408   current CMD limit is changed to be 220A.
      Temp = 220000;
    if (Temp != Curr_Cmd)
    {
      Curr_Cmd = Temp;
      CanTransmitData.Can_Step = 10;
    }

    if (RUN_OK_F_Old == 0)
    {
      CanTransmitData.Can_Step = 10;
    }
  }
  else if (CoreStatusFlag.bit.RUN0_OK_F == 0)		//关机
  {
    if (RUN_OK_F_Old == 1)
    {
      CanTransmitData.Can_Step = 12;
    }
  }

  switch (CanTransmitData.Can_Step)
  {
  case 10:		//设定电压电流
    tempCanData[0] = 00;
    tempCanData[1] = (Curr_Cmd & 0x00FF0000) >> 16;		//电流设定
    tempCanData[2] = (Curr_Cmd & 0x0000FF00) >> 8;
    tempCanData[3] = (Curr_Cmd & 0x000000FF) >> 0;

    tempCanData[4] = (Volt_Cmd & 0xFF000000) >> 24;		//电压设定
    tempCanData[5] = (Volt_Cmd & 0x00FF0000) >> 16;
    tempCanData[6] = (Volt_Cmd & 0x0000FF00) >> 8;		//
    tempCanData[7] = (Volt_Cmd & 0x000000FF) >> 0;
    break;
  case 11:		//开机
    tempCanData[0] = 02;
    tempCanData[1] = 00;
    tempCanData[2] = 00;
    tempCanData[3] = 00;
    tempCanData[4] = 00;		//
    tempCanData[5] = 00;
    tempCanData[6] = 00;		//
    tempCanData[7] = 0x55;
    break;
  case 12:		//关机
    tempCanData[0] = 02;
    tempCanData[1] = 00;
    tempCanData[2] = 00;
    tempCanData[3] = 00;
    tempCanData[4] = 00;		//
    tempCanData[5] = 00;
    tempCanData[6] = 00;		//
    tempCanData[7] = 0xAA;
    break;
  case 0:		//读取电压电流及状态
  case 4:
    //break;
  case 5:
    //break;
  default:		//读取电压电流及状态
    tempCanData[0] = 01;
    tempCanData[1] = 00;
    tempCanData[2] = 00;
    tempCanData[3] = 00;

    tempCanData[4] = 00;		//返回速度
    tempCanData[5] = 00;
    tempCanData[6] = 00;		//返回状态字
    tempCanData[7] = 00;
    break;
  }

  //CAN001_UpdateMODataRegisters(&CAN001_Handle0, 2, 8, tempCanData); //发送数据
  //CAN001_SendDataFrame(&CAN001_Handle0, 2); //发送监控指令

  FuncCode_Handle.Ram.FA0.FA054 = (uint32_t) tempCanData[0] + ((uint32_t) tempCanData[1] << 8)
      + ((uint32_t) tempCanData[2] << 16) + ((uint32_t) tempCanData[3] << 24);

  g_canview2 += (2 << 10);
  g_canview4 += 2000;

  RUN_OK_F_Old = CoreStatusFlag.bit.RUN0_OK_F;
}

void CanResive(void)
{
  uint8_t data0;
  uint8_t data1;
  uint8_t data2;
  uint8_t data3;
  uint8_t data4;
  uint8_t data5;
  uint8_t data6;
  uint8_t data7;

  static uint16_t RUN_Cnt = 0;
  static uint16_t STOP_Cnt = 0;

  typedef struct StatusBit
  {
    uint16_t RUN_OK_F :1;	//[0]:1开机0关机
    uint16_t FO :1;	//[1]:
    uint16_t VIS :1;	//[2]:
    uint16_t FAN :1;	//[3]:
    uint16_t POV :1;    //[4]:
    uint16_t PLV :1;
    uint16_t OV :1;    //[6]:
    uint16_t LV :1;    //[7]:
    uint16_t DOC :1;    //[8]:
    uint16_t DOT :1;    //[9]:
    uint16_t RUN0_OK_F :1;    //[10]:开关机设置状态1：设置关机 0：设置开机
    uint16_t RES :5;    //[11~15]:reserved
  } StatusBit;

  typedef union ChargerStatusFlg
  {
    uint16_t all;
    StatusBit bit;
  } ChargerStatusFlg;

  ChargerStatusFlg ChargerStatusFlag;

  if(1) //((CAN001_GetMOFlagStatus(&CAN001_Handle0, 3, RECEIVE_PENDING | NEW_DATA))) //接收数据
  {
    //FuncCode_Handle.Ram.FA0.FA058 += CAN001_ReadMsgObj(&CAN001_Handle0, &CAN001_MessageHandle0_3, 3) * 10;

//    data0 = CAN001_MessageHandle0_3.data[0];
//    data1 = CAN001_MessageHandle0_3.data[1];
//    data2 = CAN001_MessageHandle0_3.data[2];
//    data3 = CAN001_MessageHandle0_3.data[3];
//    data4 = CAN001_MessageHandle0_3.data[4];
//    data5 = CAN001_MessageHandle0_3.data[5];
//    data6 = CAN001_MessageHandle0_3.data[6];
//    data7 = CAN001_MessageHandle0_3.data[7];

    switch (CanTransmitData.Can_Step)
    {
    case 10:		//设定电压电流
      if ((data0 == 0) && (data1 == 0xFF))		//电压电流设定成功
      {
        CanTransmitData.Can_Step = 11;		//准备开机
      }
      break;
    case 11:		//开机
      if ((data0 == 0x02) && ((data1 & 0x0F) == 0x0F))		//开机设定成功
      {
        CanTransmitData.Can_Step = 0;				//读取电压电流及状态
      }
      break;
    case 12:				//关机
      if ((data0 == 0x02) && (((data1 & 0x0F) == 0x0F) || ((data1 & 0xF0) == 0xF0)))				//关机设定成功 2号机
      {
        CanTransmitData.Can_Step = 0;				//读取电压电流及状态
      }
      break;
    case 0:				//读取电压电流及状态
      if ((data0 == 1) && (data1 == 0x00))				//电压电流读取成功
      {
        g_IrmsCalc = (((uint32_t) data2) << 8) + (uint32_t) data3;
        g_VrmsCalc = (((uint32_t) data4) << 8) + (uint32_t) data5;
        g_PrmsCalc = g_IrmsCalc * g_VrmsCalc;
        ChargerStatusFlag.all = (((uint16_t) data6) << 8) + (uint16_t) data7;

        FuncCode_Handle.Ram.FA0.FA027 = (uint32_t) ChargerStatusFlag.all + (((uint32_t) data3) << 24)
            + (((uint32_t) data5) << 16);

        AlarmStatusFlag.bit.Driver.DOC = ChargerStatusFlag.bit.DOC;
        AlarmStatusFlag.bit.Driver.DOT = ChargerStatusFlag.bit.DOT;
        AlarmStatusFlag.bit.Driver.FO = ChargerStatusFlag.bit.FO;
        AlarmStatusFlag.bit.Driver.PLV = ChargerStatusFlag.bit.PLV;
        AlarmStatusFlag.bit.Driver.POV = ChargerStatusFlag.bit.POV;
        AlarmStatusFlag.bit.Driver.PD = ChargerStatusFlag.bit.OV;
        AlarmStatusFlag.bit.Driver.POSE = ChargerStatusFlag.bit.FAN;

        if (CoreStatusFlag.bit.RUN0_OK_F)					//运行指令
        {
          CoreStatusFlag.bit.RUN_OK_F = 1;
          RUN_Cnt = 0;
          if (ChargerStatusFlag.bit.RUN_OK_F)					//停机
          {
            if ((STOP_Cnt * CanTransmitData.tempCanMSEn) > 20000)					//0.5*40=20S
              AlarmStatusFlag.bit.Driver.SD = OK;
            else
              STOP_Cnt++;
          }
          else
          {
            AlarmStatusFlag.bit.Driver.SD = NG;
            STOP_Cnt = 0;
          }
        }
        else					//停机指令
        {
          STOP_Cnt = 0;
          if (RUN_Cnt >= 6)					//500*6=3000ms
          {
            CoreStatusFlag.bit.RUN_OK_F = ~ChargerStatusFlag.bit.RUN_OK_F;
          }
          else
            RUN_Cnt++;
        }
        //***********************************************************************
      }
      break;

    default:
      break;
    }

    CanTransmitData.Can_ReceiveNo_Cnt = 0;
    AlarmStatusFlag.bit.Motor.SEDC = 0;

    g_canview22 = g_PulInCan << 8;

    FuncCode_Handle.Ram.FA0.FA055 = g_PulInCan + (g_SpdInCan << 6);	//FA-55 is HEX,the low 16bit is g_PulInCan,the high is g_SpdInCan
  }
  else
  {
    CanTransmitData.Can_ReceiveNo_Cnt++;
    FuncCode_Handle.Ram.FA0.FA056++;

    if (FuncCode_Handle.Ram.F60.F6017 == 0)
    {
      AlarmStatusFlag.bit.Motor.SEDC = 0;
    }
    else if (CanTransmitData.Can_ReceiveNo_Cnt > (FuncCode_Handle.Ram.F60.F6017 + 100))		//增加F6-17功能码通讯断线报警设定
    {
      AlarmStatusFlag.bit.Motor.SEDC = 1;
      CoreStatusFlag.bit.RUN_OK_F = NG;		//有故障，状态清零		20200117
    }
  }

  //CAN001_ClearMOFlagStatus(&CAN001_Handle0, 3, RECEIVE_PENDING | NEW_DATA | MESSAGE_LOST);

  FuncCode_Handle.Ram.FA0.FA057 = ((int32_t) FuncCode_Handle.Ram.FA0.FA057 * 63 + g_SpdInCan) >> (6 + 10);
}

/*************************************************
 Function: Can Communication Handle
 Description:Period:1ms
 Input: No
 Return: No
 Others: No
 *************************************************/
void CanHandle(void)
{
  static uint16_t sCanMSCnt = 0;		//CAN主从通信发送时间间隔计数值
  static uint16_t sCanMRCnt = 0;		//CAN主从通信接收时间间隔计数值

  if (CanTransmitData.tempCanMREn > 0) //CAN主从通信功能  接收
  {
    if (sCanMRCnt < CanTransmitData.tempCanMREn - 1)
    {
      sCanMRCnt++;
    }
    else
    {
      CanResive();		//接收数据
      sCanMRCnt = 0;
      g_canview19 = FuncCode_Handle.Ram.FA0.FA056 * 16777;
    }
  }

  if (CanTransmitData.tempCanMSEn > 0) //CAN主从通信功能   发送
  {
    if (sCanMSCnt < CanTransmitData.tempCanMSEn - 1)
    {
      sCanMSCnt++;
    }
    else
    {
      CanSend(); //发送数据
      sCanMSCnt = 0;
    }
  }

  if ((CanTransmitData.tempCanMonEn > 0) && (FuncCode_Handle.Ram.F60.F6025 != 0)) //上位机监控功能
  {
    if (Cnt_Can_1ms < CanTransmitData.tempCanMonEn - 1) // 发送时间间隔
    {
      Cnt_Can_1ms++;
    }
    else
    {
      Cnt_Can_1ms = 0;
      //g_NoValue = -16777216;
      CanChannelHandle();
      /* Update data value to be transmitted by  Node "Request" in message object 1 (LM01) with ID 0x101 */
      //CAN001_UpdateMODataRegisters(&CAN001_Handle0, 1, 8, CanData);
      /* Send Data frame with ID 0x101 on chip internal CAN bus  */
      //CAN001_SendDataFrame(&CAN001_Handle0, 1); //发送监控数据

      g_canview1 += (1 << 10);
      g_canview3 += 1000;
    }
  }
}

/*************************************************
 Function: Can Data Handle
 Description:Period:1ms
 Input: No
 Return: No
 Others: Be Called by CanChannelHandle()
 *************************************************/
uint16_t CanDataHandle(int32_t Data)
{
  uint16_t Temp = 0;
  Temp = (uint16_t) (Data >> 10);
  return Temp;
}

/*************************************************
 Function: Can Data Handle
 Description:Period:1ms
 Input: No
 Return: No
 Others: Be Called by CanHandle()//CanValueTable
 *************************************************/
static void CanChannelHandle(void)
{
  uint16_t Channel1, Channel2, Channel3, Channel4;

  Channel1 = (*CanValueTable[CanTransmitData.channel1].add >> 10);
  Channel2 = (*CanValueTable[CanTransmitData.channel2].add >> 10);
  Channel3 = (*CanValueTable[CanTransmitData.channel3].add >> 10);
  Channel4 = (*CanValueTable[CanTransmitData.channel4].add >> 10);

  CanData[0] = (uint8_t) ((Channel1 >> 8) & 0xFF);
  CanData[1] = (uint8_t) (Channel1 & 0xFF);
  CanData[2] = (uint8_t) ((Channel2 >> 8) & 0xFF);
  CanData[3] = (uint8_t) (Channel2 & 0xFF);
  CanData[4] = (uint8_t) ((Channel3 >> 8) & 0xFF);
  CanData[5] = (uint8_t) (Channel3 & 0xFF);
  CanData[6] = (uint8_t) ((Channel4 >> 8) & 0xFF);
  CanData[7] = (uint8_t) (Channel4 & 0xFF);
}

/*************************************************
 Function: Can Data Handle
 Description:Period:mainloop
 Input: No
 Return: No
 Others: Be Called by CanHandle()//CanValueTable
 *************************************************/
void CanUpdateHandle(void)
{
  uint32_t FcodeValue = 0;
  uint32_t Temp = 0;

  FcodeValue = FuncCode_Handle.Ram.F60.F6025;

  Temp = FcodeValue / 1000000;
  CanTransmitData.channel1 = Temp % 100;
  Temp = FcodeValue / 10000;
  CanTransmitData.channel2 = Temp % 100;
  Temp = FcodeValue / 100;
  CanTransmitData.channel3 = Temp % 100;
  Temp = FcodeValue;
  CanTransmitData.channel4 = Temp % 100;
  CanTransmitData.tempCanMonEn = FuncCode_Handle.Ram.F60.F6026 % 1000; //CAN上位机监控功能使能 兼具发送时间间隔功能
  Temp = FuncCode_Handle.Ram.F60.F6026 / 1000;
  CanTransmitData.tempCanMSEn = Temp / 100;
  CanTransmitData.tempCanMREn = Temp % 100;
}

int16_t WaveA[10] = { 0 };
int16_t WaveB[10] = { 0 };
int16_t WaveC[10] = { 0 };
int16_t WaveD[10] = { 0 };

void Wave_dis(void)	//1ms程序调用  0x22特殊指令用
{
  static int16_t i = 0;
  static int16_t Cnt_Wave_1ms = 0;

  if ((CanTransmitData.tempCanMonEn > 0) && (FuncCode_Handle.Ram.F60.F6025 != 0)) //上位机监控功能
  {
    if (Cnt_Wave_1ms < CanTransmitData.tempCanMonEn - 1) // 发送时间间隔
    {
      Cnt_Wave_1ms++;
    }
    else
    {
      Cnt_Wave_1ms = 0;
      if ((CoreStatusFlag.bit.WAVE_DATA_F == NG) && (CoreStatusFlag.bit.WAVE_DATA_S)) //状态字3的bit0：缓存读取完成，并且需要重新载入数据，就可以重新开始载入数据
      {
        WaveA[i] = (*CanValueTable[CanTransmitData.channel1].add >> 10);
        WaveB[i] = (*CanValueTable[CanTransmitData.channel2].add >> 10);
        WaveC[i] = (*CanValueTable[CanTransmitData.channel3].add >> 10);
        WaveD[i] = (*CanValueTable[CanTransmitData.channel4].add >> 10);

        if (i < 9)
        {
          i++;
        }
        else
        {
          CoreStatusFlag.bit.WAVE_DATA_F = OK;
          CoreStatusFlag.bit.WAVE_DATA_S = NG;
          i = 0;
        }
      }
    }
  }
}

int16_t Wave13[5] = { 0 };
void Wave13_dis(void)				 //Modbus13指令解析程序调用  0x13特殊指令用
{
  Wave13[0] = (*CanValueTable[CanTransmitData.channel1].add >> 10);
  Wave13[1] = (*CanValueTable[CanTransmitData.channel2].add >> 10);
  Wave13[2] = (*CanValueTable[CanTransmitData.channel3].add >> 10);
  Wave13[3] = (*CanValueTable[CanTransmitData.channel4].add >> 10);

  Wave13[4] = (*CanValueTable[57].add >> 10);
}

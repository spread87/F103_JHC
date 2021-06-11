/*
 * peri_PanelFunc.c
 *
 *  Created on: 2014-4-1
 *  Author: JH-LvSL
 */
#include <main.h>			//Declarations from DAVE3 Code Generation (includes SFR declaration)
#include <peri_FuncCodeHandle.h>
#include <peri_GlobalVariablesExtern.h>
#include <peri_PanelFunc.h>
#include <peri_PanelLEDButton.h>
#include <peri_SwScan.h>
#include "peri_Init.h"

uint8_t g_MenuGrade = 0;	//0: Main Menu; 1: 1st Menu; 2:2nd Menu
uint32_t g_1stDisplayData = 0;
uint32_t g_2ndDisplayData = 0;
uint32_t g_3ndDisplayData = 0;	//����1��
uint8_t g_1sttwinkle = 3;
uint8_t g_2ndtwinkle = 0;
uint8_t g_2ndSetDataSts = 0;	//Return Status After Function Code be Set
PANELFCODE Fcode;

//int32_t PanelButtonUp(int32_t Data,int32_t Max,int32_t twinkle);
int32_t PanelButtonUp(int32_t Data, int32_t Max, uint8_t Dec_Hex,
    int32_t twinkle);	//2010628 mxj
//int32_t PanelButtonDown(int32_t Data,int32_t Min,int32_t twinkle);
int32_t PanelButtonDown(int32_t Data, int32_t Min, uint8_t Dec_Hex,
    int32_t twinkle);	//2010628 mxj
int32_t PanelButtonShift(int32_t Data, int32_t Max);
int32_t PanelButtonMode(int32_t MenuGrade);
int32_t PanelButtonSet(int32_t MenuGrade, uint32_t Data, uint16_t Fcode);
void PanelButtonUpFunc(void);	//JOGģʽ
void PanelButtonDwFunc(void);	//JOGģʽ

#define JOG_FUNCTION     1
#define SAVE_FUNCTION    2
void PanelMainMenuDisplay(void);
void Panel1stMenuDisplay(void);
void Panel2ndMenuDisplay(void);
void Panel3ndMenuDisplay(void);
void Panel3ndFuncDisplay(void);

void Panel2ndMenuDisplaySts(uint8_t Sts);
extern const uint8_t FuncTable[][5];
extern uint8_t LED[6];
extern BUTTONFLG ButtonFlagLong;

const PANEL_FUNC PanelDisplay[] =
{
  PanelMainMenuDisplay,	//������
  Panel1stMenuDisplay,	//��һ���˵�
  Panel2ndMenuDisplay,	//�ڶ����˵�
  Panel3ndMenuDisplay,	//�������˵�
  NULL_FUNC
};

LEDDISPLAY PanelDataDisplayTable[] =
{
  /*Variable,								Data Attribute*/
  { &g_Velocity10times, 0x18 },
  { &g_LEDVelocityCmd, 0x04 },
  { &g_LEDPositionCmd, 0x00 },
  { &g_LEDTorqueCmd, 0x04 },
  { &g_LEDPosiDev, 0x12 },
  { &g_LEDLoadRated, 0x04 },
  { &g_LEDTem, 0x00 },
  { &g_LEDEncDev, 0x00 },
  { &g_LEDAI1, 0x18 },
  { &g_LEDAI2, 0x18 },
  { &g_LEDAI3, 0x18 },
  { &g_LEDVbus, 0x04 },
  { &g_LEDCurrent, 0x04 },
  { &g_LEDRunTime, 0x04 },
  { &g_PositionCount, 0x04 },
  { &g_out_counter, 0x04 }
};

const uint16_t FcodeGroupNum[] =
{
  FCODE_F00_Num,
  FCODE_F60_Num,
  FCODE_FA0_Num
};

/*************************************************
 Function: Panel LED&Button Handle
 Description:
 Input: No
 Return: No
 Others: Be Called SysCore1msLoop()
 *************************************************/
void PanelLEDButtonHandle(void)
{
  if (SysTimeSequ.Start_Flg)	//after e2prom init ok;
    PanelDisplay[g_MenuGrade]();	//total handle
  else
  {
    LED[0] = OPE_7SEG_LED_ZP;	//"-"
    LED[1] = OPE_7SEG_LED_J;	//"j"
    LED[2] = OPE_7SEG_LED_NULL;
    LED[3] = 0x89;	//H
    LED[4] = OPE_7SEG_LED_ZP;	//"-"
  }
  PanelLEDDisplay();	//LED��ʾ�Ͱ�ť��ȡ
}
/*************************************************
 Function: Panel Main Menu Display
 Description:
 Input: No
 Return: No
 Others: Be Called PanelDisplay[]
 *************************************************/
void PanelMainMenuDisplay(void)
{
  uint32_t temp = 0;
  static uint8_t DisplayMode = 0;		//0:display alm 1:display data
  static uint8_t DisplayDataHL = 0; //0:Low bytes data,1: High bytes data

  //if(FuncCode_Handle.Ram.F00.F0006 <= 999)//��������λ��ʾ
  if (FuncCode_Handle.Ram.F00.F0009 <= 999) //��ѹ���޸���100V������ʾ��λ
  {
    PanelDataDisplayTable[0].Att = 0x1C;
  }

  temp = FuncCode_Handle.Ram.F60.F6015; //��ʼ״̬
  if (g_AlmNum) //�й��ϵ�ʱ��
  {
    if (!DisplayMode)
      PanelAlmDisplay(g_AlmNum); //error display
    else
      PanelDataDisplay(*PanelDataDisplayTable[temp].add,
          PanelDataDisplayTable[temp].Att, DisplayDataHL, &CoreStatusFlag);
    if (ButtonShift.flg.bit.DI_OUT_F)
    {
      DisplayMode = ~DisplayMode;
      ButtonShift.flg.bit.DI_OUT_F = 0;
    }
  }
  else //�޹��ϵ�ʱ��
  {
    PanelDataDisplay(*PanelDataDisplayTable[temp].add,
        PanelDataDisplayTable[temp].Att, DisplayDataHL, &CoreStatusFlag);
  }

  g_1stDisplayData = 0;
  g_1sttwinkle = 3;
  g_MenuGrade = PanelButtonSet(g_MenuGrade, 0, 0);

  if (ButtonUp.flg.bit.DI_OUT_F)
  {
    DisplayDataHL = ~DisplayDataHL;
    ButtonUp.flg.bit.DI_OUT_F = 0;
  }

  if (ButtonDown.flg.bit.DI_OUT_F)
  {
    DisplayDataHL = ~DisplayDataHL;
    ButtonDown.flg.bit.DI_OUT_F = 0;
  }
  ButtonMode.flg.bit.DI_OUT_F = 0;
}
/*************************************************
 Function: Panel Main Menu Display
 Description:
 Input: No
 Return: No
 Others: Be Called PanelDisplay[]
 *************************************************/
void Panel1stMenuDisplay(void)
{
//	static uint8_t TwinkleBit = 3;
  //g_1stDisplayData
  int32_t HundredBit = 0;
  int32_t TenOneBit = 0;
  uint32_t FcodeNumLength = 0;

  HundredBit = (int32_t) g_1stDisplayData / 100;
  TenOneBit = (int32_t) g_1stDisplayData % 100;

  FcodeNumLength = sizeof(FcodeGroupNum) / sizeof(FcodeGroupNum[0]);//���ֽ���/�����ֽ���
  g_1sttwinkle = (uint8_t) PanelButtonShift((int32_t) g_1sttwinkle, 3);

  if (ButtonUp.flg.bit.DI_OUT_F)
  {
    switch (g_1sttwinkle)
    {
    case 3:
      HundredBit++;
      if (HundredBit >= FcodeNumLength)
      {
        HundredBit = 0;	//FcodeNumLength - 1;
      }
      TenOneBit = 0;
      break;
    case 1:
      TenOneBit += 10;
      if (TenOneBit >= (int32_t) FcodeGroupNum[HundredBit])
        TenOneBit = (int32_t) FcodeGroupNum[HundredBit] - 1;
      break;
    case 2:
      TenOneBit++;
      if (TenOneBit >= (int32_t) FcodeGroupNum[HundredBit])
        TenOneBit = (int32_t) FcodeGroupNum[HundredBit] - 1;
      break;
    default:
      break;
    }
    ButtonUp.flg.bit.DI_OUT_F = 0;
  }
  else if (ButtonDown.flg.bit.DI_OUT_F)
  {
    switch (g_1sttwinkle)
    {
    case 3:
      HundredBit--;
      if (HundredBit < 0)
        HundredBit = (int32_t) FcodeNumLength - 1;
      TenOneBit = 0;
      break;
    case 1:
      TenOneBit -= 10;
      if (TenOneBit < 0)
        TenOneBit = 0;
      break;
    case 2:
      TenOneBit--;
      if (TenOneBit < 0)
        TenOneBit = 0;
      break;
    default:
      break;
    }
    ButtonDown.flg.bit.DI_OUT_F = 0;
  }
  g_1stDisplayData = (uint32_t) (HundredBit * 100 + TenOneBit);
  Panel1stMenuDataDisplay((uint16_t) g_1stDisplayData, g_1sttwinkle);

  g_2ndtwinkle = 0;
  g_MenuGrade = PanelButtonSet(g_MenuGrade, 0, 0);
  g_MenuGrade = PanelButtonMode(g_MenuGrade);

  Fcode.FcodeNum = (uint16_t) HundredBit * 1000 + (uint16_t) (TenOneBit);
  /*Read Fcode Relative Value*/
  FuncCodePanelRead(&Fcode);	//����2���˵���ֵ
  g_2ndDisplayData = Fcode.Value;
}
/*************************************************
 Function: Panel Main Menu Display
 Description:
 Input: No
 Return: No
 Others: Be Called PanelDisplay[]
 *************************************************/
void Panel2ndMenuDisplay(void)
{
  static uint16_t Sts_Count = 0;

  /*Read Fcode Data Length*/
  if ((Fcode.Att >> 11) & 0x01)	//�ƶ�10λ����5λ
    g_2ndtwinkle = (uint8_t) PanelButtonShift((int32_t) g_2ndtwinkle,
        (int32_t) 10);
  else
    g_2ndtwinkle = (uint8_t) PanelButtonShift((int32_t) g_2ndtwinkle,
        (int32_t) 5);

  /* Key 'UP' Process */
  //g_2ndDisplayData = PanelButtonUp(g_2ndDisplayData,Fcode.Max,g_2ndtwinkle);
  g_2ndDisplayData = PanelButtonUp(g_2ndDisplayData, Fcode.Max,
      (Fcode.Att >> 5) & 0x01, g_2ndtwinkle);	//2010628 mxj
  /* Key 'DOWN' Process */
  //g_2ndDisplayData = PanelButtonDown(g_2ndDisplayData,Fcode.Min,g_2ndtwinkle);
  g_2ndDisplayData = PanelButtonDown(g_2ndDisplayData, Fcode.Min,
      (Fcode.Att >> 5) & 0x01, g_2ndtwinkle);	//2010628 mxj
  /*	Key 'SET' Process*/
  g_MenuGrade = PanelButtonSet((int32_t) g_MenuGrade, g_2ndDisplayData,
      Fcode.FcodeNum);
  /* Key 'MODE' Process*/
  g_MenuGrade = PanelButtonMode(g_MenuGrade);

  if (g_3ndDisplayData != 0)	//�������˵���ʾ
  {
    Panel3ndFuncDisplay();
  }
  else //�����˵�����ʾ
  {
    if (g_2ndSetDataSts & 0x80) //������ʾ
    {
      /*Status Display When Key 'Set' is used in the 2nd menu*/
      Sts_Count++;
      Panel2ndMenuDisplaySts(g_2ndSetDataSts & 0X7F); //����״̬��ʾ��only read,save�ȵȣ�����ʱ��1.5S

      /* Status Display 1.5s*/
      if (Sts_Count > 1500)
      {
        g_2ndSetDataSts = 0;
        Sts_Count = 0;
        g_MenuGrade--;
      }
    }
    else
    {
      if (((Fcode.Att >> 3) & 0x03) == 0x03)  //only read mode
        g_2ndDisplayData = FuncCodeOnlyReadValue(&Fcode);
      Panel2ndMenuDataDisplay(g_2ndDisplayData, g_2ndtwinkle, Fcode.Att); //������ʾ
    }
  }
}

void Panel3ndMenuDisplay()  //���ݹ�����ʾ��0 �ޣ� 1��JOG
{

  Panel3ndFuncDisplay();
  g_MenuGrade = PanelButtonMode(g_MenuGrade);
  PanelButtonUpFunc();
  PanelButtonDwFunc();
}

//���ղ�ͬ���ܽ�����ʾ
void Panel3ndFuncDisplay()
{
  static uint16_t DisplayCount = 0;
  uint8_t func;
  func = g_3ndDisplayData;
  if (func == 0)
  {
    LED[0] = OPE_7SEG_LED_N;
    LED[1] = OPE_7SEG_LED_O;
    LED[2] = OPE_7SEG_LED_N;
    LED[3] = OPE_7SEG_LED_E;
    LED[4] = OPE_7SEG_LED_NULL;
  }
  else  //�����
  {
    switch (func)
    {
    case JOG_FUNCTION:
    {
      if (!Inertia_status.bitflag.bit.run_now)  //�ǹ�����ѧϰ
      {
        LED[0] = FuncTable[func - 1][0];
        LED[1] = FuncTable[func - 1][1];
        LED[2] = FuncTable[func - 1][2];
        LED[3] = FuncTable[func - 1][3];
        LED[4] = FuncTable[func - 1][4];
      }
      else  //������ѧϰ
      {
        DisplayCount++;  //20181011
        if ((DisplayCount & 0x0100) || (Inertia_status.study_times <= 10))
          Panel2ndMenuDataDisplay(g_InertiaTotal, 0, 0);
        else
          PanelInitDisplay();
      }
      break;
    }
    case SAVE_FUNCTION:
    {
      if (g_SpecialSave != 0)  //��ȡ������
      {
        LED[0] = FuncTable[func - 1][0];
        LED[1] = FuncTable[func - 1][1];
        LED[2] = FuncTable[func - 1][2];
        LED[3] = FuncTable[func - 1][3];
        LED[4] = FuncTable[func - 1][4];
        break;
      }
      else  //��ȡ���
      {
        //g_MenuGrade--;
        g_MenuGrade -= 2;  //mxj
        g_3ndDisplayData = 0;
        break;
      }
    }
    case 3:
    {
      LED[0] = FuncTable[func - 1][0];
      LED[1] = FuncTable[func - 1][1];
      LED[2] = FuncTable[func - 1][2];
      LED[3] = FuncTable[func - 1][3];
      LED[4] = FuncTable[func - 1][4];
      break;
    }
    default:
      break;
    }
  }
}

void PanelButtonUpFunc()
{
  uint8_t func;
  if (ButtonFlagLong.bit.up == 1)
  {
    func = g_3ndDisplayData;
    switch (func)
    {
    case JOG_FUNCTION:
      g_JogRunUpButton = 1;
    default:
      break;
    }

  }
  else
  {
    g_JogRunUpButton = 0;
  }
  ButtonUp.flg.bit.DI_OUT_F = 0;
}

void PanelButtonDwFunc()
{
  uint8_t func;
  if (ButtonFlagLong.bit.down == 1)
  {
    func = g_3ndDisplayData;
    switch (func)
    {
    case JOG_FUNCTION:
      g_JogRunDwButton = 1;
    default:
      break;
    }

  }
  else
  {
    g_JogRunDwButton = 0;
  }
  ButtonDown.flg.bit.DI_OUT_F = 0;
}
/*************************************************
 Function: Panel Button 'UP' Data
 Description:
 Input: No
 Return: No
 Others: Be Called Panel1stMenuDisplay()/Panel2ndMenuDisplay()
 *************************************************/
int32_t PanelButtonUp(int32_t Data, int32_t Max, uint8_t Dec_Hex,
    int32_t twinkle)  //2010628 mxj
{
  int32_t Data_Add_Mpy;

  if (Dec_Hex)  //Hex
  {
    Data_Add_Mpy = 16;
  }
  else  //Dec
  {
    Data_Add_Mpy = 10;
  }

  if (ButtonUp.flg.bit.DI_OUT_F)
  {
    switch (twinkle)
    {
    case 0:
    case 1:
      Data++;
      if (Data > Max)
        Data = Max;
      break;
    case 2:
      Data += Data_Add_Mpy;
      if (Data > Max)
        Data = Max;
      break;
    case 3:
      Data += (Data_Add_Mpy * Data_Add_Mpy);
      if (Data > Max)
        Data = Max;
      break;
    case 4:
      Data += (Data_Add_Mpy * Data_Add_Mpy * Data_Add_Mpy);
      if (Data > Max)
        Data = Max;
      break;
    case 5:
      Data += (Data_Add_Mpy * Data_Add_Mpy * Data_Add_Mpy * Data_Add_Mpy);
      if (Data > Max)
        Data = Max;
      break;
    case 6:
      Data += (Data_Add_Mpy * Data_Add_Mpy * Data_Add_Mpy * Data_Add_Mpy
          * Data_Add_Mpy);
      if (Data > Max)
        Data = Max;
      break;
    case 7:
      Data += (Data_Add_Mpy * Data_Add_Mpy * Data_Add_Mpy * Data_Add_Mpy
          * Data_Add_Mpy * Data_Add_Mpy);
      if (Data > Max)
        Data = Max;
      break;
    case 8:
      Data += (Data_Add_Mpy * Data_Add_Mpy * Data_Add_Mpy * Data_Add_Mpy
          * Data_Add_Mpy * Data_Add_Mpy * Data_Add_Mpy);
      if (Data > Max)
        Data = Max;
      break;
    case 9:
      Data += (Data_Add_Mpy * Data_Add_Mpy * Data_Add_Mpy * Data_Add_Mpy
          * Data_Add_Mpy * Data_Add_Mpy * Data_Add_Mpy * Data_Add_Mpy);
      if (Data > Max)
        Data = Max;
      break;
    case 10:
      Data += (Data_Add_Mpy * Data_Add_Mpy * Data_Add_Mpy * Data_Add_Mpy
          * Data_Add_Mpy * Data_Add_Mpy * Data_Add_Mpy * Data_Add_Mpy
          * Data_Add_Mpy);
      if (Data > Max)
        Data = Max;
      break;
    default:
      break;
    }

    ButtonUp.flg.bit.DI_OUT_F = 0;
  }

  return Data;
}

/*************************************************
 Function: Panel Button 'DOWN' Data
 Description:
 Input: No
 Return: No
 Others: Be Called Panel1stMenuDisplay()/Panel2ndMenuDisplay()
 *************************************************/
int32_t PanelButtonDown(int32_t Data, int32_t Min, uint8_t Dec_Hex,
    int32_t twinkle)  //2010628 mxj
{
  int32_t Data_Add_Mpy;

  if (Dec_Hex)  //Hex
  {
    Data_Add_Mpy = 16;
  }
  else  //Dec
  {
    Data_Add_Mpy = 10;
  }

  if (ButtonDown.flg.bit.DI_OUT_F)
  {
    switch (twinkle)
    {
    case 0:
    case 1:
      Data--;
      if (Data < Min)
        Data = Min;
      break;
    case 2:
      Data -= Data_Add_Mpy;
      if (Data < Min)
        Data = Min;
      break;
    case 3:
      Data -= (Data_Add_Mpy * Data_Add_Mpy);
      if (Data < Min)
        Data = Min;
      break;
    case 4:
      Data -= (Data_Add_Mpy * Data_Add_Mpy * Data_Add_Mpy);
      if (Data < Min)
        Data = Min;
      break;
    case 5:
      Data -= (Data_Add_Mpy * Data_Add_Mpy * Data_Add_Mpy * Data_Add_Mpy);
      if (Data < Min)
        Data = Min;
      break;
    case 6:
      Data -= (Data_Add_Mpy * Data_Add_Mpy * Data_Add_Mpy * Data_Add_Mpy
          * Data_Add_Mpy);
      if (Data < Min)
        Data = Min;
      break;
    case 7:
      Data -= (Data_Add_Mpy * Data_Add_Mpy * Data_Add_Mpy * Data_Add_Mpy
          * Data_Add_Mpy * Data_Add_Mpy);
      if (Data < Min)
        Data = Min;
      break;
    case 8:
      Data -= (Data_Add_Mpy * Data_Add_Mpy * Data_Add_Mpy * Data_Add_Mpy
          * Data_Add_Mpy * Data_Add_Mpy * Data_Add_Mpy);
      if (Data < Min)
        Data = Min;
      break;
    case 9:
      Data -= (Data_Add_Mpy * Data_Add_Mpy * Data_Add_Mpy * Data_Add_Mpy
          * Data_Add_Mpy * Data_Add_Mpy * Data_Add_Mpy * Data_Add_Mpy);
      if (Data < Min)
        Data = Min;
      break;
    case 10:
      Data -= (Data_Add_Mpy * Data_Add_Mpy * Data_Add_Mpy * Data_Add_Mpy
          * Data_Add_Mpy * Data_Add_Mpy * Data_Add_Mpy * Data_Add_Mpy
          * Data_Add_Mpy);
      if (Data < Min)
        Data = Min;
      break;
    default:
      break;
    }

    ButtonDown.flg.bit.DI_OUT_F = 0;
  }

  return Data;
}

/*************************************************
 Function: Panel Button 'SHIFT' Data
 Description:
 Input: No
 Return: No
 Others: Be Called Panel1stMenuDisplay()/Panel2ndMenuDisplay()
 *************************************************/
int32_t PanelButtonShift(int32_t Data, int32_t Max)
{
  if (ButtonShift.flg.bit.DI_OUT_F)
  {
    Data++;
    if (Data > Max)
      Data = 1;
    ButtonShift.flg.bit.DI_OUT_F = 0;
  }

  return Data;
}
/*************************************************
 Function: Panel Button 'Mode' Data
 Description:
 Input: No
 Return: No
 Others: Be Called Panel1stMenuDisplay()/Panel2ndMenuDisplay()
 *************************************************/
int32_t PanelButtonMode(int32_t MenuGrade)
{
  if (ButtonMode.flg.bit.DI_OUT_F)
  {
    if (g_3ndDisplayData == JOG_FUNCTION)  //jog�˳�
    {
      g_3ndDisplayData = 0;
      FuncCode_Handle.Ram.F10.F1032 = 0;
      FuncCode_Handle.E2prom.F10.F1032 = 0;
    }
    else if (g_3ndDisplayData == SAVE_FUNCTION)  //�˳� SAVE 3���˵�
    {
      g_3ndDisplayData = 0;
      FuncCode_Handle.Ram.F10.F1001 = 0;
      FuncCode_Handle.E2prom.F10.F1001 = 0;
    }
    MenuGrade--;
    if (MenuGrade < 0)
      MenuGrade = 0;

    ButtonMode.flg.bit.DI_OUT_F = 0;
  }

  return MenuGrade;
}
/*************************************************
 Function: Panel Button 'Set' Data
 Description:
 Input: No
 Return: No
 Others: Be Called Panel1stMenuDisplay()/Panel2ndMenuDisplay()
 *************************************************/
int32_t PanelButtonSet(int32_t MenuGrade, uint32_t Data, uint16_t Fcode) //data���޸ĵ�ֵ��fcode�ǵ�ַ
{
  uint8_t Status = 0;
  if (ButtonSet.flg.bit.DI_OUT_F)  //set����Ч
  {
    if ((MenuGrade == 1) || (MenuGrade == 0))
    {
      MenuGrade++;
      g_2ndSetDataSts = 0;
    }
    else
    {
      //����Ӹ��ֹ��ܣ���ʱֻ��JOGģʽ
      if ((Fcode == 3001) && (Data == 1))   //�㶯RAMֵ�޸ģ����Ϊ1����������
      {
        FuncCode_Handle.Ram.F10.F1032 = 1;
        FuncCode_Handle.E2prom.F10.F1032 = 1;
        g_3ndDisplayData = JOG_FUNCTION;
        MenuGrade++;  //MenuGrade = 3;
      }
      else if (Fcode == 1001)  //SAVE_FUNCTION
      {
        if (!CoreStatusFlag.bit.RUN_OK_F)  // can be modified	when Shutdown
        {
          FuncCode_Handle.Ram.F10.F1001 = Data;
          FuncCode_Handle.E2prom.F10.F1001 = Data;
          //g_3ndDisplayData = SAVE_FUNCTION;
          g_SpecialSave = Data;  //�����ȡ
        }
        g_3ndDisplayData = SAVE_FUNCTION;
        MenuGrade++;
      }
      else  //	MenuGrade = 2;
      {
        Status = (uint8_t) FuncCodeChange(Fcode, Data, &FuncCode_Handle, OK); //�޸��ڴ浥Ԫֵ
        g_2ndSetDataSts = Status | 0x80;
      }
    }
    ButtonSet.flg.bit.DI_OUT_F = 0;  //���set��
  }

  return MenuGrade;
}


/*
 * peri_PanelLEDButton.c
 *
 * Created on: 2014-3-27
 * Author: JH-LvSL
 */
#include <main.h>			//Declarations from DAVE3 Code Generation (includes SFR declaration)
#include "gpio.h"
#include <peri_AlmWarnMacro.h>
#include <peri_GlobalVariablesExtern.h>
#include <peri_PanelLEDButton.h>
#include <peri_SwScan.h>

/*LED Display Data*/
#define LED_Data_SET() 	MX_GPIO_WritePin(D595_GPIO_Port,D595_Pin,0)
#define LED_Data_CLR() 	MX_GPIO_WritePin(D595_GPIO_Port,D595_Pin,1)
/*Storage Clock,Output Enable*/
#define CLK1_STR_SET()	MX_GPIO_WritePin(CLK1_GPIO_Port,CLK1_Pin,0)
#define	CLK1_STR_CLR()	MX_GPIO_WritePin(CLK1_GPIO_Port,CLK1_Pin,1)
/*Data Shift Clock*/
#define	CLK2_SFT_SET()	MX_GPIO_WritePin(CLK2_GPIO_Port,CLK2_Pin,0)
#define	CLK2_SFT_CLR()	MX_GPIO_WritePin(CLK2_GPIO_Port,CLK2_Pin,1)

static void PanelDataShift(uint16_t LEDEnable, uint8_t data);
void PanelLEDDisplay(void);

uint8_t LED[6];
BUTTONFLG ButtonFlag;
BUTTONFLG ButtonFlagOld;
BUTTONFLG ButtonFlagLong;
BUTTONCOUNTS ButtonCounts;
extern void Delay(uint16_t);

/*************************************************
 Function: Panel Data Init
 Description:
 Input: No
 Return: No
 Others: Be Called InitModule()
 *************************************************/
void PanelInit(void)
{
  LED_Data_CLR();
  CLK1_STR_CLR();
  CLK2_SFT_CLR();
}

/*************************************************
 Function: Panel LED&Button Handle
 Description:
 Input: No
 Return: No
 Others: Be Called SysCore1msLoop()
 *************************************************/
void PanelLEDButtonHandle1(void)
{
}
/*************************************************
 Function: Panel Button Identity
 Description:
 Input: No
 Return: No
 Others: Be Called PanelDataShift()
 *************************************************/
static void PanelButtonIdentity(uint8_t button)
{
  uint32_t temp;

  ButtonFlagOld.all = ButtonFlag.all; //��������������
  ButtonFlag.all = ~button; //��������������
  if ((!ButtonFlagOld.bit.up) & (!ButtonFlag.bit.up))
  {
    ButtonCounts.up++;
    if (ButtonCounts.up >= 2)
      ButtonFlagLong.bit.up = 1;
  }
  else
  {
    ButtonCounts.up = 0;
    ButtonFlagLong.bit.up = 0;
  }

  if ((!ButtonFlagOld.bit.down) & (!ButtonFlag.bit.down))
  {
    ButtonCounts.down++;
    if (ButtonCounts.down >= 2)
      ButtonFlagLong.bit.down = 1;
  }
  else
  {
    ButtonCounts.down = 0;
    ButtonFlagLong.bit.down = 0;
  }
  if ((!ButtonFlagOld.bit.set) & (!ButtonFlag.bit.set))
  {
    ButtonCounts.set++;
    if (ButtonCounts.set >= 2)
      ButtonFlagLong.bit.set = 1;
  }
  else
  {
    ButtonCounts.set = 0;
    ButtonFlagLong.bit.set = 0;
  }
  if ((!ButtonFlagOld.bit.shift) & (!ButtonFlag.bit.shift))
  {
    ButtonCounts.shift++;
    if (ButtonCounts.shift >= 2)
      ButtonFlagLong.bit.shift = 1;
  }
  else
  {
    ButtonCounts.shift = 0;
    ButtonFlagLong.bit.shift = 0;
  }
  if ((!ButtonFlagOld.bit.mode) & (!ButtonFlag.bit.mode))
  {
    ButtonCounts.mode++;
    if (ButtonCounts.mode >= 2)
      ButtonFlagLong.bit.mode = 1;
  }
  else
  {
    ButtonCounts.mode = 0;
    ButtonFlagLong.bit.mode = 0;
  }
  temp = ~ButtonFlagOld.all;
  temp = temp & 0x1f;
  temp = ~ButtonFlag.all;
  temp = temp & 0x1f;
  temp = temp & 0x1f;
  temp = ButtonFlagLong.all;
}

/*************************************************
 Function: Panel LED&Button Handle
 Description:
 Input: No
 Return: No
 Others: Be Called SysCore1msLoop()
 *************************************************/
static void PanelDataShift(uint16_t LEDEnable, uint8_t data)
{
  uint16_t temp = 0;
  uint16_t temp0 = 0x4000;
  uint8_t i = 0;
  uint8_t button = 0;

  temp = LEDEnable | data;

  for (i = 0; i < 15; i++)
  {
    CLK2_SFT_CLR();
    if (temp & temp0)
      LED_Data_SET();
    else
      LED_Data_CLR();

    CLK2_SFT_SET();
    if ((i < 7) && (i > 0))
    {
      button = button << 1;
      if (MX_GPIO_ReadPin(D165_GPIO_Port, D165_Pin)) //
        button += 1;
    }
    else if (i == 8)
      PanelButtonIdentity(button);
    temp0 = temp0 >> 1;

  }

}
/*************************************************
 Function: Panel LED Display
 Description:
 Input: No
 Return: No
 Others: Be Called SysCore100usLoop()
 *************************************************/
void PanelLEDDisplay(void)
{
//	uint8_t LED[6];
  static uint8_t LEDCount = 0;
  static uint16_t LEDEnable = 0xFE00;
//	static uint8_t  DisplayCount = 0;

//	DisplayCount++;
//
//	if(DisplayCount < 10)
//	{
//		return;
//	}
//	DisplayCount = 0;

  CLK1_STR_SET();

  PanelDataShift(LEDEnable, LED[LEDCount]);
  LEDEnable = LEDEnable << 1;
  LEDEnable = LEDEnable | 0x100;
  LEDCount++;
  if (LEDCount >= 5)
  {
    LEDCount = 0;
    LEDEnable = 0xFE00;
  }
  CLK1_STR_CLR();

}
/*************************************************
 Function: Panel Data Display
 Description:
 Input: No
 Return: No
 Others: Be Called PanelMainMenuDisplay()
 *************************************************/
void PanelLEDdata(uint32_t Data, uint8_t point, uint8_t DataHL, uint8_t HighFlg)
{
  if ((HighFlg == 1) && (DataHL == 0))
  {
    if (point == 4)
      LED[0] = PanelLEDTable[(uint8_t) ((Data / 10000) % 10)] & OPE_7SEG_LED_DP;
    else
      LED[0] = PanelLEDTable[(uint8_t) ((Data / 10000) % 10)];
    if (point == 3)
      LED[1] = PanelLEDTable[(uint8_t) ((Data / 1000) % 10)] & OPE_7SEG_LED_DP;
    else
      LED[1] = PanelLEDTable[(uint8_t) ((Data / 1000) % 10)];
    if (point == 2)
      LED[2] = PanelLEDTable[(uint8_t) ((Data / 100) % 10)] & OPE_7SEG_LED_DP;
    else
      LED[2] = PanelLEDTable[(uint8_t) ((Data / 100) % 10)];
    if (point == 1)
      LED[3] = PanelLEDTable[(uint8_t) ((Data / 10) % 10)] & OPE_7SEG_LED_DP;
    else
      LED[3] = PanelLEDTable[(uint8_t) ((Data / 10) % 10)];
    LED[4] = PanelLEDTable[(uint8_t) (Data % 10)];

  }
  else
  {
    if ((Data / 10000) % 10)
    {
      if (point == 4)
        LED[0] = PanelLEDTable[(uint8_t) ((Data / 10000) % 10)]
            & OPE_7SEG_LED_DP;
      else
        LED[0] = PanelLEDTable[(uint8_t) ((Data / 10000) % 10)];
      if (point == 3)
        LED[1] =
            PanelLEDTable[(uint8_t) ((Data / 1000) % 10)] & OPE_7SEG_LED_DP;
      else
        LED[1] = PanelLEDTable[(uint8_t) ((Data / 1000) % 10)];
      if (point == 2)
        LED[2] = PanelLEDTable[(uint8_t) ((Data / 100) % 10)] & OPE_7SEG_LED_DP;
      else
        LED[2] = PanelLEDTable[(uint8_t) ((Data / 100) % 10)];
      if (point == 1)
        LED[3] = PanelLEDTable[(uint8_t) ((Data / 10) % 10)] & OPE_7SEG_LED_DP;
      else
        LED[3] = PanelLEDTable[(uint8_t) ((Data / 10) % 10)];
      LED[4] = PanelLEDTable[(uint8_t) (Data % 10)];
      //	LED[5] = PanelLEDTable[(uint8_t)(Data%10)];
    }
    else
    {
      if (point == 4)
      {
        LED[0] = (PanelLEDTable[(uint8_t) ((Data / 10000) % 10)])
            & OPE_7SEG_LED_DP;
        LED[1] = PanelLEDTable[(uint8_t) ((Data / 1000) % 10)];
        LED[2] = PanelLEDTable[(uint8_t) ((Data / 100) % 10)];
        LED[3] = PanelLEDTable[(uint8_t) ((Data / 10) % 10)];
        LED[4] = PanelLEDTable[(uint8_t) (Data % 10)];
      }
      else
      {
        LED[0] = PanelLEDTable[36]; //'NUll'
        if ((Data / 1000) % 10)
        {
          if (point == 3)
            LED[1] = PanelLEDTable[(uint8_t) ((Data / 1000) % 10)]
                & OPE_7SEG_LED_DP;
          else
            LED[1] = PanelLEDTable[(uint8_t) ((Data / 1000) % 10)];
          if (point == 2)
            LED[2] = PanelLEDTable[(uint8_t) ((Data / 100) % 10)]
                & OPE_7SEG_LED_DP;
          else
            LED[2] = PanelLEDTable[(uint8_t) ((Data / 100) % 10)];
          if (point == 1)
            LED[3] = PanelLEDTable[(uint8_t) ((Data / 10) % 10)]
                & OPE_7SEG_LED_DP;
          else
            LED[3] = PanelLEDTable[(uint8_t) ((Data / 10) % 10)];
          LED[4] = PanelLEDTable[(uint8_t) (Data % 10)];
          //	LED[5] = PanelLEDTable[(uint8_t)(Data%10)];
        }
        else
        {
          if (point == 3)
          {
            LED[1] = PanelLEDTable[(uint8_t) ((Data / 1000) % 10)]
                & OPE_7SEG_LED_DP;
            LED[2] = PanelLEDTable[(uint8_t) ((Data / 100) % 10)];
            LED[3] = PanelLEDTable[(uint8_t) ((Data / 10) % 10)];
            LED[4] = PanelLEDTable[(uint8_t) (Data % 10)];
          }
          else
          {
            LED[1] = PanelLEDTable[36]; //'NUll'
            if ((Data / 100) % 10)
            {
              if (point == 2)
                LED[2] = PanelLEDTable[(uint8_t) ((Data / 100) % 10)]
                    & OPE_7SEG_LED_DP;
              else
                LED[2] = PanelLEDTable[(uint8_t) ((Data / 100) % 10)];
              if (point == 1)
                LED[3] = PanelLEDTable[(uint8_t) ((Data / 10) % 10)]
                    & OPE_7SEG_LED_DP;
              else
                LED[3] = PanelLEDTable[(uint8_t) ((Data / 10) % 10)];
              LED[4] = PanelLEDTable[(uint8_t) (Data % 10)];
              //	LED[5] = PanelLEDTable[(uint8_t)(Data%10)];
            }
            else
            {
              if (point == 2)
              {
                LED[2] = PanelLEDTable[(uint8_t) ((Data / 100) % 10)]
                    & OPE_7SEG_LED_DP;
                LED[3] = PanelLEDTable[(uint8_t) ((Data / 10) % 10)];
                LED[4] = PanelLEDTable[(uint8_t) (Data % 10)];
              }
              else
              {
                LED[2] = PanelLEDTable[36]; //'NUll'
                if (point == 1)
                  LED[3] = PanelLEDTable[(uint8_t) ((Data / 10) % 10)]
                      & OPE_7SEG_LED_DP;
                else
                {
                  if ((Data / 10) % 10)
                    LED[3] = PanelLEDTable[(uint8_t) ((Data / 10) % 10)];
                  else
                    LED[3] = PanelLEDTable[36]; //'NUll'
                }
                //if(DataLength)
                //LED[4] = PanelLEDTable[(uint8_t)(Data%10)] & OPE_7SEG_LED_DP;
                //else
                LED[4] = PanelLEDTable[(uint8_t) (Data % 10)];
              }
            }
          }
        }
      }
    }
  }

}
/*************************************************
 Function: Panel Data Display
 Description:
 Input: No
 Return: No
 Others: Be Called PanelMainMenuDisplay()
 *************************************************/
void PanelDataDisplay(int32_t DataValue, uint32_t Att, uint8_t DataHL,
    CoreStatusFlg *p)
{
  static uint16_t DisplayCount = 0;
  uint32_t ary = 0, point = 0, length = 0, sign = 0;
  int32_t Data = 0;
  uint8_t DataHighFlg = 0;
  uint8_t SignFlg = 0;

  /*Data Attribute Resolve*/
  ary = Att & 0x01;
  length = (Att >> 1) & 0x01;
  point = (Att >> 2) & 0x03;
  sign = (Att >> 4) & 0x01;

  if (DataValue <= 99999)
  {
    DataHL = 0;
    DataHighFlg = 0;
  }
  else
    DataHighFlg = 1;

  if ((DataValue < 0) && sign)
    SignFlg = 1;

  if (DataHL)
    Data = DataValue / 100000;
  else
    Data = DataValue;

  DisplayCount++;
  if (ary) //16Hex
  {
    LED[0] = PanelLEDTable[(uint8_t) ((Data >> 16) & 0xF)];
    LED[1] = PanelLEDTable[(uint8_t) ((Data >> 12) & 0xF)];
    LED[2] = PanelLEDTable[(uint8_t) ((Data >> 8) & 0xF)];
    LED[3] = PanelLEDTable[(uint8_t) ((Data >> 4) & 0xF)];
    LED[4] = PanelLEDTable[(uint8_t) (Data & 0xF)];
//		LED[5] = PanelLEDTable[(uint8_t)(Data & 0xF)];
  }
  else //10Dec
  {
    if (Data < 0)
      Data = -Data;

    PanelLEDdata(Data, point, DataHL, DataHighFlg);

    if (SignFlg)
    {
      LED[0] = LED[0] & OPE_7SEG_LED_DP;
    }
  }
  LED[5] = OPE_7SEG_LED_NULL;

  if (length)
  {
    if (DataHL)
    {
      LED[3] = LED[3] & OPE_7SEG_LED_DP;
      LED[4] = LED[4] & OPE_7SEG_LED_DP;
    }
    else
      LED[4] = LED[4] & OPE_7SEG_LED_DP;
  }

  if (!p->bit.RUN_OK_F)
  {
    //if(DisplayCount%1000 >= 500)
    if (DisplayCount & 0x0200) //201801011
    {
      LED[0] = OPE_7SEG_LED_NULL;
      LED[1] = OPE_7SEG_LED_NULL;
      LED[2] = OPE_7SEG_LED_NULL;
      LED[3] = OPE_7SEG_LED_NULL;
      LED[4] = OPE_7SEG_LED_NULL;
      LED[5] = OPE_7SEG_LED_NULL;
    }
  }

  //if(DisplayCount >= 60000)
  //	DisplayCount = 0;

}
/*************************************************
 Function: Panel Alm Display
 Description:
 Input: No
 Return: No
 Others: Be Called PanelMainMenuDisplay()
 *************************************************/
void PanelAlmDisplay(uint8_t AlmNum)
{
  if (g_LEDErrDisplayMode)
  {
    LED[0] = PanelAlmTable1[AlmNum][0];
    LED[1] = PanelAlmTable1[AlmNum][1];
    LED[2] = PanelAlmTable1[AlmNum][2];
    LED[3] = PanelAlmTable1[AlmNum][3];
    LED[4] = PanelAlmTable1[AlmNum][4];
  }
  else
  {
    LED[0] = PanelAlmTable[AlmNum][0];
    LED[1] = PanelAlmTable[AlmNum][1];
    LED[2] = PanelAlmTable[AlmNum][2];
    LED[3] = PanelAlmTable[AlmNum][3];
    LED[4] = PanelAlmTable[AlmNum][4];
  }
  LED[5] = OPE_7SEG_LED_NULL;
}
/*************************************************
 Function: Panel Menu 1st Grade Display
 Description:
 Input: No
 Return: No
 Others: Be Called Panel1stMenuDisplay()
 *************************************************/
void Panel1stMenuDataDisplay(uint16_t Data, uint8_t twinkle)
{
  static uint16_t DisplayCount = 0;

  DisplayCount++;
  if (DisplayCount > 60000)
    DisplayCount = 0;

  LED[0] = PanelLEDTable[15];	//'F'
  LED[1] = PanelLEDTable[(uint8_t) ((Data / 100) % 16)];
  LED[2] = PanelLEDTable[38];	//'-'
  LED[3] = PanelLEDTable[(uint8_t) ((Data / 10) % 10)];
  LED[4] = PanelLEDTable[(uint8_t) (Data % 10)];
  LED[5] = OPE_7SEG_LED_NULL;

  switch (twinkle)
  {
  case 3:
    if (DisplayCount % 800 > 400)
      LED[1] = OPE_7SEG_LED_NULL;
    break;
  case 2:
    if (DisplayCount % 800 > 400)
      LED[4] = OPE_7SEG_LED_NULL;
    break;
  case 1:
    if (DisplayCount % 800 > 400)
      LED[3] = OPE_7SEG_LED_NULL;
    break;
  default:
    break;
  }
}

/*************************************************
 Function: Panel Menu 2nd Grade Display
 Description:
 Input: No
 Return: No
 Others: Be Called Panel2ndMenuDisplay()
 *************************************************/
void Panel2ndMenuDataDisplay(int32_t DValue, uint8_t twinkle, uint32_t Att)
{
  static uint16_t DisplayCount = 0;
  uint32_t point = 0;
  uint32_t Data = 0;
  uint8_t DataHighFlg = 0;
  int32_t DataValue = 0;

  DataValue = DValue;
  if (DataValue < 0)
    DataValue = -DataValue;

  if (DataValue > 99999)
    DataHighFlg = 1;
  /*Judge Display Data*/
  if (twinkle > 5)
    Data = DataValue / 100000;
  else
    Data = DataValue;

  point = (Att >> 6) & 0x03;

  DisplayCount++;
  if (DisplayCount > 60000)
    DisplayCount = 0;

  if (!twinkle)
  {
    if (Att & 0x0020)	//������������������16������������
    {
      LED[0] = PanelLEDTable[(uint8_t) ((Data >> 16) & 0xF)];
      LED[1] = PanelLEDTable[(uint8_t) ((Data >> 12) & 0xF)];
      LED[2] = PanelLEDTable[(uint8_t) ((Data >> 8) & 0xF)];
      LED[3] = PanelLEDTable[(uint8_t) ((Data >> 4) & 0xF)];
      LED[4] = PanelLEDTable[(uint8_t) (Data & 0xF)];
    }
    else
    {
      PanelLEDdata(Data, point, !(Att >> 11) & 0x01, DataHighFlg);
      if ((Att >> 11) & 0x01)
      {
        LED[4] = PanelLEDTable[(uint8_t) (Data % 10)] & OPE_7SEG_LED_DP;
      }
    }
  }
  else	//LED��������������������
  {
    if (Att & 0x0020)	//������������������16��������������
    {
      LED[0] = PanelLEDTable[(uint8_t) ((Data >> 16) & 0xF)];
      LED[1] = PanelLEDTable[(uint8_t) ((Data >> 12) & 0xF)];
      LED[2] = PanelLEDTable[(uint8_t) ((Data >> 8) & 0xF)];
      LED[3] = PanelLEDTable[(uint8_t) ((Data >> 4) & 0xF)];
      LED[4] = PanelLEDTable[(uint8_t) (Data & 0xF)];
    }
    else
    {

      if (point == 4)
        LED[0] = PanelLEDTable[(uint8_t) ((Data / 10000) % 10)]
            & OPE_7SEG_LED_DP;
      else
        LED[0] = PanelLEDTable[(uint8_t) ((Data / 10000) % 10)];
      if (point == 3)
        LED[1] =
            PanelLEDTable[(uint8_t) ((Data / 1000) % 10)] & OPE_7SEG_LED_DP;
      else
        LED[1] = PanelLEDTable[(uint8_t) ((Data / 1000) % 10)];
      if (point == 2)
        LED[2] = PanelLEDTable[(uint8_t) ((Data / 100) % 10)] & OPE_7SEG_LED_DP;
      else
        LED[2] = PanelLEDTable[(uint8_t) ((Data / 100) % 10)];
      if (point == 1)
        LED[3] = PanelLEDTable[(uint8_t) ((Data / 10) % 10)] & OPE_7SEG_LED_DP;
      else
        LED[3] = PanelLEDTable[(uint8_t) ((Data / 10) % 10)];
      LED[4] = PanelLEDTable[(uint8_t) (Data % 10)];

      if ((Att >> 11) & 0x01)
      {
        if (twinkle > 5)
        {
          LED[3] =
              PanelLEDTable[(uint8_t) ((Data / 10) % 10)] & OPE_7SEG_LED_DP;
          LED[4] = PanelLEDTable[(uint8_t) (Data % 10)] & OPE_7SEG_LED_DP;
        }
        else
        {
          LED[4] = PanelLEDTable[(uint8_t) (Data % 10)] & OPE_7SEG_LED_DP;
        }
      }
    }

    switch (twinkle)
    {
    case 1:
    case 6:
      if (DisplayCount % 1000 > 500)
        LED[4] = OPE_7SEG_LED_NULL;
      break;
    case 2:
    case 7:
      if (DisplayCount % 1000 > 500)
        LED[3] = OPE_7SEG_LED_NULL;
      break;
    case 3:
    case 8:
      if (DisplayCount % 1000 > 500)
        LED[2] = OPE_7SEG_LED_NULL;
      break;
    case 4:
    case 9:
      if (DisplayCount % 1000 > 500)
        LED[1] = OPE_7SEG_LED_NULL;
      break;
    case 5:
    case 10:
      if (DisplayCount % 1000 > 500)
        LED[0] = OPE_7SEG_LED_NULL;
      break;
    default:
      break;
    }
  }
  if ((DValue < 0) && (Att >> 2) & 0x01)//add point when the datavalue is lower then 0
  {
    LED[0] = LED[0] & OPE_7SEG_LED_DP;
    //LED[1] = LED[1] & OPE_7SEG_LED_DP;
  }
}

/*************************************************
 Function: Panel Button 'Set' Data
 Description:Status Display When Key 'Set' is used in the 2nd menu.
 Input: No
 Return: No
 Others: Be Called Panel1stMenuDisplay()/Panel2ndMenuDisplay()
 *************************************************/
void Panel2ndMenuDisplaySts(uint8_t Sts)
{
  LED[0] = PanelDataStsTable[Sts][0];
  LED[1] = PanelDataStsTable[Sts][1];
  LED[2] = PanelDataStsTable[Sts][2];
  LED[3] = PanelDataStsTable[Sts][3];
  LED[4] = PanelDataStsTable[Sts][4];
}

/*************************************************
 Function: Panel Init Dispaly
 Description:
 Input: No
 Return: No
 Others: Be Called InitModule()
 *************************************************/
void PanelInitDisplay(void)
{
  //uint8_t  i;

  LED[0] = OPE_7SEG_LED_NULL;	//OPE_7SEG_LED_ZP;
  LED[1] = OPE_7SEG_LED_NULL;	//OPE_7SEG_LED_J;
  LED[2] = OPE_7SEG_LED_NULL;	//OPE_7SEG_LED_NULL;
  LED[3] = OPE_7SEG_LED_NULL;	//H
  LED[4] = OPE_7SEG_LED_NULL;	//OPE_7SEG_LED_ZP;

//	for(i=0;i<5;i++)
//	{
  //	PanelLEDDisplay();
  //	Delay(1000);
//	}
}
//PanelLEDDisplay

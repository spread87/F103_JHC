//############################################################################
// FILE:	  peri_sw_scan.c
// CONTENT:	  IO filter treat
// AUTHOR:    LvSL
// DATE:      2013/07/03
//############################################################################
#include <main.h>			//Declarations from DAVE3 Code Generation (includes SFR declaration)
#include <peri_GlobalVariablesExtern.h>
#include <peri_PanelLEDButton.h>
#include <peri_SwScan.h>



// Define constants used:
#define   BUTTON_SCAN_DLY_VAL       	(uint16_t)5       // 5ms
#define   BUTTON_SCAN_CNT           		(uint16_t)20       // 20 times

#define   DI_SCAN_DLY_VAL       	(uint16_t)3       // 5->3ms
#define   DI_SCAN_CNT           		(uint16_t)5       // 5 times  20171009 20->5

#define   POSITIVE_LOGIC							(uint8_t)0
#define   NEGATIVE_LOGIC						(uint8_t)1


SW_SCAN_FUN ButtonUp;
SW_SCAN_FUN ButtonDown;
SW_SCAN_FUN ButtonShift;
SW_SCAN_FUN ButtonMode;
SW_SCAN_FUN ButtonSet;
SW_SCAN_FUN DI9;
SW_SCAN_FUN DI1;
SW_SCAN_FUN DI2;
SW_SCAN_FUN DI3;
SW_SCAN_FUN DI4;
SW_SCAN_FUN DI5;
SW_SCAN_FUN DI6;
SW_SCAN_FUN DI7;
SW_SCAN_FUN DI8;

//##########################################################################	
// Code Start:
//##########################################################################
/**
 * @brief initializtion Scan DI port
 * @param[in]
 * @return
 *
 */
void InitButtonSwitchTime(SW_SCAN_FUN *p)
{
	p->flg.all  = 0;
	p->SCAN_CNT = 0;
	p->TRUE_CNT = 0;
	p->SET_SCAN_CNT = BUTTON_SCAN_CNT;
	p->SET_TIME_DELAY = BUTTON_SCAN_DLY_VAL;
}

void InitDiSwitchTime(SW_SCAN_FUN *p)
{
	p->flg.all  = 0;
	p->SCAN_CNT = 0;
	p->TRUE_CNT = 0;
	p->SET_SCAN_CNT = DI_SCAN_CNT;
	p->SET_TIME_DELAY = DI_SCAN_DLY_VAL;
}

/**
 * @brief initializtion Scan DI port
 * @param[in]
 * @return
 *
 */
void InitScanDiSwitch(void)
{
	/* Button*/

	InitButtonSwitchTime(&ButtonUp);
	InitButtonSwitchTime(&ButtonDown);
	InitButtonSwitchTime(&ButtonShift);
	InitButtonSwitchTime(&ButtonMode);
	InitButtonSwitchTime(&ButtonSet);

	/*Terminal DI*/
	InitDiSwitchTime(&DI9);
	InitDiSwitchTime(&DI1);
	InitDiSwitchTime(&DI2);
	InitDiSwitchTime(&DI3);
	InitDiSwitchTime(&DI4);
	InitDiSwitchTime(&DI5);
	InitDiSwitchTime(&DI6);
	InitDiSwitchTime(&DI7);
	InitDiSwitchTime(&DI8);
}

/**
 * @brief Scan DI port
 * @param[in]
 * @return
 *
 */
void ScanDiSwitch(SW_SCAN_FUN *p)
{
	if(p->flg.bit.DI_sta_F == 0)
	{
		// DI on scan
        p->flg.bit.DI_OFF_DLY_F = 0;

		if(p->SCAN_CNT >= p->SET_SCAN_CNT)
		{
            if(p->TRUE_CNT >= 4)
			{
                p->flg.bit.DI_sta_F = 1;
			}
            else
			{
			    // do nothing
			}
			p->SCAN_CNT = 0;
            p->TRUE_CNT = 0;
            p->flg.bit.DI_ON_DLY_F = 0;
		}
		else
		{
            if(p->flg.bit.DI_ON_DLY_F == 1)
            {
                if((uint16_t)(Timer1msCount - p->TIM_BASE) >= p->SET_TIME_DELAY)
                {
                    if(p->flg.bit.DI_PIN_F == 1)
                    {
					    p->TRUE_CNT++;
                    }
                    else
                    {
                        // do nothing
                    }
                    p->SCAN_CNT++;
					p->TIM_BASE = Timer1msCount;
                }
                else
                {
                    // do nothing
                }
            }
            else
            {
                if(p->flg.bit.DI_PIN_F == 1)
                {
                    p->flg.bit.DI_ON_DLY_F = 1;
                    p->TIM_BASE = Timer1msCount;
                    p->TRUE_CNT++;
                    p->SCAN_CNT++;
                }
			    else
			    {
                    p->TRUE_CNT = 0;
                    p->SCAN_CNT = 0;
			    }
		    } 
		}
	}
	else
	{
		// DI off scan
		p->flg.bit.DI_ON_DLY_F = 0;

		if(p->SCAN_CNT >= p->SET_SCAN_CNT)
		{
            if(p->TRUE_CNT >= 4)
			{
                p->flg.bit.DI_sta_F = 0;
			}
            else
			{
			    // do nothing
			}
			p->SCAN_CNT = 0;
            p->TRUE_CNT = 0;
            p->flg.bit.DI_OFF_DLY_F = 0;
		}
        else
		{
            if(p->flg.bit.DI_OFF_DLY_F == 1)
            {
                if((uint16_t)(Timer1msCount - p->TIM_BASE) >= p->SET_TIME_DELAY)
                {
                    if(p->flg.bit.DI_PIN_F == 0)
                    {
                        p->TRUE_CNT++;
                    }
                    else
                    {
					    // do nothing
                    }
                    p->SCAN_CNT++;
					p->TIM_BASE = Timer1msCount;
                }
                else
                {
				    // do nothing
                }
            }
            else
            {
                if(p->flg.bit.DI_PIN_F == 0)
                {
                    p->flg.bit.DI_OFF_DLY_F = 1;
                    p->TIM_BASE = Timer1msCount;
                    p->TRUE_CNT++;
                    p->SCAN_CNT++;
                }
                else
                {
				    p->TRUE_CNT = 0;
                    p->SCAN_CNT = 0;
                }
            } 
		}
	}
}
/**
 * @brief Scan Button port
 * @param[in]
 * @return
 *
 */
void ScanButton(SW_SCAN_FUN *p)
{
	if(p->flg.bit.DI_sta_F == 0)
	{
		// DI on scan
        p->flg.bit.DI_OFF_DLY_F = 0;

		if(p->SCAN_CNT >= p->SET_SCAN_CNT)
		{
            if(p->TRUE_CNT >= (p->SET_SCAN_CNT - 1))
			{
                p->flg.bit.DI_sta_F = 1;
			}
            else
			{
			    // do nothing
			}
			p->SCAN_CNT = 0;
            p->TRUE_CNT = 0;
            p->flg.bit.DI_ON_DLY_F = 0;
		}
		else
		{
            if(p->flg.bit.DI_ON_DLY_F == 1)
            {
                if((uint16_t)(Timer1msCount - p->TIM_BASE) >= p->SET_TIME_DELAY)
                {
                    if(p->flg.bit.DI_PIN_F == 1)
                    {
					    p->TRUE_CNT++;
                    }
                    else
                    {
                        // do nothing
                    }
                    p->SCAN_CNT++;
					p->TIM_BASE = Timer1msCount;
                }
                else
                {
                    // do nothing
                }
            }
            else
            {
                if(p->flg.bit.DI_PIN_F == 1)
                {
                    p->flg.bit.DI_ON_DLY_F = 1;
                    p->TIM_BASE = Timer1msCount;
                    p->TRUE_CNT++;
                    p->SCAN_CNT++;
                }
			    else
			    {
                    p->TRUE_CNT = 0;
                    p->SCAN_CNT = 0;
			    }
		    }
		}
	}
	else
	{
		// DI off scan
		p->flg.bit.DI_ON_DLY_F = 0;

		if(p->SCAN_CNT >= p->SET_SCAN_CNT)
		{
            if(p->TRUE_CNT >= (p->SET_SCAN_CNT - 1))
			{
                p->flg.bit.DI_sta_F = 0;
                p->flg.bit.DI_OUT_F = 1;
			}
            else
			{
			    // do nothing
			}
			p->SCAN_CNT = 0;
            p->TRUE_CNT = 0;
            p->flg.bit.DI_OFF_DLY_F = 0;
		}
        else
		{
            if(p->flg.bit.DI_OFF_DLY_F == 1)
            {
                if((uint16_t)(Timer1msCount - p->TIM_BASE) >= p->SET_TIME_DELAY)
                {
                    if(p->flg.bit.DI_PIN_F == 0)
                    {
                        p->TRUE_CNT++;
                    }
                    else
                    {
					    // do nothing
                    }
                    p->SCAN_CNT++;
					p->TIM_BASE = Timer1msCount;
                }
                else
                {
				    // do nothing
                }
            }
            else
            {
                if(p->flg.bit.DI_PIN_F == 0)
                {
                    p->flg.bit.DI_OFF_DLY_F = 1;
                    p->TIM_BASE = Timer1msCount;
                    p->TRUE_CNT++;
                    p->SCAN_CNT++;
                }
                else
                {
				    p->TRUE_CNT = 0;
                    p->SCAN_CNT = 0;
                }
            }
		}
	}
}

/**
 * @brief Read Di port value
 * @param[in]
 * @return
 * PortAtt : 0:POSITIVE_LOGIC/1:NEGATIVE_LOGIC
 */
void ReadDiPortInfo(uint8_t PortInfo,uint8_t PortAtt,SW_SCAN_FUN *p)
{
	if(PortInfo)
	{
		if(!PortAtt)
			p->flg.bit.DI_PIN_F = 1;
		else
			p->flg.bit.DI_PIN_F = 0;
	}
	else
	{
		if(!PortAtt)
			p->flg.bit.DI_PIN_F = 0;
		else
			p->flg.bit.DI_PIN_F = 1;
	}
}
/**
 * @brief Read Di port value
 * @param[in]
 * @return
 *	MX_GPIO_ReadPin(IO004_Handle11)
 */
void ReadDiPort(void)
{
    // button 'Up'
	ReadDiPortInfo((uint8_t)ButtonFlag.bit.up,POSITIVE_LOGIC,&ButtonUp);
    // button 'Down'
	ReadDiPortInfo((uint8_t)ButtonFlag.bit.down,POSITIVE_LOGIC,&ButtonDown);
     // button 'Shift'
	ReadDiPortInfo((uint8_t)ButtonFlag.bit.shift,POSITIVE_LOGIC,&ButtonShift);
    // button 'Mode'
	ReadDiPortInfo((uint8_t)ButtonFlag.bit.mode,POSITIVE_LOGIC,&ButtonMode);
    // button 'Set'
	ReadDiPortInfo((uint8_t)ButtonFlag.bit.set,POSITIVE_LOGIC,&ButtonSet); //��ʱ����


	ReadDiPortInfo((uint8_t)MX_GPIO_ReadPin(DO1_GPIO_Port, DO1_Pin),NEGATIVE_LOGIC,&DI1);//0.9
	ReadDiPortInfo((uint8_t)MX_GPIO_ReadPin(DO2_GPIO_Port, DO2_Pin),NEGATIVE_LOGIC,&DI2);//0.10
	ReadDiPortInfo((uint8_t)MX_GPIO_ReadPin(DO3_GPIO_Port, DO3_Pin),NEGATIVE_LOGIC,&DI9);//0.11
	ReadDiPortInfo((uint8_t)MX_GPIO_ReadPin(DO4_GPIO_Port, DO4_Pin),NEGATIVE_LOGIC,&DI8);//0.12
//	ReadDiPortInfo((uint8_t)MX_GPIO_ReadPin(IO004_Handle15),NEGATIVE_LOGIC,&DI7);//1.0
//	ReadDiPortInfo((uint8_t)MX_GPIO_ReadPin(IO004_Handle30),NEGATIVE_LOGIC,&DI6);//1.10
//	ReadDiPortInfo((uint8_t)MX_GPIO_ReadPin(IO004_Handle17),NEGATIVE_LOGIC,&DI5);//1.11
//	ReadDiPortInfo((uint8_t)MX_GPIO_ReadPin(IO001_Handle12),NEGATIVE_LOGIC,&DI3);//15.2
//	ReadDiPortInfo((uint8_t)MX_GPIO_ReadPin(IO001_Handle13),NEGATIVE_LOGIC,&DI4);//15.3

	//Temp = MX_GPIO_ReadPin(IO001_Handle12);//P15.2,Encoder V Phase
	//Temp = MX_GPIO_ReadPin(IO001_Handle13);//P15.3,Encoder W Phase
}

/**
 * @brief Update all Di port
 * @param[in]
 * @return
 *
 */
void UpdateAllDiPort(void)
{
    ReadDiPort();//DI_PIN_F
    /* Button Filter*/
    ScanButton(&ButtonUp);//DI_OUT_F
    ScanButton(&ButtonDown);
    ScanButton(&ButtonShift);
    ScanButton(&ButtonMode);
    ScanButton(&ButtonSet);
   /* DI Terminal Filter*/
//    ScanDiSwitch(&DI9);
    ScanDiSwitch(&DI1);
    ScanDiSwitch(&DI2);
    ScanDiSwitch(&DI3);
    ScanDiSwitch(&DI4);
//    ScanDiSwitch(&DI5);
//    ScanDiSwitch(&DI6);
//    ScanDiSwitch(&DI7);
//    ScanDiSwitch(&DI8);
}


//############################################################################
// No More.
//############################################################################


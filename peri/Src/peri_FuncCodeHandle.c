/**********************************************************
Copyright (c) 2014 JIANGSU JOINHOPE ELECTRIC CO.,LTD
FileName: peri_FuncCodeHandle.c
Author: LvSL
Date: 2014.02.13
Description:   Function Code Handle,include inititial and change function code
Version:
History:
    1. Date: 2014.02.13
       Author: LvSL
       Modification:  1st Version
    2...
***********************************************************/

#include <main.h>
#include <peri_FuncCodeHandle.h>
#include <peri_FuncCodeStruct.h>
#include <peri_GlobalVariablesExtern.h>
#include <peri_PanelFunc.h>

extern const FuncCodeSingleF00 FC_InitF00;
extern const FuncCodeSingleF00 FC_MinF00;
extern const FuncCodeSingleF00 FC_MaxF00;
extern const FuncCodeSingleF00 FC_AttF00;

extern const FuncCodeSingleF10 FC_InitF10;
extern const FuncCodeSingleF10 FC_MinF10;
extern const FuncCodeSingleF10 FC_MaxF10;
extern const FuncCodeSingleF10 FC_AttF10;

extern const FuncCodeSingleF50 FC_InitF50;
extern const FuncCodeSingleF50 FC_MinF50;
extern const FuncCodeSingleF50 FC_MaxF50;
extern const FuncCodeSingleF50 FC_AttF50;

extern const FuncCodeSingleF60 FC_InitF60;
extern const FuncCodeSingleF60 FC_MinF60;
extern const FuncCodeSingleF60 FC_MaxF60;
extern const FuncCodeSingleF60 FC_AttF60;

extern const FuncCodeSingleFA0 FC_InitFA0;
extern const FuncCodeSingleFA0 FC_MinFA0;
extern const FuncCodeSingleFA0 FC_MaxFA0;
extern const FuncCodeSingleFA0 FC_AttFA0;

const struct Fcode_FuncTbl FcodeChangeTbl[] =
{

};
/*************************************************
  Function: Init Function Code
  Description: Init Default Value,min,max,attribute
  Input: &FuncCode
  Return: No
  Others: No
*************************************************/
void FuncCodeInit(void)
{
	FuncCode_Handle.Init.F00 = FC_InitF00;
	FuncCode_Handle.Min.F00 = FC_MinF00;
	FuncCode_Handle.Max.F00 = FC_MaxF00;
	FuncCode_Handle.Att.F00 = FC_AttF00;

	FuncCode_Handle.Init.F50 = FC_InitF50;
	FuncCode_Handle.Min.F50 = FC_MinF50;
	FuncCode_Handle.Max.F50 = FC_MaxF50;
	FuncCode_Handle.Att.F50 = FC_AttF50;

	FuncCode_Handle.Init.F60 = FC_InitF60;
	FuncCode_Handle.Min.F60 = FC_MinF60;
	FuncCode_Handle.Max.F60 = FC_MaxF60;
	FuncCode_Handle.Att.F60 = FC_AttF60;

	FuncCode_Handle.Init.FA0 = FC_InitFA0;
	FuncCode_Handle.Min.FA0 = FC_MinFA0;
	FuncCode_Handle.Max.FA0 = FC_MaxFA0;
	FuncCode_Handle.Att.FA0 = FC_AttFA0;
}

/*************************************************
  Function: FuncCodeChange
  Description: change Function Codevalue
  Input: &FuncCode_Handle,Fcode,value
  Return: 0:change ok;1:don't change because running;
  	  	  2:change ok after power reset;3:only read;
  Others: No
*************************************************/
uint16_t FuncCodeChange(uint16_t Fcode,uint32_t value,FuncCode *FC_Handle,uint8_t FcodeChangeAtt)
{
	uint16_t FC_Group = 0;
	uint16_t FC_Number= 0;
	uint32_t FC_Att = 0;
	uint16_t FC_Result;

	//mxj=============================20160824=======
	FC_Group = FuncCodeReadDetect(Fcode,value);
	if(FC_Group)
	{
		FC_Result = FCODE_VALUE_ERROR;
		return FC_Result;
	}
	//mxj=============================20160824=======

	FC_Group = Fcode/100;
	FC_Number = Fcode%100;

	switch(FC_Group)
	{
		case 00: //F00xx  22RS,23LD,24LQ
			//mxj=============================20160824=======
			if(FC_Number >= FCODE_F00_Num)
			{
				FC_Result = FCODE_ERROR;
			}
			else
			{
			//mxj=============================20160824=======

			FC_Att =*(&FC_Handle->Att.F00.F0000 + FC_Number);
			if((FC_Att & 0x0018) == 0)
			{
				*(&FC_Handle->Ram.F00.F0000 + FC_Number) = value;
				*(&FC_Handle->E2prom.F00.F0000 + FC_Number) = value;
				if(FcodeChangeAtt)
				*(&FC_Handle->Att.F00.F0000 + FC_Number) |=0x0100;
				FC_Result = FCODE_CHANGE_OK;
			}
			else if((FC_Att & 0x0018) == 0x0008)
			{
				if(CoreStatusFlag.bit.RUN_OK_F)
				FC_Result = FCODE_STOP_CHANGE;
				else
				{
					*(&FC_Handle->Ram.F00.F0000 + FC_Number) = value;
					*(&FC_Handle->E2prom.F00.F0000 + FC_Number) = value;
					if(FcodeChangeAtt)
					*(&FC_Handle->Att.F00.F0000 + FC_Number) |=0x0100;
					FC_Result = FCODE_CHANGE_OK;
				}
			}
			else if((FC_Att & 0x0018) == 0x0010)
			{
				*(&FC_Handle->E2prom.F00.F0000 + FC_Number) = value;
				if(FcodeChangeAtt)
				*(&FC_Handle->Att.F00.F0000 + FC_Number) |=0x0100;
				else
				*(&FC_Handle->Ram.F00.F0000 + FC_Number) = value;
				FC_Result = FCODE_POWERRESET_CHANGE;
			}
			else if((FC_Att & 0x0018) == 0x0018)
			{
				FC_Result = FCODE_ONLY_READ;
			}
			}
		break;

		case 50://F50xx
			//mxj=============================20160824=======
			if(FC_Number >= FCODE_F50_Num)
			{
				FC_Result = FCODE_ERROR;
			}
			else
			{
			//mxj=============================20160824=======
			FC_Att =*(&FC_Handle->Att.F50.F5000 + FC_Number);
			if((FC_Att & 0x0018) == 0)
			{
				*(&FC_Handle->Ram.F50.F5000 + FC_Number) = value;
				*(&FC_Handle->E2prom.F50.F5000 + FC_Number) = value;
				if(FcodeChangeAtt)
				*(&FC_Handle->Att.F50.F5000 + FC_Number) |=0x0100;
				FC_Result = FCODE_CHANGE_OK;
			}
			else if((FC_Att & 0x0018) == 0x0008)
			{
				if(CoreStatusFlag.bit.RUN_OK_F)
				FC_Result = FCODE_STOP_CHANGE;
				else
				{
					*(&FC_Handle->Ram.F50.F5000 + FC_Number) = value;
					*(&FC_Handle->E2prom.F50.F5000 + FC_Number) = value;
					if(FcodeChangeAtt)
					*(&FC_Handle->Att.F50.F5000 + FC_Number) |=0x0100;
					FC_Result = FCODE_CHANGE_OK;
				}
			}
			else if((FC_Att & 0x0018) == 0x0010)
			{
				*(&FC_Handle->E2prom.F50.F5000 + FC_Number) = value;
				if(FcodeChangeAtt)
				*(&FC_Handle->Att.F50.F5000 + FC_Number) |=0x0100;
				else
				*(&FC_Handle->Ram.F50.F5000 + FC_Number) = value;
				FC_Result = FCODE_POWERRESET_CHANGE;
			}
			else if((FC_Att & 0x0018) == 0x0018)
			{
				FC_Result = FCODE_ONLY_READ;
			}
			}
		break;

		case 60://F60xx
			//mxj=============================20160824=======
			if(FC_Number >= FCODE_F60_Num)
			{
				FC_Result = FCODE_ERROR;
			}
			else
			{
			//mxj=============================20160824=======
			FC_Att =*(&FC_Handle->Att.F60.F6000 + FC_Number);
			if((FC_Att & 0x0018) == 0)
			{
				*(&FC_Handle->Ram.F60.F6000 + FC_Number) = value;
				*(&FC_Handle->E2prom.F60.F6000+ FC_Number) = value;
				if(FcodeChangeAtt)
				*(&FC_Handle->Att.F60.F6000 + FC_Number) |=0x0100;
				FC_Result = FCODE_CHANGE_OK;
			}
			else if((FC_Att & 0x0018) == 0x0008)
			{
				if(CoreStatusFlag.bit.RUN_OK_F)
				FC_Result = FCODE_STOP_CHANGE;
				else
				{
					*(&FC_Handle->Ram.F60.F6000 + FC_Number) = value;
					*(&FC_Handle->E2prom.F60.F6000+ FC_Number) = value;
					if(FcodeChangeAtt)
					*(&FC_Handle->Att.F60.F6000 + FC_Number) |=0x0100;
					FC_Result = FCODE_CHANGE_OK;
				}
			}
			else if((FC_Att & 0x0018) == 0x0010)
			{
				*(&FC_Handle->E2prom.F60.F6000+ FC_Number) = value;
				if(FcodeChangeAtt)
				*(&FC_Handle->Att.F60.F6000 + FC_Number) |=0x0100;
				else
				*(&FC_Handle->Ram.F60.F6000 + FC_Number) = value;
				FC_Result = FCODE_POWERRESET_CHANGE;
			}
			else if((FC_Att & 0x0018) == 0x0018)
			{
				FC_Result = FCODE_ONLY_READ;
			}
			}
		break;

		case 100://FA0xx
			//mxj=============================20160824=======
			if(FC_Number >= FCODE_FA0_Num)
			{
				FC_Result = FCODE_ERROR;
			}
			else
			{
			//mxj=============================20160824=======
			FC_Att =*(&FC_Handle->Att.FA0.FA000 + FC_Number);
			if((FC_Att & 0x0018) == 0)
			{
				*(&FC_Handle->Ram.FA0.FA000 + FC_Number) = value;
				*(&FC_Handle->E2prom.FA0.FA000+ FC_Number) = value;
				if(FcodeChangeAtt)
				*(&FC_Handle->Att.FA0.FA000 + FC_Number) |=0x0100;
				FC_Result = FCODE_CHANGE_OK;
			}
			else if((FC_Att & 0x0018) == 0x0008)
			{
				if(CoreStatusFlag.bit.RUN_OK_F)
				FC_Result = FCODE_STOP_CHANGE;
				else
				{
					*(&FC_Handle->Ram.FA0.FA000 + FC_Number) = value;
					*(&FC_Handle->E2prom.FA0.FA000+ FC_Number) = value;
					if(FcodeChangeAtt)
					*(&FC_Handle->Att.FA0.FA000 + FC_Number) |=0x0100;
					FC_Result = FCODE_CHANGE_OK;
				}
			}
			else if((FC_Att & 0x0018) == 0x0010)
			{
				*(&FC_Handle->E2prom.FA0.FA000+ FC_Number) = value;
				if(FcodeChangeAtt)
				*(&FC_Handle->Att.FA0.FA000 + FC_Number) |=0x0100;
				else
				*(&FC_Handle->Ram.FA0.FA000 + FC_Number) = value;
				FC_Result = FCODE_POWERRESET_CHANGE;
			}
			else if((FC_Att & 0x0018) == 0x0018)
			{
				FC_Result = FCODE_ONLY_READ;
			}
			}
		break;
		default:
			FC_Result = FCODE_ERROR;
		break;
	}

	//=20160804===================================================
	if((FcodeChangeAtt)&&(FC_Result <= 2))
	{
		//g_FcodeChangeFlag = 1;
		g_FcodeChangeFlag ++;
	}
	//else
	//{
	//	g_FcodeChangeFlag = 0;
	//}
	//=20160804===================================================

	return FC_Result;
}
/*************************************************
  Function: FuncCodeRead
  Description: change Function Code value
  Input:Fcode
  Return: Function Code Value
  Others: No
*************************************************/
uint32_t FuncCodeRead(uint16_t Fcode)
{
	uint16_t FC_Group = 0;
	uint16_t FC_Number= 0;
	uint32_t FC_Value = 0;

	FC_Group = Fcode/100;
	FC_Number = Fcode%100;

	switch(FC_Group)
	{
	case 00:
		FC_Value = *(&FuncCode_Handle.E2prom.F00.F0000 + FC_Number);
		break;

	case 50:
		FC_Value = *(&FuncCode_Handle.E2prom.F50.F5000 + FC_Number);

		break;
	case 60:
		FC_Value = *(&FuncCode_Handle.E2prom.F60.F6000 + FC_Number);

		break;

	case 100:
		FC_Value = *(&FuncCode_Handle.E2prom.FA0.FA000 + FC_Number);

		break;
	default:
		break;
	}

	return FC_Value;
}
/*************************************************
  Function: FuncCodeAttRead
  Description: change Function Code value
  Input:Fcode
  Return: Function Code Value
  Others: No
*************************************************/
uint32_t FuncCodeAttRead(uint16_t Fcode)
{
	uint16_t FC_Group = 0;
	uint16_t FC_Number= 0;
	uint32_t FC_Att = 0;

	FC_Group = Fcode/100;
	FC_Number = Fcode%100;

	switch(FC_Group)
	{
	case 00:
		FC_Att = *(&FuncCode_Handle.Att.F00.F0000 + FC_Number);
		break;

	case 50:
		FC_Att = *(&FuncCode_Handle.Att.F50.F5000 + FC_Number);

		break;
	case 60:
		FC_Att = *(&FuncCode_Handle.Att.F60.F6000 + FC_Number);

		break;

	case 100:
		FC_Att = *(&FuncCode_Handle.Att.FA0.FA000 + FC_Number);

		break;
	default:
		break;
	}

	return FC_Att;

}
/*************************************************
  Function: FuncCodeAttClear
  Description: Function Code Change Att Clear
  Input:Fcode
  Return: Function Code Value
  Others: No
*************************************************/
void FuncCodeChangeAttClear(uint16_t Fcode)
{
	uint16_t FC_Group = 0;
	uint16_t FC_Number= 0;

	FC_Group = Fcode/100;
	FC_Number = Fcode%100;

	switch(FC_Group)
	{
	case 00:
		*(&FuncCode_Handle.Att.F00.F0000 + FC_Number) &= 0xFFFFFEFF;
		break;

	case 50:
		*(&FuncCode_Handle.Att.F50.F5000 + FC_Number) &= 0xFFFFFEFF;
		break;

	case 60:
		*(&FuncCode_Handle.Att.F60.F6000 + FC_Number) &= 0xFFFFFEFF;
		break;

	case 100:
		*(&FuncCode_Handle.Att.FA0.FA000 + FC_Number) &= 0xFFFFFEFF;
		break;

	default:
		break;
	}
}
/*************************************************
  Function: FuncCodeRead
  Description: change Function Code value
  Input:Fcode
  Return: Function Code Value
  Others: No
*************************************************/
void FuncCodePanelRead(PANELFCODE *p)
{
	uint16_t FC_Group = 0;
	uint16_t FC_Number= 0;

	FC_Group = p->FcodeNum/100;
	FC_Number = p->FcodeNum%100;

	switch(FC_Group)
	{
	case 00:
		p->Value = *(&FuncCode_Handle.E2prom.F00.F0000 + FC_Number);
		p->Max = *(&FuncCode_Handle.Max.F00.F0000 + FC_Number);
		p->Min = *(&FuncCode_Handle.Min.F00.F0000 + FC_Number);
		p->Att = *(&FuncCode_Handle.Att.F00.F0000 + FC_Number);
		break;
	case 50:
		p->Value = *(&FuncCode_Handle.E2prom.F50.F5000 + FC_Number);
		p->Max = *(&FuncCode_Handle.Max.F50.F5000 + FC_Number);
		p->Min = *(&FuncCode_Handle.Min.F50.F5000 + FC_Number);
		p->Att = *(&FuncCode_Handle.Att.F50.F5000 + FC_Number);
		break;
	case 60:
		p->Value = *(&FuncCode_Handle.E2prom.F60.F6000 + FC_Number);
		p->Max = *(&FuncCode_Handle.Max.F60.F6000 + FC_Number);
		p->Min = *(&FuncCode_Handle.Min.F60.F6000 + FC_Number);
		p->Att = *(&FuncCode_Handle.Att.F60.F6000 + FC_Number);
		break;
	case 100:
		p->Value = *(&FuncCode_Handle.E2prom.FA0.FA000 + FC_Number);
		p->Max = *(&FuncCode_Handle.Max.FA0.FA000 + FC_Number);
		p->Min = *(&FuncCode_Handle.Min.FA0.FA000 + FC_Number);
		p->Att = *(&FuncCode_Handle.Att.FA0.FA000 + FC_Number);
		break;
	default:
		break;
	}
}
/*************************************************
  Function: FuncCodeRead
  Description: change Function Code value
  Input:Fcode
  Return: Function Code Value
  Others: No
*************************************************/
uint32_t  FuncCodeOnlyReadValue(PANELFCODE *p)
{
	uint16_t FC_Group = 0;
	uint16_t FC_Number= 0;
	uint32_t FcodeValue = 0;

	FC_Group = p->FcodeNum/100;
	FC_Number = p->FcodeNum%100;

	switch(FC_Group)
	{
	case 00:
		FcodeValue = *(&FuncCode_Handle.Ram.F00.F0000 + FC_Number);
		break;
	case 50:
		FcodeValue = *(&FuncCode_Handle.Ram.F50.F5000 + FC_Number);
		break;
	case 60:
		FcodeValue = *(&FuncCode_Handle.Ram.F60.F6000 + FC_Number);
		break;
	case 100:
		FcodeValue = *(&FuncCode_Handle.Ram.FA0.FA000 + FC_Number);
		break;
	}

	return FcodeValue;
}
/*************************************************
  Function: FuncCode detection
  Description:
  Input:Fcode,value
  Return: 0 or 1
  Others: No
*************************************************/
uint8_t FuncCodeReadDetect(uint16_t Fcode,int32_t value)
{
	uint16_t FC_Group = 0;
	uint16_t FC_Number= 0;
	uint8_t FC_Result= 0;
	int32_t Min,Max;

	FC_Group = Fcode/100;
	FC_Number = Fcode%100;

	switch(FC_Group)
	{
	case 00:
		Min = *(&FuncCode_Handle.Min.F00.F0000 + FC_Number);
		Max = *(&FuncCode_Handle.Max.F00.F0000 + FC_Number);
		if(((int32_t)value>=Min)&&((int32_t)value<=Max))
			FC_Result = 0;
		else
			FC_Result = 1;
		break;
	case 50:
		Min = *(&FuncCode_Handle.Min.F50.F5000 + FC_Number);
		Max = *(&FuncCode_Handle.Max.F50.F5000 + FC_Number);
		if(((int32_t)value>=Min)&&((int32_t)value<=Max))
			FC_Result = 0;
		else
			FC_Result = 1;
		break;
	case 60:
		Min = *(&FuncCode_Handle.Min.F60.F6000 + FC_Number);
		Max = *(&FuncCode_Handle.Max.F60.F6000 + FC_Number);
		if(((int32_t)value>=Min)&&((int32_t)value<=Max))
			FC_Result = 0;
		else
			FC_Result = 1;
		break;
	case 100:
		Min = *(&FuncCode_Handle.Min.FA0.FA000 + FC_Number);
		Max = *(&FuncCode_Handle.Max.FA0.FA000 + FC_Number);
		if(((int32_t)value>=Min)&&((int32_t)value<=Max))
			FC_Result = 0;
		else
			FC_Result = 1;
		break;

	default:
		FC_Result = 1;
		break;
	}

	return FC_Result;
}

/*************************************************
  Function: FuncCodeRead
  Description: change Function Code value
  Input:Fcode
  Return: Function Code Value
  Others: No
*************************************************/
uint32_t  FuncCodeReadRAMValue(uint16_t FcodeNum)
{
	uint16_t FC_Group = 0;
	uint16_t FC_Number= 0;
	uint32_t FcodeValue = 0;

	FC_Group =FcodeNum/100;
	FC_Number = FcodeNum%100;

	switch(FC_Group)
	{
	case 00:
		FcodeValue = *(&FuncCode_Handle.Ram.F00.F0000 + FC_Number);
		break;
	case 50:
		FcodeValue = *(&FuncCode_Handle.Ram.F50.F5000 + FC_Number);
		break;
	case 60:
		FcodeValue = *(&FuncCode_Handle.Ram.F60.F6000 + FC_Number);
		break;
	case 100:
		FcodeValue = *(&FuncCode_Handle.Ram.FA0.FA000 + FC_Number);
		break;
	}

	return FcodeValue;
}

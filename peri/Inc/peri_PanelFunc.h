/*
 *  peri_PanelFunc.h
 *
 *  Created on: 2014-4-1
 *  Author: JH-LvSL
 */

#ifndef PERI_PANELFUNC_H_
#define PERI_PANELFUNC_H_

typedef void (*PANEL_FUNC)(void);

typedef struct
{
   int32_t *add;	/* Data Address*/
   uint32_t	Att;	/*bit0: 0:Dec,1:Hex; bit1: 0:four bytes,1:eight bytes
   	   	   	   	   	   	  	  bit2,bit3:0:no point,1:one point,2:two points,3:three points
   	   	   	   	   	   	  	  bit4: Signed Bit*/
}LEDDISPLAY;

typedef struct
{
	uint32_t Value;
	uint32_t Min;
	uint32_t Max;
	uint32_t Att;
	uint16_t FcodeNum;
}PANELFCODE;

extern void PanelMainMenuDisplay(void);

extern void Panel1stMenuDisplay(void);

extern void Panel2ndMenuDisplay(void);

#endif /* PERI_PANELFUNC_H_ */

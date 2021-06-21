/*
 * peri_PanelLEDButton.h
 *
 *  Created on: 2014-3-27
 *      Author: JH-LvSL
 */

#ifndef PERI_PANELLEDBUUTON_H_
#define PERI_PANELLEDBUUTON_H_
#include "core_StructDef.h"

extern const uint8_t PanelLEDTable[];
extern const uint8_t PanelAlmTable[][5];
extern const uint8_t PanelAlmTable1[][5];
extern const uint8_t PanelDataStsTable[][5];

extern void PanelDataDisplay(int32_t ,uint32_t ,uint8_t,CoreStatusFlg *);
extern void PanelAlmDisplay(uint8_t );
extern void Panel1stMenuDataDisplay(uint16_t ,uint8_t );
extern void Panel2ndMenuDataDisplay(int32_t ,uint8_t ,uint32_t );
extern void Panel2ndMenuDisplaySts(uint8_t );
extern void PanelLEDDisplay(void);


#define OPE_7SEG_LED_NULL  0xFF    /*  */
#define OPE_7SEG_LED_0   0xC0    /*'0'*/
#define OPE_7SEG_LED_1   0xF9    /*'1'*/
#define OPE_7SEG_LED_2   0xA4    /*'2'*/
#define OPE_7SEG_LED_3   0xB0    /*'3'*/
#define OPE_7SEG_LED_4   0x99    /*'4'*/
#define OPE_7SEG_LED_5   0x92    /*'5'*/
#define OPE_7SEG_LED_6   0x82    /*'6'*/
#define OPE_7SEG_LED_7   0xF8    /*'7'*/
#define OPE_7SEG_LED_8   0x80    /*'8'*/
#define OPE_7SEG_LED_9   0x90    /*'9'*/
#define OPE_7SEG_LED_A   0x88    /*'A'*/
#define OPE_7SEG_LED_B   0x83    /*'B'*/
#define OPE_7SEG_LED_C   0xC6    /*'C'*/
#define OPE_7SEG_LED_D   0xA1    /*'D'*/
#define OPE_7SEG_LED_E   0x86    /*'E'*/
#define OPE_7SEG_LED_F   0x8E    /*'F'*/
#define OPE_7SEG_LED_G   0xC2    /*'G'*/
#define OPE_7SEG_LED_H   0x8B    /*'H'*/
#define OPE_7SEG_LED_I   0xFB    /*'I'*/
#define OPE_7SEG_LED_J   0xE1    /*'J'*/
#define OPE_7SEG_LED_K   0x85    /*'K'*///原来是89
#define OPE_7SEG_LED_L   0xC7    /*'L'*/
#define OPE_7SEG_LED_M   0xAA    /*'M'*/ //原来是E0
#define OPE_7SEG_LED_N   0xAB    /*'N'*///原来是C8
#define OPE_7SEG_LED_O   0xA3    /*'O'*/
#define OPE_7SEG_LED_P   0x8C    /*'P'*/
#define OPE_7SEG_LED_Q   0x98    /*'Q'*/
#define OPE_7SEG_LED_R   0xAF    /*'R'*/
#define OPE_7SEG_LED_S   0x92    /*'S'*/
#define OPE_7SEG_LED_T   0xCE    /*'T'*/
#define OPE_7SEG_LED_U   0xC1    /*'U'*/
#define OPE_7SEG_LED_V   0x81    /*'V'*///原来是E3
#define OPE_7SEG_LED_W   0xE2    /*'W'*///C4
#define OPE_7SEG_LED_X   0x95    /*'X'*///E7
#define OPE_7SEG_LED_Y   0x91    /*'Y'*/
#define OPE_7SEG_LED_Z   0xB6    /*'Z'*///B7
#define OPE_7SEG_LED_SP  0xF7	  /*'_'*/
#define OPE_7SEG_LED_ZP  0xBF	  /*'-'*/
#define OPE_7SEG_LED_DP  0x7F	  /*'.'*/

/* LED Enable*/
#define OPE_LED_EN_1	  0x100
#define OPE_LED_EN_2	  0x200
#define OPE_LED_EN_3	  0x400
#define OPE_LED_EN_4	  0x800
#define OPE_LED_EN_5	  0x1000
#define OPE_LED_EN_6	  0x2000

typedef struct
{
	uint8_t mode	:1;
	uint8_t shift	:1;
	uint8_t set		:1;
	uint8_t down	:1;
	uint8_t up		:1;
}ButtonBits;

typedef union
{
	uint8_t all;
	ButtonBits bit;
}BUTTONFLG;

typedef struct
{
	uint32_t mode;
	uint32_t shift;
	uint32_t set;
	uint32_t down;
	uint32_t up;
}BUTTONCOUNTS;

extern BUTTONFLG ButtonFlag;
#endif /* PERI_PANELLEDBUUTON_H_ */

/*
 * peri_TerminalAO.h
 *
 *  Created on: 2014-6-26
 *      Author: JH-LvSL
 */

#ifndef PERI_TERMINALAO_H_
#define PERI_TERMINALAO_H_

typedef void (*AO_FUNC)(void);

typedef struct
{
   int32_t *add;	/* Data Address*/
   int32_t	Att;	/**/
}AOVALUE;

typedef struct
{
	int32_t LLimitVol;
	int32_t LlimitRatio;
	int32_t HLimitVol;
	int32_t HlimitRatio;

	int32_t Gain;
	int64_t Offset;
}AOCorrection;


#endif /* PERI_TERMINALAO_H_ */

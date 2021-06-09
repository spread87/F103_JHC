/*
 * peri_TerminalAI.h
 *
 *  Created on: 2014-6-19
 *      Author: JH-LvSL
 */

#ifndef PERI_TERMINALAI_H_
#define PERI_TERMINALAI_H_

typedef struct
{
	int32_t LLimitVol;
	int32_t LlimitRatio;
	int32_t HLimitVol;
	int32_t HlimitRatio;

	int32_t Gain;
	int64_t Offset;

	int32_t Omit;
}AICorrection;

#endif /* PERI_TERMINALAI_H_ */

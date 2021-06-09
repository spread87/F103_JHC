/*
 * 	peri_AlmWarnMacro.h
 *
 *  Created on: 2014-3-4
 *  Author: JH-LvSL
 */

#ifndef PERI_ALMWARNMACRO_H_
#define PERI_ALMWARNMACRO_H_

/*
 * Alarm Code
 */
#define	AIM_CODE_ALLCOUNT	37

#define	ALM_CODE_NONE		0
#define	ALM_CODE_FO			1
#define	ALM_CODE_DOC			2
#define	ALM_CODE_POV			3
#define	ALM_CODE_COV			4
#define	ALM_CODE_PLV			5
#define	ALM_CODE_CLV			6
#define	ALM_CODE_DOL			7
#define	ALM_CODE_OS			8
#define	ALM_CODE_PD			9
#define	ALM_CODE_SD			10
#define	ALM_CODE_ES			11
#define	ALM_CODE_DOT			12
#define	ALM_CODE_EE			13
#define	ALM_CODE_EC			14
#define	ALM_CODE_BUS			15
#define	ALM_CODE_ECE			16
#define	ALM_CODE_ORL			17
#define	ALM_CODE_PIFE			18
#define	ALM_CODE_DCOF		19
#define	ALM_CODE_AI1E			20
#define	ALM_CODE_AI2E			21
#define	ALM_CODE_AI3E			22
#define	ALM_CODE_EPE			23
#define	ALM_CODE_RLE			24
#define	ALM_CODE_ECE1		25
#define	ALM_CODE_ECE2		26
#define	ALM_CODE_ECE3		27
#define	ALM_CODE_EE1		28
#define	ALM_CODE_EE2		29
#define	ALM_CODE_EGE		30
#define	ALM_CODE_POSE		31
#define	ALM_CODE_PHS		32
#define	ALM_CODE_MOC		33
#define	ALM_CODE_MOL		34
#define	ALM_CODE_MOT		35
#define	ALM_CODE_MC1		36
#define	ALM_CODE_MC2		37
#define	ALM_CODE_MDP		38

#define WARN_CODE_POT       39
#define WARN_CODE_NOT       40
#define WARN_CODE_OL        41
#define WARN_CODE_OT        42
#define WARN_CODE_POOS      43
#define WARN_CODE_AELV      44

#define	ALM_CODE_SEOT		45
#define	ALM_CODE_SEOS		46
#define	ALM_CODE_SERL		47
#define	ALM_CODE_SECE		48
#define	ALM_CODE_AEMO		49
#define	ALM_CODE_AECF		50
#define	ALM_CODE_AEBE		51
#define	ALM_CODE_SETE		52
#define	ALM_CODE_SEDE		53
#define	ALM_CODE_SEDC		54
#define	ALM_CODE_SEWE		55

#define	ALM_CODE_CUR1		56
#define	ALM_CODE_BRP1		57
#define	ALM_CODE_OLF1		58
#define	ALM_CODE_HOME		59

#define	ALM_CODE_ECAM		60
#define WARN_CODE_ECAM      61
#define WARN_CODE_ECAMREV   62

//#define	ALM_CODE_MDP		50

/*
 * Warn Code
 */
#define	WARN_CODE_ALLCOUNT	2

#define	WARN_CODE_NONE		0
//#define	WARN_CODE_OL		1
//#define	WARN_CODE_OT		2

/*
 *Over-current Protection Threshold Calc
 *Rated-current:zoom 3.5
 */
#define OC_PROTECT_THRED(value)  (value << 24)	//

/*
 *Over-Voltage Protection Threshold Calc
 */
#define	OV_THRED			400
#define 	OV_THRED_Q24		8388608////Q24(1.0) = 800V

/*
 *Under_Voltage Protection Threshold Calc
 */
#define	PLV_THRED			190
#define	PLV_THRED_Q24		10248746//IQ24(190/311)

#define PLV_THRED1          210
#define PLV_THRED1_Q24      11328866


#define	CLV_THRED			190
#define	CLV_THRED_Q24		10248746//IQ24(190/311)

#define CLV_THRED1          210
#define CLV_THRED1_Q24      11328866


/*
 * Over-Load Protection Threshold
 */
#define	OL_THRED			1000000

/*
 * Over-Temperature Protection Threshold
 */
#define	DOT_THRED			(105)	//Driver Threshold
#define	MOT_THRED			(105)	//Motor Threshold

/*
 * Analog Input Excess Protection Threshold
 */
#define	AIE_THRED			20132659L  // 12V = Q24(1.2)
void AlmReadNum(void);








#endif /* PERI_ALMWARNMACRO_H_ */

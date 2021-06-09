/*
 *  core_Com.h
 *
 *  Created on: 2014-4-21
 *  Author: JH-LvSL
 */

#ifndef CORE_COM_H_
#define CORE_COM_H_

#include <main.h>

//typedef void (*TORQUELIMIT_FUNC)(void);

#define    	CONST_PI             	3.14159265359   /* Float  PI */

//#define 		CONST_TS_100US 	0.0001					/* Float 100us*/
//#define 		CONST_TS_100US_Q24		16777216 * CONST_TS_100US
//#define		CONST_TS_100US_Q48		16777216 * CONST_TS_100US_Q24
//#define    	CONST_100US_TmTs        (int32_t)((CONST_TS_100US/(CONST_TS_100US + CONST_TS_100US)) *16777216)  /* Ts/Tm+Ts    Q24 */

#define 		CONST_TS_100US 			0.000125			/* Float 125us*/
#define 		CONST_TS_100US_Q24		2097.152			/*16777216 * CONST_TS_100US*/
#define			CONST_TS_100US_Q48		35184372088.832  	/*16777216 * CONST_TS_100US_Q24*/
#define    		CONST_100US_TmTs        8388608 /*(int32_t)((CONST_TS_100US/(CONST_TS_100US + CONST_TS_100US)) *16777216)*/  /* Ts/Tm+Ts    Q24 */

#define 		CONST_TS_1MS			0.001					/* Float 1ms */
//#define 		CONST_TS_1MS_Q24		16777216 *	CONST_TS_1MS				/* Float 1ms */
//#define 		CONST_TS_1MS_Q48		16777216 * 	CONST_TS_1MS_Q24				/* Float 1ms */
#define 		CONST_TS_1MS_Q24		16777.216				/* Float 1ms */
#define 		CONST_TS_1MS_Q48		281474976710.656				/* Float 1ms */

#define 		CONST_TS_10MS			0.01
//#define    		CONST_1MS_TmTs        	(int32_t)((CONST_TS_100US/(CONST_TS_100US + CONST_TS_1MS)) *16777216)  /* Ts/Tm+Ts    Q24 */
#define    		CONST_1MS_TmTs        	1864135 /*(int32_t)((CONST_TS_100US/(CONST_TS_100US + CONST_TS_1MS)) *16777216) */ /* Ts/Tm+Ts    Q24 */
#define   		CONST_10MS_TmTs        	(int32_t)((CONST_TS_100US/(CONST_TS_100US + CONST_TS_10MS)) *16777216)  /* Ts/Tm+Ts    Q24 */

#define    		CONST_1msTm_1msTs     	(int32_t)((CONST_TS_1MS/(CONST_TS_1MS + CONST_TS_1MS)) *16777216)  /* Ts/Tm+Ts    Q24 */
#define   		CONST_10msTm_1msTs     	(int32_t)((CONST_TS_1MS/(CONST_TS_1MS + CONST_TS_10MS)) *16777216)  /* Ts/Tm+Ts    Q24 */

/* For HPF */
#define    CONST_HPF_CTF		1UL            /* Float Cutoff Frequency, Unit Hz */

#define    CONST_HPF_TmTs        (uint32_t)((1UL/(1 + (2 * CONST_PI * CONST_TS_100US * CONST_HPF_CTF))) * 65536 * 65536)  /* Tm/Tm+Ts    Q32 */

/* For LPF */
// for Vdc  LPF
#define    CONST_VDC_Tm			0.002     /* Float Tm Unit S */

#define    CONST_VDC_TmTs        (uint32_t)((CONST_TS_100US/(CONST_TS_100US + CONST_VDC_Tm)) *16777216)  /* Ts/Tm+Ts    Q24 */

#endif /* CORE_COM_H_ */

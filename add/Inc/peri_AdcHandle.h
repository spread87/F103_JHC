/*
 * peri_AdcHandle.h
 *
 * Created on: 2014-2-26
 * Author: JH-LvSL
 */

#ifndef PERI_ADCHANDLE_H_
#define PERI_ADCHANDLE_H_

typedef struct
{
	int32_t Gain;
	int32_t Offset;
}ADCCorrection;






extern uint16_t AdcCalibrateValue(uint16_t );

#endif /* PERI_ADCHANDLE_H_ */

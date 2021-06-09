/*
 * peri_Inflexibility.h
 *
 *  Created on: 2015-9-22
 *      Author: JH-LIUCL
 */
#include <main.h>
#ifndef PERI_INFLEXIBILITY_H_
#define PERI_INFLEXIBILITY_H_

typedef struct
{
	uint32_t  PositionKp;
	uint32_t  VelocityKp;
	uint32_t  VelocityTi;
	uint32_t  VelocityFi;
}INFLEXIBILITY;

typedef struct
{
	uint32_t  PKp1;
	 int64_t  VKp1;
	uint32_t  VKi1;
	uint32_t  VFd1;
	uint32_t  VFt1;

	uint32_t  PKp2;
	 int64_t  VKp2;
	uint32_t  VKi2;
	uint32_t  VFd2;
	uint32_t  VFt2;
}GAINCHANGE;

#endif /* PERI_INFLEXIBILITY_H_ */

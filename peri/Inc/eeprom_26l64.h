#ifndef __EEPROM_H_
#define __EEPROM_H_

#include "stm32f1xx.h"

#define PARA_HEAD_ADDR	0x00
#define PARA_HEAD_SIZE	128

#define PARA_0_ADDR	(0x00 + PARA_HEAD_ADDR + PARA_HEAD_SIZE)
#define PARA_0_SIZE (0x400)
#define PARA_1_ADDR (0x00 + PARA_0_ADDR+ PARA_0_SIZE)
#define PARA_1_SIZZE 0x400

#endif


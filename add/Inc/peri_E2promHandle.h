/**********************************************************
Copyright (c) 2014 JIANGSU JOINHOPE ELECTRIC CO.,LTD
File name: peri_E2promHandle.h
Author: LvSL
Version:
Date: 2014.02.17
Description:   // 用于详细说明此程序文件完成的主要功能，与其他模块
                   // 或函数的接口，输出值、取值范围、含义及参数间的控
                   // 制、顺序、独立或依赖等关系
Others:        // 其它内容的说明
History:       // 修改历史记录列表，每条修改记录应包括修改日期、修改
                  // 者及修改内容简述
    1. Date: 2014.02.17
       Author: LvSL
       Modification:  1st Version
    2. ...
**********************************************************/
#ifndef PERI_E2PROMHANDLE_H_
#define PERI_E2PROMHANDLE_H_

extern void EepromInit(void);

extern void EepromWriteFcode(uint16_t ,uint32_t );

extern uint32_t EepromReadFcode(uint16_t );

extern void FcodeValueToEEPROM(void);

#endif /* PERI_E2PROMHANDLE_H_ */

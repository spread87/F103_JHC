/**********************************************************
Copyright (c) 2014 JIANGSU JOINHOPE ELECTRIC CO.,LTD
File name: peri_FuncCodeHandle.h
Author: LvSL
Version:
Date: 2014.02.13
Description:   // 用于详细说明此程序文件完成的主要功能，与其他模块
                   // 或函数的接口，输出值、取值范围、含义及参数间的控
                   // 制、顺序、独立或依赖等关系
Others:        // 其它内容的说明
History:       // 修改历史记录列表，每条修改记录应包括修改日期、修改
                  // 者及修改内容简述
    1. Date: 2014.02.13
       Author: LvSL
       Modification:  1st Version
    2. ...
**********************************************************/
#include <peri_FuncCodeStruct.h>
#include <peri_PanelFunc.h>
#define FCODE_CHANGE_OK			0U
#define FCODE_STOP_CHANGE		1U
#define FCODE_POWERRESET_CHANGE	2U
#define FCODE_ONLY_READ			3U
#define FCODE_ERROR				4U
#define FCODE_VALUE_ERROR		5U

#define FCODE_F00_Num				100U
#define FCODE_F10_Num				50U
#define FCODE_F20_Num				30U
#define FCODE_F30_Num				40U
#define FCODE_F40_Num				80U
#define FCODE_F50_Num				60U
#define FCODE_F60_Num				40U
#define FCODE_F90_Num				30U
#define FCODE_FA0_Num				100U

#define FCODE_TOTAL_Num			( FCODE_F00_Num + FCODE_F10_Num + FCODE_F20_Num + FCODE_F30_Num + FCODE_F40_Num +\
                              FCODE_F50_Num + FCODE_F60_Num + FCODE_F90_Num + FCODE_FA0_Num)
#define FCODE_F00ToF10_Num			( FCODE_F00_Num + FCODE_F10_Num )
#define FCODE_F00ToF20_Num			( FCODE_F00ToF10_Num + FCODE_F20_Num )
#define FCODE_F00ToF30_Num			( FCODE_F00ToF20_Num + FCODE_F30_Num )
#define FCODE_F00ToF40_Num			( FCODE_F00ToF30_Num + FCODE_F40_Num )
#define FCODE_F00ToF50_Num			( FCODE_F00ToF40_Num + FCODE_F50_Num )
#define FCODE_F00ToF60_Num			( FCODE_F00ToF50_Num + FCODE_F60_Num )
#define FCODE_F00ToF90_Num			( FCODE_F00ToF60_Num + FCODE_F90_Num )

extern void FuncCodeInit(void);
extern uint16_t FuncCodeChange(uint16_t ,uint32_t ,FuncCode *,uint8_t);
extern uint32_t FuncCodeRead(uint16_t);
extern uint32_t FuncCodeReadRAMValue(uint16_t );
extern void FuncCodePanelRead(PANELFCODE *);
extern  uint32_t  FuncCodeOnlyReadValue(PANELFCODE *);
extern uint8_t FuncCodeReadDetect(uint16_t ,int32_t );
extern uint32_t FuncCodeAttRead(uint16_t );
extern void FuncCodeChangeAttClear(uint16_t );

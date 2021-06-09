/**********************************************************
Copyright (c) 2014 JIANGSU JOINHOPE ELECTRIC CO.,LTD
File name: peri_FuncCodeHandle.h
Author: LvSL
Version:
Date: 2014.02.13
Description:   // ������ϸ˵���˳����ļ���ɵ���Ҫ���ܣ�������ģ��
                   // �����Ľӿڣ����ֵ��ȡֵ��Χ�����弰������Ŀ�
                   // �ơ�˳�򡢶����������ȹ�ϵ
Others:        // �������ݵ�˵��
History:       // �޸���ʷ��¼�б�ÿ���޸ļ�¼Ӧ�����޸����ڡ��޸�
                  // �߼��޸����ݼ���
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
#define FCODE_F50_Num				60U
#define FCODE_F60_Num				40U
#define FCODE_FA0_Num				100U

#define FCODE_TOTAL_Num			( FCODE_F00_Num + FCODE_F60_Num + FCODE_FA0_Num)
#define FCODE_F00ToF10_Num			( FCODE_F00_Num + FCODE_F10_Num )
#define FCODE_F00ToF50_Num			( FCODE_F00ToF10_Num + FCODE_F50_Num )
#define FCODE_F00ToF60_Num			( FCODE_F00ToF50_Num + FCODE_F60_Num )

extern void FuncCodeInit(void);
extern uint16_t FuncCodeChange(uint16_t ,uint32_t ,FuncCode *,uint8_t);
extern uint32_t FuncCodeRead(uint16_t);
extern uint32_t FuncCodeReadRAMValue(uint16_t );
extern void FuncCodePanelRead(PANELFCODE *);
extern  uint32_t  FuncCodeOnlyReadValue(PANELFCODE *);
extern uint8_t FuncCodeReadDetect(uint16_t ,int32_t );
extern uint32_t FuncCodeAttRead(uint16_t );
extern void FuncCodeChangeAttClear(uint16_t );

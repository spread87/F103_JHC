/**********************************************************
Copyright (c) 2014 JIANGSU JOINHOPE ELECTRIC CO.,LTD
File name: peri_E2promHandle.h
Author: LvSL
Version:
Date: 2014.02.17
Description:   // ������ϸ˵���˳����ļ���ɵ���Ҫ���ܣ�������ģ��
                   // �����Ľӿڣ����ֵ��ȡֵ��Χ�����弰������Ŀ�
                   // �ơ�˳�򡢶����������ȹ�ϵ
Others:        // �������ݵ�˵��
History:       // �޸���ʷ��¼�б�ÿ���޸ļ�¼Ӧ�����޸����ڡ��޸�
                  // �߼��޸����ݼ���
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

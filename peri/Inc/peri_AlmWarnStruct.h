/*
 * peri_AlmWarnStruct.h
 *
 *  Created on: 2014-3-4
 *  Author: JH-LvSL
 */

#ifndef PERI_ALMWARNSTRUCT_H_
#define PERI_ALMWARNSTRUCT_H_

/**
 * Driver Alm
 */
typedef struct AlarmDrvBit
{
	uint32_t FO 	: 1;	//[0]:IPM error protection
	uint32_t DOC 	: 1;	//[1]:driver over-current protection
	uint32_t POV 	: 1;	//[2]:power over-voltage protection
	uint32_t COV 	: 1;	//[3]:ctrl board over-voltage protection
	uint32_t PLV 	: 1;	//[4]:power under-voltage protection
	uint32_t CLV 	: 1;	//[5]:ctrl board under-voltage protection
	uint32_t DOL 	: 1;	//[6]:driver over-load protection
	uint32_t OS		: 1;	//[7]:over-speed protection
	uint32_t PD		: 1;	//[8]:position deviation excess protection
	uint32_t SD		: 1;	//[9]:speed deviation excess protection
	uint32_t ES		: 1;	//[10]:Emergency Stop
	uint32_t DOT	: 1;	//[11]:driver IGBT over-temperature protection
	uint32_t EE		: 1;	//[12]:EEPROM error
	uint32_t EC		: 1; 	//[13]:(Reserved)232/485 communication error
	uint32_t BUS	: 1;	//[14]:bus excess protection
	uint32_t ECE	: 1;	//[15]:encoder communication error��send by api
	uint32_t ORL	: 1;	//[16]:(Reserved)over-regeneration load protection
	uint32_t PIFE	: 1;	//[17]:(Reserved)command pulse input frequency error protection
	uint32_t DCOF	: 1;	//[18]:Deviation counter overflow protection
	uint32_t AI1E	: 1;	//[19]:analog input1 excess protection
	uint32_t AI2E	: 1;	//[20]:analog input2 excess protection
	uint32_t AI3E	: 1;	//[21]:analog input3 excess protection
	uint32_t EPE	: 1;	//[22]:(Reserved)encoder initial magnetic(UVW) error protection
	uint32_t RLE	: 1;	//[23]:(Reserved)relay error
	uint32_t ECE1   : 1;    //[24]:|encoder-10000|>100
	uint32_t ECE2   : 1;    //[25]; 10000 OVERFLOW
	uint32_t ECE3   : 1;    //[26]; encoder_disconnect
	uint32_t EE1    : 1;    //[27]; e2prom init
	uint32_t EE2    : 1;    //[28]; ACK
	uint32_t EGE    : 1;    //[29];���ӳ��ֱ��趨����
	uint32_t POSE   : 1;    //[30];��������趨����
	uint32_t BRKFO	: 1;	//[31]:Break FO
}AlarmDrvBit;

/**
 * motor alm
 */
typedef struct AlarmMotBit
{
	uint32_t MOC	: 1;	//[0]: motor over-current protection
	uint32_t MOL	: 1;	//[1]: motor over-load protection
	uint32_t MOT	: 1;	//[2]: motor over-temperature protection
	uint32_t MC1	: 1;	//[3]: motor-line( U V W ) connection error��rotorlocked
	uint32_t MC2    : 1;    //[4]: motor-line( U V W ) connection error1,negetive_runaway
	uint32_t MDP    : 1;
	uint32_t SEOT   : 1;    //[6]:���б���������
	uint32_t SEOS   : 1;    //[7]:���б������ϵ�����
	uint32_t SERL   : 1;    //[8]:���б���������ȱʧ
	uint32_t SECE   : 1;    //[9]:���б�������������
	uint32_t AEMO   : 1;    //[10]:����ֵ��������Ȧ�������
	uint32_t AECF   : 1;    //[11]:����ֵ��������Ȧ��������
	uint32_t AEBE   : 1;    //[12]:����ֵ��������ش���
	uint32_t SETE   : 1;    //[13]:���ʶ�����
	uint32_t SEDE   : 1;    //[14]:���б�����ͨ���쳣
	uint32_t SEDC   : 1;    //[15]:���б���������
	uint32_t SEWE   : 1;    //[16]:���б�����д�����
	uint32_t PHS   : 1;    //[17]:�Ƕ�ѧϰ������ʱ�������
	uint32_t CUr   : 1;    //[18]:���������쳣
	uint32_t BRP   : 1;    //[19]:�ƶ����蹦�ʲ���
	uint32_t OLF   : 1;    //[20]:���ȱ���쳣
	uint32_t HOME  : 1;    //[21]:��ԭ��ʱ����  20190126
	uint32_t RES	: 9;	//[22~31]:reserved

}AlarmMotBit;

typedef struct AlarmStatusBit
{
	AlarmDrvBit Driver;		//servo driver
	AlarmMotBit Motor;		//servo motor
}AlarmStatusBit;

typedef union AlarmStatusFlg
{
	uint64_t all;
	AlarmStatusBit bit;
}AlarmStatusFlg;

/*
 * Warn define
 */
typedef struct WarnStatusBit
{
	uint16_t POT    : 1;    //[0]; ��ת������λ
	uint16_t NOT    : 1;    //[1]; ��ת������λ
	uint16_t OL		: 1;	//over-load warn
	uint16_t OT		: 1;	//over-temperature warn
	uint16_t POOS   : 1;    //[4]; �����������
	uint16_t AELV   : 1;    //[5]:����ֵ���������Ƿѹ
	uint16_t RES	: 11;	//reserved

}WarnStatusBit;

typedef union WarnStatusFlg
{
	uint16_t all;
	WarnStatusBit bit;
}WarnStatusFlg;

typedef struct AlarmWarnSelectBit
{
	uint16_t DOL    : 1;    //[0]; Drive Over Load
	uint16_t MOL    : 1;    //[1]:Motor Over Load
	uint16_t OS		: 1;	//
	uint16_t PD		: 1;	//
	uint16_t SD		: 1;	//
	uint16_t DOT	: 1;	//
	uint16_t BUS		: 1;	//
	uint16_t AI1		: 1;	//
	uint16_t AI2		: 1;	//
	uint16_t AI3		: 1;	//
	uint16_t EGE		: 1;	//
	uint16_t POSE		: 1;	//
	uint16_t BRP		: 1;	//
	uint16_t RES1		: 1;	//
	uint16_t RES2		: 1;	//
	uint16_t PLV   : 1;    //[4]:Low voltage
	uint16_t POV   : 1;    //[5]:Over voltage
	uint16_t RES	: 11;	//reserved
}AlarmWarnSelectBit;

typedef union AlarmWarnSelect
{
	uint16_t all;
	AlarmWarnSelectBit bit;
}AlarmWarnSelect;



#endif /* PERI_ALMWARNSTRUCT_H_ */

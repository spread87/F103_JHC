/*
 * peri_Inertia.h
 *
 *  Created on: 2015-12-8
 *      Author: JH-LIUCL
 */

#ifndef PERI_INERTIA_H_
#define PERI_INERTIA_H_

typedef struct RUNFLAGBIT
{

	uint8_t run_old  : 1;
	uint8_t run_now  : 1;
	uint8_t wait_old : 1;
	uint8_t wait_now : 1;
	uint8_t acc_start: 1;  //����ʱ������ʼ
	uint8_t dec_start: 1;  //����ʱ������ʼ
	uint8_t acc_finish:1;  //���ٲ�������
	uint8_t dec_finish:1;  //���ٲ�������

}RUNFLAGBIT;

typedef union RUNFLAG
{
	RUNFLAGBIT bit;
	uint8_t all;
}RUNFLAG;


typedef struct INERTIA_STATUS
{
	RUNFLAG bitflag;//����״̬  0ֹͣ  1����
	uint8_t wait_flag;//��϶�ȴ�״̬ 0 �ǵȴ� 1�ȴ�
	uint8_t g_Time_add_flag;//20180930
	uint8_t mode;//���з�ʽ  0������ 1����  2һ������
	uint8_t direct;//���з��� 0��������  1��������
	uint64_t remain_pulses;//��ǰ���е�������
    uint64_t pos_pulses;//ʣ��������
    uint64_t neg_pulses;
	uint64_t steady_velocity;//���������ٶ�  20170615 32->64
	uint64_t steady_velocity_max;//���������ٶ�  20180929
	uint64_t acc;//�Ӽ���б��
	uint64_t sample_value;//�������ֵ��ʼ����
	uint64_t sample_value1;
	uint32_t sample_times;//����������
	uint8_t  stage;//�ٶȴ�����һ�׶Σ����٣����ٻ���٣�ֹͣʱΪ0
	uint32_t vel_to_pulses;
	uint32_t pulses_to_vel;
	int64_t  pulses_start;
	int64_t  pulses_stop;
	int32_t  acc_w1;
	int32_t  acc_w2;
	int32_t  dec_w1;
	int32_t  dec_w2;
	uint32_t  study_times;
}INERTIA_STATUS;

#define  INERTIA_DEFAULTS {0,\
	    0,\
	    0,\
	    0,\
	    0,\
	    0,\
	    0,\
	    0,\
	    0,\
	    0,\
	    0,\
		0,\
		0,\
		0,\
		0,\
		0,\
		500,\
		10,\
		30,\
		30,\
		128,\
		0,\
        1,\
        1,\
        0,\
        0,\
        0,\
        0,\
	    0}


#endif /* PERI_INERTIA_H_ */

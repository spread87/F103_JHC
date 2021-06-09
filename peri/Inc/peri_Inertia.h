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
	uint8_t acc_start: 1;  //加速时采样开始
	uint8_t dec_start: 1;  //减速时采样开始
	uint8_t acc_finish:1;  //加速采样结束
	uint8_t dec_finish:1;  //减速采样结束

}RUNFLAGBIT;

typedef union RUNFLAG
{
	RUNFLAGBIT bit;
	uint8_t all;
}RUNFLAG;


typedef struct INERTIA_STATUS
{
	RUNFLAG bitflag;//运行状态  0停止  1运行
	uint8_t wait_flag;//间隙等待状态 0 非等待 1等待
	uint8_t g_Time_add_flag;//20180930
	uint8_t mode;//运行方式  0不运行 1往复  2一个方向
	uint8_t direct;//运行方向 0：正方向  1：反方向
	uint64_t remain_pulses;//当前运行的脉冲数
    uint64_t pos_pulses;//剩余脉冲数
    uint64_t neg_pulses;
	uint64_t steady_velocity;//匀速运行速度  20170615 32->64
	uint64_t steady_velocity_max;//匀速运行速度  20180929
	uint64_t acc;//加减速斜率
	uint64_t sample_value;//大于这个值开始采样
	uint64_t sample_value1;
	uint32_t sample_times;//采样周期数
	uint8_t  stage;//速度处于哪一阶段，加速，匀速或减速，停止时为0
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

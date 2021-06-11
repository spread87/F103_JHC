/*
 * peri_Wave.c
 *
 *  Created on: Jun 14, 2021
 *      Author: luke
 */
#include "stdint.h"

int16_t WaveA[10] = { 0 };
int16_t WaveB[10] = { 0 };
int16_t WaveC[10] = { 0 };
int16_t WaveD[10] = { 0 };
int16_t Wave13[5] = { 0 };

void Wave_dis(void) //1ms�������  0x22����ָ����
{/*
  static int16_t i = 0;
  static int16_t Cnt_Wave_1ms = 0;

  if ((CanTransmitData.tempCanMonEn > 0)
      && (FuncCode_Handle.Ram.F60.F6025 != 0)) //��λ����ع���
  {
    if (Cnt_Wave_1ms < CanTransmitData.tempCanMonEn - 1) // ����ʱ����
    {
      Cnt_Wave_1ms++;
    }
    else
    {
      Cnt_Wave_1ms = 0;
      //if(CoreStatusFlag.bit.WAVE_DATA_F==NG) //״̬��3��bit0�������ȡ��ɣ��������¿�ʼ��������
      if ((CoreStatusFlag.bit.WAVE_DATA_F == NG)
          && (CoreStatusFlag.bit.WAVE_DATA_S)) //״̬��3��bit0�������ȡ��ɣ�������Ҫ�����������ݣ��Ϳ������¿�ʼ��������
      {
        WaveA[i] = (*CanValueTable[CanTransmitData.channel1].add >> 10);
        WaveB[i] = (*CanValueTable[CanTransmitData.channel2].add >> 10);
        WaveC[i] = (*CanValueTable[CanTransmitData.channel3].add >> 10);
        WaveD[i] = (*CanValueTable[CanTransmitData.channel4].add >> 10);
        //WaveD[i] = (Timer1msCount*16777)>>10;//0.001
        //WaveE[i] = (*CanValueTable[57].add >>10);
        //WaveE[i] = 16777*i;//0.001
        //WaveE[i] = i;//0.001

        if (i < 999)
        {
          i++;
        }
        else
        {
          //commu_func1.State_Word3=commu_func1.State_Word3|0x0001;//״̬��2 ��bit8 �����λ�����ɣ����Զ�ȡ
          CoreStatusFlag.bit.WAVE_DATA_F = OK;
          CoreStatusFlag.bit.WAVE_DATA_S = NG;
          i = 0;
        }
      }
    }
  }
*/}

void Wave13_dis(void)                 //Modbus13ָ������������  0x13����ָ����
{/*
  Wave13[0] = (*CanValueTable[CanTransmitData.channel1].add >> 10);
  Wave13[1] = (*CanValueTable[CanTransmitData.channel2].add >> 10);
  Wave13[2] = (*CanValueTable[CanTransmitData.channel3].add >> 10);
  Wave13[3] = (*CanValueTable[CanTransmitData.channel4].add >> 10);

  Wave13[4] = (*CanValueTable[57].add >> 10);
*/}

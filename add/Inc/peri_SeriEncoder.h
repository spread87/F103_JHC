/*
 * peri_SeriEncoder.h
 *
 *  Created on: 2017-3-10
 *      Author: Library
 */

#ifndef PERI_SERIENCODER_H_
#define PERI_SERIENCODER_H_

extern void SeriEncHandle();
//extern void SeriEncReadHandle();
//extern void SeriEncWriteHandle();
extern  void AlmCheckSEnc();
extern void InitSEncPara();
extern void SEncReset();
extern uint8_t  SeriEncWrtMtrCode();
extern uint8_t SeriEncWrtMtrCodeCmd();
extern uint8_t SeriEncClrSglPos();
extern uint8_t SeriEncClrSglPosCmd();
#endif /* PERI_SERIENCODER_H_ */

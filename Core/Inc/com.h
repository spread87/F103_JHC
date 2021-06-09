#ifndef __COM_H__
#define __COM_H__

int rs485_raw_send(unsigned char * data, int len);
void RS485_init(void);
#endif

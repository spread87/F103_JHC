#include "gpio.h"
#include "tim.h"
#include "stm32f1xx.h"
#include "usart.h"

#define REDE_Pin GPIO_PIN_8
#define REDE_GPIO_Port GPIOA

#define RS485_REDE_RE	HAL_GPIO_WritePin(REDE_GPIO_Port, REDE_Pin, GPIO_PIN_RESET)
#define RS485_REDE_DE	HAL_GPIO_WritePin(REDE_GPIO_Port, REDE_Pin, GPIO_PIN_SET)

void rx485_raw_read(void *arg, char data);

void RS485_init(void)
{
	RS485_REDE_RE;
	MX_USART1_UART_Init();
	MX_TIM2_Init();
	set_usart_recvice_callback(rx485_raw_read, NULL);
}

int rs485_raw_send(unsigned char * data, int len)
{
	int res;

	RS485_REDE_DE;
	res = usart1_raw_send(data, len);
	RS485_REDE_RE;

	return res;
}

static unsigned char local_rx485_rxbuf[32];
static int local_rx_cnt = 0;

void rx485_raw_read(void *arg, char data)
{
	(void)arg;
	//local_rx485_rxbuf[local_rx_cnt++] = data;
	rs485_raw_send(&data, 1);
}


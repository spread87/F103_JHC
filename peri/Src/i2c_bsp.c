#include "i2c_bsp.h"

/* IIC delay 2ms = 500Kbps */
#define IIC_DELAY delay_us(10)

#define EEPROM_SCL_Pin GPIO_PIN_10
#define EEPROM_SCL_GPIO_Port GPIOB
#define EEPROM_SDA_Pin GPIO_PIN_11
#define EEPROM_SDA_GPIO_Port GPIOB

#define EEPROM_DEVICE_ID  0xA0

#define I2C_SCL_1()  i2c_scl_ctrl(1)
#define I2C_SCL_0()  i2c_scl_ctrl(0)
#define I2C_SDA_1()  i2c_sda_ctrl(1)
#define I2C_SDA_0()  i2c_sda_ctrl(0)
#define I2C_SDA_READ() i2c_sda_read()

static void i2c_scl_ctrl(uint32_t val);
static void i2c_sda_ctrl(uint32_t val);

static void delay_us(uint32_t xus)
{
  volatile uint32_t i;

  while (xus--)
  {
    for (i = 0; i < 100; i++);
  }
}

void IIC_init( void )
{
  I2C_SCL_1();
  I2C_SDA_1();
}

//-------------------------------
static void i2c_scl_ctrl(uint32_t val)
{
  HAL_GPIO_WritePin(EEPROM_SCL_GPIO_Port, EEPROM_SCL_Pin, val);
}

static void i2c_sda_ctrl(uint32_t val)
{
  HAL_GPIO_WritePin(EEPROM_SDA_GPIO_Port, EEPROM_SDA_Pin, val);
}

static int i2c_sda_read(void)
{
  return HAL_GPIO_ReadPin(EEPROM_SDA_GPIO_Port, EEPROM_SDA_Pin);
}

//   IIC  STOP timing ------
static void i2c_Stop(void)
{
  I2C_SDA_0();
  delay_us(2);
  I2C_SCL_1();
  delay_us(6);
  I2C_SDA_1();
}

//   IIC  Start timing ------
static void i2c_Start(void)
{
  I2C_SDA_1();
  I2C_SCL_1();
  delay_us(7);
  I2C_SDA_0();
  delay_us(5);
  I2C_SCL_0();
}

// IIC  send one byte timing----------------
static void i2c_SendByte(unsigned char _ucByte)
{
  unsigned char i;

  for (i = 0; i < 8; i++)
  {
    delay_us(3);
    if (_ucByte & 0x80)
    {
      I2C_SDA_1();
    }
    else
    {
      I2C_SDA_0();
    }
    delay_us(2);
    I2C_SCL_1();
    delay_us(5);
    I2C_SCL_0();
    delay_us(2);
//		if (i == 7)
//		{
//			I2C_SDA_1(); // �ͷ�����
//		}
    _ucByte <<= 1; /* ����һ��bit */
  }
  IIC_DELAY;
  I2C_SDA_1();
}

// IIC  Read one Byte timing----------------
static unsigned char i2c_ReadByte(void)
{
  unsigned char i;
  unsigned char value;

  value = 0;
  for (i = 0; i < 8; i++)
  {
    value <<= 1;
    delay_us(4);
    I2C_SCL_1();
    delay_us(2);
    if (I2C_SDA_READ())
    {
      value++;
    }
    delay_us(4);
    I2C_SCL_0();
    delay_us(4);
  }

  return value;
}

// IIC  Wait Ack one Byte timing--------------
static unsigned char i2c_WaitAck(void)
{
  unsigned char re;

  I2C_SDA_1(); /* CPU�ͷ�SDA���� */
  delay_us(3);
  I2C_SCL_1(); /* CPU����SCL = 1, ��ʱ�����᷵��ACKӦ�� */
  delay_us(5);
  if (I2C_SDA_READ()) /* CPU��ȡSDA����״̬ */
  {
    re = 1;
  }
  else
  {
    re = 0;
  }
  I2C_SCL_0();
  delay_us(2);
  return re;
}

// IIC  Ack  timing--------------
static void i2c_Ack(void)
{
  I2C_SDA_0();
  delay_us(3);
  I2C_SCL_1();
  delay_us(6);
  I2C_SCL_0();
  delay_us(2);
  I2C_SDA_1();
  delay_us(2);
}

// IIC  NO Ack  timing--------------
static void i2c_NAck(void)
{
  I2C_SDA_1();
  delay_us(3);
  I2C_SCL_1();
  delay_us(6);
  I2C_SCL_0();
  delay_us(2);
}

//  IIC  write buffer  --------------
uint8_t IIC_Write_Buffer(uint16_t len, uint8_t *data)
{
  unsigned char device_addr;

  device_addr = EEPROM_DEVICE_ID; // save device addr

  i2c_Start();
  i2c_SendByte(device_addr | I2C_WR);   //send  address adn write command  1
  if (i2c_WaitAck() != 0)          // check ack
  {
    i2c_Stop();
    return 0;
  }

  while (len--)
  {
    i2c_SendByte(*data++);      //send high byte  3
    if (i2c_WaitAck() != 0)    // check ack
    {
      i2c_Stop();
      return 0;
    }
  };

  i2c_Stop();                   // send  succeed
  delay_us(5000);
  return 1;
}

//  IIC  Read Data buffer  --------------
uint16_t IIC_Read_Buffer(uint16_t tx_len, uint8_t *txbuf, uint16_t rx_len, uint8_t *rxbuf)
{
  unsigned char device_addr;

  device_addr = EEPROM_DEVICE_ID;                   // save device addr

  i2c_Start();                   // start communication
  i2c_SendByte(device_addr | I2C_WR);   //send  address adn write command
  if (i2c_WaitAck() != 0)          // check ack
  {
    i2c_Stop();
    return 0;
  }
  while (tx_len--)
  {
    i2c_SendByte(*txbuf);            //send register
    if (i2c_WaitAck() != 0)          // check ack
    {
      i2c_Stop();
      return 0;
    }
    txbuf++;
  }

  i2c_Start();                   // repetition Start timing
  i2c_SendByte(device_addr | I2C_RD);   //send  address adn read command
  if (i2c_WaitAck() != 0)          // check ack
  {
    i2c_Stop();
    return 0;
  }

  while (rx_len)
  {
    *rxbuf = i2c_ReadByte();     // Read data
    if (rx_len == 1)               // if receive one byte  send no ack
      i2c_NAck();
    else
      i2c_Ack();

    rxbuf++;
    rx_len--;
  }

  i2c_Stop();
  return 1;
}

int Isok_i2c_Device(void)
{
  uint8_t ucAck;
  uint8_t device_addr;

  device_addr = EEPROM_DEVICE_ID;
  i2c_Start();                   // repetition Start timing
  i2c_SendByte(device_addr | I2C_WR);   //send  address adn write command

  ucAck = i2c_WaitAck();         // gain ack signal
  i2c_Stop();

  return !ucAck;
}
#if 0
unsigned char eeprom_page_write(unsigned char addr,unsigned short reg, unsigned int len, unsigned char *data)
{
	unsigned int remain_num = 0;
	unsigned int op_len = 0;

	if(len == 0)
		return 1;

	while(1){
		remain_num = 16 - (reg & 0x0F); // calc ability of this address 

		if(remain_num >= len) // if opreation length more than or equal writing length . op_len is len.
			op_len = len;
		else
			op_len = remain_num;

		IIC_Write_Buffer(addr,reg,op_len,data);

		len = len - op_len;
		reg = reg + op_len;
		data = data + op_len;

		if(len == 0) break;
	}
	return 0;
	/* frist write remain data */
}

unsigned char eeprom_page_read(unsigned char addr,unsigned short reg, unsigned int len, unsigned char *data)
{
	unsigned int remain_num = 0;
	unsigned int op_len = 0;

	if(len == 0)
		return 1;

	while(1){
		remain_num = 16 - (reg & 0x0F); // calc ability of this address 

		if(remain_num >= len) // if opreation length more than or equal writing length . op_len is len.
			op_len = len;
		else
			op_len = remain_num;


		IIC_Read_Buffer(addr,reg,op_len,data);

		len = len - op_len;
		reg = reg + op_len;
		data = data + op_len;

		if(len == 0) break;
	}
	return 0;
}
#endif

#if 0
unsigned char IIC_Write_EEPROM(unsigned char addr, u16 reg, unsigned char data)
{
	addr = (addr & 0xf1) | (( reg & 0x700) >> 7);
	i2c_Start();
	i2c_SendByte(addr | I2C_WR);   //send  address adn write command  1
	if(i2c_WaitAck() != 0)          // check ack
	{
		i2c_Stop();
		return 0;
	}
#if 0
	i2c_SendByte((unsigned char )((reg & 0xff00) >> 8));            //send register  2

	if(i2c_WaitAck() != 0)        // check ack
	{
		i2c_Stop();
		return 0;
	}
#endif
	i2c_SendByte((unsigned char )(reg & 0xff) );      //send high byte  3
	if(i2c_WaitAck() != 0)    // check ack
	{
		i2c_Stop();
		return 0;
	}
	i2c_SendByte(data);      //send low byte  4
	if(i2c_WaitAck() != 0)    // check ack
	{
		i2c_Stop();
		return 0;
	}
	i2c_Stop();                   // send  succeed
	return 1;
}

void IIC_Read_EEPROM(unsigned char addr, u16 reg, unsigned char* buf)
{
	addr = (addr & 0xf1) | (( reg & 0x700) >> 7);
	IIC_Read_Buffer(addr,(unsigned char)(reg & 0xff),1, buf);

}
#endif

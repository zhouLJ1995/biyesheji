#include "moni_UART.h"

#define COM_TX_LOW()    P2OUT &= 0x00
#define COM_TX_HIGH()   P2OUT |= 0x08

#define _300BuadRate	     3150
#define _600BuadRate	     1700
#define _1200BuadRate	 825
#define _9600BuadRate	 105
#define _115200BuadRate	 8

#define delayTime _115200BuadRate //定义发送延时

void VirtualCOM_GPIOConfig(void)
{
	//TX
	P2DIR |= 0x08;
	P2OUT |= 0x08;
//	P2OUT ^= 0x08;                            // Toggle P1.6
}

void VirtualCOM_ByteSend(uint8_t val)//发送一个uint8_t
{
	uint8_t i = 0;
	COM_TX_LOW();			   //起始位
	Delay_us(delayTime);
	for(i = 0; i < 8; i++)	           //8位数据位
	{
		if(val & 0x01)
			COM_TX_HIGH();//TXD输出 1
		else
			COM_TX_LOW();
		Delay_us(delayTime);//相应波特率延时
		val >>= 1;
	}
	COM_TX_HIGH();		   //停止位
	Delay_us(delayTime);
}

void VirtualCOM_StringSend(uint8_t *str)//发送字符串
{
	while(*str)
	{
		VirtualCOM_ByteSend(*str++);
	}
}

int putchar(int c)//重定义printf
{
	VirtualCOM_ByteSend((uint8_t)c);
	return c;
}

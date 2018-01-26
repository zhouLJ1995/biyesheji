#include "moni_UART.h"

#define COM_TX_LOW()    P2OUT &= 0x00
#define COM_TX_HIGH()   P2OUT |= 0x08

#define _300BuadRate	     3150
#define _600BuadRate	     1700
#define _1200BuadRate	 825
#define _9600BuadRate	 105
#define _115200BuadRate	 8

#define delayTime _115200BuadRate //���巢����ʱ

void VirtualCOM_GPIOConfig(void)
{
	//TX
	P2DIR |= 0x08;
	P2OUT |= 0x08;
//	P2OUT ^= 0x08;                            // Toggle P1.6
}

void VirtualCOM_ByteSend(uint8_t val)//����һ��uint8_t
{
	uint8_t i = 0;
	COM_TX_LOW();			   //��ʼλ
	Delay_us(delayTime);
	for(i = 0; i < 8; i++)	           //8λ����λ
	{
		if(val & 0x01)
			COM_TX_HIGH();//TXD��� 1
		else
			COM_TX_LOW();
		Delay_us(delayTime);//��Ӧ��������ʱ
		val >>= 1;
	}
	COM_TX_HIGH();		   //ֹͣλ
	Delay_us(delayTime);
}

void VirtualCOM_StringSend(uint8_t *str)//�����ַ���
{
	while(*str)
	{
		VirtualCOM_ByteSend(*str++);
	}
}

int putchar(int c)//�ض���printf
{
	VirtualCOM_ByteSend((uint8_t)c);
	return c;
}

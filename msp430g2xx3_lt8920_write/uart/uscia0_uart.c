#include <msp430g2553.h>

#include "uscia0_uart.h"

/*****************************************************************
*���ƣ�UART_Set()
*���ܣ�UART��������
*��ڲ�����
				baud: ������1200 2400 4800 9600(Ĭ��) 19200 38400 56000 115200
				data: ����λ��8��8λ��7��7λ��Ĭ��8λ
				jiouwei; ��żλ��0:�ޣ�Ĭ�ϣ�,1:��У��,2:żУ��
				stop: ֹͣλ��2:2λֹͣλ��������ΪĬ�ϵ�1λ
				R_T: �ж�ģʽ��1���գ�Ĭ�ϣ���2������3���շ�
*���ڲ�������
*ʱ��˵����ACLK = 32.768KHz, SMCLK = CALxxx_16MHZ = 16MHz
				  BCSCTL1 |= DIVA_0;BCSCTL2 |= DIVM_0 + DIVS_0;��ʱ�Ӳ���Ƶ��
				  baud<=9600: ʱ��=ACLK     baud>9600: ʱ��=SMCLK
*ʹ�÷�����UART_Set(9600,8,0,1,1);/������9600  8λ����λ  ����żУ��  1λֹͣλ  �������ж�
*****************************************************************/
void UART_Set(uchar baud,uchar data,char jiouwei,uchar stop,uchar R_T)
{
	UCA0CTL1 |=UCSWRST;//�����λ
	if(baud<=baud_9600)
	{
		UCA0CTL1 |= UCSSEL_1;//ACLK
	}
	else
	{
		UCA0CTL1 |= UCSSEL_2;//SMCLK
	}
	switch(baud)
	{
#if CPU_CLOCK == 16000000
		case baud_1200:
			UCA0BR0=27;//1200������													//�����ʼ���
			UCA0BR1 = 0;																		//������=BRCLK/N=(UBR+(M7+M6+M5+M4+M3+M2+M1+M0)/8
			UCA0MCTL = UCBRS1;// ������С������*8 = UCBRSx = 7	//���磺BRCLK=8MHz��Ҫ����BITCLK=115200Hz����Ƶ���ķ�Ƶϵ��Ϊ8000 / 115.2 =69.44444444
		break;																						//�������÷�Ƶ���ļ���ֵΪ69��С������0.44444444��Ϊ�����Ĵ����Ĳ�����ʹ�����¹�ʽ����
		case baud_2400: 																		//����������������UxMCTL��=8��С�����֣����ȡ�������֣�
			UCA0BR0 = 13;//2400������												//Ӧ����8��0.44444444=3.5,��ô������ȡ3��4
			UCA0BR1 = 0;
			UCA0MCTL = UCBRS2 + UCBRS1;
		break;
		case baud_4800:
			UCA0BR0 = 6;//4800������
			UCA0BR1 = 0;
			UCA0MCTL = UCBRS2 + UCBRS1 +UCBRS0;
		break;
		case baud_9600:
			UCA0BR0 = 3;//9600������
			UCA0BR1 = 0;
			UCA0MCTL = UCBRS1 +UCBRS0;
		break;
		case baud_19200:
			UCA0BR0 = 65;//19200������
			UCA0BR1 = 3;
			UCA0MCTL = UCBRS1;
		break;
		case baud_38400:
			UCA0BR0 = 0xa0;//38400������
			UCA0BR1 = 0x01;
			UCA0MCTL = UCBRS2 + UCBRS1;
		break;
		case baud_56000:
			UCA0BR0 = 0x1d;//57600������
			UCA0BR1 = 0x01; 
			UCA0MCTL = UCBRS2 + UCBRS1; 
		break; 
		case baud_115200:
			UCA0BR0 = 138;//115200������
			UCA0BR1 = 0; 
			UCA0MCTL = UCBRS2 + UCBRS1 + UCBRS0; 
		break;  
		default: 	 
			UCA0BR0 = 3;//9600������
			UCA0BR1 = 0;  
			UCA0MCTL = UCBRS1 +UCBRS0;
		break; 	 
	} 
#else
	#error "ʱ��MCLK����16M��uart�����������¼���!"
#endif
	if(data==8) 	 
	{	 
		UCA0CTL0 &= ~UC7BIT; 	 //8λ����λ	 
	} 	 
	else 	 
	{	 
		if(data==7) 	 //7λ����λ	 
		{	 
			UCA0CTL0|= UC7BIT;	 
		}  
		else 	 
		{	 
			UCA0CTL0 &= ~UC7BIT; 	 //Ĭ��8λ����λ 
		} 	 
	} 
	switch(jiouwei) 	 
	{	 
		case 0: UCA0CTL0 &= ~UCPEN;//��У�� 
			break; 
		case 1: UCA0CTL0 |= UCPEN; //��У�� 
			break; 
		case 2: UCA0CTL0 |= UCPEN + UCPAR; //żУ��	 
			break;  	 
		default: 	 
		UCA0CTL0 &= ~UCPEN;//Ĭ����У��	 
			break;
	} 
	if(stop==1) 	 
	{
		UCA0CTL0 &= ~UCSPB;//1λֹͣλ�� 
	}  
	else
	{
		if(stop==2)  
	{ 
			UCA0CTL0 |= UCSPB;//2λֹͣλ 
	} 
	else  
	{ 
		UCA0CTL0 &= ~UCSPB;//Ĭ��Ϊ1λֹͣλ	 
	} 	 
	} 
	UCA0CTL1 &=~UCSWRST;//��ʼ�� USCI_A0 ״̬��
	P1SEL|=BIT1+BIT2;// P1.1 = RXD, P1.2=TXD
	P1SEL2|=BIT1+BIT2; 
	//P1DIR|=BIT2;�ڶ�������������	 
	switch(R_T)//�жϿ��ƼĴ���2	 
	{	 
		case 1: 	 
		IE2 |= UCA0RXIE; 	 //���ж�	 
			break;  	 
		case 2: 	 
		IE2 |= UCA0TXIE; 	 //���ж� 
			break;  
		case 3: 	 
		IE2 |= UCA0RXIE+UCA0TXIE; //�շ��ж�	 
			break; 
		default:  
		IE2 |= UCA0RXIE;
			break; 	 
	} 
}

/****************************************************************
*���ڷ���һ�ֽں�������ѯ��ʽ
****************************************************************/
void UART_sent(uchar Chr)
{
	while (!(IFG2&UCA0TXIFG));// USCI_A0 TX buffer ready �ж��ϴ������Ƿ���
	UCA0TXBUF=Chr;
//	IFG2&=~UCA0TXIFG;
}

int putchar(int c)
{
	UART_sent((uchar)c);
	return c;
}

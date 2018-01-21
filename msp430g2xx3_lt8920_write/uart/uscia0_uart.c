#include <msp430g2553.h>

#include "uscia0_uart.h"

/*****************************************************************
*名称：UART_Set()
*功能：UART串口设置
*入口参数：
				baud: 波特率1200 2400 4800 9600(默认) 19200 38400 56000 115200
				data: 数据位，8：8位，7：7位，默认8位
				jiouwei; 奇偶位，0:无（默认）,1:奇校验,2:偶校验
				stop: 停止位，2:2位停止位，其他均为默认的1位
				R_T: 中断模式，1：收（默认）；2：发；3：收发
*出口参数：无
*时钟说明：ACLK = 32.768KHz, SMCLK = CALxxx_16MHZ = 16MHz
				  BCSCTL1 |= DIVA_0;BCSCTL2 |= DIVM_0 + DIVS_0;（时钟不分频）
				  baud<=9600: 时钟=ACLK     baud>9600: 时钟=SMCLK
*使用范例：UART_Set(9600,8,0,1,1);/波特率9600  8位数据位  无奇偶校验  1位停止位  开启收中断
*****************************************************************/
void UART_Set(uchar baud,uchar data,char jiouwei,uchar stop,uchar R_T)
{
	UCA0CTL1 |=UCSWRST;//软件复位
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
			UCA0BR0=27;//1200波特率													//波特率计算
			UCA0BR1 = 0;																		//波特率=BRCLK/N=(UBR+(M7+M6+M5+M4+M3+M2+M1+M0)/8
			UCA0MCTL = UCBRS1;// 波特率小数部分*8 = UCBRSx = 7	//例如：BRCLK=8MHz，要产生BITCLK=115200Hz，分频器的分频系数为8000 / 115.2 =69.44444444
		break;																						//所以设置分频器的计数值为69。小数部分0.44444444作为修整寄存器的参数，使用如下公式计算
		case baud_2400: 																		//波特率修整参数（UxMCTL）=8×小数部分（结果取整数部分）
			UCA0BR0 = 13;//2400波特率												//应该是8×0.44444444=3.5,那么，可以取3或4
			UCA0BR1 = 0;
			UCA0MCTL = UCBRS2 + UCBRS1;
		break;
		case baud_4800:
			UCA0BR0 = 6;//4800波特率
			UCA0BR1 = 0;
			UCA0MCTL = UCBRS2 + UCBRS1 +UCBRS0;
		break;
		case baud_9600:
			UCA0BR0 = 3;//9600波特率
			UCA0BR1 = 0;
			UCA0MCTL = UCBRS1 +UCBRS0;
		break;
		case baud_19200:
			UCA0BR0 = 65;//19200波特率
			UCA0BR1 = 3;
			UCA0MCTL = UCBRS1;
		break;
		case baud_38400:
			UCA0BR0 = 0xa0;//38400波特率
			UCA0BR1 = 0x01;
			UCA0MCTL = UCBRS2 + UCBRS1;
		break;
		case baud_56000:
			UCA0BR0 = 0x1d;//57600波特率
			UCA0BR1 = 0x01; 
			UCA0MCTL = UCBRS2 + UCBRS1; 
		break; 
		case baud_115200:
			UCA0BR0 = 138;//115200波特率
			UCA0BR1 = 0; 
			UCA0MCTL = UCBRS2 + UCBRS1 + UCBRS0; 
		break;  
		default: 	 
			UCA0BR0 = 3;//9600波特率
			UCA0BR1 = 0;  
			UCA0MCTL = UCBRS1 +UCBRS0;
		break; 	 
	} 
#else
	#error "时钟MCLK不是16M，uart波特率需重新计算!"
#endif
	if(data==8) 	 
	{	 
		UCA0CTL0 &= ~UC7BIT; 	 //8位数据位	 
	} 	 
	else 	 
	{	 
		if(data==7) 	 //7位数据位	 
		{	 
			UCA0CTL0|= UC7BIT;	 
		}  
		else 	 
		{	 
			UCA0CTL0 &= ~UC7BIT; 	 //默认8位数据位 
		} 	 
	} 
	switch(jiouwei) 	 
	{	 
		case 0: UCA0CTL0 &= ~UCPEN;//无校验 
			break; 
		case 1: UCA0CTL0 |= UCPEN; //奇校验 
			break; 
		case 2: UCA0CTL0 |= UCPEN + UCPAR; //偶校验	 
			break;  	 
		default: 	 
		UCA0CTL0 &= ~UCPEN;//默认无校验	 
			break;
	} 
	if(stop==1) 	 
	{
		UCA0CTL0 &= ~UCSPB;//1位停止位， 
	}  
	else
	{
		if(stop==2)  
	{ 
			UCA0CTL0 |= UCSPB;//2位停止位 
	} 
	else  
	{ 
		UCA0CTL0 &= ~UCSPB;//默认为1位停止位	 
	} 	 
	} 
	UCA0CTL1 &=~UCSWRST;//初始化 USCI_A0 状态机
	P1SEL|=BIT1+BIT2;// P1.1 = RXD, P1.2=TXD
	P1SEL2|=BIT1+BIT2; 
	//P1DIR|=BIT2;第二功能无需配置	 
	switch(R_T)//中断控制寄存器2	 
	{	 
		case 1: 	 
		IE2 |= UCA0RXIE; 	 //收中断	 
			break;  	 
		case 2: 	 
		IE2 |= UCA0TXIE; 	 //发中断 
			break;  
		case 3: 	 
		IE2 |= UCA0RXIE+UCA0TXIE; //收发中断	 
			break; 
		default:  
		IE2 |= UCA0RXIE;
			break; 	 
	} 
}

/****************************************************************
*串口发送一字节函数，查询方式
****************************************************************/
void UART_sent(uchar Chr)
{
	while (!(IFG2&UCA0TXIFG));// USCI_A0 TX buffer ready 判断上次数据是否发完
	UCA0TXBUF=Chr;
//	IFG2&=~UCA0TXIFG;
}

int putchar(int c)
{
	UART_sent((uchar)c);
	return c;
}

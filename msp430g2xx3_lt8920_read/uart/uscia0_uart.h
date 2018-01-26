#ifndef __USCIA0_UART
#define __USCIA0_UART

#include "../system.h"

enum BAUD
{
	baud_1200 , baud_2400 , baud_4800 , baud_9600 , baud_19200 , baud_38400 , baud_56000 , baud_115200
};

void UART_Set(uchar baud,uchar data,char jiouwei,uchar stop,uchar R_T);
void UART_sent(uchar Chr) ;
//int putchar(int c);//÷ÿ∂®“Âprintf

#endif /* #ifndef __MSP430G2XX3_USCIA0_UART */

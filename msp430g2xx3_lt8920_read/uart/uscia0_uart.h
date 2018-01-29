#ifndef __USCIA0_UART
#define __USCIA0_UART

#include "../system.h"

enum BAUD
{
	baud_1200 , baud_2400 , baud_4800 , baud_9600 , baud_19200 , baud_38400 , baud_56000 , baud_115200
};

extern char UARTdata[70];
extern uint8_t UARTnum;

void UART_Set(uchar baud,uchar data,char jiouwei,uchar stop,uchar R_T);
void UART_sent(uchar Chr) ;
void UART_StringSend(char *str);//发送字符串
void UART_StringRead(char *str);//接收字符串
//int putchar(int c);//重定义printf

#endif /* #ifndef __MSP430G2XX3_USCIA0_UART */

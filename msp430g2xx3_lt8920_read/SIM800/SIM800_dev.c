/*
 * SIM800_dev.c
 *
 *  Created on: 2018年1月28日
 *      Author: zhouLJ
 */
#include <msp430.h>
#include "../uart/uscia0_uart.h"
#include "../printf-stdarg.h"

int8_t SIM800_Send(char *ord,char time)//ord:发送的命令 rev:期望得到的回复
{
	uint8_t rev;
	UART_StringSend(ord);
	IE2 |= UCA0RXIE; 	 //收中断
	while(time--)
		Delay_ms(100);
	IE2 |= UCA0RXIE; 	 //关中断
	UARTdata[UARTnum-1]='\0';
	rev=UARTnum;
	UARTnum=0;
#ifdef DEBUG
	printf("%s\n",UARTdata);
#endif
	return rev;
}

int8_t SIM800_TCPSendData(char *data)
{
//	char r[50];

	SIM800_Send("AT+CSQ\r\n", 1);

	SIM800_Send("AT+CIPSTART=\"TCP\",\"103.44.145.245\",35682\r\n", 13);

	SIM800_Send("AT+CIPSEND\r\n", 1);

	SIM800_Send(data, 4);

	SIM800_Send("AT+CIPCLOSE=1\r\n", 1);

	SIM800_Send("AT+CIPSHUT\r\n", 1);

	return 0;
}


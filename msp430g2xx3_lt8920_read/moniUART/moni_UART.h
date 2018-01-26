/*模拟串口配置函数库*/
/*1.使用了两个IO口，可以调整
  2.使用了定时器2，可以调整  
  3.使用滴答定时器作为发送过程的波特率调整*/
#ifndef __moni_UART_H
#define __moni_UART_H

#include "../system.h"
#include <msp430.h>

void VirtualCOM_ByteSend(uint8_t val);//发送一个uint8_t
void VirtualCOM_StringSend(uint8_t *str);//发送字符串
void VirtualCOM_GPIOConfig(void);//IO口初始化
int putchar(int c);//重定义printf

#endif /* __moni_UART_H */

/*ģ�⴮�����ú�����*/
/*1.ʹ��������IO�ڣ����Ե���
  2.ʹ���˶�ʱ��2�����Ե���  
  3.ʹ�õδ�ʱ����Ϊ���͹��̵Ĳ����ʵ���*/
#ifndef __moni_UART_H
#define __moni_UART_H

#include "../system.h"
#include <msp430.h>

void VirtualCOM_ByteSend(uint8_t val);//����һ��uint8_t
void VirtualCOM_StringSend(uint8_t *str);//�����ַ���
void VirtualCOM_GPIOConfig(void);//IO�ڳ�ʼ��
int putchar(int c);//�ض���printf

#endif /* __moni_UART_H */

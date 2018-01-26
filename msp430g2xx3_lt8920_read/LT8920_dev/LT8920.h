/*
 * LT8920.h
 *
 *  Created on: 2017��12��9��
 *      Author: zhouLJ
 */

#ifndef _LT8920_H_
#define _LT8920_H_

#include "../system.h"
#include <msp430.h>

//extern uint8_t RegH,RegL;
//extern uint8_t RSSI;

uint8_t SPI_ReadWriteByte(uint8_t data);
void LT8920_writereg(uint8_t reg,uint8_t H,uint8_t L);
void LT8920_readreg(uint8_t reg);
void LT8920_init(void);
int8_t LT8920_TX(uint8_t oder, const uint8_t *temp);//����ʧ�ܷ���-1���ɹ����ط��͸���
inline void LT8920_RXconf(uint8_t channel);
int8_t LT8920_RX(uint8_t *const data);//����ʧ�ܷ���-1���ɹ����ط��͸���
inline void LT8920_TXRX_exit(void);
void LT8920_read_RSSI(void);
//void readKa(void);//Ѱ��

#endif /* _LT8920_H_ */

/*
 * system.h
 *
 *  Created on: 2017��12��10��
 *      Author: dell
 */

#ifndef SYSTEM_H_
#define SYSTEM_H_

#include <stdint.h>

#ifndef CPU_CLOCK//ϵͳʱ��MCLK��16M
	#define CPU_CLOCK 16000000
#endif

#ifndef uchar
#define uchar unsigned char
#endif
#ifndef uint
#define uint unsigned int
#endif

//******************************************************************************************
//��ʱ����
//******************************************************************************************
#ifdef CPU_CLOCK
	#define Delay_us(us)    __delay_cycles(CPU_CLOCK/1000000u*(us))
	#define Delay_ms(ms)   __delay_cycles( (ms) * CPU_CLOCK/1000u  )
 #else
	#error "ʱ��MCLK��Ƶ��CPU_CLOCKû�ж���!"
#endif

#endif /* SYSTEM_H_ */

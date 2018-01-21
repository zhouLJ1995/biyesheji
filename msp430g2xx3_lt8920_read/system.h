/*
 * system.h
 *
 *  Created on: 2017年12月10日
 *      Author: dell
 */

#ifndef SYSTEM_H_
#define SYSTEM_H_

#include <stdint.h>

#ifndef CPU_CLOCK//系统时钟MCLK是16M
	#define CPU_CLOCK 16000000
#endif

#ifndef uchar
#define uchar unsigned char
#endif
#ifndef uint
#define uint unsigned int
#endif

//******************************************************************************************
//延时函数
//******************************************************************************************
#ifdef CPU_CLOCK
	#define Delay_us(us)    __delay_cycles(CPU_CLOCK/1000000u*(us))
	#define Delay_ms(ms)   __delay_cycles( (ms) * CPU_CLOCK/1000u  )
 #else
	#error "时钟MCLK的频率CPU_CLOCK没有定义!"
#endif

#endif /* SYSTEM_H_ */

/*
 * printf-stdarg.h
 *
 *  Created on: 2015��10��7��
 *      Author: bobde163
 */

#ifndef PRINTF_STDARG_H_
#define PRINTF_STDARG_H_

#ifdef __cpluslpus
  extern "C" {
#endif

#include "stdarg.h"//C���Ա�׼�⺯����Ϊ�ú����ܹ����տɱ����

/*�ɵ�������д��΢�ʹ�ӡ���ܺ�����ʵ�ֻ����ĸ�ʽ�����*/
extern int printf(const char *format, ...);
extern int sprintf(char *out, const char *format, ...);
extern int snprintf( char *buf, unsigned int count, const char *format, ... );

/*����ֲʱ��Ҫʵ�ֵĵ����ַ���ʾ����*/
extern int putchar(int c);

#ifdef __cplusplus
  }
#endif
#endif /* PRINTF_STDARG_H_ */

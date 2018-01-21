/*
 * printf-stdarg.h
 *
 *  Created on: 2015年10月7日
 *      Author: bobde163
 */

#ifndef PRINTF_STDARG_H_
#define PRINTF_STDARG_H_

#ifdef __cpluslpus
  extern "C" {
#endif

#include "stdarg.h"//C语言标准库函数，为让函数能够接收可变参数

/*由第三方编写的微型打印功能函数，实现基本的格式化输出*/
extern int printf(const char *format, ...);
extern int sprintf(char *out, const char *format, ...);
extern int snprintf( char *buf, unsigned int count, const char *format, ... );

/*在移植时需要实现的单个字符显示函数*/
extern int putchar(int c);

#ifdef __cplusplus
  }
#endif
#endif /* PRINTF_STDARG_H_ */

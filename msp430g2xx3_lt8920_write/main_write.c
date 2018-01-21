//******************************************************************************
//   MSP430G2xx3 - USCI_A0, UART
//
//   Description: Echo a received character, RX ISR used. Normal mode is LPM0.
//   USCI_A0 RX interrupt triggers TX Echo.
//   baud: 1200 2400 4800 9600(默认)19200 38400 56000 115200
//   ACLK = 32.768KHz, MCLK = SMCLK = CALxxx_16MHZ = 16MHz
//
//                MSP430G2xx3
//             -----------------
//         /|\|              XIN|-
//          | |                 |32kHz
//          --|RST          XOUT|-
//            |                 |
//            |     P1.2/UCA0TXD|------------>
//            |                 | 115200 - 8N1
//            |     P1.1/UCA0RXD|<------------
//
//******************************************************************************
/*
 * msp430g2xx3_uscia0_uart_main.c
 *
 *  Created on: 2017年7月28日
 *      Author:  周灵杰
 */
//******************************************************************************
#include <msp430.h>

#include "LT8920_dev/LT8920.h"
#include "system.h"
#include "uart/uscia0_uart.h"
#include "printf-stdarg.h"

int main(void)
{
//	int8_t i;

	WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
	if (CALBC1_16MHZ==0xFF)					//
	{
		while(1);                               // do not load, trap CPU!!
	}
	//__delay_cycles(5000);
	DCOCTL = 0;                               // Select lowest DCOx and MODx settings
	BCSCTL1 |= CALBC1_16MHZ + DIVA_1;//MCLK = SMCLK = CALxxx_1MHZ = 1MHz ACLK二分频
	DCOCTL |= CALDCO_16MHZ;     // Set DCO 需要在system.h中定义CPU_CLOCK
	BCSCTL2 |= DIVM_0 + DIVS_0;//不分频

	//timer
	CCTL0 = CCIE;                             // CCR0 interrupt enabled
	CCR0 = 0xffff;
	TACTL = TASSEL_1 + MC_1;                  //ACLK, upmode

	//LED
	P1DIR |= 0x40;
	P1OUT |= 0x40;

	LT8920_init();    		//初始化LT8920
//	LT8920_TXconf(0x7f);

	UART_Set(baud_115200,8,0,1,1);
	printf("周灵杰");//

	__bis_SR_register(GIE);       //GIE是打开全局中断使能
	while(1)
	{
		LT8920_TX(4, "abcd");
		__bis_SR_register(LPM3_bits);
//		Delay_ms(100);
	}
}

//  Echo back RXed character, confirm TX buffer is ready first
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=USCIAB0RX_VECTOR
__interrupt void USCI0RX_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(USCIAB0RX_VECTOR))) USCI0RX_ISR (void)
#else
#error "编译器不支持!"
#endif
{
	while (!(IFG2&UCA0TXIFG));                 // USCI_A0 TX buffer ready?
	UCA0TXBUF = UCA0RXBUF;                     // TX -> RXed character
}

// Timer A0 interrupt service routine
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer_A (void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(TIMER0_A0_VECTOR))) Timer_A (void)
#else
#error Compiler not supported!
#endif
{
	P1OUT ^= 0x40;                            // Toggle P1.6
	__bic_SR_register_on_exit(LPM3_bits);
}

//#pragma vector =PORT1_VECTOR
//__interrupt void Port1_ISR(void)
//{
//	if( P1IFG & BIT3 )
//	{
//		P1IFG &= ~BIT3;
//		__bic_SR_register_on_exit(LPM3_bits);
//	}
//}

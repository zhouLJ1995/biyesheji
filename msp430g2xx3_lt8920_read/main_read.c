//******************************************************************************
//                MSP430G2xx3
//             -----------------
//         /|\|              XIN|-
//          | |                 |
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
#include "moniUART/moni_UART.h"
#include "printf-stdarg.h"

#define DEBUG

int main(void)
{
	int8_t i;
	uint8_t data[10];
	WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
	if (CALBC1_16MHZ==0xFF)					//
	{
		while(1);                               // do not load, trap CPU!!
	}

	DCOCTL = 0;                               // Select lowest DCOx and MODx settings
	BCSCTL1 |= CALBC1_16MHZ + DIVA_0;//MCLK = SMCLK = CALxxx_1MHZ = 1MHz
	DCOCTL |= CALDCO_16MHZ;     // Set DCO 需要在system.h中定义CPU_CLOCK
	BCSCTL2 |= DIVM_0 + DIVS_0;//不分频

//	//timer
//	CCTL0 = CCIE;                             // CCR0 interrupt enabled
//	CCR0 = 0x4fff;
//	TACTL = TASSEL_2 + MC_1;                  //SMCLK, upmode
#ifdef DEBUG
	VirtualCOM_GPIOConfig();//初始化模拟UART 波特率115200 P2.3
//	printf("周灵杰\n");//
	printf("Author : zhouLJ\n");
	printf("System start...\n");
#endif

	//LED
	P1DIR |= 0x40;
	P1OUT |= 0x40;

	LT8920_init();    		//初始化LT8920
	UART_Set(baud_115200,8,0,1,1);

	__bis_SR_register(GIE);//GIE是打开全局中断使能
	LT8920_RXconf(0x7f);
	while(1)
	{
		LT8920_read_RSSI();
		i=LT8920_RX(data);
		if( i>0 )
		{
			LT8920_RXconf(0x7f);
			data[i]='\0';
			printf("%s\n",data );
		}
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

#pragma vector =PORT2_VECTOR
__interrupt void Port2_ISR(void)
{
	if( P2IFG & BIT2 )
	{
		P2IFG &= ~BIT2;
		__bic_SR_register_on_exit(LPM3_bits);
	}
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

}

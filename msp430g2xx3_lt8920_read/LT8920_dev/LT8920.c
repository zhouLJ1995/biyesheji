/*
 * NRF24L02.c
 *
 *  Created on: 2017��12��9��
 *      Author: zhouLJ
 */
#include "LT8920.h"

#include "../printf-stdarg.h"

//=======================LT8920_RST�˿�=========================================
#define LT8920_RST_0 P1OUT &=~BIT3
#define LT8920_RST_1 P1OUT |= BIT3
//=============================LT8920_CS�˿�==================================
#define LT8920_CS_0 P2OUT &=~BIT1
#define LT8920_CS_1 P2OUT |= BIT1
//=============================LT8920_SCK�˿�======================================
#define LT8920_SCK_0 P2OUT &=~BIT0
#define LT8920_SCK_1 P2OUT |= BIT0
//============================= LT8920_MOSI�˿�================================
#define LT8920_MOSI_0 P1OUT &=~BIT5
#define LT8920_MOSI_1 P1OUT |= BIT5
//=============================LT8920_MISO�˿�=========================================
#define LT8920_MISO (P1IN & BIT4)
//==========================LT8920_PKT״̬============================================
#define LT8920_PKT (P2IN & BIT2)

//#define channel 0x7f  //Ƶ��
uint8_t RegH,RegL;
uint8_t RSSI;

//==============================================================================
//������char SPI_ReadWriteByte(uchar)
//���ܣ�NRF24L01��SPIдʱ��
//******************************************************************************
uint8_t SPI_ReadWriteByte(uint8_t data)
{
	uint8_t i,temp=0;
	for(i=0;i<8;i++) // output 8-bit
	{
		if((data & 0x80)==0x80)
		{
			LT8920_MOSI_1; // output 'uchar', MSB to MOSI
		}
		else
		{
			LT8920_MOSI_0;
		}
		data = (data << 1); // shift next bit into MSB..
		temp<<=1;
		LT8920_SCK_1; // Set SCK high..
		if( LT8920_MISO )
			temp++; // capture current MISO bit
		LT8920_SCK_0; // ..then set SCK low again
	}
	return(temp); // return read uchar
}


void LT8920_writereg(uint8_t reg,uint8_t H,uint8_t L)
{
	LT8920_CS_0;
	Delay_us(1);
	SPI_ReadWriteByte(reg);
	Delay_us(1);
	SPI_ReadWriteByte(H);
	Delay_us(1);
	SPI_ReadWriteByte(L);
	Delay_us(1);
	LT8920_CS_1;
}

void LT8920_readreg(uint8_t reg)
{
	LT8920_CS_0;
	Delay_us(1);
	SPI_ReadWriteByte(reg+0x80);
	Delay_us(1);
	RegH=SPI_ReadWriteByte(0xff);
	Delay_us(1);
	RegL=SPI_ReadWriteByte(0xff);
	Delay_us(1);
	LT8920_CS_1;
}

inline void LT8920_RXconf(uint8_t channel)
{
	LT8920_writereg(52,0x80,0x80);//���FIFO
	LT8920_writereg(7,0x00,0x80+channel);//���ý���Ƶ��
}

//���պ���
int8_t LT8920_RX(uint8_t *const data)
{
	int8_t i=-1;
	uint8_t n;

//	LT8920_writereg(52,0x80,0x80);//���FIFO
//	LT8920_writereg(7,0x00,0x80+0x7fl);//���ý���Ƶ��
//
//	while(LT8920_PKT==0){i++;if(i>0x7f){break;}};//�ȴ�����

	if(LT8920_PKT)
	{
		__bic_SR_register(GIE);//�ر�ȫ���ж�ʹ��
		LT8920_readreg(48);//��ѯcrc�Ƿ���ȷ
		if((RegH&0x80)==0)
		{
			LT8920_readreg(50);          //FIFO���ݶ�ȡ���
			n=RegH;
//			data[1]=RegL;
			printf("%d\n",n);

			while( i<n )
			{
				LT8920_readreg(50);          //FIFO���ݶ�ȡ���
				data[++i]=RegH;
				data[++i]=RegL;
			}
			printf("RSSI=%d\n",RSSI);
			data[++i]=RSSI;
			RSSI=0;
		}
		__bis_SR_register(GIE);//����ȫ���ж�ʹ��
	}
	return i;
}

//���ͺ���
int8_t LT8920_TX(uint8_t oder, const uint8_t *temp)
{
	uint32_t i=0;
	int8_t j=0;

	LT8920_readreg(5);
	printf("%x %x\n",RegH,RegL);

	LT8920_writereg(7,0x00,0);	//ֹͣ�ӷ�����idel״̬
	LT8920_writereg(52,0x80,0x80);//���fifo
	LT8920_writereg(50,oder+1,0);//���͵����� ��һ���ֽھ������� �û����ݵ��ֽ���
	while( j<oder )
		LT8920_writereg(50,temp[j++],temp[j++]);
	LT8920_writereg(7,0x01,0x7f);                   //����״̬�����÷���Ƶ��
	while(LT8920_PKT==0)
	{
		i++;
		if(i>0xafff) {j=-1;break;}//
	}
//	if(LT8920_PKT==1)LED1=~LED1;
	return j;
}

inline void LT8920_TXRX_exit(void)
{
	LT8920_writereg(7,0x00,0);	//ֹͣ���շ��� �������״̬
}

void LT8920_read_RSSI(void)
{
	LT8920_readreg(6);          //RSSI���ݶ�ȡ���
	if( (RegH>>2) > RSSI )
		RSSI=RegH>>2;
}

void LT8920_init()
{
//	P2REN |= BIT1+BIT0;//����
	P2DIR |= BIT1+BIT0;//��� ��λʱΪ0��Ĭ������
	P1DIR |= BIT3+BIT5;

//	P2DIR &= ~BIT2; //IN
//	P2OUT  &= ~BIT2;
////	P2SEL &= ~BIT2;//ѡ��IO����
//	P2IES &= ~BIT2;//�����ش���
//	P2IFG &= ~BIT2; //����жϱ�־
//	P2IE |= BIT2;//����BIT2�ж�

	LT8920_RST_0;
	Delay_ms(10);
	LT8920_RST_1;
	Delay_ms(50);
	LT8920_CS_0;
	Delay_ms(10);

	LT8920_writereg(0, 0x6F, 0xE0);
	LT8920_writereg(1, 0x56, 0x81);
	LT8920_writereg(2, 0x66, 0x17);
	LT8920_writereg(4, 0x9C, 0xC9);
	LT8920_writereg(5, 0x66, 0x37);
	LT8920_writereg(7, 0x00, 0x00);// use for setting 	RF frequency and to start or stop TX /RX packets
	LT8920_writereg(8, 0x6C, 0x90);
	LT8920_writereg(9, 0x18, 0x40);//set TX power level
	LT8920_writereg(10, 0x7F, 0xFD);//crystal osc.enabled
	LT8920_writereg(11, 0x00, 0x08);//rssi enabled
	LT8920_writereg(12, 0x00, 0x00);
	LT8920_writereg(13, 0x48, 0xBD);

	LT8920_writereg(22, 0x00, 0xff);
	LT8920_writereg(23, 0x80, 0x05);
	LT8920_writereg(24, 0x00, 0x67);
	LT8920_writereg(25, 0x16, 0x59);
	LT8920_writereg(26, 0x19, 0xE0);
	LT8920_writereg(27, 0x13, 0x00);
	LT8920_writereg(28, 0x18, 0x00);

	LT8920_writereg(32, 0x48, 0x00);//set preamble_len :3 byes.  set syncword_len:16 bits ----Reg36[15:0]
	LT8920_writereg(33, 0x3f, 0xC7);
	LT8920_writereg(34, 0x20, 0x00);
	LT8920_writereg(35, 0x03, 0x00);
	LT8920_writereg(36, 0x05, 0x0A);//set sync words
	//	LT8920_writereg(37, 0x03, 0x80);//set sync words
	//	LT8920_writereg(38, 0x5A, 0x5A);//set sync words
	LT8920_writereg(39, 0x55, 0xAA);//set sync words
	LT8920_writereg(40, 0x44, 0x01);
	LT8920_writereg(41, 0xb0, 0x00);//crc on scramble off ,1st byte packet length ,auto ack  off
	LT8920_writereg(42, 0xFD, 0xB0);
	LT8920_writereg(43, 0x00, 0x0F);//configure scan_rssi
	LT8920_writereg(44, 0x10, 0x00);
	LT8920_writereg(45, 0x05, 0x52);
	LT8920_writereg(50, 0x00, 0x00);
	Delay_ms(10);
}

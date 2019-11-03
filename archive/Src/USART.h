#ifndef __USART_H
#define __USART_H
//#define u16 unsigned long int
//#define u8 unsigned int
//请无视下面报错   添加宏定义会冲突导致乱码  又不是不能用
	
extern void USART1_SendByte(u16 dat);
extern void USART1Write(u8* data,u16 len);

void USART1_Configuration(void);

extern void USART2_SendByte(u16 dat);
extern void USART2Write(u8* data,u16 len);

void USART2_Configuration(void);
void UART2Test(void);

#define DIR485_H  GPIOC->BSRR=1<<1
#define DIR485_L  GPIOC->BRR=1<<1 


#endif

#include "stm32f10x.h"
#include "USART.h"

void USART1_SendByte(u16 Data)
{ 
   while (!(USART1->SR & USART_FLAG_TXE));
   USART1->DR = (Data & (uint16_t)0x01FF);	 
   
}
void USART1Write(u8* data,u16 len)
{
	u16 i;
//	DIR485_H ;
	for (i=0; i<len-1; i++){
		USART1_SendByte(data[i]);
	}
//	Delay10us(1000);
//	DIR485_L;		
}	
void USART1_Configuration(void)
{
	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate            = 115200  ;
	USART_InitStructure.USART_WordLength          = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits            = USART_StopBits_1;
	USART_InitStructure.USART_Parity              = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode                = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1, &USART_InitStructure);
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	USART_Cmd(USART1, ENABLE);     
/*	DIR485_L;  */
}

void USART1_IRQHandler(void)
{
	u8 c;
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
	{ 	
	    c=USART1->DR;
		USART1_SendByte(c); 	    
	} 
	 
}
/****************************************************************************************************************************

说真的，上面串口一的函数没啥用吧


*******************************************************************************************************************************/
void USART2_SendByte(u16 Data)
{ 
   while (!(USART2->SR & USART_FLAG_TXE));
   USART2->DR = (Data & (uint16_t)0x01FF);	 
   
}
void USART2Write(u8* data,u16 len)
{
	u16 i;
//	DIR485_H ;
	for (i=0; i<len; i++){
		USART2_SendByte(data[i]);
	}
//	Delay10us(1000);
//	DIR485_L;		
}	
void USART2_Configuration(void)
{
	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate            = 115200  ;
	USART_InitStructure.USART_WordLength          = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits            = USART_StopBits_1;
	USART_InitStructure.USART_Parity              = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode                = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART2, &USART_InitStructure);
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
	USART_Cmd(USART2, ENABLE);     
/*	DIR485_L;  */
}

void USART2_IRQHandler(void)
{
	u8 c;
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
	{ 	
	    c=USART2->DR;
		USART2_SendByte(c); 	    
	} 
	 
}
/**************************************************************************************
移植的测试函数 接受立即返回


**********************************************************************************/
/*接收一个字节数据*/
unsigned char UART2GetByte(unsigned char* GetData)
{   	   
        if(USART_GetFlagStatus(USART2, USART_FLAG_RXNE) == RESET)
        {  return 0;//没有收到数据 
		}
        *GetData = USART_ReceiveData(USART2); 
        return 1;//收到数据
}
/*接收一个数据，马上返回接收到的这个数据*/
void UART2Test(void)
{
       unsigned char i = 0;

       while(1)
       {    
		 while(UART2GetByte(&i))
        {
				if (i=='0') GPIO_SetBits(GPIOA,GPIO_Pin_1);	  // 灭;
				if (i=='1') GPIO_ResetBits(GPIOA,GPIO_Pin_1);	  // 亮;
         USART_SendData(USART2,i);
        }      
       }     
}





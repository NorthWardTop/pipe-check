/**************************************************************
西安石油大学大创项目--无人机管道自动巡检
基于stm32_esp8266_ov2640
版本 0.1.1
完成日期 2018-11-24 15:11
修改人：牧之
***************************************************************/


#include "stm32f10x.h"
//#include "GLCD.h"  //貌似屏幕驱动函数？应该没用 已经被我删掉了
#include "USART.h"
#include <stdio.h>


#ifdef __GNUC__
/* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printfset to 'Yes') calls __io_putchar() */
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */

void GPIO_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	/* Configure IO connected to LD1, LD2, LD3 and LD4 leds *********************/	
	//GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11;
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;    //使用PA1点亮LED
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_Init(GPIOA, &GPIO_InitStructure);
		GPIO_SetBits(GPIOA, GPIO_Pin_1 );	 // 关闭所有LED
	
	/***********************************************************************************
	串口1部分没用   后期删掉
	********************************************************************************/
   	/* Configure USART1 Tx (PA.09) as alternate function push-pull */
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_Init(GPIOA, &GPIO_InitStructure);
    
  	/* Configure USART1 Rx (PA.10) as input floating */
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* Configure USART2 Tx (PA.02) as alternate function push-pull */
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_Init(GPIOA, &GPIO_InitStructure);
    
  	/* Configure USART2 Rx (PA.03) as input floating */
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  	GPIO_Init(GPIOA, &GPIO_InitStructure);
		
}

//系统中断管理
void NVIC_Configuration(void)
{ 
  	/* Configure the NVIC Preemption Priority Bits */  
  	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);

	#ifdef  VECT_TAB_RAM  
	  /* Set the Vector Table base location at 0x20000000 */ 
	  NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0); 
	#else  /* VECT_TAB_FLASH  */
	  /* Set the Vector Table base location at 0x08000000 */ 
	  NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);   
	#endif
}

//配置系统时钟,使能各外设时钟
void RCC_Configuration(void)
{
	SystemInit();	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA 
                           |RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC
                           |RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE
						   |RCC_APB2Periph_ADC1  | RCC_APB2Periph_AFIO 
                           |RCC_APB2Periph_SPI1, ENABLE );
  // RCC_APB2PeriphClockCmd(RCC_APB2Periph_ALL ,ENABLE );
     RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4 | RCC_APB1Periph_USART2
                           |RCC_APB1Periph_USART3|RCC_APB1Periph_TIM2	                           
                           , ENABLE );
	 RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
}

//~ void InitDis(void) 
//~ {
   //~ /* LCD Module init */
   //~ GLCD_init();
   //~ GLCD_clear(White);
   //~ GLCD_setTextColor(Blue);
   //~ GLCD_displayStringLn(Line1, "     FireBull");
   //~ GLCD_displayStringLn(Line2, "   USART example");
   //~ GLCD_setTextColor(Red);
//~ }

//配置所有外设
void Init_All_Periph(void)
{
	RCC_Configuration();	
	//InitDis();
	GPIO_Configuration();
	NVIC_Configuration();
	//USART1_Configuration();
	USART2_Configuration();
	//USART1Write((u8*)"    FireBull  USART_example ",sizeof("    FireBull  USART_example "));
	//USART2Write((u8*)"    FireBull  USART_example ",sizeof("    FireBull  USART_example ")-1);
}

void Delay(vu32 nCount)
{
  for(; nCount != 0; nCount--);
}


//上电stm32自动初始化esp8266
void SendMessage(void)
{ 
		
	Delay(8000000);
  USART2Write((u8*)"AT+CWJAP_DEF=\"K2P\",\"19216811\"\r\r\n",sizeof("AT+CWJAP_DEF=\"K2P\",\"19216811\"\r\r\n")-1) ;
	Delay(32000000);
	USART2Write((u8*)"AT+CIPSTART=\"TCP\",\"192.168.2.174\",8080\r\r\n",sizeof("AT+CIPSTART=\"TCP\",\"192.168.2.174\",8080\r\r\n")-1) ;
	Delay(8000000);
	USART2Write((u8*)"AT+CIPMODE=1\r\r\n",sizeof("AT+CIPMODE=1\r\r\n")-1);
	Delay(8000000);
	USART2Write((u8*)"AT+CIPSEND\r\r\n",sizeof("AT+CIPSEND\r\r\n")-1) ;
	Delay(8000000);
	USART2Write((u8*)"Hello World! USART2_Test\r\n",sizeof("Hello WorldUSART2_Test\r\n")-1) ;	

}

//命令一定注意换行符和指针下标  少一个字符都不行  折腾大半晚上的拼接ASCII码  必要时请使用16进制查看
//附抓取的AT指令16进制表达方式  如异常请自行对比
/***************************************************************************************************************************
41 54 2B 43 57 4A 41 50 5F 44 45 46 3D 22 4B 32 50 22 2C 22 31 39 32 31 36 38 31 31 22 0D 0D 0A
41 54 2B 43 49 50 53 54 41 52 54 3D 22 54 43 50 22 2C 22 31 39 32 2E 31 36 38 2E 32 2E 31 37 34 22 2C 38 30 38 30 0D 0D 0A
41 54 2B 43 49 50 4D 4F 44 45 3D 31 0D 0D 0A 0D 0A 4F 4B 0D 0A
41 54 2B 43 49 50 53 45 4E 44 0D 0D 0A 0D 0A 4F 4B 0D 0A 0D 0A 3E

AT+CWJAP_DEF="K2P","19216811"
AT+CIPSTART="TCP","192.168.2.174",8080
AT+CIPMODE=1
AT+CIPSEND
***************************************************************************************************************************/
int main(void)
{  
	Init_All_Periph();
	SendMessage();
	USART2Write((u8*)"go out",sizeof("go out")-1) ;
	USART2Write((u8*)"go fun in ",sizeof("go fun in ")-1) ;
 while(1)
  {
		GPIO_ResetBits(GPIOA,GPIO_Pin_1);			  // PA2输出高电平亮
		//Delay(30);
		//GPIO_SetBits(GPIOA,GPIO_Pin_1);	  // 灭
		//Delay(30);
		UART2Test();				//移植的测试函数 接受立即返回  函数定义位于USART.c
  	}
}
/*******************************************************************************
* Function Name  : PUTCHAR_PROTOTYPE
* Description    : Retargets the C library printf function to the USART.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
PUTCHAR_PROTOTYPE
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the USART */
  USART_SendData(USART1, (u8) ch);

  /* Loop until the end of transmission */
  while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
  {}

  return ch;
}
#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param file: pointer to the source file name
  * @param line: assert_param error line source number
  * @retval : None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif



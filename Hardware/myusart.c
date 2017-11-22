#include "sys.h"
#include "stm32f10x.h"
#include "stm32f10x_tim.h"


unsigned char RXBuff[60],TXBuff[60];
unsigned char timeout=1,rec_bit=0;
volatile unsigned char count=0;

void USART1_ini(unsigned long rate)
{
	
	
	USART_InitTypeDef ud;
	USART_ClockInitTypeDef uc;
	NVIC_InitTypeDef ni;
	GPIO_InitTypeDef gp;
	
	RCC_PCLK2Config(RCC_HCLK_Div1);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_USART1,ENABLE);
	
	gp.GPIO_Pin = GPIO_Pin_9;
	gp.GPIO_Mode = GPIO_Mode_AF_PP;
	gp.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&gp);
	
	gp.GPIO_Pin = GPIO_Pin_10;
	gp.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA,&gp);
	
	ni.NVIC_IRQChannel = USART1_IRQn;
	ni.NVIC_IRQChannelPreemptionPriority = 0;
	ni.NVIC_IRQChannelSubPriority  = 0;
	ni.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&ni);
	
	
	
	ud.USART_BaudRate = rate;
	ud.USART_WordLength = USART_WordLength_8b;
	ud.USART_StopBits = USART_StopBits_1;
	ud.USART_Parity = USART_Parity_No;
	ud.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	ud.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	
	uc.USART_Clock = USART_Clock_Disable;
	uc.USART_CPOL = USART_CPOL_Low;
	uc.USART_CPHA = USART_CPHA_2Edge;
	uc.USART_LastBit= USART_LastBit_Disable;
	
	USART_Init(USART1,&ud);
	USART_ClockInit(USART1,&uc);
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE); 
  USART_Cmd(USART1,ENABLE);
	
}


void USART1_Send_Char(char c)
{
	USART_SendData(USART1,c);
	while(USART_GetFlagStatus(USART1,USART_FLAG_TXE) == RESET);
}

void USART1_Send_String(char *s)
{
	while(*s)
	{
		USART1_Send_Char(*s);
		s++;
	}
}

void USART1_Send_Chars(unsigned char *p,u8 n)
{
	while(n--)
	{
		USART1_Send_Char(*p);
		p++;
	}
}


void USART1_IRQHandler()
{
	if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
	{
		
		if(timeout == 1)
		{
			
			rec_bit = 1;
			count=0;
			RXBuff[count++] = USART_ReceiveData(USART1);	
		}
		else
		{
			RXBuff[count++] = USART_ReceiveData(USART1);
		}
		timeout = 0;
		TIM_SetCounter(TIM2,0);
		
		
		
	  
		
		USART_ClearITPendingBit(USART1,USART_IT_RXNE);
	}
}



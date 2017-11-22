#include "sys.h"
#include "mytim2.h"
#include "stm32f10x_tim.h"

#include "myusart.h"

extern u8 haveread;

void Tim2_ini(int ms)
{
	NVIC_InitTypeDef NVIC_InitStructure; 
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);  
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;      
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;  
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  
	NVIC_Init(&NVIC_InitStructure); 
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);   
  
    TIM_DeInit(TIM2);  
  
    TIM_TimeBaseStructure.TIM_Period= (ms<<1) -1;  //??????????  
    TIM_TimeBaseStructure.TIM_Prescaler=(36000-1);         //??????  
    TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;  //????  
    TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //??????  
    TIM_TimeBaseInit(TIM2,&TIM_TimeBaseStructure);  
    TIM_ClearFlag(TIM2,TIM_FLAG_Update);               //????????  
    TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);  
    TIM_Cmd(TIM2,ENABLE);      
}

void TIM2_IRQHandler()
{
	
	if(TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
	{
			timeout = 1;
			if(rec_bit == 1)
			{
				RXBuff[count] = 0;
				rec_bit=0;
				haveread=0;
			}
	}
	
	TIM_ClearITPendingBit(TIM2 , TIM_FLAG_Update);
}


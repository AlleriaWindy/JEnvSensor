#include "LED.h"

static unsigned char led_flag = 0;

void LED_Init(void)
{
	
	GPIO_InitTypeDef gp;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	gp.GPIO_Mode = GPIO_Mode_Out_PP;
	gp.GPIO_Speed = GPIO_Speed_2MHz;
	gp.GPIO_Pin = GPIO_Pin_8;
	GPIO_Init(GPIOA,&gp);
	
	GPIO_SetBits(GPIOA,GPIO_Pin_8);
}

__inline void LED_On(void)
{
	GPIO_ResetBits(GPIOA,GPIO_Pin_8);
	led_flag = 1;
}

__inline void LED_Off(void)
{
	GPIO_SetBits(GPIOA,GPIO_Pin_8);
	led_flag = 0;
}

void LED_Toggle(void)
{
	if(led_flag)
		LED_Off();
	else
		LED_On();
}






















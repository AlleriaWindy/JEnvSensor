#include "relays.h"
#include "sys.h"

void Relays_Ini()
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	 
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	    
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_7;			    
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 	 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;	 
  GPIO_Init(GPIOB, &GPIO_InitStructure);
}

void Set_Relay(unsigned char rl,unsigned char State)
{
	PBout((6+rl)) = State;
}


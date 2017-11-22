#include "sensor.h"
#include "sys.h"



void Smoke_Ini()
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	 
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	    
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;			    
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 	 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;	 
  GPIO_Init(GPIOA, &GPIO_InitStructure);			     
}

void Fire_Ini()
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	 
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	    
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;			    
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 	 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;	 
  GPIO_Init(GPIOA, &GPIO_InitStructure);	
}

void Sensor_Ini()
{
	Smoke_Ini();
	Fire_Ini();
}

unsigned char Read_Smoke()
{
	unsigned char rt = GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_6);
	return !rt;
}

unsigned char Read_Fire()
{
	unsigned char rt = GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_7);
	return !rt;
}


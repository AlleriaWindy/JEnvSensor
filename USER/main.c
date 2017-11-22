#include "stm32f10x.h"
#include "sys.h"
#include "delay.h"
#include "stm32f10x_usart.h"

#include "myusart.h"
#include "mytim2.h"
#include "modbus.h"
#include "LED.h"
#include "oled.h"
#include "dht11.h"
#include "bh1750.h"
#include "BMP180.h"

#define TEMP_MSER 1  //1=Use BMP180  0=Use DHT22

extern u8 haveread; 
int led_time = 0,sensor_time = 0;
int bright_time = 0,press_time = 0;
u8 bh1750_flag = 0;//0->Resolute OK , 1->Resoluting
u8 bmp180_flag = 0;//0=OK 1=Wait for UT 2=Wait for UP

u32 bmp_press = 0;

void Refresh_Sensor(void);
void Refresh_Display(void);
void Disp_Title(void);


 int main(void)
 {	
	 
	 
	 SystemInit();
	 NVIC_Configuration();
	 delay_init();	 
	 USART1_ini(9600);
	 Tim2_ini(3);
	 LED_Init();
	 oled_ini(100,0);
	 DHT11_Init();
	 BH1750_Init();
	 
	 BMP180_Calibration();

	Disp_Title();
  while(1)
	{
		
		if(timeout == 1 && haveread == 0)
		{
			Function_Modbus();
			haveread = 1;
		}
		
		if(led_time++ >= 100)
		{
			LED_Toggle();
			led_time = 0;			
		}
		if(bmp180_flag == 0 && press_time++ >= 100)
		{
			press_time = 0;
			BMP180_BeginGetUT();
			bmp180_flag = 1;
		}
		if(bmp180_flag == 1 && press_time++ >= 5)
		{
			press_time = 0;
			BMP180_EndGetUT();
			BMP180_BeginGetUP();
		  s16 tmm = BMP180_CalcTemp();
#if TEMP_MSER==1
			Reg_TempH = tmm / 0xFF;
			Reg_TempL = tmm % 0xFF;
#endif
			bmp180_flag = 2;
		}
		if(bmp180_flag == 2 && press_time++ >= 5)
		{
			press_time = 0;
			BMP180_EndGetUP();
			bmp_press = BMP180_CalcPress();
			Reg_AirH = bmp_press / 0xFFFF;
			Reg_AirM = bmp_press / 0xFF % 0xFF;
			Reg_AirL = bmp_press % 0xFF;
			bmp180_flag = 0;
		}
		
		
		if(bh1750_flag == 0 && bright_time++>=50)
		{
			BH1750_StartResolutionH();
			bh1750_flag = 1;
			bright_time = 0;
		}
		if(bh1750_flag == 1 && bright_time++>=18)
		{
			u8 rdbuf[2];
			BH1750_ReadBrightRaw(rdbuf);
			Reg_LightH = rdbuf[0];
			Reg_LightL = rdbuf[1];
		}
		
		if(sensor_time++ >= 65)
		{
			sensor_time = 0;
			Refresh_Sensor();
			Refresh_Display();
		}
		
		
		delay_ms(10);
		
	}
 }
 
void Disp_Title(void)
{
	oled_8x16_print(0,0,"Temp:N/A");
	oled_8x16_print(0,2,"Hr:N/A");
	oled_8x16_print(0,4,"Bright:N/A");
	oled_8x16_print(0,6,"AP:N/A");
}	
 
 
void Refresh_Sensor(void)
{
		u8 ttt[4];
		DHT11_Read_Data(ttt);
		Reg_RHH = ttt[0];
		Reg_RHL = ttt[1];
#if TEMP_MSER==0
		Reg_TempH = ttt[2];
		Reg_TempL = ttt[3];
#endif
}

void Refresh_Display(void)
{
	s32 rrr = Reg_RHH*256+Reg_RHL;
	s32 ttt = Reg_TempH*256+Reg_TempL;
	oled_8x16_print(40,0,"     ");	
	oled_DispDouble(40,0,ttt/10,ttt%10,2);
	oled_8x16_print(Oled_Curr_X+8,0,"D.C.");
	oled_8x16_print(24,2,"     ");
	oled_DispDouble(24,2,rrr/10,rrr%10,2);
	oled_8x16_print(Oled_Curr_X+8,2,"%");
	oled_8x16_print(56,4,"       ");
	u16 light_disp = (u16)((float)((Reg_LightH<<8)+Reg_LightL)/1.2);
	oled_DispInt(56,4,light_disp,2);
	oled_8x16_print(Oled_Curr_X+8,4,"Lx");
	oled_8x16_print(24,6,"        ");
	oled_DispInt(24,6,bmp_press,2);
	oled_8x16_print(Oled_Curr_X+8,6,"Pa");
}












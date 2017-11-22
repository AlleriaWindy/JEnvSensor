#ifndef __OLED_H
#define __OLED_H 

#include "stm32f10x.h"
#include "stm32f10x_spi.h"

/*******************/
#define X_WIDTH 128
#define Y_WIDTH 64
/*******************/

/*
*PB0 ------> RST
*PC1 ------> C/S
*PB1 ------> D/C
*PA7 ------> DIN
*PA5 ------> SCK
*/

/*******************/
#define RST_L GPIO_ResetBits(GPIOB,GPIO_Pin_0)
#define RST_H GPIO_SetBits(GPIOB,GPIO_Pin_0)

#define CS_L GPIO_ResetBits(GPIOC,GPIO_Pin_1)
#define CS_H GPIO_SetBits(GPIOC,GPIO_Pin_1)

#define DC_L GPIO_ResetBits(GPIOB,GPIO_Pin_1)
#define DC_H GPIO_SetBits(GPIOB,GPIO_Pin_1)

#define DIN_L GPIO_ResetBits(GPIOA,GPIO_Pin_7)
#define DIN_H GPIO_SetBits(GPIOA,GPIO_Pin_7)

#define SCK_L GPIO_ResetBits(GPIOA,GPIO_Pin_5)
#define SCK_H GPIO_SetBits(GPIOA,GPIO_Pin_5)
/*******************/

/*******************************/
void oled_big_print(u8 x,u8 y,char p[]);
void oled_8x16_print(u8 x,u8 y,char p[]);
void oled_6x8_print(u8 x,u8 y,char p[]);
void oled_draw_pic(u8 x,u8 y,u8 w,u8 h,const unsigned char n[]);
void LCD_P6x8Str(unsigned char x,unsigned char y,unsigned char ch[]);
void LCD_P8x16Str(unsigned char x,unsigned char y,unsigned char ch[]);
void LCD_P44x16Ch(unsigned char x,unsigned char y,unsigned char n);
void OLED_DrawPoint(u8 x,u8 y,u8 t);
void OLED_Fill(u8 x1,u8 y1,u8 x2,u8 y2,u8 dot);
void oled_dat(u8);
void oled_cmd(u8);
void oled_ini(u8,u8);
void LCD_Fill(unsigned char);
void LCD_P6x8Str(unsigned char x,unsigned char y,unsigned char ch[]);
/*******************************/


void oled_DispInt(u8 x,u8 y,s32 num,u8 fontsize);
void oled_DispDouble(u8 x,u8 y,s32 high,u32 low,u8 fontsize);	

extern u8 Oled_Curr_X,Oled_Curr_Y;
#endif

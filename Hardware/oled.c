
#include "oled.h"
#include "oled_font.h"
//#include "FD24x32.c"
/******************/
unsigned char  OLED_GRAM[128][8];
u8 Oled_Curr_X=0,Oled_Curr_Y=0;
/*********************************************/


void LCD_DLY_ms(unsigned int ms)
{                         
  unsigned int a;
  while(ms)
  {
    a=5000;
    while(a--);
    ms--;
  }
  return;
} 

void oled_ini_pins()
{
	GPIO_InitTypeDef a;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	a.GPIO_Mode = GPIO_Mode_Out_PP;
	a.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_7;
	a.GPIO_Speed = GPIO_Speed_10MHz;
	
	GPIO_Init(GPIOA,&a);
	
	a.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
	GPIO_Init(GPIOB,&a);
	
}

void oled_cmd(u8 cmd)
{
	u8 i = 8;
	CS_L;
	DC_L;
	SCK_L;
	 while(i--)
  {
    if(cmd&0x80)
    {   
      DIN_H;
    }
    else
    {
      DIN_L;
    }
    SCK_H;

    SCK_L;    
    cmd<<=1;
    ;   
  }
	CS_H;
}

void oled_dat(u8 dat)
{
	u8 i=8;
	CS_L;
	DC_H;
	SCK_L;
	 while(i--)
  {
    if(dat&0x80)
    {   
      DIN_H;
    }
    else
    {
      DIN_L;
    }
    SCK_H;

    SCK_L;    
    dat<<=1;
    ;   
  }
	CS_H;
	
}
void LCD_Fill(unsigned char bmp_dat)
{
	unsigned char y,x;
	
	for(y=0;y<8;y++)
	{
		oled_cmd(0xb0+y);
		oled_cmd(0x01);
		oled_cmd(0x10);
		for(x=0;x<X_WIDTH;x++)
			oled_dat(bmp_dat);
	}
}
void LCD_Set_Pos(unsigned char x, unsigned char y)
{ 
  oled_cmd(0xb0+y);
  oled_cmd(((x&0xf0)>>4)|0x10);
  oled_cmd((x&0x0f)|0x01); 
} 
void oled_ini(u8 bright,u8 reverse)
{
	oled_ini_pins();
	
	SCK_H;
        CS_L;		
	RST_L;
        
	LCD_DLY_ms(50);
	RST_H;
	
	oled_cmd(0xae);//--turn off oled panel
  oled_cmd(0x00);//---set low column address
  oled_cmd(0x10);//---set high column address
  oled_cmd(0x40);//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
  oled_cmd(0x81);//--set contrast control register
  oled_cmd(bright); // Set SEG Output Current Brightness
	if(reverse)
	{
		oled_cmd(0xa1);//--Set SEG/Column Mapping     0xa0???? 0xa1??
		oled_cmd(0xc8);//Set COM/Row Scan Direction   0xc0???? 0xc8??
	}
	else
	{
		oled_cmd(0xa0);
		oled_cmd(0xc0);
  }
	
  oled_cmd(0xa6);//--set normal display
  oled_cmd(0xa8);//--set multiplex ratio(1 to 64)
  oled_cmd(0x3f);//--1/64 duty
  oled_cmd(0xd3);//-set display offset	Shift Mapping RAM Counter (0x00~0x3F)
  oled_cmd(0x00);//-not offset
  oled_cmd(0xd5);//--set display clock divide ratio/oscillator frequency
  oled_cmd(0x80);//--set divide ratio, Set Clock as 100 Frames/Sec
  oled_cmd(0xd9);//--set pre-charge period
  oled_cmd(0xf1);//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
  oled_cmd(0xda);//--set com pins hardware configuration
  oled_cmd(0x12);
  oled_cmd(0xdb);//--set vcomh
  oled_cmd(0x40);//Set VCOM Deselect Level
  oled_cmd(0x20);//-Set Page Addressing Mode (0x00/0x01/0x02)
  oled_cmd(0x02);//
  oled_cmd(0x8d);//--set Charge Pump enable/disable
  oled_cmd(0x14);//--set(0x10) disable
  oled_cmd(0xa4);// Disable Entire Display On (0xa4/0xa5)
  oled_cmd(0xa6);// Disable Inverse Display On (0xa6/a7) 
  oled_cmd(0xaf);//--turn on oled panel
  LCD_Fill(0x00);  //????
  LCD_Set_Pos(0,0); 
	
}
void OLED_Refresh_Gram(void)
{
	u8 i,n;		    
	for(i=0;i<8;i++)  
	{  
		oled_cmd(0xb0+i);    //?????(0~7)
		oled_cmd(0x00);      //??????—????
		oled_cmd(0x10);      //??????—????   
		for(n=0;n<128;n++)oled_dat(OLED_GRAM[n][i]); 
	}   
}

//?? 
//x:0~127
//y:0~63
//t:1 ?? 0,??				   
void OLED_DrawPoint(u8 x,u8 y,u8 t)
{
	u8 pos,bx,temp=0;
	if(x>127||y>63)return;//?????.
	pos=7-y/8;
	bx=y%8;
	temp=1<<(7-bx);
	if(t)OLED_GRAM[x][pos]|=temp;
	else OLED_GRAM[x][pos]&=~temp;	    
}
//x1,y1,x2,y2 ?????????
//??x1<=x2;y1<=y2 0<=x1<=127 0<=y1<=63	 	 
//dot:0,??;1,??	  
void OLED_Fill(u8 x1,u8 y1,u8 x2,u8 y2,u8 dot)  
{  
	u8 x,y;  
	for(x=x1;x<=x2;x++)
	{
		for(y=y1;y<=y2;y++)OLED_DrawPoint(x,y,dot);
	}													    
	OLED_Refresh_Gram();//????
}



void oled_draw_pic(u8 x,u8 y,u8 w,u8 h,const unsigned char n[])
{
	u8 a,b;
	int addr=0;
	for(a=0;a<(h/8);a++)
	{
		LCD_Set_Pos(x,y+a);
		for(b=0;b<w;b++)
		{
			
			oled_dat(n[addr]);
			addr++;
		}
	}
	Oled_Curr_X = x,
	Oled_Curr_Y = y;
	//OLED_Refresh_Gram();
}
void oled_6x8_print(u8 x,u8 y,char p[])
{
	u8 i=0;
	while(p[i]!='\0')
	{
		oled_draw_pic(x+(i*6),y,6,8,F6x8[p[i]-32]);
		i++;
	}
}
void oled_8x16_print(u8 x,u8 y,char p[])
{
	u8 i=0;
	while(p[i]!='\0')
	{
		oled_draw_pic(x+(i*8),y,8,16,F8X16[p[i]-32]);
		i++;
	}
}
void oled_big_print(u8 x,u8 y,char p[])
{
	u8 i=0;
	while(p[i]!='\0')
	{
		switch(p[i])
		{
			case ':':oled_draw_pic(x+(i*16),y,16,32,F_16x32_Sig[0]);break;
			case '.':oled_draw_pic(x+(i*16),y,16,32,F_16x32_Sig[1]);break;
			case '-':oled_draw_pic(x+(i*16),y,16,32,F_16x32_Sig[2]);break;
			case '+':oled_draw_pic(x+(i*16),y,16,32,F_16x32_Sig[3]);break;
			default:oled_draw_pic(x+(i*16),y,16,32,F_16x32[p[i]-'0']);break;
		}
		i++;
	}
}

static s32 Pow10(u8 n)
{
	s32 tmp = 1;
	u8 i=0;
	for(;i<n;i++)
		tmp *= 10;
	return tmp;
}

static u8 Int2String(s32 num,char * buff,u8 len)
{
	u8 i = 0,zero_flag = 1;
	
	if(num == 0)
	{
		buff[i++]='0';
		if(len > 1)
		{
			for(int j =0;j<len-1;j++)
				buff[i++] = '0';
		}
		buff[i] = 0;
		return i;
	}
	
	if(num < 0)
	{
		num = -num;
		buff[i++] = '-';
	}
	
	if(len == 0)
	{
		signed char j=9;
		for(;j>=0;j--)
		{
			s32 temp = num / Pow10(j);
			if(zero_flag && temp == 0)
				continue;
			zero_flag = 0;
			temp %= 10;
			buff[i++] = temp + '0';
		}
		buff[i] = 0;
	}
	else
	{
		signed char j=len-1;
		for(;j>=0;j--)
		{
			s32 temp = num / Pow10(j);
			temp %= 10;
			buff[i++] = temp + '0';
		}
		buff[i] = 0;
	}
	return i;
}

/*static u8 CalcSNum(double num)
{
	int i = 0;
	for(;i<100;i++)
	{
		if(num - (s32)num <= 0.0000000000001)
			break;
		num *= 10;
	}
	return i;
}

static u8 Double2String(double num,char *buff,u8 slen)
{
	u8 i = 0;
	s32 high = (s32)num;
	if(slen == 0)
		slen = CalcSNum(num - (s32)num);
	s32 low = (num - (s32)num) * Pow10(slen);
	i = Int2String(high,buff,0);
	buff[++i]='.';
	Int2String(low,buff+i+1,0);
	
}*/

void oled_DispInt(u8 x,u8 y,s32 num,u8 fontsize)
{
	char tmp[11];
	Int2String(num,tmp,0);
	switch(fontsize)
	{
		case 1:oled_6x8_print(x,y,tmp);break;
		case 2:oled_8x16_print(x,y,tmp);break;
		case 3:oled_big_print(x,y,tmp);break;
		default: oled_8x16_print(x,y,tmp);break;
	}
	
}


void oled_DispDouble(u8 x,u8 y,s32 high,u32 low,u8 fontsize)
{
	char tmp[22];
	u8 i = 0;
	i = Int2String(high,tmp,0);
	tmp[i++] = '.';
	Int2String(low,tmp+i,0);
	switch(fontsize)
	{
		case 1:oled_6x8_print(x,y,tmp);break;
		case 2:oled_8x16_print(x,y,tmp);break;
		case 3:oled_big_print(x,y,tmp);break;
		default: oled_8x16_print(x,y,tmp);break;
	}
	
}












#include "BMP180.h"
#include "myiic.h"

BMP_Reg bmp_reg;

static s32 bmp_UT = 0;
static u32 bmp_UP = 0;

void BMP180_Init(void)
{
	IIC_Init();
}

u8 BMP180_ReadByte(u8 addr)
{
	IIC_Start();
	IIC_Send_Byte(BMP_Addr);
	IIC_Wait_Ack();
	IIC_Send_Byte(addr);
	IIC_Wait_Ack();
	IIC_Stop();
	
	IIC_Start();
	IIC_Send_Byte(BMP_Addr + 1);
	IIC_Wait_Ack();
	u8 rt = IIC_Read_Byte(0);
	IIC_Stop();
	
	return rt;
}

u16 BMP180_ReadUInt(u8 addr)
{
	IIC_Start();
	IIC_Send_Byte(BMP_Addr);
	IIC_Wait_Ack();
	IIC_Send_Byte(addr);
	IIC_Wait_Ack();
	IIC_Stop();
	
	IIC_Start();
	IIC_Send_Byte(BMP_Addr + 1);
	IIC_Wait_Ack();
	u8 high = IIC_Read_Byte(1);
	u8 low = IIC_Read_Byte(0);
	IIC_Stop();
	
	return (high << 8) + low;
	
}

s16 BMP180_ReadInt(u8 addr)
{
	u16 rt = BMP180_ReadUInt(addr);
	return (s16)rt;
}

void BMP180_Write(u8 addr,u8 data)
{
	IIC_Start();
	IIC_Send_Byte(BMP_Addr);
	IIC_Wait_Ack();
	IIC_Send_Byte(addr);
	IIC_Wait_Ack();
	IIC_Send_Byte(data);
	IIC_Wait_Ack();
	IIC_Stop();
}

void BMP180_Calibration(void)
{
	bmp_reg.AC1 = BMP180_ReadInt(0xAA);
	bmp_reg.AC2 = BMP180_ReadInt(0xAC);
	bmp_reg.AC3 = BMP180_ReadInt(0xAE);
	bmp_reg.AC4 = BMP180_ReadUInt(0xB0);
	bmp_reg.AC5 = BMP180_ReadUInt(0xB2);
	bmp_reg.AC6 = BMP180_ReadUInt(0xB4);
	bmp_reg.B1 = BMP180_ReadInt(0xB6);
	bmp_reg.B2 = BMP180_ReadInt(0xB8);
	bmp_reg.MB = BMP180_ReadInt(0xBA);
	bmp_reg.MC = BMP180_ReadInt(0xBC);
	bmp_reg.MD = BMP180_ReadInt(0xBE);
}

void BMP180_BeginGetUT(void)
{
	BMP180_Write(0xF4,0x2E);
}

void BMP180_EndGetUT(void)
{
	bmp_UT = BMP180_ReadInt(0xF6);
}

void BMP180_BeginGetUP(void)
{
	BMP180_Write(0xF4,0x34 + (BMP_OSS << 6));
}

void BMP180_EndGetUP(void)
{
	u8 msb,lsb,xlsb;
	msb = BMP180_ReadByte(0xF6);
  lsb = BMP180_ReadByte(0xF7);
  xlsb = BMP180_ReadByte(0xF8);
	
	bmp_UP = (((unsigned long) msb << 16) | ((unsigned long) lsb << 8) | (unsigned long) xlsb) >> (8-BMP_OSS);

}

/***************************************
long x1, x2;

  x1 = (((long)ut - (long)ac6)*(long)ac5) >> 15;
  x2 = ((long)mc << 11)/(x1 + md);
  b5 = x1 + x2;

  float temp = ((b5 + 8)>>4);
  temp = temp /10;

  return temp;

***************************************/

s16 BMP180_CalcTemp(void)
{
	s32 x1 = ((bmp_UT - (s32)bmp_reg.AC6)*(s32)bmp_reg.AC5) >> 15;
	s32 x2 = ((s32)bmp_reg.MC << 11)/(x1 + bmp_reg.MD);
	bmp_reg.B5 = x1 + x2;
	return ((bmp_reg.B5 + 8)>>4);
}

/***************************************
long x1, x2, x3, b3, b6, p;
  unsigned long b4, b7;
 b6 = b5 - 4000;
  // Calculate B3
  x1 = (b2 * (b6 * b6)>>12)>>11;
  x2 = (ac2 * b6)>>11;
  x3 = x1 + x2;
  b3 = (((((long)ac1)*4 + x3)<<OSS) + 2)>>2;

  // Calculate B4
  x1 = (ac3 * b6)>>13;
  x2 = (b1 * ((b6 * b6)>>12))>>16;
  x3 = ((x1 + x2) + 2)>>2;
  b4 = (ac4 * (unsigned long)(x3 + 32768))>>15;

  b7 = ((unsigned long)(up - b3) * (50000>>OSS));
  if (b7 < 0x80000000)
    p = (b7<<1)/b4;
  else
    p = (b7/b4)<<1;

  x1 = (p>>8) * (p>>8);
  x1 = (x1 * 3038)>>16;
  x2 = (-7357 * p)>>16;
  p += (x1 + x2 + 3791)>>4;

  long temp = p;
  return temp;
***************************************/

u32 BMP180_CalcPress(void)
{
	s32 p;
	s32 b6 = bmp_reg.B5 - 4000;
	s32 x1 = (bmp_reg.B2 * (b6 * b6)>>12)>>11;
  s32 x2 = (bmp_reg.AC2 * b6)>>11;
  s32 x3 = x1 + x2;
  s32 b3 = (((((s32)bmp_reg.AC1)*4 + x3)<<BMP_OSS) + 2)>>2;
	
	x1 = (bmp_reg.AC3 * b6)>>13;
  x2 = (bmp_reg.B1 * ((b6 * b6)>>12))>>16;
  x3 = ((x1 + x2) + 2)>>2;
  u32 b4 = (bmp_reg.AC4 * (unsigned long)(x3 + 32768))>>15;
	
	u32 b7 = ((unsigned long)(bmp_UP - b3) * (50000>>BMP_OSS));
  if (b7 < 0x80000000)
    p = (b7<<1)/b4;
  else
    p = (b7/b4)<<1;
	x1 = (p>>8) * (p>>8);
  x1 = (x1 * 3038)>>16;
  x2 = (-7357 * p)>>16;
  p += (x1 + x2 + 3791)>>4;
	
	return p;
	
}




















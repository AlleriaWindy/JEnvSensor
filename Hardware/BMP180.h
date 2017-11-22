#ifndef __BMP180_H__
#define __BMP180_H__

#include "sys.h"

#define BMP_Addr 0xEE

#define BMP_OSS 0

typedef struct BMP_Reg
{
	s16 AC1;
	s16 AC2;
	s16 AC3;
	u16 AC4;
	u16 AC5;
	u16 AC6;
	s16 B1;
	s16 B2;
	s16 MB;
	s16 MC;
	s16 MD;
	s32 B5;
	
}BMP_Reg;

//extern BMP_Reg bmp_reg;

/****************************/

void BMP180_Calibration(void);
void BMP180_BeginGetUT(void);
void BMP180_EndGetUT(void);
void BMP180_BeginGetUP(void);
void BMP180_EndGetUP(void);
s16 BMP180_CalcTemp(void);
u32 BMP180_CalcPress(void);

#endif


#ifndef __BH1750_H__
#define __BH1750_H__

#include "sys.h"
#include "myiic.h"

/*********************
PB6 ---> SCL
PB7 ---> SDA
**********************/


#define BH1750_Addr 0x46

#define BH1750_Command_PowerOn 0x01
#define BH1750_Command_ResH 0x10


void BH1750_Send(u8 data);
u16 BH1750_ReadBright(void);
void BH1750_Init(void);
void BH1750_PowerOn(void);
void BH1750_StartResolutionH(void);
void BH1750_ReadBrightRaw(u8 buff[2]);

#endif


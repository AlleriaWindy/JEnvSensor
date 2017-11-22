#include "bh1750.h"

void BH1750_Send(u8 data)
{
	IIC_Start();
	IIC_Send_Byte(BH1750_Addr);
	IIC_Wait_Ack();
	IIC_Send_Byte(data);
	IIC_Wait_Ack();
	IIC_Stop();
}

u16 BH1750_ReadBright(void)
{
	u8 low=0,high=0;
	IIC_Start();
	IIC_Send_Byte(BH1750_Addr + 1);
	IIC_Wait_Ack();
	high = IIC_Read_Byte(1);
	low = IIC_Read_Byte(0);
	IIC_Stop();
	u16 rt = (high<<8) + low;
	rt = (u16)((float)rt / 1.2);
	return rt;
}

void BH1750_ReadBrightRaw(u8 buff[2])
{

	IIC_Start();
	IIC_Send_Byte(BH1750_Addr + 1);
	IIC_Wait_Ack();
	buff[0] = IIC_Read_Byte(1);
	buff[1] = IIC_Read_Byte(0);
	IIC_Stop();
	
}


void BH1750_Init(void)
{
	IIC_Init();
	BH1750_Send(BH1750_Command_PowerOn);

}

void BH1750_PowerOn(void)
{
	BH1750_Send(BH1750_Command_PowerOn);
}

void BH1750_StartResolutionH(void)
{
	BH1750_Send(BH1750_Command_ResH);
}










#ifndef __MODBUS_H_
#define __MODBUS_H_


#define PROJ_ID 0x05
#define Reg_LEN 10

extern unsigned char reg[];

#define Reg_Addr reg[0]
#define Reg_TempH reg[1]
#define Reg_TempL reg[2]
#define Reg_RHH reg[3]
#define Reg_RHL reg[4]
#define Reg_AirH reg[5]
#define Reg_AirM reg[6]
#define Reg_AirL reg[7]
#define Reg_LightH reg[8]
#define Reg_LightL reg[9]


unsigned short int Get_CRC16( unsigned char * pucFrame, unsigned short int usLen );
unsigned char Check_CRC(void);
void Function_Modbus(void);
void Send_Modbus(unsigned char *,unsigned char,unsigned char);
void Handle_0x02(void);
void Handle_0x04(void);

#endif

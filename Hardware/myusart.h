#ifndef __MYUSART_H_
#define __MYUSART_H_

void USART1_ini(unsigned long);
void USART1_Send_Char(char);
void USART1_Send_String(char*);
void USART1_Send_Chars(unsigned char *,unsigned char);

extern unsigned char RXBuff[];
extern unsigned char TXBuff[];
extern unsigned char timeout,rec_bit,count;



#endif

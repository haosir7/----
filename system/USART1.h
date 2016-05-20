#ifndef _USART_H_
#define _USART_H_
#include "stm32f10x.h"

#define BYTE0(dwTemp)       (*(char *)(&dwTemp))
#define BYTE1(dwTemp)       (*((char *)(&dwTemp) + 1))
#define BYTE2(dwTemp)       (*((char *)(&dwTemp) + 2))
#define BYTE3(dwTemp)       (*((char *)(&dwTemp) + 3))

struct PID {	float Kp; float Ki; float Kd; } ;

extern struct PID PID_main;
extern struct PID PID_101_pos;
extern struct PID PID_101_spe;

void USART1_IRQHandler(void);

void Uart1_Put_String(unsigned char *Str);
void Uart1_Put_Buf(unsigned char *DataToSend , u8 data_num);
void Uart1_Init(void);

void Data_Send_Sensor(void);

void Data_Receive_Anl(u8 *data_buf,u8 num);
void Data_Send_Check(u16 check);
#endif

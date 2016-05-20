#include "stm32f10x.h"
#include "PID.h"
#include "math.h"
#include "SysInit.h"
#include "CAN1.h"
#include "usart1.h" 

#define Moter_main_forward  GPIO_SetBits(GPIOB,GPIO_Pin_13);   GPIO_ResetBits(GPIOB,GPIO_Pin_12)
#define Moter_main_back     GPIO_ResetBits(GPIOB,GPIO_Pin_13); GPIO_SetBits(GPIOB,GPIO_Pin_12)
#define Moter_main_stop     GPIO_SetBits(GPIOB,GPIO_Pin_13);   GPIO_SetBits(GPIOB,GPIO_Pin_12)

#define Moter_101_forward   GPIO_SetBits(GPIOB,GPIO_Pin_14);   GPIO_ResetBits(GPIOB,GPIO_Pin_15)
#define Moter_101_back      GPIO_ResetBits(GPIOB,GPIO_Pin_14); GPIO_SetBits(GPIOB,GPIO_Pin_15)
#define Moter_101_stop      GPIO_SetBits(GPIOB,GPIO_Pin_14);   GPIO_SetBits(GPIOB,GPIO_Pin_15)

void LED_Flash(void);
void Motion_Control(void);



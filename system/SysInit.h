
#include "stm32f10x.h"

#define STOP    0
#define Forward 220       //70%×ªËÙ
#define Back    -220
#define Manual  0x00
#define Auto    0x01

extern int Motor_Target;
extern int Mode;

void RCC_Configuration(void);
void TIM2_EncoderMode(void);
void TIM3_EncoderMode(void);
void TIM1_InterruptMode(void);
void TIM4_PWMMode(void);
void USART1_Init(void);
void USART2_Init(void);
void LED_Init(void);
void USART1_Send(char *Data);
void USART2_Scope(float *Data);
void Dir_Config(void);
void ADC1_Configuration(void);
void Control_Input_Config(void);




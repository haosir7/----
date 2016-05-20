#ifndef _AI_H_
#define _AI_H_

#include "stm32f10x.h"

#define ADC1_DR_Address    ((u32)0x40012400+0x4c)

#define START_ADC PAout(6) = 0;\
									WWDG_SetCounter(0x7f);delay_ms(15);WWDG_SetCounter(0x7f);delay_ms(15);WWDG_SetCounter(0x7f);\
									ADC_Cmd(ADC1,ENABLE);delay_us(50);ADC_Cmd(ADC1,ENABLE);delay_us(50);WWDG_SetCounter(0x7f);
									
									

#define STOP_ADC  WWDG_SetCounter(0x7f);PAout(6) = 1;\
									ADC_Cmd(ADC1,DISABLE);delay_ms(15);WWDG_SetCounter(0x7f);
									
									
									

// 

void AI_init(void);

extern volatile uint16_t ADCConvertedValue[2];

#endif


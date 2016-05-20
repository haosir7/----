/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "delay.h"
#include "SysInit.h"
#include <stm32f10x.h>
#include "CAN1.h"
#include "AI.h"
#include "usart1.h" 

extern int16_t position[8];
int Motor_Target = 0; int Mode = Auto;
float motor1_cur = 0,motor2_cur = 0;
float mai = 0,inc = 0,E101 = 0;
void Get_current(void);
/***********************************************************************************************/
/*                                                                                             */
/*                                      ������                                                 */
/*                                                                                             */
/***********************************************************************************************/
int main(void)
{ 
	
	RCC_Configuration();	           //ϵͳʱ��
	delay_init();                   
	delay_ms(200);
	LED_Init();
	
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);//����jtag,����swd
	
	CAN1_GPIOConfig();
	CAN1_NVIC_Configuration(); 
	CAN1_Configuration();
	
	TIM2_EncoderMode();              //����������    ������ת��5950/min����Ȧ800������
	TIM3_EncoderMode();              //�ӻ�������
	TIM2->CNT = 0;
	TIM3->CNT = 0;
	TIM4_PWMMode();                  //2·PWM����
	Dir_Config();                    //������з�����ƶ�
	
	AI_init();
	Uart1_Init();
	Control_Input_Config();          //��������˿�  ǰ�������ˡ�ģʽ�л�
	
	TIM1_InterruptMode();            //ϵͳ��ʱˢ��	  4ms
	
  while (1)
  {
		delay_ms(10);
		Data_Send_Sensor();

		if( GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_11) )            //ǰ��
			{
				delay_ms(10);
				if( GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_11) )
					Motor_Target = Forward;
			}
			else if( GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_10) )     //����
			{
				delay_ms(10);
				if( GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_10) )
					Motor_Target = Back;
			}
			else 	if( GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_3) )     //ֹͣ
			{
				delay_ms(10);
				if( GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_3) ) 
				{ 
					Motor_Target = STOP;
				}
			}
				
		Get_current();      //�ɼ�����ֵ        
  }
}

void Get_current(void)
{
	int i;
	float motor1_temp = 0,motor2_temp = 0;
	float motor1_sum  = 0,motor2_sum  = 0;
	
	for(i=0;i<8;i++)
	{
		motor1_temp = ADCConvertedValue[0] /4095.0 * 39.6 ;     //PA2
		motor2_temp = ADCConvertedValue[1] /4095.0 * 39.6 ;     //PA3
		motor1_sum += motor1_temp;
		motor2_sum += motor2_temp;
		delay_ms(1);
	}
	
	motor1_cur = motor1_sum/8.0;
	motor2_cur = motor2_sum/8.0;
}



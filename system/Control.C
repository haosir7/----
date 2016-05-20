
#include "control.h"

struct PID_InitTypeDef PID_main_Speed = {4,0.1,0,0,0,0};      //Kp Ki Bias Last_bias Int_Bias Out
struct PID_InitTypeDef PID_101_Speed  = {4,0.1,0,0,0,0};      //Kp Ki Bias Last_bias Int_Bias Out
struct PID_InitTypeDef PID_101_Pos    = {0.06,0,0,0,0,0};       //Kp Ki Bias Last_bias Int_Bias Out

double Encoder_main = 0,Encoder_101 = 0;  
extern int16_t position[8];
u8 Data[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};

/****************************************************************************************
  * @函数描述：  定时器1定时中断
  * @入口参数：  无.
  * @返回值  :   无.
****************************************************************************************/
void TIM1_UP_IRQHandler(void) 
{	
	float Enc_main_inc = 0,Enc_101_inc = 0;
	static float ID_101_Pos_target = 0;
	static int transfer_cnt = 0,pos_loop_cnt = 0;
	
	if(TIM_GetITStatus(TIM1,TIM_IT_Update)==SET) 
	{ 
		/******************************  修改PID参数、运行PID运算  ************************/
//		PID_main_Speed.Kp = PID_main.Kp;
//		PID_main_Speed.Ki = PID_main.Ki;
//		PID_101_Speed.Kp = PID_101_spe.Kp;
//		PID_101_Speed.Ki = PID_101_spe.Ki;
//		PID_101_Pos.Kp = PID_101_pos.Kp;
//		PID_101_Pos.Ki = PID_101_pos.Ki;
		
		if (Mode == Auto)
		{
			Enc_main_inc = (short)TIM_GetCounter(TIM2); 
			Enc_101_inc  = (short)TIM_GetCounter(TIM3); 
			Encoder_main +=  Enc_main_inc;
			Encoder_101  +=  Enc_101_inc;
		}
		TIM2->CNT = 0;
		TIM3->CNT = 0;
		
		PID_Spe_Control(Enc_main_inc, &PID_main_Speed, Motor_Target);        //主站速度控制
		
		if(transfer_cnt >= 12)                                               //每48ms，主站位置发往从站
		 { 
				ID_101_Pos_target = Encoder_main;
				PID_101_Pos.Integral_Bias = 0;
				transfer_cnt = 0;
				CAN1_Send(0x100,Data);                                           //发往CAN上的从站
			} 
		else { transfer_cnt++; }  
		
		if(pos_loop_cnt >=3)                                                 //每12ms，从站101位置控制
			{ 
				PID_Pos_Control(Encoder_101, &PID_101_Pos, ID_101_Pos_target);     
				pos_loop_cnt = 0; 
			} 
		else { pos_loop_cnt++; }
		
		PID_Spe_Control(Enc_101_inc, &PID_101_Speed, PID_101_Pos.Out);        //从站101速度控制
		
		Motion_Control();

		LED_Flash();
		
		position[0] = PID_main_Speed.Bias;
		position[1] = PID_101_Speed.Bias;
		position[2] = Enc_main_inc;
		position[3] = Enc_101_inc;
		position[4] = Encoder_main;
		position[5] = Encoder_101;
	}

	TIM_ClearITPendingBit(TIM1, TIM_IT_Update);

} 

void Motion_Control(void) 
{
		if(PID_main_Speed.Out > 35)                       //主机方向控制
			{
				Moter_main_forward;
				TIM_SetCompare1(TIM4,PID_main_Speed.Out);
			}
		else if(PID_main_Speed.Out < -35)  
			{ 
				Moter_main_back;
				TIM_SetCompare1(TIM4,-PID_main_Speed.Out);
			} 
		else
			{
				Moter_main_stop;	
				TIM_SetCompare1(TIM4,0);
			}
			
		if(PID_101_Speed.Out > 35)                       //从机101放向控制
			{
				Moter_101_forward;
				TIM_SetCompare2(TIM4,PID_101_Speed.Out);
			}
		else if(PID_101_Speed.Out < -35)                                                        
			{
				Moter_101_back;
				TIM_SetCompare2(TIM4,-PID_101_Speed.Out);
			}		
		else
			{
				Moter_101_stop;
				TIM_SetCompare2(TIM4,0);
			}
}

void LED_Flash(void)
{
	
		static int CNT = 0;
	
		if( CNT <= 125 )
			{	
				GPIO_SetBits(GPIOC,GPIO_Pin_13);
				CNT++;
			}
		else if( CNT <= 250 )
			{
				GPIO_ResetBits(GPIOC,GPIO_Pin_13);
				CNT++;
			}
		else
			CNT=0;
}


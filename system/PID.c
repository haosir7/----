
#include "PID.h"

/***************************************************************************************
  * @函数描述：  速度控制
  * @入口参数：  Feedback----速度增量反馈，PID----待运算PID结构体，Target----给定
  * @返回值  :   无
****************************************************************************************/
void PID_Spe_Control(float Feedback,struct PID_InitTypeDef *PID,int Target)
{
	
	PID->Bias = Target - Feedback;
	
	PID->Out += PID->Kp * (PID->Bias - PID->Last_Bias) +	PID->Ki * PID->Bias;
	
	PID->Last_Bias = PID->Bias;

	if(PID->Out >= 9500) PID->Out = 9500;
	else  if(PID->Out <= -9500) PID->Out = -9500;
	
}


/***************************************************************************************
  * @函数描述：  位置控制
  * @入口参数：  Feedback----位置反馈，PID----待运算PID结构体，Target----给定
  * @返回值  :   无
****************************************************************************************/
void PID_Pos_Control (float Feedback,struct PID_InitTypeDef *PID,int Target)
{ 	
	
	 PID->Bias = Target - Feedback;  
	
	 PID->Integral_Bias += PID->Bias;	
	
	 PID->Out = PID->Kp * PID->Bias + PID->Ki * PID->Integral_Bias;
	
} 



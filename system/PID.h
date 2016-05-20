
#include "stm32f10x.h" 

struct PID_InitTypeDef      
{
	float Kp;               //Kp参数
	float Ki;               //Ki参数
	float Bias;             //当次误差
	float Last_Bias;
	float Integral_Bias;    //误差积分

	float Out;              //输出
};

void PID_Spe_Control(float Feedback,struct PID_InitTypeDef *PID,int Target);
void PID_Pos_Control (float Feedback,struct PID_InitTypeDef *PID,int Target);





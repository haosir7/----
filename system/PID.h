
#include "stm32f10x.h" 

struct PID_InitTypeDef      
{
	float Kp;               //Kp����
	float Ki;               //Ki����
	float Bias;             //�������
	float Last_Bias;
	float Integral_Bias;    //������

	float Out;              //���
};

void PID_Spe_Control(float Feedback,struct PID_InitTypeDef *PID,int Target);
void PID_Pos_Control (float Feedback,struct PID_InitTypeDef *PID,int Target);





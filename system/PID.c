
#include "PID.h"

/***************************************************************************************
  * @����������  �ٶȿ���
  * @��ڲ�����  Feedback----�ٶ�����������PID----������PID�ṹ�壬Target----����
  * @����ֵ  :   ��
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
  * @����������  λ�ÿ���
  * @��ڲ�����  Feedback----λ�÷�����PID----������PID�ṹ�壬Target----����
  * @����ֵ  :   ��
****************************************************************************************/
void PID_Pos_Control (float Feedback,struct PID_InitTypeDef *PID,int Target)
{ 	
	
	 PID->Bias = Target - Feedback;  
	
	 PID->Integral_Bias += PID->Bias;	
	
	 PID->Out = PID->Kp * PID->Bias + PID->Ki * PID->Integral_Bias;
	
} 



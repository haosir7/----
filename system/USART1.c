#include "usart1.h" 

u8 data_to_send[0xff];
u8 TxBuffer[0xff];

u8 count=0;
static u8 RxBuffer[50];
static u8 RxState = 0;

int16_t position[8] = {0,100,200,400,500,600,700,800};

struct PID PID_main;
struct PID PID_101_pos;
struct PID PID_101_spe;

u8 Data_Chec,Send_PID1,Send_PID3;

static void Uart1_Nvic_Conf(void)
{
			NVIC_InitTypeDef NVIC_InitStructure;

			/* Configure the NVIC Preemption Priority Bits */  
			NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);

			/* Enable the USART1 Interrupt */
			NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
			NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
			NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
			NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
			NVIC_Init(&NVIC_InitStructure);
}

 void Uart1_Gpio_Conf(void)
{ 	
			GPIO_InitTypeDef GPIO_InitStructure;
			//配置PA9作为USART1　Tx
	   	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
			GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_9;
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
			GPIO_Init(GPIOA , &GPIO_InitStructure);
			//配置PA10作为USART1　Rx
			GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_10;
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
			GPIO_Init(GPIOA , &GPIO_InitStructure);
	}

	void Uart1_Conf(void)
	{
			USART_InitTypeDef USART_InitStructure;
			USART_ClockInitTypeDef USART_ClockInitStruct;
			
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);           //开启USART1时钟
		
			//配置USART1
			USART_InitStructure.USART_BaudRate = 9600;                       //波特率可以通过地面站配置
			USART_InitStructure.USART_WordLength = USART_WordLength_8b;      //8位数据
			USART_InitStructure.USART_StopBits = USART_StopBits_1;      	   //在帧结尾传输1个停止位
			USART_InitStructure.USART_Parity = USART_Parity_No;              //禁用奇偶校验
			USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //硬件流控制失能
			USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;  //发送、接收使能
			//配置USART1时钟
			USART_ClockInitStruct.USART_Clock = USART_Clock_Disable; 				 //时钟低电平活动
			USART_ClockInitStruct.USART_CPOL = USART_CPOL_Low; 							 //SLCK引脚上时钟输出的极性->低电平
			USART_ClockInitStruct.USART_CPHA = USART_CPHA_2Edge;  					 //时钟第二个边沿进行数据捕获
			USART_ClockInitStruct.USART_LastBit = USART_LastBit_Disable;     //最后一位数据的时钟脉冲不从SCLK输出
			
			USART_Init(USART1, &USART_InitStructure);
			USART_ClockInit(USART1, &USART_ClockInitStruct);

			//使能USART1接收中断
			USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
			//使能USART1
			USART_Cmd(USART1, ENABLE); 
}

void Uart1_Init()
{
			Uart1_Gpio_Conf();
			Uart1_Nvic_Conf();
			Uart1_Conf();

}

/************************************************************
**  说明：以下为下位机发往上位机的函数
**  data_to_send：发送缓存区。
**  Uart1_Put_Buf(data_to_send,_cnt)：串口发送函数
**  _cnt为需要发送的字节数。
**  BYTE3，BYTE2，BYTE1，BYTE0:表示取某变量的高低字节，0最低，3最高。
*************************************************************/
void Uart1_Put_Buf(unsigned char *DataToSend , u8 data_num)
{     u8 i;
			for(i=0;i<data_num;i++)    
				{    
				 USART_SendData(USART1, DataToSend[i]);  
				 while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET); //等待数据发送完毕
				 
				}
}
//发送基本信息（姿态，锁定状态）


//发送传感器数据
void Data_Send_Sensor(void)
{
				u8 _cnt=0;
				u8 sum = 0;
				u8 i;
				
				data_to_send[_cnt++]=0xAA;
				data_to_send[_cnt++]=0xAA;
				data_to_send[_cnt++]=0x02;
				data_to_send[_cnt++]=0;
				data_to_send[_cnt++] = BYTE1(position[0]);  
				data_to_send[_cnt++] = BYTE0(position[0]);  
				data_to_send[_cnt++] = BYTE1(position[1]);
				data_to_send[_cnt++] = BYTE0(position[1]);
				data_to_send[_cnt++] = BYTE1(position[2]);
				data_to_send[_cnt++] = BYTE0(position[2]);
				data_to_send[_cnt++] = BYTE1(position[3]);
				data_to_send[_cnt++] = BYTE0(position[3]);
				data_to_send[_cnt++] = BYTE1(position[4]);
				data_to_send[_cnt++] = BYTE0(position[4]);
				data_to_send[_cnt++] = BYTE1(position[5]);
				data_to_send[_cnt++] = BYTE0(position[5]);
				data_to_send[_cnt++] = BYTE1(position[6]);
				data_to_send[_cnt++] = BYTE0(position[6]);
				data_to_send[_cnt++] = BYTE1(position[7]);
				data_to_send[_cnt++] = BYTE0(position[7]);
				data_to_send[_cnt++] = BYTE1(position[8]);
				data_to_send[_cnt++] = BYTE0(position[8]);
				
				data_to_send[3] = _cnt-4;

				for(i=0;i<_cnt;i++)
				sum += data_to_send[i];
				data_to_send[_cnt++] = sum;

				Uart1_Put_Buf(data_to_send,_cnt);
}


/************************************************************
**  说明：下位机解析上位机发送数据函数
*************************************************************/
void USART1_IRQHandler(void)
{      
		 if (USART_GetFlagStatus(USART1, USART_FLAG_ORE) != RESET)//溢出错误标志位
			{ 
				USART_ClearITPendingBit(USART1,USART_IT_RXNE);//清除中断标志位
				USART_ReceiveData(USART1);
			}
			if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //接收中断 (接收寄存器非空) 
			{
				u8 com_data =USART_ReceiveData(USART1);
				static u8 _data_len = 0,_data_cnt = 0;
				if(RxState==0&&com_data==0xAA)
				{
					RxState=1;
					RxBuffer[0]=com_data;
				}
				else if(RxState==1&&com_data==0xAF)
				{
					RxState=2;
					RxBuffer[1]=com_data;
				}
				else if(RxState==2&&com_data>0&&com_data<0XF1)
				{
					RxState=3;
					RxBuffer[2]=com_data;
				}
				else if(RxState==3&&com_data<50)
				{
					RxState = 4;
					RxBuffer[3]=com_data;
					_data_len = com_data;
					_data_cnt = 0;
				}
				else if(RxState==4&&_data_len>0)
				{
					_data_len--;
					RxBuffer[4+_data_cnt++]=com_data;
					if(_data_len==0)
					RxState = 5;
				}
				else if(RxState==5)
				{
					RxState = 0;
					RxBuffer[4+_data_cnt]=com_data;
					Data_Receive_Anl(RxBuffer,_data_cnt+5);
				}
				else
					RxState = 0;
			}
}

void Data_Receive_Anl(u8 *data_buf,u8 num)
{
			u8 sum = 0;
			u8 i;
			for(i=0;i<(num-1);i++)
				 sum += *(data_buf+i);
			if(!(sum==*(data_buf+num-1)))		return;		//判断sum
			if(!(*(data_buf)==0xAA && *(data_buf+1)==0xAF))		return;		//判断帧头
		/////////////////////////////////////////////////////////////////
			if(*(data_buf+2)==0X02)
			{
			if(*(data_buf+4)==0X01)
				{
		
				}
			}
			if(*(data_buf+2)==0X10)//PID1
			{	
					PID_main.Kp = (float)((vs16)(*(data_buf+4)<<8)|*(data_buf+5));
					PID_main.Ki   = (float)((vs16)(*(data_buf+6)<<8)|*(data_buf+7));
					PID_main.Kd   = (float)((vs16)(*(data_buf+8)<<8)|*(data_buf+9));
					PID_101_pos.Kp  = (float)((vs16)(*(data_buf+10)<<8)|*(data_buf+11));
					PID_101_pos.Ki  = (float)((vs16)(*(data_buf+12)<<8)|*(data_buf+13));
					PID_101_pos.Kd  = (float)((vs16)(*(data_buf+14)<<8)|*(data_buf+15));
					PID_101_spe.Kp = (float)((vs16)(*(data_buf+16)<<8)|*(data_buf+17));
					PID_101_spe.Ki = (float)((vs16)(*(data_buf+18)<<8)|*(data_buf+19));
					PID_101_spe.Kd = (float)((vs16)(*(data_buf+20)<<8)|*(data_buf+21));
		     	Data_Send_Check(sum);
			}
			if(*(data_buf+2)==0X12)//PID3
			{
//					PITCH_OUT_PID.P = (float)((vs16)(*(data_buf+4)<<8)|*(data_buf+5))/100;
//					PITCH_OUT_PID.I = (float)((vs16)(*(data_buf+6)<<8)|*(data_buf+7))/100;
//					PITCH_OUT_PID.D = (float)((vs16)(*(data_buf+8)<<8)|*(data_buf+9))/100;
					Data_Send_Check(sum);
			}
}

void Data_Send_Check(u16 check)
{	
				u8 sum = 0;
				u8 i;
				data_to_send[0]=0xAA;
				data_to_send[1]=0xAA;
				data_to_send[2]=0xF0;
				data_to_send[3]=3;
				data_to_send[4]=0xBA;
				
				data_to_send[5]=BYTE1(check);
				data_to_send[6]=BYTE0(check);
				
				for( i=0;i<7;i++)
				sum += data_to_send[i];
				data_to_send[7]=sum;
				Uart1_Put_Buf(data_to_send,8);
}


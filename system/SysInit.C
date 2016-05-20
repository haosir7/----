
#include "SysInit.h"
#include "stdio.h"

uint8_t USART1_Rx_Buf[1];
uint8_t para_rev[32];

/***************************************************************************************
  * @函数描述：  系统时钟初始化
  * @入口参数：  无.
  * @返回值  :   无.
****************************************************************************************/

void RCC_Configuration(void)
{
	RCC_DeInit();
	RCC_HSEConfig(RCC_HSE_ON);
	while(RCC_WaitForHSEStartUp()!=SUCCESS);
	RCC_HCLKConfig(RCC_SYSCLK_Div1);
	RCC_PCLK1Config(RCC_HCLK_Div2);
	RCC_PCLK2Config(RCC_HCLK_Div1);
	RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);   //倍频至72MHz
	RCC_PLLCmd(ENABLE);
	while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY)==RESET);
	RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
	while(RCC_GetSYSCLKSource()!= 0x08);
	
	/*--------------------------启动具体时钟--------------------------------*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	
}

/***************************************************************************************
  * @函数描述：  控制输入管脚初始化   PB3__自动模式， 自动模式：PB10__前进        PB11__后退   手动
                                                      手动模式：PB0__主站前进     PB1__主站后退
                                                                PB4__从站101前进  PB5__从站101后退
  * @入口参数：  无.
  * @返回值  :   无.
****************************************************************************************/
void Control_Input_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure; 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_10 | GPIO_Pin_11;        
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; 
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}

/***************************************************************************************
  * @函数描述：  电机运转方向控制端              主机__PB12 PB13   1号从机__PB14 PB15
  * @入口参数：  无.
  * @返回值  :   无.
****************************************************************************************/
void Dir_Config(void)         
{
	
  GPIO_InitTypeDef GPIO_InitStructure; 
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;        
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_ResetBits(GPIOB,GPIO_Pin_12);
	GPIO_ResetBits(GPIOB,GPIO_Pin_13);
	GPIO_ResetBits(GPIOB,GPIO_Pin_14);
	GPIO_ResetBits(GPIOB,GPIO_Pin_15);
	
}

/***************************************************************************************
  * @函数描述：  定时器2编码器模式初始化    PA0 PA1
  * @入口参数：  无.
  * @返回值  :   无.
****************************************************************************************/
void TIM2_EncoderMode(void)                                                    
{  
	GPIO_InitTypeDef GPIO_InitStructure;   
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;   
	TIM_ICInitTypeDef TIM_ICInitStructure; 
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 
	GPIO_Init(GPIOA, &GPIO_InitStructure);  
	
	TIM_DeInit(TIM2);
	
	TIM_TimeBaseStructure.TIM_Period =0xFFFF;                                              
	TIM_TimeBaseStructure.TIM_Prescaler = 0;                                               
	TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;                                        
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;     
	TIM_TimeBaseInit(TIM2, & TIM_TimeBaseStructure);

	TIM_EncoderInterfaceConfig(TIM2, TIM_EncoderMode_TI1,TIM_ICPolarity_Rising, TIM_ICPolarity_Rising); 

	TIM_ICStructInit(&TIM_ICInitStructure);
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;
	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
	TIM_ICInitStructure.TIM_ICFilter = 0x0;
	TIM_ICInit(TIM2, &TIM_ICInitStructure);

  TIM_SetCounter(TIM2,0);
	TIM_Cmd(TIM2, ENABLE);  //启动时钟
	
}


/***************************************************************************************
  * @函数描述：  定时器3编码器模式初始化  PA6 PA7 
  * @入口参数：  无.  
  * @返回值  :   无.  
****************************************************************************************/
void TIM3_EncoderMode(void)                                                            
{
	GPIO_InitTypeDef GPIO_InitStructure; 
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_ICInitTypeDef TIM_ICInitStructure;                                              
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	TIM_DeInit(TIM3);
	
	TIM_TimeBaseStructure.TIM_Period =0xFFFF;                                             
	TIM_TimeBaseStructure.TIM_Prescaler = 0;                                              
	TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;                                        
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;                           
	TIM_TimeBaseInit(TIM3, & TIM_TimeBaseStructure);

	TIM_EncoderInterfaceConfig(TIM3, TIM_EncoderMode_TI1,TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);   

	TIM_ICStructInit(&TIM_ICInitStructure);
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;
	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
	TIM_ICInitStructure.TIM_ICFilter = 0x0;
	TIM_ICInit(TIM3, &TIM_ICInitStructure);

  TIM_SetCounter(TIM3,0);
	TIM_Cmd(TIM3, ENABLE);  
}

/***************************************************************************************
  * @函数描述：  定时器4 PWM模式初始化   通道1——PB6  通道2——PB7 通道3——PB8  通道4——PB9
  * @入口参数：  无.
  * @返回值  :   无.
****************************************************************************************/
void TIM4_PWMMode(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
  TIM_OCInitTypeDef TIM_OCInitStructure;                                      
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 ; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	TIM_DeInit(TIM4);
	
	TIM_TimeBaseStructure.TIM_Period = 10000;                                    //重装值,所以定时器频率为10KHz
	TIM_TimeBaseStructure.TIM_Prescaler = 0;                                    //分频  系统时钟72MHz,分频8后为9MHz
	TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;                              
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;                
	TIM_TimeBaseInit(TIM4, & TIM_TimeBaseStructure);
	
	TIM_InternalClockConfig(TIM4);                                             
	
	/********************通道1PWM配置*************/
	TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM1;                            
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;               
	TIM_OCInitStructure.TIM_Pulse = 0;                                          
	TIM_OCInitStructure.TIM_OCPolarity =TIM_OCPolarity_High;                   
	
	TIM_OC1Init(TIM4,&TIM_OCInitStructure);                                     
	TIM_OC1PreloadConfig(TIM4,TIM_OCPreload_Enable);
	
	/********************通道2PWM配置**************/
	TIM_OC2Init(TIM4,&TIM_OCInitStructure);                                     
	TIM_OC2PreloadConfig(TIM4,TIM_OCPreload_Enable);                                                       
	
	TIM_ARRPreloadConfig(TIM4, ENABLE);                                         
  TIM_Cmd(TIM4, ENABLE);                                                     
	TIM_SetCompare1(TIM4,0);
	TIM_SetCompare2(TIM4,0);

}
/***************************************************************************************
  * @函数描述：  定时器1中断模式初始化   定时刷新系统
  * @入口参数：  无.
  * @返回值  :   无.
****************************************************************************************/
void TIM1_InterruptMode(void)
{
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);
	
	TIM_InternalClockConfig(TIM1);  
	TIM_DeInit(TIM1);
	
	TIM_TimeBaseStructure.TIM_Period = 380;                //4ms定时中断刷新系统
	TIM_TimeBaseStructure.TIM_Prescaler = 719;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM1, & TIM_TimeBaseStructure);
	
	TIM_ClearITPendingBit(TIM1, TIM_IT_Update);  
	TIM_ITConfig(TIM1, TIM_IT_Update, ENABLE );  
	
	TIM_ARRPreloadConfig(TIM1, ENABLE);

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1); 
	NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	TIM_Cmd(TIM1, ENABLE);  //启动时钟
	
}

/***************************************************************************************
  * @函数描述：  模拟量采集初始化   ADC1_PC3
  * @入口参数：  无.
  * @返回值  :   无.
****************************************************************************************/
void ADC1_Configuration(void)
{
	ADC_InitTypeDef ADC_InitStructure; 
	GPIO_InitTypeDef GPIO_InitStructure; 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_ADC1,ENABLE);
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);  //ADC时钟12MHz
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;      //模拟量输入管脚
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN; 
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	ADC_DeInit(ADC1);
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent; 
	ADC_InitStructure.ADC_ScanConvMode = DISABLE; 
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE; 
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;   //软件启动转化
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right; 
	ADC_InitStructure.ADC_NbrOfChannel = 1;                       //转换通道数目
	ADC_Init(ADC1, &ADC_InitStructure);
	ADC_Cmd(ADC1,ENABLE);
	ADC_TempSensorVrefintCmd(ENABLE);        //打开内部温度传感器通道
	
	ADC_ResetCalibration(ADC1);                           //开启复位校准
	while(ADC_GetResetCalibrationStatus(ADC1));           //等待复位校准结束
	ADC_StartCalibration(ADC1);                           //开启AD校准
	while(ADC_GetCalibrationStatus(ADC1));                //等待校准结束
}


/***************************************************************************************
  * @函数描述：  指示灯初始化   PC13
  * @入口参数：  无.
  * @返回值  :   无.
****************************************************************************************/
void LED_Init(void)       
{
	GPIO_InitTypeDef GPIO_InitStructure; 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;    
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	GPIO_SetBits(GPIOC,GPIO_Pin_13);   
}

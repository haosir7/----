#include "AI.h"

volatile uint16_t ADCConvertedValue[2];

void ADC1_DMA_Init(void);
void Adc_GPIO_Init(void);
void Adc_Init(void);

void Adc_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_2 | GPIO_Pin_3;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
  
}


void Adc_Init(void)
{
	ADC_InitTypeDef ADC_InitStructure;
	
	ADC_DeInit(ADC1);
	
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_ADC1, ENABLE); 
	RCC_ADCCLKConfig(RCC_PCLK2_Div8);//设置ADC时钟（ADCCLK） 72MHZ/6 = 12MHZ
	
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;//独立模式
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;//mul 通道)
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;//除能连续模式
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;//
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//ADC数据右对齐
	ADC_InitStructure.ADC_NbrOfChannel = 2;//规则转换的ADC的通道数目,取值范围为1-16   3
	ADC_Init(ADC1,&ADC_InitStructure);//初始化配置
   
	ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 1, ADC_SampleTime_13Cycles5);//Pa1
	ADC_RegularChannelConfig(ADC1, ADC_Channel_3, 2, ADC_SampleTime_13Cycles5);//Pa2
	
	ADC_Cmd(ADC1,ENABLE);//  adc	
	
	ADC_ResetCalibration(ADC1);
	while( ADC_GetResetCalibrationStatus(ADC1));//获取ADC重置校准寄存器的状态 等待复位结束
	ADC_StartCalibration(ADC1);//开始指定ADC的校准状态
	while( ADC_GetCalibrationStatus(ADC1) );
	

	ADC1_DMA_Init();
	ADC_DMACmd(ADC1, ENABLE);//enable adc DMA
	
		/* 由于没有采用外部触发，所以使用软件触发ADC转换 */ 
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
	
}

void ADC1_DMA_Init(void)
{
	 DMA_InitTypeDef DMA_InitStructure;
	
   RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

   //ADC1 DMA channel 1
   DMA_DeInit(DMA1_Channel1);
   DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_Address;
   DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&ADCConvertedValue;
   DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
   DMA_InitStructure.DMA_BufferSize = 2;
   DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
   DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//  内存自增
   DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;//16位
   DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;//16位
   DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;//
   DMA_InitStructure.DMA_Priority = DMA_Priority_High;
   DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
   DMA_Init(DMA1_Channel1, &DMA_InitStructure);
   
   /* Enable DMA1 channel1 */
   DMA_Cmd(DMA1_Channel1, ENABLE);
	
}

void AI_init(void)
{
	Adc_GPIO_Init();
	
	Adc_Init();
}

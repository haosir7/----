#include "CAN1.h"

/****************************************************************************************
  * @����������  CAN��ʼ��
  * @��ڲ�����  ��.
  * @����ֵ  :   ��.
****************************************************************************************/
void CAN1_GPIOConfig(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO,ENABLE);      //����PB���ܽŸ���
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;                                     //Rx
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;                                     //Tx
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
//	GPIO_PinRemapConfig(GPIO_Remap1_CAN1,ENABLE);         //�ܽ���ӳ��
}

void CAN1_NVIC_Configuration(void)
{
  	NVIC_InitTypeDef NVIC_InitStructure;
 
  	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
  	NVIC_InitStructure.NVIC_IRQChannel=USB_LP_CAN1_RX0_IRQn;;
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  	NVIC_Init(&NVIC_InitStructure);
}
	
	
void CAN1_Configuration(void)
{
	CAN_InitTypeDef CAN_InitStructure;
	CAN_FilterInitTypeDef CAN_FilterInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1,ENABLE);    //ע�⣬����APB1

	/************************ģʽ�벨��������*************/
	CAN_DeInit(CAN1);
	CAN_StructInit(&CAN_InitStructure);
	
	CAN_InitStructure.CAN_TTCM=DISABLE; 
	CAN_InitStructure.CAN_ABOM=DISABLE;
	CAN_InitStructure.CAN_AWUM=DISABLE; 
	CAN_InitStructure.CAN_NART=DISABLE;
	CAN_InitStructure.CAN_RFLM=DISABLE; 
	CAN_InitStructure.CAN_TXFP=DISABLE;
	
	CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;   //CAN_Mode_LoopBack���ػ�ģʽ��CAN_Mode_Normal:����ģʽ
	CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;
	CAN_InitStructure.CAN_BS1 = CAN_BS1_5tq;
	CAN_InitStructure.CAN_BS2 = CAN_BS2_3tq; 
	CAN_InitStructure.CAN_Prescaler = 4;            //100Kͨ���ٶ�
	CAN_Init(CAN1,&CAN_InitStructure);
	
	/***********************����������********************/
	CAN_FilterInitStructure.CAN_FilterNumber = 0;    //0�Ź�����
	CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask; 
	CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit; 
	CAN_FilterInitStructure.CAN_FilterIdHigh = 0x0000;         //�ر���������
	CAN_FilterInitStructure.CAN_FilterIdLow = 0x0000; 
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0x0000; 
	CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0x0000; 
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_FIFO0;    //����FIFO0
	CAN_FilterInitStructure.CAN_FilterActivation = ENABLE; 
	CAN_FilterInit(&CAN_FilterInitStructure);
	
	CAN_ITConfig(CAN1,CAN_IT_FMP0, ENABLE);         //���������ж�
	
}
void CAN1_Send(uint32_t ID,u8 *Data)       
{  
	u8 TransmitMailbox=0,i=0;  
	CanTxMsg TxMessage;  
	
	TxMessage.StdId = ID;            
	TxMessage.ExtId = 0x0000;
	TxMessage.IDE = CAN_ID_STD; 
	TxMessage.RTR = CAN_RTR_DATA;
	TxMessage.DLC = 8; 
	for(i=0;i<8;i++)
	{
		TxMessage.Data[i] = *Data++; 
	}
	
	TransmitMailbox=CAN_Transmit(CAN1,&TxMessage);
	
	while((CAN_TransmitStatus(CAN1,TransmitMailbox)!=CANTXOK)&&(i!=0xFF))
	{
		i++;
	}
	i=0;
	while((CAN_MessagePending(CAN1,CAN_FIFO0)<1)&&(i!=0xFF))
	{
		i++;
	}
}

void USB_LP_CAN1_RX0_IRQHandler(void)
{
	CanRxMsg RxMessage;
	
	/*********************Rx��������***********************/
	RxMessage.StdId=0x00;
	RxMessage.IDE =CAN_ID_STD;
	RxMessage.RTR=CAN_RTR_DATA;
	RxMessage.DLC = 8; 

	RxMessage.Data[0] = 0x00; 
	RxMessage.Data[1] = 0x00; 
	CAN_Receive(CAN1,CAN_FIFO0,&RxMessage);
	
} 


#include "delay.h"

static u8  fac_us=0;							//us延时倍乘数			   
static u16 fac_ms=0;							//ms延时倍乘数,在ucos下,代表每个节拍的ms�
#define SYSTICK_COUNTFLAG           16
	

void delay_init()
{
	SysTick->CTRL  &=		(~2);				//0xfffffffb;		//bit2清空,选择外部时钟  HCLK/8
	
	fac_us=(72000000/1000000)/8;	 //1us需计算的脉冲个数,其中(SystemFrequency/1000000)代表1US的脉冲个数,8是需要8分频		 
	fac_ms=(u16)fac_us*1000;           //1ms需计算的脉冲个数
}								    

	    								   
void delay_us(u32 nus)
{		
	u32 temp;	    	 
	SysTick->LOAD=nus*fac_us; //时间加载	  		 
	SysTick->VAL   =  (0x00);                     /* Load the SysTick Counter Value */         //清空计数器
	SysTick->CTRL = (1<<0);		/* Enable SysTick and SysTick Timer */  //开始倒数 	 
	do
		{
		temp=SysTick->CTRL;
		}
	while(temp&0x01&&!(temp&(1<<SYSTICK_COUNTFLAG)));//等待时间到达   
	//SysTick->CTRL	&=  (~(1<<SYSTICK_ENABLE));    //关闭计数器
	SysTick->CTRL = 0x00;
	SysTick->VAL   =  (0x00);                     /* Load the SysTick Counter Value */         //清空计数器
}
 
void delay_ms(u16 nms)
{	 		  	  
	u32 temp;	
	SysTick->LOAD=(u32)nms*fac_ms;//时间加载(SysTick->LOAD为24bit)
	SysTick->VAL   =  (0x00);                     /* Load the SysTick Counter Value */         //清空计数器
	SysTick->CTRL =  (1<<0);   /* Enable SysTick and SysTick Timer */      //开始倒数  
	do
		{
		temp=SysTick->CTRL;
		}
	while((temp&0x01)&&(!(temp&(1<<SYSTICK_COUNTFLAG))));//等待时间到达   
	SysTick->CTRL	&=  (~(1<<0));    //关闭计数器
	SysTick->VAL   =  (0x00);                     /* Load the SysTick Counter Value */         //清空计数器  
} 



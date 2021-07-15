#include<KEY.h>
#include<stm32f10x_gpio.h>
#include<delay.h>
void KEY_Init()
{	GPIO_InitTypeDef GPIO_InitStructure;
	
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);//使能IO口时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;//上拉输入
	GPIO_Init(GPIOA,&GPIO_InitStructure);//GPIOA口初始化
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_5;
	GPIO_Init(GPIOC,&GPIO_InitStructure);//GPIOD初始化
	
}

u8 KEY_SCAN(u8 mode)//mode 为0不支持连按，为1支持
{
	static u8 key_up = 1;//1代表先前未按下
	if(mode==1) key_up = 1;
	if(key_up&&(KEY0==0||KEY1 == 0))
	{	
		delay_ms(10);
		key_up = 0;
		if(KEY0 == 0)
			return 1;//key0按下
		else if(KEY1 ==0)
			return 2;//key1按下
	}
	else if(KEY0 == 1&&KEY1==1)key_up = 1;
		return 0;//都没按下
}

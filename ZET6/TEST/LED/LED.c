#include<LED.h>
#include<stm32f10x_gpio.h>

void LED_init()
{	
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);//ʹ��IO��ʱ��
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_Init(GPIOC,&GPIO_InitStructure);//GPIOC�ڳ�ʼ��
	LED = 0;
}

void LED_ctl()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG,ENABLE);//ʹ��IO��ʱ��
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_9;
	GPIO_Init(GPIOG,&GPIO_InitStructure);//GPIOG�ڳ�ʼ��
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_10;
	GPIO_Init(GPIOG,&GPIO_InitStructure);//GPIOG�ڳ�ʼ��
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_11;
	GPIO_Init(GPIOG,&GPIO_InitStructure);//GPIOG�ڳ�ʼ��
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_12;
	GPIO_Init(GPIOG,&GPIO_InitStructure);//GPIOG�ڳ�ʼ��
	LED1 = 0;
	LED2 = 0;
	LED3 = 0;
	LED4 = 0;
}

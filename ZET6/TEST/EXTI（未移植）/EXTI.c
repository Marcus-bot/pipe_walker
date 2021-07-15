#include <EXTI.h>
#include <KEY.h>
#include <USART1.h>
#include<stm32f10x_gpio.h>
void myEXTI1_Init()
{
	EXTI_InitTypeDef EXTI_InitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;
	KEY_Init();//��ʼ��IO
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);//����IO����ʱ��
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource15);//�ж���ӳ��
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource5);
	
	
	EXTI_InitStruct.EXTI_Line=EXTI_Line5;//��ʼ���ⲿ�ж�
	EXTI_InitStruct.EXTI_LineCmd=ENABLE;
	EXTI_InitStruct.EXTI_Mode=EXTI_Mode_Interrupt;
	EXTI_InitStruct.EXTI_Trigger=EXTI_Trigger_Falling;
	EXTI_Init(&EXTI_InitStruct);
	EXTI_InitStruct.EXTI_Line=EXTI_Line15;
	EXTI_Init(&EXTI_InitStruct);
	
	
	NVIC_InitStruct.NVIC_IRQChannel=EXTI9_5_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=1;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority=2;
	NVIC_Init(&NVIC_InitStruct);
	NVIC_InitStruct.NVIC_IRQChannel=EXTI15_10_IRQn;
	NVIC_Init(&NVIC_InitStruct);
}
	

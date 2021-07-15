#include<LED.h>
#include<stm32f10x.h>
#include<delay.h>
#include<usart.h>
#include<usart2.h>
#include<mpu6050.h>
#include<inv_mpu.h>
#include<timer.h>
#include<myiic.h>
#include<control.h>


extern Angle_def Angle;

u8 flag=1,last;
u8 pidcal20ms;//10ms��
u8 check;
vu8 Con_Info[16];//�����ź�

void TIM5_IRQHandler()//TIM5�жϷ�����
{
	if(TIM_GetITStatus(TIM5,TIM_IT_Update)!=RESET)
	{
		TIM_ClearITPendingBit(TIM5,TIM_IT_Update);
		
		pidcal20ms++;
		PID_Control();
	}
	TIM_ClearITPendingBit(TIM5,TIM_IT_Update);
}
char testled = 1;

int main()
{	
	delay_init();
	IIC_Init();								//��Ҫ
	LED_init();								//����ָʾ�Ƴ�ʼ��								
	LED_ctl();								////��ɫ�źŵƳ�ʼ��	
	Motor_Init();		
	PID_Init();
	myTIMER5_Init(1999,719);
	myTIMER4_Init(1999,719);
	uart_init(921600);						//����1����ݮ�ɣ�
	//USART2_Init(9600);					//����2��ʼ��
	while(mpu_dmp_init()!=0);				//��ʼ��������
	mpu_dmp_get_data(&(Angle.pitch),&(Angle.roll),&(Angle.yaw));	//��ȡ��̬����
	LED = 1;
	TIM5_SET(ENABLE);
	while(1)
	{
		//delay_ms(1);
		mpu_dmp_get_data(&(Angle.pitch),&(Angle.roll),&(Angle.yaw));
		UART1_Receive();
		check = verify(Con_Info);
		if(check == Con_Info[15])
		{
			Motor_1234(Con_Info[0],Con_Info[1],Con_Info[8],Con_Info[9],Con_Info[10],Con_Info[11],Con_Info[12]);//1��Ǳ2�ϸ�
			Motor_5678(Con_Info[2],Con_Info[3],Con_Info[4],Con_Info[5],Con_Info[6],Con_Info[7]);
			//Motor_5678(Con_Info[2],Con_Info[3],Con_Info[4],Con_Info[5],1,15);
			LED_1234(Con_Info[14]);
			Light_Set(Con_Info[13]);
		}
		//printf("AA%cBB\r\n",30);

	}
}


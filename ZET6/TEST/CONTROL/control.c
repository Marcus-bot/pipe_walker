#include<control.h>
#include<timer.h>
#include<MPU6050.h>
#include<delay.h>
#include<LED.h>
#include "MS5837.h"
#define dir1 (1)
#define dir2 (-1)
#define dir3 (1)
#define dir4 (-1)
#define dir5 (1)
#define dir6 (-1)
#define dir7 (-1)
#define dir8 (1)


short standPWM=150;

u8 MAX = 100;

PID_def PID_ROL, PID_PIT,PID_DEP;
Angle_def Angle;
Angle_def Target;

float moto1 = 0, moto2 = 0, moto3 = 0, moto4 = 0;
float T;
float test1,test2,test3;
void Motor_Init()
{
	myTIMER3_PWM_Init(1999,719);			//计数2000次，720分频，20ms
	myTIMER1_PWM_Init(1999,719);			//计数2000次，720分频，20ms	
	Motor1_set(0);
	Motor2_set(0);
	Motor3_set(0);
	Motor4_set(0);
	Motor5_set(0);
	Motor6_set(0);
	Motor7_set(0);
	Motor8_set(0);
	delay_ms(1000);
	delay_ms(1000);
	delay_ms(500);
	delay_ms(1000);
	delay_ms(1000);
	

}

void Motor1_set(short speed)//dir正反桨叶，取值forward和backward,speed正负控制正推反推
{
	if(speed >= MAX)
		speed = MAX;
	if(speed <= (-1)*MAX)
		speed = (-1)*MAX;
	TIM_SetCompare1(TIM3,standPWM+dir1*speed);
	test1 = standPWM+dir1*speed;
	
}

void Motor2_set(short speed)
{
	if(speed >= MAX)
		speed = MAX;
	if(speed <= (-1)*MAX)
		speed = (-1)*MAX;
	TIM_SetCompare2(TIM3,standPWM+dir2*speed);
}

void Motor3_set(short speed)
{
	if(speed >= MAX)
		speed = MAX;
	if(speed <= (-1)*MAX)
		speed = (-1)*MAX;
	TIM_SetCompare4(TIM3,standPWM+dir3*speed);
}

void Motor4_set(short speed)
{
	if(speed >= MAX)
		speed = MAX;
	if(speed <= (-1)*MAX)
		speed = (-1)*MAX;
	TIM_SetCompare3(TIM3,standPWM+dir4*speed);
}

void Motor5_set(short speed)
{
	if(speed >= MAX)
		speed = MAX;
	if(speed <= (-1)*MAX)
		speed = (-1)*MAX;
	TIM_SetCompare2(TIM1,standPWM+dir5*speed);
}

void Motor6_set(short speed)
{
	if(speed >= MAX)
		speed = MAX;
	if(speed <= (-1)*MAX)
		speed = (-1)*MAX;
	TIM_SetCompare1(TIM1,standPWM+dir6*speed);
}

void Motor7_set(short speed)
{
	if(speed >= MAX)
		speed = MAX;
	if(speed <= (-1)*MAX)
		speed = (-1)*MAX;
	TIM_SetCompare3(TIM1,standPWM+dir7*speed);
}

void Motor8_set(short speed)
{
	if(speed >= MAX)
		speed = MAX;
	if(speed <= (-1)*MAX)
		speed = (-1)*MAX;
	TIM_SetCompare4(TIM1,standPWM+dir8*speed);
}

void Motor_1234(u8 UD,u8 speed_UD,u8 POS,u8 PIT,u8 speed_PIT,u8 ROL,u8 speed_ROL)//1下潜2上浮0不动1低头2抬头1左滚2右滚
{	
	signed char M1x,M2x,M3x,M4x;
	signed char M1y,M2y,M3y,M4y;
	signed char M1z,M2z,M3z,M4z;
	short out_M1,out_M2,out_M3,out_M4;
	if(UD!=0)
	{
		Target_DEP = Pressure;
	}
	switch (UD){
		case 0: {M1y=0;M2y=0;M3y=0;M4y=0;break;}
		case 1: {M1y=1;M2y=1;M3y=1;M4y=1;break;}
		case 2: {M1y=(signed char)(-1);M2y=(signed char)(-1);M3y=(signed char)(-1);M4y=(signed char)(-1);break;}
	}
	switch (PIT){
		case 0: {M1z=0;M2z=0;M3z=0;M4z=0;break;}
		case 1: {M1z=(signed char)(-1);M2z=1;M3z=1;M4z=(signed char)(-1);break;}
		case 2: {M1z=1;M2z=(signed char)(-1);M3z=(signed char)(-1);M4z=1;break;}

	}
	switch (ROL){
		case 0: {M1x=0;M2x=0;M3x=0;M4x=0;break;}
		case 1: {M1x=(signed char)(-1);M2x=(signed char)(-1);M3x=1;M4x=1;break;}
		case 2: {M1x=1;M2x=1;M3x=(signed char)(-1);M4x=(signed char)(-1);break;}
	}
	out_M1 = M1y*speed_UD+M1z*speed_PIT+M1x*speed_ROL;
	out_M2 = M2y*speed_UD+M2z*speed_PIT+M2x*speed_ROL;
	out_M3 = M3y*speed_UD+M3z*speed_PIT+M3x*speed_ROL;
	out_M4 = M4y*speed_UD+M4z*speed_PIT+M4x*speed_ROL;
	
	if( POS ==  0)
	{
		out_M1+=moto1;
		out_M2+=moto2;
		out_M3+=moto3;
		out_M4+=moto4;
	}
	
	Motor1_set(out_M1);
	Motor2_set(out_M2);
	Motor3_set(out_M3);
	Motor4_set(out_M4);
}

void Motor_5678(u8 FB, u8 speed1, u8 LR, u8 speed2,u8 SH, u8 speed3)//1前进2后退0不动，1左转2右转0不动
{
	signed char M5x,M6x,M7x,M8x;//正反推
	signed char M5y,M6y,M7y,M8y;
	signed char M5z,M6z,M7z,M8z;//正反推
	switch (FB){
		case 0: {M5x=0;M6x=0;M7x=0;M8x=0;break;}
		case 1: {M5x=1;M6x=1;M7x=(signed char)(-1);M8x=(signed char)(-1);break;}
		case 2: {M5x=(signed char)(-1);M6x=(signed char)(-1);M7x=1;M8x=1;break;}
	}
	switch (SH){
		case 0: {M5y=0;M6y=0;M7y=0;M8y=0;break;}
		case 1: {M5y=(signed char)(-1);M6y=1;M7y=1;M8y=(signed char)(-1);break;}
		case 2: {M5y=1;M6y=(signed char)(-1);M7y=(signed char)(-1);M8y=1;break;}
	}
	switch (LR){
		case 0: {M5z=0;M6z=0;M7z=0;M8z=0;break;}
		case 1: {M5z=(signed char)(-1);M6z=1;M7z=(signed char)(-1);M8z=1;break;}
		case 2: {M5z=1;M6z=(signed char)(-1);M7z=1;M8z=(signed char)(-1);break;}
	}
	
	Motor5_set(M5x*speed1+M5z*speed2+M5y*speed3);
	Motor6_set(M6x*speed1+M6z*speed2+M6y*speed3);
	Motor7_set(M7x*speed1+M7z*speed2+M7y*speed3);
	Motor8_set(M8x*speed1+M8z*speed2+M8y*speed3);
}

void PID_Init()
{
	//ROLL
	PID_ROL.P=0.6;
	PID_ROL.I=0;
	PID_ROL.D=0.1;
	PID_ROL.Ek=0;
	PID_ROL.preEk=0;
	PID_ROL.P_out=0;
	PID_ROL.I_out=0;
	PID_ROL.D_out=0;
	PID_ROL.OUT=0;
	PID_ROL.I_limit = 0;
	//PITCH
	PID_PIT.P=0.4;
	PID_PIT.I=0;
	PID_PIT.D=0.05;
	PID_PIT.Ek=0;
	PID_PIT.preEk=0;
	PID_PIT.P_out=0;
	PID_PIT.I_out=0;
	PID_PIT.D_out=0;
	PID_PIT.OUT=0;
	PID_PIT.I_limit = 0;
	
//	PID_ROL.P=0.75;
//	PID_ROL.I=0;
//	PID_ROL.D=0;
//	PID_ROL.Ek=0;
//	PID_ROL.preEk=0;
//	PID_ROL.P_out=0;
//	PID_ROL.I_out=0;
//	PID_ROL.D_out=0;
//	PID_ROL.OUT=0;
//	PID_ROL.I_limit = 0;
//	//PITCH
//	PID_PIT.P=0.55;
//	PID_PIT.I=0;
//	PID_PIT.D=0;
//	PID_PIT.Ek=0;
//	PID_PIT.preEk=0;
//	PID_PIT.P_out=0;
//	PID_PIT.I_out=0;
//	PID_PIT.D_out=0;
//	PID_PIT.OUT=0;
//	PID_PIT.I_limit = 0;
	
	PID_DEP.P=2.5;
	PID_DEP.I=0.02;
	PID_DEP.D=1.2;
	PID_DEP.Ek=0;
	PID_DEP.preEk=0;
	PID_DEP.P_out=0;
	PID_DEP.I_out=0;
	PID_DEP.D_out=0;
	PID_DEP.OUT=0;
	PID_DEP.I_limit = 12;
	
	
	Target.pitch = 0;
	Target.roll = 0;
	Target.yaw = 0;
}

void getMxMi(float* moto)
{
	if(*moto>=(float)100) *moto = (float)100;
	if(*moto<=(float)(-100)) *moto = (float)(-100);
}




void PID_Calc( PID_def* PID, float target, float angle)
{
	float Imax;
	
	Imax = PID->I_limit;
	PID->Ek = target - angle;
	
	PID->P_out = PID->P * PID->Ek;
	
	PID->I_out += PID->I * PID->Ek;
	if(PID->I_out<-Imax)
	{
		PID->I_out = -Imax;
	}
	else if(PID->I_out>Imax)
	{
		PID->I_out=Imax;
	}
	PID->D_out = PID->D * (PID->Ek - PID->preEk);
	
	PID->OUT = PID->P_out + PID->I_out + PID->D_out;
	
	PID->preEk = PID->Ek;
}
int32_t Target_DEP;
void PID_Control()
{
	PID_Calc(&PID_PIT,Target.pitch,Angle.pitch);
	PID_Calc(&PID_ROL,Target.roll,Angle.roll);
	PID_Calc(&PID_DEP,Target_DEP,Pressure);
	moto1 =  - PID_ROL.OUT + PID_PIT.OUT - PID_DEP.OUT;
	moto2 =  - PID_ROL.OUT - PID_PIT.OUT - PID_DEP.OUT;
	moto3 =  + PID_ROL.OUT - PID_PIT.OUT - PID_DEP.OUT;
	moto4 =  + PID_ROL.OUT + PID_PIT.OUT - PID_DEP.OUT;
}

void LED_1234(u8 ctr8)
{
	if(ctr8==1)
	{
		LED1 = 1;
		LED2 = 0;
		LED3 = 0;
		LED4 = 0;
	}
	if(ctr8==2)
		{
		LED1 = 0;
		LED2 = 1;
		LED3 = 0;
		LED4 = 0;
	}
	if(ctr8==3)
		{
		LED1 = 0;
		LED2 = 0;
		LED3 = 1;
		LED4 = 0;
	}
	if(ctr8==4)
		{
		LED1 = 0;
		LED2 = 0;
		LED3 = 0;
		LED4 = 1;
	}
	if(ctr8==0)
		{
		LED1 = 0;
		LED2 = 0;
		LED3 = 0;
		LED4 = 0;
	}
}

void Light_Set(u8 light)//typical 35
{
	u8 LIGHT = 110;
	LIGHT = 110+light;
	if(LIGHT >= 180)
		LIGHT = 180;
	if(LIGHT <= 110)
		LIGHT = 110;
	TIM_SetCompare1(TIM4,LIGHT);
	TIM_SetCompare2(TIM4,LIGHT);
}

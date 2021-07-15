#include<PID.h>
#include<PCA9685.h>
#include<MS5837.h>
#include<control.h>
PID pid;
PID2 pid2;
extern float pitch,roll,yaw;
void MODE1_PID_Init()
{
	pid.Set=Pressure;//设定值
	pid.Kp=5;//比例系数
	pid.T=100;//采样时间
	pid.Ti=200;//积分周期
	pid.Td=20;//微分周期
	pid.Ek_1=0;
	pid.Ek_2=0;
	pid.currout=standardPWM;
}

void MODE2_PID2_Init()//////姿态调整2
{
	pid2.Set=0;//设定值
	pid2.Kp1=5;//比例系数
	pid2.T=100;//采样时间
	pid2.Ti1=200;//积分周期
	pid2.Td1=20;//微分周期
	pid2.Ek1_1=0;
	pid2.Ek1_2=0;
	
	pid2.Kp2=5;//比例系数
	pid2.T=100;//采样时间
	pid2.Ti2=200;//积分周期
	pid2.Td2=20;//微分周期
	pid2.Ek2_1=0;
	pid2.Ek2_2=0;
	
	pid2.currout1=0;
	pid2.currout2=0;
}


void MODE1_PID_Clac(float Pressure,u8* pidcal10ms,u8 str8,u8 str9)//中间螺旋桨辅助定深
{
	float dk1,dk2;
	float t1,t2,t3;
	if(str8 == 1||str9 == 1) 
		{
		//	setPWM(Motor6,0,standardPWM);//上升下降时不转
			pid.Set = Pressure;
			return ;
		}
	if((*pidcal10ms*10)<pid.T) return ;
	
	pid.Ek=pid.Set-Pressure;
	dk1=pid.Ek-pid.Ek_1;
	dk2=pid.Ek-2*pid.Ek_1-pid.Ek_2;
	t1=(float)pid.Kp*dk1;
	t2=(float)pid.Kp*pid.T*pid.Ek/pid.Ti;
	t3=(float)pid.Kp*pid.Td*dk2/pid.T;
	
	pid.Dout=t1+t2+t3;
	pid.currout+=pid.Dout;
	if(pid.currout>MotorMAX) pid.currout=MotorMAX;
	else if(pid.currout<MotorMIN)	pid.currout = MotorMIN;
	else ;
	setPWM(Motor6,0,(u16)pid.currout);
	pid.Ek_2=pid.Ek_1;
	pid.Ek_1=pid.Ek;
	*pidcal10ms=0;
}


void MODE2_PID1_Clac(float Pressure,u8 str5,u8* pidcal10ms,u8 str8,u8 str9)//中间螺旋桨辅助定深
{
	float dk1,dk2;
	float t1,t2,t3;
	static u8 oldstr5 = 0;
	if(oldstr5!=str5)
		{
			Motor_2[1] = 0;
			oldstr5 = 0;
		}
	if(str8 == 1||str9 == 1) 
		{
			pid.Set = Pressure;
		}
	if(str5 == 0)return;
	oldstr5 = str5;
	if((*pidcal10ms*10)<pid.T) return ;
	
	pid.Ek=pid.Set-Pressure;
	dk1=pid.Ek-pid.Ek_1;
	dk2=pid.Ek-2*pid.Ek_1-pid.Ek_2;
	t1=(float)pid.Kp*dk1;
	t2=(float)pid.Kp*pid.T*pid.Ek/pid.Ti;
	t3=(float)pid.Kp*pid.Td*dk2/pid.T;
	
	pid.Dout=t1+t2+t3;
	pid.currout+=pid.Dout;
	if(pid.currout>MotorMAX) pid.currout=MotorMAX;
	else if(pid.currout<MotorMIN)	pid.currout = MotorMIN;
	else ;
	setPWM(Motor6,0,(u16)pid.currout);
	pid.Ek_2=pid.Ek_1;
	pid.Ek_1=pid.Ek;
	*pidcal10ms=0;
}

void MODE2_PID2_Calc(u8* pidcal10ms)
{
	float dk1,dk2;
	float t1,t2,t3;
	if((*pidcal10ms*10)<pid2.T) return ;
	
	pid2.Ek1=pid.Set-pitch;
	dk1=pid2.Ek1-pid2.Ek1_1;
	dk2=pid2.Ek1-2*pid2.Ek1_1-pid2.Ek1_2;
	t1=(float)pid2.Kp1*dk1;
	t2=(float)pid2.Kp1*pid2.T*pid2.Ek1/pid2.Ti1;
	t3=(float)pid2.Kp1*pid2.Td1*dk2/pid2.T;
	pid2.Dout=t1+t2+t3;
	pid2.currout1+=pid.Dout;
	pid2.currout2+=pid.Dout;
	if(pid2.currout1>MotorMAX) pid2.currout1=MotorMAX;
	else if(pid2.currout1<MotorMIN)	pid2.currout1 = MotorMIN;
	
	
	pid2.Ek2=pid.Set-roll;
	dk1=pid2.Ek2-pid2.Ek2_1;
	dk2=pid2.Ek2-2*pid2.Ek2_1-pid2.Ek2_2;
	t1=(float)pid2.Kp2*dk1;
	t2=(float)pid2.Kp2*pid2.T*pid2.Ek2/pid2.Ti2;
	t3=(float)pid2.Kp2*pid2.Td2*dk2/pid2.T;
	pid2.Dout=t1+t2+t3;
	pid2.currout1+=pid.Dout;
	pid2.currout2+=pid.Dout;
	if(pid2.currout2>MotorMAX) pid2.currout2=MotorMAX;
	else if(pid2.currout2<MotorMIN)	pid2.currout2 = MotorMIN;
	
	setPWM(Motor1,0,(u16)(standardPWM+reverse*pid2.currout1+reverse*pid2.currout2));
	setPWM(Motor2,0,(u16)(standardPWM+forward*pid2.currout1-forward*pid2.currout2));
	setPWM(Motor3,0,(u16)(standardPWM+reverse*pid2.currout1+reverse*pid2.currout2));
	setPWM(Motor4,0,(u16)(standardPWM+forward*pid2.currout1-forward*pid2.currout2));
	
	pid2.Ek1_2=pid2.Ek1_1;
	pid2.Ek1_1=pid2.Ek1;
	
	pid2.Ek2_2=pid2.Ek2_1;
	pid2.Ek2_1=pid2.Ek2;
	*pidcal10ms=0;
}

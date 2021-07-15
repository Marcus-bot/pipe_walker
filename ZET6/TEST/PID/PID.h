#ifndef _PID_H
#define _PID_H
#include<sys.h>
typedef struct{
	float Set;//设定值
	
	float Ek;//当前偏差
	float Ek_1;//前一次偏差
	float Ek_2;//前前次偏差
	float Kp;//比例系数
	u16 T;//采样周期
	u16 Ti;//积分周期
	u16 Td;//微分周期
	short Dout;
	short currout;//当前应当输出 
	
}PID;
typedef struct{
	float Set;//设定值

	float Ek1;//当前偏差
	float Ek1_1;//前一次偏差
	float Ek1_2;//前前次偏差
	
	float Ek2;//当前偏差
	float Ek2_1;//前一次偏差
	float Ek2_2;//前前次偏差
	
	u16 T;//采样周期
	
	float Kp1;//比例系数
	u16 Ti1;//积分周期
	u16 Td1;//微分周期
	
	float Kp2;//比例系数
	u16 Ti2;//积分周期
	u16 Td2;//微分周期
	
	short Dout;
	short currout1;//当前应当输出 
	short currout2;//当前应当输出 
}PID2;



extern PID pid;
void MODE1_PID_Init(void);
void MODE1_PID_Clac(float Pressure,u8* pidcal10ms,u8 str8,u8 str9);
void MODE2_PID1_Clac(float Pressure,u8 str5,u8* pidcal10ms,u8 str8,u8 str9);
void MODE2_PID2_Init(void);
void MODE2_PID2_Calc(u8* pidcal10ms);
#endif



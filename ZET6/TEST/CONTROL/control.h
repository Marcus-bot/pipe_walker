#ifndef __CONTROL_H
#define __CONTROL_H
#include<sys.h>

// typedef struct PID{float Kp,Ti,Td,DOUT,OUT,Ek,Ek_1,Ek_2;}PID;
typedef struct PID_typedef{float P,I,D,P_out,I_out,D_out,OUT,Ek,preEk,I_limit;}PID_def;
typedef struct Angle_typedef{float pitch,roll,yaw;}Angle_def;

void Motor_Init(void);
void Motor1_set(short speed);
void Motor2_set(short speed);
void Motor3_set(short speed);
void Motor4_set(short speed);
void Motor5_set(short speed);
void Motor6_set(short speed);
void Motor7_set(short speed);
void Motor8_set(short speed);
void Motor_1234(u8 UD,u8 speed_UD,u8 POS,u8 PIT,u8 speed_PIT,u8 ROL,u8 speed_ROL);
void Motor_5678(u8 FB, u8 speed1, u8 LR, u8 speed2,u8 SH, u8 speed3);
void PID_Calc( PID_def* PID, float target, float angle);
void PID_Control(void);
void PID_Init(void);
void LED_1234(u8 ctr8);
void Light_Set(u8 light);

#endif

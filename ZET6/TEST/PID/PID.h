#ifndef _PID_H
#define _PID_H
#include<sys.h>
typedef struct{
	float Set;//�趨ֵ
	
	float Ek;//��ǰƫ��
	float Ek_1;//ǰһ��ƫ��
	float Ek_2;//ǰǰ��ƫ��
	float Kp;//����ϵ��
	u16 T;//��������
	u16 Ti;//��������
	u16 Td;//΢������
	short Dout;
	short currout;//��ǰӦ����� 
	
}PID;
typedef struct{
	float Set;//�趨ֵ

	float Ek1;//��ǰƫ��
	float Ek1_1;//ǰһ��ƫ��
	float Ek1_2;//ǰǰ��ƫ��
	
	float Ek2;//��ǰƫ��
	float Ek2_1;//ǰһ��ƫ��
	float Ek2_2;//ǰǰ��ƫ��
	
	u16 T;//��������
	
	float Kp1;//����ϵ��
	u16 Ti1;//��������
	u16 Td1;//΢������
	
	float Kp2;//����ϵ��
	u16 Ti2;//��������
	u16 Td2;//΢������
	
	short Dout;
	short currout1;//��ǰӦ����� 
	short currout2;//��ǰӦ����� 
}PID2;



extern PID pid;
void MODE1_PID_Init(void);
void MODE1_PID_Clac(float Pressure,u8* pidcal10ms,u8 str8,u8 str9);
void MODE2_PID1_Clac(float Pressure,u8 str5,u8* pidcal10ms,u8 str8,u8 str9);
void MODE2_PID2_Init(void);
void MODE2_PID2_Calc(u8* pidcal10ms);
#endif



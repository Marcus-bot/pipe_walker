#ifndef __MYIIC_H
#define __MYIIC_H
#include "sys.h"
/////////////////////////////////////////////////////////////////////////////////////////		 
//B30 ��ȴ�������������
//�����壺BlueTest STM32
//������̳: www.Bluerobots.cn ��BlueRobots ˮ�»�����������
//�޸�����: 2019/4/30
//���̰汾��V1.2
//��ϵ���䣺info@bluerobots.cn
//�ر���������������Դ�����磬��BlueRobots ���������޸ĺ����ڽ�������ʹ�������ге�һ�к����
/////////////////////////////////////////////////////////////////////////////////////////	
   	   		   
#define SDA_IN()  {GPIOA->CRL&=0XFFF0FFFF;GPIOA->CRL|=(u32)8<<16;}
#define SDA_OUT() {GPIOA->CRL&=0XFFF0FFFF;GPIOA->CRL|=(u32)3<<16;}

//IO????	 
#define IIC_SCL    PAout(5) //SCL
#define IIC_SDA    PAout(4) //SDA	 
#define READ_SDA   PAin(4)  //??SDA 
 
//IIC���в�������
void IIC3_Init(void);                //��ʼ��IIC��IO��				 
void IIC3_Start(void);								//����IIC��ʼ�ź�
void IIC3_Stop(void);	  						//����IICֹͣ�ź�
void IIC3_Send_Byte(u8 txd);					//IIC����һ���ֽ�
u8 IIC3_Read_Byte(unsigned char ack);//IIC��ȡһ���ֽ�
u8 IIC3_Wait_Ack(void); 							//IIC�ȴ�ACK�ź�
void IIC3_Ack(void);									//IIC����ACK�ź�
void IIC3_NAck(void);								//IIC������ACK�ź�

void IIC3_Write_One_Byte(u8 daddr,u8 addr,u8 data);
u8 IIC3_Read_One_Byte(u8 daddr,u8 addr);	  

#endif

// BlueRobots Lab















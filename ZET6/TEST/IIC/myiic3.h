#ifndef __MYIIC_H
#define __MYIIC_H
#include "sys.h"
/////////////////////////////////////////////////////////////////////////////////////////		 
//B30 深度传感器测试例程
//开发板：BlueTest STM32
//交流论坛: www.Bluerobots.cn （BlueRobots 水下机器人社区）
//修改日期: 2019/4/30
//例程版本：V1.2
//联系邮箱：info@bluerobots.cn
//特别声明：本程序来源于网络，经BlueRobots 社区网友修改后用于交流分享，使用者自行承担一切后果！
/////////////////////////////////////////////////////////////////////////////////////////	
   	   		   
#define SDA_IN()  {GPIOA->CRL&=0XFFF0FFFF;GPIOA->CRL|=(u32)8<<16;}
#define SDA_OUT() {GPIOA->CRL&=0XFFF0FFFF;GPIOA->CRL|=(u32)3<<16;}

//IO????	 
#define IIC_SCL    PAout(5) //SCL
#define IIC_SDA    PAout(4) //SDA	 
#define READ_SDA   PAin(4)  //??SDA 
 
//IIC所有操作函数
void IIC3_Init(void);                //初始化IIC的IO口				 
void IIC3_Start(void);								//发送IIC开始信号
void IIC3_Stop(void);	  						//发送IIC停止信号
void IIC3_Send_Byte(u8 txd);					//IIC发送一个字节
u8 IIC3_Read_Byte(unsigned char ack);//IIC读取一个字节
u8 IIC3_Wait_Ack(void); 							//IIC等待ACK信号
void IIC3_Ack(void);									//IIC发送ACK信号
void IIC3_NAck(void);								//IIC不发送ACK信号

void IIC3_Write_One_Byte(u8 daddr,u8 addr,u8 data);
u8 IIC3_Read_One_Byte(u8 daddr,u8 addr);	  

#endif

// BlueRobots Lab















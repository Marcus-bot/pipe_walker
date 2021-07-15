#include<PCA9685.h>
#include<myiic.h>
#include<math.h>
#include<delay.h>
void WritePCA9685(u8 address,u8 data)
{
	IIC_Init();
	IIC_Start();
	IIC_Send_Byte(PCA9685_adrr);
	IIC_Wait_Ack();
	IIC_Send_Byte(address);
	IIC_Wait_Ack();
	IIC_Send_Byte(data);
	IIC_Wait_Ack();
	IIC_Stop();
}

u8 ReadPCA9685(u8 address)
{
	u8 data;
	IIC_Init();
	IIC_Start();
	IIC_Send_Byte(PCA9685_adrr);
	IIC_Wait_Ack();
	IIC_Send_Byte(address);
	IIC_Wait_Ack();
	IIC_Start();
	IIC_Send_Byte(PCA9685_adrr|0x01);
	IIC_Wait_Ack();	
	data=IIC_Read_Byte(0);
	IIC_Stop();
	return data;
}
void reset()
{
	WritePCA9685(PCA9685_MODE1,0x0);
}
void PCA9685_Init(u8 freq)
{
	reset();
	setPWMfreq(freq);
}
void setPWMfreq(float freq)
{
	float prescaleval;
	u8 oldmode,newmode,prescale;
	freq*=0.96;
	prescaleval = 25000000/4096;
	prescaleval /=freq;
  prescaleval -= 1;
  prescale = floor(prescaleval + 0.5);
	
	oldmode=ReadPCA9685(PCA9685_MODE1);
	newmode=(oldmode&0x7F)|0x10;
	WritePCA9685(PCA9685_MODE1,newmode);
	WritePCA9685(PCA9685_PRESCALE,prescale);
	WritePCA9685(PCA9685_MODE1,oldmode);
	delay_ms(2);
	WritePCA9685(PCA9685_MODE1,oldmode|0xa1);
}

void setPWM(u16 num,u16 on,u16 off)
{
	if (num>=0&&num<=1)
	{
		if(off>435)off = 435;
		else if(off<202) off = 202;
	}
	else if (num>=2&&num<=8)
	{
		if (off>424) off=424;
		else if (off<212)off=212;
	}
		
	WritePCA9685(LED0_ON_L+4*num,on);
	WritePCA9685(LED0_ON_H+4*num,on>>8);
	WritePCA9685(LED0_OFF_L+4*num,off);
	WritePCA9685(LED0_OFF_H+4*num,off>>8);
}

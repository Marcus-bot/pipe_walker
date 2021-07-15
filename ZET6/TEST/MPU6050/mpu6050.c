#include<mpu6050.h>
#include<myiic.h>
#include<inv_mpu.h>
#include "inv_mpu_dmp_motion_driver.h"
#include "math.h"
#include<usart.h>
#include<delay.h>
static signed char gyro_orientation[9] = {-1, 0, 0,
                                           0,-1, 0,
                                           0, 0, 1};/////???????????

u8 MPU_Write(u8 addr,u8 reg,u8 len,u8* data)
{
	u8 i;       
	IIC_Start();
	IIC_Send_Byte((addr<<1)+0);
	if(IIC_Wait_Ack())
	{
		IIC_Stop();
		return 1;
	}
	IIC_Send_Byte(reg);
	if(IIC_Wait_Ack())
	{
		IIC_Stop();
		return 1;
	}
	for(i=0;i<len;i++)
	{
		IIC_Send_Byte(*data);
		data++;
		if(IIC_Wait_Ack())
	{
		IIC_Stop();
		return 1;
	}
	}
	IIC_Stop();
	return 0;
}
u8 MPU_Read(u8 addr,u8 reg,u8 len,u8* loc)
{
	u8 i;
	IIC_Init();
	IIC_Start();
	IIC_Send_Byte((addr<<1)+0);
	IIC_Wait_Ack();
	IIC_Send_Byte(reg);
	IIC_Wait_Ack();
	IIC_Start();
	IIC_Send_Byte((addr<<1)+1);
	IIC_Wait_Ack();
	for(i=0;i<len;i++)
	{
		if(i<len-1) *loc = IIC_Read_Byte(1);
		else *loc = IIC_Read_Byte(0);
		loc++;
	}

	IIC_Stop();
	return 0;
}

void MPU_DMP_Init()//mpu——dmp初始化
{
	IIC_Init();
	
	delay_ms(20);
	if(!mpu_init())USART_SendData(USART1,'1');
	if(!mpu_set_sensors(INV_XYZ_GYRO|INV_XYZ_ACCEL))USART_SendData(USART1,'2');
	if(!mpu_configure_fifo(INV_XYZ_GYRO|INV_XYZ_ACCEL))USART_SendData(USART1,'3');
	if(!mpu_set_sample_rate(DEFAULT_MPU_HZ))USART_SendData(USART1,'4');
	if(!dmp_load_motion_driver_firmware())USART_SendData(USART1,'5');
	if(!dmp_set_orientation(inv_orientation_matrix_to_scalar(gyro_orientation)))USART_SendData(USART1,'6');
	if(!dmp_enable_feature(DMP_FEATURE_6X_LP_QUAT|DMP_FEATURE_TAP|	//设置dmp功能
		    DMP_FEATURE_ANDROID_ORIENT|DMP_FEATURE_SEND_RAW_ACCEL|DMP_FEATURE_SEND_CAL_GYRO|
		    DMP_FEATURE_GYRO_CAL))USART_SendData(USART1,'7');
	if(!dmp_set_fifo_rate(DEFAULT_MPU_HZ))USART_SendData(USART1,'8');
	if(!mpu_set_dmp_state(1))USART_SendData(USART1,'9');
}
void MPU_Write_Byte(u8 reg,u8 data)
{
	IIC_Start();
	IIC_Send_Byte((MPU_ADDR<<1)+0);
	IIC_Wait_Ack();
	IIC_Send_Byte(reg);
	IIC_Wait_Ack();
	IIC_Send_Byte(data);
	IIC_Wait_Ack();
	IIC_Stop();
}
u8 MPU_Read_Byte(u8 reg)
{
	u8 data;
	IIC_Start();
	IIC_Send_Byte((MPU_ADDR<<1)+0);
	IIC_Wait_Ack();
	IIC_Send_Byte(reg);
	IIC_Wait_Ack();
	IIC_Start();
	IIC_Send_Byte((MPU_ADDR<<1)+1);
	IIC_Wait_Ack();
	data=IIC_Read_Byte(0);
	IIC_Stop();
	return data;
}

#include "MS5837.h"
#include "math.h"
uint32_t Cal_C[7];      
float Aux;
signed int dT,TEMP;
int64_t OFF_,SENS;
uint32_t D1_Pres,D2_Temp;	
double Temperature;
int32_t Pressure,Pressure_old;				
int32_t OFFi=0,SENSi=0,Ti=0;
int64_t OFF2 = 0;
int64_t SENS2 = 0;
uint32_t TEMP2;	//温度校验值
double T2;
//#define IIC_Start IIC3_Start
//#define IIC_Send_Byte IIC3_Send_Byte
//#define IIC_Read_Byte IIC3_Read_Byte
//#define IIC_Wait_Ack IIC3_Wait_Ack
//#define IIC_Stop IIC3_Stop
//#define IIC_Init IIC3_Init




void MS5837_30BA_ReSet(void)
{
		IIC_Start();
		IIC_Send_Byte(MS5837_30BA_WriteCommand);//发送写命令
	  IIC_Wait_Ack();
    IIC_Send_Byte(MS5837_30BA_ResetCommand);//发送复位命令
	  IIC_Wait_Ack();
    IIC_Stop();
	
}

u8 MS5837_30BA_PROM(void)
 {	 
  u8  inth,intl;
  int i;
  
	 MS5837_30BA_ReSet();	                                             //复位MS5837
  delay_ms(20); 
	 for (i=0;i<7;i++) 
	{
 
		IIC_Start();
    IIC_Send_Byte(MS5837_30BA_WriteCommand);      //发送写命令
		IIC_Wait_Ack();
		IIC_Send_Byte(MS5837_30BA_PROM_RD + (i*2));   //发送PROM获取命令
		IIC_Wait_Ack();
    IIC_Stop();
		delay_us(5);
		IIC_Start();
		IIC_Send_Byte(MS5837_30BA_ReadCommand);       //进入接收模式
		delay_us(5);
		IIC_Wait_Ack();
		inth = IIC_Read_Byte(1);  		                //带ACK的读数据
		delay_us(5);
		intl = IIC_Read_Byte(0); 			                //最后一个字节NACK
		IIC_Stop();
    Cal_C[i] = (((uint16_t)inth << 8) | intl);
	}
	 return !Cal_C[0];
 }



unsigned long MS5837_30BA_GetConversion(u8 command)
{
 
			unsigned long conversion = 0;
			u8 temp[3];
	
	    IIC_Start();
			IIC_Send_Byte(MS5837_30BA_WriteCommand); 		//写地址
			IIC_Wait_Ack();
			IIC_Send_Byte(command); //写转换命令
			IIC_Wait_Ack();
			IIC_Stop();

			delay_ms(10);
			IIC_Start();
			IIC_Send_Byte(MS5837_30BA_WriteCommand); 		//写地址
			IIC_Wait_Ack();
			IIC_Send_Byte(0);				// start read sequence
			IIC_Wait_Ack();
			IIC_Stop();
		 
			IIC_Start();
			IIC_Send_Byte(MS5837_30BA_ReadCommand);  //进入接收模式
			IIC_Wait_Ack();
			temp[0] = IIC_Read_Byte(1);  //带ACK的读数据  bit 23-16
			temp[1] = IIC_Read_Byte(1);  //带ACK的读数据  bit 8-15
			temp[2] = IIC_Read_Byte(0);  //带NACK的读数据 bit 0-7
			IIC_Stop();
			
			conversion = (unsigned long)temp[0] * 65536 + (unsigned long)temp[1] * 256 + (unsigned long)temp[2];
			return conversion;
 
}



void MS5837_30BA_GetData(void)
{
	
	D2_Temp = MS5837_30BA_GetConversion(MS5837_30BA_D2_OSR_8192);
	delay_ms(20);
	D1_Pres= MS5837_30BA_GetConversion(MS5837_30BA_D1_OSR_8192);
	delay_ms(20);
	dT=D2_Temp - (((uint32_t)Cal_C[5])*256);
	SENS=(int64_t)Cal_C[1]*65536l+((int64_t)Cal_C[3]*dT)/128;
	OFF_=(int64_t)Cal_C[2]*131072l+((int64_t)Cal_C[4]*dT)/64;
	TEMP = 2000+(int64_t)(dT)*Cal_C[6]/8388608LL;
	
	if(TEMP<2000)  
	{
		Ti = (11*(int64_t)(dT)*(int64_t)(dT)/(34359738368LL));
		OFFi = (31*(TEMP-2000)*(TEMP-2000))/8;
		SENSi = (63*(TEMP-2000)*(TEMP-2000))/32;
	}
  OFF2 = OFF_-OFFi;           
	SENS2 = SENS-SENSi;
  Pressure=((D1_Pres*SENS2)/2097152l-OFF2)/32768/100;          //校准后压力数据
  Temperature=(TEMP-Ti)/100.0;                                //校准后温度数据  
}

unsigned char MS5837_30BA_Crc4()
{
	int cnt;
	int t;
	unsigned int n_rem=0;
	unsigned char n_bit;	
	unsigned char  a=0;
	unsigned char  b=0;
	unsigned short  int n_prom[8];
	
	for( t=0;t<7;t++)
{
   n_prom[t]=Cal_C[t];
}	
	n_prom[0]=((n_prom[0]) & 0x0FFF);
	n_prom[7]=0;
	for (cnt = 0; cnt < 16; cnt++)
		{
	if (cnt%2==1) n_rem ^= (unsigned short) ((n_prom[cnt>>1]) & 0x00FF);	
	else n_rem ^= (unsigned short) (n_prom[cnt>>1]>>8);		
	for (n_bit = 8; n_bit > 0; n_bit--)
	{
	if (n_rem & (0x8000)) n_rem = (n_rem << 1) ^ 0x3000;
	else n_rem = (n_rem << 1);
	}

		}
	n_rem= ((n_rem >> 12) & 0x000F);	
    a=(n_rem ^ 0x00);
	   b=Cal_C[0]>>12;
		if (a==b)
		{
		   return 1;
		}
		else return 0;
	
}


u8 MS5837_Init()
{
	u8 ans=3;
	MS5837_30BA_ReSet();	                                                   //复位MS5837
	delay_ms(20); 
	MS5837_30BA_PROM();                                                     //初始化MS5837
	delay_ms(20);
	if(!MS5837_30BA_Crc4())                                               //CRC校验
			{
				ans = 0;
			}
			else 
			{
			     ans = 1;
			}
	return ans;
}

void Pressure_Send()
{
	u16 h;
	h = ((Pressure-950)*1000)/98;
	while(USART_GetFlagStatus(USART2,USART_FLAG_TC)==0);
	USART_SendData(USART2,0x41);
	while(USART_GetFlagStatus(USART2,USART_FLAG_TC)==0);
	USART_SendData(USART2,0x41);
	while(USART_GetFlagStatus(USART2,USART_FLAG_TC)==0);
	USART_SendData(USART2,'d');
	while(USART_GetFlagStatus(USART2,USART_FLAG_TC)==0);
	USART_SendData(USART2,h>>8);
	while(USART_GetFlagStatus(USART2,USART_FLAG_TC)==0);
	USART_SendData(USART2,h%255);
	while(USART_GetFlagStatus(USART2,USART_FLAG_TC)==0);
	USART_SendData(USART2,0x42);
	while(USART_GetFlagStatus(USART2,USART_FLAG_TC)==0);
	USART_SendData(USART2,0x42);
	while(USART_GetFlagStatus(USART2,USART_FLAG_TC)==0);
	USART_SendData(USART2,'\r');
	while(USART_GetFlagStatus(USART2,USART_FLAG_TC)==0);
	USART_SendData(USART2,'\n');
}


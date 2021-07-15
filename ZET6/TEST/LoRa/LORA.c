#include<LORA.h>
#include<usart2.h>
#include<usart.h>
#include<string.h>
#include<stm32f10x_gpio.h>
#include<delay.h>

void start_config()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOA, ENABLE );	
	   
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	MD0 = 1;
}
void stop_config()
{
	MD0 = 0;
}
u8* LORA_check_cmd(u8 *str)
{
	char *strx=0;
	if(USART2_RX_STA&0X8000)		
	{ 
		USART2_RX_BUF[USART2_RX_STA&0X7FFF]=0;
		strx=strstr((const char*)USART2_RX_BUF,(const char*)str);
	} 
	return (u8*)strx;
}

u8 LORA_send_cmd(u8 *cmd,u8 *ack,u16 waittime)
{
	u8 res=0; 
	USART2_RX_STA=0;
	delay_ms(10);
	u2_printf("%s\r\n",cmd);	
	if(ack&&waittime)		
	{
		while(--waittime)	
		{
			delay_ms(10);
			if(USART2_RX_STA&0X8000)
			{
				if(LORA_check_cmd(ack))
				{
				//	printf("%s\r\n",(u8*)ack);
					break;
				}
					USART2_RX_STA=0;
			} 
		}
		if(waittime==0)res=1; 
	}
	return res;
} 
//void got_data(u8* cmd,u8* ack,u16 waittime)//测试使用
//{
//	u8 len=0; 
//	u16 i;
//	USART2_RX_STA=0;
//	delay_ms(10);
//	u2_printf("%s\r\n",cmd);	
//	while(--waittime)	//等待倒计时
//		{
//			delay_ms(10);
//			if(1)
//			{
//				if(LORA_check_cmd(ack)!=0)
//					printf("Got it\r\n");
//				len = USART2_RX_STA&0x7fff;
//				for(i=0;i<len;i++)
//				{
//					USART_SendData(USART1,USART2_RX_BUF[i]);
//					while(USART_GetFlagStatus(USART1,USART_FLAG_TC)==0);
//				}
//				break;
//			} 
//		}
//		USART2_RX_STA=0;
//}

u8 LORA_start()
{
	u8 status=0;
	start_config();
	USART2_Init(115200);
	status+=LORA_send_cmd("AT","OK",40);
	status+=LORA_send_cmd("AT+ADDR=00,08","OK",40);
	status+=LORA_send_cmd("AT+WLRATE=23,5","OK",40);
	stop_config();
	return status;
}


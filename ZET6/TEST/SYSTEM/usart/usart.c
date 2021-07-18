#include "sys.h"
#include "usart.h"	  
////////////////////////////////////////////////////////////////////////////////// 	 
//���ʹ��ucos,����������ͷ�ļ�����.
#if SYSTEM_SUPPORT_OS
#include "includes.h"					//ucos ʹ��	  
#endif

u8 UART1_Receive(void);


//�������´���,֧��printf����,������Ҫѡ��use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���                 
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
void _sys_exit(int x) 
{ 
	x = x; 
} 
//�ض���fputc���� 
int fputc(int ch, FILE *f)
{      
	while((USART1->SR&0X40)==0);//ѭ������,ֱ���������   
    USART1->DR = (u8) ch;      
	return ch;
}
#endif 

 
 
#if EN_USART1_RX   //���ʹ���˽���
//����1�жϷ������
//ע��,��ȡUSARTx->SR�ܱ���Ī������Ĵ���   	
u8 USART_RX_BUF[USART_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.
//����״̬
//bit15��	������ɱ�־
//bit14��	���յ�0x0d
//bit13~0��	���յ�����Ч�ֽ���Ŀ
u16 USART_RX_STA=0;       //����״̬���	  
  
void uart_init(u32 bound){
	//GPIO�˿�����
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);	//ʹ��USART1��GPIOAʱ��
  
	//USART1_TX   GPIOA.9
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
	GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA.9
   
	//USART1_RX	  GPIOA.10��ʼ��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//PA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
	GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA.10  

	//Usart1 NVIC ����
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1 ;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
  
	//USART ��ʼ������

	USART_InitStructure.USART_BaudRate = bound;//���ڲ�����
	USART_InitStructure.USART_WordLength = USART_WordLength_9b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_Even;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ

	USART_Init(USART1, &USART_InitStructure); //��ʼ������1
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//�������ڽ����ж�
	USART_Cmd(USART1, ENABLE);                    //ʹ�ܴ���1 

}
u8 cmd_flag;
void USART1_IRQHandler(void)                	//����1�жϷ������
{
	u8 Res;
#if SYSTEM_SUPPORT_OS 		//���SYSTEM_SUPPORT_OSΪ�棬����Ҫ֧��OS.
	OSIntEnter();    
#endif
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
		{
			Res =USART_ReceiveData(USART1);	//��ȡ���յ�������
			if(Res == '{')
			{
				cmd_flag = 1; //��һλ��{��ʼ����
				USART_RX_STA = 0;
			}
			if(cmd_flag == 1)
			{
				if((USART_RX_STA&0x8000)==0)//����δ���
				{
				if(USART_RX_STA&0x4000)//���յ���y
					{
						if(Res!='z')USART_RX_STA=0;//���մ���,���¿�ʼ
						else USART_RX_STA|=0x8000,cmd_flag = 0;	//��������� 
					}
				else //��û�յ�0X0D
					{
					if(Res=='y')USART_RX_STA|=0x4000;
					else
						{
							USART_RX_BUF[USART_RX_STA&0X3FFF]=Res ;
							USART_RX_STA++;
							if((USART_RX_STA&0X3FFF)>(USART_REC_LEN-1))USART_RX_STA=0;//�������ݴ���,���¿�ʼ����	  
						}		 
					}
				}   		 
			}
    }
	USART_ClearITPendingBit(USART1, USART_IT_RXNE);	
	if(USART_GetFlagStatus(USART1,USART_FLAG_ORE) == SET)
	{
		USART_ClearFlag(USART1,USART_FLAG_ORE);
		USART_ReceiveData(USART1);
	}
		
#if SYSTEM_SUPPORT_OS 	//���SYSTEM_SUPPORT_OSΪ�棬����Ҫ֧��OS.
	OSIntExit();  											 
#endif

} 
#endif	


extern vu8 Con_Info[10];
int sum;
//u8 UART1_Receive(void)
//{
//	u8 i,j;
//	u8 Start_flag=3;
//	if(USART_RX_STA&0x8000)
//	{
//		for (i=0;i<20;i++)
//		{
//			if (USART_RX_BUF[i]=='A'&&USART_RX_BUF[++i]=='A')
//				Start_flag=1;	//��ʼ��������
//			if (Start_flag==1)
//			{
//				for (j=0,++i;USART_RX_BUF[i]!='B';i++)
//				{
//					Con_Info[j]=USART_RX_BUF[i];
//					j++;
//				}
//			if (USART_RX_BUF[i]=='B'&&USART_RX_BUF[++i]=='B')
//				Start_flag=0;	//�������ݽ������
//			}
//		}
//	USART_RX_STA=0;
//	}
//	
//	return Start_flag;
//}

u8 UART1_Receive(void)
{
	u8 i,j;
	u8 Start_flag=1;
	if(USART_RX_STA&0x8000)
	{
			if (USART_RX_BUF[1]=='A'&&USART_RX_BUF[2]=='A')
			{
				if(USART_RX_BUF[USART_REC_LEN-1]=='B'&&USART_RX_BUF[USART_REC_LEN-2]=='B')
				{
					for (j=0,i=3;i<USART_REC_LEN-2;j++,i++)
					{
						Con_Info[j]=USART_RX_BUF[i];
					}
				Start_flag=0;	//�������ݽ������
				}
			}
	USART_RX_STA=0;
	}
	
	return Start_flag;
}



u8 verify(vu8* info)
{
	u8 i;
	int check = 0;
	for(i=0;i<15;i++)
	{
		check += info[i];
	}
	check = (check+1105)%120;
	return check;
}


#include "USART1.h"
#include "stm32f10x.h"
u8  USART_RX_BUF[16]; //���ջ���
u16 USART_RX_STA;         		//����״̬���	
//���ڳ�ʼ��
void USART1_init(u32 bound)
{
	GPIO_InitTypeDef  GPIO_InitStrue;
	USART_InitTypeDef USART_InitStrue;
	NVIC_InitTypeDef NVIC_InitStrue;
//ʱ��ʹ��	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
//GPIO9ʹ��	
	GPIO_InitStrue.GPIO_Mode=GPIO_Mode_AF_PP;//�����������
	GPIO_InitStrue.GPIO_Pin=GPIO_Pin_9;						//9��
	GPIO_InitStrue.GPIO_Speed=GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
	GPIO_Init(GPIOA,&GPIO_InitStrue);					 //�����趨������ʼ��GPIOA_9
//GPIO10ʹ�� 
	GPIO_InitStrue.GPIO_Mode=GPIO_Mode_IN_FLOATING;//�����������
	GPIO_InitStrue.GPIO_Pin=GPIO_Pin_10;						//10��
	GPIO_InitStrue.GPIO_Speed=GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
	GPIO_Init(GPIOA,&GPIO_InitStrue);					 //�����趨������ʼ��GPIOA_10
//USART��ʼ��
	USART_InitStrue.USART_BaudRate=bound;;//������9600
	USART_InitStrue.USART_HardwareFlowControl=USART_HardwareFlowControl_None;//��Ӳ��������
	USART_InitStrue.USART_Mode=USART_Mode_Rx|USART_Mode_Tx;//ʹ�ܷ��͡�����
	USART_InitStrue.USART_Parity=USART_Parity_No;//�޻�żУ��
	USART_InitStrue.USART_StopBits=USART_StopBits_1;//ֹͣλ1λ
	USART_InitStrue.USART_WordLength=USART_WordLength_8b;//����8λ
	USART_Init(USART1,&USART_InitStrue);//�����趨��ʼ��USART1
//USART1ʹ��
	USART_Cmd(USART1,ENABLE);
//���ж�
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);//���ж�USART1
	NVIC_InitStrue.NVIC_IRQChannel=USART1_IRQn;//ͨ��
	NVIC_InitStrue.NVIC_IRQChannelCmd=ENABLE;//ʹ��
	NVIC_InitStrue.NVIC_IRQChannelPreemptionPriority=1;//��ռ���ȼ�
	NVIC_InitStrue.NVIC_IRQChannelSubPriority=1;//�����ȼ�
	NVIC_Init(&NVIC_InitStrue);
}
//�жϷ�����
void USART1_IRQHandler(void)
{
	u8 USART1_in;
	if(USART_GetITStatus(USART1,USART_IT_RXNE))
	{
			USART1_in=USART_ReceiveData(USART1);
			if((USART_RX_STA&0x8000)==0)//����δ���
			{
				if(USART_RX_STA&0x4000)//���յ���0x0d
				{
					if(USART1_in!=0x0a)USART_RX_STA=0;//���մ���,���¿�ʼ
					else USART_RX_STA|=0x8000;	//��������� 
				}
				else //��û�յ�0X0D
				{	
					if(USART1_in==0x0d)USART_RX_STA|=0x4000;
					else
					{
						USART_RX_BUF[USART_RX_STA&0X3FFF]=USART1_in ;
						USART_RX_STA++;	
					}
				}
			}
	}
}
//�жϴ���С����
uint8_t get_ok_or_not(void)
{
  if(USART_RX_STA&0x8000)
		return(1);
	else
		return(0);
}
uint16_t get_long(void)
{
	u16 len;
	len=USART_RX_STA&0x3fff;//�õ��˴ν��յ������ݳ���
	return(len);		
}
void USART1_Send_String(u8 *p,u8 cnt)
{   
		 while(cnt>0)
		 {	 
		     USART_ClearFlag(USART1, USART_FLAG_TC);
		     USART_SendData(USART1,*p++);			 
		     while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
		     {
		   	     ;
		     }
				 cnt--;
		 }
}
void USART1_Send_Byte(uint16_t Data)
{ 
	while(!USART_GetFlagStatus(USART1,USART_FLAG_TXE));	  //USART_GetFlagStatus���õ�����״̬λ
														  //USART_FLAG_TXE:���ͼĴ���Ϊ�� 1��Ϊ�գ�0��æ״̬
	USART_SendData(USART1,Data);						  //����һ���ַ�
}
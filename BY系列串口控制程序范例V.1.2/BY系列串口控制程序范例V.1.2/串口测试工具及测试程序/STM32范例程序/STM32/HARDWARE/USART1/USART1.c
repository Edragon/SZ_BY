#include "USART1.h"
#include "stm32f10x.h"
u8  USART_RX_BUF[16]; //接收缓冲
u16 USART_RX_STA;         		//接收状态标记	
//串口初始化
void USART1_init(u32 bound)
{
	GPIO_InitTypeDef  GPIO_InitStrue;
	USART_InitTypeDef USART_InitStrue;
	NVIC_InitTypeDef NVIC_InitStrue;
//时钟使能	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
//GPIO9使能	
	GPIO_InitStrue.GPIO_Mode=GPIO_Mode_AF_PP;//复用推挽输出
	GPIO_InitStrue.GPIO_Pin=GPIO_Pin_9;						//9脚
	GPIO_InitStrue.GPIO_Speed=GPIO_Speed_50MHz;		 //IO口速度为50MHz
	GPIO_Init(GPIOA,&GPIO_InitStrue);					 //根据设定参数初始化GPIOA_9
//GPIO10使能 
	GPIO_InitStrue.GPIO_Mode=GPIO_Mode_IN_FLOATING;//复用推挽输出
	GPIO_InitStrue.GPIO_Pin=GPIO_Pin_10;						//10脚
	GPIO_InitStrue.GPIO_Speed=GPIO_Speed_50MHz;		 //IO口速度为50MHz
	GPIO_Init(GPIOA,&GPIO_InitStrue);					 //根据设定参数初始化GPIOA_10
//USART初始化
	USART_InitStrue.USART_BaudRate=bound;;//波特率9600
	USART_InitStrue.USART_HardwareFlowControl=USART_HardwareFlowControl_None;//无硬件流控制
	USART_InitStrue.USART_Mode=USART_Mode_Rx|USART_Mode_Tx;//使能发送、接收
	USART_InitStrue.USART_Parity=USART_Parity_No;//无基偶校验
	USART_InitStrue.USART_StopBits=USART_StopBits_1;//停止位1位
	USART_InitStrue.USART_WordLength=USART_WordLength_8b;//数据8位
	USART_Init(USART1,&USART_InitStrue);//根据设定初始化USART1
//USART1使能
	USART_Cmd(USART1,ENABLE);
//开中断
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);//开中断USART1
	NVIC_InitStrue.NVIC_IRQChannel=USART1_IRQn;//通道
	NVIC_InitStrue.NVIC_IRQChannelCmd=ENABLE;//使能
	NVIC_InitStrue.NVIC_IRQChannelPreemptionPriority=1;//抢占优先级
	NVIC_InitStrue.NVIC_IRQChannelSubPriority=1;//子优先级
	NVIC_Init(&NVIC_InitStrue);
}
//中断服务函数
void USART1_IRQHandler(void)
{
	u8 USART1_in;
	if(USART_GetITStatus(USART1,USART_IT_RXNE))
	{
			USART1_in=USART_ReceiveData(USART1);
			if((USART_RX_STA&0x8000)==0)//接收未完成
			{
				if(USART_RX_STA&0x4000)//接收到了0x0d
				{
					if(USART1_in!=0x0a)USART_RX_STA=0;//接收错误,重新开始
					else USART_RX_STA|=0x8000;	//接收完成了 
				}
				else //还没收到0X0D
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
//中断处理小函数
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
	len=USART_RX_STA&0x3fff;//得到此次接收到的数据长度
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
	while(!USART_GetFlagStatus(USART1,USART_FLAG_TXE));	  //USART_GetFlagStatus：得到发送状态位
														  //USART_FLAG_TXE:发送寄存器为空 1：为空；0：忙状态
	USART_SendData(USART1,Data);						  //发送一个字符
}
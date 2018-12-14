/*
("//�� Ŀ ��:MP3оƬ�ο�����\n");
("//����Ҫ��:������������MP3���ܺ���������");
("//Ӳ������:STM32��Сϵͳ�ʹ���ͨ��");
("//1) MCU�ͺ�:STM32F103");
("//2) �ⲿ��Ƶ��Ϊ:8M");
("//����汾:UART1: ");
("//�� ˾ ��: �����а�Ϊ���ӿƼ����޹�˾\n");
("//��    ��: lanq");
("//		Ӳ�����棺Pro");
("//		����ļ���Code\n");
��"//		����������keil5\n"��
("//����֧�֣�E-mail:balway@163.com\n");
("//�޸ļ�¼: 
*/
#include "stm32f10x.h"
#include "USART1.h"
//λ������,ʵ��51���Ƶ�GPIO���ƹ���
//����ʵ��˼��,�ο�<<CM3Ȩ��ָ��>>������(87ҳ~92ҳ).
//IO�ڲ����궨��
#define BITBAND(addr, bitnum) ((addr & 0xF0000000)+0x2000000+((addr &0xFFFFF)<<5)+(bitnum<<2)) 
#define MEM_ADDR(addr)  *((volatile unsigned long  *)(addr)) 
#define BIT_ADDR(addr, bitnum)   MEM_ADDR(BITBAND(addr, bitnum)) 
//IO�ڵ�ַӳ��
#define GPIOA_ODR_Addr    (GPIOA_BASE+12) //0x4001080C 
#define GPIOB_ODR_Addr    (GPIOB_BASE+12) //0x40010C0C  

#define GPIOA_IDR_Addr    (GPIOA_BASE+8) //0x40010808 
#define GPIOB_IDR_Addr    (GPIOB_BASE+8) //0x40010C08 

#define PAout(n)   BIT_ADDR(GPIOA_ODR_Addr,n)  //��� 
#define PAin(n)    BIT_ADDR(GPIOA_IDR_Addr,n)  //���� 

#define PBout(n)   BIT_ADDR(GPIOB_ODR_Addr,n)  //��� 
#define PBin(n)    BIT_ADDR(GPIOB_IDR_Addr,n)  //���� 

#define LED_H	    GPIO_SetBits(GPIOA, GPIO_Pin_1)	     // PC1 �ߵ�ƽ
#define	LED_L	    GPIO_ResetBits(GPIOA, GPIO_Pin_1)	 // PC1�͵�ƽ	   

#define KEY_1 PBin(7)			//����ʹ��
#define KEY_2	PBin(10)			//����ʹ��
#define KEY_3 PBin(13)			//����ʹ��
#define KEY_4	PBin(15)			//����ʹ��
#define KEY_5 PBin(12)			//����ʹ��
#define KEY_6	PBin(16)			//����ʹ��

u8 Check = 0;
u8 Play[]={0x7E,0x03,0x01,0x02,0xEF};//����ָ��
u8 Call[]={0x7E,0x03,0x0E,0x0D,0xEF};//ָֹͣ��

u8 Next[]={0x7E,0x03,0x03,0x00,0xEF};//��һ��ָ��
u8 Last[]={0x7E,0x03,0x04,0x07,0xEF};//��һ��ָ��

u8 Add[]={0x7E,0x03,0x05,0x06,0xEF};					//����+ָ��	
u8 Sub[]={0x7E,0x03,0x06,0x05,0xEF};         //������ָ��

u8 BCCheck(u8 *nDataBuf, u8 nLen)//�����麯��
{	
	u8 nSum = 0;
	u8 i    = 0;
	for(i=0; i<nLen; i++)
	{
		nSum ^= nDataBuf[i];//�����麯��
	}
	return nSum;
}
void Key_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

   
	 RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB, ENABLE);
//�������
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_10| GPIO_Pin_13| GPIO_Pin_15; 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
  GPIO_Init(GPIOB, &GPIO_InitStructure);
}
void Led_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

   
	 RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA, ENABLE);
//�������
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1; 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
  GPIO_Init(GPIOA, &GPIO_InitStructure);
}
u8 KEY_Scan(u8 mode)
{	 
	static u8 key_up=1;//�������ɿ���־
	if(mode)key_up=1;  //֧������
	if(key_up&&(KEY_1==0||KEY_2==0||KEY_3 ==0||KEY_4==0||KEY_5==0||KEY_6==0))
	{
			key_up=0;
			if(KEY_1==0) return '1';
			else if(KEY_2==0)return '2';
			else if(KEY_3==0)return '3';
			else if(KEY_4==0)return '4';
			else if(KEY_5==0)return '5';
			else if(KEY_6==0)return '6';
	} 
	if(KEY_1==1&&KEY_2==1&&KEY_3==1&&KEY_4==1&&KEY_5==1&&KEY_6==1)key_up=1;
	return 0;// �ް�������
}	

int main(void)
{
	u8 t;
	NVIC_PriorityGroupConfig( NVIC_PriorityGroup_2);
	USART1_init(9600);			//��ʼ�����ڲ�����9600
	USART1_Send_String("123456789\r\n",11);
	while(1)
	{	
		 t = KEY_Scan(0);
		 if(t)
		{
			switch(t)
			{
				case 1:USART1_Send_String(Play,sizeof(Play));break;
				case 2:USART1_Send_String(Call,sizeof(Call));break;
				case 3:USART1_Send_String(Next,sizeof(Next));break;
				case 4:USART1_Send_String(Last,sizeof(Last));break;
				case 5:USART1_Send_String(Add,sizeof(Add));break;
				case 6:USART1_Send_String(Sub,sizeof(Sub));break;
			}
		}
	}
}

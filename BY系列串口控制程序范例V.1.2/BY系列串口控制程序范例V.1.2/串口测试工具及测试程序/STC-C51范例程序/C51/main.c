/*("/*********************************************
("//�� Ŀ ��:MP3оƬ�ο�����\n");
("//����Ҫ��:������������MP3���ܺ���������");
("//Ӳ������:C51��Сϵͳ�ʹ���ͨ��");
("//1) MCU�ͺ�:stc89c52\AT89c52");
("//2) �ⲿ��Ƶ��Ϊ:11.0592M");
("//����汾:UART1: ");
("//�� ˾ ��: �����а�Ϊ���ӿƼ����޹�˾\n");
("//��    ��: lanq");
("//		Ӳ�����棺Pro");
("//		����ļ���Code\n");
��"//		����������keil4\n"��
("//����֧�֣�E-mail:balway@163.com\n");
("//�޸ļ�¼: 
("/*********************************************
*/

#include <reg52.h>			//C51ͷ�ļ�����
#define uint unsigned int
#define uchar unsigned char

sbit KEY1=P1^0;//����
sbit KEY2=P1^3;//ֹͣ
sbit KEY3=P1^5;//����+��
sbit KEY4=P1^7;//��������
sbit KEY5=P1^6;//��һ����
sbit KEY6=P1^2;//��һ����

uchar Check = 0;
uchar Play[]={0x7E,0x03,0x01,0x02,0xEF};//����ָ��
uchar Call[]={0x7E,0x03,0x0E,0x0D,0xEF};//ָֹͣ��

uchar Next[]={0x7E,0x03,0x03,0x00,0xEF};//��һ��ָ��
uchar Last[]={0x7E,0x03,0x04,0x07,0xEF};//��һ��ָ��

uchar Add[]={0x7E,0x03,0x05,0x06,0xEF};					//����+ָ��	
uchar Sub[]={0x7E,0x03,0x06,0x05,0xEF};         //������ָ��

uchar BCCheck(uchar *nDataBuf, uchar nLen)//�����麯��
{	
	uchar nSum = 0;
	uchar i    = 0;
	for(i=0; i<nLen; i++)
	{
		nSum ^= nDataBuf[i];//�����麯��
	}
	return nSum;
}  
void Delay(uint x)		//��ʱ����
{
 	uchar i;
	while(x--)
	{
	 	for(i=0;i<120;i++);
	}
}

void putc_to_SerialPort(uchar c)	//���ڷ��͵����ֽ�����
{
 	SBUF = c;
	while(TI == 0);
	TI = 0;
}

void puts_to_SerialPort(uchar *p,uchar cnt)	//�����ַ�����������
{
	while(cnt>0)
	{	 
		putc_to_SerialPort(*p++);			
		Delay(5);	 
		cnt--;
	}
}

void main()//������
{
 	uchar c = 0;
	SCON = 0x40;//���ڹ����ڷ�ʽ1 
	TMOD = 0x20;//��ʱ��1������8λ�Զ���װ��ģʽ�����ڲ��������� 
	PCON = 0x00;//����������λ9600��ע���ⲿ������Ҫ����11.0592M���У�
	TH1  = 0xfd;
	TL1  = 0xfd;
	TI   = 0;		//�����ж�
	TR1  = 1;		
	puts_to_SerialPort("123456789\r\n",sizeof("123456789\r\n"));	//�����ַ���
	Delay(100);
	while(1)
	{ 
		if(KEY1==0)//�����һ���������²���
		{		
				while(!KEY1);//�ȴ������ɿ�
				Play[0] = 0;
				Check = BCCheck(Play, 3);
				Play[0] = 0x7E;
			  Play[3] = Check;
				puts_to_SerialPort(Play,sizeof(Play));
		}		
		else if(KEY2==0)//����ڶ�����������ֹͣ
		{		
				while(!KEY2);//�ȴ������ɿ�
				puts_to_SerialPort(Call,sizeof(Call));
		}		
		else if(KEY3==0)//�����������������������
		{		
				while(!KEY3);//�ȴ������ɿ�
				puts_to_SerialPort(Add,sizeof(Add));
		}	
		else if(KEY4==0)//������ĸ���������������
		{		
				while(!KEY4);//�ȴ������ɿ�
				puts_to_SerialPort(Sub,sizeof(Sub));
		}			
		else if(KEY5==0)//������ĸ���������������
		{		
				while(!KEY5);//�ȴ������ɿ�
				puts_to_SerialPort(Next,sizeof(Next));
		}			
		else if(KEY6==0)//������ĸ���������������
		{		
				while(!KEY6);//�ȴ������ɿ�
				puts_to_SerialPort(Last,sizeof(Last));
		}			
	}
}
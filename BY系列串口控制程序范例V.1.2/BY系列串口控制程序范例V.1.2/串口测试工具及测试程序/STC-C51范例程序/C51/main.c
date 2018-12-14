/*("/*********************************************
("//项 目 名:MP3芯片参考代码\n");
("//功能要求:按键操作播放MP3功能和音量设置");
("//硬件配置:C51最小系统和串口通信");
("//1) MCU型号:stc89c52\AT89c52");
("//2) 外部振荡频率为:11.0592M");
("//软件版本:UART1: ");
("//公 司 名: 深圳市百为电子科技有限公司\n");
("//作    者: lanq");
("//		硬件仿真：Pro");
("//		软件文件：Code\n");
（"//		开发环境：keil4\n"）
("//技术支持：E-mail:balway@163.com\n");
("//修改记录: 
("/*********************************************
*/

#include <reg52.h>			//C51头文件定义
#define uint unsigned int
#define uchar unsigned char

sbit KEY1=P1^0;//播放
sbit KEY2=P1^3;//停止
sbit KEY3=P1^5;//音量+键
sbit KEY4=P1^7;//音量减键
sbit KEY5=P1^6;//上一曲键
sbit KEY6=P1^2;//下一曲键

uchar Check = 0;
uchar Play[]={0x7E,0x03,0x01,0x02,0xEF};//播放指令
uchar Call[]={0x7E,0x03,0x0E,0x0D,0xEF};//停止指令

uchar Next[]={0x7E,0x03,0x03,0x00,0xEF};//下一曲指令
uchar Last[]={0x7E,0x03,0x04,0x07,0xEF};//上一曲指令

uchar Add[]={0x7E,0x03,0x05,0x06,0xEF};					//音量+指令	
uchar Sub[]={0x7E,0x03,0x06,0x05,0xEF};         //音量减指令

uchar BCCheck(uchar *nDataBuf, uchar nLen)//异或检验函数
{	
	uchar nSum = 0;
	uchar i    = 0;
	for(i=0; i<nLen; i++)
	{
		nSum ^= nDataBuf[i];//异或检验函数
	}
	return nSum;
}  
void Delay(uint x)		//延时函数
{
 	uchar i;
	while(x--)
	{
	 	for(i=0;i<120;i++);
	}
}

void putc_to_SerialPort(uchar c)	//串口发送单个字节数据
{
 	SBUF = c;
	while(TI == 0);
	TI = 0;
}

void puts_to_SerialPort(uchar *p,uchar cnt)	//发送字符串或者数组
{
	while(cnt>0)
	{	 
		putc_to_SerialPort(*p++);			
		Delay(5);	 
		cnt--;
	}
}

void main()//主函数
{
 	uchar c = 0;
	SCON = 0x40;//串口工作在方式1 
	TMOD = 0x20;//定时器1工作在8位自动重装载模式，用于产生波特率 
	PCON = 0x00;//波特率设置位9600（注意外部晶振需要焊接11.0592M才行）
	TH1  = 0xfd;
	TL1  = 0xfd;
	TI   = 0;		//开总中断
	TR1  = 1;		
	puts_to_SerialPort("123456789\r\n",sizeof("123456789\r\n"));	//发送字符串
	Delay(100);
	while(1)
	{ 
		if(KEY1==0)//如果第一个键被按下播放
		{		
				while(!KEY1);//等待按键松开
				Play[0] = 0;
				Check = BCCheck(Play, 3);
				Play[0] = 0x7E;
			  Play[3] = Check;
				puts_to_SerialPort(Play,sizeof(Play));
		}		
		else if(KEY2==0)//如果第二个按键按下停止
		{		
				while(!KEY2);//等待按键松开
				puts_to_SerialPort(Call,sizeof(Call));
		}		
		else if(KEY3==0)//如果第三个按键按下音量加
		{		
				while(!KEY3);//等待按键松开
				puts_to_SerialPort(Add,sizeof(Add));
		}	
		else if(KEY4==0)//如果第四个按键按下音量减
		{		
				while(!KEY4);//等待按键松开
				puts_to_SerialPort(Sub,sizeof(Sub));
		}			
		else if(KEY5==0)//如果第四个按键按下音量减
		{		
				while(!KEY5);//等待按键松开
				puts_to_SerialPort(Next,sizeof(Next));
		}			
		else if(KEY6==0)//如果第四个按键按下音量减
		{		
				while(!KEY6);//等待按键松开
				puts_to_SerialPort(Last,sizeof(Last));
		}			
	}
}
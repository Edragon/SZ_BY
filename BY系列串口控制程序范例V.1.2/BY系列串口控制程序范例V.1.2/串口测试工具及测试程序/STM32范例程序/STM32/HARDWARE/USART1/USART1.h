#ifndef __USART1_H
#define __USART1_H
#include "stm32f10x.h"
extern u8  USART_RX_BUF[16]; //接收缓冲
extern u16 USART_RX_STA;         		//接收状态标记	
void USART1_init(u32 bound);//初始化
void USART1_IRQHandler(void);//中断函数
uint8_t get_ok_or_not(void);
uint16_t get_long(void);
void USART1_Send_String(u8 *p,u8 cnt);
void USART1_Send_Byte(uint16_t Data);
#endif

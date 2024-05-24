/*
***************************************************************	
*注意：本程序只供学习使用，未经作者许可，不得用于其它任何用途
*描述：串口打印
*版本：V1.0
*作者：OneTree
*创建日期：2012/11/5
*修改日期：2012/11/5 
*版权所有，盗版必究。
*Copyright(C) OneTree 2012-2022
*All rights reserved
***************************************************************
*硬件连接：	  RXD----------------------PA9
			  TXD----------------------PA10
			
***************************************************************
*修改记录：

	
*/ 
#ifndef __USART_H
#define __USART_H

#include	"stm32f10x.h"
#include	"stdio.h"


#define EN_USART1_RX  	 0
	

void uart_init(u32 bound);
void USART1_printf(USART_TypeDef* USARTx, uint8_t *Data,...);


#endif

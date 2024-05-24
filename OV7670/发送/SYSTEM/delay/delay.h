/*
***************************************************************	
*注意：本程序只供学习使用，未经作者许可，不得用于其它任何用途
*描述：CPU延时
*版本：V1.0
*作者：OneTree
*创建日期：2012/11/5
*修改日期：2012/11/5 
*版权所有，盗版必究。
*Copyright(C) OneTree 2012-2022
*All rights reserved
***************************************************************
*硬件连接：	  			
***************************************************************
*修改记录：

	
*/ 
#ifndef __DELAY_H
#define __DELAY_H 	
		   
#include "stm32f10x.h"


#define	    USE_UCOS_ENABLE	   0			//使用UCOS-II系统
 
#if		USE_UCOS_ENABLE>0

#define		delay_ms(x)    OSTimeDlyHMSM (0, 0, 0,x)  
void delay_us(u32 nus);
void Systick_Init(void);

#else
void delay_init(u8 SYSCLK);
void delay_ms(u16 nms);
void delay_us(u32 nus);
#endif


#endif






























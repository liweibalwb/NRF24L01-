/*
***************************************************************	
*ע�⣺������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
*������CPU��ʱ
*�汾��V1.0
*���ߣ�OneTree
*�������ڣ�2012/11/5
*�޸����ڣ�2012/11/5 
*��Ȩ���У�����ؾ���
*Copyright(C) OneTree 2012-2022
*All rights reserved
***************************************************************
*Ӳ�����ӣ�	  			
***************************************************************
*�޸ļ�¼��

	
*/ 
#ifndef __DELAY_H
#define __DELAY_H 	
		   
#include "stm32f10x.h"


#define	    USE_UCOS_ENABLE	   0			//ʹ��UCOS-IIϵͳ
 
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






























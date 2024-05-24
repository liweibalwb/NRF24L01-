/*
***************************************************************	
*注意：本程序只供学习使用，未经作者许可，不得用于其它任何用途
*描述：蜂鸣器
*版本：V1.0
*作者：OneTree
*创建日期：2012/11/5
*修改日期：2012/11/5 
*版权所有，盗版必究。
*Copyright(C) OneTree 2012-2022
*All rights reserved
***************************************************************
*硬件连接： BUZZER -------------------- PD3

***************************************************************
*修改记录：

	
*/
#ifndef		__BUZZER_H__
#define		__BUZZER_H__

#include	"main.h"

#define		GPIO_BUZZER_CLK		RCC_APB2Periph_GPIOD
#define		GPIO_BUZZER			GPIOD
#define		BUZZER_PIN			GPIO_Pin_3


#define		BUZZER_SET  		(GPIO_BUZZER->BSRR = BUZZER_PIN) 
#define		BUZZER_CLR  		(GPIO_BUZZER->BRR =  BUZZER_PIN) 


void Buzzer_Init(void);

#endif



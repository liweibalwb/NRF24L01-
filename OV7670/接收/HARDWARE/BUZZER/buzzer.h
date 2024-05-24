/*
***************************************************************	
*ע�⣺������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
*������������
*�汾��V1.0
*���ߣ�OneTree
*�������ڣ�2012/11/5
*�޸����ڣ�2012/11/5 
*��Ȩ���У�����ؾ���
*Copyright(C) OneTree 2012-2022
*All rights reserved
***************************************************************
*Ӳ�����ӣ� BUZZER -------------------- PD3

***************************************************************
*�޸ļ�¼��

	
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



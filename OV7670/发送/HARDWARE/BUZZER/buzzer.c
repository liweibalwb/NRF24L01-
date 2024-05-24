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
#include	"buzzer.h"



void Buzzer_Init(void)
{
 
 GPIO_InitTypeDef  GPIO_InitStructure;				   //����GPIO��ʼ���ṹ��
 	
 RCC_APB2PeriphClockCmd(GPIO_BUZZER_CLK, ENABLE);	   //ʹ��PF�˿�ʱ��
	
 GPIO_InitStructure.GPIO_Pin = BUZZER_PIN;			
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		//�������
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		//IO���ٶ�50MHZ

 GPIO_Init(GPIO_BUZZER, &GPIO_InitStructure);					
 GPIO_ResetBits(GPIO_BUZZER,BUZZER_PIN);                  //�����

}






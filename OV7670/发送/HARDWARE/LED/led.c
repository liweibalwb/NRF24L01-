/*
***************************************************************	
*ע�⣺������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
*������LED0��200MS��������˸
*�汾��V1.0
*���ߣ�OneTree
*�������ڣ�2012/11/5
*�޸����ڣ�2012/11/5 
*��Ȩ���У�����ؾ���
*Copyright(C) OneTree 2012-2022
*All rights reserved
***************************************************************
*Ӳ�����ӣ�  LED0 -------------------- PA8

***************************************************************
*�޸ļ�¼��

	
*/
#include	"led.h"


/*
***************************************************************
*��������   LED_Init
*���ܣ�		��ʼ��LED0��IO��
*������		��
*����ֵ��	��
*ע�⣺		
*�����÷���
****************************************************************
*/	
void LED_Init(void)
{
 
 GPIO_InitTypeDef  GPIO_InitStructure;					 //����GPIO��ʼ���ṹ��
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	 //ʹ��PA�˿�ʱ��
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;			
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		//IO���ٶ�50MHZ
 GPIO_Init(GPIOA, &GPIO_InitStructure);					
 GPIO_SetBits(GPIOA,GPIO_Pin_8); //�����

}
 




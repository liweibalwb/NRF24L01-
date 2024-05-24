/*
***************************************************************	
*ע�⣺������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
*��������������
*�汾��V1.0
*���ߣ�OneTree
*�������ڣ�2012/11/5
*�޸����ڣ�2012/11/5 
*��Ȩ���У�����ؾ���
*Copyright(C) OneTree 2012-2022
*All rights reserved
***************************************************************
*Ӳ�����ӣ� WK_UP -------------------- PA0
			KEY0  -------------------- PE5
			KEY1  -------------------- PE6

***************************************************************
*�޸ļ�¼��

	
*/
#include	"key.h"



void KEY_Init(void) //IO��ʼ��
{ 
 	GPIO_InitTypeDef GPIO_InitStructure;

		 
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE,ENABLE);
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_5 | GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
 	GPIO_Init(GPIOE, &GPIO_InitStructure);


}

u8 KEY_Scan(void)
{	 
	static u8 key_up=1;								//�������ɿ���־	
	if(key_up&&(KEY0==0||KEY1==0))
	{
		delay_ms(10);								//ȥ���� 
		key_up=0;
	    if(KEY0==0)
		{ 
			return 1;
		}
		else if(KEY1==0)
		{
			return 2;
		}
	}else if(KEY0&&KEY1)key_up=1; 	    
 
	return 0;// �ް�������
}


/*
***************************************************************	
*注意：本程序只供学习使用，未经作者许可，不得用于其它任何用途
*描述：独立按键
*版本：V1.0
*作者：OneTree
*创建日期：2012/11/5
*修改日期：2012/11/5 
*版权所有，盗版必究。
*Copyright(C) OneTree 2012-2022
*All rights reserved
***************************************************************
*硬件连接： WK_UP -------------------- PA0
			KEY0  -------------------- PE5
			KEY1  -------------------- PE6

***************************************************************
*修改记录：

	
*/
#include	"key.h"



void KEY_Init(void) //IO初始化
{ 
 	GPIO_InitTypeDef GPIO_InitStructure;

		 
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE,ENABLE);
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_5 | GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
 	GPIO_Init(GPIOE, &GPIO_InitStructure);


}

u8 KEY_Scan(void)
{	 
	static u8 key_up=1;								//按键按松开标志	
	if(key_up&&(KEY0==0||KEY1==0))
	{
		delay_ms(10);								//去抖动 
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
 
	return 0;// 无按键按下
}


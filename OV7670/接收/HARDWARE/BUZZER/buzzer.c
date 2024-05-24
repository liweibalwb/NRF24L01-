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
#include	"buzzer.h"



void Buzzer_Init(void)
{
 
 GPIO_InitTypeDef  GPIO_InitStructure;				   //定义GPIO初始化结构体
 	
 RCC_APB2PeriphClockCmd(GPIO_BUZZER_CLK, ENABLE);	   //使能PF端口时钟
	
 GPIO_InitStructure.GPIO_Pin = BUZZER_PIN;			
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		//推完输出
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		//IO口速度50MHZ

 GPIO_Init(GPIO_BUZZER, &GPIO_InitStructure);					
 GPIO_ResetBits(GPIO_BUZZER,BUZZER_PIN);                  //输出低

}






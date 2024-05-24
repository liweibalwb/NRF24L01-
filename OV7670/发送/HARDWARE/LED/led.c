/*
***************************************************************	
*注意：本程序只供学习使用，未经作者许可，不得用于其它任何用途
*描述：LED0以200MS的周期闪烁
*版本：V1.0
*作者：OneTree
*创建日期：2012/11/5
*修改日期：2012/11/5 
*版权所有，盗版必究。
*Copyright(C) OneTree 2012-2022
*All rights reserved
***************************************************************
*硬件连接：  LED0 -------------------- PA8

***************************************************************
*修改记录：

	
*/
#include	"led.h"


/*
***************************************************************
*函数名：   LED_Init
*功能：		初始化LED0的IO口
*参数：		无
*返回值：	无
*注意：		
*典型用法：
****************************************************************
*/	
void LED_Init(void)
{
 
 GPIO_InitTypeDef  GPIO_InitStructure;					 //定义GPIO初始化结构体
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	 //使能PA端口时钟
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;			
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		//IO口速度50MHZ
 GPIO_Init(GPIOA, &GPIO_InitStructure);					
 GPIO_SetBits(GPIOA,GPIO_Pin_8); //输出高

}
 




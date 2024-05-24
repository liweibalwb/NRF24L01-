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
*硬件连接： WK_UP -------------------- PA0		硬件有问题，这里暂不使用
			KEY0  -------------------- PE5
			KEY1  -------------------- PE6

***************************************************************
*修改记录：

	
*/
#ifndef		__KEY_H__
#define		__KEY_H__

#include	"stm32f10x.h"
#include	"delay.h"


#define 	KEY0 	GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_5)	
#define 	KEY1 	GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_6)	

void KEY_Init(void);//IO初始化
u8   KEY_Scan(void);  //按键扫描函数	


#endif




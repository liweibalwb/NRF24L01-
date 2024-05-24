/*
***************************************************************	
*注意：本程序只供学习使用，未经作者许可，不得用于其它任何用途
*描述：I2C 协议
*版本：V1.0
*作者：OneTree
*创建日期：2012/11/5
*修改日期：2012/11/5 
*版权所有，盗版必究。
*Copyright(C) OneTree 2012-2022
*All rights reserved
***************************************************************
*硬件连接：  SCL -------------------- PG14
			 SDA -------------------- PG15

***************************************************************
*修改记录：

	
*/
#ifndef __MYIIC_H
#define __MYIIC_H

#include	"main.h"  

   	   		   
//IO方向设置
#define SDA_IN()  {GPIOG->CRH&=0X0FFFFFFF;GPIOG->CRH|=(u32)8<<28;}
#define SDA_OUT() {GPIOG->CRH&=0X0FFFFFFF;GPIOG->CRH|=(u32)3<<28;}

//IO操作函数	 
#define IIC_SCL_SET    (GPIOG->BSRR = GPIO_Pin_14) //SCL
#define IIC_SCL_CLR    (GPIOG->BRR = GPIO_Pin_14) //SCL

#define IIC_SDA_SET    (GPIOG->BSRR = GPIO_Pin_15) //SDA
#define IIC_SDA_CLR    (GPIOG->BRR = GPIO_Pin_15) //SDA

	 
#define READ_SDA   GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_15)  //输入SDA 

//IIC所有操作函数
void IIC_Init(void);                //初始化IIC的IO口				 
void IIC_Start(void);				//发送IIC开始信号
void IIC_Stop(void);	  			//发送IIC停止信号
void IIC_Send_Byte(u8 txd);			//IIC发送一个字节
u8 IIC_Read_Byte(unsigned char ack);//IIC读取一个字节
u8 IIC_Wait_Ack(void); 				//IIC等待ACK信号
void IIC_Ack(void);					//IIC发送ACK信号
void IIC_NAck(void);				//IIC不发送ACK信号

void IIC_Write_One_Byte(u8 daddr,u8 addr,u8 data);
u8 IIC_Read_One_Byte(u8 daddr,u8 addr);	  
#endif

















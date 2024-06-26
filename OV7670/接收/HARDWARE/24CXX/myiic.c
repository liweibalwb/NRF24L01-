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
#include "myiic.h"
#include "delay.h"
 

//初始化IIC
void IIC_Init(void)
{					     
 GPIO_InitTypeDef  GPIO_InitStructure;					 //定义GPIO初始化结构体
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG, ENABLE);	 //使能PB端口时钟
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14|GPIO_Pin_15;			
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度50MHZ
 GPIO_Init(GPIOG, &GPIO_InitStructure);					
 GPIO_SetBits(GPIOG,GPIO_Pin_14|GPIO_Pin_15); 			 //输出高
}
//产生IIC起始信号
void IIC_Start(void)
{
	SDA_OUT();     //sda线输出
	IIC_SDA_SET;	  	  
	IIC_SCL_SET;
	delay_us(4);
 	IIC_SDA_CLR;//START:when CLK is high,DATA change form high to low 
	delay_us(4);
	IIC_SCL_CLR;//钳住I2C总线，准备发送或接收数据 
}	  
//产生IIC停止信号
void IIC_Stop(void)
{
	SDA_OUT();//sda线输出
	IIC_SCL_CLR;
	IIC_SDA_CLR;//STOP:when CLK is high DATA change form low to high
 	delay_us(4);
	IIC_SCL_SET; 
	IIC_SDA_SET;//发送I2C总线结束信号
	delay_us(4);							   	
}
//等待应答信号到来
//返回值：1，接收应答失败
//        0，接收应答成功
u8 IIC_Wait_Ack(void)
{
	u8 ucErrTime=0;
	SDA_IN();      //SDA设置为输入  
	IIC_SDA_SET;delay_us(1);	   
	IIC_SCL_SET;delay_us(1);	 
	while(READ_SDA)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			IIC_Stop();
			return 1;
		}
	}
	IIC_SCL_CLR;//时钟输出0 	   
	return 0;  
} 
//产生ACK应答
void IIC_Ack(void)
{
	IIC_SCL_CLR;
	SDA_OUT();
	IIC_SDA_CLR;
	delay_us(2);
	IIC_SCL_SET;
	delay_us(2);
	IIC_SCL_CLR;
}
//不产生ACK应答		    
void IIC_NAck(void)
{
	IIC_SCL_CLR;
	SDA_OUT();
	IIC_SDA_SET;
	delay_us(2);
	IIC_SCL_SET;
	delay_us(2);
	IIC_SCL_CLR;
}					 				     
//IIC发送一个字节
//返回从机有无应答
//1，有应答
//0，无应答			  
void IIC_Send_Byte(u8 txd)
{                        
    u8 t;   
	SDA_OUT(); 	    
    IIC_SCL_CLR;//拉低时钟开始数据传输
    for(t=0;t<8;t++)
    {              
		if(txd&0x80)
			IIC_SDA_SET;
		else
			IIC_SDA_CLR;	
        txd<<=1; 	  
		delay_us(2);   //对TEA5767这三个延时都是必须的
		IIC_SCL_SET;
		delay_us(2); 
		IIC_SCL_CLR;	
		delay_us(2);
    }	 
} 	    
//读1个字节，ack=1时，发送ACK，ack=0，发送nACK   
u8 IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	SDA_IN();//SDA设置为输入
    for(i=0;i<8;i++ )
	{
        IIC_SCL_CLR; 
        delay_us(2);
		IIC_SCL_SET;
        receive<<=1;
        if(READ_SDA)receive++;   
		delay_us(1); 
    }					 
    if (!ack)
        IIC_NAck();//发送nACK
    else
        IIC_Ack(); //发送ACK   
    return receive;
}




























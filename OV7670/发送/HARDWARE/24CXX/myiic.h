/*
***************************************************************	
*ע�⣺������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
*������I2C Э��
*�汾��V1.0
*���ߣ�OneTree
*�������ڣ�2012/11/5
*�޸����ڣ�2012/11/5 
*��Ȩ���У�����ؾ���
*Copyright(C) OneTree 2012-2022
*All rights reserved
***************************************************************
*Ӳ�����ӣ�  SCL -------------------- PG14
			 SDA -------------------- PG15

***************************************************************
*�޸ļ�¼��

	
*/
#ifndef __MYIIC_H
#define __MYIIC_H

#include	"main.h"  

   	   		   
//IO��������
#define SDA_IN()  {GPIOG->CRH&=0X0FFFFFFF;GPIOG->CRH|=(u32)8<<28;}
#define SDA_OUT() {GPIOG->CRH&=0X0FFFFFFF;GPIOG->CRH|=(u32)3<<28;}

//IO��������	 
#define IIC_SCL_SET    (GPIOG->BSRR = GPIO_Pin_14) //SCL
#define IIC_SCL_CLR    (GPIOG->BRR = GPIO_Pin_14) //SCL

#define IIC_SDA_SET    (GPIOG->BSRR = GPIO_Pin_15) //SDA
#define IIC_SDA_CLR    (GPIOG->BRR = GPIO_Pin_15) //SDA

	 
#define READ_SDA   GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_15)  //����SDA 

//IIC���в�������
void IIC_Init(void);                //��ʼ��IIC��IO��				 
void IIC_Start(void);				//����IIC��ʼ�ź�
void IIC_Stop(void);	  			//����IICֹͣ�ź�
void IIC_Send_Byte(u8 txd);			//IIC����һ���ֽ�
u8 IIC_Read_Byte(unsigned char ack);//IIC��ȡһ���ֽ�
u8 IIC_Wait_Ack(void); 				//IIC�ȴ�ACK�ź�
void IIC_Ack(void);					//IIC����ACK�ź�
void IIC_NAck(void);				//IIC������ACK�ź�

void IIC_Write_One_Byte(u8 daddr,u8 addr,u8 data);
u8 IIC_Read_One_Byte(u8 daddr,u8 addr);	  
#endif

















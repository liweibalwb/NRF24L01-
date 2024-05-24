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
#include "myiic.h"
#include "delay.h"
 

//��ʼ��IIC
void IIC_Init(void)
{					     
 GPIO_InitTypeDef  GPIO_InitStructure;					 //����GPIO��ʼ���ṹ��
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG, ENABLE);	 //ʹ��PB�˿�ʱ��
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14|GPIO_Pin_15;			
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�50MHZ
 GPIO_Init(GPIOG, &GPIO_InitStructure);					
 GPIO_SetBits(GPIOG,GPIO_Pin_14|GPIO_Pin_15); 			 //�����
}
//����IIC��ʼ�ź�
void IIC_Start(void)
{
	SDA_OUT();     //sda�����
	IIC_SDA_SET;	  	  
	IIC_SCL_SET;
	delay_us(4);
 	IIC_SDA_CLR;//START:when CLK is high,DATA change form high to low 
	delay_us(4);
	IIC_SCL_CLR;//ǯסI2C���ߣ�׼�����ͻ�������� 
}	  
//����IICֹͣ�ź�
void IIC_Stop(void)
{
	SDA_OUT();//sda�����
	IIC_SCL_CLR;
	IIC_SDA_CLR;//STOP:when CLK is high DATA change form low to high
 	delay_us(4);
	IIC_SCL_SET; 
	IIC_SDA_SET;//����I2C���߽����ź�
	delay_us(4);							   	
}
//�ȴ�Ӧ���źŵ���
//����ֵ��1������Ӧ��ʧ��
//        0������Ӧ��ɹ�
u8 IIC_Wait_Ack(void)
{
	u8 ucErrTime=0;
	SDA_IN();      //SDA����Ϊ����  
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
	IIC_SCL_CLR;//ʱ�����0 	   
	return 0;  
} 
//����ACKӦ��
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
//������ACKӦ��		    
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
//IIC����һ���ֽ�
//���شӻ�����Ӧ��
//1����Ӧ��
//0����Ӧ��			  
void IIC_Send_Byte(u8 txd)
{                        
    u8 t;   
	SDA_OUT(); 	    
    IIC_SCL_CLR;//����ʱ�ӿ�ʼ���ݴ���
    for(t=0;t<8;t++)
    {              
		if(txd&0x80)
			IIC_SDA_SET;
		else
			IIC_SDA_CLR;	
        txd<<=1; 	  
		delay_us(2);   //��TEA5767��������ʱ���Ǳ����
		IIC_SCL_SET;
		delay_us(2); 
		IIC_SCL_CLR;	
		delay_us(2);
    }	 
} 	    
//��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK   
u8 IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	SDA_IN();//SDA����Ϊ����
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
        IIC_NAck();//����nACK
    else
        IIC_Ack(); //����ACK   
    return receive;
}




























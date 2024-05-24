/*
***************************************************************	
*ע�⣺������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
*������SPIЭ�飬����SPI1��SPI2
*�汾��V1.0
*���ߣ�OneTree
*�������ڣ�2012/11/5
*�޸����ڣ�2012/11/5 
*��Ȩ���У�����ؾ���
*Copyright(C) OneTree 2012-2022
*All rights reserved
***************************************************************
*Ӳ�����ӣ� 
			SPI1_SCK---------------------PA5 
			SPI1_MISO--------------------PA6 
			SPI1_MOSI--------------------PA7 

			SPI2_SCK---------------------PB13 
			SPI2_MISO--------------------PB14 
			SPI2_MOSI--------------------PB15 
***************************************************************
*�޸ļ�¼��

	
*/
#include 	"spi.h"

SPI_InitTypeDef  SPI_InitStructure;	  		//�ڵ���SRAMʱ�����ں����л����Ӳ�����󣬹����Ƕ�ջ��������Խ��������Ϊȫ�ֱ���	   
//SPI2�ĳ�ʼ�� 
void SPI1_Init(void)
{

	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOA | RCC_APB2Periph_SPI1, ENABLE );       //PORTA��SPI1ʱ��ʹ�� 
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;             //�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

 	GPIO_SetBits(GPIOA,GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7);
 
 
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //����SPI�������˫�������ģʽ:SPI����Ϊ˫��˫��ȫ˫��
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		//����SPI����ģʽ:����Ϊ��SPI
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;		//����SPI�����ݴ�С:SPI���ͽ���8λ֡�ṹ
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;		//ѡ���˴���ʱ�ӵ���̬:ʱ�����ո�
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;	//���ݲ����ڵڶ���ʱ����
	
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		//NSS�ź���Ӳ����NSS�ܽţ����������ʹ��SSIλ������:�ڲ�NSS�ź���SSIλ����
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;		//���岨����Ԥ��Ƶ��ֵ:������Ԥ��ƵֵΪ256
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	//ָ�����ݴ����MSBλ����LSBλ��ʼ:���ݴ����MSBλ��ʼ
	SPI_InitStructure.SPI_CRCPolynomial = 7;	//CRCֵ����Ķ���ʽ
	SPI_Init(SPI1, &SPI_InitStructure);  //����SPI_InitStruct��ָ���Ĳ�����ʼ������SPIx�Ĵ���
 
	SPI_Cmd(SPI1, ENABLE); //ʹ��SPI����
	
	SPI1_ReadWriteByte(0xff);//��������		 
}  


// SPI_SPEED_2   Fsck=Fpclk/2=36Mhz
// SPI_SPEED_4   Fsck=Fpclk/4=18Mhz
// SPI_SPEED_8   Fsck=Fpclk/8=9Mhz
// SPI_SPEED_16  Fsck=Fpclk/16=4.5Mhz
// SPI_SPEED_32  Fsck=Fpclk/32=2.25Mhz
// SPI_SPEED_64  Fsck=Fpclk/64=1.125Mhz
// SPI_SPEED_128 Fsck=Fpclk/128=562.5Khz
// SPI_SPEED_256 Fsck=Fpclk/256=281.25Khz
void SPI1_SetSpeed(u8 SpeedSet)
{
	SPI_InitStructure.SPI_BaudRatePrescaler = SpeedSet ;
  SPI_Init(SPI1, &SPI_InitStructure);
	SPI_Cmd(SPI1,ENABLE);
} 

//SPI1 ��д
u8 SPI1_ReadWriteByte(u8 TxData)
{						 
	while((SPI1->SR&1<<1)==0);//�ȴ���������				  
	SPI1->DR=TxData;	 	  //����һ��byte 
	while((SPI1->SR&1<<0)==0); //�ȴ�������һ��byte    						    
	return SPI1->DR;          //�����յ�������				    
}




//SPI2�ĳ�ʼ��
void SPI2_Init(void)
{

	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOB, ENABLE );       //PORTBʱ��ʹ�� 
	RCC_APB1PeriphClockCmd(	RCC_APB1Periph_SPI2,  ENABLE );      //SPI2ʱ��ʹ��
 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;             //�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

 	GPIO_SetBits(GPIOB,GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15);
 
 
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //����SPI�������˫�������ģʽ:SPI����Ϊ˫��˫��ȫ˫��
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		//����SPI����ģʽ:����Ϊ��SPI
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;		//����SPI�����ݴ�С:SPI���ͽ���8λ֡�ṹ
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;		//ѡ���˴���ʱ�ӵ���̬:ʱ�����ո�
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;	//���ݲ����ڵڶ���ʱ����
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		//NSS�ź���Ӳ����NSS�ܽţ����������ʹ��SSIλ������:�ڲ�NSS�ź���SSIλ����
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;		//���岨����Ԥ��Ƶ��ֵ:������Ԥ��ƵֵΪ256
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	//ָ�����ݴ����MSBλ����LSBλ��ʼ:���ݴ����MSBλ��ʼ
	SPI_InitStructure.SPI_CRCPolynomial = 7;	//CRCֵ����Ķ���ʽ
	SPI_Init(SPI2, &SPI_InitStructure);  //����SPI_InitStruct��ָ���Ĳ�����ʼ������SPIx�Ĵ���
 
	SPI_Cmd(SPI2, ENABLE); //ʹ��SPI����
	
	SPI2_ReadWriteByte(0xff);//��������		 
}  


// SPI_SPEED_2   Fsck=Fpclk/2=36Mhz
// SPI_SPEED_4   Fsck=Fpclk/4=18Mhz
// SPI_SPEED_8   Fsck=Fpclk/8=9Mhz
// SPI_SPEED_16  Fsck=Fpclk/16=4.5Mhz
// SPI_SPEED_32  Fsck=Fpclk/32=2.25Mhz
// SPI_SPEED_64  Fsck=Fpclk/64=1.125Mhz
// SPI_SPEED_128 Fsck=Fpclk/128=562.5Khz
// SPI_SPEED_256 Fsck=Fpclk/256=281.25Khz
void SPI2_SetSpeed(u8 SpeedSet)
{
	SPI_InitStructure.SPI_BaudRatePrescaler = SpeedSet ;
  	SPI_Init(SPI2, &SPI_InitStructure);
	SPI_Cmd(SPI2,ENABLE);
} 

//SPI2 ��д
u8 SPI2_ReadWriteByte(u8 TxData)
{						 
	while((SPI2->SR&1<<1)==0);//�ȴ���������				  
	SPI2->DR=TxData;	 	  //����һ��byte 
	while((SPI2->SR&1<<0)==0); //�ȴ�������һ��byte    						    
	return SPI2->DR;          //�����յ�������				    
}



























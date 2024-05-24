/*
***************************************************************	
*注意：本程序只供学习使用，未经作者许可，不得用于其它任何用途
*描述：SPI协议，包含SPI1和SPI2
*版本：V1.0
*作者：OneTree
*创建日期：2012/11/5
*修改日期：2012/11/5 
*版权所有，盗版必究。
*Copyright(C) OneTree 2012-2022
*All rights reserved
***************************************************************
*硬件连接： 
			SPI1_SCK---------------------PA5 
			SPI1_MISO--------------------PA6 
			SPI1_MOSI--------------------PA7 

			SPI2_SCK---------------------PB13 
			SPI2_MISO--------------------PB14 
			SPI2_MOSI--------------------PB15 
***************************************************************
*修改记录：

	
*/
#include 	"spi.h"

SPI_InitTypeDef  SPI_InitStructure;	  		//在调试SRAM时，放在函数中会出现硬件错误，估计是堆栈溢出，所以将这个定义为全局变量	   
//SPI2的初始化 
void SPI1_Init(void)
{

	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOA | RCC_APB2Periph_SPI1, ENABLE );       //PORTA和SPI1时钟使能 
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;             //复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

 	GPIO_SetBits(GPIOA,GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7);
 
 
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //设置SPI单向或者双向的数据模式:SPI设置为双线双向全双工
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		//设置SPI工作模式:设置为主SPI
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;		//设置SPI的数据大小:SPI发送接收8位帧结构
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;		//选择了串行时钟的稳态:时钟悬空高
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;	//数据捕获于第二个时钟沿
	
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		//NSS信号由硬件（NSS管脚）还是软件（使用SSI位）管理:内部NSS信号有SSI位控制
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;		//定义波特率预分频的值:波特率预分频值为256
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	//指定数据传输从MSB位还是LSB位开始:数据传输从MSB位开始
	SPI_InitStructure.SPI_CRCPolynomial = 7;	//CRC值计算的多项式
	SPI_Init(SPI1, &SPI_InitStructure);  //根据SPI_InitStruct中指定的参数初始化外设SPIx寄存器
 
	SPI_Cmd(SPI1, ENABLE); //使能SPI外设
	
	SPI1_ReadWriteByte(0xff);//启动传输		 
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

//SPI1 读写
u8 SPI1_ReadWriteByte(u8 TxData)
{						 
	while((SPI1->SR&1<<1)==0);//等待发送区空				  
	SPI1->DR=TxData;	 	  //发送一个byte 
	while((SPI1->SR&1<<0)==0); //等待接收完一个byte    						    
	return SPI1->DR;          //返回收到的数据				    
}




//SPI2的初始化
void SPI2_Init(void)
{

	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOB, ENABLE );       //PORTB时钟使能 
	RCC_APB1PeriphClockCmd(	RCC_APB1Periph_SPI2,  ENABLE );      //SPI2时钟使能
 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;             //复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

 	GPIO_SetBits(GPIOB,GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15);
 
 
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //设置SPI单向或者双向的数据模式:SPI设置为双线双向全双工
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		//设置SPI工作模式:设置为主SPI
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;		//设置SPI的数据大小:SPI发送接收8位帧结构
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;		//选择了串行时钟的稳态:时钟悬空高
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;	//数据捕获于第二个时钟沿
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		//NSS信号由硬件（NSS管脚）还是软件（使用SSI位）管理:内部NSS信号有SSI位控制
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;		//定义波特率预分频的值:波特率预分频值为256
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	//指定数据传输从MSB位还是LSB位开始:数据传输从MSB位开始
	SPI_InitStructure.SPI_CRCPolynomial = 7;	//CRC值计算的多项式
	SPI_Init(SPI2, &SPI_InitStructure);  //根据SPI_InitStruct中指定的参数初始化外设SPIx寄存器
 
	SPI_Cmd(SPI2, ENABLE); //使能SPI外设
	
	SPI2_ReadWriteByte(0xff);//启动传输		 
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

//SPI2 读写
u8 SPI2_ReadWriteByte(u8 TxData)
{						 
	while((SPI2->SR&1<<1)==0);//等待发送区空				  
	SPI2->DR=TxData;	 	  //发送一个byte 
	while((SPI2->SR&1<<0)==0); //等待接收完一个byte    						    
	return SPI2->DR;          //返回收到的数据				    
}



























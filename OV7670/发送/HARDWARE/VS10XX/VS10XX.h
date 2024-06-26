#ifndef __VS10XX_H__
#define __VS10XX_H__

#include	"main.h"
		    
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//Mini STM32开发板
//VS10XX  驱动代码		   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2011/6/18
//版本：V1.3
//版权所有，盗版必究。
//Copyright(C) 正点原子 2009-2019
//All rights reserved
//********************************************************************************
//V1.2
//1,增加VS1053的FLAC播放支持.
//2,修改GetHeadInfo函数.    
//V1.3修改
//1,规范各个函数接口
//2,修正1.2版本在VS1053模块上无法播放WMA,WAV的错误.
//3,加入芯片自动识别
//////////////////////////////////////////////////////////////////////////////////	 
//VS10XX的XX(ID列表)
#define	 VS1001 	0
#define	 VS1011 	1
#define	 VS1002 	2
#define	 VS1003 	3
#define	 VS1053 	4
#define	 VS1033 	5
#define	 VS1103 	7
extern u8 VS10XX_ID;	  		  //VS10XX的真实型号
//命令					  
#define VS_WRITE_COMMAND 	0x02  //写命令
#define VS_READ_COMMAND 	0x03  //读命令
//VS10XX寄存器定义
#define SPI_MODE        	0x00  //模式控制   
#define SPI_STATUS      	0x01  //VS10XX的状态寄存器 
#define SPI_BASS        	0x02  //高低音控制 
#define SPI_CLOCKF      	0x03  //时钟频率倍乘寄存器 
#define SPI_DECODE_TIME 	0x04  //解码时间长度 
#define SPI_AUDATA      	0x05  //各种音频数据 
#define SPI_WRAM        	0x06  //RAM 写/读 
#define SPI_WRAMADDR    	0x07  //RAM 写/读的起始地址 
#define SPI_HDAT0       	0x08  //流数据标头0 
#define SPI_HDAT1       	0x09  //流数据标头1	 
   
#define SPI_AIADDR      	0x0a  //应用程序的起始地址 
#define SPI_VOL         	0x0b  //音量控制 
#define SPI_AICTRL0     	0x0c  //应用程序控制寄存器0 
#define SPI_AICTRL1     	0x0d  //应用程序控制寄存器1 
#define SPI_AICTRL2     	0x0e  //应用程序控制寄存器2 
#define SPI_AICTRL3     	0x0f  //应用程序控制寄存器3
//SPI_MODE的各位功能,下表根据VS1053的手册标注
#define SM_DIFF         	0x01  //差分 
#define SM_LAYER12         	0x02  //允许MPEG 1,2解码 FOR vs1053 
#define SM_RESET        	0x04  //软件复位 
#define SM_CANCEL       	0x08  //取消当前解码 
#define SM_EARSPEAKER_LO  	0x10  //EarSpeaker低设定 
#define SM_TESTS        	0x20  //允许SDI测试 
#define SM_STREAM       	0x40  //流模式 
#define SM_EARSPEAKER_HI   	0x80  //EarSpeaker高设定  
#define SM_DACT         	0x100 //DCLK的有效边沿  
#define SM_SDIORD       	0x200 //SDI位顺序  
#define SM_SDISHARE     	0x400 //共享SPI片选  
#define SM_SDINEW       	0x800 //VS1002 本地SPI模式  
#define SM_ADPCM        	0x1000//ADPCM录音激活   
#define SM_LINE1         	0x4000//咪/线路1 选择 		 
#define SM_CLK_RANGE     	0x8000//输入时钟范围 	
	 
//VS10xx与外部接口定义(未包含MISO,MOSI,SCK)
#define VS_DQ_SET      (GPIOB->BSRR = GPIO_Pin_7) 		//DREQ 	  PB7
#define VS_DQ_CLR      (GPIOB->BRR = GPIO_Pin_7) 		//DREQ 
#define VS_DQ_READ	   GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_7)

#define VS_RST_SET      (GPIOG->BSRR = GPIO_Pin_11)  		//RST	 PG11
#define VS_RST_CLR      (GPIOG->BRR = GPIO_Pin_11)  		//RST

#define VS_XCS_SET      (GPIOB->BSRR = GPIO_Pin_5)   		//XCS  PB5
#define VS_XCS_CLR     (GPIOB->BRR = GPIO_Pin_5)   		//XCS


#define VS_XDCS_SET     (GPIOB->BSRR = GPIO_Pin_6)  		//XDCS 		PB6
#define VS_XDCS_CLR    (GPIOB->BRR = GPIO_Pin_6)  		//XDCS 

							 
u16  VS_RD_Reg(u8 address);			//读寄存器
void VS_WR_Cmd(u8 address,u16 data);//写指令
void VS_WR_Data(u8 data);			//写数据  
u16  VS_RD_Wram(u16 addr);			//读WRAM	
  
void VS_Init(void);					//初始化VS10XX	  			    
u8   VS_HD_Reset(void);				//硬复位
void VS_Soft_Reset(void);     		//软复位   
u16  VS_Ram_Test(void);           	//RAM测试
void VS_Sine_Test(void);          	//正弦测试	  
u16  VS_Get_DecodeTime(void);   	//得到解码时间
u16  VS_Get_HeadInfo(void);     	//得到比特率
void VS_Rst_DecodeTime(void);		//重设解码时间
												    
void set10XX(void);        			//设置VS10XX
void VS_Load_FlacPatch(void);  		//加载FLAC解码代码 	 

#endif





















			  

















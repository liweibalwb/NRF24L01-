/*
***************************************************************	
*注意：本程序只供学习使用，未经作者许可，不得用于其它任何用途
*描述：SPI_FLASH W25X16
*版本：V1.0
*作者：OneTree
*创建日期：2012/11/5
*修改日期：2012/11/5 
*版权所有，盗版必究。
*Copyright(C) OneTree 2012-2022
*All rights reserved
***************************************************************
*硬件连接： 
			W_CS-------------------------PB12
			SPI2_SCK---------------------PB13 
			SPI2_MISO--------------------PB14 
			SPI2_MOSI--------------------PB15 
***************************************************************
*修改记录：

	
*/
#ifndef __FLASH_H
#define __FLASH_H			    

#include	"main.h"  


#define	SPI_FLASH_CS_SET (GPIOB->BSRR = GPIO_Pin_12)  //选中FLASH  PB12
#define	SPI_FLASH_CS_CLR (GPIOB->BRR = GPIO_Pin_12)  //选中FLASH	



#define FLASHE_WRITE_EN        //允许写操作				 
////////////////////////////////////////////////////////////////////////////
//W25X16读写
#define FLASH_ID 0XEF14
//指令表
#define W25X_WriteEnable		0x06 
#define W25X_WriteDisable		0x04 
#define W25X_ReadStatusReg		0x05 
#define W25X_WriteStatusReg		0x01 
#define W25X_ReadData			0x03 
#define W25X_FastReadData		0x0B 
#define W25X_FastReadDual		0x3B 
#define W25X_PageProgram		0x02 
#define W25X_BlockErase			0xD8 
#define W25X_SectorErase		0x20 
#define W25X_ChipErase			0xC7 
#define W25X_PowerDown			0xB9 
#define W25X_ReleasePowerDown	0xAB 
#define W25X_DeviceID			0xAB 
#define W25X_ManufactDeviceID	0x90 
#define W25X_JedecDeviceID		0x9F 

void SPI_Flash_Init(void);
u16  SPI_Flash_ReadID(void);  	    //读取FLASH ID
u8	 SPI_Flash_ReadSR(void);        //读取状态寄存器 
void SPI_FLASH_Write_SR(u8 sr);  	//写状态寄存器
void SPI_FLASH_Write_Enable(void);  //写使能 
void SPI_FLASH_Write_Disable(void);	//写保护
void SPI_Flash_Read(u8* pBuffer,u32 ReadAddr,u16 NumByteToRead);   //读取flash
void SPI_Flash_Write(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite);//写入flash
void SPI_Flash_Erase_Chip(void);    	  //整片擦除
void SPI_Flash_Erase_Sector(u32 Dst_Addr);//扇区擦除
void SPI_Flash_Wait_Busy(void);           //等待空闲
void SPI_Flash_PowerDown(void);           //进入掉电模式
void SPI_Flash_WAKEUP(void);			  //唤醒


#endif

















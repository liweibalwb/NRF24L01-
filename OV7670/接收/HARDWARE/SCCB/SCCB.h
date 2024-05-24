/****************************************Copyright (c)**************************************************                         
**
**                                 http://www.powermcu.com
**
**--------------File Info-------------------------------------------------------------------------------
** File name:			SCCB.h
** Descriptions:		SCCB ²Ù×÷º¯Êý¿â 
**
**------------------------------------------------------------------------------------------------------
** Created by:			AVRman
** Created date:		2011-2-13
** Version:				1.0
** Descriptions:		The original version
**
**------------------------------------------------------------------------------------------------------
** Modified by:			
** Modified date:	
** Version:
** Descriptions:		
********************************************************************************************************/
#ifndef __SCCB_H
#define __SCCB_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"

/* Private define ------------------------------------------------------------*/
#define SCCB_SCL_H         GPIOB->BSRR = GPIO_Pin_8	 /* GPIO_SetBits(GPIOB , GPIO_Pin_10)   */
#define SCCB_SCL_L         GPIOB->BRR  = GPIO_Pin_8  /* GPIO_ResetBits(GPIOB , GPIO_Pin_10) */
   
#define SCCB_SDA_H         GPIOB->BSRR = GPIO_Pin_9	 /* GPIO_SetBits(GPIOB , GPIO_Pin_11)   */
#define SCCB_SDA_L         GPIOB->BRR  = GPIO_Pin_9	 /* GPIO_ResetBits(GPIOB , GPIO_Pin_11) */

#define SCCB_SCL_read      GPIOB->IDR  & GPIO_Pin_8	 /* GPIO_ReadInputDataBit(GPIOB , GPIO_Pin_10) */
#define SCCB_SDA_read      GPIOB->IDR  & GPIO_Pin_9	 /* GPIO_ReadInputDataBit(GPIOB , GPIO_Pin_11) */


#define SCCB_READ_SDA   SCCB_SDA_read   
#define SCCB_ID   			0X42  			//OV7670µÄID

#define SCCB_SDA_IN()  {GPIOB->CRH&=0XFFFFFF0F;GPIOB->CRH|=0X00000080;}
#define SCCB_SDA_OUT() {GPIOB->CRH&=0XFFFFFF0F;GPIOB->CRH|=0X00000030;}

///////////////////////////////////////////
void SCCB_Init(void);
void SCCB_Start(void);
void SCCB_Stop(void);
void SCCB_No_Ack(void);
u8 SCCB_WR_Byte(u8 dat);
u8 SCCB_RD_Byte(void);
u8 SCCB_WR_Reg(u8 reg,u8 data);
u8 SCCB_RD_Reg(u8 reg);

#endif 
/*********************************************************************************************************
      END FILE
*********************************************************************************************************/

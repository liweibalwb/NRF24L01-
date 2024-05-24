/****************************************Copyright (c)****************************************************
**                                      
**                                 http://www.powermcu.com
**
**--------------File Info---------------------------------------------------------------------------------
** File name:               OV7670.h
** Descriptions:            None
**
**--------------------------------------------------------------------------------------------------------
** Created by:              AVRman
** Created date:            2011-2-13
** Version:                 v1.0
** Descriptions:            The original version
**
**--------------------------------------------------------------------------------------------------------
** Modified by:             
** Modified date:           
** Version:                 
** Descriptions:            
**
*********************************************************************************************************/

#ifndef __OV7670_H
#define __OV7670_H 

/* Includes ------------------------------------------------------------------*/	   
#include "stm32f10x.h"

/*------------------------------------------------------
  ģ���������� | ����            |     STM32���������� |
  ------------------------------------------------------
  SCCB_SCL     : SCCBʱ��        : PB8    I2C2_SCL
  SCCB_SDA     : SCCB����        : PB9    I2C2_SDA
  CAM_VSYNC    : ֡ͬ��          : PB10     �ⲿ�ж�0
  CAM_HREF     : FIFO            : PB11     GPIO
  CAM_WEN      : FIFOд����      : PB12     GPIO
  XCLK         : CMOS��������ʱ��: PA8     MCO���	û��
  CAM_RRST     : FIFO����ַ��λ  : PB13     GPIO
  CAM_REN      : FIFOƬѡ        : PB14     GPIO
  CAM_RCLK     : FIFO��ʱ��      : PB15     GPIO
  FIFO D0~D7   : FIFO�������    : PC0~PC7 GPIO
  -----------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

#define FIFO_CS_PIN     GPIO_Pin_14   /* FIFOƬѡ */ 
#define FIFO_RRST_PIN   GPIO_Pin_13   /* FIFO����ַ��λ */ 
#define FIFO_RCLK_PIN   GPIO_Pin_15   /* FIFO��ʱ�� */ 
#define FIFO_WE_PIN     GPIO_Pin_12   /* FIFOд���� */

#define FIFO_CS_H()     GPIOB->BSRR =FIFO_CS_PIN	  /* GPIO_SetBits(GPIOD , FIFO_CS_PIN)   */
#define FIFO_CS_L()     GPIOB->BRR  =FIFO_CS_PIN	  /* GPIO_ResetBits(GPIOD , FIFO_CS_PIN) */

#define FIFO_RRST_H()   GPIOB->BSRR =FIFO_RRST_PIN	  /* GPIO_SetBits(GPIOE , FIFO_RRST_PIN)   */
#define FIFO_RRST_L()   GPIOB->BRR  =FIFO_RRST_PIN	  /* GPIO_ResetBits(GPIOE , FIFO_RRST_PIN) */

#define FIFO_RCLK_H()   GPIOB->BSRR =FIFO_RCLK_PIN	  /* GPIO_SetBits(GPIOE , FIFO_RCLK_PIN)   */
#define FIFO_RCLK_L()   GPIOB->BRR  =FIFO_RCLK_PIN	  /* GPIO_ResetBits(GPIOE , FIFO_RCLK_PIN) */

#define FIFO_WE_H()     GPIOB->BSRR =FIFO_WE_PIN	  /* GPIO_SetBits(GPIOD , FIFO_WE_PIN)   */
#define FIFO_WE_L()     GPIOB->BRR  =FIFO_WE_PIN	  /* GPIO_ResetBits(GPIOD , FIFO_WE_PIN) */

#define OV7670							   						 0x73
#define OV7670_REG_NUM                     114
#define PORT_VSYNC_CMOS                    GPIOB
#define RCC_APB2Periph_PORT_VSYNC_CMOS     RCC_APB2Periph_GPIOB
#define PIN_VSYNC_CMOS                     GPIO_Pin_10
#define EXTI_LINE_VSYNC_CMOS               EXTI_Line10
#define PORT_SOURCE_VSYNC_CMOS             GPIO_PortSourceGPIOB
#define PIN_SOURCE_VSYNC_CMOS              GPIO_PinSource10

/* Private variables ---------------------------------------------------------*/	
extern uint8_t Vsync;

/* Private function prototypes -----------------------------------------------*/
int Sensor_Init(void);
void FIFO_GPIO_Configuration(void);
void OV7670_NVIC_Configuration(void);
void OV7670_EXTI_Configuration(void);
int  OV7670_ReadReg(uint8_t LCD_Reg,uint16_t LCD_RegValue);
int  OV7670_WriteReg(uint8_t LCD_Reg,uint16_t LCD_RegValue);

#endif
/*********************************************************************************************************
      END FILE
*********************************************************************************************************/


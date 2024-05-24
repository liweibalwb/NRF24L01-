/*
***************************************************************	
*ע�⣺������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
*��������������
*�汾��V1.0
*���ߣ�OneTree
*�������ڣ�2012/11/5
*�޸����ڣ�2012/11/5 
*��Ȩ���У�����ؾ���
*Copyright(C) OneTree 2012-2022
*All rights reserved
***************************************************************
*Ӳ�����ӣ� WK_UP -------------------- PA0		Ӳ�������⣬�����ݲ�ʹ��
			KEY0  -------------------- PE5
			KEY1  -------------------- PE6

***************************************************************
*�޸ļ�¼��

	
*/
#ifndef		__KEY_H__
#define		__KEY_H__

#include	"stm32f10x.h"
#include	"delay.h"


#define 	KEY0 	GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_5)	
#define 	KEY1 	GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_6)	

void KEY_Init(void);//IO��ʼ��
u8   KEY_Scan(void);  //����ɨ�躯��	


#endif




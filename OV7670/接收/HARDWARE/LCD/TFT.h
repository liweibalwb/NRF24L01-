/*
***************************************************************	
*ע�⣺������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
*������R61509V TFT_LCD
*�汾��V1.0
*���ߣ�OneTree
*�������ڣ�2012/11/5
*�޸����ڣ�2012/11/5 
*��Ȩ���У�����ؾ���
*Copyright(C) OneTree 2012-2022
*All rights reserved
***************************************************************
*Ӳ�����ӣ� 
	    FSMC_NE4��PG12			FSMC_NWE��PD5		
		FSMC_A0 ��PF0			FSMC_NOE��PD4
		FSMC_D0 ��PD14			FSMC_D1 ��PD15
		FSMC_D2 ��PE0			FSMC_D3 ��PD1
		FSMC_D4 ��PE7			FSMC_D5 ��PE8
		FSMC_D6 ��PE9			FSMC_D7 ��PE10
		FSMC_D8 ��PE11			FSMC_D9 ��PE12
		FSMC_D10��PE13			FSMC_D11��PE14
		FSMC_D12��PE15			FSMC_D13��PD8
		FSMC_D14��PD9			FSMC_D15��PD10
		REST    ��NRST(STM32�ĸ�λ����)
***************************************************************
*�޸ļ�¼��

	
*/
#ifndef __TFT_H
#define __TFT_H	




#include	"main.h"		 


typedef struct
{
  vu16 TFT_REG;
  vu16 TFT_RAM;
} TFT_TypeDef;	 

/* TFT is connected to the FSMC_Bank1_NOR/SRAM4 and NE4 is used as ship select signal */
#define TFT_BASE   ((u32)(0x60000000 | 0x0C000000))
#define TFT         ((TFT_TypeDef *) TFT_BASE)

/////////////////////////////////////�û�������///////////////////////////////////	 
//����2���궨�壬������Ļ����ʾ��ʽ��IO�ٶ�
#define USE_HORIZONTAL  1	//�����Ƿ�ʹ�ú��� 		0,��ʹ��.1,ʹ��.
//////////////////////////////////////////////////////////////////////////////////	 


 
//TFTTFT������Ҫ���õĺ���		   
extern u16  POINT_COLOR;//Ĭ�Ϻ�ɫ    
extern u16  BACK_COLOR; //������ɫ.Ĭ��Ϊ��ɫ
//����TFT�ĳߴ�
#if USE_HORIZONTAL==1	//ʹ�ú���
#define TFT_W 400
#define TFT_H 240
#else
#define TFT_W 240
#define TFT_H 400
#endif
////////////////////////////////////////////////////////////////////
//-----------------TFT�˿ڶ���---------------- 
#define	TFT_LED_SET (GPIOA->BSRR = GPIO_Pin_1) //TFT����    		 PA1 
#define	TFT_LED_CLR (GPIOA->BRR = GPIO_Pin_1) 

								    

//////////////////////////////////////////////////////////////////////
//������ɫ
#define WHITE         	 0xFFFF
#define BLACK         	 0x0000	  
#define BLUE         	 0x001F  
#define BRED             0XF81F
#define GRED 			 0XFFE0
#define GBLUE			 0X07FF
#define RED           	 0xF800
#define MAGENTA       	 0xF81F
#define GREEN         	 0x07E0
#define CYAN          	 0x7FFF
#define YELLOW        	 0xFFE0
#define BROWN 			 0XBC40 //��ɫ
#define BRRED 			 0XFC07 //�غ�ɫ
#define GRAY  			 0X8430 //��ɫ
//GUI��ɫ

#define DARKBLUE      	 0X01CF	//����ɫ
#define LIGHTBLUE      	 0X7D7C	//ǳ��ɫ  
#define GRAYBLUE       	 0X5458 //����ɫ
//������ɫΪPANEL����ɫ 
 
#define LIGHTGREEN     	 0X841F //ǳ��ɫ
//#define LIGHTGRAY        0XEF5B //ǳ��ɫ(PANNEL)
#define LGRAY 			 0XC618 //ǳ��ɫ(PANNEL),���屳��ɫ

#define LGRAYBLUE        0XA651 //ǳ����ɫ(�м����ɫ)
#define LBBLUE           0X2B12 //ǳ����ɫ(ѡ����Ŀ�ķ�ɫ)
	    															  
extern u16 BACK_COLOR, POINT_COLOR ;  
void TFT_Init(void);
void TFT_DisplayOn(void);
void TFT_DisplayOff(void);
void TFT_Clear(u16 Color);	 
void TFT_SetCursor(u16 Xpos, u16 Ypos);
void TFT_SetWindow(u16 xsta,u16 ysta,u16 xend,u16 yend);  //����
void Recover_Window(void);	 //�ָ�ԭ���ĵ�ַ���ã��������ã�

void TFT_DrawPoint(u16 x,u16 y,u16 color);//����
u16  TFT_ReadPoint(u16 x,u16 y); //����

void TFT_DrawLine(u16 x0,u16 y0,u16 len,u16 color);					//��ֱ��
void TFT_Fill(u16 xsta,u16 ysta,u16 xend,u16 yend,u16 color);
void TFT_FillRectangle(u16 xsta,u16 ysta,u16 width,u16 height,u16 color);

void TFT_ShowChar(u16 x,u16 y,u16 color,u8 num,u8 size,u8 mode); //��ʾһ���ַ�
void TFT_ShowNum(u16 x,u16 y,u16 color,u32 num,u8 len,u8 size);  //��ʾһ������
void TFT_Show2Num(u16 x,u16 y,u16 color,u16 num,u8 len,u8 size,u8 mode);//��ʾ2������
void TFT_ShowString(u16 x,u16 y,u16 color,const u8 *p);		 //��ʾһ���ַ���,16����
void TFT_ShowImage(u16 xsta,u16 ysta,u16 xend,u16 yend,const u16 *p);	//��ʾָ����С��ͼƬ

void TFT_WR_REG(u16 data);
void TFT_WR_DATA(u16 dat);									    
void TFT_WriteReg(u16 TFT_Reg, u16 TFT_RegValue);
u16 TFT_ReadReg(u16 TFT_Reg);
void TFT_WriteRAM_Prepare(void);
void TFT_WriteRAM(u16 RGB_Code);
u16 TFT_ReadRAM(void);		   
u16 TFT_BGR2RGB(u16 c);

void TFT_CtrlLinesConfig(void);
void TFT_FSMCConfig(void);
																				 
#define R0             0x00
#define R1             0x01
#define R2             0x02
#define R3             0x03
#define R4             0x04
#define R5             0x05
#define R6             0x06
#define R7             0x07
#define R8             0x08
#define R9             0x09
#define R10            0x0A
#define R12            0x0C
#define R13            0x0D
#define R14            0x0E
#define R15            0x0F
#define R16            0x10
#define R17            0x11
#define R18            0x12
#define R19            0x13
#define R20            0x14
#define R21            0x15
#define R22            0x16
#define R23            0x17
#define R24            0x18
#define R25            0x19
#define R26            0x1A
#define R27            0x1B
#define R28            0x1C
#define R29            0x1D
#define R30            0x1E
#define R31            0x1F
#define R32            0x20
#define R33            0x21
#define R34            0x22
#define R36            0x24
#define R37            0x25
#define R40            0x28
#define R41            0x29
#define R43            0x2B
#define R45            0x2D
#define R48            0x30
#define R49            0x31
#define R50            0x32
#define R51            0x33
#define R52            0x34
#define R53            0x35
#define R54            0x36
#define R55            0x37
#define R56            0x38
#define R57            0x39
#define R59            0x3B
#define R60            0x3C
#define R61            0x3D
#define R62            0x3E
#define R63            0x3F
#define R64            0x40
#define R65            0x41
#define R66            0x42
#define R67            0x43
#define R68            0x44
#define R69            0x45
#define R70            0x46
#define R71            0x47
#define R72            0x48
#define R73            0x49
#define R74            0x4A
#define R75            0x4B
#define R76            0x4C
#define R77            0x4D
#define R78            0x4E
#define R79            0x4F
#define R80            0x50
#define R81            0x51
#define R82            0x52
#define R83            0x53
#define R96            0x60
#define R97            0x61
#define R106           0x6A
#define R118           0x76
#define R128           0x80
#define R129           0x81
#define R130           0x82
#define R131           0x83
#define R132           0x84
#define R133           0x85
#define R134           0x86
#define R135           0x87
#define R136           0x88
#define R137           0x89
#define R139           0x8B
#define R140           0x8C
#define R141           0x8D
#define R143           0x8F
#define R144           0x90
#define R145           0x91
#define R146           0x92
#define R147           0x93
#define R148           0x94
#define R149           0x95
#define R150           0x96
#define R151           0x97
#define R152           0x98
#define R153           0x99
#define R154           0x9A
#define R157           0x9D
#define R192           0xC0
#define R193           0xC1
#define R200		       0x0200
#define R201		       0x0201
#define R202		       0x0202
#define R229           0xE5	

						  		 
#endif  
	 
	 




/*
***************************************************************	
*ע�⣺������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
*������XPT2046 ������ʵ��
*�汾��V1.0
*���ߣ�OneTree
*�������ڣ�2012/11/7
*�޸����ڣ�2012/11/7 
*��Ȩ���У�����ؾ���
*Copyright(C) OneTree 2012-2022
*All rights reserved
***************************************************************
*Ӳ�����ӣ�INT-------------PF10		MISO--------------PF7
		   MOSI------------PF8		SCLK--------------PF9
		   CS--------------PF6 

		
***************************************************************
*�޸ļ�¼��

	
*/
#ifndef __TOUCH_H__
#define __TOUCH_H__

#include	"main.h"



//����״̬	 
#define Key_Down 0x01
#define Key_Up   0x00 
//�ʸ˽ṹ��
typedef struct 
{
	int X0;//ԭʼ����
	int Y0;		  
	u16 X; //����/�ݴ�����
	u16 Y;						   	    
	u8  Key_Sta;//�ʵ�״̬	
//�ʼĴ���
//bit7:���ڱ�Ǵ��㱻�������,���ⲿ�����޸�
//bit1,0:��һ�ΰ��µİ���ģʽ(1:����;2,�㴥)
	u8 Key_Reg;//�ʼĴ���		  
//������У׼����
	float xfac;
	float yfac;
	short xoff;
	short yoff;
}Pen_Holder;	   
extern Pen_Holder Pen_Point;
//�봥����оƬ��������	 
  
#define PEN  GPIO_ReadInputDataBit(GPIOF,GPIO_Pin_10)   //PF10  INT
#define DOUT GPIO_ReadInputDataBit(GPIOF,GPIO_Pin_7)    //PF7  MISO

#define TDIN_SET (GPIOF->BSRR = GPIO_Pin_8)   //PF8  MOSI
#define TDIN_CLR (GPIOF->BRR = GPIO_Pin_8)   //PF8   MOSI

#define TCLK_SET (GPIOF->BSRR = GPIO_Pin_9)  //PF9  SCLK
#define TCLK_CLR (GPIOF->BRR = GPIO_Pin_9)  //PF9   SCLK


#define TCS_SET  (GPIOF->BSRR = GPIO_Pin_6) //PF6 CS
#define TCS_CLR  (GPIOF->BRR = GPIO_Pin_6)  //PF6 CS
    


//ADS7843/7846/UH7843/7846/XPT2046/TSC2046 ָ�
//#define CMD_RDX   0X90  //0B10010000���ò�ַ�ʽ��X����
//#define CMD_RDY	0XD0  //0B11010000���ò�ַ�ʽ��Y����
extern u8 CMD_RDX;
extern u8 CMD_RDY;
   											 
#define TEMP_RD	0XF0  //0B11110000���ò�ַ�ʽ��Y����    
//ʹ�ñ���
#define ADJ_SAVE_ENABLE	    
			  
u16 T_abs(u16 m,u16 n);      //������֮��ľ���ֵ
void Touch_Init(void);		 //��ʼ��
u8 Read_ADS(u16 *x,u16 *y);	 //��������˫�����ȡ
u8 Read_ADS2(u16 *x,u16 *y); //����ǿ�˲���˫���������ȡ
u16 ADS_Read_XY(u8 xy);		 //���˲��������ȡ(������)
u16 ADS_Read_AD(u8 CMD);	 //��ȡADת��ֵ
void ADS_Write_Byte(u8 num); //�����оƬд��һ������
void Drow_Touch_Point(u16 x,u16 y,u16 color);//��һ�������׼��
void Set_Pen_Width(u16 x,u16 y,u16 color,u8 width); //���ñʿ��
void Touch_Adjust(void);          //������У׼
void Save_Adjdata(void);		  //����У׼����
u8 Get_Adjdata(void); 			  //��ȡУ׼����
void Pen_Int_Set(u8 en); 		  //PEN�ж�ʹ��/�ر�
void Convert_Pos(void);           //���ת������
u8 AI_Read_TP(u8 MS_TIME);        //��ȡTP״̬	 
u8 Is_In_Area(u16 x,u16 y,u16 xlen,u16 ylen);//�жϰ����Ƿ������������.	 
#endif


















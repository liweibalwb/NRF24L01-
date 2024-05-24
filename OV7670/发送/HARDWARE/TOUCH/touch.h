/*
***************************************************************	
*注意：本程序只供学习使用，未经作者许可，不得用于其它任何用途
*描述：XPT2046 触摸屏实验
*版本：V1.0
*作者：OneTree
*创建日期：2012/11/7
*修改日期：2012/11/7 
*版权所有，盗版必究。
*Copyright(C) OneTree 2012-2022
*All rights reserved
***************************************************************
*硬件连接：INT-------------PF10		MISO--------------PF7
		   MOSI------------PF8		SCLK--------------PF9
		   CS--------------PF6 

		
***************************************************************
*修改记录：

	
*/
#ifndef __TOUCH_H__
#define __TOUCH_H__

#include	"main.h"



//按键状态	 
#define Key_Down 0x01
#define Key_Up   0x00 
//笔杆结构体
typedef struct 
{
	int X0;//原始坐标
	int Y0;		  
	u16 X; //最终/暂存坐标
	u16 Y;						   	    
	u8  Key_Sta;//笔的状态	
//笔寄存器
//bit7:用于标记触点被按下与否,由外部函数修改
//bit1,0:第一次按下的按键模式(1:滑动;2,点触)
	u8 Key_Reg;//笔寄存器		  
//触摸屏校准参数
	float xfac;
	float yfac;
	short xoff;
	short yoff;
}Pen_Holder;	   
extern Pen_Holder Pen_Point;
//与触摸屏芯片连接引脚	 
  
#define PEN  GPIO_ReadInputDataBit(GPIOF,GPIO_Pin_10)   //PF10  INT
#define DOUT GPIO_ReadInputDataBit(GPIOF,GPIO_Pin_7)    //PF7  MISO

#define TDIN_SET (GPIOF->BSRR = GPIO_Pin_8)   //PF8  MOSI
#define TDIN_CLR (GPIOF->BRR = GPIO_Pin_8)   //PF8   MOSI

#define TCLK_SET (GPIOF->BSRR = GPIO_Pin_9)  //PF9  SCLK
#define TCLK_CLR (GPIOF->BRR = GPIO_Pin_9)  //PF9   SCLK


#define TCS_SET  (GPIOF->BSRR = GPIO_Pin_6) //PF6 CS
#define TCS_CLR  (GPIOF->BRR = GPIO_Pin_6)  //PF6 CS
    


//ADS7843/7846/UH7843/7846/XPT2046/TSC2046 指令集
//#define CMD_RDX   0X90  //0B10010000即用差分方式读X坐标
//#define CMD_RDY	0XD0  //0B11010000即用差分方式读Y坐标
extern u8 CMD_RDX;
extern u8 CMD_RDY;
   											 
#define TEMP_RD	0XF0  //0B11110000即用差分方式读Y坐标    
//使用保存
#define ADJ_SAVE_ENABLE	    
			  
u16 T_abs(u16 m,u16 n);      //求两数之差的绝对值
void Touch_Init(void);		 //初始化
u8 Read_ADS(u16 *x,u16 *y);	 //带舍弃的双方向读取
u8 Read_ADS2(u16 *x,u16 *y); //带加强滤波的双方向坐标读取
u16 ADS_Read_XY(u8 xy);		 //带滤波的坐标读取(单方向)
u16 ADS_Read_AD(u8 CMD);	 //读取AD转换值
void ADS_Write_Byte(u8 num); //向控制芯片写入一个数据
void Drow_Touch_Point(u16 x,u16 y,u16 color);//画一个坐标叫准点
void Set_Pen_Width(u16 x,u16 y,u16 color,u8 width); //设置笔宽度
void Touch_Adjust(void);          //触摸屏校准
void Save_Adjdata(void);		  //保存校准参数
u8 Get_Adjdata(void); 			  //读取校准参数
void Pen_Int_Set(u8 en); 		  //PEN中断使能/关闭
void Convert_Pos(void);           //结果转换函数
u8 AI_Read_TP(u8 MS_TIME);        //读取TP状态	 
u8 Is_In_Area(u16 x,u16 y,u16 xlen,u16 ylen);//判断按键是否在这个区域内.	 
#endif


















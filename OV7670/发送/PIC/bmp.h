#ifndef __BMP_H
#define __BMP_H
#include "tft.h"
#include "integer.h"
#include "ff.h"	
#include "explorer.h"
//////////////////////////////////////////////////////////////////////////////////	 
//Mini STM32开发板
//JPG/JPEG/BMP图片显示 解码代码		   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2010/6/18 
//版本：V1.4
//版权所有，盗版必究。
//Copyright(C) 正点原子 2009-2019
//All rights reserved
//********************************************************************************

//对BMP,支持16bit 24bit 和32bit图片显示 									 
//////////////////////////////////////////////////////////////////////////////////	 	   

//BMP信息头
typedef __packed struct
{
    DWORD biSize ;		   //说明BITMAPINFOHEADER结构所需要的字数。
    LONG  biWidth ;		   //说明图象的宽度，以象素为单位 
    LONG  biHeight ;	   //说明图象的高度，以象素为单位 
    WORD  biPlanes ;	   //为目标设备说明位面数，其值将总是被设为1 
    WORD  biBitCount ;	   //说明比特数/象素，其值为1、4、8、16、24、或32
    DWORD biCompression ;  //说明图象数据压缩的类型。其值可以是下述值之一：
	//BI_RGB：没有压缩；
	//BI_RLE8：每个象素8比特的RLE压缩编码，压缩格式由2字节组成(重复象素计数和颜色索引)；  
    //BI_RLE4：每个象素4比特的RLE压缩编码，压缩格式由2字节组成
  	//BI_BITFIELDS：每个象素的比特由指定的掩码决定。
    DWORD biSizeImage ;//说明图象的大小，以字节为单位。当用BI_RGB格式时，可设置为0  
    LONG  biXPelsPerMeter ;//说明水平分辨率，用象素/米表示
    LONG  biYPelsPerMeter ;//说明垂直分辨率，用象素/米表示
    DWORD biClrUsed ;	   //说明位图实际使用的彩色表中的颜色索引数
    DWORD biClrImportant ; //说明对图象显示有重要影响的颜色索引的数目，如果是0，表示都重要。 
}BITMAPINFOHEADER ;
//BMP头文件
typedef __packed struct
{
    WORD  bfType ;     //文件标志.只对'BM',用来识别BMP位图类型
    DWORD bfSize ;	   //文件大小,占四个字节
    WORD  bfReserved1 ;//保留
    WORD  bfReserved2 ;//保留
    DWORD bfOffBits ;  //从文件开始到位图数据(bitmap data)开始之间的的偏移量
}BITMAPFILEHEADER ;
//彩色表 
typedef __packed struct 
{
    BYTE rgbBlue ;    //指定蓝色强度
    BYTE rgbGreen ;	  //指定绿色强度 
    BYTE rgbRed ;	  //指定红色强度 
    BYTE rgbReserved ;//保留，设置为0 
}RGBQUAD ;
//位图信息头
typedef __packed struct
{ 
	BITMAPFILEHEADER bmfHeader;
	BITMAPINFOHEADER bmiHeader;  
	//RGBQUAD bmiColors[256];  
}BITMAPINFO; 

typedef RGBQUAD * LPRGBQUAD;//彩色表  

	  												 	  					    		
//BMP解码函数
int BmpDecode(const TCHAR *file_path,FIL *filetemp);		    
int Load_BMP_File(const TCHAR *file_path,u16 sx,u16 sy,u16 ex,u16 ey);//智能显示图片
		 
#endif









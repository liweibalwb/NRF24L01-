#ifndef __BMP_H
#define __BMP_H
#include "tft.h"
#include "integer.h"
#include "ff.h"	
#include "explorer.h"
//////////////////////////////////////////////////////////////////////////////////	 
//Mini STM32������
//JPG/JPEG/BMPͼƬ��ʾ �������		   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2010/6/18 
//�汾��V1.4
//��Ȩ���У�����ؾ���
//Copyright(C) ����ԭ�� 2009-2019
//All rights reserved
//********************************************************************************

//��BMP,֧��16bit 24bit ��32bitͼƬ��ʾ 									 
//////////////////////////////////////////////////////////////////////////////////	 	   

//BMP��Ϣͷ
typedef __packed struct
{
    DWORD biSize ;		   //˵��BITMAPINFOHEADER�ṹ����Ҫ��������
    LONG  biWidth ;		   //˵��ͼ��Ŀ�ȣ�������Ϊ��λ 
    LONG  biHeight ;	   //˵��ͼ��ĸ߶ȣ�������Ϊ��λ 
    WORD  biPlanes ;	   //ΪĿ���豸˵��λ��������ֵ�����Ǳ���Ϊ1 
    WORD  biBitCount ;	   //˵��������/���أ���ֵΪ1��4��8��16��24����32
    DWORD biCompression ;  //˵��ͼ������ѹ�������͡���ֵ����������ֵ֮һ��
	//BI_RGB��û��ѹ����
	//BI_RLE8��ÿ������8���ص�RLEѹ�����룬ѹ����ʽ��2�ֽ����(�ظ����ؼ�������ɫ����)��  
    //BI_RLE4��ÿ������4���ص�RLEѹ�����룬ѹ����ʽ��2�ֽ����
  	//BI_BITFIELDS��ÿ�����صı�����ָ�������������
    DWORD biSizeImage ;//˵��ͼ��Ĵ�С�����ֽ�Ϊ��λ������BI_RGB��ʽʱ��������Ϊ0  
    LONG  biXPelsPerMeter ;//˵��ˮƽ�ֱ��ʣ�������/�ױ�ʾ
    LONG  biYPelsPerMeter ;//˵����ֱ�ֱ��ʣ�������/�ױ�ʾ
    DWORD biClrUsed ;	   //˵��λͼʵ��ʹ�õĲ�ɫ���е���ɫ������
    DWORD biClrImportant ; //˵����ͼ����ʾ����ҪӰ�����ɫ��������Ŀ�������0����ʾ����Ҫ�� 
}BITMAPINFOHEADER ;
//BMPͷ�ļ�
typedef __packed struct
{
    WORD  bfType ;     //�ļ���־.ֻ��'BM',����ʶ��BMPλͼ����
    DWORD bfSize ;	   //�ļ���С,ռ�ĸ��ֽ�
    WORD  bfReserved1 ;//����
    WORD  bfReserved2 ;//����
    DWORD bfOffBits ;  //���ļ���ʼ��λͼ����(bitmap data)��ʼ֮��ĵ�ƫ����
}BITMAPFILEHEADER ;
//��ɫ�� 
typedef __packed struct 
{
    BYTE rgbBlue ;    //ָ����ɫǿ��
    BYTE rgbGreen ;	  //ָ����ɫǿ�� 
    BYTE rgbRed ;	  //ָ����ɫǿ�� 
    BYTE rgbReserved ;//����������Ϊ0 
}RGBQUAD ;
//λͼ��Ϣͷ
typedef __packed struct
{ 
	BITMAPFILEHEADER bmfHeader;
	BITMAPINFOHEADER bmiHeader;  
	//RGBQUAD bmiColors[256];  
}BITMAPINFO; 

typedef RGBQUAD * LPRGBQUAD;//��ɫ��  

	  												 	  					    		
//BMP���뺯��
int BmpDecode(const TCHAR *file_path,FIL *filetemp);		    
int Load_BMP_File(const TCHAR *file_path,u16 sx,u16 sy,u16 ex,u16 ey);//������ʾͼƬ
		 
#endif









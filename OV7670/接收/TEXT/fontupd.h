#ifndef __FONTUPD_H__
#define __FONTUPD_H__
	 
#include "main.h"
														   
#define EN_UPDATA_FONT


extern unsigned int FONT16ADDR ;
extern unsigned int FONT12ADDR ;
extern unsigned int OEM2UNIADDR;
extern unsigned int UNI2OEMSIZE;    //UNICODE TO OEM ���С
extern unsigned int UNI2OEMADDR;
extern unsigned int OEM2UNISIZE;	//OEM TO UNICODE ���С

unsigned char Updata_Font(void);//�����ֿ�   
unsigned char Font_Init(void);//��ʼ���ֿ�

#endif






















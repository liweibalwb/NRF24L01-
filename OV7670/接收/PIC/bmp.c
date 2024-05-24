#include "bmp.h" 
#include "string.h"   
  
//////////////////////////////////////////////////////////////////////////////////	 
//Mini STM32������
//BMPͼƬ��ʾ �������		   
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
	 	   	 

//ͼ����Ϣ
typedef struct
{			 
	u32 ImgWidth; //ͼ���ʵ�ʿ�Ⱥ͸߶�
	u32 ImgHeight;

	u32 Div_Fac;  //����ϵ�� (������10000����)
	
	u32 S_Height; //�趨�ĸ߶ȺͿ��
	u32 S_Width;
	
	u32	S_XOFF;	  //x���y���ƫ����
	u32 S_YOFF;

	u32 staticx;  //��ǰ��ʾ���ģ�������
	u32 staticy;
}PIC_POS;
		  
PIC_POS PICINFO;//ͼ��λ����Ϣ

////////////////////////////////////////////////////  
static void AI_Drow_Init(void);        //���ܻ�ͼ,��ʼ��.�õ���������PICINFO.Div_Fac	   


//���ݻ�����		  
//u8 bmp_buffer[1024];//���ݻ�����	
/////////////////////////////////////////////////////    

//��ʼ�����ܻ���
static void AI_Drow_Init(void)
{
	float temp,temp1;	   
	temp=(float)PICINFO.S_Width/PICINFO.ImgWidth;
	temp1=(float)PICINFO.S_Height/PICINFO.ImgHeight;
						 
	if(temp<temp1)temp1=temp;//ȡ��С���Ǹ�	 
	if(temp1>1)temp1=1;	  
	//ʹͼƬ��������������м�
	PICINFO.S_XOFF+=(PICINFO.S_Width-temp1*PICINFO.ImgWidth)/2;
	PICINFO.S_YOFF+=(PICINFO.S_Height-temp1*PICINFO.ImgHeight)/2;
	temp1*=10000;//����10000��	 
	PICINFO.Div_Fac=temp1;
			   
	PICINFO.staticx=500;
	PICINFO.staticy=500;//�ŵ�һ�������ܵ�ֵ����			 										    
}   
//�ж���������Ƿ������ʾ
//(x,y) :����ԭʼ����
//chg   :���ܱ���. 
//����ֵ:0,����Ҫ��ʾ.1,��Ҫ��ʾ
__inline u8 IsElementOk(u16 x,u16 y,u8 chg)
{				  
	if(x!=PICINFO.staticx||y!=PICINFO.staticy)
	{
		if(chg==1)
		{
			PICINFO.staticx=x;
			PICINFO.staticy=y;
		} 
		return 1;
	}
	else return 0;
}
//���ܻ�ͼ
//FileName:Ҫ��ʾ��ͼƬ�ļ�  BMP
//(sx,sy) :��ʼ��ʾ�������
//(ex,ey) :������ʾ�������
//ͼƬ�ڿ�ʼ�ͽ���������㷶Χ����ʾ
int Load_BMP_File(const TCHAR* file_path,u16 sx,u16 sy,u16 ex,u16 ey)
{	
	int	funcret;//����ֵ
	FIL filetemp;  
	FILINFO fileinfo;  //�ļ���Ϣ�ṹ�建��
	FRESULT res;  	


	//�õ���ʾ�����С	  	 
	if(ey>sy)PICINFO.S_Height=ey-sy;
	else PICINFO.S_Height=sy-ey;	 
	if(ex>sx)PICINFO.S_Width=ex-sx;
	else PICINFO.S_Width=sx-ex;
	//��ʾ������Ч
	if(PICINFO.S_Height==0||PICINFO.S_Width==0)
	{
		PICINFO.S_Height=TFT_H;
		PICINFO.S_Width=TFT_W;
		return FALSE;   
	}
	//Ӱ���ٶ�
	//SD_Init();//��ʼ��SD����������γ�֮���������ʹ��
	//��ʾ�Ŀ�ʼ�����
	PICINFO.S_YOFF=sy;
	PICINFO.S_XOFF=sx;
	//�ļ�������
	res = f_stat(file_path,&fileinfo);
	if(res)  return FALSE;
	if(T_BMP==FileType_Tell(fileinfo.fname))//�õ�һ��BMPͼ��
	{
		if(!f_open(&filetemp,file_path,FA_READ))
		{
			funcret=BmpDecode(file_path,&filetemp); //�õ�һ��BMPͼ��
		}
		else return FALSE;
		f_close(&filetemp);
		return funcret;    		  
	}
	else  return FALSE;
}

//�������BMP�ļ�							 	      	  
int BmpDecode(const TCHAR *file_path,FIL *filetemp)
{
    u16 count;		    	   
	u8  rgb ,color_byte;
	u16 x ,y,color,tmp_color ;	  
	u16 uiTemp;	   //x�᷽�����ؼ����� 
	u16 countpix=0;//��¼���� 	 
	//x,y��ʵ������	
	u16  realx=0;
//	u8  x_end=0;
//	u16 y_end=0;
	u16 realy=0;
	u8  yok=1;  
	FRESULT res;
	UINT br;				   
	BITMAPINFO *pbmp=NULL;//��ʱָ�� 
	u8* bmp_buffer=NULL;
	u32 buf_size =	4096;

	pbmp = (BITMAPINFO*)mem_malloc(sizeof(BITMAPINFO)) ;
	if(pbmp==NULL){
		return FALSE;
	}
	bmp_buffer = (u8*)mem_malloc(buf_size);	//����4K���ڴ�	
	if(bmp_buffer==NULL){
		mem_free(pbmp);
		pbmp = NULL;
		return 	FALSE;
	}							  
	f_read(filetemp,bmp_buffer,buf_size,&br);//��ȡbuf_size�ֽ�	  

	pbmp=(BITMAPINFO*)bmp_buffer;//�õ�BMP��ͷ����Ϣ   
	count=pbmp->bmfHeader.bfOffBits;        //����ƫ��,�õ����ݶεĿ�ʼ��ַ
	color_byte=pbmp->bmiHeader.biBitCount/8;//��ɫλ 16/24/32  
	PICINFO.ImgHeight=pbmp->bmiHeader.biHeight;//�õ�ͼƬ�߶�
	PICINFO.ImgWidth=pbmp->bmiHeader.biWidth;  //�õ�ͼƬ���   
	//ˮƽ���ر�����4�ı���!!
	if((PICINFO.ImgWidth*color_byte)%4)
		uiTemp=((PICINFO.ImgWidth*color_byte)/4+1)*4;
	else
		uiTemp=PICINFO.ImgWidth*color_byte;	    
	AI_Drow_Init();//��ʼ�����ܻ�ͼ	    
	//��ʼ����BMP   	 													 
	x =0 ;
//	y=PICINFO.ImgHeight;
	y = 0;
	rgb=0;      
	realy=y*PICINFO.Div_Fac/10000;

	while(1)
	{				 
		while(count<buf_size)  
	    {
			if(color_byte==3)   //24λ��ɫͼ
			{
				switch (rgb) 
				{
					case 0:
						tmp_color = bmp_buffer[count]>>3 ;
						color |= tmp_color;
						break ;	   
					case 1: 
						tmp_color = bmp_buffer[count]>>2 ;
						tmp_color <<= 5 ;
						color |= tmp_color ;
						break;	  
					case 2 : 
						tmp_color = bmp_buffer[count]>>3 ;
						tmp_color <<= 11 ;
						color |= tmp_color ;
						break ;			
				}   
			}
			else
			{
				if(color_byte==2)  //16λ��ɫͼ
				{
					switch(rgb)
					{
						case 0 : 
							color=bmp_buffer[count]&0x1f;
						    tmp_color=bmp_buffer[count]>>5;
							tmp_color<<=6;
							color|=tmp_color;
							break ;   
						case 1 : 			  
							tmp_color=bmp_buffer[count];
							tmp_color<<=9 ;
							color |= tmp_color ;
							break ;	 
					}		     
				}
				else 
				{
					if(color_byte==4)//32λ��ɫͼ
					{
						switch (rgb)
						{
							case 0 :  
								tmp_color=bmp_buffer[count];
								color|=tmp_color>>3;
								break ;     
							case 1 :  
								tmp_color=bmp_buffer[count];
								tmp_color>>=2;
								color|=tmp_color<<5;
								break ;	  
							case 2 :  
								tmp_color=bmp_buffer[count];
								tmp_color>>=3;
								color|=tmp_color<<11;
								break ;	 
							case 3 :break ;   
						}		  	 
					}  
				}     
			}//λͼ��ɫ�õ�	
			rgb++;	  
			count++ ;		  
			if(rgb==color_byte) //ˮƽ�����ȡ��1���������ݺ���ʾ
			{	
				if(x<PICINFO.ImgWidth)	 					 			   
				{	
					realx=x*PICINFO.Div_Fac/10000;//x��ʵ��ֵ
					if(IsElementOk(realx,realy,1)&&yok)//��������
					{						 				 	  	       
						TFT_DrawPoint(realx+PICINFO.S_XOFF,TFT_H-1-(realy+PICINFO.S_YOFF),color); 	 
					}   									    
				}
				x++;//x������һ������ 
				color=0x00; 
				rgb=0;  		  
			}
			countpix++;//�����ۼ�
			if(countpix>=uiTemp)//ˮƽ��������ֵ����.����
			{	
				 y++; 
				if(y>PICINFO.ImgHeight)return TRUE; 
				realy=y*PICINFO.Div_Fac/10000;//ʵ��yֵ�ı�	 
				if(IsElementOk(realx,realy,0))yok=1;//�˴����ı�PICINFO.staticx,y��ֵ	 
				else yok=0; 
				x=0; 
				countpix=0;
				color=0x00;
				rgb=0;
				
			}	 
		} 
	   	res = f_read(filetemp,bmp_buffer,buf_size,&br);	  
		if(res||br==0) break;
	 	count=0 ;
	}
	mem_free(bmp_buffer);		//�ͷ��ڴ�
	mem_free(pbmp);
	pbmp=NULL;
	bmp_buffer=NULL;
			 
	return TRUE;//BMP��ʾ����.    					   
} 





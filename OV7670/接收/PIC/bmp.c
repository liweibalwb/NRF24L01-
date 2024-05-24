#include "bmp.h" 
#include "string.h"   
  
//////////////////////////////////////////////////////////////////////////////////	 
//Mini STM32开发板
//BMP图片显示 解码代码		   
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
	 	   	 

//图像信息
typedef struct
{			 
	u32 ImgWidth; //图像的实际宽度和高度
	u32 ImgHeight;

	u32 Div_Fac;  //缩放系数 (扩大了10000倍的)
	
	u32 S_Height; //设定的高度和宽度
	u32 S_Width;
	
	u32	S_XOFF;	  //x轴和y轴的偏移量
	u32 S_YOFF;

	u32 staticx;  //当前显示到的ｘｙ坐标
	u32 staticy;
}PIC_POS;
		  
PIC_POS PICINFO;//图像位置信息

////////////////////////////////////////////////////  
static void AI_Drow_Init(void);        //智能画图,初始化.得到比例因子PICINFO.Div_Fac	   


//数据缓冲区		  
//u8 bmp_buffer[1024];//数据缓存区	
/////////////////////////////////////////////////////    

//初始化智能画点
static void AI_Drow_Init(void)
{
	float temp,temp1;	   
	temp=(float)PICINFO.S_Width/PICINFO.ImgWidth;
	temp1=(float)PICINFO.S_Height/PICINFO.ImgHeight;
						 
	if(temp<temp1)temp1=temp;//取较小的那个	 
	if(temp1>1)temp1=1;	  
	//使图片处于所给区域的中间
	PICINFO.S_XOFF+=(PICINFO.S_Width-temp1*PICINFO.ImgWidth)/2;
	PICINFO.S_YOFF+=(PICINFO.S_Height-temp1*PICINFO.ImgHeight)/2;
	temp1*=10000;//扩大10000倍	 
	PICINFO.Div_Fac=temp1;
			   
	PICINFO.staticx=500;
	PICINFO.staticy=500;//放到一个不可能的值上面			 										    
}   
//判断这个像素是否可以显示
//(x,y) :像素原始坐标
//chg   :功能变量. 
//返回值:0,不需要显示.1,需要显示
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
//智能画图
//FileName:要显示的图片文件  BMP
//(sx,sy) :开始显示的坐标点
//(ex,ey) :结束显示的坐标点
//图片在开始和结束的坐标点范围内显示
int Load_BMP_File(const TCHAR* file_path,u16 sx,u16 sy,u16 ex,u16 ey)
{	
	int	funcret;//返回值
	FIL filetemp;  
	FILINFO fileinfo;  //文件信息结构体缓存
	FRESULT res;  	


	//得到显示方框大小	  	 
	if(ey>sy)PICINFO.S_Height=ey-sy;
	else PICINFO.S_Height=sy-ey;	 
	if(ex>sx)PICINFO.S_Width=ex-sx;
	else PICINFO.S_Width=sx-ex;
	//显示区域无效
	if(PICINFO.S_Height==0||PICINFO.S_Width==0)
	{
		PICINFO.S_Height=TFT_H;
		PICINFO.S_Width=TFT_W;
		return FALSE;   
	}
	//影响速度
	//SD_Init();//初始化SD卡，在意外拔出之后可以正常使用
	//显示的开始坐标点
	PICINFO.S_YOFF=sy;
	PICINFO.S_XOFF=sx;
	//文件名传递
	res = f_stat(file_path,&fileinfo);
	if(res)  return FALSE;
	if(T_BMP==FileType_Tell(fileinfo.fname))//得到一个BMP图像
	{
		if(!f_open(&filetemp,file_path,FA_READ))
		{
			funcret=BmpDecode(file_path,&filetemp); //得到一个BMP图像
		}
		else return FALSE;
		f_close(&filetemp);
		return funcret;    		  
	}
	else  return FALSE;
}

//解码这个BMP文件							 	      	  
int BmpDecode(const TCHAR *file_path,FIL *filetemp)
{
    u16 count;		    	   
	u8  rgb ,color_byte;
	u16 x ,y,color,tmp_color ;	  
	u16 uiTemp;	   //x轴方向像素计数器 
	u16 countpix=0;//记录像素 	 
	//x,y的实际坐标	
	u16  realx=0;
//	u8  x_end=0;
//	u16 y_end=0;
	u16 realy=0;
	u8  yok=1;  
	FRESULT res;
	UINT br;				   
	BITMAPINFO *pbmp=NULL;//临时指针 
	u8* bmp_buffer=NULL;
	u32 buf_size =	4096;

	pbmp = (BITMAPINFO*)mem_malloc(sizeof(BITMAPINFO)) ;
	if(pbmp==NULL){
		return FALSE;
	}
	bmp_buffer = (u8*)mem_malloc(buf_size);	//申请4K的内存	
	if(bmp_buffer==NULL){
		mem_free(pbmp);
		pbmp = NULL;
		return 	FALSE;
	}							  
	f_read(filetemp,bmp_buffer,buf_size,&br);//读取buf_size字节	  

	pbmp=(BITMAPINFO*)bmp_buffer;//得到BMP的头部信息   
	count=pbmp->bmfHeader.bfOffBits;        //数据偏移,得到数据段的开始地址
	color_byte=pbmp->bmiHeader.biBitCount/8;//彩色位 16/24/32  
	PICINFO.ImgHeight=pbmp->bmiHeader.biHeight;//得到图片高度
	PICINFO.ImgWidth=pbmp->bmiHeader.biWidth;  //得到图片宽度   
	//水平像素必须是4的倍数!!
	if((PICINFO.ImgWidth*color_byte)%4)
		uiTemp=((PICINFO.ImgWidth*color_byte)/4+1)*4;
	else
		uiTemp=PICINFO.ImgWidth*color_byte;	    
	AI_Drow_Init();//初始化智能画图	    
	//开始解码BMP   	 													 
	x =0 ;
//	y=PICINFO.ImgHeight;
	y = 0;
	rgb=0;      
	realy=y*PICINFO.Div_Fac/10000;

	while(1)
	{				 
		while(count<buf_size)  
	    {
			if(color_byte==3)   //24位颜色图
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
				if(color_byte==2)  //16位颜色图
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
					if(color_byte==4)//32位颜色图
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
			}//位图颜色得到	
			rgb++;	  
			count++ ;		  
			if(rgb==color_byte) //水平方向读取到1像素数数据后显示
			{	
				if(x<PICINFO.ImgWidth)	 					 			   
				{	
					realx=x*PICINFO.Div_Fac/10000;//x轴实际值
					if(IsElementOk(realx,realy,1)&&yok)//符合条件
					{						 				 	  	       
						TFT_DrawPoint(realx+PICINFO.S_XOFF,TFT_H-1-(realy+PICINFO.S_YOFF),color); 	 
					}   									    
				}
				x++;//x轴增加一个像素 
				color=0x00; 
				rgb=0;  		  
			}
			countpix++;//像素累加
			if(countpix>=uiTemp)//水平方向像素值到了.换行
			{	
				 y++; 
				if(y>PICINFO.ImgHeight)return TRUE; 
				realy=y*PICINFO.Div_Fac/10000;//实际y值改变	 
				if(IsElementOk(realx,realy,0))yok=1;//此处不改变PICINFO.staticx,y的值	 
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
	mem_free(bmp_buffer);		//释放内存
	mem_free(pbmp);
	pbmp=NULL;
	bmp_buffer=NULL;
			 
	return TRUE;//BMP显示结束.    					   
} 





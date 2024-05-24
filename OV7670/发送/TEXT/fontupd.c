#include "fontupd.h"



	 
//系统文件路径
const unsigned char *sysfile_path[4]=
{			  
"0:/SYSTEM/FONT/UNI2OEM.BIN",
"0:/SYSTEM/FONT/OEM2UNI.BIN",
"0:/SYSTEM/FONT/GBK16.FON",
"0:/SYSTEM/FONT/GBK12.FON",	  
};

//字节0~3, 	记录UNI2OEMADDR;字节4~7 ,	记录UNI2OEMADDR的大小
//字节8~11,	记录OEM2UNIADDR;字节12~15 ,	记录OEM2UNIADDR的大小
//字节16~19,	记录FONT16ADDR ;字节20~23,	记录FONT16ADDR的大小
//字节24~27,	记录FONT12ADDR ;字节28~31,	记录FONT12ADDR的大小
//字节32 存放字库存在标志 0XAA
//字体存放,从100K处开始
//100K,存放UNICODE2GBK的转换码	

u32 FONT16ADDR ;	//16字体存放的地址
u32 FONT12ADDR ;	//12字体存放的地址
u32 UNI2OEMADDR;	//UNICODE TO OEM 表存放地址
u32 UNI2OEMSIZE;    //UNICODE TO OEM 表大小
u32 OEM2UNIADDR;	//OEM TO UNICODE 表存放地址
u32 OEM2UNISIZE;	//OEM TO UNICODE 表大小



//更新某一个
//fxpath:路径
//fx:更新的内容 0,ungbk;1 ,gbk16;
//返回值:0,成功;其他,失败.
#ifdef EN_UPDATA_FONT
static u8 temp[512];	  //临时空间
//FIL gbkFile;


u8 Updata_Font(void)
{
	u32 i=0;
	u8 t;
	float prog;
 	u32 tempsys[2];  //临时记录文件起始位置和文件大小

 	FIL filetemp;
//	DIR dirtemp;
 	FRESULT res;
	UINT br;	
	 
	TFT_Clear(WHITE); 
	TFT_ShowString(20,90,RED,"Write UNI2OEM to FLASH...");		
	TFT_ShowString(108,110,RED,"%");
 	res=f_open(&filetemp,(const TCHAR*)sysfile_path[0],FA_OPEN_EXISTING|FA_READ); 	  //打开UNI2OEM.BIN文件
	printf("%d",res);
	for(;;)//成功读出512个字节
	{	
		res = f_read(&filetemp,temp,sizeof(temp),&br);	
		if(res||br==0) break;   //读取文件完成则退出
		if(i<filetemp.fsize)//不超过文件大小
		{
			SPI_Flash_Write(temp,i+100000,512);//从100K字节处开始写入512个数据   
			i+=512;//增加512个字节
		}
		prog=(float)i/filetemp.fsize;
		prog*=100;
		if(t!=prog)
		{
			t=prog;
			if(t>100)t=100;
			TFT_ShowNum(84,110,RED,t,3,16);//显示数值
		}					    
	}
	UNI2OEMADDR = 100000;//UNI2OEMADDR从100K处开始写入. 
	OEM2UNIADDR = UNI2OEMADDR + filetemp.fsize; 
	tempsys[0]=UNI2OEMADDR;
	tempsys[1]=filetemp.fsize;	 	  //UNI2GBKADDR 大小
	SPI_Flash_Write((u8*)tempsys,0,8);//记录在地址0~7处
//	printf("UNI2OEMADDR写入FLASH完毕!\n");
//	printf("写入数据长度:%d\n",filetemp.fsize);
	res = f_close(&filetemp);        //关闭文件
	delay_ms(1000);	

 	i=0;
	TFT_Clear(WHITE); 	 
	TFT_ShowString(20,90,RED,"Write OEM2UNI to FLASH...");		
	TFT_ShowString(108,110,RED,"%");
 	res=f_open(&filetemp,(const TCHAR*)sysfile_path[1],FA_OPEN_EXISTING|FA_READ); 	  //打开OEM2UNI.BIN文件
	for(;;)//成功读出512个字节
	{	
		res = f_read(&filetemp,temp,sizeof(temp),&br);	
		if(res||br==0) break;   //读取文件完成则退出
		if(i<filetemp.fsize)//不超过文件大小
		{
			SPI_Flash_Write(temp,i+OEM2UNIADDR,512);//从100K字节处开始写入512个数据   
			i+=512;//增加512个字节
		}
		prog=(float)i/filetemp.fsize;
		prog*=100;
		if(t!=prog)
		{
			t=prog;
			if(t>100)t=100;
			TFT_ShowNum(84,110,RED,t,3,16);//显示数值
		}					    
	}

	FONT16ADDR = OEM2UNIADDR + filetemp.fsize; 
	tempsys[0]=OEM2UNIADDR;
	tempsys[1]=filetemp.fsize;	 	  //UNI2GBKADDR 大小
	SPI_Flash_Write((u8*)tempsys,8,8);//记录在地址8~15处
//	printf("OEM2UNIADDR写入FLASH完毕!\n");
//	printf("写入数据长度:%d\n",filetemp.fsize);
	res = f_close(&filetemp);        //关闭文件
	delay_ms(1000);	
    
	 i=0;
	TFT_Clear(WHITE); 	 
	TFT_ShowString(20,90,RED,"Write GBK16.FONT to FLASH...");		
	TFT_ShowString(108,110,RED,"%");
 	res=f_open(&filetemp,(const TCHAR*)sysfile_path[2],FA_OPEN_EXISTING|FA_READ); 	  //打开GBK16.FONT文件
	for(;;)//成功读出512个字节
	{	
		res = f_read(&filetemp,temp,sizeof(temp),&br);	
		if(res||br==0) break;   //读取文件完成则退出
		if(i<filetemp.fsize)//不超过文件大小
		{
			SPI_Flash_Write(temp,i+FONT16ADDR,512);//从100K字节处开始写入512个数据   
			i+=512;//增加512个字节
		}
		prog=(float)i/filetemp.fsize;
		prog*=100;
		if(t!=prog)
		{
			t=prog;
			if(t>100)t=100;
			TFT_ShowNum(84,110,RED,t,3,16);//显示数值
		}					    
	}

	FONT12ADDR = FONT16ADDR + filetemp.fsize; 
	tempsys[0]=FONT16ADDR;
	tempsys[1]=filetemp.fsize;	 	  //UNI2GBKADDR 大小
	SPI_Flash_Write((u8*)tempsys,16,8);//记录在地址16~23处
//	printf("FONT16ADDR 写入FLASH完毕!\n");
//	printf("写入数据长度:%d\n",filetemp.fsize);
	res = f_close(&filetemp);        //关闭文件
	delay_ms(1000);	

   	 i=0;
	TFT_Clear(WHITE); 	 
	TFT_ShowString(20,90,RED,"Write GBK12.FONT to FLASH...");		
	TFT_ShowString(108,110,RED,"%");
 	res=f_open(&filetemp,(const TCHAR*)sysfile_path[3],FA_OPEN_EXISTING|FA_READ); 	  //打开GBK12.FONT文件
	for(;;)//成功读出512个字节
	{	
		res = f_read(&filetemp,temp,sizeof(temp),&br);	
		if(res||br==0) break;   //读取文件完成则退出
		if(i<filetemp.fsize)//不超过文件大小
		{
			SPI_Flash_Write(temp,i+FONT12ADDR,512);//从100K字节处开始写入512个数据   
			i+=512;//增加512个字节
		}
		prog=(float)i/filetemp.fsize;
		prog*=100;
		if(t!=prog)
		{
			t=prog;
			if(t>100)t=100;
			TFT_ShowNum(84,110,RED,t,3,16);//显示数值
		}					    
	}

	tempsys[0]=FONT12ADDR;
	tempsys[1]=filetemp.fsize;	 	  //UNI2GBKADDR 大小
	SPI_Flash_Write((u8*)tempsys,24,8);//记录在地址24~31处
//	printf("FONT12ADDR 写入FLASH完毕!\n");
//	printf("写入数据长度:%d\n",filetemp.fsize);
	res = f_close(&filetemp);        //关闭文件
	delay_ms(1000);	

	t=0XAA;
   	SPI_Flash_Write(&t,32,1);//写入字库存在标志	0XAA
	TFT_ShowString(20,90,RED,"  Font Update Successed  ");		    
	delay_ms(1000);		
	delay_ms(1000);		
	return 0;//成功

}
#endif


//初始化字体
//返回值:0,字库完好.
//其他,字库丢失
u8 Font_Init(void)
{			  
	u32 tempsys[3];//临时记录文件起始位置和文件大小
	u8 fontok=0;
 	SPI_Flash_Read(&fontok,32,1);//读出字库标志位,看是否存在字库
	if(fontok!=0XAA)
		return 1;	//字库错误.FLASH不存在字库

	SPI_Flash_Read((u8*)tempsys,0,8);//读出8个字节   
//	printf("tempsys[0]:%d\n",tempsys[0]);
//	printf("tempsys[1]:%d\n",tempsys[1]);
	UNI2OEMADDR=tempsys[0];
	UNI2OEMSIZE=tempsys[1];

	SPI_Flash_Read((u8*)tempsys,8,8);//读出8个字节   
//	printf("tempsys[0]:%d\n",tempsys[0]);
//	printf("tempsys[1]:%d\n",tempsys[1]);
	OEM2UNIADDR=tempsys[0];
	OEM2UNISIZE=tempsys[1];				   

	SPI_Flash_Read((u8*)tempsys,16,8);//读出8个字节 
//	printf("tempsys[0]:%d\n",tempsys[0]);
//	printf("tempsys[1]:%d\n",tempsys[1]);	  
	FONT16ADDR=tempsys[0];	

	SPI_Flash_Read((u8*)tempsys,24,8);//读出8个字节
//	printf("tempsys[0]:%d\n",tempsys[0]);
//	printf("tempsys[1]:%d\n",tempsys[1]);	   
	FONT12ADDR=tempsys[0];	

	return 0;		    
}




























#include "mp3player.h"	 




u16 f_kbps=0;//歌曲文件位率	
//显示播放时间,比特率 等事信息 
void MP3_Msg_Show(u32 lenth,u16 index,u16 total)
{	
	static u16 playtime=0;//播放时间标记	     
 	u16 time=0;// 时间变量
	u16 temp=0;

//	if(f_kbps==0xffff)//未更新过
//	{
//		playtime=0;
//		f_kbps=VS_Get_HeadInfo();	   //获得比特率
//	}	 	 
	time=VS_Get_DecodeTime(); //得到解码时间
//	if(playtime==0)playtime=time;
	if((time!=playtime)&&(time!=0))//1s时间到,更新显示数据
	{
		playtime=time;//更新时间 	 				    
		temp=VS_Get_HeadInfo(); //获得比特率	   				 
		if(temp!=f_kbps)
		{
			f_kbps=temp;//更新KBPS	  				     
		}			 
		//显示播放时间			 
		TFT_Show2Num(60,110,RED,time/60,2,16,0);   //分钟
		TFT_ShowChar(60+16,110,RED,':',16,0);
		TFT_Show2Num(60+24,110,RED,time%60,2,16,0);//秒钟
		TFT_ShowChar(60+40,110,RED,'/',16,0);	    	 
		//显示总时间
		if(f_kbps)time=(lenth/f_kbps)/125;//得到秒钟数   (文件长度(字节)/(1000/8)/比特率=持续秒钟数    	  
		else time=0;//非法位率	  
 		TFT_Show2Num(60+48,110,RED,time/60,2,16,0);//分钟
		TFT_ShowChar(60+64,110,RED,':',16,0);
		TFT_Show2Num(60+72,110,RED,time%60,2,16,0);//秒钟		    
		//显示当前曲目的索引,及总曲目数
  		TFT_Show2Num(60+0,130,RED,index,3,16,0);  //索引
		TFT_ShowChar(60+24,130,RED,'/',16,0);
  		TFT_Show2Num(60+32,130,RED,total,3,16,0); //总曲目		  
		//显示位率			   
 		TFT_ShowNum(60+60,130,RED,f_kbps,3,16);//显示位率
		TFT_ShowString(60+84,130,RED,"Kbps");	  	  
	}   		 
}			  		 

//功能：通过按键选择播放，指定路径上的文件
//输入：*path:路径
//输出：1:硬件复位失败， 2：获取音频文件失败
u32 MUSIC_FILE_TYPE = T_UNTELL;             //存储文件类型,默认不能识别的文件类型
u8 Play_Music(char *path)
{
	u16 i;
	u8 key;	 						    
	u16 mus_total=0;//总音乐文件的个数

	if(VS_HD_Reset())return 1;	   //硬复位
	VS_Soft_Reset();  				//软复位VS10XX   
	set10XX();        				//设置音量等信息  

	if(VS10XX_ID==VS1053)		  //在软件复位时得到ID
	{
		MUSIC_FILE_TYPE = T_MP3|T_WMA|T_WAV|T_MID|T_FLAC|T_OGG;
	} 
	else  MUSIC_FILE_TYPE =  T_MP3|T_WMA|T_WAV|T_MID;

	Get_File_Info(path,MUSIC_FILE_TYPE,&mus_total);//获取当前文件夹下面的目标文件个数 

	if(!mus_total)  return 2;	//无音频文件则返回
	i=1;
	while(1)
	{	    
		key=Play_Song(path,i,mus_total); 
		if(key==1)
		{
			if(i<mus_total)i++;
			else i=1;
		}else if(key==2)
		{
			if(i>1)i--;
			else i=mus_total;
		}else
		{
			;
		}	   
	}	 
}
////////////////////////////////////////////////////////
//播放音乐
//index:播放的歌曲编号
//返回值：0,成功；1，下一曲；2，上一曲；0xff 得到文件信息失败；0xfe，硬复位失败；	 
u8 MUSIC_BUFFER[512];

extern u8 SPI_FLASH_BUF[4096];
u8 *music_path = (u8*)SPI_FLASH_BUF;   

u8 Play_Song(char *path,u16 index,u16 total)
{	 
	u8 key;
	u16 i,count=0;	 
	u8 pause=0,n;//不暂停  
	FRESULT res;
	FILINFO fno;
	DIR dir;
	FIL music_file;
	UINT br;

	#if _USE_LFN
	static char lfn[_MAX_LFN+1];
	fno.lfname = lfn;
	fno.lfsize = sizeof(lfn);
	#endif

	res = f_opendir(&dir,path);
	if(!res)
	{	
		for(;;)
		{
			res = f_readdir(&dir,&fno);   //顺序读取目录项
			if(res||fno.fname[0]==0) break;
			if(fno.fname[0]=='.')  continue;  //跳过自身目录项和父目录项

			if(fno.fattrib&AM_DIR) continue;  //跳过文件夹
			else
			{
				if(FileType_Tell(fno.fname)&MUSIC_FILE_TYPE)  //使用短文件名查找
				{
				count++;
				if(count==index) {count=0;break;}
				}
			}
		}	
	}
//	Show_Str(60,150,RED,(u8*)fno.lfname,16,0); 				//显示歌曲长文件名
	printf("%s\n",(u8*)fno.lfname);
	strcpy((char*)music_path,path);
	strcat((char*)music_path,"/");
	strcat((char*)music_path,(char*)fno.fname);		   //使用短文件名合成路径，寻址~
	res = f_open(&music_file,(const TCHAR*)music_path,FA_READ);		
	
	VS_HD_Reset();  //硬复位						 				  		 	  
	VS_Soft_Reset();//软复位 
	set10XX();        //设置音量等信息 
	if(T_FLAC==FileType_Tell(fno.fname))  //使用短文件名查找
	VS_Load_FlacPatch(); 		 				  		 	   														   							 	 	    				    		
	while(1)   //播放音乐的主循环
	{

		res=f_read(&music_file,MUSIC_BUFFER,sizeof(MUSIC_BUFFER),&br);//这里很容易死在这里(堆栈溢出)															  //因为默认的堆栈才512字节
		if(res||br==0)
		{
			f_close(&music_file);  
			 return 1;	//播放完毕，则播放下一首
		}

		i=0;
		delay_ms(1);
		SPI2_SetSpeed(SPI_SPEED_4);//高速,对VS1003B,最大值不能超过36.864/6Mhz，这里设置为4.5M  
		do      //主播放循环
	    {  	   
			if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_7)==1&&pause==0)  // 非暂停 送数据给VS1003 
			{			   	 
	    		VS_XDCS_CLR;    
	            for(n=0;n<32;n++)
				{
					SPI2_ReadWriteByte(MUSIC_BUFFER[i++]);	 			
				}
				VS_XDCS_SET;    
 			}
			key=KEY_Scan();
			if(key)
			{
				switch(key)
				{
					case 1://下一首歌
 						 f_close(&music_file); 
						return 1;
					case 3://上一首歌
					//	f_close(&music_file); 
						return 2;
					case 2://暂停/播放
						pause=!pause;
				}
			} 	    
		}while(i<511);	//循环发送512个字节 
	//	MP3_Msg_Show(fno.fsize,index,total);
	}
	    
}














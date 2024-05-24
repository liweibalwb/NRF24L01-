
#include  "explorer.h"


//文件类型
//3gp,3g2,m4a,mp4也是支持的.
//返回值:对应的类型   
//0,mp1;1,mp2;2,mp3;3,mp4;4,m4a;5,3gp;6,3g2;7,ogg;8,acc;9,wma;10,wav;11,midi;12,flac;
//13,lrc;14,txt;15,c;16,h;17,FON;18,SYS;19,bmp;20,jpg;21,jpeg;   
const unsigned char *filetype[22]=
{
"MP1","MP2","MP3","MP4","M4A","3GP","3G2","OGG","ACC","WMA","WAV","MID","FLA",
"LRC","TXT","C  ","H  ","FON","SYS","BMP","JPG","JPE"
};

//////////////////////////////////////////////////
//全局变量声明,BMP和JPEG共用
//将小写字母转为大写字母,如果是数字,则保持不变.
u8 static char_upper(u8 c)
{
	if(c<'A')return c;//数字,保持不变.
	if(c>='a')return c-0x20;//变为大写.
	else return c;//大写,保持不变
}	   	

//功能：查找指定文件的类型
//输入:filename 文件名
//返回值:文件类型名
u32 FileType_Tell(char * filename)
{
    u8 i,temp;
	u8 buf[4];
    temp = strlen((const char *)filename); 
	for(i=0;i<3;i++)
	{
		buf[i] = filename[temp-3+i];
	}
	buf[3] = '\0';
	for(i=0;i<4;i++) buf[i] = char_upper(buf[i]);
	for(i=0; i<22; i++)
	{
    	if(strcmp((const char*)buf,(const char*)filetype[i])==0)
		return 1<<i;
	}
    return T_UNTELL;
}

//功能：当前目录下，查找指定扩展名文件个数
//*path:路径 filetype:扩展名 count 指定文件类型个数
//输出 0：成功	其他：失败(无指定文件)
u8 Get_File_Info(char *path,u32 filetype,u16 *count)
{
	FRESULT res;
	FILINFO fno;
	DIR dir;
	res = f_opendir(&dir,(const TCHAR *)path);	  //打开目录对象
	if(res) return res;
	for(;;)
	{
		res = f_readdir(&dir,&fno);  //顺序读取目录项
		if(res||fno.fname[0]==0) break;
		if(fno.fname[0]=='.')  continue;  //跳过自身目录项和父目录项
		if(fno.fattrib&AM_DIR)  continue;  //过滤目录
		else
		{
			if(FileType_Tell((char *)fno.fname)&filetype)
			(*count)++;
		} 

	}
	if(*count) return 0;
	else return 1;	
}

//扫描文件夹下所有文件并显示

FRESULT scan_files (char* path)
{
    FRESULT res;
    FILINFO fno;
    DIR dir;
    int i;
    char *fn;
	#if _USE_LFN
    static char lfn[_MAX_LFN  + 1];
    fno.lfname = lfn;
    fno.lfsize = sizeof(lfn);
	#endif
 
    res = f_opendir(&dir, path);
	if(!res) printf("###");
    if (res == FR_OK) {
        i = strlen(path);
        for (;;) {
            res = f_readdir(&dir, &fno);
            if (res != FR_OK || fno.fname[0] == 0) break;
            if (fno.fname[0] == '.') continue;

			#if _USE_LFN
            fn = *fno.lfname ? fno.lfname : fno.fname;
			#else
            fn = fno.fname;
			#endif

            if (fno.fattrib & AM_DIR) {
             //   LCD_ShowString(17,90,(u8 *)path);
			//	 printf("%s",fn);

			//	 printf("%s",path);
				 sprintf(&path[i],"/%s",fn);
			//	strcat(path,"/");								 //合成文件夹路径
			//	strcat(path,fn);								 //合成文件夹路径
			
				 printf("%s",path);
                res = scan_files(path);
                if (res != FR_OK) break;
                path[i] = 0;	 //结束下一级目录
				 printf("$$$");
            } else {
			//	LCD_ShowString(17,90,(u8 *)path);
			//	LCD_ShowString(17,110+j*20,(u8 *)fn);
				printf("%s/%s\n", (u8 *)path, (u8 *)fn);
	
            }
        }
    }
    return res;
}

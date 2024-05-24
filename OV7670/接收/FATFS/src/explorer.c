
#include  "explorer.h"


//�ļ�����
//3gp,3g2,m4a,mp4Ҳ��֧�ֵ�.
//����ֵ:��Ӧ������   
//0,mp1;1,mp2;2,mp3;3,mp4;4,m4a;5,3gp;6,3g2;7,ogg;8,acc;9,wma;10,wav;11,midi;12,flac;
//13,lrc;14,txt;15,c;16,h;17,FON;18,SYS;19,bmp;20,jpg;21,jpeg;   
const unsigned char *filetype[22]=
{
"MP1","MP2","MP3","MP4","M4A","3GP","3G2","OGG","ACC","WMA","WAV","MID","FLA",
"LRC","TXT","C  ","H  ","FON","SYS","BMP","JPG","JPE"
};

//////////////////////////////////////////////////
//ȫ�ֱ�������,BMP��JPEG����
//��Сд��ĸתΪ��д��ĸ,���������,�򱣳ֲ���.
u8 static char_upper(u8 c)
{
	if(c<'A')return c;//����,���ֲ���.
	if(c>='a')return c-0x20;//��Ϊ��д.
	else return c;//��д,���ֲ���
}	   	

//���ܣ�����ָ���ļ�������
//����:filename �ļ���
//����ֵ:�ļ�������
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

//���ܣ���ǰĿ¼�£�����ָ����չ���ļ�����
//*path:·�� filetype:��չ�� count ָ���ļ����͸���
//��� 0���ɹ�	������ʧ��(��ָ���ļ�)
u8 Get_File_Info(char *path,u32 filetype,u16 *count)
{
	FRESULT res;
	FILINFO fno;
	DIR dir;
	res = f_opendir(&dir,(const TCHAR *)path);	  //��Ŀ¼����
	if(res) return res;
	for(;;)
	{
		res = f_readdir(&dir,&fno);  //˳���ȡĿ¼��
		if(res||fno.fname[0]==0) break;
		if(fno.fname[0]=='.')  continue;  //��������Ŀ¼��͸�Ŀ¼��
		if(fno.fattrib&AM_DIR)  continue;  //����Ŀ¼
		else
		{
			if(FileType_Tell((char *)fno.fname)&filetype)
			(*count)++;
		} 

	}
	if(*count) return 0;
	else return 1;	
}

//ɨ���ļ����������ļ�����ʾ

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
			//	strcat(path,"/");								 //�ϳ��ļ���·��
			//	strcat(path,fn);								 //�ϳ��ļ���·��
			
				 printf("%s",path);
                res = scan_files(path);
                if (res != FR_OK) break;
                path[i] = 0;	 //������һ��Ŀ¼
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

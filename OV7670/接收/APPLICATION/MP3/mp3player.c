#include "mp3player.h"	 




u16 f_kbps=0;//�����ļ�λ��	
//��ʾ����ʱ��,������ ������Ϣ 
void MP3_Msg_Show(u32 lenth,u16 index,u16 total)
{	
	static u16 playtime=0;//����ʱ����	     
 	u16 time=0;// ʱ�����
	u16 temp=0;

//	if(f_kbps==0xffff)//δ���¹�
//	{
//		playtime=0;
//		f_kbps=VS_Get_HeadInfo();	   //��ñ�����
//	}	 	 
	time=VS_Get_DecodeTime(); //�õ�����ʱ��
//	if(playtime==0)playtime=time;
	if((time!=playtime)&&(time!=0))//1sʱ�䵽,������ʾ����
	{
		playtime=time;//����ʱ�� 	 				    
		temp=VS_Get_HeadInfo(); //��ñ�����	   				 
		if(temp!=f_kbps)
		{
			f_kbps=temp;//����KBPS	  				     
		}			 
		//��ʾ����ʱ��			 
		TFT_Show2Num(60,110,RED,time/60,2,16,0);   //����
		TFT_ShowChar(60+16,110,RED,':',16,0);
		TFT_Show2Num(60+24,110,RED,time%60,2,16,0);//����
		TFT_ShowChar(60+40,110,RED,'/',16,0);	    	 
		//��ʾ��ʱ��
		if(f_kbps)time=(lenth/f_kbps)/125;//�õ�������   (�ļ�����(�ֽ�)/(1000/8)/������=����������    	  
		else time=0;//�Ƿ�λ��	  
 		TFT_Show2Num(60+48,110,RED,time/60,2,16,0);//����
		TFT_ShowChar(60+64,110,RED,':',16,0);
		TFT_Show2Num(60+72,110,RED,time%60,2,16,0);//����		    
		//��ʾ��ǰ��Ŀ������,������Ŀ��
  		TFT_Show2Num(60+0,130,RED,index,3,16,0);  //����
		TFT_ShowChar(60+24,130,RED,'/',16,0);
  		TFT_Show2Num(60+32,130,RED,total,3,16,0); //����Ŀ		  
		//��ʾλ��			   
 		TFT_ShowNum(60+60,130,RED,f_kbps,3,16);//��ʾλ��
		TFT_ShowString(60+84,130,RED,"Kbps");	  	  
	}   		 
}			  		 

//���ܣ�ͨ������ѡ�񲥷ţ�ָ��·���ϵ��ļ�
//���룺*path:·��
//�����1:Ӳ����λʧ�ܣ� 2����ȡ��Ƶ�ļ�ʧ��
u32 MUSIC_FILE_TYPE = T_UNTELL;             //�洢�ļ�����,Ĭ�ϲ���ʶ����ļ�����
u8 Play_Music(char *path)
{
	u16 i;
	u8 key;	 						    
	u16 mus_total=0;//�������ļ��ĸ���

	if(VS_HD_Reset())return 1;	   //Ӳ��λ
	VS_Soft_Reset();  				//��λVS10XX   
	set10XX();        				//������������Ϣ  

	if(VS10XX_ID==VS1053)		  //�������λʱ�õ�ID
	{
		MUSIC_FILE_TYPE = T_MP3|T_WMA|T_WAV|T_MID|T_FLAC|T_OGG;
	} 
	else  MUSIC_FILE_TYPE =  T_MP3|T_WMA|T_WAV|T_MID;

	Get_File_Info(path,MUSIC_FILE_TYPE,&mus_total);//��ȡ��ǰ�ļ��������Ŀ���ļ����� 

	if(!mus_total)  return 2;	//����Ƶ�ļ��򷵻�
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
//��������
//index:���ŵĸ������
//����ֵ��0,�ɹ���1����һ����2����һ����0xff �õ��ļ���Ϣʧ�ܣ�0xfe��Ӳ��λʧ�ܣ�	 
u8 MUSIC_BUFFER[512];

extern u8 SPI_FLASH_BUF[4096];
u8 *music_path = (u8*)SPI_FLASH_BUF;   

u8 Play_Song(char *path,u16 index,u16 total)
{	 
	u8 key;
	u16 i,count=0;	 
	u8 pause=0,n;//����ͣ  
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
			res = f_readdir(&dir,&fno);   //˳���ȡĿ¼��
			if(res||fno.fname[0]==0) break;
			if(fno.fname[0]=='.')  continue;  //��������Ŀ¼��͸�Ŀ¼��

			if(fno.fattrib&AM_DIR) continue;  //�����ļ���
			else
			{
				if(FileType_Tell(fno.fname)&MUSIC_FILE_TYPE)  //ʹ�ö��ļ�������
				{
				count++;
				if(count==index) {count=0;break;}
				}
			}
		}	
	}
//	Show_Str(60,150,RED,(u8*)fno.lfname,16,0); 				//��ʾ�������ļ���
	printf("%s\n",(u8*)fno.lfname);
	strcpy((char*)music_path,path);
	strcat((char*)music_path,"/");
	strcat((char*)music_path,(char*)fno.fname);		   //ʹ�ö��ļ����ϳ�·����Ѱַ~
	res = f_open(&music_file,(const TCHAR*)music_path,FA_READ);		
	
	VS_HD_Reset();  //Ӳ��λ						 				  		 	  
	VS_Soft_Reset();//��λ 
	set10XX();        //������������Ϣ 
	if(T_FLAC==FileType_Tell(fno.fname))  //ʹ�ö��ļ�������
	VS_Load_FlacPatch(); 		 				  		 	   														   							 	 	    				    		
	while(1)   //�������ֵ���ѭ��
	{

		res=f_read(&music_file,MUSIC_BUFFER,sizeof(MUSIC_BUFFER),&br);//�����������������(��ջ���)															  //��ΪĬ�ϵĶ�ջ��512�ֽ�
		if(res||br==0)
		{
			f_close(&music_file);  
			 return 1;	//������ϣ��򲥷���һ��
		}

		i=0;
		delay_ms(1);
		SPI2_SetSpeed(SPI_SPEED_4);//����,��VS1003B,���ֵ���ܳ���36.864/6Mhz����������Ϊ4.5M  
		do      //������ѭ��
	    {  	   
			if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_7)==1&&pause==0)  // ����ͣ �����ݸ�VS1003 
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
					case 1://��һ�׸�
 						 f_close(&music_file); 
						return 1;
					case 3://��һ�׸�
					//	f_close(&music_file); 
						return 2;
					case 2://��ͣ/����
						pause=!pause;
				}
			} 	    
		}while(i<511);	//ѭ������512���ֽ� 
	//	MP3_Msg_Show(fno.fsize,index,total);
	}
	    
}














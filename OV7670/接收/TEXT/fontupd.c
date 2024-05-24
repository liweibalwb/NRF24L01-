#include "fontupd.h"



	 
//ϵͳ�ļ�·��
const unsigned char *sysfile_path[4]=
{			  
"0:/SYSTEM/FONT/UNI2OEM.BIN",
"0:/SYSTEM/FONT/OEM2UNI.BIN",
"0:/SYSTEM/FONT/GBK16.FON",
"0:/SYSTEM/FONT/GBK12.FON",	  
};

//�ֽ�0~3, 	��¼UNI2OEMADDR;�ֽ�4~7 ,	��¼UNI2OEMADDR�Ĵ�С
//�ֽ�8~11,	��¼OEM2UNIADDR;�ֽ�12~15 ,	��¼OEM2UNIADDR�Ĵ�С
//�ֽ�16~19,	��¼FONT16ADDR ;�ֽ�20~23,	��¼FONT16ADDR�Ĵ�С
//�ֽ�24~27,	��¼FONT12ADDR ;�ֽ�28~31,	��¼FONT12ADDR�Ĵ�С
//�ֽ�32 ����ֿ���ڱ�־ 0XAA
//������,��100K����ʼ
//100K,���UNICODE2GBK��ת����	

u32 FONT16ADDR ;	//16�����ŵĵ�ַ
u32 FONT12ADDR ;	//12�����ŵĵ�ַ
u32 UNI2OEMADDR;	//UNICODE TO OEM ���ŵ�ַ
u32 UNI2OEMSIZE;    //UNICODE TO OEM ���С
u32 OEM2UNIADDR;	//OEM TO UNICODE ���ŵ�ַ
u32 OEM2UNISIZE;	//OEM TO UNICODE ���С



//����ĳһ��
//fxpath:·��
//fx:���µ����� 0,ungbk;1 ,gbk16;
//����ֵ:0,�ɹ�;����,ʧ��.
#ifdef EN_UPDATA_FONT
static u8 temp[512];	  //��ʱ�ռ�
//FIL gbkFile;


u8 Updata_Font(void)
{
	u32 i=0;
	u8 t;
	float prog;
 	u32 tempsys[2];  //��ʱ��¼�ļ���ʼλ�ú��ļ���С

 	FIL filetemp;
//	DIR dirtemp;
 	FRESULT res;
	UINT br;	
	 
	TFT_Clear(WHITE); 
	TFT_ShowString(20,90,RED,"Write UNI2OEM to FLASH...");		
	TFT_ShowString(108,110,RED,"%");
 	res=f_open(&filetemp,(const TCHAR*)sysfile_path[0],FA_OPEN_EXISTING|FA_READ); 	  //��UNI2OEM.BIN�ļ�
	printf("%d",res);
	for(;;)//�ɹ�����512���ֽ�
	{	
		res = f_read(&filetemp,temp,sizeof(temp),&br);	
		if(res||br==0) break;   //��ȡ�ļ�������˳�
		if(i<filetemp.fsize)//�������ļ���С
		{
			SPI_Flash_Write(temp,i+100000,512);//��100K�ֽڴ���ʼд��512������   
			i+=512;//����512���ֽ�
		}
		prog=(float)i/filetemp.fsize;
		prog*=100;
		if(t!=prog)
		{
			t=prog;
			if(t>100)t=100;
			TFT_ShowNum(84,110,RED,t,3,16);//��ʾ��ֵ
		}					    
	}
	UNI2OEMADDR = 100000;//UNI2OEMADDR��100K����ʼд��. 
	OEM2UNIADDR = UNI2OEMADDR + filetemp.fsize; 
	tempsys[0]=UNI2OEMADDR;
	tempsys[1]=filetemp.fsize;	 	  //UNI2GBKADDR ��С
	SPI_Flash_Write((u8*)tempsys,0,8);//��¼�ڵ�ַ0~7��
//	printf("UNI2OEMADDRд��FLASH���!\n");
//	printf("д�����ݳ���:%d\n",filetemp.fsize);
	res = f_close(&filetemp);        //�ر��ļ�
	delay_ms(1000);	

 	i=0;
	TFT_Clear(WHITE); 	 
	TFT_ShowString(20,90,RED,"Write OEM2UNI to FLASH...");		
	TFT_ShowString(108,110,RED,"%");
 	res=f_open(&filetemp,(const TCHAR*)sysfile_path[1],FA_OPEN_EXISTING|FA_READ); 	  //��OEM2UNI.BIN�ļ�
	for(;;)//�ɹ�����512���ֽ�
	{	
		res = f_read(&filetemp,temp,sizeof(temp),&br);	
		if(res||br==0) break;   //��ȡ�ļ�������˳�
		if(i<filetemp.fsize)//�������ļ���С
		{
			SPI_Flash_Write(temp,i+OEM2UNIADDR,512);//��100K�ֽڴ���ʼд��512������   
			i+=512;//����512���ֽ�
		}
		prog=(float)i/filetemp.fsize;
		prog*=100;
		if(t!=prog)
		{
			t=prog;
			if(t>100)t=100;
			TFT_ShowNum(84,110,RED,t,3,16);//��ʾ��ֵ
		}					    
	}

	FONT16ADDR = OEM2UNIADDR + filetemp.fsize; 
	tempsys[0]=OEM2UNIADDR;
	tempsys[1]=filetemp.fsize;	 	  //UNI2GBKADDR ��С
	SPI_Flash_Write((u8*)tempsys,8,8);//��¼�ڵ�ַ8~15��
//	printf("OEM2UNIADDRд��FLASH���!\n");
//	printf("д�����ݳ���:%d\n",filetemp.fsize);
	res = f_close(&filetemp);        //�ر��ļ�
	delay_ms(1000);	
    
	 i=0;
	TFT_Clear(WHITE); 	 
	TFT_ShowString(20,90,RED,"Write GBK16.FONT to FLASH...");		
	TFT_ShowString(108,110,RED,"%");
 	res=f_open(&filetemp,(const TCHAR*)sysfile_path[2],FA_OPEN_EXISTING|FA_READ); 	  //��GBK16.FONT�ļ�
	for(;;)//�ɹ�����512���ֽ�
	{	
		res = f_read(&filetemp,temp,sizeof(temp),&br);	
		if(res||br==0) break;   //��ȡ�ļ�������˳�
		if(i<filetemp.fsize)//�������ļ���С
		{
			SPI_Flash_Write(temp,i+FONT16ADDR,512);//��100K�ֽڴ���ʼд��512������   
			i+=512;//����512���ֽ�
		}
		prog=(float)i/filetemp.fsize;
		prog*=100;
		if(t!=prog)
		{
			t=prog;
			if(t>100)t=100;
			TFT_ShowNum(84,110,RED,t,3,16);//��ʾ��ֵ
		}					    
	}

	FONT12ADDR = FONT16ADDR + filetemp.fsize; 
	tempsys[0]=FONT16ADDR;
	tempsys[1]=filetemp.fsize;	 	  //UNI2GBKADDR ��С
	SPI_Flash_Write((u8*)tempsys,16,8);//��¼�ڵ�ַ16~23��
//	printf("FONT16ADDR д��FLASH���!\n");
//	printf("д�����ݳ���:%d\n",filetemp.fsize);
	res = f_close(&filetemp);        //�ر��ļ�
	delay_ms(1000);	

   	 i=0;
	TFT_Clear(WHITE); 	 
	TFT_ShowString(20,90,RED,"Write GBK12.FONT to FLASH...");		
	TFT_ShowString(108,110,RED,"%");
 	res=f_open(&filetemp,(const TCHAR*)sysfile_path[3],FA_OPEN_EXISTING|FA_READ); 	  //��GBK12.FONT�ļ�
	for(;;)//�ɹ�����512���ֽ�
	{	
		res = f_read(&filetemp,temp,sizeof(temp),&br);	
		if(res||br==0) break;   //��ȡ�ļ�������˳�
		if(i<filetemp.fsize)//�������ļ���С
		{
			SPI_Flash_Write(temp,i+FONT12ADDR,512);//��100K�ֽڴ���ʼд��512������   
			i+=512;//����512���ֽ�
		}
		prog=(float)i/filetemp.fsize;
		prog*=100;
		if(t!=prog)
		{
			t=prog;
			if(t>100)t=100;
			TFT_ShowNum(84,110,RED,t,3,16);//��ʾ��ֵ
		}					    
	}

	tempsys[0]=FONT12ADDR;
	tempsys[1]=filetemp.fsize;	 	  //UNI2GBKADDR ��С
	SPI_Flash_Write((u8*)tempsys,24,8);//��¼�ڵ�ַ24~31��
//	printf("FONT12ADDR д��FLASH���!\n");
//	printf("д�����ݳ���:%d\n",filetemp.fsize);
	res = f_close(&filetemp);        //�ر��ļ�
	delay_ms(1000);	

	t=0XAA;
   	SPI_Flash_Write(&t,32,1);//д���ֿ���ڱ�־	0XAA
	TFT_ShowString(20,90,RED,"  Font Update Successed  ");		    
	delay_ms(1000);		
	delay_ms(1000);		
	return 0;//�ɹ�

}
#endif


//��ʼ������
//����ֵ:0,�ֿ����.
//����,�ֿⶪʧ
u8 Font_Init(void)
{			  
	u32 tempsys[3];//��ʱ��¼�ļ���ʼλ�ú��ļ���С
	u8 fontok=0;
 	SPI_Flash_Read(&fontok,32,1);//�����ֿ��־λ,���Ƿ�����ֿ�
	if(fontok!=0XAA)
		return 1;	//�ֿ����.FLASH�������ֿ�

	SPI_Flash_Read((u8*)tempsys,0,8);//����8���ֽ�   
//	printf("tempsys[0]:%d\n",tempsys[0]);
//	printf("tempsys[1]:%d\n",tempsys[1]);
	UNI2OEMADDR=tempsys[0];
	UNI2OEMSIZE=tempsys[1];

	SPI_Flash_Read((u8*)tempsys,8,8);//����8���ֽ�   
//	printf("tempsys[0]:%d\n",tempsys[0]);
//	printf("tempsys[1]:%d\n",tempsys[1]);
	OEM2UNIADDR=tempsys[0];
	OEM2UNISIZE=tempsys[1];				   

	SPI_Flash_Read((u8*)tempsys,16,8);//����8���ֽ� 
//	printf("tempsys[0]:%d\n",tempsys[0]);
//	printf("tempsys[1]:%d\n",tempsys[1]);	  
	FONT16ADDR=tempsys[0];	

	SPI_Flash_Read((u8*)tempsys,24,8);//����8���ֽ�
//	printf("tempsys[0]:%d\n",tempsys[0]);
//	printf("tempsys[1]:%d\n",tempsys[1]);	   
	FONT12ADDR=tempsys[0];	

	return 0;		    
}




























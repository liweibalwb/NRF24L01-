
#include "VS10XX.h"		 
#include "flac.h" 
	   	 
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//Mini STM32开发板
//VS10XX  驱动代码		   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2011/6/18
//版本：V1.3
//版权所有，盗版必究。
//Copyright(C) 正点原子 2009-2019
//All rights reserved
//********************************************************************************
//V1.2
//1,增加VS1053的FLAC播放支持.
//2,修改GetHeadInfo函数.    
//V1.3修改
//1,规范各个函数接口
//2,修正1.2版本在VS1053模块上无法播放WMA,WAV的错误.
//3,加入芯片自动识别
//////////////////////////////////////////////////////////////////////////////////	 
 
//VS10XX设置参数
//0,高音控制；1，最低频限；2，低音控制；3，最低频限
//0,henh.1,hfreq.2,lenh.3,lfreq 5,主音量
u8 VS10XX_ID=0;
const u8 VS10XXram[5]={128,0,0,0,250};
									 
	   	   
//软复位VS10XX
//注意,在这里设置完后,系统SPI时钟被修改成9M左右
void VS_Soft_Reset(void)
{	 

	u8 retry; 
	   		   
	SPI2_SetSpeed(SPI_SPEED_256);   	//256分频 超低速 281.25Khz	
	while(VS_DQ_READ==0);					//等待空闲
	SPI2_ReadWriteByte(0X00);       	//启动传输	 
	retry=0; 					
	VS10XX_ID=VS_RD_Reg(SPI_STATUS);//读取状态寄存器
	VS10XX_ID>>=4;					//得到VS10XX的芯片信号 
	
//	printf("VS1053ID:%d",VS10XX_ID);
	if(VS10XX_ID==VS1053)VS_WR_Cmd(SPI_MODE,0x0816);//软件复位,新模式
	else VS_WR_Cmd(SPI_MODE,0x0804);				//软件复位,新模式

	while((VS_DQ_READ==0)&&retry<200)			//等待DREQ为高
	{
		retry++;
		delay_us(50);
	}; 	
	retry=0;
	while(VS_RD_Reg(SPI_CLOCKF)!=0X9800)//等待设置成功
	{
		VS_WR_Cmd(SPI_CLOCKF,0X9800);  	//设置VS10XX的时钟,3.5倍频 ,2xADD  
		if(retry++>100)break; 
	}		   				
 	VS_Rst_DecodeTime();				//复位解码时间	    
    //向VS10XX发送4个字节无效数据，用以启动SPI发送
 	SPI2_SetSpeed(SPI_SPEED_8);			//8分频速度	  
    VS_XDCS_CLR;							//选中数据传输 记得,这里一定要传送0X00
	SPI2_ReadWriteByte(0X0);
	SPI2_ReadWriteByte(0X0);
	SPI2_ReadWriteByte(0X0);
	SPI2_ReadWriteByte(0X0);
	VS_XDCS_SET;							//取消数据传输								    
} 
//硬复位MP3
//返回1:复位失败
//返回0:复位成功
u8 VS_HD_Reset(void)
{
	u8 retry=0;
	VS_XDCS_SET;	//取消数据传输
	VS_XCS_SET;	//取消指令传输
	VS_RST_CLR;			    
	delay_ms(100);  
	VS_RST_SET;    
	while((VS_DQ_READ==0)&&retry<200)//等待DREQ为高
	{
		retry++;
		delay_us(50); 	 
	}; 	
		   
	delay_ms(20);	
	if(retry>=200)return 1;
	else return 0;
		    		 
}

//正弦测试 
void VS_Sine_Test(void)
{										    
	VS_HD_Reset();	 
	VS_WR_Cmd(0x0b,0X2020);	  //设置音量	 
 	VS_WR_Cmd(SPI_MODE,0x0820);//进入VS10XX的测试模式   
	while(VS_DQ_READ==0);     //等待DREQ为高
	//printf("mode sin:%x\n",VS_RD_Reg(SPI_MODE));
 	//向VS10XX发送正弦测试命令：0x53 0xef 0x6e n 0x00 0x00 0x00 0x00
 	//其中n = 0x24, 设定VS10XX所产生的正弦波的频率值，具体计算方法见VS10XX的datasheet
  	SPI2_SetSpeed(SPI_SPEED_64);//低速 
	VS_XDCS_CLR;//选中数据传输
	SPI2_ReadWriteByte(0x53);
	SPI2_ReadWriteByte(0xef);
	SPI2_ReadWriteByte(0x6e);
	SPI2_ReadWriteByte(0x24);
	SPI2_ReadWriteByte(0x00);
	SPI2_ReadWriteByte(0x00);
	SPI2_ReadWriteByte(0x00);
	SPI2_ReadWriteByte(0x00);
	delay_ms(500);

	 
	VS_XDCS_SET; 
    //退出正弦测试
    VS_XDCS_CLR;//选中数据传输
	SPI2_ReadWriteByte(0x45);
	SPI2_ReadWriteByte(0x78);
	SPI2_ReadWriteByte(0x69);
	SPI2_ReadWriteByte(0x74);
	SPI2_ReadWriteByte(0x00);
	SPI2_ReadWriteByte(0x00);
	SPI2_ReadWriteByte(0x00);
	SPI2_ReadWriteByte(0x00);
	delay_ms(500);

	VS_XDCS_SET;		 


    //再次进入正弦测试并设置n值为0x44，即将正弦波的频率设置为另外的值
    VS_XDCS_CLR;//选中数据传输      
	SPI2_ReadWriteByte(0x53);
	SPI2_ReadWriteByte(0xef);
	SPI2_ReadWriteByte(0x6e);
	SPI2_ReadWriteByte(0x44);
	SPI2_ReadWriteByte(0x00);
	SPI2_ReadWriteByte(0x00);
	SPI2_ReadWriteByte(0x00);
	SPI2_ReadWriteByte(0x00);
	delay_ms(1000);


	VS_XDCS_SET;

	
    //退出正弦测试
    VS_XDCS_CLR;//选中数据传输
	SPI2_ReadWriteByte(0x45);
	SPI2_ReadWriteByte(0x78);
	SPI2_ReadWriteByte(0x69);
	SPI2_ReadWriteByte(0x74);
	SPI2_ReadWriteByte(0x00);
	SPI2_ReadWriteByte(0x00);
	SPI2_ReadWriteByte(0x00);
	SPI2_ReadWriteByte(0x00);


	delay_ms(100);
	VS_XDCS_SET;	 
} 	  
//VS10XX ram 测试 	
//返回值:
//对1053:0X83FF,表明全部测试OK
//对1003:0x807F,表明全部测试OK																			 
u16 VS_Ram_Test(void)
{	
	
	u16 RAM_Read;				 
	VS_HD_Reset();     		   
 	VS_WR_Cmd(SPI_MODE,0x0820);		// 进入VS10XX的RAM测试模式
	while (VS_DQ_READ==0); 				// 等待DREQ为高	   
	SPI2_SetSpeed(SPI_SPEED_256);   //256分频速度	     
	VS_XDCS_CLR;												   	 
	SPI2_ReadWriteByte(0x4d);
	SPI2_ReadWriteByte(0xea);
	SPI2_ReadWriteByte(0x6d);
	SPI2_ReadWriteByte(0x54);
	SPI2_ReadWriteByte(0x00);
	SPI2_ReadWriteByte(0x00);
	SPI2_ReadWriteByte(0x00);
	SPI2_ReadWriteByte(0x00);
	VS_XDCS_SET;		  
	delay_ms(200);  
	RAM_Read=VS_RD_Reg(SPI_HDAT0);
	// 如果得到的值为0x807F，则表明完好。			 
//	printf("SPI_HDAT0:%x\n",RAM_Read);//输出结果 
	return RAM_Read;
}		   
//向VS10XX写命令
//address:命令地址
//data:命令数据
void VS_WR_Cmd(u8 address,u16 data)
{

	u16 retry=0;

    while((VS_DQ_READ==0)&&(retry++)<0xfffe)retry++;	//等待空闲   
	
	 
	SPI2_SetSpeed(SPI_SPEED_64);    			//64分频速度	     
	VS_XDCS_SET; 
	VS_XCS_CLR;  	   
	SPI2_ReadWriteByte(VS_WRITE_COMMAND);//发送VS10XX的写命令
	SPI2_ReadWriteByte(address); //地址
	SPI2_ReadWriteByte(data>>8); //发送高八位
	SPI2_ReadWriteByte(data);	 //第八位
	VS_XCS_SET;               
	SPI2_SetSpeed(SPI_SPEED_8);  //8分频速度


} 
//向VS10XX写数据
void VS_WR_Data(u8 data)
{
	VS_XDCS_CLR;    
	SPI2_ReadWriteByte(data);
	VS_XDCS_SET; 
	 	     
}         
//读VS10XX的寄存器     
//注意不要用倍速读取,会出错
u16 VS_RD_Reg(u8 address)
{ 

	u16 temp=0; 
	u8 retry=0;

    while((VS_DQ_READ==0)&&(retry++)<0XFE);	//等待空闲	  
	if(retry>=0XFE)return 0;	 						    
	SPI2_SetSpeed(SPI_SPEED_64);		//64分频速度	
	VS_XDCS_SET;       
	VS_XCS_CLR;       
	SPI2_ReadWriteByte(VS_READ_COMMAND);//发送VS10XX的读命令
	SPI2_ReadWriteByte(address);        //地址
	temp=SPI2_ReadWriteByte(0xff);		//读取高字节
	temp=temp<<8;
	temp+=SPI2_ReadWriteByte(0xff); 	//读取低字节
	VS_XCS_SET;      
	SPI2_SetSpeed(SPI_SPEED_8);			//8分频速度	
    return temp; 
}  
//读取VS10XX的RAM
u16 VS_RD_Wram(u16 addr) 
{ 			   	  
	VS_WR_Cmd(SPI_WRAMADDR, addr); 
	return VS_RD_Reg(SPI_WRAM);  
} 
  
//FOR WAV HEAD0 :0X7761 HEAD1:0X7665    
//FOR MIDI HEAD0 :other info HEAD1:0X4D54
//FOR WMA HEAD0 :data speed HEAD1:0X574D
//FOR MP3 HEAD0 :data speed HEAD1:ID
//比特率预定值
const u16 bitrate[2][16]=
{ 
{0,8,16,24,32,40,48,56,64,80,96,112,128,144,160,0}, 
{0,32,40,48,56,64,80,96,112,128,160,192,224,256,320,0}
};
//返回Kbps的大小
//得到mp3&wma的波特率
u16 VS_Get_HeadInfo(void)
{

	unsigned int HEAD0;
	unsigned int HEAD1; 
	
	           
    HEAD0=VS_RD_Reg(SPI_HDAT0); 
    HEAD1=VS_RD_Reg(SPI_HDAT1);
    switch(HEAD1)
    {        
        case 0x7665:	return 0;//WAV格式
        case 0X4D54:			//MIDI格式 
		case 0X4154:			//AAC_ADTS
		case 0X4144:			//AAC_ADIF
		case 0X4D34:			//AAC_MP4/M4A
		case 0X4F67:			//OGG
        case 0X574D:			//WMA格式
        {
		//	printf("HEAD0:%d\n",HEAD0);
            HEAD1=HEAD0*2/25;					//相当于*8/100

            if((HEAD1%10)>5)return HEAD1/10+1;	//对小数点第一位四舍五入
            else return HEAD1/10;
        }
        default://MP3格式
        {
            HEAD1>>=3;
            HEAD1=HEAD1&0x03; 
            if(HEAD1==3)HEAD1=1;
            else HEAD1=0;
	
            return bitrate[HEAD1][HEAD0>>12];
        }
    } 
}  
//重设解码时间                          
void VS_Rst_DecodeTime(void)
{
	VS_WR_Cmd(SPI_DECODE_TIME,0x0000);
	VS_WR_Cmd(SPI_DECODE_TIME,0x0000);//操作两次
}
//得到mp3的播放时间n sec
u16 VS_Get_DecodeTime(void)
{ 

    return VS_RD_Reg(SPI_DECODE_TIME);   
} 	 

//加载用户代码到VS10XX
//FLAC解码.但是加载后不支持WMA.
//写入plugin  
void VS_Load_FlacPatch(void) 
{	 
 
	u16 i; 
	u16 addr, n, val; 
				  			   
	for(i=0;i<VS1053B_PATCHLEN;) 
	{ 
		addr = vs1053b_patch[i++]; 
		n    = vs1053b_patch[i++]; 
		if(n & 0x8000U) //RLE run, replicate n samples 
		{ 
			n  &= 0x7FFF; 
			val = vs1053b_patch[i++]; 
			while(n--)VS_WR_Cmd(addr, val);  
		}else //copy run, copy n sample 
		{ 
			while(n--) 
			{ 
				val = vs1053b_patch[i++]; 
				VS_WR_Cmd(addr, val); 
			} 
		} 
	} 		   
}  	     		   
//设定VS10XX播放的音量和高低音 
void set10XX(void)
{

    u8 t;
    u16 bass=0; //暂存音调寄存器值
    u16 volt=0; //暂存音量值
    u8 vset=0;  //暂存音量值 
			 
    vset=255-VS10XXram[4];//取反一下,得到最大值,表示最大的表示 
    volt=vset;
    volt<<=8;
    volt+=vset;//得到音量设置后大小
     //0,henh.1,hfreq.2,lenh.3,lfreq        
    for(t=0;t<4;t++)
    {
        bass<<=4;
        bass+=VS10XXram[t]; 
    }  
	      
	VS_WR_Cmd(SPI_BASS,bass);//BASS   
    VS_WR_Cmd(SPI_VOL,volt); //设音量 
} 
//初始化VS10XX的IO口	 
void VS_Init(void)
{

    GPIO_InitTypeDef  GPIO_InitStructure;					 //定义GPIO初始化结构体	
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOG, ENABLE);	 //使能PB,PG端口时钟
	
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_12;	 //PB5 PB6	PB12	
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		//IO口速度50MHZ
    GPIO_Init(GPIOB, &GPIO_InitStructure);					
    GPIO_SetBits(GPIOB,GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_12); //输出高	 

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;				 //PB7
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		    //上拉输入
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		//IO口速度50MHZ
    GPIO_Init(GPIOB, &GPIO_InitStructure);	

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;				  //PG11
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		//IO口速度50MHZ
    GPIO_Init(GPIOG, &GPIO_InitStructure);					
    GPIO_SetBits(GPIOG,GPIO_Pin_11);                        //输出高	 

	SPI2_Init();								//初始化SPI


	 
} 	 












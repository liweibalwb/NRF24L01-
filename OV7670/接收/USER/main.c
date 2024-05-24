/*

*/

#include	"main.h"

extern u8 ov_frame;	//在timer.c里面定义	  

FATFS fs;
FIL	file;


int main(void)
{
  int countx;
	int county;
	
	u32 count=0;
	u32 receivecount=0;
  u16 CMOS_Data=0;	
	u8* showbuf;
  SystemInit(); 			 						//系统时钟初始化为72M	  SYSCLK_FREQ_72MHz	 
  delay_init(72); 			 						//初始化延时函数
  uart_init(115200);			 						//初始化串口1波特率9600
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//设置NVIC中断分组2:2位抢占优先级，2位响应优先级
  LED_Init();										//初始化LED
//  KEY_Init();										//初始化按键
//  Buzzer_Init();									//初始化蜂鸣器
//  AT24CXX_Init();                                   //初始化24C02
 // SPI_Flash_Init();									//初始化W25X16
  FSMC_SRAM_Init();									//初始化IS62LV25616
  TFT_Init();										//初始化TFT
	
	NRF24L01_Init();    	//初始化NRF24L01 
	if(NRF24L01_Check())	//检查24L01是否存在
	{
			printf("NRF24L01不存在！\r\n");
	}else{
		
		printf("NRF24L01存在！\r\n");
	}	
//  Touch_Init();		                                //初始化触摸屏XTP2046
//  SD_Init();										//初始化SD卡
 // CH375_Init();										//初始化CH375
//  VS_Init();										//初始化VS1053
   mem_init();										//初始化外部SRAM内存池
//  f_mount(0,&fs);									//挂载FATFS文件系统
 // if(Font_Init()){		  							//如果字库不存在则更新字库
 // 	Updata_Font();
 // }
// 	if(!RTC_Init()){
// 			printf("初始化RTC成功！\r\n");
// 	}else{
// 			printf("初始化RTC失败！\r\n");
// 	}
//   TIM6_Int_Init(10000,7199);			//10Khz计数频率,1秒钟中断	
// 	
	
	showbuf = (u8*)mem_malloc(240*320*2+5);
	if(!showbuf){
			printf("申请内存失败!\r\n");
	}
  NRF24L01_TX_Mode();//配置为发送模式
  NRF24L01_TxPacket(showbuf);	//发送同步信号	通知开始采集图像
	LED0_CLR;
	delay_ms(1000);
	LED0_SET;
	delay_ms(1000);
	
	NRF24L01_RX_Mode();//配置为接收模式
	
	while(1)
	{		
			while(NRF24L01_RxPacket(showbuf)!=0);	//等待数据接收完毕
			showbuf +=32;
			count=0;
			receivecount +=32;
			if(receivecount>=(240*320*2)){
				 showbuf -= 240*320*2;
				 receivecount=0;
				 count=0;
				 for( county = 0; county <240; county++){
					TFT_SetCursor(0, county); 
					TFT_WriteRAM_Prepare(); /* Prepare to write GRAM */  
					for(countx=0; countx<320;countx++){		
						CMOS_Data = showbuf[count];
						count++;
						CMOS_Data <<=8;
						CMOS_Data |= showbuf[count];
						count++;
						TFT->TFT_RAM = CMOS_Data;	
					 }
				 }
				 
				 NRF24L01_TX_Mode();//配置为发送模式
				 NRF24L01_TxPacket(showbuf);	//发送同步信号 通知采集下一帧
				 printf("发送同步信号成功!\r\n");
		//		 LED0_CLR;
	//			 delay_ms(200);
	//			 LED0_SET;
	//			 delay_ms(200);
				 
				 NRF24L01_RX_Mode();//配置为接收模式
			}
	}	 
			 
}


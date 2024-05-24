/*
***************************************************************	
*注意：本程序只供学习使用，未经作者许可，不得用于其它任何用途
*描述：VS1053和显示LCD测试代码
*版本：V1.0
*作者：OneTree
*创建日期：2012/11/9
*修改日期：2012/11/9 
*版权所有，盗版必究。
*Copyright(C) OneTree 2012-2022
*All rights reserved
***************************************************************
*硬件连接：
		
***************************************************************
*修改记录：

	
*/

#include	"main.h"

extern u8 ov_frame;	//在timer.c里面定义	  

FATFS fs;
FIL	file;


int main(void)
{
  u32 count;
	u32 sendcount=0;
  u8 tmp_buf[32];		
  SystemInit(); 			 						//系统时钟初始化为72M	  SYSCLK_FREQ_72MHz	 
  delay_init(72); 			 						//初始化延时函数
  uart_init(9600);			 						//初始化串口1波特率9600
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
// mem_init();										//初始化外部SRAM内存池
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
  FIFO_GPIO_Configuration();
	
	FIFO_CS_H();
  FIFO_CS_L();
  FIFO_WE_L();
	
  while( 1 != Sensor_Init() );

	
	 FIFO_RRST_L(); 
	 FIFO_RCLK_L();

	 FIFO_RCLK_H();
	 FIFO_RCLK_L();

	 FIFO_RRST_H();    
	 FIFO_RCLK_H();

  Vsync = 0;

	NRF24L01_RX_Mode();//配置为接收模式
	while(NRF24L01_RxPacket(tmp_buf)!=0);	//等待同步信号
	LED0_CLR;
	delay_ms(1000);
	LED0_SET;
  delay_ms(1000);	
	
	while(1)
	{		
		
    if( Vsync == 2 )
    { 	
			 NRF24L01_TX_Mode();//配置为发送模式
			
       FIFO_RRST_L();
			
       FIFO_RCLK_L();
       FIFO_RCLK_H();
			 FIFO_RCLK_L();	
			
			
       FIFO_RRST_H();    
       FIFO_RCLK_H();
			
			for( count = 0; count <240*320*2; count++){
					FIFO_RCLK_L(); 
				  tmp_buf[sendcount] = (u8)((GPIOC->IDR)& 0x00ff);
					sendcount++;
					FIFO_RCLK_H();	
					if(sendcount>=32){
						sendcount=0;
						while(NRF24L01_TxPacket(tmp_buf)!=TX_OK);	//等待数据发送完毕
					}	
			}	
			Vsync = 0;	
			NRF24L01_RX_Mode();//配置为接收模式
			while(NRF24L01_RxPacket(tmp_buf)!=0);	//等待同步信号
			printf("收到同步信号！\r\n");
	//		LED0_CLR;
//			delay_ms(200);
	//		LED0_SET;
	//		delay_ms(200);			
	//		ov_frame++; 
		} 
	}
 }


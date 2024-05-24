/*
***************************************************************	
*ע�⣺������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
*������VS1053����ʾLCD���Դ���
*�汾��V1.0
*���ߣ�OneTree
*�������ڣ�2012/11/9
*�޸����ڣ�2012/11/9 
*��Ȩ���У�����ؾ���
*Copyright(C) OneTree 2012-2022
*All rights reserved
***************************************************************
*Ӳ�����ӣ�
		
***************************************************************
*�޸ļ�¼��

	
*/

#include	"main.h"

extern u8 ov_frame;	//��timer.c���涨��	  

FATFS fs;
FIL	file;


int main(void)
{
  u32 count;
	u32 sendcount=0;
  u8 tmp_buf[32];		
  SystemInit(); 			 						//ϵͳʱ�ӳ�ʼ��Ϊ72M	  SYSCLK_FREQ_72MHz	 
  delay_init(72); 			 						//��ʼ����ʱ����
  uart_init(9600);			 						//��ʼ������1������9600
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
  LED_Init();										//��ʼ��LED
//  KEY_Init();										//��ʼ������
//  Buzzer_Init();									//��ʼ��������
//  AT24CXX_Init();                                   //��ʼ��24C02
 // SPI_Flash_Init();									//��ʼ��W25X16
  FSMC_SRAM_Init();									//��ʼ��IS62LV25616
  TFT_Init();										//��ʼ��TFT
	
	NRF24L01_Init();    	//��ʼ��NRF24L01 
	 if(NRF24L01_Check())	//���24L01�Ƿ����
	{
			printf("NRF24L01�����ڣ�\r\n");
	}else{
		
		printf("NRF24L01���ڣ�\r\n");
	}
	
//  Touch_Init();		                                //��ʼ��������XTP2046
//  SD_Init();										//��ʼ��SD��
 // CH375_Init();										//��ʼ��CH375
//  VS_Init();										//��ʼ��VS1053
// mem_init();										//��ʼ���ⲿSRAM�ڴ��
//  f_mount(0,&fs);									//����FATFS�ļ�ϵͳ
 // if(Font_Init()){		  							//����ֿⲻ����������ֿ�
 // 	Updata_Font();
 // }
// 	if(!RTC_Init()){
// 			printf("��ʼ��RTC�ɹ���\r\n");
// 	}else{
// 			printf("��ʼ��RTCʧ�ܣ�\r\n");
// 	}
//   TIM6_Int_Init(10000,7199);			//10Khz����Ƶ��,1�����ж�	
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

	NRF24L01_RX_Mode();//����Ϊ����ģʽ
	while(NRF24L01_RxPacket(tmp_buf)!=0);	//�ȴ�ͬ���ź�
	LED0_CLR;
	delay_ms(1000);
	LED0_SET;
  delay_ms(1000);	
	
	while(1)
	{		
		
    if( Vsync == 2 )
    { 	
			 NRF24L01_TX_Mode();//����Ϊ����ģʽ
			
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
						while(NRF24L01_TxPacket(tmp_buf)!=TX_OK);	//�ȴ����ݷ������
					}	
			}	
			Vsync = 0;	
			NRF24L01_RX_Mode();//����Ϊ����ģʽ
			while(NRF24L01_RxPacket(tmp_buf)!=0);	//�ȴ�ͬ���ź�
			printf("�յ�ͬ���źţ�\r\n");
	//		LED0_CLR;
//			delay_ms(200);
	//		LED0_SET;
	//		delay_ms(200);			
	//		ov_frame++; 
		} 
	}
 }


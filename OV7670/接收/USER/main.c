/*

*/

#include	"main.h"

extern u8 ov_frame;	//��timer.c���涨��	  

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
  SystemInit(); 			 						//ϵͳʱ�ӳ�ʼ��Ϊ72M	  SYSCLK_FREQ_72MHz	 
  delay_init(72); 			 						//��ʼ����ʱ����
  uart_init(115200);			 						//��ʼ������1������9600
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
   mem_init();										//��ʼ���ⲿSRAM�ڴ��
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
	
	showbuf = (u8*)mem_malloc(240*320*2+5);
	if(!showbuf){
			printf("�����ڴ�ʧ��!\r\n");
	}
  NRF24L01_TX_Mode();//����Ϊ����ģʽ
  NRF24L01_TxPacket(showbuf);	//����ͬ���ź�	֪ͨ��ʼ�ɼ�ͼ��
	LED0_CLR;
	delay_ms(1000);
	LED0_SET;
	delay_ms(1000);
	
	NRF24L01_RX_Mode();//����Ϊ����ģʽ
	
	while(1)
	{		
			while(NRF24L01_RxPacket(showbuf)!=0);	//�ȴ����ݽ������
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
				 
				 NRF24L01_TX_Mode();//����Ϊ����ģʽ
				 NRF24L01_TxPacket(showbuf);	//����ͬ���ź� ֪ͨ�ɼ���һ֡
				 printf("����ͬ���źųɹ�!\r\n");
		//		 LED0_CLR;
	//			 delay_ms(200);
	//			 LED0_SET;
	//			 delay_ms(200);
				 
				 NRF24L01_RX_Mode();//����Ϊ����ģʽ
			}
	}	 
			 
}


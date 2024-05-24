#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "lcd.h"
#include "usart.h"
#include "24l01.h"
#include "sram.h"
#include "malloc.h"

/************************************************
 ALIENTEKս��STM32������ʵ��33
 ����ͨ�� ʵ��
 ����֧�֣�www.openedv.com
 �Ա����̣�http://eboard.taobao.com
 ��ע΢�Ź���ƽ̨΢�źţ�"����ԭ��"����ѻ�ȡSTM32���ϡ�
 ������������ӿƼ����޹�˾
 ���ߣ�����ԭ�� @ALIENTEK
************************************************/

#define  OV7725_WINDOW_WIDTH		320 // <=320
#define  OV7725_WINDOW_HEIGHT		240 // <=240

int main(void)
{
    int countx;
    int county;
    u8* showbuf;
    u8 key,mode;
    u16 t=0;
    u16 CMOS_Data=0;
    u32 count=0;
    u32 receivecount=0;
    u8 sramx=1;				//Ĭ��Ϊ�ڲ�sram
    u8 tmp_buf[33];
    delay_init();	    	 //��ʱ������ʼ��
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//�����ж����ȼ�����Ϊ��2��2λ��ռ���ȼ���2λ��Ӧ���ȼ�
    uart_init(115200);	 	//���ڳ�ʼ��Ϊ115200
    LED_Init();		  			//��ʼ����LED���ӵ�Ӳ���ӿ�
    KEY_Init();					//��ʼ������
    LCD_Init();			   		//��ʼ��LCD
    NRF24L01_Init();    		//��ʼ��NRF24L01
    POINT_COLOR=RED;			//��������Ϊ��ɫ
    //��ʼ���ⲿSRAM�ڴ��
    LCD_ShowString(30,50,200,16,16,"WarShip STM32");
    LCD_ShowString(30,70,200,16,16,"NRF24L01 Rev");
    LCD_ShowString(30,90,200,16,16,"ATOM@ALIENTEK");
    LCD_ShowString(30,110,200,16,16,"2024/5/25");
    FSMC_SRAM_Init();			//��ʼ���ⲿSRAM
    my_mem_init(SRAMIN);		//��ʼ���ڲ��ڴ��
    my_mem_init(SRAMEX);		//��ʼ���ⲿ�ڴ��


    showbuf=mymalloc(sramx,240*320*2+5);//����2K�ֽ�

    if(showbuf==NULL)
    {
        printf("�����ڴ�ʧ��!\r\n");
    }
    else
    {
        printf("�����ڴ�ɹ�!\r\n");
    }

    while(NRF24L01_Check())
    {
        LCD_ShowString(30,130,200,16,16,"NRF24L01 Error");
        delay_ms(200);
        LCD_Fill(30,130,239,130+16,WHITE);
        delay_ms(200);
    }
    LCD_ShowString(30,130,200,16,16,"NRF24L01 OK");
    while(1)
    {
        key=KEY_Scan(0);
        if(key==KEY0_PRES)
        {
            mode=0;
            break;
        } else if(key==KEY1_PRES)
        {
            mode=1;
            break;
        }
        t++;
        if(t==100)LCD_ShowString(10,150,230,16,16,"KEY0:RX_Mode  KEY1:TX_Mode"); //��˸��ʾ��ʾ��Ϣ
        if(t==200)
        {
            LCD_Fill(10,150,230,150+16,WHITE);
            t=0;
        }
        delay_ms(5);
    }
    LCD_Fill(10,150,240,166,WHITE);//����������ʾ
    POINT_COLOR=BLUE;//��������Ϊ��ɫ
    if(mode==0)//RXģʽ
    {
        LCD_ShowString(30,150,200,16,16,"NRF24L01 RX_Mode");
        LCD_ShowString(30,170,200,16,16,"Received DATA:");
        NRF24L01_RX_Mode();
        while(1)
        {
//            if(NRF24L01_RxPacket(tmp_buf)==0)//һ�����յ���Ϣ,����ʾ����.
//            {
//                tmp_buf[32]=0;//�����ַ���������
//                LCD_ShowString(0,190,lcddev.width-1,32,16,tmp_buf);
//            } else
//                delay_us(100);

            while(NRF24L01_RxPacket(showbuf)!=0);	//�ȴ����ݽ������
            showbuf +=32;
            count=0;
            receivecount +=32;

            LCD_Scan_Dir(U2D_L2R);//���ϵ���,������
            LCD_Set_Window((lcddev.width-OV7725_WINDOW_WIDTH)/2,(lcddev.height-OV7725_WINDOW_HEIGHT)/2,OV7725_WINDOW_WIDTH,OV7725_WINDOW_HEIGHT);//����ʾ�������õ���Ļ����
            if(lcddev.id==0X1963)
                LCD_Set_Window((lcddev.width-OV7725_WINDOW_WIDTH)/2,(lcddev.height-OV7725_WINDOW_HEIGHT)/2,OV7725_WINDOW_HEIGHT,OV7725_WINDOW_WIDTH);//����ʾ�������õ���Ļ����
            LCD_WriteRAM_Prepare();     //��ʼд��GRAM

            if(receivecount>=(240*320*2)) {
                showbuf -= 240*320*2;
                receivecount=0;
                count=0;
                for( county = 0; county <240; county++) {
//								TFT_SetCursor(0, county);
//								TFT_WriteRAM_Prepare(); /* Prepare to write GRAM */
                    for(countx=0; countx<320; countx++) {
                        CMOS_Data = showbuf[count];
                        count++;
                        CMOS_Data <<=8;
                        CMOS_Data |= showbuf[count];
                        count++;
                        //	TFT->TFT_RAM = CMOS_Data;
                        LCD->LCD_RAM=CMOS_Data;
                    }
                }
								
								
                NRF24L01_TX_Mode();//����Ϊ����ģʽ
                NRF24L01_TxPacket(showbuf);	//����ͬ���ź� ֪ͨ�ɼ���һ֡
                printf("����ͬ���źųɹ�!\r\n");
                NRF24L01_RX_Mode();//����Ϊ����ģʽ
                t++;
                if(t==1)//��Լ1s�Ӹı�һ��״̬
                {
                    t=0;
                    LED0=!LED0;
                }
            }
        }
//        else//TXģʽ
//        {
//            LCD_ShowString(30,150,200,16,16,"NRF24L01 TX_Mode");
//            NRF24L01_TX_Mode();
//            mode=' ';//�ӿո����ʼ
//            while(1)
//            {
//                if(NRF24L01_TxPacket(tmp_buf)==TX_OK)
//                {
//                    LCD_ShowString(30,170,239,32,16,"Sended DATA:");
//                    LCD_ShowString(0,190,lcddev.width-1,32,16,tmp_buf);
//                    key=mode;
//                    for(t=0; t<32; t++)
//                    {
//                        key++;
//                        if(key>('~'))key=' ';
//                        tmp_buf[t]=key;
//                    }
//                    mode++;
//                    if(mode>'~')mode=' ';
//                    tmp_buf[32]=0;//���������
//                } else
//                {
//                    LCD_Fill(0,170,lcddev.width,170+16*3,WHITE);//�����ʾ
//                    LCD_ShowString(30,170,lcddev.width-1,32,16,"Send Failed ");
//                };
//                LED0=!LED0;
//                delay_ms(1500);
//            }
    }
}



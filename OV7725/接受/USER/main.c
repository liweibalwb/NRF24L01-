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
 ALIENTEK战舰STM32开发板实验33
 无线通信 实验
 技术支持：www.openedv.com
 淘宝店铺：http://eboard.taobao.com
 关注微信公众平台微信号："正点原子"，免费获取STM32资料。
 广州市星翼电子科技有限公司
 作者：正点原子 @ALIENTEK
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
    u8 sramx=1;				//默认为内部sram
    u8 tmp_buf[33];
    delay_init();	    	 //延时函数初始化
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置中断优先级分组为组2：2位抢占优先级，2位响应优先级
    uart_init(115200);	 	//串口初始化为115200
    LED_Init();		  			//初始化与LED连接的硬件接口
    KEY_Init();					//初始化按键
    LCD_Init();			   		//初始化LCD
    NRF24L01_Init();    		//初始化NRF24L01
    POINT_COLOR=RED;			//设置字体为红色
    //初始化外部SRAM内存池
    LCD_ShowString(30,50,200,16,16,"WarShip STM32");
    LCD_ShowString(30,70,200,16,16,"NRF24L01 Rev");
    LCD_ShowString(30,90,200,16,16,"ATOM@ALIENTEK");
    LCD_ShowString(30,110,200,16,16,"2024/5/25");
    FSMC_SRAM_Init();			//初始化外部SRAM
    my_mem_init(SRAMIN);		//初始化内部内存池
    my_mem_init(SRAMEX);		//初始化外部内存池


    showbuf=mymalloc(sramx,240*320*2+5);//申请2K字节

    if(showbuf==NULL)
    {
        printf("申请内存失败!\r\n");
    }
    else
    {
        printf("申请内存成功!\r\n");
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
        if(t==100)LCD_ShowString(10,150,230,16,16,"KEY0:RX_Mode  KEY1:TX_Mode"); //闪烁显示提示信息
        if(t==200)
        {
            LCD_Fill(10,150,230,150+16,WHITE);
            t=0;
        }
        delay_ms(5);
    }
    LCD_Fill(10,150,240,166,WHITE);//清空上面的显示
    POINT_COLOR=BLUE;//设置字体为蓝色
    if(mode==0)//RX模式
    {
        LCD_ShowString(30,150,200,16,16,"NRF24L01 RX_Mode");
        LCD_ShowString(30,170,200,16,16,"Received DATA:");
        NRF24L01_RX_Mode();
        while(1)
        {
//            if(NRF24L01_RxPacket(tmp_buf)==0)//一旦接收到信息,则显示出来.
//            {
//                tmp_buf[32]=0;//加入字符串结束符
//                LCD_ShowString(0,190,lcddev.width-1,32,16,tmp_buf);
//            } else
//                delay_us(100);

            while(NRF24L01_RxPacket(showbuf)!=0);	//等待数据接收完毕
            showbuf +=32;
            count=0;
            receivecount +=32;

            LCD_Scan_Dir(U2D_L2R);//从上到下,从左到右
            LCD_Set_Window((lcddev.width-OV7725_WINDOW_WIDTH)/2,(lcddev.height-OV7725_WINDOW_HEIGHT)/2,OV7725_WINDOW_WIDTH,OV7725_WINDOW_HEIGHT);//将显示区域设置到屏幕中央
            if(lcddev.id==0X1963)
                LCD_Set_Window((lcddev.width-OV7725_WINDOW_WIDTH)/2,(lcddev.height-OV7725_WINDOW_HEIGHT)/2,OV7725_WINDOW_HEIGHT,OV7725_WINDOW_WIDTH);//将显示区域设置到屏幕中央
            LCD_WriteRAM_Prepare();     //开始写入GRAM

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
								
								
                NRF24L01_TX_Mode();//配置为发送模式
                NRF24L01_TxPacket(showbuf);	//发送同步信号 通知采集下一帧
                printf("发送同步信号成功!\r\n");
                NRF24L01_RX_Mode();//配置为接收模式
                t++;
                if(t==1)//大约1s钟改变一次状态
                {
                    t=0;
                    LED0=!LED0;
                }
            }
        }
//        else//TX模式
//        {
//            LCD_ShowString(30,150,200,16,16,"NRF24L01 TX_Mode");
//            NRF24L01_TX_Mode();
//            mode=' ';//从空格键开始
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
//                    tmp_buf[32]=0;//加入结束符
//                } else
//                {
//                    LCD_Fill(0,170,lcddev.width,170+16*3,WHITE);//清空显示
//                    LCD_ShowString(30,170,lcddev.width-1,32,16,"Send Failed ");
//                };
//                LED0=!LED0;
//                delay_ms(1500);
//            }
    }
}



/*
***************************************************************
*Ӳ�����ӣ� 
		FSMC_D0 ��PD14			FSMC_D1 ��PD15
		FSMC_D2 ��PE0			FSMC_D3 ��PD1
		FSMC_D4 ��PE7			FSMC_D5 ��PE8
		FSMC_D6 ��PE9			FSMC_D7 ��PE10
		FSMC_D8 ��PE11			FSMC_D9 ��PE12
		FSMC_D10��PE13			FSMC_D11��PE14
		FSMC_D12��PE15			FSMC_D13��PD8
		FSMC_D14��PD9			FSMC_D15��PD10
	    FSMC_NE4��PG12			FSMC_NWE��PD5		
		FSMC_A0 ��PF0			FSMC_NOE��PD4
		REST    ��NRST(STM32�ĸ�λ����)
***************************************************************
*�޸ļ�¼��

	
*/
#include    "tft.h"
#include    "font.h"  



					 
//������ɫ,������ɫ
u16 POINT_COLOR = 0x0000,BACK_COLOR = 0xFFFF;  
u16 DeviceCode;	 


/*
******************************************************************************
* Function Name  : TFT_CtrlLinesConfig
* Description    : Configures TFT Control lines (FSMC Pins) in alternate function
                   Push-Pull mode.
* Input          : None
* Output         : None
* Return         : None
******************************************************************************
*/
void TFT_CtrlLinesConfig(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  /* Enable FSMC, GPIOD, GPIOE, GPIOF, GPIOG and AFIO clocks */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC, ENABLE);

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE |
                         RCC_APB2Periph_GPIOF | RCC_APB2Periph_GPIOG |
                         RCC_APB2Periph_AFIO, ENABLE);

  /* Set PD.00(D2), PD.01(D3), PD.04(NOE), PD.05(NWE), PD.08(D13), PD.09(D14),
     PD.10(D15), PD.14(D0), PD.15(D1) as alternate 
     function push pull */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_4 | GPIO_Pin_5 |
                                GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_14 | 
                                GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOD, &GPIO_InitStructure);

  /* Set PE.07(D4), PE.08(D5), PE.09(D6), PE.10(D7), PE.11(D8), PE.12(D9), PE.13(D10),
     PE.14(D11), PE.15(D12) as alternate function push pull */
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | 
                                GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | 
                                GPIO_Pin_15;
  GPIO_Init(GPIOE, &GPIO_InitStructure);

  /* Set PF.00(A0 (RS)) as alternate function push pull */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  GPIO_Init(GPIOF, &GPIO_InitStructure);

  /* Set PG.12(NE4 (LCD/CS)) as alternate function push pull - CE3(LCD /CS) */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
  GPIO_Init(GPIOG, &GPIO_InitStructure);

  
  /* LEDs pins configuration */

  /* Set PD7 to disable NAND */


}

/*
******************************************************************************
* Function Name  : TFT_FSMCConfig
* Description    : Configures the Parallel interface (FSMC) for TFT(Parallel mode)
* Input          : None
* Output         : None
* Return         : None
******************************************************************************
*/
void TFT_FSMCConfig(void)
{
	FSMC_NORSRAMInitTypeDef  FSMC_NORSRAMInitStructure;
    FSMC_NORSRAMTimingInitTypeDef  readWriteTiming; 
	FSMC_NORSRAMTimingInitTypeDef  writeTiming;

	readWriteTiming.FSMC_AddressSetupTime = 0x01;	 //��ַ����ʱ�䣨ADDSET��Ϊ2��HCLK 1/36M=27ns
    readWriteTiming.FSMC_AddressHoldTime = 0x00;	 //��ַ����ʱ�䣨ADDHLD��ģʽAδ�õ�	
    readWriteTiming.FSMC_DataSetupTime = 0x0f;		 // ���ݱ���ʱ��Ϊ16��HCLK,��ΪҺ������IC�Ķ����ݵ�ʱ���ٶȲ���̫�죬�����1289���IC��
    readWriteTiming.FSMC_BusTurnAroundDuration = 0x00;
    readWriteTiming.FSMC_CLKDivision = 0x00;
    readWriteTiming.FSMC_DataLatency = 0x00;
    readWriteTiming.FSMC_AccessMode = FSMC_AccessMode_A;	 //ģʽA 
    

	writeTiming.FSMC_AddressSetupTime = 0x00;	 //��ַ����ʱ�䣨ADDSET��Ϊ1��HCLK  
    writeTiming.FSMC_AddressHoldTime = 0x00;	 //��ַ����ʱ�䣨A		
    writeTiming.FSMC_DataSetupTime = 0x03;		 ////���ݱ���ʱ��Ϊ4��HCLK	
    writeTiming.FSMC_BusTurnAroundDuration = 0x00;
    writeTiming.FSMC_CLKDivision = 0x00;
    writeTiming.FSMC_DataLatency = 0x00;
    writeTiming.FSMC_AccessMode = FSMC_AccessMode_A;	 //ģʽA 

 
    FSMC_NORSRAMInitStructure.FSMC_Bank = FSMC_Bank1_NORSRAM4;//  ��������ʹ��NE4 ��Ҳ�Ͷ�ӦBTCR[6],[7]��
    FSMC_NORSRAMInitStructure.FSMC_DataAddressMux = FSMC_DataAddressMux_Disable; // ���������ݵ�ַ
    FSMC_NORSRAMInitStructure.FSMC_MemoryType =FSMC_MemoryType_SRAM;// FSMC_MemoryType_SRAM;  //SRAM   
    FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_16b;//�洢�����ݿ��Ϊ16bit   
    FSMC_NORSRAMInitStructure.FSMC_BurstAccessMode =FSMC_BurstAccessMode_Disable;// FSMC_BurstAccessMode_Disable; 
    FSMC_NORSRAMInitStructure.FSMC_WaitSignalPolarity = FSMC_WaitSignalPolarity_Low;
	FSMC_NORSRAMInitStructure.FSMC_AsynchronousWait=FSMC_AsynchronousWait_Disable; 
    FSMC_NORSRAMInitStructure.FSMC_WrapMode = FSMC_WrapMode_Disable;   
    FSMC_NORSRAMInitStructure.FSMC_WaitSignalActive = FSMC_WaitSignalActive_BeforeWaitState;  
    FSMC_NORSRAMInitStructure.FSMC_WriteOperation = FSMC_WriteOperation_Enable;	//  �洢��дʹ��
    FSMC_NORSRAMInitStructure.FSMC_WaitSignal = FSMC_WaitSignal_Disable;   
    FSMC_NORSRAMInitStructure.FSMC_ExtendedMode = FSMC_ExtendedMode_Enable; // ��дʹ�ò�ͬ��ʱ��
    FSMC_NORSRAMInitStructure.FSMC_WriteBurst = FSMC_WriteBurst_Disable; 
    FSMC_NORSRAMInitStructure.FSMC_ReadWriteTimingStruct = &readWriteTiming; //��дʱ��
    FSMC_NORSRAMInitStructure.FSMC_WriteTimingStruct = &writeTiming;  //дʱ��

    FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure);  //��ʼ��FSMC����

   	FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM4, ENABLE);  // ʹ��BANK1 
     
}



//д�Ĵ�������
void TFT_WR_REG(u16 TFT_Reg)
{ 	
  TFT->TFT_REG = TFT_Reg;	  
}
	
//д����
void TFT_WR_DATA(u16 TFT_RegValue)
{
  TFT->TFT_RAM = TFT_RegValue;	
} 

//ָ���Ĵ���д����
void TFT_WriteReg(u16 TFT_Reg, u16 TFT_RegValue)
{	
  TFT->TFT_REG = TFT_Reg;
  TFT->TFT_RAM =TFT_RegValue;	    
  		 
}
	   
//���Ĵ���
u16 TFT_ReadReg(u16 TFT_Reg)
{										   

	TFT->TFT_REG = TFT_Reg;
	return (TFT->TFT_RAM);  
}


   
//��ʼдGRAM
void TFT_WriteRAM_Prepare(void)
{
	TFT->TFT_REG = R202;
}

	 
//TFTдGRAM
void TFT_WriteRAM(u16 RGB_Code)
{							    
	TFT_WR_DATA(RGB_Code);//дʮ��λGRAM
}


//����������ΪGBR��ʽ��������д���ʱ��ΪRGB��ʽ��
//ͨ���ú���ת��
//c:GBR��ʽ����ɫֵ
//����ֵ��RGB��ʽ����ɫֵ
u16 TFT_BGR2RGB(u16 c)
{
	u16  r,g,b,rgb;   
	b=(c>>0)&0x1f;
	g=(c>>5)&0x3f;
	r=(c>>11)&0x1f;	 
	rgb=(b<<11)+(g<<5)+(r<<0);		 
	return(rgb);
}		 
//��ȡ��ĳ�����ɫֵ	 
//x:0~239
//y:0~319
//����ֵ:�˵����ɫ
u16 TFT_ReadPoint(u16 x,u16 y)
{
	u16 t;	
	if(x>=TFT_W||y>=TFT_H)return 0;//�����˷�Χ,ֱ�ӷ���		   
	TFT_SetCursor(x,y);    
	TFT->TFT_REG = R202;	  		 //ѡ��GRAM��ַ
   t = TFT->TFT_RAM; 						//��Ҫ������
	 t = TFT->TFT_RAM; 
	 return t;
}
//TFT������ʾ
void TFT_DisplayOn(void)
{					   
	//TFT_WriteReg(R7, 0x0173); //26��ɫ��ʾ����
	TFT_LED_SET;
}	 
//TFT�ر���ʾ
void TFT_DisplayOff(void)
{	   
	//TFT_WriteReg(R7, 0x0);//�ر���ʾ 
	TFT_LED_CLR;
}   
//���ù��λ��
//Xpos:������
//Ypos:������
__inline void TFT_SetCursor(u16 Xpos, u16 Ypos)
{
#if USE_HORIZONTAL==1 
	TFT_WriteReg(R200, Ypos);
	TFT_WriteReg(R201, 399-Xpos);
							   
#else
	TFT_WriteReg(R200, Xpos);
	TFT_WriteReg(R201, Ypos);
						    
#endif
}  
//����
//x:0~239
//y:0~319
//POINT_COLOR:�˵����ɫ
void TFT_DrawPoint(u16 x,u16 y,u16 color)
{
	TFT_SetCursor(x,y);//���ù��λ�� 
	TFT_WR_REG(R202);//��ʼд��GRAM
	TFT_WR_DATA(color); 
} 	 
//��ʼ��TFT
//�ó�ʼ���������Գ�ʼ������ILI93XXҺ��,�������������ǻ���ILI9320��!!!
//�������ͺŵ�����оƬ��û�в���! 
void TFT_Init(void)
{ 
	GPIO_InitTypeDef GPIO_InitStructure;

 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); 
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;	   //GPIO_Pin_1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure); 
	GPIO_SetBits(GPIOA,GPIO_Pin_1);					 //PA1

    TFT_CtrlLinesConfig();
    TFT_FSMCConfig();

												  

	delay_ms(50); // delay 50 ms 
	TFT_WriteReg(0x0000,0x0001);
	delay_ms(50); // delay 50 ms 
	DeviceCode = TFT_ReadReg(0x0000);   
	printf(" TFT ID:%x\n",DeviceCode); //��ӡTFT ID  

	TFT_WriteReg(0x000,0x0000);
	TFT_WriteReg(0x000,0x0000);
	TFT_WriteReg(0x000,0x0000);
	TFT_WriteReg(0x000,0x0000);
	delay_ms(15);

	TFT_WriteReg(0x008,0x0808);
	TFT_WriteReg(0x010,0x0010);
	TFT_WriteReg(0x400,0x6200);

	TFT_WriteReg(0x300,0x0c06);//GAMMA
	TFT_WriteReg(0x301,0x9d0f);
	TFT_WriteReg(0x302,0x0b05);
	TFT_WriteReg(0x303,0x1217);
	TFT_WriteReg(0x304,0x3333);
	TFT_WriteReg(0x305,0x1712);
	TFT_WriteReg(0x306,0x950b);	
	TFT_WriteReg(0x307,0x0f0d);
	TFT_WriteReg(0x308,0x060c); 
	TFT_WriteReg(0x309,0x0000);

	TFT_WriteReg(0x011,0x0202);
	TFT_WriteReg(0x012,0x0101);
	TFT_WriteReg(0x013,0x0001);

	TFT_WriteReg(0x007,0x0001);
	TFT_WriteReg(0x100,0x0730);//BT,AP 0x0330
	TFT_WriteReg(0x101,0x0237);//DC0,DC1,VC
	TFT_WriteReg(0x103,0x2b00);//VDV	//0x0f00
	TFT_WriteReg(0x280,0x4000);//VCM
	TFT_WriteReg(0x102,0x81b0);//VRH,VCMR,PSON,PON
	delay_ms(15);

	TFT_WriteReg(0x001,0x0100);
	TFT_WriteReg(0x002,0x0100);
	TFT_WriteReg(0x003,0x1018);	 
	TFT_WriteReg(0x009,0x0001);
	
	TFT_WriteReg(0x0C,0x0000);	//MCU interface 
//  	TFT_WriteReg(0x0C,0x0110);	//RGB interface 18bit
//   TFT_WriteReg(0x0C,0x0111);	//RGB interface 16bit
//   TFT_WriteReg(0x0C,0x0020);	//VSYNC interface

	TFT_WriteReg(0x090,0x8000);
	TFT_WriteReg(0x00f,0x0000);

	TFT_WriteReg(0x210,0x0000);
	TFT_WriteReg(0x211,0x00ef);
	TFT_WriteReg(0x212,0x0000);
	TFT_WriteReg(0x213,0x018f);

	TFT_WriteReg(0x500,0x0000);
	TFT_WriteReg(0x501,0x0000);
	TFT_WriteReg(0x502,0x005f);
	TFT_WriteReg(0x401,0x0001);
	TFT_WriteReg(0x404,0x0000);
	delay_ms(15);	
	TFT_WriteReg(0x007,0x0100);
	delay_ms(15);
	TFT_WriteReg(0x200,0x0000);
	TFT_WriteReg(0x201,0x0000);
			 
	TFT_LED_SET;//��������	 
	TFT_Clear(WHITE);
}  		  
  
//��������
//Color:Ҫ���������ɫ
void TFT_Clear(u16 Color)
{
	u32 index=0;      
	TFT_SetCursor(0x00,0x0000);//���ù��λ�� 
	TFT_WriteRAM_Prepare();     //��ʼд��GRAM	 	  
	for(index=0;index<96000;index++)
	{
		TFT_WR_DATA(Color);    
	}
} 

//������׼�������������
void TFT_SetWindow(u16 xsta,u16 ysta,u16 xend,u16 yend)  //����
{
		/*���ô��ڴ�С*/
	TFT_WriteReg(0x0210, ysta);
	TFT_WriteReg(0x0212, xsta);
	TFT_WriteReg(0x0211, yend);
	TFT_WriteReg(0x0213, xend); 
	TFT_WriteRAM_Prepare();  //��ʼд��GRAM	 	
}

//�뿪���������ʹ�� ���ڻָ�ԭ���ĵ�ַ����
void Recover_Window(void)	   //�ָ�ԭ���ĵ�ַ���ã��������ã�
{
	TFT_WriteReg(0x0210, 0);   // Horizontal GRAM Start Address
	TFT_WriteReg(0x0211, 239); // Horizontal GRAM End Address
	TFT_WriteReg(0x0212, 0); // Vertical GRAM Start Address
	TFT_WriteReg(0x0213, 399); // Vertical GRAM Start Address

	TFT_WriteReg(0x0200, 0); //StartX);
	TFT_WriteReg(0x0201, 0); //StartY);	
}
 

//��ֱ��
void TFT_DrawLine(u16 x0,u16 y0,u16 len,u16 color){
//	TFT_Fill(x0,y0,x0+len-1,y0,color);
	u16 i;
	TFT_SetCursor(x0,y0);      //���ù��λ��   
	TFT_WriteRAM_Prepare();     //��ʼд��GRAM 	
	for(i=0;i<len;i++)
	{
		TFT_WR_DATA(color);//���ù��λ�� 
	}	  
}

//��ָ�����������ָ����ɫ
//�����С:
//  (xend-xsta)*(yend-ysta)
void TFT_Fill(u16 xsta,u16 ysta,u16 xend,u16 yend,u16 color)
{          
	u16 i,j;
	u16 xlen=0;
#if USE_HORIZONTAL==1
	xlen=xend-xsta+1;	 
	for(i=ysta;i<=yend;i++)
	{	 
		TFT_SetCursor(xsta,i);      //���ù��λ��   
		TFT_WriteRAM_Prepare();     //��ʼд��GRAM 
		for(j=0;j<xlen;j++)
		{
			TFT_WR_DATA(color);//���ù��λ�� 
		}	  
	}	
#else
	xlen=xend-xsta+1;	   
	for(i=ysta;i<=yend;i++)
	{
	 	TFT_SetCursor(xsta,i);      //���ù��λ�� 
		TFT_WriteRAM_Prepare();     //��ʼд��GRAM	  
		for(j=0;j<xlen;j++)TFT_WR_DATA(color);//���ù��λ�� 	    
	}
#endif						  	    
}  

//���Ϸ�������
void TFT_FillRectangle(u16 xsta,u16 ysta,u16 width,u16 height,u16 color){
	TFT_Fill(xsta,ysta,xsta+width-1,ysta+height-1,color);
}
//��ָ��λ����ʾһ���ַ�
//x:0~234
//y:0~308
//num:Ҫ��ʾ���ַ�:" "--->"~"
//size:�����С 12/16
//mode:���ӷ�ʽ(1)���Ƿǵ��ӷ�ʽ(0)
//��ָ��λ����ʾһ���ַ�
//x:0~234
//y:0~308
//num:Ҫ��ʾ���ַ�:" "--->"~"
//size:�����С 12/16
//mode:���ӷ�ʽ(1)���Ƿǵ��ӷ�ʽ(0)
void TFT_ShowChar(u16 x,u16 y,u16 color,u8 num,u8 size,u8 mode)
{  
#if USE_HORIZONTAL==1
#define MAX_CHAR_POSX 392
#define MAX_CHAR_POSY 232 
#else     
#define MAX_CHAR_POSX 232
#define MAX_CHAR_POSY 392
#endif 
    u8 temp;
    u8 pos,t;
	u16 x0=x;  
    if(x>MAX_CHAR_POSX||y>MAX_CHAR_POSY)return;	    
	//���ô���		   
	num=num-' ';//�õ�ƫ�ƺ��ֵ
	if(!mode) //�ǵ��ӷ�ʽ
	{
		for(pos=0;pos<size;pos++)
		{
			if(size==12)temp=asc2_1206[num][pos];//����1206����
			else temp=asc2_1608[num][pos];		 //����1608����
			for(t=0;t<size/2;t++)
		    {                 
		        if(temp&0x01)TFT_DrawPoint(x,y,color);
				else TFT_DrawPoint(x,y,BACK_COLOR);
					
				temp>>=1; 
				x++;
		    }
			x=x0;
			y++;
		}	
	}else//���ӷ�ʽ
	{
		for(pos=0;pos<size;pos++)
		{
			if(size==12)temp=asc2_1206[num][pos];//����1206����
			else temp=asc2_1608[num][pos];		 //����1608����
			for(t=0;t<size/2;t++)
		    {                 
		        if(temp&0x01)TFT_DrawPoint(x+t,y+pos,color);//��һ����     
		        temp>>=1; 
		    }
		}
	} 	   	 	  
}   
//m^n����
u32 mypow(u8 m,u8 n)
{
	u32 result=1;	 
	while(n--)result*=m;    
	return result;
}			 
//��ʾ2������
//x,y :�������	 
//len :���ֵ�λ��
//size:�����С
//color:��ɫ
//num:��ֵ(0~4294967295);	 
void TFT_ShowNum(u16 x,u16 y,u16 color,u32 num,u8 len,u8 size)
{         	
	u8 t,temp;
	u8 enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/mypow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				TFT_ShowChar(x+(size/2)*t,y,color,'0',size,0);
				continue;
			}else enshow=1; 
		 	 
		}
	 	TFT_ShowChar(x+(size/2)*t,y,color,temp+'0',size,0); 
	}
} 
//��ʾ2������
//x,y:�������
//size:�����С
//mode:ģʽ	0,���ģʽ;1,����ģʽ
//num:��ֵ(0~99);	 
void TFT_Show2Num(u16 x,u16 y,u16 color,u16 num,u8 len,u8 size,u8 mode)
{         	
	u8 t,temp;						   
	for(t=0;t<len;t++)
	{
		temp=(num/mypow(10,len-t-1))%10;
	 	TFT_ShowChar(x+(size/2)*t,y,color,temp+'0',size,mode); 
	}
} 
//��ʾ�ַ���
//x,y:�������  
//*p:�ַ�����ʼ��ַ
//��16����
void TFT_ShowString(u16 x,u16 y,u16 color,const u8 *p)
{         
    while(*p!='\0')
    {       
        if(x>MAX_CHAR_POSX){x=0;y+=16;}
        if(y>MAX_CHAR_POSY){y=x=0;TFT_Clear(WHITE);}
        TFT_ShowChar(x,y,color,*p,16,1);
        x+=8;
        p++;
    }  
}


/*
***************************************************************
*��������    TFT_ShowImage
*���ܣ�		 ��ʾָ����С��ͼƬ
*������		 xsta,ysta:������� xend,yend:�յ����� *p:ָ��ͼƬ
*����ֵ��	 ��
*ע�⣺
*�����÷���
***************************************************************
*/
void TFT_ShowImage(u16 xsta,u16 ysta,u16 xend,u16 yend,const u16 *p)
{
	u16 i,j;
	u16 xlen;
	xlen=xend-xsta;	 
	for(i=ysta;i<yend;i++)
	{	 
		TFT_SetCursor(xsta,i);      //���ù��λ��   
		TFT_WriteRAM_Prepare();     //��ʼд��GRAM 
		for(j=0;j<xlen;j++)
		{
			TFT_WR_DATA(*p++);//���ù��λ�� 
		}	  
	}		
}































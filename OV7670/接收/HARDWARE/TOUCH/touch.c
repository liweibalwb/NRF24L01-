/*
***************************************************************	
*ע�⣺������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
*������XPT2046 ������ʵ��
*�汾��V1.0
*���ߣ�OneTree
*�������ڣ�2012/11/7
*�޸����ڣ�2012/11/7 
*��Ȩ���У�����ؾ���
*Copyright(C) OneTree 2012-2022
*All rights reserved
***************************************************************
*Ӳ�����ӣ�INT-------------PF10		MISO--------------PF7
		   MOSI------------PF8		SCLK--------------PF9
		   CS--------------PF6 

		
***************************************************************
*�޸ļ�¼��

	
*/
#include 	"touch.h" 


  
Pen_Holder Pen_Point;//�����ʵ��
//Ĭ��Ϊtouchtype=0������.
u8 CMD_RDX=0X90;
u8 CMD_RDY=0XD0;

//SPIд����
//��7843д��1byte����   
void ADS_Write_Byte(u8 num)    
{  
	u8 count=0;   
	for(count=0;count<8;count++)  
	{ 	  
		if(num&0x80)TDIN_SET;  
		else TDIN_CLR;   
		num<<=1;    
		TCLK_CLR;//��������Ч	   	 
		TCLK_SET;      
	} 			    
} 		 
//SPI������ 
//��7846/7843/XPT2046/UH7843/UH7846��ȡadcֵ	   
u16 ADS_Read_AD(u8 CMD)	  
{ 	 
	u8 count=0; 	  
	u16 Num=0; 
	TCLK_CLR;//������ʱ�� 	 
	TCS_CLR; //ѡ��ADS7843	 
	ADS_Write_Byte(CMD);//����������
	delay_us(6);//ADS7846��ת��ʱ���Ϊ6us
	TCLK_SET;//��1��ʱ�ӣ����BUSY   	    
	TCLK_CLR; 	 
	for(count=0;count<16;count++)  
	{ 				  
		Num<<=1; 	 
		TCLK_SET;//�½�����Ч  	    	   
		TCLK_CLR;
		if(DOUT)Num++; 		 
	}  	
	Num>>=4;   //ֻ�и�12λ��Ч.
	TCS_SET;//�ͷ�ADS7843	 
	return(Num);   
}
//��ȡһ������ֵ
//������ȡREAD_TIMES������,����Щ������������,
//Ȼ��ȥ����ͺ����LOST_VAL����,ȡƽ��ֵ 
#define READ_TIMES 15 //��ȡ����
#define LOST_VAL 5	  //����ֵ
u16 ADS_Read_XY(u8 xy)
{
	u16 i, j;
	u16 buf[READ_TIMES];
	u16 sum=0;
	u16 temp;
	for(i=0;i<READ_TIMES;i++)
	{				 
		buf[i]=ADS_Read_AD(xy);	    
	}				    
	for(i=0;i<READ_TIMES-1; i++)//����
	{
		for(j=i+1;j<READ_TIMES;j++)
		{
			if(buf[i]>buf[j])//��������
			{
				temp=buf[i];
				buf[i]=buf[j];
				buf[j]=temp;
			}
		}
	}	  
	sum=0;
	for(i=LOST_VAL;i<READ_TIMES-LOST_VAL;i++)sum+=buf[i];
	temp=sum/(READ_TIMES-2*LOST_VAL);
	return temp;   
} 
//���˲��������ȡ
//��Сֵ��������100.
u8 Read_ADS(u16 *x,u16 *y)
{
	u16 xtemp,ytemp;			 	 		  
	xtemp=ADS_Read_XY(CMD_RDX);
	ytemp=ADS_Read_XY(CMD_RDY);	  												   
	if(xtemp<100||ytemp<100)return 0;//����ʧ��
	*x=xtemp;
	*y=ytemp;
	return 1;//�����ɹ�
}	
//2�ζ�ȡADS7846,������ȡ2����Ч��ADֵ,�������ε�ƫ��ܳ���
//50,��������,����Ϊ������ȷ,�����������.	   
//�ú����ܴ�����׼ȷ��
#define ERR_RANGE 50 //��Χ 
u8 Read_ADS2(u16 *x,u16 *y) 
{
	u16 x1,y1;
 	u16 x2,y2;
 	u8 flag;    
    flag=Read_ADS(&x1,&y1);   
    if(flag==0)return(0);
    flag=Read_ADS(&x2,&y2);	   
    if(flag==0)return(0);   
    if(((x2<=x1&&x1<x2+ERR_RANGE)||(x1<=x2&&x2<x1+ERR_RANGE))//ǰ�����β�����+-50��
    &&((y2<=y1&&y1<y2+ERR_RANGE)||(y1<=y2&&y2<y1+ERR_RANGE)))
    {
        *x=(x1+x2)/2;
        *y=(y1+y2)/2;
        return 1;
    }else return 0;	  
} 
//��ȡһ������ֵ	
//������ȡһ��,֪��PEN�ɿ��ŷ���!					   
u8 Read_TP_Once(void)
{
	u8 t=0;	    
	Pen_Int_Set(0);//�ر��ж�
	Pen_Point.Key_Sta=Key_Up;
	Read_ADS2(&Pen_Point.X,&Pen_Point.Y);
	while(PEN==0&&t<=250)
	{
		t++;
		delay_ms(10);
	};
	Pen_Int_Set(1);//�����ж�		 
	if(t>=250)return 0;//����2.5s ��Ϊ��Ч
	else return 1;	
}

/****************************************************************************
* ��    �ƣ�void GUI_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2,u16 color)
* ��    �ܣ���ָ������ֱ��
* ��ڲ�����x1,y1 ��� x2,y2 �е�  color ��ɫ 
* ���ڲ�����
* ˵    ����
* ���÷�����
****************************************************************************/ 
static void Touch_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2,u16 color)
{
	u16 t; 
	int xerr=0,yerr=0,delta_x,delta_y,distance; 
	int incx,incy,uRow,uCol; 

	delta_x=x2-x1; //������������ 
	delta_y=y2-y1; 
	uRow=x1; 
	uCol=y1; 
	if(delta_x>0)incx=1; //���õ������� 
	else if(delta_x==0)incx=0;//��ֱ�� 
	else {incx=-1;delta_x=-delta_x;} 
	if(delta_y>0)incy=1; 
	else if(delta_y==0)incy=0;//ˮƽ�� 
	else{incy=-1;delta_y=-delta_y;} 
	if( delta_x>delta_y)distance=delta_x; //ѡȡ�������������� 
	else distance=delta_y; 
	for(t=0;t<=distance+1;t++ )//������� 
	{  
		TFT_DrawPoint(uRow,uCol,color);//���� 
		xerr+=delta_x ; 
		yerr+=delta_y ; 
		if(xerr>distance) 
		{ 
			xerr-=distance; 
			uRow+=incx; 
		} 
		if(yerr>distance) 
		{ 
			yerr-=distance; 
			uCol+=incy; 
		} 
	}  
}  

/****************************************************************************
* ��    �ƣ�void GUI_DrawCircle(u16 cx,u16 cy,u16 r,u16 color,u8 fill)
* ��    �ܣ���ָ�����껭Բ�������
* ��ڲ�����cx,xy Բ�� r�뾶
* ���ڲ�����
* ˵    ����
* ���÷�����
****************************************************************************/
static void Touch_DrawCircle(u16 cx,u16 cy,u16 r,u16 color,u8 fill)
{
    s16 x,y,d;
    y = r;
    d = 3 - (r + r);
    x = 0;

	while(x<=y)
	{
	    if(fill)
		{
			Touch_DrawLine(cx+x,cy+y,cx-x,cy+y,color);
			Touch_DrawLine(cx+x,cy-y,cx-x,cy-y,color);
			Touch_DrawLine(cx+y,cy+x,cx-y,cy+x,color);
			Touch_DrawLine(cx+y,cy-x,cx-y,cy-x,color);
		}
		else
		{
	   		TFT_DrawPoint(cx+x,cy+y,color);
	  		TFT_DrawPoint(cx-x,cy+y,color);
	  		TFT_DrawPoint(cx+x,cy-y,color);
	   		TFT_DrawPoint(cx-x,cy-y,color);
	  		TFT_DrawPoint(cx+y,cy+x,color);
	  		TFT_DrawPoint(cx-y,cy+x,color);
	  		TFT_DrawPoint(cx+y,cy-x,color);
	  		TFT_DrawPoint(cx-y,cy-x,color);
		}
        if(d < 0)
            d += (x + x + x + x) + 6;
        else
        {
            d+=((x - y) + (x - y) + (x - y) + (x - y)) + 10;
            y = y - 1;
        }
        x = x + 1;
	}
}

//////////////////////////////////////////////////
//��TFT�����йصĺ���  
//��һ��������
//����У׼�õ�
void Drow_Touch_Point(u16 x,u16 y,u16 color)
{
	Touch_DrawLine(x-12, y, x+13, y , YELLOW);//����
	Touch_DrawLine(x, y-12 ,x, y+13, YELLOW);//����
	Touch_DrawCircle( x, y, 6, YELLOW,0);//������Ȧ
}	  
//��һ�����
//2*2�ĵ�			   
void Set_Pen_Width(u16 x,u16 y,u16 color,u8 width)
{	  
	Touch_DrawCircle(x,y,width,color,1);	 	  	
}

//�õ�������֮��ľ���ֵ
u16 T_abs(u16 m,u16 n)
{
	if(m>n)return m-n;
	else return n-m;
}
//���ܶ�ȡ����������		   
//����ֵ0��û���κΰ�������
//����ֵ1������������,���������ƶ���
//����ֵ2����������,û���ƶ�
//PEN�ź�������ⰴ���Ƿ��ɿ�
//2010/6/19 �޸��˼���СBUG V0.0.2
#define PT_RANGE 3//���ΰ��µ�ķ�Χֵ
//����:MS_TIME ���ֻ����붨���ʱ��,һ������Ϊ30
//������Ϊ50ʱ,�Ƚ��ʺϻ������.����Ϊ30ʱ,�Ƚ��ʺ϶�����.
//��ֵ��Ӱ�쵽��������ٶ�.50��ʵҲ���Խ�����.
//���Ϊ255.
u8 AI_Read_TP(u8 MS_TIME)
{
	static u8 LSTA=0;	   
	u8 times;		   
	switch(LSTA)
	{
		case 0:	  
		if(Pen_Point.Key_Sta==Key_Down)//�а���������
		{				 
			Pen_Int_Set(0);  //�ر��ж� 
			Convert_Pos();//ת��ΪTFT����	 
			Pen_Point.X0=Pen_Point.X;//��¼��һ�ΰ��µİ�������λ��
			Pen_Point.Y0=Pen_Point.Y;
			times=0;			 
			do
			{
				Convert_Pos();	   //�޸��������ϵ��ԭ��,ִ��һ����Ҫ��10ms���ҵ�ʱ��		 								 
				if((T_abs(Pen_Point.X,Pen_Point.X0)>PT_RANGE)||(T_abs(Pen_Point.Y,Pen_Point.Y0)>PT_RANGE))//�ƶ��������PT_RANGE
				{	
					Pen_Point.Key_Reg|=1;//��ǵ�һ�ΰ���Ϊ����ģʽ				 				 
					LSTA=1;
					return 1;//��������,�������ƶ�
				}else times++;	    
				if(times>MS_TIME)//�涨ʱ����û���ƶ�����PT_RANGE��Χ,����������
				{
					Pen_Point.Key_Reg|=2;//��ǵ�һ�ΰ���Ϊ�㴥ģʽ						 			 
					LSTA=2;
					return 2;
				}
			}while(PEN==0);//����һֱ�ڰ���
			//�������������ɿ���,��Ϊ�̰�  
			LSTA=2;
			return 2;
		}
		break;	  
		case 1://�����ڻ���	 
		if(PEN==0)
		{					    
			Convert_Pos(); //��ȡ���겢ת��	
			delay_us(100); //������!
			return LSTA;   //��Ȼ���ƶ���
		}
		Pen_Int_Set(1); //�����ж�
	    Pen_Point.Key_Sta=Key_Up; 
		break;		 
		case 2://����δ����	   
		if(PEN==0)
		{					    
			Convert_Pos(); //��ȡ���겢ת��ΪTFT����
			delay_us(100);		   
			if((T_abs(Pen_Point.X,Pen_Point.X0)>PT_RANGE)||(T_abs(Pen_Point.Y,Pen_Point.Y0)>PT_RANGE))//�ƶ��������PT_RANGE
			{
				LSTA=1;
				return 1;//��������,�������ƶ�
			}
			return 2;   //��Ȼû���ƶ�
		}
		Pen_Int_Set(1); //�����ж�
	    Pen_Point.Key_Sta=Key_Up;  
		break;
	}						    
	LSTA=0;	   
	return 0;  	 
}
//�жϴ����ǲ�����ָ������֮��
//(x,y):��ʼ����
//xlen,ylen:��x,y�����ϵ�ƫ�Ƴ���
//����ֵ :1,�ڸ�������.0,���ڸ�������.
u8 Is_In_Area(u16 x,u16 y,u16 xlen,u16 ylen)
{
	if(Pen_Point.X0<=(x+xlen)&&Pen_Point.X0>=x&&Pen_Point.Y0<=(y+ylen)&&Pen_Point.Y0>=y)return 1;
	else return 0;
} 
//////////////////////////////////////////////////

//ת�����
//���ݴ�������У׼����������ת����Ľ��,������X0,Y0��
void Convert_Pos(void)
{		 	  
	if(Read_ADS2(&Pen_Point.X,&Pen_Point.Y))
	{
		Pen_Point.X0=Pen_Point.xfac*Pen_Point.X+Pen_Point.xoff;
		Pen_Point.Y0=Pen_Point.yfac*Pen_Point.Y+Pen_Point.yoff;  
	}
}	   
//�ж�,��⵽PEN�ŵ�һ���½���.
//��λPen_Point.Key_StaΪ����״̬
//�ж���0���ϵ��жϼ��
// void EXTI15_10_IRQHandler(void)
// { 		   			 
// 	Pen_Point.Key_Sta=Key_Down;//��������  	 				 
// 	EXTI->PR=1<<10;  //���LINE10�ϵ��жϱ�־λ 
// } 
//PEN�ж�����	 
void Pen_Int_Set(u8 en)
{
	if(en)EXTI->IMR|=1<<10;   //����line10�ϵ��ж�	  	
	else EXTI->IMR&=~(1<<10); //�ر�line10�ϵ��ж�	   
}
	  
//////////////////////////////////////////////////////////////////////////
//�˲����漰��ʹ���ⲿEEPROM,���û���ⲿEEPROM,���δ˲��ּ���
#ifdef ADJ_SAVE_ENABLE
//������EEPROM����ĵ�ַ�����ַ,ռ��13���ֽ�(RANGE:SAVE_ADDR_BASE~SAVE_ADDR_BASE+12)
#define SAVE_ADDR_BASE 40
//����У׼����										    
void Save_Adjdata(void)
{
	s32 temp;			 
	//����У�����!		   							  
	temp=Pen_Point.xfac*100000000;//����xУ������      
    AT24CXX_WriteLenByte(SAVE_ADDR_BASE,temp,4);   
	temp=Pen_Point.yfac*100000000;//����yУ������    
    AT24CXX_WriteLenByte(SAVE_ADDR_BASE+4,temp,4);
	//����xƫ����
    AT24CXX_WriteLenByte(SAVE_ADDR_BASE+8,Pen_Point.xoff,2);		    
	//����yƫ����
	AT24CXX_WriteLenByte(SAVE_ADDR_BASE+10,Pen_Point.yoff,2);	
	temp=0X0A;//���У׼����
	AT24CXX_WriteOneByte(SAVE_ADDR_BASE+12,temp);			 
}
//�õ�������EEPROM�����У׼ֵ
//����ֵ��1���ɹ���ȡ����
//        0����ȡʧ�ܣ�Ҫ����У׼
u8 Get_Adjdata(void)
{					  
	s32 tempfac;
	tempfac=AT24CXX_ReadOneByte(SAVE_ADDR_BASE+12);//��ȡ�����,���Ƿ�У׼���� 		 
	if(tempfac==0X0A)//�������Ѿ�У׼����			   
	{    												 
		tempfac=AT24CXX_ReadLenByte(SAVE_ADDR_BASE,4);		   
		Pen_Point.xfac=(float)tempfac/100000000;//�õ�xУ׼����
		tempfac=AT24CXX_ReadLenByte(SAVE_ADDR_BASE+4,4);			          
		Pen_Point.yfac=(float)tempfac/100000000;//�õ�yУ׼����
	    //�õ�xƫ����
		Pen_Point.xoff=AT24CXX_ReadLenByte(SAVE_ADDR_BASE+8,2);			   	  
 	    //�õ�yƫ����
		Pen_Point.yoff=AT24CXX_ReadLenByte(SAVE_ADDR_BASE+10,2);				 	  	 
		return 1;	 
	}
	return 0;
}
#endif	
//
	 
//������У׼����
//�õ��ĸ�У׼����
void Touch_Adjust(void)
{								 
	signed short left_val,up_val,right_val,down_val;//���껺��ֵ
	u8  cnt=0;		   				
	TFT_Clear(BLACK);//���� 
	Pen_Point.Key_Sta=Key_Up;//���������ź� 
	Pen_Point.xfac=0;//xfac��������Ƿ�У׼��,����У׼֮ǰ�������!�������	
	Drow_Touch_Point(20,20,RED);//����1  
	while(1)
	{
		if(Pen_Point.Key_Sta==Key_Down)//����������
		{
			if(Read_TP_Once()&&(cnt==0))//�õ����ΰ���ֵ
			{  								   
				left_val=Pen_Point.X;
				up_val  =Pen_Point.Y;
				TFT_Clear(BLACK);//���� 
				Drow_Touch_Point(380,220,RED);//����2
				cnt++;
				continue;
			}
			else
			{
				right_val=Pen_Point.X;
				down_val  =Pen_Point.Y;
				TFT_Clear(BLACK);//���� 	
			}
			Pen_Point.xfac = (float)360.0/(float)(right_val - left_val);
			Pen_Point.xoff =  20 - Pen_Point.xfac*left_val;

			Pen_Point.yfac = (float)200.0/(float)(down_val - up_val);
			Pen_Point.yoff =  20 - Pen_Point.yfac*up_val;					 
			TFT_Clear(BLACK);//����
			TFT_ShowString(35,110,BLUE,"Touch Screen Adjust OK!");//У�����
			delay_ms(500);
			TFT_Clear(BLACK);//���� 
			Save_Adjdata();  
			return;//У�����				 
		}
	} 
}		    
//�ⲿ�жϳ�ʼ������
void Touch_Init(void)
{			    		   
	NVIC_InitTypeDef NVIC_InitStructure;  //�ж�
	GPIO_InitTypeDef GPIO_InitStructure;	//GPIO
	EXTI_InitTypeDef EXTI_InitStructure;	//�ⲿ�ж���		    		   
	//ע��,ʱ��ʹ��֮��,��GPIO�Ĳ�������Ч
	//��������֮ǰ,����ʹ��ʱ��.����ʵ���������������
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF  |RCC_APB2Periph_AFIO, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_8|GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //������� 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOF, &GPIO_InitStructure);	

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7|GPIO_Pin_10 ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU ;  //PF7,PF10��������
	GPIO_Init(GPIOF, &GPIO_InitStructure);

 	   
 	Read_ADS(&Pen_Point.X,&Pen_Point.Y);//��һ�ζ�ȡ��ʼ��	
			 
	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn; //ʹ�ܰ������ڵ��ⲿ�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2; //��ռ���ȼ�2��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; //�����ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //ʹ���ⲿ�ж�ͨ��
	NVIC_Init(&NVIC_InitStructure); //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ��� 
	  
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOF, GPIO_PinSource10); 
	  
	EXTI_InitStructure.EXTI_Line = EXTI_Line10;	//�ⲿ��·EXIT10
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;			//�����ⲿ�ж�ģʽ:EXTI��·Ϊ�ж�����
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;  //�ⲿ�жϴ�����ѡ��:����������·�½���Ϊ�ж�����
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;		//ʹ���ⲿ�ж���״̬

	EXTI_Init(&EXTI_InitStructure);		//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���	   
			 

#ifdef ADJ_SAVE_ENABLE	  
	AT24CXX_Init();//��ʼ��24CXX
	if(Get_Adjdata())return;//�Ѿ�У׼
	else			   //δУ׼?
	{ 										    
		TFT_Clear(BLACK);//����
	    Touch_Adjust();  //��ĻУ׼ 
		Save_Adjdata();	 
	}			
	Get_Adjdata();
#else
    Touch_Adjust();  //��ĻУ׼,���Զ�����			   
#endif												 
	printf("Pen_Point.xfac:%f\n",Pen_Point.xfac);
	printf("Pen_Point.yfac:%f\n",Pen_Point.yfac);
	printf("Pen_Point.xoff:%d\n",Pen_Point.xoff);
	printf("Pen_Point.yoff:%d\n",Pen_Point.yoff);
}


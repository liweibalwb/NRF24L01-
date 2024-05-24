/*
***************************************************************	
*注意：本程序只供学习使用，未经作者许可，不得用于其它任何用途
*描述：XPT2046 触摸屏实验
*版本：V1.0
*作者：OneTree
*创建日期：2012/11/7
*修改日期：2012/11/7 
*版权所有，盗版必究。
*Copyright(C) OneTree 2012-2022
*All rights reserved
***************************************************************
*硬件连接：INT-------------PF10		MISO--------------PF7
		   MOSI------------PF8		SCLK--------------PF9
		   CS--------------PF6 

		
***************************************************************
*修改记录：

	
*/
#include 	"touch.h" 


  
Pen_Holder Pen_Point;//定义笔实体
//默认为touchtype=0的数据.
u8 CMD_RDX=0X90;
u8 CMD_RDY=0XD0;

//SPI写数据
//向7843写入1byte数据   
void ADS_Write_Byte(u8 num)    
{  
	u8 count=0;   
	for(count=0;count<8;count++)  
	{ 	  
		if(num&0x80)TDIN_SET;  
		else TDIN_CLR;   
		num<<=1;    
		TCLK_CLR;//上升沿有效	   	 
		TCLK_SET;      
	} 			    
} 		 
//SPI读数据 
//从7846/7843/XPT2046/UH7843/UH7846读取adc值	   
u16 ADS_Read_AD(u8 CMD)	  
{ 	 
	u8 count=0; 	  
	u16 Num=0; 
	TCLK_CLR;//先拉低时钟 	 
	TCS_CLR; //选中ADS7843	 
	ADS_Write_Byte(CMD);//发送命令字
	delay_us(6);//ADS7846的转换时间最长为6us
	TCLK_SET;//给1个时钟，清除BUSY   	    
	TCLK_CLR; 	 
	for(count=0;count<16;count++)  
	{ 				  
		Num<<=1; 	 
		TCLK_SET;//下降沿有效  	    	   
		TCLK_CLR;
		if(DOUT)Num++; 		 
	}  	
	Num>>=4;   //只有高12位有效.
	TCS_SET;//释放ADS7843	 
	return(Num);   
}
//读取一个坐标值
//连续读取READ_TIMES次数据,对这些数据升序排列,
//然后去掉最低和最高LOST_VAL个数,取平均值 
#define READ_TIMES 15 //读取次数
#define LOST_VAL 5	  //丢弃值
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
	for(i=0;i<READ_TIMES-1; i++)//排序
	{
		for(j=i+1;j<READ_TIMES;j++)
		{
			if(buf[i]>buf[j])//升序排列
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
//带滤波的坐标读取
//最小值不能少于100.
u8 Read_ADS(u16 *x,u16 *y)
{
	u16 xtemp,ytemp;			 	 		  
	xtemp=ADS_Read_XY(CMD_RDX);
	ytemp=ADS_Read_XY(CMD_RDY);	  												   
	if(xtemp<100||ytemp<100)return 0;//读数失败
	*x=xtemp;
	*y=ytemp;
	return 1;//读数成功
}	
//2次读取ADS7846,连续读取2次有效的AD值,且这两次的偏差不能超过
//50,满足条件,则认为读数正确,否则读数错误.	   
//该函数能大大提高准确度
#define ERR_RANGE 50 //误差范围 
u8 Read_ADS2(u16 *x,u16 *y) 
{
	u16 x1,y1;
 	u16 x2,y2;
 	u8 flag;    
    flag=Read_ADS(&x1,&y1);   
    if(flag==0)return(0);
    flag=Read_ADS(&x2,&y2);	   
    if(flag==0)return(0);   
    if(((x2<=x1&&x1<x2+ERR_RANGE)||(x1<=x2&&x2<x1+ERR_RANGE))//前后两次采样在+-50内
    &&((y2<=y1&&y1<y2+ERR_RANGE)||(y1<=y2&&y2<y1+ERR_RANGE)))
    {
        *x=(x1+x2)/2;
        *y=(y1+y2)/2;
        return 1;
    }else return 0;	  
} 
//读取一次坐标值	
//仅仅读取一次,知道PEN松开才返回!					   
u8 Read_TP_Once(void)
{
	u8 t=0;	    
	Pen_Int_Set(0);//关闭中断
	Pen_Point.Key_Sta=Key_Up;
	Read_ADS2(&Pen_Point.X,&Pen_Point.Y);
	while(PEN==0&&t<=250)
	{
		t++;
		delay_ms(10);
	};
	Pen_Int_Set(1);//开启中断		 
	if(t>=250)return 0;//按下2.5s 认为无效
	else return 1;	
}

/****************************************************************************
* 名    称：void GUI_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2,u16 color)
* 功    能：在指定区域画直线
* 入口参数：x1,y1 起点 x2,y2 中点  color 颜色 
* 出口参数：
* 说    明：
* 调用方法：
****************************************************************************/ 
static void Touch_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2,u16 color)
{
	u16 t; 
	int xerr=0,yerr=0,delta_x,delta_y,distance; 
	int incx,incy,uRow,uCol; 

	delta_x=x2-x1; //计算坐标增量 
	delta_y=y2-y1; 
	uRow=x1; 
	uCol=y1; 
	if(delta_x>0)incx=1; //设置单步方向 
	else if(delta_x==0)incx=0;//垂直线 
	else {incx=-1;delta_x=-delta_x;} 
	if(delta_y>0)incy=1; 
	else if(delta_y==0)incy=0;//水平线 
	else{incy=-1;delta_y=-delta_y;} 
	if( delta_x>delta_y)distance=delta_x; //选取基本增量坐标轴 
	else distance=delta_y; 
	for(t=0;t<=distance+1;t++ )//画线输出 
	{  
		TFT_DrawPoint(uRow,uCol,color);//画点 
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
* 名    称：void GUI_DrawCircle(u16 cx,u16 cy,u16 r,u16 color,u8 fill)
* 功    能：在指定座标画圆，可填充
* 入口参数：cx,xy 圆心 r半径
* 出口参数：
* 说    明：
* 调用方法：
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
//与TFT部分有关的函数  
//画一个触摸点
//用来校准用的
void Drow_Touch_Point(u16 x,u16 y,u16 color)
{
	Touch_DrawLine(x-12, y, x+13, y , YELLOW);//横线
	Touch_DrawLine(x, y-12 ,x, y+13, YELLOW);//竖线
	Touch_DrawCircle( x, y, 6, YELLOW,0);//画中心圈
}	  
//画一个大点
//2*2的点			   
void Set_Pen_Width(u16 x,u16 y,u16 color,u8 width)
{	  
	Touch_DrawCircle(x,y,width,color,1);	 	  	
}

//得到两个数之差的绝对值
u16 T_abs(u16 m,u16 n)
{
	if(m>n)return m-n;
	else return n-m;
}
//智能读取触摸屏按键		   
//返回值0：没有任何按键按下
//返回值1：按键按下了,并且正在移动中
//返回值2：按键按下,没有移动
//PEN信号用来检测按键是否松开
//2010/6/19 修改了几个小BUG V0.0.2
#define PT_RANGE 3//单次按下点的范围值
//参数:MS_TIME 区分滑动与定点的时间,一般设置为30
//当设置为50时,比较适合滑动检测.设置为30时,比较适合定点检测.
//该值会影响到定点检测的速度.50其实也可以接受了.
//最大为255.
u8 AI_Read_TP(u8 MS_TIME)
{
	static u8 LSTA=0;	   
	u8 times;		   
	switch(LSTA)
	{
		case 0:	  
		if(Pen_Point.Key_Sta==Key_Down)//有按键按下了
		{				 
			Pen_Int_Set(0);  //关闭中断 
			Convert_Pos();//转换为TFT坐标	 
			Pen_Point.X0=Pen_Point.X;//记录第一次按下的按键坐标位置
			Pen_Point.Y0=Pen_Point.Y;
			times=0;			 
			do
			{
				Convert_Pos();	   //修改相对坐标系的原点,执行一次需要近10ms左右的时间		 								 
				if((T_abs(Pen_Point.X,Pen_Point.X0)>PT_RANGE)||(T_abs(Pen_Point.Y,Pen_Point.Y0)>PT_RANGE))//移动距离大于PT_RANGE
				{	
					Pen_Point.Key_Reg|=1;//标记第一次按下为滑动模式				 				 
					LSTA=1;
					return 1;//按键按下,并且在移动
				}else times++;	    
				if(times>MS_TIME)//规定时间内没有移动超过PT_RANGE范围,则主动返回
				{
					Pen_Point.Key_Reg|=2;//标记第一次按下为点触模式						 			 
					LSTA=2;
					return 2;
				}
			}while(PEN==0);//按键一直在按着
			//按键按下了又松开了,视为短按  
			LSTA=2;
			return 2;
		}
		break;	  
		case 1://触点在滑动	 
		if(PEN==0)
		{					    
			Convert_Pos(); //读取坐标并转换	
			delay_us(100); //不能少!
			return LSTA;   //仍然在移动中
		}
		Pen_Int_Set(1); //开启中断
	    Pen_Point.Key_Sta=Key_Up; 
		break;		 
		case 2://触点未滑动	   
		if(PEN==0)
		{					    
			Convert_Pos(); //读取坐标并转换为TFT坐标
			delay_us(100);		   
			if((T_abs(Pen_Point.X,Pen_Point.X0)>PT_RANGE)||(T_abs(Pen_Point.Y,Pen_Point.Y0)>PT_RANGE))//移动距离大于PT_RANGE
			{
				LSTA=1;
				return 1;//按键按下,并且在移动
			}
			return 2;   //仍然没有移动
		}
		Pen_Int_Set(1); //开启中断
	    Pen_Point.Key_Sta=Key_Up;  
		break;
	}						    
	LSTA=0;	   
	return 0;  	 
}
//判断触点是不是在指定区域之内
//(x,y):起始坐标
//xlen,ylen:在x,y方向上的偏移长度
//返回值 :1,在该区域内.0,不在该区域内.
u8 Is_In_Area(u16 x,u16 y,u16 xlen,u16 ylen)
{
	if(Pen_Point.X0<=(x+xlen)&&Pen_Point.X0>=x&&Pen_Point.Y0<=(y+ylen)&&Pen_Point.Y0>=y)return 1;
	else return 0;
} 
//////////////////////////////////////////////////

//转换结果
//根据触摸屏的校准参数来决定转换后的结果,保存在X0,Y0中
void Convert_Pos(void)
{		 	  
	if(Read_ADS2(&Pen_Point.X,&Pen_Point.Y))
	{
		Pen_Point.X0=Pen_Point.xfac*Pen_Point.X+Pen_Point.xoff;
		Pen_Point.Y0=Pen_Point.yfac*Pen_Point.Y+Pen_Point.yoff;  
	}
}	   
//中断,检测到PEN脚的一个下降沿.
//置位Pen_Point.Key_Sta为按下状态
//中断线0线上的中断检测
// void EXTI15_10_IRQHandler(void)
// { 		   			 
// 	Pen_Point.Key_Sta=Key_Down;//按键按下  	 				 
// 	EXTI->PR=1<<10;  //清除LINE10上的中断标志位 
// } 
//PEN中断设置	 
void Pen_Int_Set(u8 en)
{
	if(en)EXTI->IMR|=1<<10;   //开启line10上的中断	  	
	else EXTI->IMR&=~(1<<10); //关闭line10上的中断	   
}
	  
//////////////////////////////////////////////////////////////////////////
//此部分涉及到使用外部EEPROM,如果没有外部EEPROM,屏蔽此部分即可
#ifdef ADJ_SAVE_ENABLE
//保存在EEPROM里面的地址区间基址,占用13个字节(RANGE:SAVE_ADDR_BASE~SAVE_ADDR_BASE+12)
#define SAVE_ADDR_BASE 40
//保存校准参数										    
void Save_Adjdata(void)
{
	s32 temp;			 
	//保存校正结果!		   							  
	temp=Pen_Point.xfac*100000000;//保存x校正因素      
    AT24CXX_WriteLenByte(SAVE_ADDR_BASE,temp,4);   
	temp=Pen_Point.yfac*100000000;//保存y校正因素    
    AT24CXX_WriteLenByte(SAVE_ADDR_BASE+4,temp,4);
	//保存x偏移量
    AT24CXX_WriteLenByte(SAVE_ADDR_BASE+8,Pen_Point.xoff,2);		    
	//保存y偏移量
	AT24CXX_WriteLenByte(SAVE_ADDR_BASE+10,Pen_Point.yoff,2);	
	temp=0X0A;//标记校准过了
	AT24CXX_WriteOneByte(SAVE_ADDR_BASE+12,temp);			 
}
//得到保存在EEPROM里面的校准值
//返回值：1，成功获取数据
//        0，获取失败，要重新校准
u8 Get_Adjdata(void)
{					  
	s32 tempfac;
	tempfac=AT24CXX_ReadOneByte(SAVE_ADDR_BASE+12);//读取标记字,看是否校准过！ 		 
	if(tempfac==0X0A)//触摸屏已经校准过了			   
	{    												 
		tempfac=AT24CXX_ReadLenByte(SAVE_ADDR_BASE,4);		   
		Pen_Point.xfac=(float)tempfac/100000000;//得到x校准参数
		tempfac=AT24CXX_ReadLenByte(SAVE_ADDR_BASE+4,4);			          
		Pen_Point.yfac=(float)tempfac/100000000;//得到y校准参数
	    //得到x偏移量
		Pen_Point.xoff=AT24CXX_ReadLenByte(SAVE_ADDR_BASE+8,2);			   	  
 	    //得到y偏移量
		Pen_Point.yoff=AT24CXX_ReadLenByte(SAVE_ADDR_BASE+10,2);				 	  	 
		return 1;	 
	}
	return 0;
}
#endif	
//
	 
//触摸屏校准代码
//得到四个校准参数
void Touch_Adjust(void)
{								 
	signed short left_val,up_val,right_val,down_val;//坐标缓存值
	u8  cnt=0;		   				
	TFT_Clear(BLACK);//清屏 
	Pen_Point.Key_Sta=Key_Up;//消除触发信号 
	Pen_Point.xfac=0;//xfac用来标记是否校准过,所以校准之前必须清掉!以免错误	
	Drow_Touch_Point(20,20,RED);//画点1  
	while(1)
	{
		if(Pen_Point.Key_Sta==Key_Down)//按键按下了
		{
			if(Read_TP_Once()&&(cnt==0))//得到单次按键值
			{  								   
				left_val=Pen_Point.X;
				up_val  =Pen_Point.Y;
				TFT_Clear(BLACK);//清屏 
				Drow_Touch_Point(380,220,RED);//画点2
				cnt++;
				continue;
			}
			else
			{
				right_val=Pen_Point.X;
				down_val  =Pen_Point.Y;
				TFT_Clear(BLACK);//清屏 	
			}
			Pen_Point.xfac = (float)360.0/(float)(right_val - left_val);
			Pen_Point.xoff =  20 - Pen_Point.xfac*left_val;

			Pen_Point.yfac = (float)200.0/(float)(down_val - up_val);
			Pen_Point.yoff =  20 - Pen_Point.yfac*up_val;					 
			TFT_Clear(BLACK);//清屏
			TFT_ShowString(35,110,BLUE,"Touch Screen Adjust OK!");//校正完成
			delay_ms(500);
			TFT_Clear(BLACK);//清屏 
			Save_Adjdata();  
			return;//校正完成				 
		}
	} 
}		    
//外部中断初始化函数
void Touch_Init(void)
{			    		   
	NVIC_InitTypeDef NVIC_InitStructure;  //中断
	GPIO_InitTypeDef GPIO_InitStructure;	//GPIO
	EXTI_InitTypeDef EXTI_InitStructure;	//外部中断线		    		   
	//注意,时钟使能之后,对GPIO的操作才有效
	//所以上拉之前,必须使能时钟.才能实现真正的上拉输出
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF  |RCC_APB2Periph_AFIO, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_8|GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //推挽输出 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOF, &GPIO_InitStructure);	

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7|GPIO_Pin_10 ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU ;  //PF7,PF10上拉输入
	GPIO_Init(GPIOF, &GPIO_InitStructure);

 	   
 	Read_ADS(&Pen_Point.X,&Pen_Point.Y);//第一次读取初始化	
			 
	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn; //使能按键所在的外部中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2; //先占优先级2级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; //从优先级0级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //使能外部中断通道
	NVIC_Init(&NVIC_InitStructure); //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器 
	  
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOF, GPIO_PinSource10); 
	  
	EXTI_InitStructure.EXTI_Line = EXTI_Line10;	//外部线路EXIT10
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;			//设外外部中断模式:EXTI线路为中断请求
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;  //外部中断触发沿选择:设置输入线路下降沿为中断请求
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;		//使能外部中断新状态

	EXTI_Init(&EXTI_InitStructure);		//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器	   
			 

#ifdef ADJ_SAVE_ENABLE	  
	AT24CXX_Init();//初始化24CXX
	if(Get_Adjdata())return;//已经校准
	else			   //未校准?
	{ 										    
		TFT_Clear(BLACK);//清屏
	    Touch_Adjust();  //屏幕校准 
		Save_Adjdata();	 
	}			
	Get_Adjdata();
#else
    Touch_Adjust();  //屏幕校准,带自动保存			   
#endif												 
	printf("Pen_Point.xfac:%f\n",Pen_Point.xfac);
	printf("Pen_Point.yfac:%f\n",Pen_Point.yfac);
	printf("Pen_Point.xoff:%d\n",Pen_Point.xoff);
	printf("Pen_Point.yoff:%d\n",Pen_Point.yoff);
}


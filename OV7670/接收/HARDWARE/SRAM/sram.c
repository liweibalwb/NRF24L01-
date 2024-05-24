/*
***************************************************************
*硬件连接： 
	FSMC_A0------------PF0		   FSMC_D0------------PD14
	FSMC_A1------------PF1 	       FSMC_D1------------PD15
	FSMC_A-------------PF2		   FSMC_D2------------PD0
	FSMC_A3------------PF3 	       FSMC_D3------------PD1
	FSMC_A4------------PF4		   FSMC_D4------------PE7
	FSMC_A5------------PF5 	       FSMC_D5------------PE8
	FSMC_A6------------PF12		   FSMC_D6------------PE9
	FSMC_A7------------PF13 	   FSMC_D7------------PE10
	FSMC_A8------------PF14		   FSMC_D8------------PE11
	FSMC_A9----- ------PF15	       FSMC_D9------------PE12
	FSMC_A10-----------PG0		   FSMC_D10-----------PE13
	FSMC_A11-----------PG1 	       FSMC_D11-----------PE14
	FSMC_A12-----------PG2		   FSMC_D12-----------PE15
	FSMC_A13-----------PG3 	       FSMC_D13-----------PD8
	FSMC_A14-----------PG4	       FSMC_D14-----------PD9
	FSMC_A15-----------PG5 	       FSMC_D15-----------PD10
 	FSMC_A16-----------PD11
	FSMC_A17-----------PD12
 	FSMC_A18-----------PD13

	FSMC_NE3-----------PG10			   FSMC_NWE-----------PD5
	FSMC_NOE-----------PD4		   FSMC_NBL0----------PE0
	FSMC_NBL1----------PE1
***************************************************************
*修改记录：

	
*/
#include	"sram.h"

#define Bank1_SRAM3_ADDR    ((u32)0x68000000)													//定义FSMC的访问基址地址


/*
***************************************************************
*函数名：FSMC_SRAM_Init
*功能：	 初始化FSMC的SRAM
*参数：	 无
*返回值：无
****************************************************************
*/
void FSMC_SRAM_Init(void)
{
	FSMC_NORSRAMInitTypeDef  FSMC_NORSRAMInitStructure;
  	FSMC_NORSRAMTimingInitTypeDef  p;
  	GPIO_InitTypeDef GPIO_InitStructure; 
  
    /*使能FSMC时钟 */
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC, ENABLE); //使能FSMC时钟  
	/*FSMC总线使用的GPIO组时钟使能*/
  	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOG | RCC_APB2Periph_GPIOE |
                           RCC_APB2Periph_GPIOF, ENABLE);

  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_8 | GPIO_Pin_9 |
                                  GPIO_Pin_10 | GPIO_Pin_14 | GPIO_Pin_15;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_Init(GPIOD, &GPIO_InitStructure); 
  
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 |
                                  GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | 
                                  GPIO_Pin_15;
  	GPIO_Init(GPIOE, &GPIO_InitStructure);
  
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | 
                                  GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_12 | GPIO_Pin_13 | 
                                  GPIO_Pin_14 | GPIO_Pin_15;
  	GPIO_Init(GPIOF, &GPIO_InitStructure);
  
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | 
                                  GPIO_Pin_4 | GPIO_Pin_5;
  	GPIO_Init(GPIOG, &GPIO_InitStructure);
  
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13; 
  	GPIO_Init(GPIOD, &GPIO_InitStructure);
   
  	/*FSMC NOE和NWE初试化*/  
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 |GPIO_Pin_5;
  	GPIO_Init(GPIOD, &GPIO_InitStructure);
  
  	/*FSMC NE3初试化*/  
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; 
  	GPIO_Init(GPIOG, &GPIO_InitStructure);
  
  	/*FSMC NBL0和NBL1初试*/  
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1; 
  	GPIO_Init(GPIOE, &GPIO_InitStructure); 
  
/*--------------FSMC 总线 存储器参数配置------------------------------*/
  	p.FSMC_AddressSetupTime = 0;           														//地址建立时间    
  	p.FSMC_AddressHoldTime = 0;            														//地址保持时间  
  	p.FSMC_DataSetupTime = 2;              														//数据建立时间
  	p.FSMC_BusTurnAroundDuration = 0;      														//总线恢复时间
  	p.FSMC_CLKDivision = 0;                														// 时钟分频因子 
  	p.FSMC_DataLatency = 0;          	   														//数据产生时间
  	p.FSMC_AccessMode =  FSMC_AccessMode_A; 													//FSMC NOR控制器时序
  	
/*--------------FSMC 总线 参数配置------------------------------*/
  	FSMC_NORSRAMInitStructure.FSMC_Bank = FSMC_Bank1_NORSRAM3;                  				//使用了FSMC的BANK1的子板块3             
  	FSMC_NORSRAMInitStructure.FSMC_DataAddressMux = FSMC_DataAddressMux_Disable;				//禁止地址数据线复用
  	FSMC_NORSRAMInitStructure.FSMC_MemoryType = FSMC_MemoryType_SRAM;           				//存储器类型为SRAM
  	FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_16b;  				//存储器数据宽度为16位
  	FSMC_NORSRAMInitStructure.FSMC_BurstAccessMode = FSMC_BurstAccessMode_Disable; 				//关闭突发模式访问
    //等待信号优先级，只有在使能突发访问模式才有效
	FSMC_NORSRAMInitStructure.FSMC_WaitSignalPolarity = FSMC_WaitSignalPolarity_Low; 
    //关闭Wrapped burst access mode，只有在使能突发访问模式才有效
  	FSMC_NORSRAMInitStructure.FSMC_WrapMode = FSMC_WrapMode_Disable;         
    //等待信号设置，只有在使能突发访问模式才有效
  	FSMC_NORSRAMInitStructure.FSMC_WaitSignalActive = FSMC_WaitSignalActive_BeforeWaitState;
  	FSMC_NORSRAMInitStructure.FSMC_WriteOperation = FSMC_WriteOperation_Enable;     			//使能这个BANK的写操作
    //使能/关闭等待信息设置，只在使能突发访问模式才有效
  	FSMC_NORSRAMInitStructure.FSMC_WaitSignal = FSMC_WaitSignal_Disable;     
  	FSMC_NORSRAMInitStructure.FSMC_ExtendedMode = FSMC_ExtendedMode_Disable; 					//关闭Extend Mode
  	FSMC_NORSRAMInitStructure.FSMC_WriteBurst = FSMC_WriteBurst_Disable;     					//关闭Write Burst Mode   
  	FSMC_NORSRAMInitStructure.FSMC_ReadWriteTimingStruct = &p;               					//读操作时序参数
  	FSMC_NORSRAMInitStructure.FSMC_WriteTimingStruct = &p;                   					//写操作时序参数

  	FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure); 

/*--------------使能BANK1的子板块3------------------------------*/
  	FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM3, ENABLE);  
}

/*
***************************************************************
*函数名：FSMC_SRAM_WriteBuffer
*功能：	 向SRAM写数据
*参数：	 pBuffer：指向将缓存
		 WriteAddr：SRAM内部地址
		 NumHalfwordToWrite：要写入的16位字节的个数
*返回值：无
****************************************************************
*/
void FSMC_SRAM_WriteBuffer(u16* pBuffer, u32 WriteAddr, u32 NumHalfwordToWrite)
{
  for(; NumHalfwordToWrite != 0; NumHalfwordToWrite--) /* while there is data to write */
  {
    /* Transfer data to the memory */
    *(u16 *) (Bank1_SRAM3_ADDR + WriteAddr) = *pBuffer++;
    
    /* Increment the address*/  
    WriteAddr += 2;
  }   
}


/*
***************************************************************
*函数名：FSMC_SRAM_ReadBuffer
*功能：	 向SRAM读数据
*参数：	 pBuffer：指向将缓存
		 WriteAddr：SRAM内部地址
		 NumHalfwordToWrite：要读出的16位字节的个数
*返回值：无
****************************************************************
*/
void FSMC_SRAM_ReadBuffer(u16* pBuffer, u32 ReadAddr, u32 NumHalfwordToRead)
{
  for(; NumHalfwordToRead != 0; NumHalfwordToRead--) /* while there is data to read */
  {
    /* Read a half-word from the memory */
    *pBuffer++ = *(vu16*) (Bank1_SRAM3_ADDR + ReadAddr);

    /* Increment the address*/  
    ReadAddr += 2;
  }  
}




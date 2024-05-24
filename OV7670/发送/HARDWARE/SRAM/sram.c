/*
***************************************************************
*Ӳ�����ӣ� 
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
*�޸ļ�¼��

	
*/
#include	"sram.h"

#define Bank1_SRAM3_ADDR    ((u32)0x68000000)													//����FSMC�ķ��ʻ�ַ��ַ


/*
***************************************************************
*��������FSMC_SRAM_Init
*���ܣ�	 ��ʼ��FSMC��SRAM
*������	 ��
*����ֵ����
****************************************************************
*/
void FSMC_SRAM_Init(void)
{
	FSMC_NORSRAMInitTypeDef  FSMC_NORSRAMInitStructure;
  	FSMC_NORSRAMTimingInitTypeDef  p;
  	GPIO_InitTypeDef GPIO_InitStructure; 
  
    /*ʹ��FSMCʱ�� */
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC, ENABLE); //ʹ��FSMCʱ��  
	/*FSMC����ʹ�õ�GPIO��ʱ��ʹ��*/
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
   
  	/*FSMC NOE��NWE���Ի�*/  
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 |GPIO_Pin_5;
  	GPIO_Init(GPIOD, &GPIO_InitStructure);
  
  	/*FSMC NE3���Ի�*/  
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; 
  	GPIO_Init(GPIOG, &GPIO_InitStructure);
  
  	/*FSMC NBL0��NBL1����*/  
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1; 
  	GPIO_Init(GPIOE, &GPIO_InitStructure); 
  
/*--------------FSMC ���� �洢����������------------------------------*/
  	p.FSMC_AddressSetupTime = 0;           														//��ַ����ʱ��    
  	p.FSMC_AddressHoldTime = 0;            														//��ַ����ʱ��  
  	p.FSMC_DataSetupTime = 2;              														//���ݽ���ʱ��
  	p.FSMC_BusTurnAroundDuration = 0;      														//���߻ָ�ʱ��
  	p.FSMC_CLKDivision = 0;                														// ʱ�ӷ�Ƶ���� 
  	p.FSMC_DataLatency = 0;          	   														//���ݲ���ʱ��
  	p.FSMC_AccessMode =  FSMC_AccessMode_A; 													//FSMC NOR������ʱ��
  	
/*--------------FSMC ���� ��������------------------------------*/
  	FSMC_NORSRAMInitStructure.FSMC_Bank = FSMC_Bank1_NORSRAM3;                  				//ʹ����FSMC��BANK1���Ӱ��3             
  	FSMC_NORSRAMInitStructure.FSMC_DataAddressMux = FSMC_DataAddressMux_Disable;				//��ֹ��ַ�����߸���
  	FSMC_NORSRAMInitStructure.FSMC_MemoryType = FSMC_MemoryType_SRAM;           				//�洢������ΪSRAM
  	FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_16b;  				//�洢�����ݿ��Ϊ16λ
  	FSMC_NORSRAMInitStructure.FSMC_BurstAccessMode = FSMC_BurstAccessMode_Disable; 				//�ر�ͻ��ģʽ����
    //�ȴ��ź����ȼ���ֻ����ʹ��ͻ������ģʽ����Ч
	FSMC_NORSRAMInitStructure.FSMC_WaitSignalPolarity = FSMC_WaitSignalPolarity_Low; 
    //�ر�Wrapped burst access mode��ֻ����ʹ��ͻ������ģʽ����Ч
  	FSMC_NORSRAMInitStructure.FSMC_WrapMode = FSMC_WrapMode_Disable;         
    //�ȴ��ź����ã�ֻ����ʹ��ͻ������ģʽ����Ч
  	FSMC_NORSRAMInitStructure.FSMC_WaitSignalActive = FSMC_WaitSignalActive_BeforeWaitState;
  	FSMC_NORSRAMInitStructure.FSMC_WriteOperation = FSMC_WriteOperation_Enable;     			//ʹ�����BANK��д����
    //ʹ��/�رյȴ���Ϣ���ã�ֻ��ʹ��ͻ������ģʽ����Ч
  	FSMC_NORSRAMInitStructure.FSMC_WaitSignal = FSMC_WaitSignal_Disable;     
  	FSMC_NORSRAMInitStructure.FSMC_ExtendedMode = FSMC_ExtendedMode_Disable; 					//�ر�Extend Mode
  	FSMC_NORSRAMInitStructure.FSMC_WriteBurst = FSMC_WriteBurst_Disable;     					//�ر�Write Burst Mode   
  	FSMC_NORSRAMInitStructure.FSMC_ReadWriteTimingStruct = &p;               					//������ʱ�����
  	FSMC_NORSRAMInitStructure.FSMC_WriteTimingStruct = &p;                   					//д����ʱ�����

  	FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure); 

/*--------------ʹ��BANK1���Ӱ��3------------------------------*/
  	FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM3, ENABLE);  
}

/*
***************************************************************
*��������FSMC_SRAM_WriteBuffer
*���ܣ�	 ��SRAMд����
*������	 pBuffer��ָ�򽫻���
		 WriteAddr��SRAM�ڲ���ַ
		 NumHalfwordToWrite��Ҫд���16λ�ֽڵĸ���
*����ֵ����
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
*��������FSMC_SRAM_ReadBuffer
*���ܣ�	 ��SRAM������
*������	 pBuffer��ָ�򽫻���
		 WriteAddr��SRAM�ڲ���ַ
		 NumHalfwordToWrite��Ҫ������16λ�ֽڵĸ���
*����ֵ����
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




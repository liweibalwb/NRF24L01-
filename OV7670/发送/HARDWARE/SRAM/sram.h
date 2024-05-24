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
#ifndef		__SRAM_H__
#define		__SRAM_H__


#include	"main.h"

void FSMC_SRAM_Init(void);
void FSMC_SRAM_WriteBuffer(u16* pBuffer, u32 WriteAddr, u32 NumHalfwordToWrite);
void FSMC_SRAM_ReadBuffer(u16* pBuffer, u32 ReadAddr, u32 NumHalfwordToRead);



#endif


#ifndef __CH375_H__
#define __CH375_H__

#include	"main.h"

typedef enum
{
  FALSE = 0, TRUE  = !FALSE
}
bool;


#define  CH375_INT_WIRE     (GPIOA->IDR & GPIO_Pin_0)// CH375_INT PA0CH375���ж���INT#����,����CH375��INT#����,���ڲ�ѯ�ж�״̬

#define LIB_CFG_FILE_IO			1		/* �ļ���д�����ݵĸ��Ʒ�ʽ,0Ϊ"�ⲿ�ӳ���",1Ϊ"�ڲ�����" */
#define LIB_CFG_INT_EN			0		/* CH375��INT#�������ӷ�ʽ,0Ϊ"��ѯ��ʽ",1Ϊ"�жϷ�ʽ" */

/* ��Ƭ����RAM����,����CH375�ӳ�����512�ֽ�,ʣ��RAM���ֿ��������ļ���д���� */
#define DISK_BASE_BUF_LEN		1024	/* Ĭ�ϵĴ������ݻ�������СΪ512�ֽ�,����ѡ��Ϊ2048����4096��֧��ĳЩ��������U��,Ϊ0���ֹ��.H�ļ��ж��建��������Ӧ�ó�����pDISK_BASE_BUF��ָ�� */
#define FILE_DATA_BUF_LEN		1024	/* �ⲿRAM���ļ����ݻ�����,���������Ȳ�С��һ�ζ�д�����ݳ��� */
/* ���׼��ʹ��˫�����������д,��ô��Ҫ����FILE_DATA_BUF_LEN,�����ڲ�����ָ����������ַ,��CH375FileReadX����CH375FileRead,��CH375FileWriteX����CH375FileWrite */

#define NO_DEFAULT_CH375_F_ENUM		1		/* δ����CH375FileEnumer����ʽ�ֹ�Խ�Լ���� */
#define NO_DEFAULT_CH375_F_QUERY	1		/* δ����CH375FileQuery����ʽ�ֹ�Խ�Լ���� */


extern bool CH375_Init(void);
extern u8 OpenFile(char* fil_name);
extern u8 CreateFile(char* fil_name);
extern u8 WriteFile(u8* buf,u32 len);
extern u8 CloseFile(void);
extern u8 MkDir( char* dir_name );
extern u8 ReadFile(u8* buf,u32 len,u32* br);
extern u8 WriteFile(u8* buf,u32 len);
extern u8 LocatFile(u32 offest);


#endif

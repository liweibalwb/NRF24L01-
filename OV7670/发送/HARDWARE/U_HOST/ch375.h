#ifndef __CH375_H__
#define __CH375_H__

#include	"main.h"

typedef enum
{
  FALSE = 0, TRUE  = !FALSE
}
bool;


#define  CH375_INT_WIRE     (GPIOA->IDR & GPIO_Pin_0)// CH375_INT PA0CH375的中断线INT#引脚,连接CH375的INT#引脚,用于查询中断状态

#define LIB_CFG_FILE_IO			1		/* 文件读写的数据的复制方式,0为"外部子程序",1为"内部复制" */
#define LIB_CFG_INT_EN			0		/* CH375的INT#引脚连接方式,0为"查询方式",1为"中断方式" */

/* 单片机的RAM有限,其中CH375子程序用512字节,剩余RAM部分可以用于文件读写缓冲 */
#define DISK_BASE_BUF_LEN		1024	/* 默认的磁盘数据缓冲区大小为512字节,建议选择为2048甚至4096以支持某些大扇区的U盘,为0则禁止在.H文件中定义缓冲区并由应用程序在pDISK_BASE_BUF中指定 */
#define FILE_DATA_BUF_LEN		1024	/* 外部RAM的文件数据缓冲区,缓冲区长度不小于一次读写的数据长度 */
/* 如果准备使用双缓冲区交替读写,那么不要定义FILE_DATA_BUF_LEN,而是在参数中指定缓冲区起址,用CH375FileReadX代替CH375FileRead,用CH375FileWriteX代替CH375FileWrite */

#define NO_DEFAULT_CH375_F_ENUM		1		/* 未调用CH375FileEnumer程序故禁止以节约代码 */
#define NO_DEFAULT_CH375_F_QUERY	1		/* 未调用CH375FileQuery程序故禁止以节约代码 */


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

#ifndef __EXPLORER_H__
#define __EXPLORER_H__


#include	"main.h"

////定义文件类型
#define T_MP1  1<<0
#define T_MP2  1<<1
#define T_MP3  1<<2
#define T_MP4  1<<3
#define T_M4A  1<<4
#define T_3GP  1<<5
#define T_3G2  1<<6
#define T_OGG  1<<7
#define T_ACC  1<<8
#define T_WMA  1<<9
#define T_WAV  1<<10
#define T_MID  1<<11	
#define T_FLAC 1<<12

#define T_LRC  1<<13
#define T_TXT  1<<14
#define T_C    1<<15
#define T_H    1<<16
						 
#define T_FON  1<<17
#define T_SYS  1<<18

#define T_BMP  1<<19
#define T_JPG  1<<20
#define T_JPEG 1<<21

#define T_UNTELL 0

u32 FileType_Tell(char * filename);
u8 Get_File_Info(char *path,u32 filetype,u16 *count);

#endif




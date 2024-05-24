#ifndef __MP3PLAYER_H
#define __MP3PLAYER_H


#include	"main.h"


//Mini STM32开发板
//音乐播放器 V1.0
//正点原子@ALIENTEK
//2010/12/13	
						  
						  	    													  
u8 Play_Music(char *path);
u8 Play_Song(char *path,u16 index,u16 total);//播放一首歌曲
void MP3_Msg_Show(u32 lenth,u16 index,u16 total);

#endif













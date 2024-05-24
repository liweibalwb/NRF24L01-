/*
***************************************************************	
*注意：本程序只供学习使用，未经作者许可，不得用于其它任何用途
*描述：工具文件
*版本：V1.0
*作者：OneTree
*创建日期：2013/1/19
*修改日期：2013/1/19
*版权所有，盗版必究。
*Copyright(C) OneTree 2012-2022
*All rights reserved
***************************************************************
*修改记录：

	
*/
#include "util.h"


/*
***************************************************************
*函数名：   ASSERT
*功能：		判断y表达式的真假
*参数：		无
*返回值：	无
*注意：		
*典型用法：
****************************************************************
*/

void ASSERT(const u8* str, int y){
	if(y<0){
	   printf((const char*)str);
	   printf(":表达式为假！\n");
	}else{
	   printf((const char*)str);
	   printf(":表达式为真！\n");		
	}
}



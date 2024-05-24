/**************************************************************
模块名:ch375.c 
功能实现:CH375硬件底层的移植部分,另外还包括软件层的改进,由于CH375的库函数书写比较不够规范, 
 		故此模块负责把库函数重新封装成文件读写的通用格式.经封装后读写速度会有所下降.如果需要
 		较高速的操作,请自己去看原库函数
 		***一旦调用,则不应再使用原库的函数,否则会有数据错误
作者:电子白菜



版本号:V1.0 
版本说明:	1.CH375硬件驱动函数.基层IO指令需要有延迟,这里用了一个硬delay函数,可自己根据CPU的速度,降低函数速度,使系统更快
			2.读写部分使用通用格式,并且所有操作都以扇区为单位,故不适合51之流使用,中间使用了缓冲隔离,优点是增加通用性,缺点是降低了速度. 
		 	3.增加了创建目录函数  
日期:2009-4-10
***************************************************************/
#include	"ch375.h"
#include    "CH375HFM.h"
#include    <string.h>
#include     <stdio.h>


//虚拟总线宏定义-为了使操作速度加快,这里没有使用库
#define CS375_H		GPIOB->BSRR=GPIO_Pin_2						  //CH375_CS   PB2
#define CS375_L		GPIOB->BRR=GPIO_Pin_2

#define WR375_H		GPIOA->BSRR=GPIO_Pin_2						 //CH375_WR   PA2  
#define WR375_L		GPIOA->BRR=GPIO_Pin_2  

#define RD375_H		GPIOA->BSRR=GPIO_Pin_3						 //CH375_RD   PA3
#define RD375_L		GPIOA->BRR=GPIO_Pin_3  

#define ADD375_H	GPIOF->BSRR=GPIO_Pin_11						 //CH375_A   PF11 
#define ADD375_L	GPIOF->BRR=GPIO_Pin_11  	

//#define RST375_H	GPIOC->BSRR=GPIO_Pin_1						
//#define RST375_L    GPIOC->BRR=GPIO_Pin_1   

//#define GET_INT		(GPIOB->IDR & GPIO_Pin_0)	/*获取INT脚状态*/

#define SET_BUS_OUT		GPIOC->CRL=0x33333333	/* PC  推挽输出50MHZ速度?*/
#define SET_BUS_IN		GPIOC->CRL=0x44444444	/*浮空输入*/

/**************************************************************
以下两个宏定义用于改变IO口的输入输出状态,为了提高速度,这里采用直接改变寄存器的办法,具体应用根据 
使用的IO脚的不同而会有所差别,这里使用PC0-7作为DATA脚 ,故操作的寄存器为GPIOC->CRL
***************************************************************/
#define GET_DATA375()		(GPIOC->IDR&0xff)
#define SET_DATA375(dat)	GPIOC->BSRR=(dat|(((~dat)<<16)))	//这个可用ODR寄存器


//硬延迟函数,可根据CPU速度逐步调整到最高速度的临界点
void delay(u16 t)
{
	u8 i;
	while(t--)
	{
		for(i=0;i<20;i++);
	}
}


/**************************************************************
** 函数名:Ch375_Configuration
** 功能:硬件配置CH375
** 注意事项:
***************************************************************/
void CH375_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	//A,B,C,F
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOF|RCC_APB2Periph_GPIOC, ENABLE);

	// PA2-WE,PA3-OE
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//50M时钟速度
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	//PF11-A0
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//50M时钟速度
	GPIO_Init(GPIOF, &GPIO_InitStructure);


	//PA0-INT,
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	//上拉输入
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	//PB2-CS
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//50M时钟速度
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	CS375_H;										  //CS输出高电平
	RD375_H;										  //RD输出高电平
	WR375_H;										  //WR输出高电平
}


/***************************************
** 函数名:xWriteCH375Cmd 
** 功能 :写命令
** 注意事项: ADD=1为命令,锁存时间至少要有4US
***************************************/
void xWriteCH375Cmd(u8 cmd)
{
	//延迟2US
	delay(1);
	SET_BUS_OUT;
	ADD375_H;
	WR375_L;
	SET_DATA375(cmd);
	CS375_L;
	//等待至少2US
	delay(1);
	WR375_H;
	CS375_H;
}
/**************************************************************
** 函数名:xWriteCH375Data
** 功能:写数据
** 注意事项:ADD=0时为写数据
***************************************************************/
void xWriteCH375Data(u8 dat)
{
	//延迟2US
	delay(1);
	SET_BUS_OUT;
	ADD375_L;
	WR375_L;
	SET_DATA375(dat);
	CS375_L;
	//等待至少2US
	delay(1);
	WR375_H;
	CS375_H;
}

/**************************************************************
** 函数名:xReadCH375Data
** 功能:读数据
** 注意事项:ADD=0时为数据
***************************************************************/
u8 xReadCH375Data(void)
{
	u8 tmp;
	//延迟2US
	delay(1);
	SET_BUS_IN;
	ADD375_L;
	RD375_L;
	CS375_L;
	//等待至少2US
	delay(1);
	tmp=GET_DATA375();
	RD375_H;
	CS375_H;
	return tmp;
}



/**************************************************************
    以下函数体均调用375库函数,之所以要从新编写这些OPENFILE之类的函数,
 	是因为375的库写的不是很规范---一些读写参数要通过直接操作结构体后才能调用功能函数,
 	所以以下函数把375的库从新封装成标准的FAT32函数,这样做会让代码稍微慢了点,但对于STM32 72M的运行速度来说,可以接受
	v1.0版-只为ISP100系列而做的简易版本
***************************************************************/

//以下几个变量是为ISP100系统而存在
//static u32 Read_File_Left_Size;		//文件剩下多大没读取
//static u32 Read_Sector_Cnt;			//每次需要读取的扇区数
//static u32 Read_Each_Size;			//每次可以读取的大小
static u32 File_Size;				//文件大小,此值实时更新在打开文件后确定,在WRITE后改变
//static u32 File_Offset;				//文件指针偏移,此值实时更新


/**************************************************************
** 函数名:OpenFile,CloseFile
** 功能:打开文件
** 注意事项:输入文件名必须为大写,这是375库的规定~~, 另外如果要读文件,则文件大小CH375vFileSize必须加大(一个扇区-1)字节 
** 				这些均通过函数内部解决了. 
** 返回:错误代码 
** 版本:V1.0 建议加入文件小写判断,自动改小写为大写,这样本函数就不受文件名的大小写限制 
***************************************************************/
u8 OpenFile(char* fil_name)
{
	u8 rtn;
	strcpy( (char *)mCmdParam.Open.mPathName, fil_name);
	rtn=CH375FileOpen();

	File_Size=CH375vFileSize;
	//	File_Offset=0;
	CH375vFileSize += CH375vSectorSize-1;	//CH375系统特别部分,文件大小要加大一点,
	return(rtn);
}

//关闭文件*库不支持两个文件同时操作,故无需输入文件名
u8 CloseFile(void)
{
	return(CH375FileClose());
}

/**************************************************************
** 函数名:CreateFile
** 功能:创建文件
** 注意事项:创建文件后,同时也打开了文件,直接关闭,则文件的大小会是1个字节.
** 版本:
***************************************************************/
u8 CreateFile(char* fil_name)
{
	u8 rtn;
	strcpy( (char *)mCmdParam.Open.mPathName, fil_name);
	rtn=CH375FileCreate();
	File_Size=1;
//	File_Offset=0;
	return rtn;
}

/**************************************************************
** 函数名:MkDir
** 功能:创建目录
** 注意事项:CH375的函数库本身没有创建目录的函数,这个是官方后期补漏的函数,内部涉及了一些FAT的底层操作
***************************************************************/
const char* DirConstData=".          \x10\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x21\x30\x0\x0\x0\x0\x0\x0..         \x10\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x21\x30\x0\x0\x0\x0\x0\x0";
u8	MkDir( char* dir_name )
{
	u8	i, j;
	u16	count;
	u32	UpDirCluster;
	u8*	DirXramBuf;
	
	strcpy((char*)mCmdParam.Create.mPathName,(const char*)dir_name);

	j = 0xFF;
	for (i = 0; i != sizeof( mCmdParam.Create.mPathName ); i ++) 
	{  /* 检查目录路径 */
		if (mCmdParam.Create.mPathName[ i ] == 0) 
			break;
		if (mCmdParam.Create.mPathName[ i ] == PATH_SEPAR_CHAR1 || mCmdParam.Create.mPathName[ i ] == PATH_SEPAR_CHAR2) 
			j = i;  /* 记录上级目录 */
	}
	i = ERR_SUCCESS;
	if( j == 0 || j == 2 && mCmdParam.Create.mPathName[1] == ':' ) 
		UpDirCluster = 0;  /* 在根目录下创建子目录 */
	else 
	{
		if (j != 0xFF) 
		{  /* 对于绝对路径应该获取上级目录的起始簇号 */
			mCmdParam.Create.mPathName[ j ] = 0;
			i = CH375FileOpen( );  /* 打开上级目录 */
			if (i == ERR_SUCCESS) i = ERR_MISS_DIR;  /* 是文件而非目录 */
			else if (i == ERR_OPEN_DIR) i = ERR_SUCCESS;  /* 成功打开上级目录 */
			mCmdParam.Create.mPathName[ j ] = PATH_SEPAR_CHAR1;  /* 恢复目录分隔符 */
		}
		UpDirCluster = CH375vStartCluster;  /* 保存上级目录的起始簇号 */
	}
	if (i == ERR_SUCCESS) 
	{  /* 成功获取上级目录的起始簇号 */
		i = CH375FileOpen();  /* 打开本级子目录 */
		if (i == ERR_SUCCESS) 
			i = ERR_FOUND_NAME;  /* 是文件而非目录 */
		else if (i == ERR_OPEN_DIR) 
			i = ERR_SUCCESS;  /* 目录已经存在 */
		else if (i == ERR_MISS_FILE) 
		{  /* 目录不存在,可以新建 */
			i = CH375FileCreate();  /* 以创建文件的方法创建目录 */
			if (i == ERR_SUCCESS) 
			{
//				if ( &FILE_DATA_BUF[0] == &DISK_BASE_BUF[0] ) CH375DirtyBuffer( );  /* 如果FILE_DATA_BUF与DISK_BASE_BUF合用则必须清除磁盘缓冲区 */
				DirXramBuf = &FILE_DATA_BUF[0];  /* 文件数据缓冲区 */
				for ( i = 0x40; i != 0; i -- ) 
				{  /* 目录的保留单元,分别指向自身和上级目录 */
					*DirXramBuf = *DirConstData;
					DirXramBuf ++;
					DirConstData ++;
				}

				FILE_DATA_BUF[0x1A] = (u8)CH375vStartCluster;  /* 自身的起始簇号 */
				FILE_DATA_BUF[0x1B] = (u8)(CH375vStartCluster>>8);
				FILE_DATA_BUF[0x14] = (u8)(CH375vStartCluster>>16);
				FILE_DATA_BUF[0x15] = (u8)(CH375vStartCluster>>24);
				FILE_DATA_BUF[0x20+0x1A] = (u8)UpDirCluster;  /* 上级目录的起始簇号 */
				FILE_DATA_BUF[0x20+0x1B] = (u8)(UpDirCluster>>8);
				FILE_DATA_BUF[0x20+0x14] = (u8)(UpDirCluster>>16);
				FILE_DATA_BUF[0x20+0x15] = (u8)(UpDirCluster>>24);

				for ( count = 0x40; count != CH375vSectorSize; count ++ ) 
				{  /* 清空目录区剩余部分 */
					*DirXramBuf = 0;
					DirXramBuf ++;
				}
				mCmdParam.Write.mSectorCount = 1;
				i = CH375FileWrite( );  /* 写目录的第一个扇区 */
				if ( i == ERR_SUCCESS ) 
				{
					DirXramBuf = &FILE_DATA_BUF[0];
					for ( i = 0x40; i != 0; i -- ) 
					{  /* 清空目录区 */
						*DirXramBuf = 0;
						DirXramBuf ++;
					}
					for ( j = 1; j != CH375vSecPerClus; j ++ ) 
					{
//						if ( &FILE_DATA_BUF[0] == &DISK_BASE_BUF[0] ) CH375DirtyBuffer( );  /* 如果FILE_DATA_BUF与DISK_BASE_BUF合用则必须清除磁盘缓冲区 */
						mCmdParam.Write.mSectorCount = 1;
						i = CH375FileWrite( );  /* 清空目录的剩余扇区 */
						if ( i != ERR_SUCCESS ) break;
					}
					if ( j == CH375vSecPerClus ) 
					{  /* 成功清空目录 */
						mCmdParam.Modify.mFileSize = 0;  /* 目录的长度总是0 */
						mCmdParam.Modify.mFileDate = 0xFFFF;
						mCmdParam.Modify.mFileTime = 0xFFFF;
						mCmdParam.Modify.mFileAttr = 0x10;  /* 置目录属性 */
						i = CH375FileModify( );  /* 将文件信息修改为目录 */
					}
				}
			}
		}
	}
	return( i );
}
/**************************************************************
** 函数名:ReadFile
** 功能:读取文件到指定缓冲区
** 注意事项:需要打开文件后才能调用, 
** 返回值为错误代码,buf:缓冲指针,len 要读取的值,cnt已读取的值
** 这里使用读扇区的方式读取文件,如果文件的偏移是跨扇区的,则程序 
** 内部有对应的处理方式 
**		这个没空写,迟点再说~
***************************************************************/
u8 ReadFile(u8* buf,u32 len,u32* cnt)
{
	return ERR_SUCCESS;
}

/**************************************************************
** 函数名:WriteFile
** 功能:写文件
** 注意事项:1.采用了过渡的缓冲区,虽然这样速度会有所下降,但可以使函数写的更规范更容易用于移植 
**  		
**  		2.为了能加快速度,过渡缓冲只用于处理写入缓冲少于一个扇区的情况,也就是说,如果len大于一个扇区,则扇区
**  		采用直接写入方式
			3.此函数的效率还有待提高
***************************************************************/
u8 WriteFile(u8* buf,u32 len)
{
	u32 i;
	u8* tmp_buf;
	u8 rtn;
	
	if(len==0)
	{
		return ERR_SUCCESS;
	}

	//每次只写入一个扇区
	i=0;
	tmp_buf=buf;
	mCmdParam.Write.mSectorCount=1;
	//整片扇区写入
	while(len>=CH375vSectorSize)
	{
		len-=CH375vSectorSize;
		mCmdParam.WriteX.mDataBuffer =tmp_buf;
		i++;
		tmp_buf=buf+i*CH375vSectorSize;
		rtn=CH375FileWriteX();
		if(rtn!=ERR_SUCCESS)
		{
			return rtn;
		}
		//实时修改文件大小
		File_Size+=CH375vSectorSize;
	}
	//零碎写入
	if(len)
	{
		//搬运到过渡缓冲中
		for(i=0;i<len;i++)
		{
			FILE_DATA_BUF[i]=*tmp_buf++;
		}
		mCmdParam.WriteX.mDataBuffer=FILE_DATA_BUF;
		rtn=CH375FileWriteX();
		if(rtn!=ERR_SUCCESS)
		{
			return rtn;
		}
		//实时修改文件大小
		File_Size+=len;

		//零碎写入需修改FAT底层部分,使其大小为真实大小
		mCmdParam.Modify.mFileSize = File_Size;   /* 输入参数: 新的文件长度,扇区模式下涉及到零头数据不便自动更新长度 */
		mCmdParam.Modify.mFileAttr = 0xff;  /* 输入参数: 新的文件属性,为0FFH则不修改 */
		mCmdParam.Modify.mFileTime = 0xffff;  /* 输入参数: 新的文件时间,为0FFH则不修改 */
		mCmdParam.Modify.mFileDate = 0xffff;  /* 输入参数: 新的文件日期,为0FFH则不修改 */
		rtn=CH375FileModify();   /* 修改当前文件的信息,修改文件长度 */
	}
	return(rtn);
}

/**************************************************************
** 函数名:LocatFile
** 功能:定位文件指针
** 注意事项:0xffffffff为移动到文件末尾
***************************************************************/
u8 LocatFile(u32 offest)
{
	mCmdParam.Locate.mSectorOffset = offest;
	return(CH375FileLocate());
}
u32 GetLocat(void)
{
	return 0;
}

/**************************************************************
** 函数名:CH375_Init
** 功能:初始化CH375程序库和CH375芯片
** 注意事项:初始化失败返回FLASE
***************************************************************/
bool CH375_Init(void)
{
	u8 rtn;
	CH375_Configuration();		//375IO口初始化
	rtn = CH375LibInit( ); 		//375软件初始化
	if(rtn!=ERR_SUCCESS)
	{
		printf("CH375程序库和IO口初始化失败！\r\n");
		return(FALSE);	
	}
	printf("CH375程序库和IO口初始化成功！\r\n");

	rtn=CH375DiskConnect();		//检查是否连接
	if(rtn!=ERR_SUCCESS)
	{
		printf("U盘未连接！\r\n");
		return(FALSE);
	}
	printf("U盘已连接！\r\n");
	//这里需要延迟一下.如果U盘没有准备好

	//检查U盘是否准备好-加上这一步骤,则可兼容大多数U盘
	rtn=CH375DiskReady();
	if(rtn!=ERR_SUCCESS)
	{
		printf("U盘未准备好了！\r\n");
		return(FALSE);
	}
	printf("U盘准备好了！\r\n");

#if DISK_BASE_BUF_LEN
	if( DISK_BASE_BUF_LEN < CH375vSectorSize ) // 检查磁盘数据缓冲区是否足够大,CH375vSectorSize是U盘的实际扇区大小 
	{ 
        printf( "磁盘数据缓冲区太小！\n" );		
		return(FALSE);						   //磁盘数据缓冲区太小
	}
	printf("U盘的扇区大小:%d\r\n",CH375vSectorSize);
#endif	

	rtn=CH375DiskSize( );  					 //查询磁盘物理容量 
	if(rtn!=ERR_SUCCESS)
	{
		printf("查询磁盘物理容量失败！\r\n");
		return(FALSE);
	}
	printf( "U盘容量：%u MB \r\n", (unsigned int)( mCmdParam.DiskSize.mDiskSizeSec * (CH375vSectorSize/512)/2048));  // 显示为以MB为单位的容量
	return(TRUE);
}



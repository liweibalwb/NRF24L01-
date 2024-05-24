/********************************************************************************
Modify by:		Andy.wu
Date:			2006.6.11
Description:	This file is from LwIP, and modified for ucos
 
在上面的基础上我做了一些修改：
Modify by:		OneTree
Date:			2013/1/19
Description:	主要加入了注释^_^,和对分配空间的大小做了最小限制，避免出现难以利用的碎片
				当前最大可分配的内存大小为 MEM_SIZE-SIZEOF_STRUCT_MEM	              
********************************************************************************/


#include 	"mem_malloc.h"
//#include	"string.h"							

#define MEM_ALIGNMENT	4		 //4字节对齐保证读写效率，因为STM32是32位的


struct mem
{
    mem_size_t next, prev;
    u8_t used;
};

static struct mem *ram_end;			  //最后一个节点，主要用作标记,标记到了链表尾部
__align(4) u8_t ram[MEM_SIZE + sizeof(struct mem) + MEM_ALIGNMENT] __attribute__((at(0x68000000)));	 //管理的内存大小
//static u8_t ram[MEM_SIZE + sizeof(struct mem) + MEM_ALIGNMENT];	 //管理的内存大小

#define MIN_SIZE 12	  //空闲块最小值

#define SIZEOF_STRUCT_MEM (sizeof(struct mem) + \
    (((sizeof(struct mem) % MEM_ALIGNMENT) == 0)? 0 : \
    (4 - (sizeof(struct mem) % MEM_ALIGNMENT))))

static struct mem *lfree; /*总是指向地址最低的空闲块的指针*/

/******************************************************************************
* 函数名  : MemCpy()
* 描述    : 将*des中的0到Byte数据段设置为值C
* 输入    : *des:数据目标指针   n:设置数目 	dat:设置值
* 输出    : None
* 返回    : None
* 作者    : 
* 创建日期:
* 版本    : 
*******************************************************************************/
static void MemSet(u8 *des,u8 dat,u32 n)
{
		if(!n) return;
		while(n--){
			*des++ = dat;	
		}
}

/******************************************************************************
* 函数名  : MemCpy()
* 描述    : 将*src中的数据拷贝到*src中
* 输入    : *dst:数据目标指针   *src:数据源指针
* 输出    : None
* 返回    : None
* 作者    : 
* 创建日期: 
* 版本    : 
*******************************************************************************/
static void MemCpy(u8 *des,u8 *src,u16 n)
{
		if(!n) return;	
		while(n--)
		{
			*des++ = *src++;
		}
}


/******************************************************************************
* 函数名  : plug_holes()
* 描述    : 合并相邻的空闲碎块
* 输入    : 
* 输出    : None
* 返回    : None
* 作者    :
* 创建日期: 
* 版本    : 
*******************************************************************************/
static void plug_holes(struct mem *mem)
{
    struct mem *nmem;
    struct mem *pmem;

    ASSERT("plug_holes: mem >= ram", (u8_t*)mem >= ram);
    ASSERT("plug_holes: mem < ram_end", (u8_t*)mem < (u8_t*)ram_end);
    ASSERT("plug_holes: mem->used == 0", mem->used == 0);
    /* plug hole forward */
    ASSERT("plug_holes: mem->next <= MEM_SIZE", mem->next <= MEM_SIZE);

    nmem = (struct mem*) &ram[mem->next];
    if (mem != nmem && nmem->used == 0 && (u8_t*)nmem != (u8_t*)ram_end)
    {
        if (lfree == nmem)
        {
            lfree = mem;
        } mem->next = nmem->next;
        ((struct mem*) &ram[nmem->next])->prev = (u8_t*)mem - ram;
    }

    /* plug hole backward */
    pmem = (struct mem*) &ram[mem->prev];
    if (pmem != mem && pmem->used == 0)
    {
        if (lfree == mem)
        {
            lfree = pmem;
        } pmem->next = mem->next;
        ((struct mem*) &ram[mem->next])->prev = (u8_t*)pmem - ram;
    }

}
/******************************************************************************
* 函数名  : mem_init()
* 描述    : 初始化内存池
*		  :	使用内存分配或者释放内存前必须先调用此函数初始化内存池!
* 输入    : None
* 输出    : None
* 返回    : None
* 作者    :
* 创建日期: 
* 版本    :
*******************************************************************************/
void mem_init(void)
{
    struct mem *mem;

    MemSet(ram, 0, MEM_SIZE);
    mem = (struct mem*)ram;
    mem->next = MEM_SIZE;
    mem->prev = 0;
    mem->used = 0;
    ram_end = (struct mem*) &ram[MEM_SIZE];
    ram_end->used = 1;
    ram_end->next = MEM_SIZE;
    ram_end->prev = MEM_SIZE;

    lfree = (struct mem*)ram;

}
/******************************************************************************
* 函数名  : mem_free()
* 描述    : 释放内存
* 输入    : *rmem:指向要释放的内存的指针
* 输出    : None
* 返回    : None
* 作者    :
* 创建日期: 
* 版本    : 
*******************************************************************************/
void mem_free(void *rmem)
{
    struct mem *mem;

    if (rmem == NULL)	  //判断要释放的内存是否为空
    {
        return ;
    }
    ASSERT("mem_free: legal memory", (u8_t*)rmem >= (u8_t*)ram && (u8_t*) rmem < (u8_t*)ram_end);
    
	if ((u8_t*)rmem < (u8_t*)ram || (u8_t*)rmem >= (u8_t*)ram_end)//判断要释放的内存是否为管理内的内存
    {
        return ;
    }
    mem = (struct mem*)((u8_t*)rmem - SIZEOF_STRUCT_MEM);
    ASSERT("mem_free: mem->used", mem->used);
    mem->used = 0;				 //将该内存块标记为，未使用的状态
    if (mem < lfree)			//判断释放的内存空间地址是否比lfree指向的地址低
    {							//如果释放的内存空间地址低，则改变lfree,让lfree指向它
        lfree = mem;
    }
    plug_holes(mem);

}
/******************************************************************************
* 函数名  : *mem_reallocm()
* 描述    : 重新分配内存，并且将旧内存中的数据拷贝到新的内存区中
* 输入    : *rmem:要重新分配内存的指针  newsize:重新分配的内存的大小(单位:字节)
* 输出    : None
* 返回    : nmem:重新分配好的内存的头地址
* 作者    :
* 创建日期: 
* 版本    : 
*******************************************************************************/
void *mem_reallocm(void *rmem, mem_size_t newsize)
{
    void *nmem;
    nmem = mem_malloc(newsize);
    if (nmem == NULL)
    {
        return mem_realloc(rmem, newsize);
    }
    MemCpy(nmem, rmem, newsize);
    mem_free(rmem);
    return nmem;
}
/******************************************************************************
* 函数名  : *mem_realloc()
* 描述    : 重新分配内存，但不将旧内存中的数据拷贝到新的内存区中
* 输入    : *rmem:要重新分配内存的指针  newsize:重新分配的内存的大小(单位:字节)
* 输出    : None
* 返回    : rmem:重新分配好的内存的头地址
* 作者    :
* 创建日期: 
* 版本    : 
*******************************************************************************/
void *mem_realloc(void *rmem, mem_size_t newsize)
{
    mem_size_t size;
    mem_size_t ptr, ptr2;
    struct mem *mem,  *mem2;

    /* Expand the size of the allocated memory region so that we can
    adjust for alignment. */
    if ((newsize % MEM_ALIGNMENT) != 0)
    {
        newsize += MEM_ALIGNMENT - ((newsize + SIZEOF_STRUCT_MEM) % MEM_ALIGNMENT);
    }
	if(newsize < MIN_SIZE){
		return NULL;
	}	
    if (newsize > MEM_SIZE)
    {
        return NULL;
    }
	ASSERT("mem_realloc: legal memory", 
	      (u8_t*)rmem >= (u8_t*)ram && (u8_t*)rmem < (u8_t*)ram_end);

    if ((u8_t*)rmem < (u8_t*)ram || (u8_t*)rmem >= (u8_t*)ram_end)
    {
        return rmem;
    }
    mem = (struct mem*)((u8_t*)rmem - SIZEOF_STRUCT_MEM);
    ptr = (u8_t*)mem - ram;
    size = mem->next - ptr - SIZEOF_STRUCT_MEM; 
    if (newsize + SIZEOF_STRUCT_MEM + MIN_SIZE < size)//分割
    {
        ptr2 = ptr + SIZEOF_STRUCT_MEM + newsize;
        mem2 = (struct mem*) &ram[ptr2];
        mem2->used = 0;
        mem2->next = mem->next;
        mem2->prev = ptr;
        mem->next = ptr2;
        if (mem2->next != MEM_SIZE)
        {
            ((struct mem*) &ram[mem2->next])->prev = ptr2;
        }

        plug_holes(mem2);
    }
    return rmem;
}
/******************************************************************************
* 函数名  : *mem_malloc()
* 描述    : 分配内存
* 输入    : size:需要分配内存的大小(单位:字节)
* 输出    : None
* 返回    : 分配好的内存的头地址
* 作者    :
* 创建日期: 
* 版本    :
*******************************************************************************/
void *mem_malloc(mem_size_t size)
{
    mem_size_t ptr, ptr2;
    struct mem *mem,  *mem2;
    if (size == 0)						//如果分配空间的大小为零，则分配失败，返回NULL
    {
        return NULL;
    }

    /* Expand the size of the allocated memory region so that we can
    adjust for alignment. */
    if ((size % MEM_ALIGNMENT) != 0)   
    {
        size += MEM_ALIGNMENT - ((size + SIZEOF_STRUCT_MEM) % MEM_ALIGNMENT);
    }
	if(size < MIN_SIZE){		      //如果分配的值小于最小内存空间则分配失败
		 return NULL;
	}
    if (size > MEM_SIZE)			  //需要分配的内存大于，额定的内存大小，分配失败
    {
        return NULL;
    }
	//ptr 保存空闲块的地址	查找到满足SIZE大小的空闲块	
	//如果lfree此空闲块不满足要求，则遍历所有链表，直到找到合适SIZE的空闲块或到链表尾部结束
    for (ptr = (u8_t*)lfree - ram; ptr < MEM_SIZE; ptr = ((struct mem*) &ram[ptr])->next)
    {
        mem = (struct mem*) &ram[ptr];					//mem指向当前空闲块的地址
        if (!mem->used && mem->next - (ptr + SIZEOF_STRUCT_MEM) >= size +
            SIZEOF_STRUCT_MEM + MIN_SIZE)					//如果找到了空闲块，并且此空间块符合分割条件
        {													//则将此空闲块的节点插入到链表中
            ptr2 = ptr + SIZEOF_STRUCT_MEM + size;	   //保存下一个空闲块的首地址
            mem2 = (struct mem*) &ram[ptr2];		   //mem2指向下一个空闲块中创建的结构体的首地址
            mem2->prev = ptr;						   //mem2的前驱保存当前空闲块的地址
            mem2->next = mem->next;					   //mem2的后驱保存下一个节点的地址
            mem->next = ptr2;						  //mem的后驱保存mem2空闲块的地址
            if (mem2->next != MEM_SIZE)				 //如果mem2的下一个节点不是ram_end
            {										  //则将下一个节点的前驱保存mem2
                ((struct mem*) &ram[mem2->next])->prev = ptr2;
            }
            mem2->used = 0;							 //标记mem2下一个空闲块未使用
            mem->used = 1;     						 //标记当前空闲块已被分配
            if (mem == lfree)						  //当前空闲块地址mem和指向空闲块的指针lfree相同
            {
                /* Find next free block after mem */		 
                while (lfree->used && lfree != ram_end)	 //lfree 找到下一个可用的空闲块
                {
                    lfree = (struct mem*) &ram[lfree->next];
                }
                ASSERT("mem_malloc: !lfree->used", !lfree->used);
            }
            ASSERT("mem_malloc: allocated memory properly aligned.", 
                   (unsigned long)((u8_t*)mem + SIZEOF_STRUCT_MEM) % MEM_ALIGNMENT == 0);			 
            return (u8_t*)mem + SIZEOF_STRUCT_MEM;	  //申请的空间地址，从空闲块的节点结束地址开始
        }else if(!mem->used && mem->next - (ptr + SIZEOF_STRUCT_MEM) >= size
           			)	//不满足分割条件，则整块分配		   
		{
			mem->used = 1;	
            if (mem == lfree)						  //当前空闲块地址mem和指向空闲块的指针lfree相同
            {
                /* Find next free block after mem */		 
                while (lfree->used && lfree != ram_end)	 //lfree 找到下一个可用的空闲块
                {
                    lfree = (struct mem*) &ram[lfree->next];
                }
                ASSERT("mem_malloc: !lfree->used", !lfree->used);
            }
            ASSERT("mem_malloc: allocated memory properly aligned.", 
                   (unsigned long)((u8_t*)mem + SIZEOF_STRUCT_MEM) % MEM_ALIGNMENT == 0);			 
            return (u8_t*)mem + SIZEOF_STRUCT_MEM;	  //申请的空间地址，从空闲块的节点结束地址开始			 		
		}

    }

    return NULL;
}

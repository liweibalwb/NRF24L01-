/********************************************************************************
Modify by:		Andy.wu
Date:			2006.6.11
Description:	This file is from LwIP, and modified for ucos
 
������Ļ�����������һЩ�޸ģ�
Modify by:		OneTree
Date:			2013/1/19
Description:	��Ҫ������ע��^_^,�ͶԷ���ռ�Ĵ�С������С���ƣ���������������õ���Ƭ
				��ǰ���ɷ�����ڴ��СΪ MEM_SIZE-SIZEOF_STRUCT_MEM	              
********************************************************************************/


#include 	"mem_malloc.h"
//#include	"string.h"							

#define MEM_ALIGNMENT	4		 //4�ֽڶ��뱣֤��дЧ�ʣ���ΪSTM32��32λ��


struct mem
{
    mem_size_t next, prev;
    u8_t used;
};

static struct mem *ram_end;			  //���һ���ڵ㣬��Ҫ�������,��ǵ�������β��
__align(4) u8_t ram[MEM_SIZE + sizeof(struct mem) + MEM_ALIGNMENT] __attribute__((at(0x68000000)));	 //������ڴ��С
//static u8_t ram[MEM_SIZE + sizeof(struct mem) + MEM_ALIGNMENT];	 //������ڴ��С

#define MIN_SIZE 12	  //���п���Сֵ

#define SIZEOF_STRUCT_MEM (sizeof(struct mem) + \
    (((sizeof(struct mem) % MEM_ALIGNMENT) == 0)? 0 : \
    (4 - (sizeof(struct mem) % MEM_ALIGNMENT))))

static struct mem *lfree; /*����ָ���ַ��͵Ŀ��п��ָ��*/

/******************************************************************************
* ������  : MemCpy()
* ����    : ��*des�е�0��Byte���ݶ�����ΪֵC
* ����    : *des:����Ŀ��ָ��   n:������Ŀ 	dat:����ֵ
* ���    : None
* ����    : None
* ����    : 
* ��������:
* �汾    : 
*******************************************************************************/
static void MemSet(u8 *des,u8 dat,u32 n)
{
		if(!n) return;
		while(n--){
			*des++ = dat;	
		}
}

/******************************************************************************
* ������  : MemCpy()
* ����    : ��*src�е����ݿ�����*src��
* ����    : *dst:����Ŀ��ָ��   *src:����Դָ��
* ���    : None
* ����    : None
* ����    : 
* ��������: 
* �汾    : 
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
* ������  : plug_holes()
* ����    : �ϲ����ڵĿ������
* ����    : 
* ���    : None
* ����    : None
* ����    :
* ��������: 
* �汾    : 
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
* ������  : mem_init()
* ����    : ��ʼ���ڴ��
*		  :	ʹ���ڴ��������ͷ��ڴ�ǰ�����ȵ��ô˺�����ʼ���ڴ��!
* ����    : None
* ���    : None
* ����    : None
* ����    :
* ��������: 
* �汾    :
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
* ������  : mem_free()
* ����    : �ͷ��ڴ�
* ����    : *rmem:ָ��Ҫ�ͷŵ��ڴ��ָ��
* ���    : None
* ����    : None
* ����    :
* ��������: 
* �汾    : 
*******************************************************************************/
void mem_free(void *rmem)
{
    struct mem *mem;

    if (rmem == NULL)	  //�ж�Ҫ�ͷŵ��ڴ��Ƿ�Ϊ��
    {
        return ;
    }
    ASSERT("mem_free: legal memory", (u8_t*)rmem >= (u8_t*)ram && (u8_t*) rmem < (u8_t*)ram_end);
    
	if ((u8_t*)rmem < (u8_t*)ram || (u8_t*)rmem >= (u8_t*)ram_end)//�ж�Ҫ�ͷŵ��ڴ��Ƿ�Ϊ�����ڵ��ڴ�
    {
        return ;
    }
    mem = (struct mem*)((u8_t*)rmem - SIZEOF_STRUCT_MEM);
    ASSERT("mem_free: mem->used", mem->used);
    mem->used = 0;				 //�����ڴ����Ϊ��δʹ�õ�״̬
    if (mem < lfree)			//�ж��ͷŵ��ڴ�ռ��ַ�Ƿ��lfreeָ��ĵ�ַ��
    {							//����ͷŵ��ڴ�ռ��ַ�ͣ���ı�lfree,��lfreeָ����
        lfree = mem;
    }
    plug_holes(mem);

}
/******************************************************************************
* ������  : *mem_reallocm()
* ����    : ���·����ڴ棬���ҽ����ڴ��е����ݿ������µ��ڴ�����
* ����    : *rmem:Ҫ���·����ڴ��ָ��  newsize:���·�����ڴ�Ĵ�С(��λ:�ֽ�)
* ���    : None
* ����    : nmem:���·���õ��ڴ��ͷ��ַ
* ����    :
* ��������: 
* �汾    : 
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
* ������  : *mem_realloc()
* ����    : ���·����ڴ棬���������ڴ��е����ݿ������µ��ڴ�����
* ����    : *rmem:Ҫ���·����ڴ��ָ��  newsize:���·�����ڴ�Ĵ�С(��λ:�ֽ�)
* ���    : None
* ����    : rmem:���·���õ��ڴ��ͷ��ַ
* ����    :
* ��������: 
* �汾    : 
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
    if (newsize + SIZEOF_STRUCT_MEM + MIN_SIZE < size)//�ָ�
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
* ������  : *mem_malloc()
* ����    : �����ڴ�
* ����    : size:��Ҫ�����ڴ�Ĵ�С(��λ:�ֽ�)
* ���    : None
* ����    : ����õ��ڴ��ͷ��ַ
* ����    :
* ��������: 
* �汾    :
*******************************************************************************/
void *mem_malloc(mem_size_t size)
{
    mem_size_t ptr, ptr2;
    struct mem *mem,  *mem2;
    if (size == 0)						//�������ռ�Ĵ�СΪ�㣬�����ʧ�ܣ�����NULL
    {
        return NULL;
    }

    /* Expand the size of the allocated memory region so that we can
    adjust for alignment. */
    if ((size % MEM_ALIGNMENT) != 0)   
    {
        size += MEM_ALIGNMENT - ((size + SIZEOF_STRUCT_MEM) % MEM_ALIGNMENT);
    }
	if(size < MIN_SIZE){		      //��������ֵС����С�ڴ�ռ������ʧ��
		 return NULL;
	}
    if (size > MEM_SIZE)			  //��Ҫ������ڴ���ڣ�����ڴ��С������ʧ��
    {
        return NULL;
    }
	//ptr ������п�ĵ�ַ	���ҵ�����SIZE��С�Ŀ��п�	
	//���lfree�˿��п鲻����Ҫ���������������ֱ���ҵ�����SIZE�Ŀ��п������β������
    for (ptr = (u8_t*)lfree - ram; ptr < MEM_SIZE; ptr = ((struct mem*) &ram[ptr])->next)
    {
        mem = (struct mem*) &ram[ptr];					//memָ��ǰ���п�ĵ�ַ
        if (!mem->used && mem->next - (ptr + SIZEOF_STRUCT_MEM) >= size +
            SIZEOF_STRUCT_MEM + MIN_SIZE)					//����ҵ��˿��п飬���Ҵ˿ռ����Ϸָ�����
        {													//�򽫴˿��п�Ľڵ���뵽������
            ptr2 = ptr + SIZEOF_STRUCT_MEM + size;	   //������һ�����п���׵�ַ
            mem2 = (struct mem*) &ram[ptr2];		   //mem2ָ����һ�����п��д����Ľṹ����׵�ַ
            mem2->prev = ptr;						   //mem2��ǰ�����浱ǰ���п�ĵ�ַ
            mem2->next = mem->next;					   //mem2�ĺ���������һ���ڵ�ĵ�ַ
            mem->next = ptr2;						  //mem�ĺ�������mem2���п�ĵ�ַ
            if (mem2->next != MEM_SIZE)				 //���mem2����һ���ڵ㲻��ram_end
            {										  //����һ���ڵ��ǰ������mem2
                ((struct mem*) &ram[mem2->next])->prev = ptr2;
            }
            mem2->used = 0;							 //���mem2��һ�����п�δʹ��
            mem->used = 1;     						 //��ǵ�ǰ���п��ѱ�����
            if (mem == lfree)						  //��ǰ���п��ַmem��ָ����п��ָ��lfree��ͬ
            {
                /* Find next free block after mem */		 
                while (lfree->used && lfree != ram_end)	 //lfree �ҵ���һ�����õĿ��п�
                {
                    lfree = (struct mem*) &ram[lfree->next];
                }
                ASSERT("mem_malloc: !lfree->used", !lfree->used);
            }
            ASSERT("mem_malloc: allocated memory properly aligned.", 
                   (unsigned long)((u8_t*)mem + SIZEOF_STRUCT_MEM) % MEM_ALIGNMENT == 0);			 
            return (u8_t*)mem + SIZEOF_STRUCT_MEM;	  //����Ŀռ��ַ���ӿ��п�Ľڵ������ַ��ʼ
        }else if(!mem->used && mem->next - (ptr + SIZEOF_STRUCT_MEM) >= size
           			)	//������ָ����������������		   
		{
			mem->used = 1;	
            if (mem == lfree)						  //��ǰ���п��ַmem��ָ����п��ָ��lfree��ͬ
            {
                /* Find next free block after mem */		 
                while (lfree->used && lfree != ram_end)	 //lfree �ҵ���һ�����õĿ��п�
                {
                    lfree = (struct mem*) &ram[lfree->next];
                }
                ASSERT("mem_malloc: !lfree->used", !lfree->used);
            }
            ASSERT("mem_malloc: allocated memory properly aligned.", 
                   (unsigned long)((u8_t*)mem + SIZEOF_STRUCT_MEM) % MEM_ALIGNMENT == 0);			 
            return (u8_t*)mem + SIZEOF_STRUCT_MEM;	  //����Ŀռ��ַ���ӿ��п�Ľڵ������ַ��ʼ			 		
		}

    }

    return NULL;
}

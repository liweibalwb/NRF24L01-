#ifndef _MEM_MALLOC_H_
#define _MEM_MALLOC_H_

#include	"stm32f10x.h"
#include	"util.h"

//MEM_SIZE is the total size 
#define MEM_SIZE	200*1024


typedef	unsigned int 	mem_size_t;
typedef	unsigned char	u8_t;
typedef unsigned short	u16_t;
typedef unsigned int 	u32_t;

void mem_init(void);
void *mem_malloc(mem_size_t size);
void mem_free(void *mem);
void *mem_realloc(void *mem, mem_size_t size);
void *mem_reallocm(void *mem, mem_size_t size);

#ifndef NULL
#define NULL	0
#endif

#define null NULL


#endif 

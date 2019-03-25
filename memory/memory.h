/*******************************************************************************
 * ＊ 文件名：	memory.h
 * ＊ 描述	：	内存操作头文件
 * ＊ 创建者：	UnartyChen
 * ＊ 版本	：	V1.0
 * ＊ 日期	：	2015.03.13
 * ＊ 修改	：	2017.03.17 Unarty
                更新字节对齐方法 移除内存起始边界判断， 
                修改内存标记方法，MEMORY_FLAG^areaSize
 * ****************************************************************************/
#ifndef __MEMORY_H
#define	__MEMORY_H

#include "typedef.h"


void memory_init(void *memAddr, mcu_t memSize);
void *memory_apply(mcu_t size);
void memory_release(void *addr);

#endif//Memory.h end
/******************************************************************************/


/*******************************************************************************
 * �� �ļ�����	memory.h
 * �� ����	��	�ڴ����ͷ�ļ�
 * �� �����ߣ�	UnartyChen
 * �� �汾	��	V1.0
 * �� ����	��	2015.03.13
 * �� �޸�	��	2017.03.17 Unarty
                �����ֽڶ��뷽�� �Ƴ��ڴ���ʼ�߽��жϣ� 
                �޸��ڴ��Ƿ�����MEMORY_FLAG^areaSize
 * ****************************************************************************/
#ifndef __MEMORY_H
#define	__MEMORY_H

#include "typedef.h"


void memory_init(void *memAddr, mcu_t memSize);
void *memory_apply(mcu_t size);
void memory_release(void *addr);

#endif//Memory.h end
/******************************************************************************/


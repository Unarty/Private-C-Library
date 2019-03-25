/***************************Copyright QYKJ   2015-06-18*************************
��	����	aLgorithm.c
˵	����	�㷨ʵ�ֺ���
�ޡ��ģ�	2015.06.23 Unarty establish
            2015.06.27 Unarty Add �ֲ�����Ӻ��� BinaryFind
*******************************************************************************/
#include "algorithm.h"


static mcu_t BinaryFind(mcu_t start, mcu_t end, u16_t size);


/*******************************************************************************
  ������:  	ALG_binaryFind
����˵��:  	����ֲ����㷨
    ����:  	arm:		���ҵ�Ŀ��
    		srcStart:	����Դ��ʼ��ַ
    		srcEnd:		����Դ�Ľ�����ַ
    		onceSize:	����Դ�����ݿռ��С	
  ����ֵ:  	ƥ�䵽�����ݴ洢��ַ
*******************************************************************************/
void *ALG_binaryFind(u16_t arm, void *srcStart, void *srcEnd, u16_t onceSize)
{
    u8_t *cmp = (u8_t*)(BinaryFind((mcu_t)srcStart, (mcu_t)srcEnd, onceSize));

	while ((void*)cmp < srcEnd)
	{
        if (arm > *(u16_t*)cmp)
        {
            srcStart = cmp + onceSize;
            cmp = (u8_t*)(BinaryFind((mcu_t)srcStart, (mcu_t)srcEnd, onceSize));;
        }
        else if (arm < *(u16_t*)cmp)
        {
            srcEnd = cmp;
            cmp = (u8_t*)(BinaryFind((mcu_t)srcStart, (mcu_t)srcEnd, onceSize));;
        }
        else 
        {
            return cmp;
        }
	}

    return null;
}


/*******************************************************************************
  ������:  	BinaryFind
����˵��:  	����ֶ�λ
    ����:  	start:	����Դ��ʼ��ַ
    		end:	����Դ�Ľ�����ַ
    		size:	����Դ�����ݿռ��С	
  ����ֵ:  	ƫ�Ƶ�ֵַ; ��λ(byte)
*******************************************************************************/
static mcu_t BinaryFind(mcu_t start, mcu_t end, u16_t size)
{
    mcu_t i = (end - start)>>1;     // n/2

	i = i - i%size + start;

    return i;
}


/*******************************Copyright QYKJ*********************************/


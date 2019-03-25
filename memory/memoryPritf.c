/********************************Copyright I&S Team*****************************
 * �� �ļ�����	memory.c
 * �� ����	��	�ڴ����ʵ��ԭ��
 * �� �����ߣ�	UnartyChen
 * �� �汾	��	V1.0
 * �� ����	��	2015.03.12
 * �� �޸�	��	2017.03.17 Unarty
                �����ֽڶ��뷽�� line = 50. �Ƴ��ڴ���ʼ�߽��жϣ� 
                �޸��ڴ��Ƿ�����MEMORY_FLAG^areaSize
                2017.09.05 Unarty
                ���·���Bug, ���ɷ������� n - size < sizeof(MemArea_t) ʱ, ��������½���ڴ�ֵ����
                �޸ķ����ڴ��Ƿ�����
 * ****************************************************************************/
#include "memory.h"
#include "swap.h"

#define MEMORY_FLAG		(0xA55A55AA)//((void*)0x4885A112)
#define MEMORY_ALIGNMENT (4)                //�ڴ���С�߽����
   
typedef struct memArea
{
	struct memArea	*nextArea;
	mcu_t			areaSize;
}MemArea_t;		//�ڴ����������

static MemArea_t *mem;
static mcu_t startAddr;
static mcu_t endAddr;

/*******************************************************************************
 * �� ��������	memory_init
 * �� ����	��	�ڴ��������ռ��ʼ��
 * �� ����	��	memAddr:	��̬�ڴ����ĵ�ַ
				memSize:	��̬�ڴ����Ŀռ�
 * �� ����	��	��
 * �� ע��	��	��
 * ****************************************************************************/
void memory_init(void *memAddr, mcu_t memSize)
{
	mem = (MemArea_t*)memAddr;
	mem->areaSize = memSize;
	mem->nextArea = null;
	startAddr = (int)memAddr;
	endAddr = startAddr + memSize;
}

/*******************************************************************************
 * �� ��������	memory_apply
 * �� ����	��	�ڴ�����
 * �� ����	��	size:	�ռ��С
 * �� ����	��	����ĵ�ַ��null������ʧ�ܣ�����Ч�ڴ��ַ������ɹ�)
 * ****************************************************************************/
void *memory_apply(mcu_t size)
{
    MemArea_t *tail, *pion;
    void *memAddr;

    /*�ֽڶ���*/
    size += sizeof(MemArea_t) + (MEMORY_ALIGNMENT - 1);
    size &= ~(MEMORY_ALIGNMENT - 1);
    
    for (pion = (MemArea_t*)&mem, tail = mem; tail != null; pion = tail, tail = tail->nextArea)
    {
        if (tail->areaSize >= size)
        {
            memAddr = tail;
            if ((tail->areaSize - size) < (sizeof(MemArea_t) + MEMORY_ALIGNMENT))
            {
                pion->nextArea = tail->nextArea;
                size = tail->areaSize;
            }
            else
            {
                tail = (MemArea_t*)((u8_t*)memAddr+size);
                tail->areaSize = ((MemArea_t*)memAddr)->areaSize - size;
                tail->nextArea = ((MemArea_t*)memAddr)->nextArea;
                pion->nextArea = tail;	
                ((MemArea_t*)memAddr)->areaSize = size;
            }
            
            ((MemArea_t*)memAddr)->nextArea = (void*)((~size)^MEMORY_FLAG);
            
            return ((u8_t*)memAddr+sizeof(MemArea_t));
        }			
    }

	return null;
}
/*******************************************************************************
 * �� ��������	memory_release
 * �� ����	��	�ڴ��ͷ�
 * �� ����	��	addr:	Ŀ���ͷŵ�ַ
 * �� ����	��	��
 * ****************************************************************************/
void memory_release(void *addr)
{
    if (null != addr && (int)addr > startAddr && (int)addr < endAddr)
    {
        MemArea_t *tail, *pion;
        MemArea_t *rlsArea = (MemArea_t*)((mcu_t)addr - sizeof(MemArea_t));
        
        if ((void*)((~rlsArea->areaSize)^MEMORY_FLAG) == rlsArea->nextArea)
        {
            /*�ҵ�����ڵ�*/
            for (pion = (MemArea_t*)&mem, tail = mem; (tail < rlsArea)&& (tail != null); 
                            pion = tail, tail = tail->nextArea)
            {}	
            
            rlsArea->nextArea = tail;
            pion->nextArea = rlsArea;
            if (pion != (MemArea_t*)&mem)//Ŀ�겻����ͷ�ڵ�
            {
                rlsArea = pion;
            }
            while (rlsArea != null &&  rlsArea < tail)
            {
                if (((u8_t*)(rlsArea) + rlsArea->areaSize) == (u8_t*)rlsArea->nextArea)	
                {
                    rlsArea->areaSize += rlsArea->nextArea->areaSize;
                    rlsArea->nextArea = rlsArea->nextArea->nextArea;
                }
                else
                {
                    rlsArea = rlsArea->nextArea;
                }
            }
        }
    }
}

void memory_trans(void_fun_u8p_u16 fun)
{
	MemArea_t *tail;
	char str[12];

	for (tail = mem; tail != null; tail = tail->nextArea)
	{
		Swap_numChar(str, tail->areaSize);
		fun((u8_t*)str, r_strlen(str));
		fun(", 0x", 4);
		Swap_hexChar(str, &tail->nextArea, 4, 0);
		fun((u8_t*)str, r_strlen(str));
		fun("; ", 2);
	}
	fun("\r\n", 3);
}

	
/******************************************************************************/


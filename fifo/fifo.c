/***************************Copyright QYKJ   2015-06-18*************************
��	����	main.c
˵	����	ϵͳ������
�ޡ��ģ�	2015.06.18 Unarty establish
*******************************************************************************/
#include "fifo.h"
#include "stdlib/r_stdlib.h"



/*******************************************************************************
�� �� ����  fifo_init
����˵����  ���г�ʼ��
��	  ����  fifo:  	����ͷ
            buf:  	���ݴ洢�׵�ַ
            size:   ���ݴ洢���򳤶�
�� �� ֵ��  ��ʼ����� 0/1
*******************************************************************************/
void fifo_init(FIFO_t *fifo, void *buf, mcu_t size)
{
    ERRR((null == fifo)||(null == buf)||(0 == size), return);
    fifo->buf = buf;
    fifo->size = size;
    fifo->front = fifo->rear;
}

/*******************************************************************************
�� �� ����  fifo_reset
����˵����  �������
��	  ����  fifo:  ����ͷ
�� �� ֵ��  ��
*******************************************************************************/
void fifo_reset(FIFO_t *fifo)
{
    ERRR(fifo == null, return);
    fifo->front = fifo->rear;
}

/*******************************************************************************
�� �� ����  fifo_clearn
����˵����  ��������
��	  ����  fifo:  ����ͷ
            len:   ������
�� �� ֵ��  ��
*******************************************************************************/
void fifo_clearn(FIFO_t *fifo, mcu_t len)
{
    ERRR(fifo == null, return);

    if (len >= fifo_userSpace(fifo))
    {
        fifo_reset(fifo);
    }
    else
    {
        fifo->front = (fifo->front + len) % fifo->size;
    }
}

/*******************************************************************************
�� �� ����  fifo_offset
����˵����  ����ͷָ��ƫ��
��	  ����  fifo:  ����ͷ
            offset:   ƫ��λ��
�� �� ֵ��  ��
*******************************************************************************/
void fifo_offset(FIFO_t *fifo, mcu_t offset)
{
    ERRR(fifo == null, return);

    fifo->front = offset % fifo->size;

}


/*******************************************************************************
�� �� ����  fifo_empty
����˵����  �ж϶����Ƿ�Ϊ��
��	  ����  *fifo:  ����ͷ
�� �� ֵ��  0(����Ϊ��)/1
*******************************************************************************/
mcu_t fifo_empty(FIFO_t *fifo)
{  
    return ((fifo->front == fifo->rear) ? 0 : 1);
}

/*******************************************************************************
�� �� ����  fifo_full
����˵����  �ж϶����Ƿ�����
��	  ����  *fifo:  ����ͷ
�� �� ֵ��  0(��������)/1
*******************************************************************************/
mcu_t fifo_full(FIFO_t *fifo)
{  
    return ((fifo->front == ((fifo->rear+1)%fifo->size)) ? 0 : 1);
}

/*******************************************************************************
�� �� ����  fifo_freeSpace
����˵����  ���п��ÿռ�
��	  ����  *fifo:  ����ͷ
�� �� ֵ��  ʣ��ռ��С(�ַ�Ϊ��λ)
*******************************************************************************/
mcu_t fifo_freeSpace(FIFO_t *fifo)
{
    return ((fifo->front + fifo->size  - fifo->rear - 1)%fifo->size);
}

/*******************************************************************************
�� �� ����  fifo_userSpace
����˵����  �������ÿռ�
��	  ����  *fifo:  ����ͷ
�� �� ֵ��  ���ÿռ��С(�ַ�Ϊ��λ)
*******************************************************************************/
mcu_t fifo_userSpace(FIFO_t *fifo)
{

	return ((fifo->rear + fifo->size - fifo->front)%fifo->size);
}

/*******************************************************************************
�� �� ����  fifo_puts
����˵����  ���
��	  ����  *fifo:  ����ͷ
            *data:  �������
            len:    ���ݳ���
�� �� ֵ��  ��ӽ��(0ʧ��)/(��ӳ���)
*******************************************************************************/
mcu_t fifo_puts(FIFO_t *fifo, void *data, mcu_t len)
{  
	mcu_t size;
    int i;
    
    i = len - fifo_freeSpace(fifo);
    
    if (i > 0)
    {
        fifo->front = (fifo->front + i)%fifo->size;
    }
    
	size = MIN(len, fifo->size - fifo->rear);
	r_memcpy(fifo->buf + fifo->rear, data, size);
	r_memcpy(fifo->buf, (u8_t*)data + size, len - size);

	fifo->rear = (fifo->rear + len)%fifo->size;

    return len;   
}

/*******************************************************************************
�� �� ����  fifo_gets
����˵����  ����
��	  ����  *fifo:  ����ͷ
            *data:  ��������
            len:    �������ݳ���
�� �� ֵ��  ���ӽ�� 0/1
*******************************************************************************/
mcu_t fifo_gets(FIFO_t *fifo, void *data, mcu_t len)
{
	mcu_t size;

	len = MIN(len, fifo_userSpace(fifo));
	size = MIN(len, fifo->size - fifo->front);
	r_memcpy(data, fifo->buf + fifo->front, size);
	r_memcpy((u8_t*)data + size, fifo->buf, len - size);

	fifo->front = (fifo->front + len)%fifo->size;
 
    return len;   
}

/*******************************************************************************
�� �� ����  fifo_putc
����˵����  ���һ���ַ�
��	  ����  *fifo:  ����ͷ
            data:   Ҫ��ӵ�����
�� �� ֵ��  ��ӽ�� 0/1
*******************************************************************************/
mcu_t fifo_putc(FIFO_t *fifo, u8_t data)
{
    if (0 == fifo_full(fifo)) //�ж϶������Ƿ��д洢�ռ�
    {
        fifo->front = (fifo->front + 1)%fifo->size;
    }
    
    fifo->buf[fifo->rear++] = data;

    fifo->rear %= fifo->size;

    return 0;   
}

/*******************************************************************************
�� �� ����  fifo_getc
����˵����  ����һ���ַ�
��	  ����  *fifo:  ����ͷ
�� �� ֵ��  ��������
*******************************************************************************/
mcu_t fifo_getc(FIFO_t *fifo, u8_t *data)
{
    ERRR(0 == fifo_empty(fifo), return 1); //����Ϊ��

    *data = fifo->buf[fifo->front++];
    fifo->front %= fifo->size;

    return 0;   
}

/*******************************************************************************
�� �� ����  fifo_dataGet
����˵����  �������ݻ�ȡ
��	  ����  *fifo:  ����ͷ
            offset: ����ƫ��λ�ã� 
            len:    Ŀ�곤��
�� �� ֵ��  ��������
*******************************************************************************/
void fifo_dataGet(FIFO_t *fifo, u8_t *trg, mcu_t offset, mcu_t len)
{  
    mcu_t size;
    ERRR(0 == fifo_empty(fifo), return); //����Ϊ��   
    
    offset %= fifo->size;
	size = MIN(len, fifo->size - offset);
    
	r_memcpy(trg, fifo->buf + offset, size);
	r_memcpy(trg + size, fifo->buf, len - size);
}


/*******************************************************************************
�� �� ����  fifo_Find
����˵����  �������ݲ���
��	  ����  fifo:  		����ͷ
			findStart:	����ͷ
			findStop:	����β
            *data:  ����ƥ������
            len:    ƥ�����ݳ���
�� �� ֵ��  ����ƥ��ָ�룬���ҵ���λ�� 
*******************************************************************************/
mcu_t fifo_find(FIFO_t *fifo, mcu_t findStart, mcu_t findStop, const u8_t *cmpData, mcu_t cmpSize)
{
	mcu_t i = 0;
	
    findStart %= fifo->size;
	findStop %= fifo->size;
    
	while (findStart != findStop)
	{
		if (fifo->buf[findStart] == cmpData[i])	//�����������
		{
			if (++i == cmpSize)	//ƥ�䳤�����
			{
                i -= 1;
				return ((findStart + fifo->size - i)%fifo->size);
			}
		}
		else
		{
			i = 0;
		}
        
        findStart++;
		findStart %= fifo->size;
	}
	
	return (findStop);
}

/*******************************Copyright QYKJ*********************************/


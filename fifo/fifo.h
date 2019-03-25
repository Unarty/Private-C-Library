/***************************Copyright QYKJ   2015-06-18*************************
文	件：	main.c
说	明：	系统主函数
修　改：	2015.06.18 Unarty establish
*******************************************************************************/
#ifndef __FIFO_H
#define __FIFO_H

#include "typedef.h"


typedef struct
{
	mcu_t   	front;
	mcu_t   	rear;
	mcu_t   	size;
	u8_t 	 	*buf;
}FIFO_t;



void 	fifo_init(FIFO_t *fifo, void *buf, mcu_t size); //队列初始化
void 	fifo_reset(FIFO_t *fifo);                    //队列清空
void    fifo_clearn(FIFO_t *fifo, mcu_t len);         //队列清理
void    fifo_offset(FIFO_t *fifo, mcu_t offset);
mcu_t 	fifo_freeSpace(FIFO_t *fifo);     		//队列可用空间
mcu_t 	fifo_userSpace(FIFO_t *fifo);        	//队列已用空间
mcu_t	fifo_empty(FIFO_t *fifo);					//队列是否为空
mcu_t	fifo_full(FIFO_t *fifo);						//队列滿判断f
mcu_t 	fifo_puts(FIFO_t *fifo, void *buf, mcu_t len);
mcu_t	fifo_gets(FIFO_t *fifo, void *buf, mcu_t len);
mcu_t 	fifo_putc(FIFO_t *fifo, u8_t dat);
mcu_t 	fifo_getc(FIFO_t *fifo, u8_t *data);
void    fifo_dataGet(FIFO_t *fifo, u8_t *trg, mcu_t offset, mcu_t len);  //队列数据获取
mcu_t 	fifo_find(FIFO_t *fifo, mcu_t findStart, mcu_t findStop, const u8_t *cmpData, mcu_t cmpSize);
							
#endif //fifo.h end
/*******************************Copyright QYKJ*********************************/


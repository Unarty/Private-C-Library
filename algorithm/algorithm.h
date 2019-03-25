/***************************Copyright QYKJ   2015-06-18*************************
文	件：	aLgorithm.h
说	明：	算法实现函数
修　改：	2015.06.23 Unarty establish
*******************************************************************************/
#ifndef __ALGORITHM_H
#define __ALGORITHM_H

#include "typedef.h"


#define BINARY_FIND(addr)       (void*)addr, (void*)(&addr + 1), sizeof(addr[0])    //

void *ALG_binaryFind(u16_t arm, void *srcStart, void *srcEnd, u16_t onceSize);

#endif //algorithm.h end
/*******************************Copyright QYKJ*********************************/


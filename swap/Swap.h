/***************************Copyright QYKJ   2015-06-18*************************
文	件：	Swap.h
说	明：	内存数据交换/转换相关函数
修　改：	2015.06.28 Unarty establish
*******************************************************************************/
#ifndef __SWAP_H
#define __SWAP_H

#include "typedef.h"
#include "List\List.h"

                                
void  Swap_bigSmallShort(u16_t *pShort);   //0xaa55 -> 0x55aa
void  Swap_bigSmallLong(u32_t *pLong);   //0x12345678 -> 0x87654321
void  Swap_headTail(u8_t *head, u16_t len);  //0xaabbccddeeffgg -> 0xggffeeddccbbaa
u16_t Swap_hexChar(char *buf, u8_t *hex, u16_t len, char fill); //0x12,0x34,0x56,0x78,0xab,0x0c -> "12 34 56 78 ab 0c" 返回字符长度
u16_t Swap_charHex(u8_t *hex, char *buf); //"12 34 56 87 ab 0c" -> 0x12,0x34,0x56,0x78,0xab,0x0c 返回数据长度
u8_t  Swap_numChar(char *buf, u32_t num);    //0x0400 -> "1024" 返回字符长度
u32_t Swap_charNum(char *buf);           //"1024" 返回 0x0400;
u16_t Swap_BCDChar(char *buf, u8_t *bcd, u16_t len);  //0x123456 -> "123456" 返回字符长度
u16_t Swap_CharBCD(u8_t *bcd, char *buf);   //"98765" -> 0x098765
u16_t Swap_numBCD(u8_t *bcd, u64_t num);  //0x0100 -> 0x0256
u64_t Swap_BCDNum(u8_t *bcd, u16_t len);      //0x1024 -> 0x0400
s32_t Swap_stringSigned(char *buf);
u8_t  Swap_signedString(char *buf, s32_t num);
void  Swap_numCharFixed(char *buf, u8_t bufLen, u64_t num);
u8_t  Swap_CharString(char *buf, s8_t num) ;
int r_strsplit(ListHandler_t *subStr , char *str, char ch);

#endif //Swap.h end
/*******************************Copyright QYKJ*********************************/
							

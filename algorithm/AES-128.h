/**************Copyright(C)，2015-2026 QY TEMP  ********************************
  *@brief   : AES 128加密算法头文件
  *@notes   : 2016.11.10 CGQ 创建   
*******************************************************************************/
#ifndef _AES_128_H
#define _AES_128_H
    
#include "typedef.h"

#define     AES_ECB_MODE    
#define     AES_CBC_MODE
#define     AES_CFB_MODE
#define     AES_OFB_MODE

#define AES_TYPE    (128)
#define AES_BLOCK_SIZE  (AES_TYPE/8)        //块大小
#define AES_MATRIX      (AES_TYPE/32)       //矩阵大小  MATRIX * 4
#define AES_ROUND       (AES_MATRIX + 6)



typedef struct
{
    u32_t key[4];
}AESKey_t;

typedef enum
{
#ifdef  AES_ECB_MODE
    AES_ECB,
#endif
#ifdef  AES_CBC_MODE
    AES_CBC,
#endif
#ifdef  AES_CFB_MODE
    AES_CFB,
#endif
#ifdef  AES_OFB_MODE
    AES_OFB,
#endif
}AESMode_e;

typedef struct Cipher
{
    u8_t  extendKey[AES_BLOCK_SIZE*(AES_ROUND + 1)];    
    u8_t  zeroIV[AES_BLOCK_SIZE];
    u8_t *(*encrypt)(struct Cipher *cipher, u8_t *input, int *lenght);
    u8_t *(*decrypt)(struct Cipher *cipher, u8_t *input, int *lenght) ;
}Cipher_t;


void Cipher_init(u8_t *cipherKey, u8_t *zeroIV, AESMode_e mode, Cipher_t *cipher);

#endif	//	_AES_128_H
/******************************************************************************/


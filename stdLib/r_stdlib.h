/***************************Copyright QYKJ   2015-06-18*************************
��	����	r_stdlib.h
˵	����	��дC��׼�⺯��
�ޡ��ģ�	2015.06.20 Unarty establish
*******************************************************************************/
#ifndef __R_STDLIB_H
#define __R_STDLIB_H

#include "typedef.h"
                                
void 	*r_memcpy(void *trg, const void *src, u32_t num); //�ڴ濼��
u32_t  	r_memcmp(void *trg,  const void *src, u32_t num);	//�ڴ�Ƚ�
void 	*r_memmove(void *trg,  const void *src, u32_t num);	//�ڴ��ƶ�
void 	r_memset(void *trg, u8_t const val, u32_t num);	//�ڴ����� 
void 	*r_memfind(void *trg, u32_t trgLen, const void *src, u32_t srcLen);

u32_t 	r_strlen(const char *src);
u32_t 	r_strcpy(char *trg, const char *src);
u32_t 	r_stradd(char *trg, const char *src);
u32_t 	r_strcmp(const char *trg, const char *src);
u32_t 	r_strcmpformat(const char *trg, const char *format);  //**:**:**
int 	r_strfind(const char *trg, const char *src);
void    r_strSub(char *trg, const char *src, char c);


#endif //r_stdlib.h end
/*******************************Copyright QYKJ*********************************/
							

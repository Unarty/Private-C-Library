/***************************Copyright QYKJ   2015-06-18*************************
��	����	r_stdlib.c
˵	����	��дC��׼�⺯��
�ޡ��ģ�	2015.06.20 Unarty establish
            2016.01.29 Unarty ����RT_thread �޸�:memcpy - memmov - memset ����
*******************************************************************************/

#include "r_stdlib.h"

//#define RT_TINY_SIZE
/*******************************************************************************
�� �� ����	r_memcpy
����˵���� 	�ڴ濼��
��	  ���� 	trg:	Ŀ���ڴ��ַ
			src:	Դ�ڴ��ַ
			count:	�����ڴ��С
�� �� ֵ��	Ŀ���ڴ��ַ
*******************************************************************************/
void *r_memcpy(void *trg, const void *src, mcu_t count)
{
#if (mcu_t != u32_t)
    if (trg != src)	//Ŀ����Դ����ͬһλ��
    {
        u8_t *pT = (u8_t*)trg;
        u8_t *pS = (u8_t*)src;
        while (count--)
        {
        	*pT++ = *pS++;
        }
    }
#else
#define UNALIGNED(X, Y)                                 \
                        (((mcu_t)X & (sizeof(mcu_t) - 1))   \
                        |((mcu_t)Y & (sizeof(mcu_t) - 1)))
#define BIGBLOCKSIZE    (sizeof(mcu_t) << 2)
#define LITTLEBLOCKSIZE (sizeof(mcu_t))
#define TOO_SMALL(LEN)  ((LEN) < BIGBLOCKSIZE)

    if (trg != src)    //Ŀ����Դ����ͬһλ��
    {
        u8_t *dst_ptr = (u8_t*)trg;
        u8_t *src_ptr = (u8_t*)src;

        /* If the size is small, or either SRC or DST is unaligned,
        then punt into the byte copy loop.  This should be rare. */
        if (!TOO_SMALL(count) && !UNALIGNED(src_ptr, dst_ptr))
        {
            mcu_t *aligned_dst = (mcu_t *)trg;
            mcu_t *aligned_src = (mcu_t *)src;

            /* Copy 4X long words at a time if possible. */
            while (count >= BIGBLOCKSIZE)
            {
                *aligned_dst++ = *aligned_src++;
                *aligned_dst++ = *aligned_src++;
                *aligned_dst++ = *aligned_src++;
                *aligned_dst++ = *aligned_src++;
                count -= BIGBLOCKSIZE;
            }

            /* Copy one long word at a time if possible. */
            while (count >= LITTLEBLOCKSIZE)
            {
                *aligned_dst++ = *aligned_src++;
                count -= LITTLEBLOCKSIZE;
            }

            /* Pick up any residual with a byte copier. */
            dst_ptr = (u8_t *)aligned_dst;
            src_ptr = (u8_t *)aligned_src;
        }

        while (count--)
        {
            *dst_ptr++ = *src_ptr++;
        }
    }

#undef UNALIGNED
#undef BIGBLOCKSIZE
#undef LITTLEBLOCKSIZE
#undef TOO_SMALL
#endif
    return trg;
}
                                                                                
/*******************************************************************************
�� �� ����	memcmp
����˵���� 	�ڴ�Ƚ�
��	  ���� 	trg:	Ŀ���ڴ��ַ
			src:	Դ�ڴ��ַ
			count:	�Ƚ��ڴ��С
�� �� ֵ��	0(OK)/!0(�����)
*******************************************************************************/
mcu_t r_memcmp(void *trg,  const void *src, mcu_t count)
{
#if (mcu_t != u32_t)
    if (trg != src)	//Ŀ����Դ����ͬһλ��
    {
        u8_t *pT = (u8_t*)trg;
        u8_t *pS = (u8_t*)src;
        while (count--)
        {
        	if (*pT++ != *pS++)
			{
				return 1;
			}
        }
    }
#else
#define UNALIGNED(X, Y)                                 \
                        (((mcu_t)X & (sizeof(mcu_t) - 1))   \
                        |((mcu_t)Y & (sizeof(mcu_t) - 1)))
#define LITTLEBLOCKSIZE (sizeof(mcu_t))
#define TOO_SMALL(LEN)  ((LEN) < LITTLEBLOCKSIZE)

    if (trg != src)    //Ŀ����Դ����ͬһλ��
    {
        u8_t *dst_ptr = (u8_t*)trg;
        u8_t *src_ptr = (u8_t*)src;

        /* If the size is small, or either SRC or DST is unaligned,
        then punt into the byte copy loop.  This should be rare. */
        if (!TOO_SMALL(count) && !UNALIGNED(src_ptr, dst_ptr))
        {
            mcu_t *aligned_dst = (mcu_t *)trg;
            mcu_t *aligned_src = (mcu_t *)src;

            /* Copy one long word at a time if possible. */
            while (count >= LITTLEBLOCKSIZE)
            {
                if (*aligned_dst++ != *aligned_src++)
				{
					return 1;
				}
                count -= LITTLEBLOCKSIZE;
            }

            /* Pick up any residual with a byte copier. */
            dst_ptr = (u8_t *)aligned_dst;
            src_ptr = (u8_t *)aligned_src;
        }

        while (count--)
        {
            if (*dst_ptr++ != *src_ptr++)
			{
				return 1;
			}
        }
    }
#undef UNALIGNED
#undef LITTLEBLOCKSIZE
#undef TOO_SMALL
#endif

	return 0;
}

/*******************************************************************************
�� �� ����	r_memset
����˵���� 	�ڴ�����
��	  ���� 	trg:	Ŀ���ڴ��ַ
			val:	Ŀ��ֵ
			count:	�Ƚ��ڴ��С
�� �� ֵ��	��(�ο�RT_thread 32λ
*******************************************************************************/
void r_memset(void *trg, u8_t const val, mcu_t count)
{
#if (mcu_t != u32_t)
	u8_t *pT = (u8_t*)trg;
	
	while (count--)
	{
		*pT++ = val;
	}
#else
   #define LBLOCKSIZE      (sizeof(mcu_t))
   #define UNALIGNED(X)    ((mcu_t)X & (LBLOCKSIZE - 1))
   #define TOO_SMALL(LEN)  ((LEN) < LBLOCKSIZE) 
     
   u8_t *pT = (u8_t*)trg;    
   
   if (!TOO_SMALL(count) && !UNALIGNED(trg))    
   { 
        mcu_t *aligned_addr = (mcu_t*)trg;        
        mcu_t buffer = (val<<24)|(val<<16)|(val<<8)|(val<<0);
        
        while (count >= LBLOCKSIZE * 4)       
        {            
            *aligned_addr++ = buffer;           
            *aligned_addr++ = buffer;            
            *aligned_addr++ = buffer;            
            *aligned_addr++ = buffer;            
            count -= 4 * LBLOCKSIZE;        
        }        
        while (count >= LBLOCKSIZE)        
        {            
            *aligned_addr++ = buffer;            
            count -= LBLOCKSIZE;        
        }
        /* Pick up the remainder with a bytewise loop. */        
        pT = (u8_t *)aligned_addr;
   }
   
    while (count--)    
    {        
        *pT++ = val;    
    }  
    #undef LBLOCKSIZE
    #undef UNALIGNED
    #undef TOO_SMALL
#endif
}

/*******************************************************************************
�� �� ����	r_memmove
����˵���� 	�ڴ��ƶ�
��	  ���� 	trg:	Ŀ���ڴ��ַ
			src:	Դ�ڴ��ַ
			count:	�ƶ��ڴ��С
�� �� ֵ��	Ŀ���ڴ��ַ
*******************************************************************************/
void *r_memmove(void *trg,  const void *src, mcu_t count)
{
	if (trg < src)
	{
		r_memcpy(trg, src, count);
	}
	else if (trg > src)
	{ 
        u8_t *pT = (u8_t*)trg + count;
        u8_t *pS = (u8_t*)src + count;
#if (mcu_t != u32_t)
        while (count--)
        {
            *--pT = *--pS;
        } 
#else
#define UNALIGNED(X, Y)                                 \
                                (((mcu_t)X & (sizeof(mcu_t) - 1))   \
                                |((mcu_t)Y & (sizeof(mcu_t) - 1)))
#define BIGBLOCKSIZE    (sizeof(mcu_t) << 2)
#define LITTLEBLOCKSIZE (sizeof(mcu_t))
#define TOO_SMALL(LEN)  ((LEN) < BIGBLOCKSIZE)
        /* If the size is small, or either SRC or DST is unaligned,
        then punt into the byte copy loop.  This should be rare. */
        if (!TOO_SMALL(count) && !UNALIGNED(pT, pS))
        {
            mcu_t *aligned_trg = (mcu_t *)pT;
            mcu_t *aligned_src = (mcu_t *)pS;

            /* Copy 4X long words at a time if possible. */
            while (count >= BIGBLOCKSIZE)
            {
                *--aligned_trg = *--aligned_src;
                *--aligned_trg = *--aligned_src;
                *--aligned_trg = *--aligned_src;
                *--aligned_trg = *--aligned_src;
                count -= BIGBLOCKSIZE;
            }

            /* Copy one long word at a time if possible. */
            while (count >= LITTLEBLOCKSIZE)
            {
                *--aligned_trg = *--aligned_src;
                count -= LITTLEBLOCKSIZE;
            }

            /* Pick up any residual with a byte copier. */
            pT = (u8_t*)aligned_trg;
            pS = (u8_t*)aligned_src;
        }

        while (count--)
        {
            *--pT = *--pS;
        }
#undef UNALIGNED
#undef BIGBLOCKSIZE
#undef LITTLEBLOCKSIZE
#undef TOO_SMALL
#endif
	}

    return trg;
}

/*******************************************************************************
�� �� ����	r_memfind
����˵���� 	�ڴ����
��	  ���� 	trg:	Ŀ������ڴ��ַ
			trgLen:	Ŀ���С
			src:	Դ�ڴ��ַ
			srcLen:	�ƶ��ڴ��С
�� �� ֵ��	null(δ�ҵ�)/add(Ŀ����ʼ��ַ)
*******************************************************************************/
void *r_memfind(void *trg, mcu_t trgLen, const void *src, mcu_t srcLen)
{
	mcu_t i = 0;
	u8_t *pT = (u8_t*)trg;
	u8_t *pS = (u8_t*)src;
	
	while (srcLen-- > 0)
	{
		if (*pS++ == pT[i])	//�����������
		{
			if (++i == trgLen)	//ƥ�䳤�����
			{
				return (pS - i);
			}
		}
		else
		{
			i = 0;
		}
	}

	return null;
}

/*******************************************************************************
�� �� ����	r_strlen
����˵���� 	����Ŀ���ַ�������
��	  ���� 	src:	Ŀ���ַ���
�� �� ֵ��	�ַ������ȡ���0���쳣)
*******************************************************************************/
mcu_t r_strlen(const char *trg)
{
	mcu_t i = 0;
	
	while ('\0' != *trg++)
	{
		i++;
	}
	
	return i;
}

/*******************************************************************************
�� �� ����	r_strcpy
����˵���� 	�ַ�������
��	  ���� 	trg:    Ŀ���ַ���
            src:	Դ�ַ���
�� �� ֵ��	�ַ������ȡ���0���쳣)
*******************************************************************************/
mcu_t r_strcpy(char *trg, const char *src)
{
    mcu_t i = 0;
    
    ERRR(null == trg || null == src, return 0);

    while ('\0' != (*trg = *src++))
    {
        trg++;
        i++;
    }
    
    return i;
}

/*******************************************************************************
�� �� ����	r_stradd
����˵���� 	�ַ������
��	  ���� 	trg:    Ŀ���ַ���
            src:	Դ�ַ���
�� �� ֵ��	�ַ������ȡ���0���쳣)
*******************************************************************************/
mcu_t r_stradd(char *trg, const char *src)
{
    mcu_t i = 0;
    
    ERRR(null == trg || null == src, return 0);

    i = r_strlen(trg);
    trg += i;

    while ('\0' != *src)
    {
        *trg++ = *src++;
        i++;
    }
    *trg = '\0';
    
    return i;
}

/*******************************************************************************
�� �� ����	r_strcmp
����˵���� 	�ַ����Ƚ�
��	  ���� 	trg:    Ŀ���ַ���
            src:	Դ�ַ���
�� �� ֵ��	0(OK)/!0(�����)
*******************************************************************************/
mcu_t r_strcmp(const char *trg, const char *src)
{
    
    ERRR(null == trg || null == src, return 0);

    while ('\0' != *trg && '\0' != *src)
    {
        if (*trg++ != *src++)
        {
            return 1;
        }
        
    }
    
    return 0;
}

/*******************************************************************************
�� �� ����	r_strcmpformat
����˵���� 	�ַ�����ʽ�Ƚ�  *: �����Ƚ�
��	  ���� 	trg:    Ŀ���ַ���
            src:	Դ�ַ���
�� �� ֵ��	0(OK)/!0(�����)
*******************************************************************************/
mcu_t r_strcmpformat(const char *trg, const char *format)
{
    
    ERRR(null == trg || null == format, return 0);

    while ('\0' != *trg && '\0' != *format)
    {
        if ((*format != '*') 
            && (*trg != *format)
            )
        {
            return 1;
        }
        
        trg++;
        format++;
    }
    
    return 0;
}



/*******************************************************************************
�� �� ����	r_strfind
����˵���� 	�ַ�������
��	  ���� 	trg:    Ŀ���ַ���
            src:	Դ�ַ���
�� �� ֵ��	Ŀ���ַ���λ��Դ�ַ�����λλ�ã�0���쳣)
*******************************************************************************/
int r_strfind(const char *trg, const char *src)
{
    int i;
    const char *p;

    ERRR(null == trg || null == src, return 0);

    for ( i = 0, p = trg; '\0' != *src && '\0' != *p; src++, i++)
    {
        if (*p == *src)
        {
            p++;
        }
        else
        {
            p = trg;
        }
    }

    return (('\0' == *p) ? (i - r_strlen(trg)) : (0xFFFFFFFF));
}

/*******************************************************************************
  * @brief  
  * @note   None
  * @param  None
  * @retval None
*******************************************************************************/
void r_strSub(char *trg, const char *src, char c)
{
    u8_t flag = 0;
    
    while (*src++ != '\0')
    {
        if (*src == c)
        {
            if (0 == flag)
            {
                flag = 1;
            }
            else
            {
                *trg++ = *src;
                break;
            }
        }
        if (0 != flag)
        {
            *trg++ = *src;
        }
    }
    
    *trg = '\0';
}

/*******************************Copyright QYKJ*********************************/


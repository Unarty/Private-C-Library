﻿/***************************Copyright QYKJ   2015-06-18*************************
文	件：	r_stdlib.c
说	明：	重写C标准库函数
修　改：	2015.06.20 Unarty establish
            2016.01.29 Unarty 依据RT_thread 修改:memcpy - memmov - memset 函数
*******************************************************************************/

#include "r_stdlib.h"

//#define RT_TINY_SIZE
/*******************************************************************************
函 数 名：	r_memcpy
功能说明： 	内存考备
参	  数： 	trg:	目标内存地址
			src:	源内存地址
			count:	考备内存大小
返 回 值：	目标内存地址
*******************************************************************************/
void *r_memcpy(void *trg, const void *src, mcu_t count)
{
#if (mcu_t != u32_t)
    if (trg != src)	//目标与源不是同一位置
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

    if (trg != src)    //目标与源不是同一位置
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
函 数 名：	memcmp
功能说明： 	内存比较
参	  数： 	trg:	目标内存地址
			src:	源内存地址
			count:	比较内存大小
返 回 值：	0(OK)/!0(不相等)
*******************************************************************************/
mcu_t r_memcmp(void *trg,  const void *src, mcu_t count)
{
#if (mcu_t != u32_t)
    if (trg != src)	//目标与源不是同一位置
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

    if (trg != src)    //目标与源不是同一位置
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
函 数 名：	r_memset
功能说明： 	内存设置
参	  数： 	trg:	目标内存地址
			val:	目标值
			count:	比较内存大小
返 回 值：	无(参考RT_thread 32位
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
函 数 名：	r_memmove
功能说明： 	内存移动
参	  数： 	trg:	目标内存地址
			src:	源内存地址
			count:	移动内存大小
返 回 值：	目标内存地址
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
函 数 名：	r_memfind
功能说明： 	内存查找
参	  数： 	trg:	目标查找内存地址
			trgLen:	目标大小
			src:	源内存地址
			srcLen:	移动内存大小
返 回 值：	null(未找到)/add(目标起始地址)
*******************************************************************************/
void *r_memfind(void *trg, mcu_t trgLen, const void *src, mcu_t srcLen)
{
	mcu_t i = 0;
	u8_t *pT = (u8_t*)trg;
	u8_t *pS = (u8_t*)src;
	
	while (srcLen-- > 0)
	{
		if (*pS++ == pT[i])	//数据内容相等
		{
			if (++i == trgLen)	//匹配长度相等
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
函 数 名：	r_strlen
功能说明： 	计算目标字符串长度
参	  数： 	src:	目标字符串
返 回 值：	字符串长度。（0：异常)
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
函 数 名：	r_strcpy
功能说明： 	字符串复制
参	  数： 	trg:    目标字符串
            src:	源字符串
返 回 值：	字符串长度。（0：异常)
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
函 数 名：	r_stradd
功能说明： 	字符串相加
参	  数： 	trg:    目标字符串
            src:	源字符串
返 回 值：	字符串长度。（0：异常)
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
函 数 名：	r_strcmp
功能说明： 	字符串比较
参	  数： 	trg:    目标字符串
            src:	源字符串
返 回 值：	0(OK)/!0(不相等)
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
函 数 名：	r_strcmpformat
功能说明： 	字符串格式比较  *: 不做比较
参	  数： 	trg:    目标字符串
            src:	源字符串
返 回 值：	0(OK)/!0(不相等)
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
函 数 名：	r_strfind
功能说明： 	字符串查找
参	  数： 	trg:    目标字符串
            src:	源字符串
返 回 值：	目标字符串位于源字符串的位位置（0：异常)
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


/***************************Copyright QiYi Team 2016.05*************************
文	件：    HashMD5.c
说	明：    MD5加密算法原码
修　改：	2016.05.19 创建 Unarty
*******************************************************************************/
#include "HashMD5.h"
#include "stdlib\r_stdlib.h"

const u32_t md5InitVal[] = {0x67452301, 0xefcdab89, 0x98badcfeU, 0x10325476};
const u8_t padding[64] = {
  0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

static void Transform (u32_t *buf, u32_t *in);


/* F, G, H and I are basic MD5 functions */
#define F(x, y, z) (((x) & (y)) | ((~x) & (z)))
#define G(x, y, z) (((x) & (z)) | ((y) & (~z)))
#define H(x, y, z) ((x) ^ (y) ^ (z))
#define I(x, y, z) ((y) ^ ((x) | (~z)))

/* ROTATE_LEFT rotates x left n bits */
#define ROTATE_LEFT(x, n) (((x) << (n)) | ((x) >> (32-(n))))

/* FF, GG, HH, and II transformations for rounds 1, 2, 3, and 4 */
/* Rotation is separate from addition to prevent recomputation */
#define FF(a, b, c, d, x, s, ac) {\
    (a) += F ((b), (c), (d)) + (x) + (u32_t)(ac); \
    (a) = ROTATE_LEFT ((a), (s)); \
    (a) += (b); \
}
#define GG(a, b, c, d, x, s, ac) {\
    (a) += G ((b), (c), (d)) + (x) + (u32_t)(ac); \
    (a) = ROTATE_LEFT ((a), (s)); \
    (a) += (b); \
}
#define HH(a, b, c, d, x, s, ac) {\
    (a) += H ((b), (c), (d)) + (x) + (u32_t)(ac); \
    (a) = ROTATE_LEFT ((a), (s)); \
    (a) += (b); \
}
#define II(a, b, c, d, x, s, ac) { \
    (a) += I ((b), (c), (d)) + (x) + (u32_t)(ac); \
    (a) = ROTATE_LEFT ((a), (s)); \
    (a) += (b); \
}

#ifdef __STDC__
#define UL(x) x##UL
#else
#ifdef WIN32
#define UL(x) x##UL
#else
#define UL(x) x
#endif
#endif

void Hash_MD5Init (MD5_t *mdContext)
{
    mdContext->len = 0;

    /* Load magic initialization constants. */
   	r_memcpy(mdContext->md, md5InitVal, sizeof(md5InitVal));
}

void Hash_MD5Update(MD5_t *mdContext, const u8_t *inBuf, mcu_t inLen)
{
	u8_t len;
	u8_t insert = mdContext->len&0x3F;
    
    mdContext->len += inLen;
    while (inLen) 
    {
        len = (0x40 - insert) < inLen ? (0x40 - insert) : inLen;
        r_memcpy((((u8_t*)mdContext->buf) + insert), inBuf, len);
		inBuf += len;
		inLen -= len;
		if (0x40 == (len + insert))
		{
	        insert = 0;
	        Transform (mdContext->md, mdContext->buf);
		}
    }
}

/* The routine MD5Final terminates the message-digest computation and
   ends with the desired message digest in mdContext->digest[0...15].
 */
void Hash_MD5Final(MD5_t *mdContext)
{
    u8_t i = mdContext->len&0x3F;

    i = (i < 56) ? (56 - i) : (120 - i);
    Hash_MD5Update(mdContext, padding, i);
    
    mdContext->len -= i;
    mdContext->buf[14] = ((mdContext->len) << 3);
    mdContext->buf[15] = ((mdContext->len) >> 29);
    
    Transform(mdContext->md, mdContext->buf);
}


void Hash_Md5Make(u8_t val[], u8_t *buffer, u32_t len)
{
    u32_t *md = (u32_t*)val;
    u32_t in[16];
    u32_t bufLen = len;
    
    /* Load magic initialization constants. */
   	r_memcpy(md, md5InitVal, sizeof(md5InitVal));

    while (bufLen > 0x40)
    {
        Transform (md, (u32_t*)buffer);
        bufLen -= 0x40;
        buffer += 0x40;
    }
    
    r_memcpy(in, buffer, bufLen);
    r_memcpy(((u8_t*)in) + bufLen, padding, 0x40 - bufLen);
    if (bufLen > 56)
    {
        bufLen = 0;
        Transform (md, in);
        r_memset(in, 0, (sizeof(in) - 8));
    }
    in[14] = len<<3;
    in[15] = len>>29;
    Transform (md, in);
}

/*校验块数值转换*/
static void Transform (u32_t *buf, u32_t *in)
{
  u32_t a = buf[0], b = buf[1], c = buf[2], d = buf[3];

  /* Round 1 */
#define S11 7
#define S12 12
#define S13 17
#define S14 22
  FF ( a, b, c, d, in[ 0], S11, UL(3614090360)); /* 1 */
  FF ( d, a, b, c, in[ 1], S12, UL(3905402710)); /* 2 */
  FF ( c, d, a, b, in[ 2], S13, UL( 606105819)); /* 3 */
  FF ( b, c, d, a, in[ 3], S14, UL(3250441966)); /* 4 */
  FF ( a, b, c, d, in[ 4], S11, UL(4118548399)); /* 5 */
  FF ( d, a, b, c, in[ 5], S12, UL(1200080426)); /* 6 */
  FF ( c, d, a, b, in[ 6], S13, UL(2821735955)); /* 7 */
  FF ( b, c, d, a, in[ 7], S14, UL(4249261313)); /* 8 */
  FF ( a, b, c, d, in[ 8], S11, UL(1770035416)); /* 9 */
  FF ( d, a, b, c, in[ 9], S12, UL(2336552879)); /* 10 */
  FF ( c, d, a, b, in[10], S13, UL(4294925233)); /* 11 */
  FF ( b, c, d, a, in[11], S14, UL(2304563134)); /* 12 */
  FF ( a, b, c, d, in[12], S11, UL(1804603682)); /* 13 */
  FF ( d, a, b, c, in[13], S12, UL(4254626195)); /* 14 */
  FF ( c, d, a, b, in[14], S13, UL(2792965006)); /* 15 */
  FF ( b, c, d, a, in[15], S14, UL(1236535329)); /* 16 */

  /* Round 2 */
#define S21 5
#define S22 9
#define S23 14
#define S24 20
  GG ( a, b, c, d, in[ 1], S21, UL(4129170786)); /* 17 */
  GG ( d, a, b, c, in[ 6], S22, UL(3225465664)); /* 18 */
  GG ( c, d, a, b, in[11], S23, UL( 643717713)); /* 19 */
  GG ( b, c, d, a, in[ 0], S24, UL(3921069994)); /* 20 */
  GG ( a, b, c, d, in[ 5], S21, UL(3593408605)); /* 21 */
  GG ( d, a, b, c, in[10], S22, UL(  38016083)); /* 22 */
  GG ( c, d, a, b, in[15], S23, UL(3634488961)); /* 23 */
  GG ( b, c, d, a, in[ 4], S24, UL(3889429448)); /* 24 */
  GG ( a, b, c, d, in[ 9], S21, UL( 568446438)); /* 25 */
  GG ( d, a, b, c, in[14], S22, UL(3275163606)); /* 26 */
  GG ( c, d, a, b, in[ 3], S23, UL(4107603335)); /* 27 */
  GG ( b, c, d, a, in[ 8], S24, UL(1163531501)); /* 28 */
  GG ( a, b, c, d, in[13], S21, UL(2850285829)); /* 29 */
  GG ( d, a, b, c, in[ 2], S22, UL(4243563512)); /* 30 */
  GG ( c, d, a, b, in[ 7], S23, UL(1735328473)); /* 31 */
  GG ( b, c, d, a, in[12], S24, UL(2368359562)); /* 32 */

  /* Round 3 */
#define S31 4
#define S32 11
#define S33 16
#define S34 23
  HH ( a, b, c, d, in[ 5], S31, UL(4294588738)); /* 33 */
  HH ( d, a, b, c, in[ 8], S32, UL(2272392833)); /* 34 */
  HH ( c, d, a, b, in[11], S33, UL(1839030562)); /* 35 */
  HH ( b, c, d, a, in[14], S34, UL(4259657740)); /* 36 */
  HH ( a, b, c, d, in[ 1], S31, UL(2763975236)); /* 37 */
  HH ( d, a, b, c, in[ 4], S32, UL(1272893353)); /* 38 */
  HH ( c, d, a, b, in[ 7], S33, UL(4139469664)); /* 39 */
  HH ( b, c, d, a, in[10], S34, UL(3200236656)); /* 40 */
  HH ( a, b, c, d, in[13], S31, UL( 681279174)); /* 41 */
  HH ( d, a, b, c, in[ 0], S32, UL(3936430074)); /* 42 */
  HH ( c, d, a, b, in[ 3], S33, UL(3572445317)); /* 43 */
  HH ( b, c, d, a, in[ 6], S34, UL(  76029189)); /* 44 */
  HH ( a, b, c, d, in[ 9], S31, UL(3654602809)); /* 45 */
  HH ( d, a, b, c, in[12], S32, UL(3873151461)); /* 46 */
  HH ( c, d, a, b, in[15], S33, UL( 530742520)); /* 47 */
  HH ( b, c, d, a, in[ 2], S34, UL(3299628645)); /* 48 */

  /* Round 4 */
#define S41 6
#define S42 10
#define S43 15
#define S44 21
  II ( a, b, c, d, in[ 0], S41, UL(4096336452)); /* 49 */
  II ( d, a, b, c, in[ 7], S42, UL(1126891415)); /* 50 */
  II ( c, d, a, b, in[14], S43, UL(2878612391)); /* 51 */
  II ( b, c, d, a, in[ 5], S44, UL(4237533241)); /* 52 */
  II ( a, b, c, d, in[12], S41, UL(1700485571)); /* 53 */
  II ( d, a, b, c, in[ 3], S42, UL(2399980690)); /* 54 */
  II ( c, d, a, b, in[10], S43, UL(4293915773)); /* 55 */
  II ( b, c, d, a, in[ 1], S44, UL(2240044497)); /* 56 */
  II ( a, b, c, d, in[ 8], S41, UL(1873313359)); /* 57 */
  II ( d, a, b, c, in[15], S42, UL(4264355552)); /* 58 */
  II ( c, d, a, b, in[ 6], S43, UL(2734768916)); /* 59 */
  II ( b, c, d, a, in[13], S44, UL(1309151649)); /* 60 */
  II ( a, b, c, d, in[ 4], S41, UL(4149444226)); /* 61 */
  II ( d, a, b, c, in[11], S42, UL(3174756917)); /* 62 */
  II ( c, d, a, b, in[ 2], S43, UL( 718787259)); /* 63 */
  II ( b, c, d, a, in[ 9], S44, UL(3951481745)); /* 64 */

  buf[0] += a;
  buf[1] += b;
  buf[2] += c;
  buf[3] += d;
}

/***********************************FILE END***********************************/

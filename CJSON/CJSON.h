/**************Copyright(C)，2015-2026 QY TEMP  ********************************
  *@brief   : AES 加密算法头文件
  *@notes   : 2016.11.10 CGQ 创建   
*******************************************************************************/
#ifndef __CJSON_H
#define __CJSON_H

#include "typedef.h"
#include "List\List.h"

typedef int (*JSONTrans)(char *data, void *dest, int size);

typedef enum
{
	CHAR		= 0,	//字符串
	SIGNED 		= 1, //有符号数
	UNSIGNED 	= 2, //无符号数
}MemberType_e;	//结构体成员类型枚举

typedef struct
{
	int  offset;		//相对偏移位置
	int  size;			//成员占用空间大小 
	JSONTrans create; //创建
	JSONTrans parse;
	char name[1];		//成员名
}StructMember_t;	//成员描述 

typedef struct
{
	char *name;
	char *para;
}CJSONMember_t;  // 


void CJSON_itemAdd(ListHandler_t *jsonMember, char *name, int offset, MemberType_e type, int size);
int CJSON_create(ListHandler_t *jsonMember, void *pStruct, char *jsonString);
int CJSON_parse(ListHandler_t *jsonMember, void *pStruct, char *jsonString);



#endif	//	__CJSON_H
/******************************************************************************/


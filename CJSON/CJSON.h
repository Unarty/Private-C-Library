/**************Copyright(C)��2015-2026 QY TEMP  ********************************
  *@brief   : AES �����㷨ͷ�ļ�
  *@notes   : 2016.11.10 CGQ ����   
*******************************************************************************/
#ifndef __CJSON_H
#define __CJSON_H

#include "typedef.h"
#include "List\List.h"

typedef int (*JSONTrans)(char *data, void *dest, int size);

typedef enum
{
	CHAR		= 0,	//�ַ���
	SIGNED 		= 1, //�з�����
	UNSIGNED 	= 2, //�޷�����
}MemberType_e;	//�ṹ���Ա����ö��

typedef struct
{
	int  offset;		//���ƫ��λ��
	int  size;			//��Առ�ÿռ��С 
	JSONTrans create; //����
	JSONTrans parse;
	char name[1];		//��Ա��
}StructMember_t;	//��Ա���� 

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


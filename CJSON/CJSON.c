/**************Copyright(C)��2015-2026��QY Temp, Ltd.***************
  *@brief   : JSON ����/����
  *@notes   : 2017.02.27 CGQ ����   
*******************************************************************************/
#include "CJSON.h"
#include "stdlib/r_stdlib.h"
#include "swap/swap.h"

static int CJSON_member(ListHandler_t *JSONMember, char *jsonString);
static u8_t CJSON_nameCmp(void *dest, void *itemName);
static int char_creat(char *str, void *dest, int size);
static int char_parse(char *str, void *dest, int size);
static int signed_creat(char *str, void *dest, int size);
static int signed_parse(char *str, void *dest, int size);
static int unsigned_creat(char *str, void *dest, int size);
static int unsigned_parse(char *str, void *dest, int size);


static JSONTrans creatTab[] = {
	char_creat,
	signed_creat,
	unsigned_creat
};

static JSONTrans parseTab[] = {
	char_parse,
	signed_parse,
	unsigned_parse
};

/*******************************************************************************
  * @brief  �ַ�����
  * @note   None
  * @param  
  * @retval ת�����ַ�������
*******************************************************************************/
static int char_creat(char *str, void *dest, int size)
{
	return r_strcpy(str, dest);
}

/*******************************************************************************
  * @brief  �ַ�ת��
  * @note   None
  * @param  
  * @retval ����json�ַ�������
*******************************************************************************/
static int char_parse(char *str, void *dest, int size)
{
	return r_strcpy(dest, str);
}


/*******************************************************************************
  * @brief  �з���������
  * @note   None
  * @param  
  * @retval ת�����ַ�������
*******************************************************************************/
static int signed_creat(char *str, void *dest, int size)
{
	int val;
	u8_t *p = (u8_t*)dest;
	
	val = (0x80 == (p[size-1]&0x80)) ? -1 : 0;
	r_memcpy((void*)&val, dest, size);
	return Swap_signedString(str, val);
}

/*******************************************************************************
  * @brief  �з�����ת��
  * @note   None
  * @param  
  * @retval ����json�ַ�������
*******************************************************************************/
static int signed_parse(char *str, void *dest, int size)
{
	int val;
	
	val = Swap_stringSigned(str);
	r_memcpy(dest, &val, size);

	return val;
}

/*******************************************************************************
  * @brief  �޷���������
  * @note   None
  * @param  
  * @retval ת�����ַ�������
*******************************************************************************/
static int unsigned_creat(char *str, void *dest, int size)
{
	mcu_t val = 0;
	
	r_memcpy(&val, dest, size);
	return Swap_numChar(str, val);
}

/*******************************************************************************
  * @brief  �޷�����ת��
  * @note   None
  * @param  
  * @retval ����json�ַ�������
*******************************************************************************/
static int unsigned_parse(char *str, void *dest, int size)
{
	mcu_t val;
	
	val = Swap_charNum(str);
	r_memcpy(dest, &val, size);

	return val;
}

/*******************************************************************************
  * @brief  ��Ա�ṹ�����
  * @note   None
  * @param  jsonMember: 	�ṹ���Ա��������ͷ
  			name:			��Ա��
  			offset: 		��Ա���ƫ�Ƶ�ַ
  			type:			��Ա����
  			size:			ռ�ÿռ��С 
  * @retval ����json�ַ�������
*******************************************************************************/
void CJSON_itemAdd(ListHandler_t *jsonMember, char *name, int offset, MemberType_e type, int size)
{
	int len;
	Node_t *node;
	StructMember_t *member;

	len = r_strlen(name);
	node = list_nodeApply(jsonMember, sizeof(StructMember_t)+len);

	member = (StructMember_t*)node->data;
	r_strcpy(member->name, name);
	member->offset = offset;
	member->size = size;
	member->create = creatTab[type];
	member->parse = parseTab[type];
}

/*******************************************************************************
  * @brief  ����JSON���ݰ�
  * @note   None
  * @param  item: 	�ṹ��������
  			struct: Ŀ��ṹ������
  			jsonString: �����洢��ַ
  * @retval ����json�ַ�������
*******************************************************************************/
int CJSON_create(ListHandler_t *jsonMember, void *pStruct, char *jsonString)
{
	StructMember_t *member;
	Node_t *node = jsonMember->node;
	char *p = jsonString;
	u8_t *pData = (u8_t*)pStruct;
	
	*p++ = '{';
	
	do {
		member = (StructMember_t *)node->data;
		*p++ = '\"';
		p += r_strcpy(p, member->name);
		p += r_stradd(p, "\":\"");
		p += member->create(p, &pData[member->offset], member->size);
		p += r_stradd(p, "\",");
		node = node->next;
	}while (node != jsonMember->node);
	
	*--p = '}';
	*++p = '\0';
	
    return ((int)p - (int)jsonString);
}

/*******************************************************************************
  * @brief  ����JSON���ݰ�
  * @note   None
  * @param  item: 	�ṹ��������
  			struct: Ŀ��ṹ������
  			jsonString: �����洢��ַ
  * @retval ����ת�����
*******************************************************************************/
int CJSON_parse(ListHandler_t *jsonMember, void *pStruct, char *jsonString)
{
	ListHandler_t JSONItem;
	int i;

	i = CJSON_member(&JSONItem, jsonString);
	
	if (i > 0)
	{
		StructMember_t *pMember;
        CJSONMember_t  *item;
		Node_t *node;
		Node_t *memberNode = jsonMember->node;
		u8_t *data = (u8_t*)pStruct;
		
		do {
			pMember = (StructMember_t *)memberNode->data;
			node = list_find(&JSONItem, CJSON_nameCmp, pMember->name);
			if (node == null)
			{
				i = -2;
				break;
			}
			item = (CJSONMember_t*)(node->data);
			pMember->parse(item->para, &data[pMember->offset], pMember->size);
			list_nodeDelete(&JSONItem, node);
			memberNode = memberNode->next;
		}while (jsonMember->node != null);
	}
	list_delete(&JSONItem);
	
	return i;
}



/*******************************************************************************
  * @brief  ��ȡJson��Ա
  * @note   None
  * @param  item: 	�ṹ��������
  			struct: Ŀ��ṹ������
  			jsonString: �����洢��ַ
  * @retval ����ת����� >0 �ṹ���Ա���� 0: JSON��ʽ���� -1 : �ڴ�����ʧ��
*******************************************************************************/
static int CJSON_member(ListHandler_t *JSONMember, char *jsonString)
{
	CJSONMember_t *pMember = null;
	char *p = jsonString;
    int i = 0;

	list_init(JSONMember);

	while (*jsonString != '\0')
	{
		if (*jsonString++ == '\"')
		{
			if (*jsonString == ':')
			{
				if (pMember == null)	
				{
					Node_t *node = list_nodeApply(JSONMember, sizeof(CJSONMember_t));
					if (node == null)
					{
						return -1;
					}
		            pMember = (CJSONMember_t *)node->data;
				}
				
                *jsonString++ = '\0';
				pMember->name = p;
				p = jsonString;
			}
			else if (*jsonString == ',' || *jsonString == '}')
			{
                *jsonString++ = '\0';
				if (pMember != null && pMember->name != null)
				{
                    i++;
					pMember->para = p;
					pMember = null;
					p = jsonString;
				}
				else
				{
					return 0;
				}
			}
		}
	}
    
    return i;
}


/*******************************************************************************
  * @brief  �ṹ���Ա���Ƚ�
  * @note   None
  * @param  dest: 	Ŀ��
  			itemName: �����
  * @retval ����ת����� >0 �ṹ���Ա���� 0: JSON��ʽ���� -1 : �ڴ�����ʧ��
*******************************************************************************/
static u8_t CJSON_nameCmp(void *dest, void *itemName)
{
	CJSONMember_t *pMember = (CJSONMember_t *)dest;
	char *p = (char*)itemName;

	return r_strcmp(pMember->name, p);
}

/******************************************************************************/


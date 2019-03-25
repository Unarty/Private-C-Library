/********************************Copyright I&S Team*****************************
 * �� �ļ�����	list.c
 * �� ����	��	�������ԭ�ļ�
 * �� �����ߣ�	UnartyChen
 * �� �汾	��	V1.0
 * �� ����	��	2015.03.1
 				2017.12.05 CGQ �޸���ؿ����ͷ
 * �� �޸�	��	����
 * ****************************************************************************/
#include "list.h"
#include "memory.h"
#include "Debug.h"


#define LIST_APPLY_FLAG			(0x55aa33cc)



static Node_t *nodeCheck(u8_t *pLoad);
static Node_t *nodeFind(ListHandler_t *head, u8_t *pLoad);

/*******************************************************************************
 * �� ��������	list_init
 * �� ����	��	�����ʼ������
 * �� ����	��	head:	Ŀ������ͷָ��
 * �� ����	��	��
 * ****************************************************************************/
void list_init(ListHandler_t *head)
{
    head->count = 0;
	head->node = NULL;
}

/*******************************************************************************
 * �� ��������	list_nodeApply 
 * �� ����	��	�ڵ�����
 * �� ����	��	size:       �ڵ�������ݴ�С
 * �� ����	��	����ڵ㸺�ص�ַ
 * ****************************************************************************/
void *list_nodeApply(mcu_t payloadSize)
{
    Node_t *node;

	node = memory_apply(payloadSize + sizeof(Node_t));
	ERRR(node == NULL, goto ERR1);
	
	/*���ݿռ�����ɹ�*/
	node->front = (Node_t *)(LIST_APPLY_FLAG^(int)node->payload);
	
    return node->payload;
ERR1:
    return null;
}

/*******************************************************************************
 * �� ��������	list_topInsert
 * �� ����	��	����ڵ�ǰ��
 * �� ����	��	head:		Ŀ���������ͷָ��
				payload:	Ŀ���������
 * �� ����	��	��������0: �ɹ�  or Fail
 * ****************************************************************************/
int list_topInsert(ListHandler_t *head, void *payload)
{
	int i = list_bottomInsert(head, payload);

	if (i == 0)
	{
		head->node = head->node->front;
	}

	return 0;
}


/*******************************************************************************
 * �� ��������	list_bottomInsert
 * �� ����	��	����ڵ�ǰ��
 * �� ����	��	head:		Ŀ���������ͷָ��
				payload:	Ŀ���������
 * �� ����	��	��������0: �ɹ�  or Fail
 * ****************************************************************************/
int list_bottomInsert(ListHandler_t *head, void *payload)
{
	Node_t *node;
	
	if (head == null)
	{
		return 1;
	}
	
	node = nodeCheck((u8_t*)payload);
	if (node == null)
	{
	    node = nodeFind(head, payload);
        if (node == null)
        {
            return 2;
        }
        else
        {
            node->front->next = node->next;
            node->next->front = node->front;
            head->count--;
        }
	}
	
	if (head->node == NULL)
	{
		head->node = node;
		node->front = node;
		node->next = node;
	}
	else
	{
        Node_t *tail = head->node;
        
		node->front = tail->front;
		node->next = head->node;
		tail->front->next = node;
		tail->front = node;
	}
    head->count++;
	
    return 0;
}


/*******************************************************************************
 * �� ��������	list_ConfInsertNodeApply
 * �� ����	��	������������
 * �� ����	��	head:		Ŀ���������ͷָ��
                cmp:	    ���ݱȽϺ��� 
				data:		�ڵ��ڴ�ռ��
			    size:        ���ݳ���
 * �� ����	��	�ڵ����ݴ洢�׵�ַ.	null������ʧ��
 * ****************************************************************************/
int list_ConfInsert(ListHandler_t *head, cmpFun cmp, void *payload)
{
	Node_t *node, *tail;

	if (head == null)
	{
		return 1;
	}
	
	node = nodeCheck((u8_t*)payload);
	if (node == null)
	{
        node = nodeFind(head, payload);
        if (node == null)
        {
            return 2;
        }
        else
        {
            node->front->next = node->next;
            node->next->front = node->front;
            head->count--;
        }
	}
	/*���ݿռ�����ɹ�*/
	if (head->node == NULL)
	{
		head->node = node;
		node->front = node;
		node->next = node;
	}
	else
	{
        tail = head->node;
        do{
            if (0 == cmp(tail->payload, payload))
            {
                if (tail == head->node) //ͷ�ڵ�ǰ��
                {
                    head->node = node;
                }
                break;
            }
            tail = tail->next;
        } while (tail != head->node);
        
        node->front = tail->front;
        node->next = tail;
        tail->front->next = node;
        tail->front = node;
	}
	head->count++;
    
	return 0;
}

/*******************************************************************************
 * �� ��������	list_nextNode
 * �� ����	��	��ȡ��ǰ����һ���ڵ�����
 * �� ����	��	 
 * �� ����	��	NULL(δ���ҵ�)/�ڵ��ַ
 * ****************************************************************************/
void *list_nextData(ListHandler_t *head, void *payload)
{
    Node_t *node;
    
    node = nodeFind(head, payload);
    
    if (node == null)
    {
        if (head != null && head->node != NULL)
        {
            return head->node->payload;
        }
    }
	else if ( node->next != head->node)
    {
       return node->next->payload;
    }
    
    return null;
}

/*******************************************************************************
 * �� ��������	list_find
 * �� ����	��	����ڵ����
 * �� ����	��	cmp:	���ݱȽϺ��� 
 *				data:	Ŀ��˶�����
 * �� ����	��	NULL(δ���ҵ�)/�ڵ��ַ
 * ****************************************************************************/
void *list_find(ListHandler_t *head, cmpFun cmp, void *conVal)
{
	Node_t *tail = head->node;

	ERRR((head == NULL || tail == NULL), goto ERR1);

	do {
		if (0 == cmp(tail->payload, conVal))
		{
			return tail->payload;
		}
		tail = tail->next;
	} while (tail != head->node);

ERR1:
	return NULL;
}
/*******************************************************************************
 * �� ��������	list_trans
 * �� ����	��	�������
 * �� ����	��	head:		Ŀ���������ͷ
                fun:        ���ݲ�������
                optPoint    �ⲿ����ָ��
 * �� ����	��	��������FALSE��TRUE
 * ****************************************************************************/
void list_trans(ListHandler_t *head, processorFun fun, void *optPoint)
{
    Node_t *tail = head->node;
    void *payload;

	ERRR((head == NULL || tail == NULL || fun == NULL), return);
	do {
        payload = tail->payload;
        tail = tail->next;
		if (0 == fun(payload, optPoint))
        {
            break;
        }
	} while (tail != head->node && head->node != NULL);
}

/*******************************************************************************
 * �� ��������	list_nodeDelete
 * �� ����	��	����ڵ�ɾ��
 * �� ����	��	node:		Ŀ������ڵ�
 * �� ����	��	��������FALSE��TRUE
 * ****************************************************************************/
void list_nodeDelete(ListHandler_t *head, void *payload)
{
	Node_t *node;

	node = nodeCheck((u8_t*)payload);
	if (node != null)
	{
		memory_release(node);
		return;
	}

	ERRR((head == NULL || head->node == NULL), return);
    node = nodeFind(head, payload);
    if (node != null)
    {
        if (head->node == node)
        {
            if (node->next == head->node)	//���һ���ڵ�
            {
                head->node = null;
            }
            else
            {
				
                head->node = node->next;
            }
        }
        node->front->next = node->next;
        node->next->front = node->front;
        memory_release(node);
        head->count--;
	}
}
/*******************************************************************************
 * �� ��������	list_delete
 * �� ����	��	����ɾ��
 * �� ����	��	head:		Ŀ���������ͷ
 * �� ����	��	��������FALSE��TRUE
 * ****************************************************************************/
void list_delete(ListHandler_t *head)
{
	Node_t *node;
    Node_t *tail = head->node;

	ERRR((head == NULL || tail == NULL), return);
	do {
		node = tail;
		tail = tail->next;
		memory_release(node);
	} while (tail != head->node && tail != NULL);
    
    list_init(head);
}

/*******************************************************************************
 * �� ��������	nodeCheck
 * �� ����	��	�˶����ݿռ��Ƿ�����ͨ�����������
 * �� ����	��	pLoad  Ŀ�����ݿռ�
 * �� ����	��	��������FALSE��TRUE
 * ****************************************************************************/
static Node_t *nodeCheck(u8_t *pLoad)
{
	Node_t *node = (Node_t*)(((u8_t*)pLoad) - sizeof(Node_t));

	if (pLoad != null && (LIST_APPLY_FLAG^(int)node->front) == (int)pLoad)
	{
		return node;
	}

	return null;
}

/*******************************************************************************
 * �� ��������	nodeFind
 * �� ����	��	�˶����ݿռ��Ƿ���������
 * �� ����	��	head:		Ŀ���������ͷ
 * �� ����	��	��������FALSE��TRUE
 * ****************************************************************************/
static Node_t *nodeFind(ListHandler_t *head, u8_t *pLoad)
{
    Node_t *node;
    Node_t *tail;
    
	ERRR((head == null || head->node == NULL || pLoad == NULL), goto END);
    
    node = (Node_t*)(((u8_t*)pLoad) - sizeof(Node_t));
    tail = head->node;
    do {
		if (node == tail)
		{
            return node;
        }
        tail = tail->next;
    } while(tail != head->node && tail != NULL);
    
END: 
	return null;
}



/*****************************Copyright I��S Team******************************/

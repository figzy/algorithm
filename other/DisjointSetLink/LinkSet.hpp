#pragma once
#include <cstdlib>

template <typename Value>
struct LSNode
{
	Value val;
	LSNode<Value>* head;
	LSNode<Value>* next;
};

/*
 * ���ཻ���ϵ������ʾ
 *
 * ģ�����
 *  Value�������д�ŵ�ֵ
 *  Compare�����ڱȽ� Value ֮���Ƿ���ȵĺ���
 *   �������ͣ�bool Compare(Value v1�� Value v2);��v1 == v2���򷵻� true�� ���򷵻� false
 */
template <typename Value, typename Compare>
class LinkSet
{
protected:
	typedef LSNode<Value> __link_set_node;

public:
	LinkSet(Compare cmp);
	~LinkSet();

	bool Exist(Value _Val);
	void MakeSet(Value _Val);
	LSNode<Value>* FindSet();

	static LSNode<Value>* FindSet(LSNode<Value>* x);
	static LSNode<Value>* Union(LinkSet<Value, Compare>* x, LinkSet<Value, Compare>* y);

	LSNode<Value>* head;
	LSNode<Value>* tail;
	Compare compare;
};

template<typename Value, typename Compare>
inline LinkSet<Value, Compare>::LinkSet(Compare cmp)
{
	head = NULL;
	tail = NULL;
	compare = cmp;
}

template<typename Value, typename Compare>
inline LinkSet<Value, Compare>::~LinkSet()
{
}

/*
 * �жϼ������Ƿ��Ѿ�����ֵ _Val
 */
template<typename Value, typename Compare>
inline bool LinkSet<Value, Compare>::Exist(Value _Val)
{
	__link_set_node* p = head;
	while (p){
		if (compare(p->val, _Val)) {
			return true;
		}
	}
	return false;
}

/*
 * �����������ֵ _Val
 */
template<typename Value, typename Compare>
inline void LinkSet<Value, Compare>::MakeSet(Value _Val)
{
	__link_set_node* node = new __link_set_node();
	node->val = _Val;

	if (!head) {
		head = node;
		tail = node;
		node->next = head;
		node->head = head;
	}
	else {
		node->next = NULL;
		node->head = head;
		tail->next = node;
		tail = node;
	}
}

/*
 * ����ָ�򼯺ϵĴ����ָ��
 */
template<typename Value, typename Compare>
inline LSNode<Value>* LinkSet<Value, Compare>::FindSet()
{
	return head;
}

template<typename Value, typename Compare>
inline LSNode<Value>* LinkSet<Value, Compare>::FindSet(LSNode<Value>* x)
{
	return x->head;
}

/*
 * �ϲ��������� x �� y
 */
template<typename Value, typename Compare>
inline LSNode<Value>* LinkSet<Value, Compare>::Union(LinkSet<Value, Compare>* x, LinkSet<Value, Compare>* y)
{
	if (!x || !y || !x->head || !y->head) {
		return NULL;
	}

	if (x == y) {
		return x->head;
	}

	LSNode<Value>* p = y->head;
	LSNode<Value>* q = NULL;
	x->tail->next = y->head;
	x->tail = y->tail;

	while (p) {
		q = p->next;
		p->head = x->head;
		p = q;
	}

	return x->head;
}
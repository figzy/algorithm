#pragma once

/*
 * ɭ�ֽڵ㶨��
 */
template <typename Value>
struct FNode
{
	Value val;
	int rank;					// ���� x �ĸ߶�
	FNode<Value>* p;			// ���ڵ�
};

/*
 * ���ཻ���ϵ�ɭ�ֱ�ʾ
 *
 * ģ�����
 *  Value: �����д�ŵ�ֵ
 *  Compare: ���ڱȽ� Value ֮���Ƿ��С�ĺ���
 *    �������ͣ�int Compare(Value v1�� Value v2);�� v1==v2���򷵻� =0;�� v1<v2���򷵻� <0;�� v1>v2���򷵻� >0.
 */
template <typename Value>
class FSet
{
public:
	static void MakeSet(FNode<Value>* x);
	static void Union(FNode<Value>* x, FNode<Value>* y);
	static FNode<Value>* FindSet(FNode<Value>* x);

private:
	static void Link(FNode<Value>* x, FNode<Value>* y);
};

/*
 * ����һ���¼��ϣ�����Ψһ��Ա�� x
 */
template<typename Value>
inline void FSet<Value>::MakeSet(FNode<Value>* x)
{
	x->p = x;
	x->rank = 0;
}

/*
 * �ϲ��������� x �� y
 */
template<typename Value>
inline void FSet<Value>::Union(FNode<Value>* x, FNode<Value>* y)
{
	Link(FindSet(x), FindSet(y));
}

/*
 * ����ָ�򼯺ϴ����ָ��
 */
template<typename Value>
inline FNode<Value>* FSet<Value>::FindSet(FNode<Value>* x)
{
	if (x != x->p) {
		x->p = FindSet(x->p);
	}
	return x->p;
}

template<typename Value>
inline void FSet<Value>::Link(FNode<Value>* x, FNode<Value>* y)
{
	if (x->rank > y->rank) {
		y->p = x;
	}
	else {
		x->p = y;
		if (x->rank == y->rank) {
			y->rank = y->rank + 1;
		}
	}
}

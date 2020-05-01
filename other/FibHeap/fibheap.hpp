#pragma once

/* 
 * 쳲������ѽ�㶨��
 * 
 * ��Ա����
 *  key���ؼ���
 *  degree����㺢�������еĺ�����Ŀ
 *  mark����ǽڵ����ϴγ�Ϊ��һ�����ĺ��Ӻ��Ƿ�ʧȥ������
 *  parent��ָ��˫�׽���ָ��
 *  child��ָ���ӽ���ָ��
 *  left��ָ�����ֵܵ�ָ��
 *  right��ָ�����ֵܵ�ָ��
 */
template<typename Key>
struct FibHeapNode 
{
	Key key;
	size_t degree;
	bool mark;
	FibHeapNode<Key, Value>* parent;
	FibHeapNode<Key, Value>* child;
	FibHeapNode<Key, Value>* left;
	FibHeapNode<Key, Value>* right;
};

/*
 * 쳲�������
 *
 * ģ�����
 *  Key��쳲������ѵĹؼ���
 *  Compare���ؼ��ֵıȽϺ������͡�
 *    �������ͣ�int Compare(Key k1, Key k2){ //... };
 *    ����ֵ���� k1<k2,����ֵ <0; �� k1==k2,����ֵ =0; �� k1>k2,����ֵ >0
 *
 * ��Ա����
 *  m_min��쳲������ѵ���С��㣨����Ϊ�գ���ΪNULL��
 *  m_num����ǰ쳲��������еĽ�����
 */
template<typename Key, typename Compare>
class FibHeap
{
public:
	FibHeap(Compare cmp);
	~FibHeap();

	void Insert(Key _Key);
	FibHeapNode<Key>* ExtractMin();

protected:
	typedef FibHeapNode<Key> __fib_heap_node;

private:
	FibHeapNode<Key>* m_min;
	size_t m_count;
	Compare compare;
};

template<typename Key, typename Compare>
inline FibHeap<Key>::FibHeap(Compare cmp)
{
	m_count = 0;
	m_min = NULL;
	compare = cmp;
}

template<typename Key, typename Compare>
inline FibHeap<Key>::~FibHeap()
{
	
}

template<typename Key, typename Compare>
inline void FibHeap<Key>::Insert(Key _Key)
{
	__fib_heap_node* node = new __fib_heap_node();
	node->key = _Key;
	node->degree = 0;
	node->parent = NULL;
	node->mark = false;
	node->child = NULL;

	if (m_min == NULL) {
		node->left = node;
		node->right = node;
		m_min = node;
	}
	else {
		node->left = m_min->left;
		node->right = m_min;
		m_min->left = node;
		if (compare(node->key, m_min->key) < 0) {
			m_min = node;
		}
	}

	++m_count;
}

template<typename Key, typename Compare>
inline FibHeapNode<Key>* FibHeap<Key, Compare>::ExtractMin()
{
	__fib_heap_node* z = m_min;

	if (z != NULL) {
		// add each child x of z to the root list
		__fib_heap_node* x = z->child;
		__fib_heap_node* p = NULL;
		while (x->left != NULL) x = x->left;
		while (x) {
			p = x->right;
			x->left = m_min->left;
			x->right = m_min;
			x->parent = NULL;
			m_min->left = x;
			x = p;
		}

		// remove z from the root list
		z->left->right = z->right;
		z->right->left = z->left;

		if (z == z->right) {
			m_min = NULL;
		}
		else {
			// TODO
		}

		m_count--;
	}

	return z;
}

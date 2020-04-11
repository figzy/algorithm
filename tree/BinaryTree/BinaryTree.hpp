#pragma once
#include <stdlib.h>
#include <stack>
#include <queue>
using namespace std;

template<typename ElemType>
struct BTNode
{
	ElemType val;
	struct BTNode<ElemType>* lchild;
	struct BTNode<ElemType>* rchild;
	struct BTNode<ElemType>* parent;
};

/*
 * ������(����������)
 *
 * 1��ElemType �������洢����������
 * 2��Compare: Ԫ�� ElemType �ıȽϺ�����
 *			����ԭ��Ϊ��size_t (*Compare)(ElemType v, ElemType w)
 *			����ֵ���ͣ�
 *					(1)�� v > w ʱ���� >0
 *					(2)�� v = w ʱ���� =0
 *					(3)�� v < w ʱ���� <0
 */
template<typename ElemType, typename Compare>
class BTree
{
public:
	BTree(Compare cmp);
	~BTree();

	bool Insert(const ElemType& _Val);
	bool Delete(const ElemType& _Val);

	void PreOrder(void (*visit)(const ElemType& _Val));
	void InOrder(void (*visit)(const ElemType& _Val));
	void PostOrder(void (*visit)(const ElemType& _Val));
	void LevelOrder(void (*visit)(const ElemType& _Val));

	ElemType Minimum() const;
	ElemType Maximum() const;

	size_t Size() const;
	size_t Depth() const;
	size_t Empty() const;

private:
	void destroy(BTNode<ElemType>* T);
	BTNode<ElemType>* search(const ElemType& _Val);
	BTNode<ElemType>* minimum(BTNode<ElemType>* T);
	BTNode<ElemType>* maximun(BTNode<ElemType>* T);

private:
	BTNode<ElemType>* m_root;
	size_t m_size;
	Compare m_cmp;
};

/*
 * ���캯��
 *
 * ����
 *  cmp ��ΪԪ�� ElemType �ıȽϺ�����ָ��
 *        ����ԭ��Ϊ��size_t (*Compare)(ElemType v, ElemType w)
 *	      cmp ����ֵҪ��
 *					(1)�� v > w ʱ���� >0
 *					(2)�� v = w ʱ���� =0
 *					(3)�� v < w ʱ���� <0
 */
template<typename ElemType, typename Compare>
inline BTree<ElemType, Compare>::BTree(Compare cmp)
{
	m_root = NULL;
	m_size = 0;
	m_cmp = cmp;
}

/*
 * ��������
 */
template<typename ElemType, typename Compare>
BTree<ElemType, Compare>::~BTree()
{
	Destroy(m_root);
}

/*
 * �ڶ������в���һ��Ԫ��
 *
 * ����
 *  _Val �������Ԫ��ֵ
 *
 * ����ֵ
 *  bool ���������ɹ�(�����в����ڸ�Ԫ��)���򷵻� true
 *         �������ʧ�ܣ����� false
 */
template<typename ElemType, typename Compare>
bool BTree<ElemType, Compare>::Insert(const ElemType& _Val)
{
	BTNode<ElemType>* p = NULL;
	BTNode<ElemType>* q = NULL;
	BTNode<ElemType>* node = NULL;

	// Find node's parent
	// Left child is less to parent
	// Right child is greater than or equal to parent
	p = m_root;
	while (p) {
		q = p;
		if (m_cmp(p->val, _Val) < 0) {
			p = p->lchild;
		}
		else {
			p = p->rchild;
		}
	}

	// Insert the node into the leaf node
	node = new BTNode<ElemType>();
	node->lchild = NULL;
	node->rchild = NULL;
	node->val = _Val;
	node->parent = q;

	if (q == NULL) {
		m_root = p; // tree was empty
	}
	else if (node->val < q->val) {
		q->lchild = node;
	}
	else {
		q->rchild = node;
	}

	++m_size;
}

/*
 * ɾ���������е�Ԫ��
 *
 * ����
 *  _Val ��ɾ����Ԫ��ֵ
 *
 * ����ֵ
 *  bool �����ɾ���ɹ�(�����ڸ�ֵΪ_Val�Ľڵ�)������ true
 *         ���ɾ��ʧ�ܣ����� false
 */
template<typename ElemType, typename Compare>
bool BTree<ElemType, Compare>::Delete(const ElemType& _Val)
{
	BTNode<ElemType>* p = NULL;
	BTNode<ElemType>* q = NULL;

	// Search the node 
	p = Search(_Val);
	if (!p) {
		return false;
	}

	if (p == m_root) {
		// no left child
		if (!p->lchild) {
			if (p->rchild) {
				p->rchild->parent = NULL;
			}
			m_root = p->rchild;
		}
		// only left child
		else if (p->lchild && !p->rchild) {
			p->lchild->parent = NULL;
			m_root = p->lchild;
		}
		// two child
		else {
			if (!p->rchild->lchild) {
				p->rchild->lchild = p->lchild;
				p->rchild->parent = NULL;
				p->lchild->parent = p->rchild;
				m_root = p->rchild;
			}
			else {
				q = Minimum(p->rchild);

				q->parent->lchild = q->rchild;
				if (q->rchild) {
					q->rchild->parent = q->parent;
				}

				q->lchild = p->lchild;
				q->rchild = p->rchild;
				q->parent = NULL;
				p->lchild->parent = q;
				p->rchild->parent = q;
				m_root = q;
			}
		}
	}
	else {
		// no left child
		if (!p->lchild) {
			if (p == p->parent->lchild) {
				p->parent->lchild = p->rchild;
			}
			else {
				p->parent->rchild = p->rchild;
			}

			if (p->rchild) {
				p->rchild->parent = p->parent;
			}
		}
		// only left child
		else if (p->lchild && !p->rchild) {
			if (p == p->parent->lchild) {
				p->parent->lchild = p->lchild;
			}
			else {
				p->parent->rchild = p->lchild;
			}

			p->lchild->parent = p->parent;
		}
		// two child
		else {
			if (!p->rchild->lchild) {
				p->lchild->parent = p->rchild;
				p->rchild->lchild = p->lchild;
				p->rchild->parent = p->parent;
			}
			else {
				q = Minimum(p->rchild);

				q->parent->lchild = q->rchild;
				if (q->rchild) {
					q->rchild->parent = q->parent;
				}

				q->lchild = p->lchild;
				q->rchild = p->rchild;
				q->parent = p->parent;
				if (p == p->parent->lchild) {
					p->parent->lchild = q;
				}
				else {
					p->parent->rchild = q;
				}
				p->lchild->parent = q;
				p->rchild->parent = q;
			}
		}
	}

	delete p;
	p = NULL;
	--m_size;

	return true;
}

/*
 * ������ǰ�����
 *
 * ����
 *  visit ���ò���Ϊ�������ڵ�ĺ���ָ�룬��ʵ�ֶ����ڵ����ز���
 */
template<typename ElemType, typename Compare>
inline void BTree<ElemType, Compare>::PreOrder(void(*visit)(const ElemType& _Val))
{
	stack<BTNode<ElemType>*> st;
	BTNode<ElemType>* p = m_root;

	while (p || !st.empty()) {
		if (p) {
			visit(p->val);
			if (p->rchild) {
				st.push(p->rchild);
			}
			p = p->lchild;
		}
		else {
			p = st.top();
			st.pop();
		}
	}
}

/*
 * �������������
 *
 * ����
 *  visit ���ò���Ϊ�������ڵ�ĺ���ָ�룬��ʵ�ֶ����ڵ����ز���
 */
template<typename ElemType, typename Compare>
inline void BTree<ElemType, Compare>::InOrder(void(*visit)(const ElemType& _Val))
{
	stack<BTNode<ElemType>*> st;
	BTNode<ElemType>* p = m_root;

	while (p || !st.empty()) {
		if (p) {
			st.push(p);
			p = p->lchild;
		}
		else {
			p = st.top();
			visit(p->val);
			st.pop();
			p = p->rchild;
		}
	}
}

/*
 * �������������
 *
 * ����
 *  visit ���ò���Ϊ�������ڵ�ĺ���ָ�룬��ʵ�ֶ����ڵ����ز���
 */
template<typename ElemType, typename Compare>
inline void BTree<ElemType, Compare>::PostOrder(void(*visit)(const ElemType& _Val))
{
	stack<BTNode<ElemType>*> st;
	BTNode<ElemType>* r = NULL;
	BTNode<ElemType>* p = m_root;

	while (p || !st.empty()) {
		if (p) {
			st.push(p);
			p = p->lchild;
		}
		else {
			p = st.top();
			if (p->rchild && p->rchild != r) {
				p = p->rchild;
			}
			else {
				visit(p->val);
				st.pop();
				r = p;
				p = NULL;
			}
		}
	}
}

/*
 * ��������α���
 *
 * ����
 *  visit ���ò���Ϊ�������ڵ�ĺ���ָ�룬��ʵ�ֶ����ڵ����ز���
 */
template<typename ElemType, typename Compare>
inline void BTree<ElemType, Compare>::LevelOrder(void(*visit)(const ElemType& _Val))
{
	queue<BTNode<ElemType>*> qu;
	BTNode<ElemType>* p = NULL;
	qu.push(m_root);

	while (!qu.empty()) {
		p = qu.front();
		visit(p->val);
		qu.pop();

		if (p->lchild) {
			qu.push(p->lchild);
		}
		if (p->rchild) {
			qu.push(p->rchild);
		}
	}
}

/*
 * ��ȡ�������е���Сֵ
 *
 * ����ֵ
 *  ElemType ���������Ϊ�գ�������С��Ԫ��ֵ
 *             �����Ϊ�գ����ж����У���ȡ��Сֵǰ���ȵ��� Empty() �����ж����Ƿ�Ϊ�գ�
 */
template<typename ElemType, typename Compare>
inline ElemType BTree<ElemType, Compare>::Minimum() const
{
	BTNode<ElemType>* p = minimum(m_root);
	if (!p) {
		abort();
	}

	return p->val;
}

/*
 * ��ȡ�������е����ֵ
 *
 * ����ֵ
 *  ElemType ���������Ϊ�գ���������Ԫ��ֵ
 *             �����Ϊ�գ����ж����У���ȡ���ֵǰ���ȵ��� Empty() �����ж����Ƿ�Ϊ�գ�
 */
template<typename ElemType, typename Compare>
inline ElemType BTree<ElemType, Compare>::Maximum() const
{
	BTNode<ElemType>* p = maximun(m_root);
	if (!p) {
		abort();
	}

	return p->val;
}

/*
 * ��ȡ���Ľڵ����
 */
template<typename ElemType, typename Compare>
inline size_t BTree<ElemType, Compare>::Size() const
{
	return m_size;
}

/*
 * ��ȡ�������
 */
template<typename ElemType, typename Compare>
size_t BTree<ElemType, Compare>::Depth() const
{
	if (!m_root) {
		return 0;
	}

	size_t level = 0;
	size_t n = 0;
	BTNode<ElemType>* p = NULL;
	queue<BTNode<ElemType>*> qu;
	qu.push(m_root);

	while (!qu.empty()) {
		n = qu.size();
		++level;

		while (n--) {
			p = qu.front();
			qu.pop();
			if (p->lchild) {
				qu.push(p->lchild);
			}
			if (p->rchild) {
				qu.push(p->rchild);
			}
		}
	}
}

/*
 * ���Ƿ�Ϊ��
 *
 * ����ֵ
 *  bool ����Ϊ�գ����� true
 *         ����Ϊ�գ����� false
 */
template<typename ElemType, typename Compare>
inline size_t BTree<ElemType, Compare>::Empty() const
{
	return m_size == 0 ? true : false;
}

/*
 * ����һ������
 *
 * ����
 *  T �������ٵ��������ڵ�
 */
template<typename ElemType, typename Compare>
void BTree<ElemType, Compare>::destroy(BTNode<ElemType>* T)
{
	if (T) {
		destroy(T->lchild);
		destroy(T->rchild);
		delete T;
		T = NULL;
	}
}

/*
 * ����Ԫ������Ӧ�Ľڵ�ָ��
 *
 * ������
 *  _Val �������ҵ�Ԫ��ֵ
 *
 * ����ֵ
 *  BTNode<ElemType>* ��������ڸ�Ԫ��ֵ���򷵻ض�Ӧ�Ľڵ�ָ��
 *                      ��������ڸ�Ԫ��ֵ���򷵻� NULL
 */
template<typename ElemType, typename Compare>
BTNode<ElemType>* BTree<ElemType, Compare>::search(const ElemType& _Val)
{
	BTNode<ElemType>* p = m_root;
	size_t n = 0;

	while (p) {
		n = m_cmp(p->val, _Val);
		if (n == 0) {
			break;
		}
		else if (n < 0) {
			p = p->lchild;
		}
		else {
			p = p->rchild;
		}
	}

	return p;
}

/*
 * ��ȡ���� T �е�����ڵ�(��Сֵ�ڵ�)
 */
template<typename ElemType, typename Compare>
BTNode<ElemType>* BTree<ElemType, Compare>::minimum(BTNode<ElemType>* T)
{
	BTNode<ElemType>* p = T;
	if (p) {
		while (p->lchild) {
			p = p->lchild;
		}
	}

	return p;
}

/*
 * ��ȡ���� T ���ҽڵ�(���ֵ�ڵ�)
 */
template<typename ElemType, typename Compare>
BTNode<ElemType>* BTree<ElemType, Compare>::maximun(BTNode<ElemType>* T)
{
	BTNode<ElemType>* p = T;
	if (p) {
		while (p->rchild) {
			p = p->rchild;
		}
	}

	return p;
}
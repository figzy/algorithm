#pragma once
#include <stdlib.h>
#include <stack>
#include <queue>
using namespace std;

/*
 * �����ɫ
 */
enum ColorType 
{
	RED, BLACK,
};

/*
 * �����
 *
 * ��Ա����
 *  val ������㱣�����Ϣֵ
 *	lchild ��ָ����������ӽ���ָ��
 *  rchild ��ָ��������Һ��ӽ���ָ��
 *  parent ��ָ�������˫�׽���ָ��
 */
template<typename KeyType, typename ValueType>
struct RBNode
{
	KeyType key;
	ValueType val;
	ColorType color;
	struct RBNode<KeyType, ValueType>* lchild;
	struct RBNode<KeyType, ValueType>* rchild;
	struct RBNode<KeyType, ValueType>* parent;
};

/*
 * ������(����������)����������С�ڡ�����㣬�����������ڡ�����㣬�����������ͬ��Ϣ�Ľ��
 *
 * ���������
 *  1��ÿ�������Ǻ�ɫ�ģ����Ǻ�ɫ��
 *  2��������Ǻ�ɫ��
 *  3��ÿ��Ҷ��㣨NIL���Ǻ�ɫ��
 *  4�����һ������Ǻ�ɫ�ģ������������ӽ�㶼�Ǻ�ɫ��
 *  5����ÿ����㣬�Ӹýڵ㶼�����к��Ҷ���ļ�·���ϣ���������ͬ��Ŀ�ĺ�ɫ���
 *
 * ģ������
 *  KeyType �������ʵ��Ӧ�������򼯺�set�� key ֵ����
 *  ValueType �������ʵ��Ӧ�������򼯺�set�� value ֵ����
 *  Compare: Ԫ�� KeyType �ıȽϺ�����
 *			����ԭ��Ϊ��int (*Compare)(KeyType v, KeyType w)
 *			����ֵ���ͣ�
 *					(1)�� v > w ʱ���� >0
 *					(2)�� v = w ʱ���� =0
 *					(3)�� v < w ʱ���� <0
 */
template<typename KeyType, typename ValueType, typename Compare>
class RBTree
{
public:
	RBTree(Compare cmp);
	virtual ~RBTree();

	virtual bool Insert(const KeyType& _Key, const KeyType, ValueType& _Val);
	virtual bool Delete(const KeyType& _Key);

	virtual void PreOrder(void (*visit)(const KeyType& _Key, const ValueType& _Val));
	virtual void InOrder(void (*visit)(const KeyType& _Key, const ValueType& _Val));
	virtual void PostOrder(void (*visit)(const KeyType& _Key, const ValueType& _Val));
	virtual void LevelOrder(void (*visit)(const KeyType& _Key, const ValueType& _Val));

	virtual ValueType Minimum();
	virtual ValueType Maximum();

	virtual size_t Size() const;
	virtual size_t Depth() const;
	virtual bool Empty() const;

private:
	void Destroy(RBNode<KeyType, ValueType>* T);
	RBNode<KeyType, ValueType>* search(const KeyType& _Key);
	RBNode<KeyType, ValueType>* minimum(RBNode<KeyType, ValueType>* T);
	RBNode<KeyType, ValueType>* maximun(RBNode<KeyType, ValueType>* T);

	void InsertFixup(RBNode<KeyType, ValueType>* T);
	void LeftRotate(RBNode<KeyType, ValueType>* T);
	void RightRotate(RBNode<KeyType, ValueType>* T);

	void DeleteFixup(RBNode<KeyType, ValueType>* T);
	void Transplant(RBNode<KeyType, ValueType>* told, RBNode<KeyType, ValueType>* tnew);

private:
	RBNode<KeyType, ValueType>* m_root;
	RBNode<KeyType, ValueType>* m_nil;// �ڱ��ս�㣨Ϊ�˱��ڴ������������еı߽�������ʹ֮����ѭ������ʽ��
	size_t m_size;
	Compare m_cmp;
};

/*
 * ���캯��
 *
 * ����
 *  cmp ��ΪԪ�� KeyType, ValueType �ıȽϺ�����ָ��
 *        ����ԭ��Ϊ��size_t (*Compare)(KeyType, ValueType v, KeyType, ValueType w)
 *	      Compare ����ֵҪ��
 *					(1)�� v > w ʱ���� >0
 *					(2)�� v = w ʱ���� =0
 *					(3)�� v < w ʱ���� <0
 */
template<typename KeyType, typename ValueType, typename Compare>
inline RBTree<KeyType, ValueType, Compare>::RBTree(Compare cmp)
{
	m_nil = new RBNode<KeyType, ValueType>();
	if (!m_nil) {
		abort();
	}
	m_nil->color = BLACK;
	m_nil->lchild = NULL;
	m_nil->rchild = NULL;
	m_nil->parent = NULL;

	m_root = NULL;
	m_size = 0;
	m_cmp = cmp;
}

/*
 * ��������
 */
template<typename KeyType, typename ValueType, typename Compare>
RBTree<KeyType, ValueType, Compare>::~RBTree()
{
	destroy(m_root);

	if (m_nil) {
		delete m_nil;
	}
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
template<typename KeyType, typename ValueType, typename Compare>
bool RBTree<KeyType, ValueType, Compare>::Insert(const KeyType& _Key, const KeyType, ValueType& _Val)
{
	RBNode<KeyType, ValueType>* p = NULL;
	RBNode<KeyType, ValueType>* q = NULL;
	RBNode<KeyType, ValueType>* newnode = NULL;
	int n = 0;

	// ���ҵ��������Ҷ��λ��
	p = m_root;
	while (p) {
		q = p;
		n = m_cmp(_Key, p->key);
		if (n < 0) {
			p = p->lchild;
		}
		else if (n > 0) {
			p = p->rchild;
		}
		else {
			return false;
		}
	}

	newnode = new RBNode<KeyType, ValueType>();
	newnode->lchild = m_nil;
	newnode->rchild = m_nil;
	newnode->key = _Key;
	newnode->val = _Val;
	newnode->parent = q;
	newnode->color = RED;

	if (q == NULL) {
		newnode->color = BLACK;
		newnode->parent = m_nil;
		m_root = newnode; // ��Ϊ��
	}
	else {
		if (n < 0) {
			q->lchild = newnode;
		}
		else if (n > 0) {
			q->rchild = newnode;
		}
		
		InsertFixup(newnode);
	}
	
	++m_size;
	return true;
}

/*
 * ɾ���������е�Ԫ��
 *
 * ����
 *  _Val ��ɾ����Ԫ��ֵ
 *
 * ����ֵ
 *  bool �����ɾ���ɹ�(�����ڸ�ֵΪ_Val�Ľ��)������ true
 *         ���ɾ��ʧ�ܣ����� false
 */
template<typename KeyType, typename ValueType, typename Compare>
bool RBTree<KeyType, ValueType, Compare>::Delete(const KeyType& _Key)
{
	RBNode<KeyType, ValueType>* p = NULL;
	RBNode<KeyType, ValueType>* q = NULL;
	RBNode<KeyType, ValueType>* t = NULL;
	ColorType tcolor;

	// p Ϊ��ɾ���Ľ�� 
	p = search(_Key);
	if (!p) {
		return false;
	}

	// ʼ��ά�� t �Ǵ�����ɾ���Ľ������������ڵĽ��
	t = p;
	tcolor = t->color;

	// ���һ���� p ���ӽ������2ʱ����� p �Ǻ�ɫ��㣬�Ƴ��󽫵��ºڸ߼�һ����� q ���� DeleteFixup
	if (!p->lchild) {
		q = p->rchild;
		Transplant(p, p->rchild);
	}
	else if (!p->rchild) {
		q = p->lchild;
		Transplant(p, p->lchild);
	}
	// ��������� p ���ӽ�����2ʱ������滻�Ľ�� t Ϊ��ɫ���򽫵����������ڸ߼�һ����Ҫ�� t->rchild ���� DeleteFixup
	else {
		// ��ȡ p �ĺ�̽��
		t = minimum(p->rchild);
		tcolor = t->color;

		// �� q ��¼ t ���Һ���(�ý�㽫�滻 t ��λ��)���� t �滻 p ��λ�ú���� t Ϊ��ɫ�������º�����ʵ��ƻ�
		q = t->rchild;

		if (t->parent == p) {
			// p ���Һ���û������
			q->parent = t;
		}
		else {
			// t ���Һ����滻 t ��λ��
			Transplant(t, t->rchild);
			t->rchild = p->rchild;
			t->rchild->parent = t;
		}

		// t �滻 p ��λ��
		Transplant(p, t);
		t->lchild = p->lchild;
		t->lchild->parent = t;

		// ���滻 p λ�õ� t ���ú� p ��ͬ����ɫ��ȷ�����滻��� p ԭ���ڵĺڸ����ʲ���
		t->color = p->color;
	}
	
	// ��� t �Ǻ�ɫ�ڵ㣬ɾ�����ƶ��������������ʵ��ƻ�
	if (tcolor == BLACK) {
		DeleteFixup(q);
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
 *  visit ���ò���Ϊ���������ĺ���ָ�룬��ʵ�ֶ���������ز���
 */
template<typename KeyType, typename ValueType, typename Compare>
inline void RBTree<KeyType, ValueType, Compare>::PreOrder(void(*visit)(const KeyType& _Key, const ValueType& _Val))
{
	stack<RBNode<KeyType, ValueType>*> st;
	RBNode<KeyType, ValueType>* p = m_root;

	while (p != m_nil || !st.empty()) {
		if (p != m_nil) {
			visit(p->key, p->val);
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
 *  visit ���ò���Ϊ���������ĺ���ָ�룬��ʵ�ֶ���������ز���
 */
template<typename KeyType, typename ValueType, typename Compare>
inline void RBTree<KeyType, ValueType, Compare>::InOrder(void(*visit)(const KeyType& _Key, const ValueType& _Val))
{
	stack<RBNode<KeyType, ValueType>*> st;
	RBNode<KeyType, ValueType>* p = m_root;

	while (p != m_nil || !st.empty()) {
		if (p != m_nil) {
			st.push(p);
			p = p->lchild;
		}
		else {
			p = st.top();
			visit(p->key, p->val);
			st.pop();
			p = p->rchild;
		}
	}
}

/*
 * �������������
 *
 * ����
 *  visit ���ò���Ϊ���������ĺ���ָ�룬��ʵ�ֶ���������ز���
 */
template<typename KeyType, typename ValueType, typename Compare>
inline void RBTree<KeyType, ValueType, Compare>::PostOrder(void(*visit)(const KeyType& _Key, const ValueType& _Val))
{
	stack<RBNode<KeyType, ValueType>*> st;
	RBNode<KeyType, ValueType>* r = NULL;
	RBNode<KeyType, ValueType>* p = m_root;

	while (p != m_nil || !st.empty()) {
		if (p != m_nil) {
			st.push(p);
			p = p->lchild;
		}
		else {
			p = st.top();
			if (p->rchild && p->rchild != r) {
				p = p->rchild;
			}
			else {
				visit(p->key, p->val);
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
 *  visit ���ò���Ϊ���������ĺ���ָ�룬��ʵ�ֶ���������ز���
 */
template<typename KeyType, typename ValueType, typename Compare>
inline void RBTree<KeyType, ValueType, Compare>::LevelOrder(void(*visit)(const KeyType& _Key, const ValueType& _Val))
{
	queue<RBNode<KeyType, ValueType>*> qu;
	RBNode<KeyType, ValueType>* p = NULL;
	qu.push(m_root);

	while (!qu.empty()) {
		p = qu.front();
		visit(p->key, p->val);
		qu.pop();

		if (p->lchild != m_nil) {
			qu.push(p->lchild);
		}
		if (p->rchild != m_nil) {
			qu.push(p->rchild);
		}
	}
}

/*
 * ��ȡ�������е���Сֵ
 *
 * ����ֵ
 *  KeyType, ValueType ���������Ϊ�գ�������С��Ԫ��ֵ
 *             �����Ϊ�գ����ж����У���ȡ��Сֵǰ���ȵ��� Empty() �����ж����Ƿ�Ϊ�գ�
 */
template<typename KeyType, typename ValueType, typename Compare>
inline ValueType RBTree<KeyType, ValueType, Compare>::Minimum()
{
	RBNode<KeyType, ValueType>* p = minimum(m_root);
	if (!p) {
		abort();
	}

	return p->val;
}

/*
 * ��ȡ�������е����ֵ
 *
 * ����ֵ
 *  KeyType, ValueType ���������Ϊ�գ���������Ԫ��ֵ
 *             �����Ϊ�գ����ж����У���ȡ���ֵǰ���ȵ��� Empty() �����ж����Ƿ�Ϊ�գ�
 */
template<typename KeyType, typename ValueType, typename Compare>
inline ValueType RBTree<KeyType, ValueType, Compare>::Maximum()
{
	RBNode<KeyType, ValueType>* p = maximun(m_root);
	if (!p) {
		abort();
	}

	return p->val;
}

/*
 * ��ȡ���Ľ�����
 */
template<typename KeyType, typename ValueType, typename Compare>
inline size_t RBTree<KeyType, ValueType, Compare>::Size() const
{
	return m_size;
}

/*
 * ��ȡ�������
 */
template<typename KeyType, typename ValueType, typename Compare>
size_t RBTree<KeyType, ValueType, Compare>::Depth() const
{
	if (!m_root) {
		return 0;
	}

	size_t level = 0;
	size_t n = 0;
	RBNode<KeyType, ValueType>* p = NULL;
	queue<RBNode<KeyType, ValueType>*> qu;
	qu.push(m_root);

	while (!qu.empty()) {
		n = qu.size();
		++level;

		while (n--) {
			p = qu.front();
			qu.pop();
			if (p->lchild != m_nil) {
				qu.push(p->lchild);
			}
			if (p->rchild != m_nil) {
				qu.push(p->rchild);
			}
		}
	}

	return level;
}

/*
 * ���Ƿ�Ϊ��
 *
 * ����ֵ
 *  bool ����Ϊ�գ����� true
 *         ����Ϊ�գ����� false
 */
template<typename KeyType, typename ValueType, typename Compare>
inline bool RBTree<KeyType, ValueType, Compare>::Empty() const
{
	return m_size == 0 ? true : false;
}

/*
 * ����һ������
 *
 * ����
 *  T �������ٵ����������
 */
template<typename KeyType, typename ValueType, typename Compare>
void RBTree<KeyType, ValueType, Compare>::Destroy(RBNode<KeyType, ValueType>* T)
{
	if (T) {
		destroy(T->lchild);
		destroy(T->rchild);
		delete T;
		T = NULL;
	}
}

/*
 * ����Ԫ������Ӧ�Ľ��ָ��
 *
 * ������
 *  _Val �������ҵ�Ԫ��ֵ
 *
 * ����ֵ
 *  BTNode<KeyType, ValueType>* ��������ڸ�Ԫ��ֵ���򷵻ض�Ӧ�Ľ��ָ��
 *                      ��������ڸ�Ԫ��ֵ���򷵻� NULL
 */
template<typename KeyType, typename ValueType, typename Compare>
RBNode<KeyType, ValueType>* RBTree<KeyType, ValueType, Compare>::search(const KeyType& _Key)
{
	RBNode<KeyType, ValueType>* p = m_root;
	int n = 0;

	while (p != m_nil) {
		n = m_cmp(_Key, p->key);
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
 * ��ȡ���� T �е�������(��Сֵ���)
 */
template<typename KeyType, typename ValueType, typename Compare>
RBNode<KeyType, ValueType>* RBTree<KeyType, ValueType, Compare>::minimum(RBNode<KeyType, ValueType>* T)
{
	RBNode<KeyType, ValueType>* p = T;
	if (p) {
		while (p->lchild != m_nil) {
			p = p->lchild;
		}
	}

	return p;
}

/*
 * ��ȡ���� T ���ҽ��(���ֵ���)
 */
template<typename KeyType, typename ValueType, typename Compare>
RBNode<KeyType, ValueType>* RBTree<KeyType, ValueType, Compare>::maximun(RBNode<KeyType, ValueType>* T)
{
	RBNode<KeyType, ValueType>* p = T;
	if (p) {
		while (p->rchild != m_nil) {
			p = p->rchild;
		}
	}

	return p;
}

/*
 * ������ʱ������������Խ��������ɫ����ת����ʹ���µ�����������������5������ 
 *
 * while ѭ����ÿ�ε����Ŀ�ͷ��������3�����ֵĲ���ʽ��
 *  a.��� p �Ǻ���
 *  b.��� p->parent �Ǹ���㣬�� p->parent �Ǻڽ��
 *  c.������κκ�����ʱ��ƻ���������ֻ��һ�����ƻ�����������2����������4��
 *    �������2���ƻ�����ԭ���Ǹ�����Ǻ��㡣
 *    �������4���ƻ�����ԭ��Ϊ p �� p->parent ���Ǻ��㡣
 */
template<typename KeyType, typename ValueType, typename Compare>
inline void RBTree<KeyType, ValueType, Compare>::InsertFixup(RBNode<KeyType, ValueType>* T)
{
	RBNode<KeyType, ValueType>* p = T;
	RBNode<KeyType, ValueType>* q = NULL;

	// ����4 ���ƻ�
	while (p->parent->color == RED) {
		// p ��˫�׽�������ӵ����
		if (p->parent == p->parent->parent->lchild) {
			q = p->parent->parent->rchild;// ����
			if (q->color == RED) {
				// ���һ�������Ǻ�ɫ��
				q->color = BLACK;
				p->parent->color = BLACK;
				p->parent->parent->color = RED;
				p = p->parent->parent;
			}
			else {
				// ������������Ǻ�ɫ�ģ��� p ��һ���Һ���
				if (p == p->parent->rchild) {
					p = p->parent;
					LeftRotate(p);
				}
				// ������������Ǻ�ɫ�ģ��� p ��һ������ 
				p->parent->color = BLACK;
				p->parent->parent->color = RED;
				RightRotate(p->parent->parent);
			}
		}
		// p ��˫�׽�����Һ��ӵ�������������ӵ������෴��
		else {
			q = p->parent->parent->lchild;
			if (q->color == RED) {
				q->color = BLACK;
				p->parent->color = BLACK;
				p->parent->parent->color = RED;
				p = p->parent->parent;
			}
			else {
				if (p == p->parent->lchild) {
					p = p->parent;
					RightRotate(p);
				}
				p->parent->color = BLACK;
				p->parent->parent->color = RED;
				LeftRotate(p->parent->parent);
			}
		}
	}

	// ����2 ���ƻ�
	m_root->color = BLACK;
}

/*
 * ������������
 */
template<typename KeyType, typename ValueType, typename Compare>
inline void RBTree<KeyType, ValueType, Compare>::LeftRotate(RBNode<KeyType, ValueType>* T)
{
	RBNode<KeyType, ValueType>* p = T;
	RBNode<KeyType, ValueType>* q = NULL;

	q = p->rchild;
	p->rchild = q->lchild;

	if (q->lchild != m_nil) {
		q->lchild->parent = p;
	}

	q->parent = p->parent;

	if (p->parent == m_nil) {
		m_root = q;
	}
	else if (T == T->parent->lchild) {
		p->parent->lchild = q;
	}
	else {
		p->parent->rchild = q;
	}

	q->lchild = p;
	p->parent = q;
}

/*
 * ������������
 */
template<typename KeyType, typename ValueType, typename Compare>
inline void RBTree<KeyType, ValueType, Compare>::RightRotate(RBNode<KeyType, ValueType>* T)
{
	RBNode<KeyType, ValueType>* p = T;
	RBNode<KeyType, ValueType>* q = NULL;

	q = p->lchild;
	p->lchild = q->rchild;

	if (q->rchild != m_nil) {
		q->rchild->parent = p;
	}

	q->parent = p->parent;

	if (p->parent == m_nil) {
		m_root = q;
	}
	else if (p == p->parent->lchild) {
		p->parent->lchild = q;
	}
	else if (p == p->parent->rchild) {
		p->parent->rchild = q;
	}

	q->rchild = p;
	p->parent = q;
}

/*
 * ɾ�����ʱ������������Խ��������ɫ����ת����ʹ���µ�����������������5������
 */
template<typename KeyType, typename ValueType, typename Compare>
inline void RBTree<KeyType, ValueType, Compare>::DeleteFixup(RBNode<KeyType, ValueType>* T)
{
	RBNode<KeyType, ValueType>* p = T;
	RBNode<KeyType, ValueType>* q = NULL;

	while (p != m_root && p->color == BLACK) {

		if (p == p->parent->lchild) {

			q = p->parent->rchild;
			if (q->color == RED) {
				// ���һ��p ���ֵܽ�� q �Ǻ�ɫ�ġ�ת�� ����� �� ����� �� �����
				q->color = BLACK; // �ֵܽ����Ϊ��ɫ
				p->parent->color = RED;
				LeftRotate(p->parent);
				q = p->parent->rchild;
			}
			
			if (q->lchild->color == BLACK && q->rchild->color == BLACK) {
				// �������p ���ֵܽ�� q �Ǻ�ɫ�ģ��� q �������ӽڵ㶼�Ǻ�ɫ��
				q->color = RED;
				p = p->parent;
			}
			else {
				if (q->rchild->color == BLACK) {
					// �������p ���ֵܽ�� q �Ǻ�ɫ�ģ��� q �������Ǻ�ɫ�ģ�q ���Һ����Ǻ�ɫ�ġ�תΪ�����
					q->lchild->color = BLACK;
					q->color = RED;
					RightRotate(q);
					q = p->parent->rchild;
				}

				// ����ģ�p ���ֵܽ���� q �Ǻ�ɫ�ģ��� q ���Һ����Ǻ�ɫ��
				q->color = p->parent->color;
				p->parent->color = BLACK;
				q->rchild->color = BLACK;
				LeftRotate(p->parent);
				p = m_root;
			}
		}

		else {
			q = p->parent->lchild;
			if (q->color == RED) {
				q->color = BLACK;
				p->parent->color = RED;
				RightRotate(p->parent);
				q = p->parent->lchild;
			}
			if (q->rchild->color == BLACK && q->rchild->color == BLACK) {
				q->color = RED;
				p = p->parent;
			}
			else {
				if (q->lchild->color == BLACK) {
					q->rchild->color = BLACK;
					q->color = RED;
					LevelOrder(q);
					q = q->parent->lchild;
				}

				q->color = p->parent->color;
				p->parent->color = BLACK;
				q->lchild->color = BLACK;
				RightRotate(p->parent);
				p = m_root;
			}
		}
	}

	p->color = BLACK;
}

/*
 * ���ж��������ƶ���ʹ������ told �滻���� told
 *
 * ����
 *  told �����滻������
 *  tnew ���滻������
 */
template<typename KeyType, typename ValueType, typename Compare>
inline void RBTree<KeyType, ValueType, Compare>::Transplant(RBNode<KeyType, ValueType>* told, RBNode<KeyType, ValueType>* tnew)
{
	if (told->parent == m_nil) {
		m_root = tnew;
	}
	else if (told == told->parent->lchild) {
		told->parent->lchild = tnew;
	}
	else {
		told->parent->rchild = tnew;
	}

	tnew->parent = told->parent;
}

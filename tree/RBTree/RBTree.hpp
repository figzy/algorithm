#pragma once
#include <stdlib.h>
#include <stack>
#include <queue>
using namespace std;

/*
 * �����ɫ
 */
typedef bool RBColor;
const RBColor RED = true;
const RBColor BLACK = false;

/*
 * �����
 *
 * ��Ա����
 *  val ������㱣�����Ϣֵ
 *	lchild ��ָ����������ӽ���ָ��
 *  rchild ��ָ��������Һ��ӽ���ָ��
 *  parent ��ָ�������˫�׽���ָ��
 */
template<typename Key, typename Value>
struct RBNode
{
	Key key;
	Value val;
	RBColor color;
	struct RBNode<Key, Value>* lchild;
	struct RBNode<Key, Value>* rchild;
	struct RBNode<Key, Value>* parent;
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
 *  Key �������ʵ��Ӧ�������򼯺�set�� key ֵ����
 *  Value �������ʵ��Ӧ�������򼯺�set�� value ֵ����
 *  Compare: Ԫ�� Key �ıȽϺ�����
 *			����ԭ��Ϊ��int (*Compare)(Key v, Key w)
 *			����ֵ���ͣ�
 *					(1)�� v > w ʱ���� >0
 *					(2)�� v = w ʱ���� =0
 *					(3)�� v < w ʱ���� <0
 */
template<typename Key, typename Value, typename Compare>
class RBTree
{
public:
	RBTree(Compare cmp);
	virtual ~RBTree();

	virtual bool InsertUnique(const Key& _Key, const Value& _Val);
	virtual bool InsertEqual(const Key& _Key, const Value& _Val);

	virtual bool Delete(const Key& _Key);
	virtual bool Modify(const Key& _Key, const Value& _Val);
	virtual Value* Search(const Key& _Key);

	virtual void PreOrder(void (*visit)(const Key& _Key, const Value& _Val));
	virtual void InOrder(void (*visit)(const Key& _Key, const Value& _Val));
	virtual void PostOrder(void (*visit)(const Key& _Key, const Value& _Val));
	virtual void LevelOrder(void (*visit)(const Key& _Key, const Value& _Val));

	virtual Value Minimum();
	virtual Value Maximum();

	virtual size_t Count() const;
	virtual size_t Depth() const;
	virtual bool Empty() const;

private:
	void destroy(RBNode<Key, Value>* T);
	bool insert(const Key& _Key, const Value& _Val, bool key_unique);
	RBNode<Key, Value>* search(const Key& _Key);
	RBNode<Key, Value>* minimum(RBNode<Key, Value>* T);
	RBNode<Key, Value>* maximun(RBNode<Key, Value>* T);

	void insert_fixup(RBNode<Key, Value>* T);

	void left_rotate(RBNode<Key, Value>* T);
	void right_rotate(RBNode<Key, Value>* T);

	void delete_fixup(RBNode<Key, Value>* T);
	void transplant(RBNode<Key, Value>* told, RBNode<Key, Value>* tnew);

private:
	RBNode<Key, Value>* m_root;
	RBNode<Key, Value>* NIL;// �ڱ��ս�㣨Ϊ�˱��ڴ������������еı߽�������ʹ֮����ѭ������ʽ��
	size_t m_count;
	Compare m_cmp;
};

/*
 * ���캯��
 *
 * ����
 *  cmp ��ΪԪ�� Key �ıȽϺ�����ָ��
 *        ����ԭ��Ϊ��size_t (*Compare)(Key v, Key w)
 *	      Compare ����ֵҪ��
 *					(1)�� v > w ʱ���� >0
 *					(2)�� v = w ʱ���� =0
 *					(3)�� v < w ʱ���� <0
 */
template<typename Key, typename Value, typename Compare>
inline RBTree<Key, Value, Compare>::RBTree(Compare cmp)
{
	NIL = new RBNode<Key, Value>();
	if (!NIL) {
		abort();
	}
	NIL->color = BLACK;
	NIL->lchild = NULL;
	NIL->rchild = NULL;
	NIL->parent = NULL;

	m_root = NIL;
	m_count = 0;
	m_cmp = cmp;
}

/*
 * ��������
 */
template<typename Key, typename Value, typename Compare>
RBTree<Key, Value, Compare>::~RBTree()
{
	destroy(m_root);

	if (NIL) {
		delete NIL;
	}
}

/*
 * �ڶ������в���һ��Ԫ�أ�keyֵΨһ
 *
 * ����
 *  _Key �������keyֵ
 *  _Val �������key��Ӧ��valueֵ
 *
 * ����ֵ
 *  bool ���������ɹ�(�����в����ڸ�Ԫ��)���򷵻� true
 *         �������ʧ�ܣ����� false
 */
template<typename Key, typename Value, typename Compare>
bool RBTree<Key, Value, Compare>::InsertUnique(const Key& _Key, const Value& _Val)
{
	return insert(_Key, _Val, true);
}


/*
 * �ڶ������в���һ��Ԫ�أ�����keyֵ�ظ�
 *
 * ����
 *  _Key �������keyֵ
 *  _Val �������key��Ӧ��valueֵ
 *
 * ����ֵ
 *  bool ���������ɹ����򷵻� true
 *         �������ʧ�ܣ����� false
 */
template<typename Key, typename Value, typename Compare>
inline bool RBTree<Key, Value, Compare>::InsertEqual(const Key& _Key, const Value& _Val)
{
	return insert(_Key, _Val, false);
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
template<typename Key, typename Value, typename Compare>
bool RBTree<Key, Value, Compare>::Delete(const Key& _Key)
{
	RBNode<Key, Value>* p = NULL;
	RBNode<Key, Value>* q = NULL;
	RBNode<Key, Value>* t = NULL;
	RBColor tcolor;

	// p Ϊ��ɾ���Ľ�� 
	p = search(_Key);
	if (p == NIL) {
		return false;
	}

	// ʼ��ά�� t �Ǵ�����ɾ���Ľ������������ڵĽ��
	t = p;
	tcolor = t->color;

	// ���һ���� p ���ӽ������2ʱ����� p �Ǻ�ɫ��㣬�Ƴ��󽫵��ºڸ߼�һ����� q ���� DeleteFixup
	if (p->lchild == NIL) {
		q = p->rchild;
		transplant(p, p->rchild);
	}
	else if (p->rchild == NIL) {
		q = p->lchild;
		transplant(p, p->lchild);
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
			transplant(t, t->rchild);
			t->rchild = p->rchild;
			t->rchild->parent = t;
		}

		// t �滻 p ��λ��
		transplant(p, t);
		t->lchild = p->lchild;
		t->lchild->parent = t;

		// ���滻 p λ�õ� t ���ú� p ��ͬ����ɫ��ȷ�����滻��� p ԭ���ڵĺڸ����ʲ���
		t->color = p->color;
	}
	
	// ��� t �Ǻ�ɫ�ڵ㣬ɾ�����ƶ��������������ʵ��ƻ�
	if (tcolor == BLACK) {
		delete_fixup(q);
	}

	delete p;
	p = NULL;
	--m_count;

	return true;
}

/*
 * ���ݼ�(key)�޸Ķ�Ӧ��ֵ(val)
 *
 * ����
 *  _Key ��׼���޸ĵļ�
 *  _Val ��������Ӧ���µ�ֵ��Ϣ
 *
 * ����ֵ
 *  bool ���޸ĳɹ�������_Keyֵ�����򷵻� true
 *         �޸�ʧ�ܣ����� false
 */
template<typename Key, typename Value, typename Compare>
bool RBTree<Key, Value, Compare>::Modify(const Key& _Key, const Value& _Val)
{
	RBNode<Key, Value>* p = search(_Key);
	if (p == NIL) {
		return false;
	}

	p->val = _Val;
	return true;
}

/*
 * ���ݼ�(key)���Ҷ�Ӧ��ֵ(val)
 *
 * ����
 *  _Key ��׼�����ҵļ�
 *
 * ����ֵ
 *  Value* �����ҳɹ�������_Keyֵ�����򷵻ؿ����Ķ�Ӧֵ��ָ��
 *               ����ʧ�ܣ����� NULL
 */
template<typename Key, typename Value, typename Compare>
inline Value* RBTree<Key, Value, Compare>::Search(const Key& _Key)
{
	RBNode<Key, Value>* p = search(_Key);
	if (p == NIL) {
		return NULL;
	}

	Value* _Val = new Value();
	*_Val = p->val;
	return _Val;
}

/*
 * ������ǰ�����
 *
 * ����
 *  visit ���ò���Ϊ���������ĺ���ָ�룬��ʵ�ֶ���������ز���
 */
template<typename Key, typename Value, typename Compare>
inline void RBTree<Key, Value, Compare>::PreOrder(void(*visit)(const Key& _Key, const Value& _Val))
{
	stack<RBNode<Key, Value>*> st;
	RBNode<Key, Value>* p = m_root;

	while (p != NIL || !st.empty()) {
		if (p != NIL) {
			visit(p->key, p->val);
			if (p->rchild != NIL) {
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
template<typename Key, typename Value, typename Compare>
inline void RBTree<Key, Value, Compare>::InOrder(void(*visit)(const Key& _Key, const Value& _Val))
{
	stack<RBNode<Key, Value>*> st;
	RBNode<Key, Value>* p = m_root;

	while (p != NIL || !st.empty()) {
		if (p != NIL) {
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
template<typename Key, typename Value, typename Compare>
inline void RBTree<Key, Value, Compare>::PostOrder(void(*visit)(const Key& _Key, const Value& _Val))
{
	stack<RBNode<Key, Value>*> st;
	RBNode<Key, Value>* r = NIL;
	RBNode<Key, Value>* p = m_root;

	while (p != NIL || !st.empty()) {
		if (p != NIL) {
			st.push(p);
			p = p->lchild;
		}
		else {
			p = st.top();
			if (p->rchild != NIL && p->rchild != r) {
				p = p->rchild;
			}
			else {
				visit(p->key, p->val);
				st.pop();
				r = p;
				p = NIL;
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
template<typename Key, typename Value, typename Compare>
inline void RBTree<Key, Value, Compare>::LevelOrder(void(*visit)(const Key& _Key, const Value& _Val))
{
	queue<RBNode<Key, Value>*> qu;
	RBNode<Key, Value>* p = NULL;
	qu.push(m_root);

	while (!qu.empty()) {
		p = qu.front();
		visit(p->key, p->val);
		qu.pop();

		if (p->lchild != NIL) {
			qu.push(p->lchild);
		}
		if (p->rchild != NIL) {
			qu.push(p->rchild);
		}
	}
}

/*
 * ��ȡ�������е���Сֵ
 *
 * ����ֵ
 *  Value ���������Ϊ�գ�������С��Ԫ��ֵ
 *          �����Ϊ�գ����ж����У���ȡ��Сֵǰ���ȵ��� Empty() �����ж����Ƿ�Ϊ�գ�
 */
template<typename Key, typename Value, typename Compare>
inline Value RBTree<Key, Value, Compare>::Minimum()
{
	RBNode<Key, Value>* p = minimum(m_root);
	if (p == NIL) {
		abort();
	}

	return p->val;
}

/*
 * ��ȡ�������е����ֵ
 *
 * ����ֵ
 *  Value ���������Ϊ�գ���������Ԫ��ֵ
 *          �����Ϊ�գ����ж����У���ȡ���ֵǰ���ȵ��� Empty() �����ж����Ƿ�Ϊ�գ�
 */
template<typename Key, typename Value, typename Compare>
inline Value RBTree<Key, Value, Compare>::Maximum()
{
	RBNode<Key, Value>* p = maximun(m_root);
	if (p == NIL) {
		abort();
	}

	return p->val;
}

/*
 * ��ȡ���Ľ�����
 */
template<typename Key, typename Value, typename Compare>
inline size_t RBTree<Key, Value, Compare>::Count() const
{
	return m_count;
}

/*
 * ��ȡ�������
 */
template<typename Key, typename Value, typename Compare>
size_t RBTree<Key, Value, Compare>::Depth() const
{
	if (m_root == NIL) {
		return 0;
	}

	size_t level = 0;
	size_t n = 0;
	RBNode<Key, Value>* p = NULL;
	queue<RBNode<Key, Value>*> qu;
	qu.push(m_root);

	while (!qu.empty()) {
		n = qu.size();
		++level;

		while (n--) {
			p = qu.front();
			qu.pop();
			if (p->lchild != NIL) {
				qu.push(p->lchild);
			}
			if (p->rchild != NIL) {
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
template<typename Key, typename Value, typename Compare>
inline bool RBTree<Key, Value, Compare>::Empty() const
{
	return m_count == 0 ? true : false;
}

/*
 * ����һ������
 *
 * ����
 *  T �������ٵ����������
 */
template<typename Key, typename Value, typename Compare>
void RBTree<Key, Value, Compare>::destroy(RBNode<Key, Value>* T)
{
	if (T) {
		destroy(T->lchild);
		destroy(T->rchild);
		delete T;
		T = NULL;
	}
}

template<typename Key, typename Value, typename Compare>
inline bool RBTree<Key, Value, Compare>::insert(const Key& _Key, const Value& _Val, bool key_unique)
{
	RBNode<Key, Value>* p = NULL;
	RBNode<Key, Value>* q = NULL;
	RBNode<Key, Value>* newnode = NULL;
	int n = 0;

	// ���ҵ��������Ҷ��λ��
	p = m_root;
	if (key_unique == true) {
		while (p != NIL) {
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
	}
	else {
		while (p != NIL) {
			q = p;
			n = m_cmp(_Key, p->key);
			if (n < 0) {
				p = p->lchild;
			}
			else{
				p = p->rchild;
			}
		}
	}

	newnode = new RBNode<Key, Value>();
	newnode->lchild = NIL;
	newnode->rchild = NIL;
	newnode->key = _Key;
	newnode->val = _Val;
	newnode->parent = q;
	newnode->color = RED;

	if (q == NULL) {
		newnode->color = BLACK;
		newnode->parent = NIL;
		m_root = newnode; // ��Ϊ��
	}
	else {
		if (n < 0) {
			q->lchild = newnode;
		}
		else if (n > 0) {
			q->rchild = newnode;
		}

		insert_fixup(newnode);
	}

	++m_count;
	return true;
}

/*
 * ����Ԫ������Ӧ�Ľ��ָ��
 *
 * ������
 *  _Val �������ҵ�Ԫ��ֵ
 *
 * ����ֵ
 *  BTNode<Key, Value>* ��������ڸ�Ԫ��ֵ���򷵻ض�Ӧ�Ľ��ָ��
 *                                ��������ڸ�Ԫ��ֵ���򷵻� NULL ���� m_nil 
 */
template<typename Key, typename Value, typename Compare>
RBNode<Key, Value>* RBTree<Key, Value, Compare>::search(const Key& _Key)
{
	RBNode<Key, Value>* p = m_root;
	int n = 0;

	while (p != NIL) {
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
template<typename Key, typename Value, typename Compare>
RBNode<Key, Value>* RBTree<Key, Value, Compare>::minimum(RBNode<Key, Value>* T)
{
	RBNode<Key, Value>* p = T;
	if (p && p != NIL) {
		while (p->lchild != NIL) {
			p = p->lchild;
		}
	}

	return p;
}

/*
 * ��ȡ���� T ���ҽ��(���ֵ���)
 */
template<typename Key, typename Value, typename Compare>
RBNode<Key, Value>* RBTree<Key, Value, Compare>::maximun(RBNode<Key, Value>* T)
{
	RBNode<Key, Value>* p = T;
	if (p && p != NIL) {
		while (p->rchild != NIL) {
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
template<typename Key, typename Value, typename Compare>
inline void RBTree<Key, Value, Compare>::insert_fixup(RBNode<Key, Value>* T)
{
	RBNode<Key, Value>* p = T;
	RBNode<Key, Value>* q = NULL;

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
					left_rotate(p);
				}
				// ������������Ǻ�ɫ�ģ��� p ��һ������ 
				p->parent->color = BLACK;
				p->parent->parent->color = RED;
				right_rotate(p->parent->parent);
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
					right_rotate(p);
				}
				p->parent->color = BLACK;
				p->parent->parent->color = RED;
				left_rotate(p->parent->parent);
			}
		}
	}

	// ����2 ���ƻ�
	m_root->color = BLACK;
}

/*
 * ������������
 */
template<typename Key, typename Value, typename Compare>
inline void RBTree<Key, Value, Compare>::left_rotate(RBNode<Key, Value>* T)
{
	RBNode<Key, Value>* p = T;
	RBNode<Key, Value>* q = NULL;

	q = p->rchild;
	p->rchild = q->lchild;

	if (q->lchild != NIL) {
		q->lchild->parent = p;
	}

	q->parent = p->parent;

	if (p->parent == NIL) {
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
template<typename Key, typename Value, typename Compare>
inline void RBTree<Key, Value, Compare>::right_rotate(RBNode<Key, Value>* T)
{
	RBNode<Key, Value>* p = T;
	RBNode<Key, Value>* q = NULL;

	q = p->lchild;
	p->lchild = q->rchild;

	if (q->rchild != NIL) {
		q->rchild->parent = p;
	}

	q->parent = p->parent;

	if (p->parent == NIL) {
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
template<typename Key, typename Value, typename Compare>
inline void RBTree<Key, Value, Compare>::delete_fixup(RBNode<Key, Value>* T)
{
	RBNode<Key, Value>* p = T;
	RBNode<Key, Value>* q = NULL;

	while (p != m_root && p->color == BLACK) {

		if (p == p->parent->lchild) {

			q = p->parent->rchild;
			if (q->color == RED) {
				// ���һ��p ���ֵܽ�� q �Ǻ�ɫ�ġ�ת�� ����� �� ����� �� �����
				q->color = BLACK; // �ֵܽ����Ϊ��ɫ
				p->parent->color = RED;
				left_rotate(p->parent);
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
					right_rotate(q);
					q = p->parent->rchild;
				}

				// ����ģ�p ���ֵܽ���� q �Ǻ�ɫ�ģ��� q ���Һ����Ǻ�ɫ��
				q->color = p->parent->color;
				p->parent->color = BLACK;
				q->rchild->color = BLACK;
				left_rotate(p->parent);
				p = m_root;
			}
		}

		else {
			q = p->parent->lchild;
			if (q->color == RED) {
				q->color = BLACK;
				p->parent->color = RED;
				right_rotate(p->parent);
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
					left_rotate(q);
					q = q->parent->lchild;
				}

				q->color = p->parent->color;
				p->parent->color = BLACK;
				q->lchild->color = BLACK;
				right_rotate(p->parent);
				p = m_root;
			}
		}
	}

	p->color = BLACK;
}

/*
 * ���ж��������ƶ���ʹ������ told �滻���� tnew
 *
 * ����
 *  told �����滻������
 *  tnew ���滻������
 */
template<typename Key, typename Value, typename Compare>
inline void RBTree<Key, Value, Compare>::transplant(RBNode<Key, Value>* told, RBNode<Key, Value>* tnew)
{
	if (told->parent == NIL) {
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

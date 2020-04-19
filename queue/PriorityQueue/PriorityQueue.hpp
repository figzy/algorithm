#pragma once

#include <cstdlib>

const size_t DEFAULT_INIT_SIZE = 32;
const size_t MAX_PREALLOC = 1024 * 1024;

/*
 * ���ȼ�����Ԫ�ض���
 *
 * ��Ա����
 *  key �����ȼ�key
 *  val ��Ԫ�ص�ֵ
 */
template <typename Key, typename Value>
struct ElemType 
{
	Key key;
	Value val;
};

/*
 * ���ȼ�������
 *
 * ģ�����
 *  Key �����ȼ���������
 *  Value ��������Ԫ�ص�ֵ����
 *  Compare �����ȼ�Key�ıȽϺ���ָ�롣
 *            �������ͣ�int Compare(Key v, Key w);
 *            Ҫ�󣺵� v > w�� ���� > 0; �� v = w�� ���� = 0; �� v �� w�� ���� < 0; 
 */
template <typename Key, typename Value, typename Compare>
class PQueue
{
public:
	PQueue(Compare cmp, size_t initlen = DEFAULT_INIT_SIZE);
	virtual ~PQueue();

	virtual void push(const ElemType<Key, Value>& _Elem);
	virtual void pop();
	virtual const ElemType<Key, Value>& front() const;
	virtual bool empty() const;
	virtual size_t size() const;

	virtual bool increase_key(size_t i, const Key& _Key);

protected:
	inline size_t lchild(size_t i);
	inline size_t rchild(size_t i);
	inline size_t parent(size_t i);
	inline void swap(size_t i, size_t j);
	inline void adjust_up(size_t k);
	inline void adjust_down(size_t k);

private:
	Compare compare;
	ElemType<Key, Value>* m_base;// ���ȼ����л�ָ��
	size_t m_heap_cap;// ��ǰ���ٵĿռ�������С
	size_t m_heap_size;// ��ǰ����Ԫ�صĸ���
};

template<typename Key, typename Value, typename Compare>
inline PQueue<Key, Value, Compare>::PQueue(Compare cmp, size_t initlen)
{
	m_base = (ElemType<Key, Value>*)calloc(initlen + 1, sizeof(ElemType<Key, Value>));
	if (!m_base) {
		throw "calloc failed.";
	}

	compare = cmp;
	m_heap_size = 0;
	m_heap_cap = initlen;// 0 ��Ԫ�ز��洢Ԫ��
}

template<typename Key, typename Value, typename Compare>
inline PQueue<Key, Value, Compare>::~PQueue()
{
	if (m_base) {
		delete[] m_base;
	}
}

/*
 * �����ȼ������в���һ��Ԫ��
 *
 * ����
 *  _Elem �������Ԫ��
 */
template<typename Key, typename Value, typename Compare>
inline void PQueue<Key, Value, Compare>::push(const ElemType<Key, Value>& _Elem)
{
	// ����ǰ�������������������
	if (m_heap_size == m_heap_cap) {
		if (m_heap_cap < MAX_PREALLOC) {
			m_heap_cap = m_heap_cap * 2;
		}
		else {
			m_heap_cap = m_heap_cap + MAX_PREALLOC;
		}

		ElemType<Key, Value>* newbase = (ElemType<Key, Value>*)realloc(m_base, (m_heap_cap + 1) * sizeof(ElemType<Key, Value>));
		if (!newbase) {
			throw "realloc failed.";
		}

		m_base = newbase;
	}

	++m_heap_size;
	m_base[m_heap_size] = _Elem;

	adjust_up(m_heap_size);
}

/*
 * �����ȼ������е�������Ԫ��
 */
template<typename Key, typename Value, typename Compare>
inline void PQueue<Key, Value, Compare>::pop()
{
	if (m_heap_size < 1) {
		throw "heap underflow.";
	}

	m_base[1] = m_base[m_heap_size];
	--m_heap_size;

	adjust_down(1);
}

/*
 * ��ȡ����Ԫ�أ����Ԫ�أ�
 */
template<typename Key, typename Value, typename Compare>
inline const ElemType<Key, Value>& PQueue<Key, Value, Compare>::front() const
{
	if (m_heap_size < 1) {
		throw "heap underflow";
	}

	return m_base[1];
}

template<typename Key, typename Value, typename Compare>
inline bool PQueue<Key, Value, Compare>::empty() const
{
	return m_heap_size == 0 ? true : false;
}

template<typename Key, typename Value, typename Compare>
inline size_t PQueue<Key, Value, Compare>::size() const
{
	return m_heap_size;
}

/*
 * �ı�����Ϊ i �����ȼ�
 *
 * ����
 *  i ������ֵ��
 *  _Key �����µ����ȼ�ֵ��
 *
 * ����ֵ
 *  bool ���� i ����������Χ��(index out of range)���� _Key ��ԭ�������ȼ���(new key is smaller than current key),���� false�����򷵻� true
 */
template<typename Key, typename Value, typename Compare>
inline bool PQueue<Key, Value, Compare>::increase_key(size_t i, const Key& _Key)
{
	if (i < 1 || i > m_heap_size) {
		//throw "index out of range.";
		return false;
	}

	if (compare(m_base[i].key, _Key) > 0) {
		//throw "new key is smaller than current key.";
		return false;
	}

	m_base[i].key = _Key;
	adjust_up(i);

	return true;
}

template<typename Key, typename Value, typename Compare>
inline size_t PQueue<Key, Value, Compare>::lchild(size_t i)
{
	return 2 * i;
}

template<typename Key, typename Value, typename Compare>
inline size_t PQueue<Key, Value, Compare>::rchild(size_t i)
{
	return 2 * i + 1;
}

template<typename Key, typename Value, typename Compare>
inline size_t PQueue<Key, Value, Compare>::parent(size_t i)
{
	return i / 2;
}

template<typename Key, typename Value, typename Compare>
inline void PQueue<Key, Value, Compare>::swap(size_t i, size_t j)
{
	ElemType<Key, Value> tmp;
	tmp = m_base[i];
	m_base[i] = m_base[j];
	m_base[j] = tmp;
}

/*
 * ���ϵ�����
 */
template<typename Key, typename Value, typename Compare>
inline void PQueue<Key, Value, Compare>::adjust_up(size_t k)
{
	size_t i;
	m_base[0] = m_base[k];
	i = parent(k);
	while (i > 0 && compare(m_base[0].key, m_base[i].key) > 0) {
		m_base[k] = m_base[i];
		k = i;
		i = parent(i);
	}
	m_base[k] = m_base[0];

	// test
// 	cout << "\n\n keys: ";
// 	for (size_t i = 1; i <= m_heap_size; ++i) {
// 		cout << m_base[i].key << " ";
// 	}
// 	cout << endl;
}

/*
 * ���µ�����
 */
template<typename Key, typename Value, typename Compare>
inline void PQueue<Key, Value, Compare>::adjust_down(size_t k)
{
	m_base[0] = m_base[k];
	size_t i = lchild(k);

	for (i; i <= m_heap_size; i = lchild(i)) {
		if (i < m_heap_size && compare(m_base[i].key, m_base[i + 1].key) < 0) {
			i = i + 1;
		}
		if (compare(m_base[0].key, m_base[i].key) >= 0) {
			break;
		}
		else {
			m_base[k] = m_base[i];
			k = i;
		}
	}

	m_base[k] = m_base[0];
}

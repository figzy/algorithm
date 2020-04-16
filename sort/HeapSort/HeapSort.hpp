#pragma once

#include<iostream>
using namespace std;;

#define DEFAULTSIZE 32
#define INCREASE 16

/*
 * �����㶨��
 */
template <typename Key, typename Value>
struct ElemType {
    Key key;
    Value val;
};

/*
 * �����
 *
 * ģ�����
 *  Key ��ģ��Ԫ�ؽ�� key
 *  Value �� ģ��Ԫ�ؽ�� value
 *  Compare ��Ԫ�صıȽϺ������� v > w ���� > 0; �� v < w ���� < 0; �� v == w ���� = 0
 */
template <typename Key, typename Value, typename Compare>
class Heap
{
public:
    Heap(Compare cmp, int initlen = DEFAULTSIZE);
    virtual ~Heap();
    
	virtual bool InsertUnique(const ElemType<Key, Value>& elem);
    virtual bool InsertEqual(const ElemType<Key, Value>& elem);
    virtual void Delete(const Key& _key);

    virtual int Search(const Key& _key);
    virtual void HeapSort();

	virtual void Clear() { m_len = 0; }
	virtual int Len() const { return m_len; }
	virtual int Cap() const { return m_cap; }

    ElemType<Key, Value>& operator[] (int i) const{
        if (i < 1 || i > m_len) {
            throw "out of range.";
        }
        return m_szBuf[i];
    }

private:
    bool insert(const ElemType<Key, Value>& elem);
    int search(const Key& _key);
    int search_binary(const Key& _key);
    void build_max_heap();
    void adjust_down(int k, int len);
    void adjust_up(int k);
    void swap(int i, int j);

private:
    Compare m_cmpFunc;
    ElemType<Key, Value>* m_szBuf;
    int m_len;
    int m_cap;
    
    // ��ǵ�ǰ�Ƿ��������������Ĳ�ѯ���ö��ַ�����
    bool m_bsort; 
};

template<typename Key, typename Value, typename Compare>
inline Heap<Key, Value, Compare>::Heap(Compare cmp, int initlen)
{
    m_szBuf = new ElemType<Key, Value>[initlen]();
    if (!m_szBuf) {
        throw "init failed";
    }

    m_len = 0;
    m_cap = initlen;
    m_cmpFunc = cmp;
    m_bsort = false;
}

template<typename Key, typename Value, typename Compare>
inline Heap<Key, Value, Compare>::~Heap()
{
    if (m_szBuf) delete[] m_szBuf;
}

/*
 * ����һ��Ԫ�أ���֤ key ֵΨһ
 */
template<typename Key, typename Value, typename Compare>
inline bool Heap<Key, Value, Compare>::InsertUnique(const ElemType<Key, Value>& elem)
{
    if (Search(elem.key)) {
        return false;
    }

    return insert(elem);
}

/*
 * ����һ��Ԫ�أ����� key �ظ�
 */
template<typename Key, typename Value, typename Compare>
inline bool Heap<Key, Value, Compare>::InsertEqual(const ElemType<Key, Value>& elem)
{
    return insert(elem);
}

/*
 * ���� key ɾ��һ��Ԫ��
 *
 * ɾ��ֵ֮���ڶԶѽ��е���
 */
template<typename Key, typename Value, typename Compare>
inline void Heap<Key, Value, Compare>::Delete(const Key& _key)
{
    bool flag = false;

    for (int i = 1; i <= m_len; ++i) {
        if (m_cmpFunc(m_szBuf[i].key, _key) == 0) {
            swap(i, m_len);
            --m_len;
            flag = true;
        }
    }

    // ɾ��Ԫ�غ��ƻ��˶�
    if (flag == true) {
        build_max_heap();
    }
}

/*
 * ���� key ����Ԫ������λ�õ��±�
 */
template<typename Key, typename Value, typename Compare>
inline int Heap<Key, Value, Compare>::Search(const Key& _key)
{
    if (!m_bsort) {
        return search(_key);
    }

    return search_binary(_key);
}

/*
 * ������(����key����������)
 *
 * ����
 *  ʱ�临�Ӷ� ��T = O(log2n)
 *  �ռ临�Ӷ� ��S = O(1)
 *  �ȶ��� �����ȶ�����
 *  ������ ��������˳��洢
 */
template <typename Key, typename Value, typename Compare>
void Heap<Key, Value, Compare>::HeapSort()
{
    // 	ElemType<Key, Value> tmp;
    // 	build_max_heap();
    for (int i = m_len; i > 1; --i) {
        swap(1, i);// ���Ѷ�Ԫ�طŵ���ǰ�������ĩβ
        adjust_down(1, i - 1); // ������
    }
    m_bsort = true;
}

/*
 * ����һ��Ԫ��
 */
template <typename Key, typename Value, typename Compare>
bool Heap<Key, Value, Compare>::insert(const ElemType<Key, Value>& elem)
{
	if (m_len + 1 == m_cap) {
		ElemType<Key, Value>* newbase = new ElemType<Key, Value>[m_cap + INCREASE]();
		if (!newbase) throw "new error.";
		for (int i = 1; i <= m_len; ++i) {
			newbase[i] = m_szBuf[i];
		}
		delete[] m_szBuf;
		m_szBuf = newbase;
		m_cap = m_cap + INCREASE;
	}

	m_len = m_len + 1;
	m_szBuf[m_len] = elem;

	adjust_up(m_len);
    m_bsort = false;

    // test
    for (int i = 1; i <= m_len; i++) {
        cout << m_szBuf[i].key << " ";
    }
    cout << endl;

    return true;
}

/*
 * ˳�����
 */
template<typename Key, typename Value, typename Compare>
inline int Heap<Key, Value, Compare>::search(const Key& _key)
{
    m_szBuf[0].key = _key;
    int i;
    for (i = m_len; m_cmpFunc(m_szBuf[i].key, _key) != 0; --i);
    return i;
}

template <typename Key, typename Value, typename Compare>
int Heap<Key, Value, Compare>::search_binary(const Key& _key)
{
	size_t low = 1, high = m_len, mid;
	int ret;

	while (low <= high) {
		mid = (low + high) / 2;
		ret = m_cmpFunc(m_szBuf[mid].key, _key);
		if (ret == 0) {
			return mid;
		}
		else if (ret > 0) {
			high = mid - 1;
		}
		else {
			low = mid + 1;
		}
	}

	return 0;
}

/*
 * ���������
 */
template<typename Key, typename Value, typename Compare>
inline void Heap<Key, Value, Compare>::build_max_heap()
{
    for (int i = m_len / 2; i > 0; --i) {
        adjust_down(i, m_len);
    }
}

/*
 * ���µ�����
 *
 * ����
 *  k ����������Ԫ�ص��±�
 */
template <typename Key, typename Value, typename Compare>
inline void Heap<Key, Value, Compare>::adjust_down(int k, int len) {
    m_szBuf[0] = m_szBuf[k];

    for (int i = 2 * k; i <= len; i *= 2) {

        if (i < len && m_cmpFunc(m_szBuf[i].key, m_szBuf[i + 1].key) < 0) {
            ++i;
        }

        if (m_cmpFunc(m_szBuf[0].key, m_szBuf[i].key) >= 0) {
            break;
        }
        else {
            m_szBuf[k] = m_szBuf[i];
            k = i;
        }
    }

    m_szBuf[k] = m_szBuf[0];
}

/*
 * ���ϵ�����
 *
 * ����
 *  k ����������Ԫ�ص��±�
 */
template<typename Key, typename Value, typename Compare>
inline void Heap<Key, Value, Compare>::adjust_up(int k)
{
    m_szBuf[0] = m_szBuf[k];
    int i = k / 2;
    while (i > 0 && m_cmpFunc(m_szBuf[i].key, m_szBuf[0].key) < 0) {
        m_szBuf[k] = m_szBuf[i];
        k = i;
        i = k / 2;
    }
    m_szBuf[k] = m_szBuf[0];
}

template<typename Key, typename Value, typename Compare>
inline void Heap<Key, Value, Compare>::swap(int i, int j)
{
    ElemType<Key, Value> tmp;
    tmp = m_szBuf[i];
    m_szBuf[i] = m_szBuf[j];
    m_szBuf[j] = tmp;
}

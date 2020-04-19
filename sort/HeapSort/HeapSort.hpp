#pragma once

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
    Heap(Compare cmp):compare(cmp){}

    virtual bool InsertUnique(ElemType<Key, Value> A[], int& len, int cap, const ElemType<Key, Value>& e);
    virtual bool InsertEqual(ElemType<Key, Value> A[], int& len, int cap, const ElemType<Key, Value>& e);

    virtual void Delete(ElemType<Key, Value> A[], int& len, const Key& _key);

    virtual void HeapSort(ElemType<Key, Value> A[], int len);

    virtual void BuildMaxHeap(ElemType<Key, Value> A[], int len);

private:
    bool insert(ElemType<Key, Value> A[], int& len, int cap, const ElemType<Key, Value>& e);

    void adjust_down(ElemType<Key, Value> A[], int k, int len);
    void adjust_up(ElemType<Key, Value> A[], int k);

    void swap(ElemType<Key, Value> A[], int i, int j);

    int parent(int i) const { return i / 2; }
    int lchild(int i) const { return 2 * i; }
    int rchild(int i) const { return 2 * i + 1; }

private:
    Compare compare;
};

/*
 * ����һ��Ԫ�أ���֤ key ֵΨһ
 */
template<typename Key, typename Value, typename Compare>
inline bool Heap<Key, Value, Compare>::InsertUnique(ElemType<Key, Value> A[], int& len, int cap, const ElemType<Key, Value>& e)
{
    int i;
    A[0] = e;
    for (i = len; compare(A[i].key, e.key) != 0; --i);
    if (i == 0) {
        return false;
    }

    return insert(A, len, cap, e);
}

/*
 * ����һ��Ԫ�أ����� key �ظ�
 */
template<typename Key, typename Value, typename Compare>
inline bool Heap<Key, Value, Compare>::InsertEqual(ElemType<Key, Value> A[], int& len, int cap, const ElemType<Key, Value>& e)
{
    return insert(A, len, cap, e);
}

/*
 * ���� key ɾ��һ��Ԫ��
 *
 * ɾ��ֵ֮���ڶԶѽ��е���
 */
template<typename Key, typename Value, typename Compare>
inline void Heap<Key, Value, Compare>::Delete(ElemType<Key, Value> A[], int& len, const Key& _key)
{
    bool flag = false;

    for (int i = 1; i <= len; ++i) {
        if (compare(A[i].key, _key) == 0) {
            swap(A, i, len);
            --len;
            flag = true;
        }
    }

    // ɾ��Ԫ�غ��ƻ��˶�
    if (flag == true) {
        BuildMaxHeap(A, len);
    }
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
void Heap<Key, Value, Compare>::HeapSort(ElemType<Key, Value> A[], int len)
{
    BuildMaxHeap(A, len);
    for (int i = len; i > 1; --i) {
        swap(A, 1, i);// ���Ѷ�Ԫ�طŵ���ǰ�������ĩβ
        adjust_down(A, 1, i - 1); // ������
    }
}

/*
 * ����һ��Ԫ��
 *
 * ����
 *  A ����Ŷѵ�����
 *  len ����ǰ����Ԫ�ظ���
 *  cap ������A������
 *  e �������Ԫ��
 *
 * ����ֵ
 *  bool ������ɹ������жѵĵ�����������true�����򷵻�false
 */
template <typename Key, typename Value, typename Compare>
bool Heap<Key, Value, Compare>::insert(ElemType<Key, Value> A[], int& len, int cap, const ElemType<Key, Value>& e)
{
    if (len >= cap) {
        return false;
    }

    len = len + 1;
	A[len] = e;
	adjust_up(A, len);
    return true;
}

/*
 * ���������
 */
template<typename Key, typename Value, typename Compare>
inline void Heap<Key, Value, Compare>::BuildMaxHeap(ElemType<Key, Value> A[], int len)
{
    for (int i = len / 2; i > 0; --i) {
        adjust_down(A, i, len);
    }
}

/*
 * ���µ�����
 *
 * ����
 *  k ����������Ԫ�ص��±�
 */
template <typename Key, typename Value, typename Compare>
inline void Heap<Key, Value, Compare>::adjust_down(ElemType<Key, Value> A[], int k, int len) {
    A[0] = A[k];
    int i = lchild(k);

    for (i; i <= len; i = lchild(i)) {
        if (i < len && compare(A[i].key, A[i + 1].key) < 0) {
            ++i;
        }

        if (compare(A[0].key, A[i].key) >= 0) {
            break;
        }
        else {
            A[k] = A[i];
            k = i;
        }
    }

    A[k] = A[0];
}

/*
 * ���ϵ�����
 *
 * ����
 *  k ����������Ԫ�ص��±�
 */
template<typename Key, typename Value, typename Compare>
inline void Heap<Key, Value, Compare>::adjust_up(ElemType<Key, Value> A[], int k)
{
    A[0] = A[k];
    int i = parent(k);
    while (i > 0 && compare(A[i].key, A[0].key) < 0) {
        A[k] = A[i];
        k = i;
        i = parent(k);
    }
    A[k] = A[0];
}

template<typename Key, typename Value, typename Compare>
inline void Heap<Key, Value, Compare>::swap(ElemType<Key, Value> A[], int i, int j)
{
    ElemType<Key, Value> tmp;
    tmp = A[i];
    A[i] = A[j];
    A[j] = tmp;
}

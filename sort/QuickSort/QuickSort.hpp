#pragma once

/*
 * �����㶨��
 */
template <typename Key, typename Value>
struct ElemType {
    Key key;
    Value val;
};

/*
 * ��������(����key����������)
 *
 * ����
 *  buf �������������
 *  len ������ĳ���
 *  compare ��Ԫ�صıȽϺ������� v > w ���� > 0; �� v < w ���� < 0; �� v == w ���� = 0
 *
 * ����
 *  ʱ�临�Ӷ� ��� T = O(N^2)
 *  �ռ临�Ӷ� ��� S = O(N)
 *  �ȶ��� �����ȶ�����
 *  ������ ��������˳��洢
 */
template <typename Key, typename Value>
void QuickSort(ElemType<Key, Value> buf[], int len, int (*compare)(Key v, Key w)) {
    quick_sort(buf, 0, len - 1, compare);
}

template <typename Key, typename Value>
void quick_sort(ElemType<Key, Value> buf[], int low, int high, int (*compare)(Key v, Key w)) {
    if (low < high) {
        int pivotpos = partition(buf, low, high, compare);
        quick_sort(buf, low, pivotpos - 1, compare);
        quick_sort(buf, pivotpos + 1, high, compare);
    }
}

template <typename Key, typename Value>
int partition(ElemType<Key, Value> buf[], int low, int high, int (*compare)(Key v, Key w)) {
    ElemType<Key, Value> pivot = buf[low];
    while (low < high) {
        while (low < high && compare(buf[high].key, pivot.key) >= 0)
            --high;
        buf[low] = buf[high];
        while (low < high && compare(buf[low].key, pivot.key) <= 0)
            ++low;
        buf[high] = buf[low];
    }
    buf[low] = pivot;
    return low;
}

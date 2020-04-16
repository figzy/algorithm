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
 * �鲢����(����key����������)
 *
 * ����
 *  buf �������������
 *  len ������ĳ���
 *  compare ��Ԫ�صıȽϺ������� v > w ���� > 0; �� v < w ���� < 0; �� v == w ���� = 0
 *
 * ����
 *  ʱ�临�Ӷ� ��T = O(nlog2n)
 *  �ռ临�Ӷ� ��S = O(n)
 *  �ȶ��� ���ȶ�����
 *  ������ ��������˳��洢
 */
template <typename Key, typename Value>
void MergeSort(ElemType<Key, Value> buf[], int len, int (*compare)(Key v, Key w)) {
    ElemType<Key, Value>* szTmp = new ElemType<Key, Value>[len + 1]();
    if (!szTmp) return;

    merge_sort(buf, szTmp, 0, len - 1, compare);

    delete szTmp;
}
template <typename Key, typename Value>
void merge_sort(ElemType<Key, Value> buf[], ElemType<Key, Value> szTmp[], int low, int high, int (*compare)(Key v, Key w)) {
    if (low < high) {
        int mid = (low + high) / 2;
        merge_sort(buf, szTmp, low, mid, compare);
        merge_sort(buf, szTmp, mid + 1, high, compare);
        merge(buf, szTmp, low, mid, high, compare);
    }
}
template <typename Key, typename Value>
void merge(ElemType<Key, Value> buf[], ElemType<Key, Value> szTmp[], int low, int mid, int high, int (*compare)(Key v, Key w)) {
    int i, j, k;

    for (k = low; k <= high; ++k) {
        szTmp[k] = buf[k];
    }

    for (i = low, j = mid + 1, k = i; i <= mid && j <= high; ++k) {
        if (compare(szTmp[i].key, szTmp[j].key) < 0) {
            buf[k] = szTmp[i];
            ++i;
        }
        else {
            buf[k] = szTmp[j];
            ++j;
        }
    }

    while (i <= mid) buf[k++] = szTmp[i++];
    while (j <= mid) buf[k++] = szTmp[j++];
}
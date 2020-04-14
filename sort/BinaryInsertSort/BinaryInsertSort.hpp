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
 * �۰�������򣨻���key���������У�
 *
 * ����
 *  buf ������������顣�涨 buf[0] Ϊ�ڱ�λ�����洢����
 *  len ������ĳ���
 *  compare ��Ԫ�صıȽϺ������� v > w ���� > 0; �� v < w ���� < 0; �� v == w ���� = 0
 *
 * ����
 *  ʱ�临�Ӷ� ��T = O(N^2)
 *  �ռ临�Ӷ� ��S = O(1)
 *  �ȶ��� ���ȶ�����
 *  ������ ����������˳��洢
 */
template <typename Key, typename Value>
void BinaryInsertSort(ElemType<Key, Value> buf[], size_t len, int (*compare)(Key v, Key w)) {
    size_t i, j, low, high, mid;

    for (i = 2; i <= len; ++i) {
        if (compare(buf[i].key, buf[i - 1].key) < 0) {
            buf[0] = buf[i];
            low = 1;
            high = i - 1;
            while (low <= high) {
                mid = (low + high) / 2;
                if (compare(buf[mid].key, buf[0].key) > 0) {
                    high = mid - 1;
                }
                else {
                    low = mid + 1;
                }
            }
            for (j = i - 1; j >= high + 1; --j) {
                buf[j + 1] = buf[j];
            }
            buf[high + 1] = buf[0];
        }
    }
}
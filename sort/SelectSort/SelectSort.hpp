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
 * ѡ������(����key����������)
 *
 * ����
 *  buf �������������
 *  len ������ĳ���
 *  compare ��Ԫ�صıȽϺ������� v > w ���� > 0; �� v < w ���� < 0; �� v == w ���� = 0
 *
 * ����
 *  ʱ�临�Ӷ� ��T = O(N^2)
 *  �ռ临�Ӷ� ��S = O(1)
 *  �ȶ��� �����ȶ�����
 *  ������ ��������˳��洢����ʽ�洢
 */
template <typename Key, typename Value>
void SelectSort(ElemType<Key, Value> buf[], int len, int (*compare)(Key v, Key w)) {
    int i, j, min;
    ElemType<Key, Value> tmp;

    for (i = 0; i < len - 1; ++i) {
        min = i;
        for (j = i + 1; j < len; ++j) {
            if (compare(buf[j].key, buf[min].key) < 0) {
                min = j;
            }
		}
        if (min != i) {
			tmp = buf[i];
			buf[i] = buf[min];
			buf[min] = tmp;
		}
    }
}
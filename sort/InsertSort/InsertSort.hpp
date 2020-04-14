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
 * ֱ�Ӳ�������(����key����������)
 *
 * ����
 *  buf ������������顣�涨 buf[0] Ϊ�ڱ�λ�����洢����
 *  len ������ĳ���
 *  compare ��Ԫ�صıȽϺ������� v > w ���� > 0; �� v < w ���� < 0; �� v == w ���� = 0
 * 
 * ����
 *  ʱ�临�Ӷ� ����� T = O(N)�� � T = O(N^2)��ƽ�� T = O(N^2)
 *  �ռ临�Ӷ� ��S = O(1)
 *  �ȶ��� ���ȶ�����
 *  ������ ��������˳��洢����ʽ�洢
 */
template <typename Key, typename Value>
void InsertSort(ElemType<Key, Value> buf[], int len, int (*compare)(Key v, Key w)) {
	int i, j;
	for (i = 2; i <= len; ++i) {
        if (compare(buf[i].key, buf[i - 1].key) < 0) {
            buf[0] = buf[i];
            for (j = i - 1; compare(buf[j].key, buf[0].key) > 0; --j) {
                buf[j + 1] = buf[j];
            }
            buf[j + 1] = buf[0];
        }
	}
}
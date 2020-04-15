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
 * ð������(����key����������)
 *
 * ����
 *  buf �������������
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
void BubbleSort(ElemType<Key, Value> buf[], int len, int (*compare)(Key v, Key w)) {
    int i, j;
    bool flag;
    ElemType<Key, Value> tmp;

    for (i = 0; i < len - 1; ++i) {
        flag = false;
        for (j = len - 1; j > i; --j) {
            if (compare(buf[j].key, buf[j - 1].key) < 0) {
                tmp = buf[j];
                buf[j] = buf[j - 1];
                buf[j - 1] = tmp;
                flag = true;
            } 
		}
        if (!flag) {
			return;
		}
    }
}
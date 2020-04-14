#pragma once

/*
 * �����㶨��
 */
template <typename Key, typename Value>
struct ElemType {
    Key key;
    Value val;
    int next;
};

/*
 * ���������
 *
 * ����
 *  buf ������������顣buf[0]��Ϊ����ͷ��㣬���洢��ʵ�����ݣ�����buf[0].nextָ��key���Ľ���±�
 *  len ������ĳ���
 *  compare ��Ԫ�صıȽϺ������� v > w ���� > 0; �� v < w ���� < 0; �� v == w ���� = 0
 *
 * ������buf���ܽ���������ʣ�ֻ��˳����ʡ�ͨ�� Arrange ��������֮�󣬾Ϳ���˳�������
 */
template <typename Key, typename Value>
void TableInsertSort(ElemType<Key, Value> buf[], size_t len, int (*compare)(Key v, Key w)) {
    if (len < 2) return;

    size_t i, p, q;
    buf[0].next = 1;
    buf[1].next = 0;
    for (i = 2; i <= len; ++i) {
        for (p = 0; buf[p].next != 0; p = buf[p].next) {
            q = buf[p].next;
            if (compare(buf[q].key, buf[i].key) > 0) {
                break;
            }
        }
		buf[i].next = buf[p].next;
		buf[p].next = i;
    }

    Arrange(buf, len);
}

//�������к���
template <typename Key, typename Value>
void Arrange(ElemType<Key, Value> buf[], size_t len) {
    size_t p, q, i;
    ElemType<Key, Value> tmp;

    p = buf[0].next;
    for (i = 1; i < len; ++i) {
        while (p < i) p = buf[p].next;
        q = buf[p].next;
        if (p != i) {
            tmp = buf[p];
            buf[p] = buf[i];
            buf[i] = tmp;
            buf[i].next = p;
        }
        p = q;
    }
}
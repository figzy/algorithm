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
 * 2-·��������
 *
 * ����
 *  buf �������������
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
void TwoWayInsertSort(ElemType<Key, Value> buf[], size_t len, int (*compare)(Key v, Key w)) {
    ElemType<Key, Value>* tmp = new ElemType<Key, Value>[len]();
    size_t front, rear, k, i;
    front = rear = 0;
    tmp[0] = buf[0];

    for (i = 1; i < len; ++i) {
        if (compare(buf[i].key, tmp[front].key) < 0) {// С����Сֵ
            front = (front - 1 + len) % len;
            tmp[front] = buf[i];
        }
        else if (compare(buf[i].key, tmp[rear].key) > 0) {// �������ֵ
            rear = (rear + 1 + len) % len;
            tmp[rear] = buf[i];
        }
        else {// �������ֵ����Сֵ֮��
            k = (rear + 1 + len) % len;
            while (compare(tmp[(k - 1 + len) % len].key, buf[i].key) > 0) {
                tmp[(k + len) % len] = tmp[(k - 1 + len) % len];
                k = (k - 1 + len) % len;
            }
            tmp[(k + len) % len] = buf[i];
            rear = (rear + 1) % len;
        }
    }

    // copy to buf
    for (i = 0; i < len; ++i) {
        buf[i] = tmp[(front + i + len) % len];
    }

    delete[] tmp;
}
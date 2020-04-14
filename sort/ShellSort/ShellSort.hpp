#pragma once
#include <iostream>
using namespace std;
/*
 * �����㶨��
 */
template <typename Key, typename Value>
struct ElemType {
    Key key;
    Value val;
};

/*
 * ϣ������ �� ��С�������򣨻���key���������У�
 *
 * ����
 *  buf ������������顣�涨 buf[0] Ϊ�ڱ�λ�����洢����
 *  len ������ĳ���
 *  compare ��Ԫ�صıȽϺ������� v > w ���� > 0; �� v < w ���� < 0; �� v == w ���� = 0
 *
 * ����
 *  ʱ�临�Ӷ� ���ʱ�临�Ӷ� T = O(N^2)
 *  �ռ临�Ӷ� ��S = O(1)
 *  �ȶ��� �����ȶ�����
 *  ������ ����������˳��洢
 */
template <typename Key, typename Value>
void ShellSort(ElemType<Key, Value> buf[], int len, int (*compare)(Key v, Key w)) {
    int i, j;
    int dk; // ����

    for (dk = len / 2; dk >= 1; dk = dk / 2) {
        for (i = dk + 1; i <= len; ++i) {
            if (compare(buf[i].key, buf[i - dk].key) < 0) {
                buf[0] = buf[i];
                for (j = i - dk; j > 0 && compare(buf[j].key, buf[0].key) > 0; j = j - dk) {
                    buf[j + dk] = buf[j];
                }
                buf[j + dk] = buf[0];
            }
        }
        //test
//         cout << "\ndk = " << dk << ": ";
//         for (i = 1; i <= len; ++i) {
//             cout << buf[i].key << " ";
//         }
    }
}
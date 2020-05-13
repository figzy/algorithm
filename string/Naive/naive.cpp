#include <iostream>
using namespace std;

/*
 * �����ַ���ƥ�䣺������ S ��ȡ�ӵ� i(��ʼֵΪpos) ���ַ��𡢳��Ⱥʹ� T ��ȵ��ִ����봮T�Ƚϣ�
 *  ������򷵻� i������ i ֵ��1��ֱ���� S �в����ںʹ� T ��ȵ��ִ�Ϊֹ��
 */
int Match(char* src, char* dst, int pos = 0) {
	if (pos < 0) {
		return -1;
	}

	int n, m, l, i, j;
	n = strlen(src);
	m = strlen(dst);
	l = n - m;

	for (i = pos; i <= l; ++i) {
		j = 0;
		while (j < m) {
			if (src[i + j] != dst[j]) {
				break;
			}
			++j;
		}
		if (j == m){
			return i;
		}
	}

	return -1;
}

int main() {
	char* src = "acaabc";
	char* dst = "aab";

	cout << Match(src, dst, 1);

	return 0;
}
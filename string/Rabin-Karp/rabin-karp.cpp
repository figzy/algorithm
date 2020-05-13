#include <iostream>
using namespace std;

// primeRK ������ Rabin-Karp �㷨�е�����
const size_t primeRK = 16777619;

bool Match(char* src, char* dst, int len) {
	int i;
	for (i = 0; i < len; i++) {
		if (src[i] != dst[i]) {
			break;
		}
	}
	if (i == len) {
		return true;
	}
	return false;
}

int RabinKarp(char* s, char* sep) {
	size_t sLen = strlen(s);
	size_t sepLen = strlen(sep);
	size_t hashsep, pow, h, sq;
	int j = 0;

	if (sepLen == 0 || sLen < sepLen) {
		return -1;
	}
	
	// ���� sep �� hash ֵ
	hashsep = 0;
	for (int i = 0; i < sepLen; i++) {
		hashsep = hashsep * primeRK + (size_t)(sep[i]);
	}

	pow = 1;
	sq = primeRK;
	for (int i = sepLen; i > 0; i >>= 1) {
		if (i & 1 != 0) { // ������������λ���� 0
			pow *= sq;
		}
		sq *= sq;
	}
	
	// ���� s �г���Ϊ sepLen �� hash ֵ
	h = 0;
	for (int i = 0; i < sepLen; i++) {
		h = h * primeRK + (size_t)(s[i]);
	}
	if (h == hashsep && Match(&s[j], sep, sepLen)) {
		return j;
	}
	j++;

	// ���� s �� hash ֵ���� sep �� hash ֵ���бȽ�
	for (int i = sepLen; i < sLen; i++) {
		// ������һ���ַ��� hash ֵ
		h *= primeRK;
		h += (size_t)(s[i]);

		// ȥ����һ���ַ��� hash ֵ
		h -= pow * (size_t)(s[i - sepLen]);
		
		// ��ʼ�Ƚ�
		if (h == hashsep && Match(&s[j], sep, sepLen)) {
			return j;
		}
		j++;
	}

	return -1;
}

int main() {
	char* src = "hfjdhfijdhfiuebghksdgfy";
	char* dst = "dhfiu";
	cout << RabinKarp(src, dst) << endl;

	return 0;
}
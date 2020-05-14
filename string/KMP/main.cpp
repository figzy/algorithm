#include <iostream>
using namespace std;

void get_next(char* T, int tLen, int* next) {
	int i = -1;
	int j = 0;

	next[j] = i;
	while (j < tLen - 1) {
		if (i == -1 || T[j] == T[i]) {
			++i; 
			++j;
			next[j] = i;
		}
		else {
			i = next[i];
		}
	}
}
/*
 * �ַ���ƥ���㷨KMP
 *
 * ����
 *  S: ����
 *  T: �ִ���ģʽ����
 *  pos: ��ʼƥ���λ��
 */
int Index_KMP(char* S, char* T, int pos = 0) {
	int i, j;
	int* next = NULL;
	int sLen = strlen(S);
	int tLen = strlen(T);
	if (sLen - tLen < pos) {
		return -1;
	}

	next = new int[tLen]();
	get_next(T, tLen, next);
	for (i = pos, j = 0; i < sLen && j < tLen; ) {
		if (j == -1 || S[i] == T[j]) {
			++i;
			++j;
		}
		else {
			j = next[j];
		}
	}
	delete next;

	if (j == tLen) {
		return i - j;
	}
	return -1;
}

int main() {
	char* S = "ababcabcacbab";
	char* T = "abcac";
	cout << Index_KMP(S, T) << endl;
	return 0;
}
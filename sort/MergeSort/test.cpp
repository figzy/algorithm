#include "MergeSort.hpp"
#include <time.h> 
#include <iostream>
using namespace std;

#define MAXSIZE 10

int cmp(int v, int w) {
	return v - w;
}

int main() {
	srand((unsigned)time(NULL));
	ElemType<int, int> elem[MAXSIZE];
	int n;

	cout << "��ʼ��<k,v>��";
	for (int i = 0; i < MAXSIZE; i++) {
		n = rand() % 20 + 1;
		elem[i] = { n , i };
		cout << "<" << n << "," << i << "> ";
	}

	MergeSort(elem, MAXSIZE, cmp);

	cout << "\n\n�����<k,v>��";
	for (int i = 0; i < MAXSIZE; i++) {
		cout << "<" << elem[i].key << "," << elem[i].val << "> ";
	}

	return 0;
}
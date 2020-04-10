#include "DoubleLinkList.hpp"
#include <assert.h>
#include <iostream>
using namespace std;

int main() {
	DuLList<int>* lst = new DuLList<int>();

	assert(lst->Empty());

	for (int i = 130; i >= 101; i--) {
		assert(lst->Insert(1, i));
	}

	assert(!lst->Empty());

	int e;

	assert(lst->Get(1, e));
	cout << "�� 1 ��Ԫ���ǣ�" << e << endl;

	assert(lst->Get(20, e));
	cout << "�� 20 ��Ԫ���ǣ�" << e << endl;

	assert(!lst->Get(40, e));

	assert(lst->Modify(3, 200));
	cout << "�޸ĵ� 3 ��Ԫ�ص�ֵΪ��200" << endl;
	assert(lst->Get(3, e));
	cout << "�� 3 ��Ԫ���ǣ�" << e << endl;

	assert(lst->Delete(3, e));
	cout << "ɾ���� 3 ��Ԫ�أ�" << e << endl;
	assert(lst->Get(3, e));
	cout << "�� 3 ��Ԫ���ǣ�" << e << endl;

	return 0;
}
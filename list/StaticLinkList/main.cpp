#include "StaticLinkList.hpp"
#include <assert.h>
#include <iostream>
using namespace std;

int main() {
	SLList<int>* lst = new SLList<int>();

	assert(lst->Empty());

	for (int i = MAXSIZE; i >= 1; i--) {
		cout << i << " ";
		assert(lst->Insert(1, i));
	}
	cout << endl;

	assert(lst->Insert(1, 133) == false);

	int e;

	assert(lst->Get(1, e));
	cout << "�� 1 ��Ԫ���ǣ�" << e << endl;

	assert(lst->Get(20, e));
	cout << "�� 20 ��Ԫ���ǣ�" << e << endl;

	assert(lst->Get(40, e) == false);

	assert(lst->Modify(3, 200));
	cout << "�޸ĵ� 3 ��Ԫ�ص�ֵΪ��200" << endl;
	assert(lst->Get(3, e));
	cout << "�� 3 ��Ԫ���ǣ�" << e << endl;

	assert(lst->Delete(3, e));
	cout << "ɾ���� 3 ��Ԫ�أ�" << e << endl;

	assert(lst->Insert(3, 133));
	cout << "����� 3 ��Ԫ�أ�133" << endl;

	assert(lst->Get(3, e));
	cout << "�� 3 ��Ԫ���ǣ�" << e << endl;

	return 0;
}
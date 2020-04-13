#include "SeqList.hpp"
#include <assert.h>
#include <iostream>
using namespace std;

typedef int ElemType;
bool Compare(ElemType e1, ElemType e2) {
	if (e1 == e2) {
		return true;
	}
	return false;
}

int main() {
	SeqList<ElemType>* lst = new SeqList<ElemType>();
	assert(lst);

	assert(lst->Empty());
	cout << "len:" << lst->Len() << endl;
	cout << "cap:" << lst->Cap() << endl;

	for (int i = 1; i <= 33; i++) {
		assert(lst->Insert(lst->Len(), i));
	}

	assert(!lst->Empty());
	cout << "����Ԫ�غ�" << endl;
	cout << "len:" << lst->Len() << endl;
	cout << "cap:" << lst->Cap() << endl;

	ElemType e;
	assert(lst->Get(1, e));
	assert(!lst->Get(34, e));
	assert(lst->Get(31, e));

	assert(!lst->Prior(0, e, Compare));
	assert(lst->Prior(6, e, Compare));
	cout << "Ԫ�� 6 ǰ���Ԫ��Ϊ: " << e << endl;

	assert(!lst->Next(33, e, Compare));
	assert(lst->Next(6, e, Compare));
	cout << "Ԫ�� 6 �����Ԫ��Ϊ��" << e << endl;

	cout << "ɾ��Ԫ�أ�";
	int len = lst->Len();
	for (int i = 1; i <= len; i++) {
		assert(lst->Delete(0, e));
		cout << e << " ";
	}

	assert(!lst->Delete(0, e));
	
	delete lst;

	return 0;
}
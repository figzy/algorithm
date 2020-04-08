#include <string>

#include "AdjacencyMatrix.hpp"
void TestUDGraph_Matrix();
void TestDGraph_Matrix();

typedef char VexType;
typedef int EdgeInfo;

typedef BOOL(*F)(VexType v1, VexType v2);
BOOL f(VexType v1, VexType v2) {
	if (v1 == v2) return TRUE;
	return FALSE;
}

int main() {

	TestUDGraph_Matrix();
	//TestDGraph_Matrix();

	return 0;
}

// ��������ͼ
void TestUDGraph_Matrix() {
	MGraph<char, int, string, F> G(f, UDG);
	G.InsertVex('a');
	G.InsertVex('b');
	G.InsertVex('c');
	G.InsertVex('d');

	string edge = "���Ǳߵ���Ϣ";
	G.InsertEdge('a', 'b', &edge);
	G.InsertEdge('a', 'c', &edge);
	G.InsertEdge('a', 'd', &edge);
	G.InsertEdge('b', 'd', &edge);
	G.InsertEdge('c', 'd', &edge);
	G.PrintInfo();
	cout << endl << endl;

	cout << "���� c �������ţ� " << G.LocateVex('c') << endl << endl;

	// ɾ����
	cout << "ɾ���� (a,b)" << endl;
	G.DeleteEdge('a', 'b');
	G.PrintInfo();
	cout << endl << endl;

	// ɾ������
	cout << "ɾ������ c" << endl;
	G.DeleteVex('c');
	G.PrintInfo();
	cout << endl << endl;
}

// ��������ͼ
void TestDGraph_Matrix() {
	MGraph<char, int, int, F> G(f, DN);
	G.InsertVex('a');
	G.InsertVex('b');
	G.InsertVex('c');
	G.InsertVex('d');

	int edge = 1;
	G.InsertEdge('a', 'b', &edge, TRUE);
	G.InsertEdge('b', 'a', &edge, TRUE);
	G.InsertEdge('a', 'c', &edge, TRUE);
	G.InsertEdge('c', 'a', &edge, TRUE);
	G.InsertEdge('a', 'd', &edge, TRUE);
	G.InsertEdge('b', 'd', &edge, TRUE);
	G.InsertEdge('c', 'd', &edge, TRUE);
	G.PrintInfo();
	cout << endl << endl;

	cout << "���� c �������ţ� " << G.LocateVex('c') << endl << endl;

	// ɾ����
	cout << "ɾ���� <a,b>" << endl;
	G.DeleteEdge('a', 'b');
	G.PrintInfo();
	cout << endl << endl;

	// ɾ������ b
	cout << "ɾ������ b" << endl;
	G.DeleteVex('b');
	G.PrintInfo();
	cout << endl << endl;

	// ɾ������ a
	cout << "ɾ������ a" << endl;
	G.DeleteVex('a');
	G.PrintInfo();
	cout << endl << endl;
}

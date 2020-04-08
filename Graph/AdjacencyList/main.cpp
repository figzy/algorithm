#include <string>
#include "AdjacencyList.hpp"

void TestUDGraph_List();
void TestDGraph_List();

typedef char VertexType;
typedef string VertexInfo;
typedef int EdgeInfo;

typedef BOOL(*FuncType)(VertexType v1, VertexType v2);
BOOL compare(VertexType v1, VertexType v2) {
	if (v1 == v2) return TRUE;
	return FALSE;
}

int main() {
	
	//TestUDGraph_List();
	TestDGraph_List();

	return 0;
}

void TestUDGraph_List() {
	ALGraph<VertexType, VertexInfo, EdgeInfo, FuncType> G(compare, UDN);
	G.InsertVex('a');
	G.InsertVex('b');
	G.InsertVex('c');
	G.InsertVex('d');

	G.InsertEdge('a', 'b');
	G.InsertEdge('a', 'c');
	G.InsertEdge('a', 'd');
	G.InsertEdge('b', 'd');
	G.InsertEdge('c', 'd');
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

	// ɾ������
	cout << "ɾ������ a" << endl;
	G.DeleteVex('a');
	G.PrintInfo();
	cout << endl << endl;
}

void TestDGraph_List() {
	ALGraph<VertexType, VertexInfo, EdgeInfo, FuncType> G(compare, DN);
	G.InsertVex('a');
	G.InsertVex('b');
	G.InsertVex('c');
	G.InsertVex('d');

	G.SetVex('a', " ����a�Ķ�����Ϣ ");

	G.InsertEdge('a', 'b');
	G.InsertEdge('a', 'c');
	G.InsertEdge('a', 'd');
	G.InsertEdge('b', 'c');
	G.InsertEdge('b', 'd');
	G.InsertEdge('c', 'd');
	G.InsertEdge('c', 'a');
	G.InsertEdge('d', 'a');

	G.PrintInfo();
	cout << endl << endl;

	// ɾ����
	cout << "ɾ���� <b, d>: " << endl;
	G.DeleteEdge('b', 'd');
	G.PrintInfo();
	cout << endl << endl;

	// ɾ������
	cout << "ɾ������ c" << endl;
	G.DeleteVex('c');
	G.PrintInfo();
	cout << endl << endl;

	// ɾ������
	cout << "ɾ������ d" << endl;
	G.DeleteVex('d');
	G.PrintInfo();
	cout << endl << endl;
}

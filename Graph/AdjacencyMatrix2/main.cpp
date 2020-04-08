#include "AdjacencyMatrix.h"
#include <string>
#include <iostream>
using namespace std;
void Test_Matrix();

int __cdecl main() {
	Test_Matrix();

	return 0;
}

// �ӿڲ���
void Test_Matrix() {
	//MGraph G(UDG);
	MGraph G(DG);

	string a = "a";
	string b = "b";
	string c = "c";
	string d = "d";

	// InsertVex
	G.InsertVex((VexType)a.c_str(), a.length());
	G.InsertVex((VexType)b.c_str(), b.length());
	G.InsertVex((VexType)c.c_str(), c.length());
	G.InsertVex((VexType)d.c_str(), d.length());

	// SetVex
	cout << "SetVex" << endl;
	string vexInfo = "Hello this is VexInfo \0\0 ������";
	G.SetVex((VexType)a.c_str(), a.length(), (VexInfo)vexInfo.c_str(), vexInfo.length(), TRUE);

	// GetVex
	string tmp;
	cout << "GetVex" << endl;
	VexInfo aInfo = G.GetVex((VexType)a.c_str(), a.length());
	tmp = string(aInfo, sdslen(aInfo));
	sdsfree(aInfo);
	cout << tmp << endl;

	VexInfo cInfo = G.GetVex((VexType)c.c_str(), c.length());
	tmp = string(cInfo, sdslen(cInfo));
	cout << tmp << endl;
	sdsfree(cInfo);
	cout << endl << endl;

	// InsertEdge
	G.InsertArc((VexType)a.c_str(), a.length(), (VexType)b.c_str(), b.length(), (ArcInfo)"����(a, b)����Ϣ", strlen("����(a, b)����Ϣ"));
	G.InsertArc((VexType)a.c_str(), a.length(), (VexType)c.c_str(), c.length(), (ArcInfo)"����(a, c)����Ϣ", strlen("����(a, c)����Ϣ"));
	G.InsertArc((VexType)a.c_str(), a.length(), (VexType)d.c_str(), d.length(), (ArcInfo)"����(a, d)����Ϣ", strlen("����(a, d)����Ϣ"));
	G.InsertArc((VexType)b.c_str(), b.length(), (VexType)d.c_str(), d.length(), (ArcInfo)"����(b, d)����Ϣ", strlen("����(b, d)����Ϣ"));
	G.InsertArc((VexType)c.c_str(), c.length(), (VexType)d.c_str(), d.length(), (ArcInfo)"����(c, d)����Ϣ", strlen("����(c, d)����Ϣ"));
	G.PrintInfo();
	cout << endl << endl;

	// GetEdge
	cout << "GetEdge" << endl;
	ArcInfo ad = G.GetArc((VexType)a.c_str(), a.length(), (VexType)d.c_str(), d.length());
	cout << string(ad, sdslen(ad));
	sdsfree(ad);
	cout << endl << endl;

	// DeleteEdge
	cout << "ɾ���� (a,b)" << endl;
	G.DeleteArc((VexType)a.c_str(), a.length(), (VexType)b.c_str(), b.length());
	G.PrintInfo();
	cout << endl << endl;

	// DeleteVex
	cout << "ɾ������ c" << endl;
	G.DeleteVex((VexType)c.c_str(), c.length());
	G.PrintInfo();
	cout << endl << endl;
}
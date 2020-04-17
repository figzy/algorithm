#pragma once

#include "../AdjacencyList/AdjacencyList.hpp"

typedef char VexType;
typedef int VexInfo;
typedef int EdgeInfo;
typedef BOOL(*FuncType)(VexType V1, VexType V2);
BOOL f(VexType V1, VexType V2) {
	if (V1 == V2) {
		return TRUE;
	}
	return FALSE;
}

/*
 * ����Ԫ�صĺ���ָ��
 *
 * ����
 *  index ����������
 *  v ���ڵ�ֵ
 */
typedef void(*VisitProc)(int i, VexType vex);
void DefaultVisitProc(int i, VexType vex) {
	cout << "����������" << i << "\t���㣺" << vex << endl;
}

/*
 * �̳����ڽӾ����ͼ��
 */
class CGraph : public ALGraph < VexType, VexInfo, EdgeInfo, FuncType >
{
public:
	CGraph() :ALGraph(f, DN) {}
	~CGraph() {}

	bool TopologicalSort(VisitProc visit);
};

/*
 * ��������
 *
 * ����
 *  visit �����������з��ʽ��ĺ���ָ��
 *
 * ����ֵ
 *  bool �������ͼ�ܽ��У����� true�����򷵻� false
 */
bool CGraph::TopologicalSort(VisitProc visit)
{
	int vexnum = GetVexNum();
	if (vexnum <= 0) {
		return false;
	}

	int* indegree = new int[vexnum]();
	int* stack = new int[vexnum]();
	int top = -1;
	int count = 0;
	int i = 0, j = 0;	
	const VNode<VexType, VexInfo, EdgeInfo>* szVex = GetAdjList();
	EdgeNode<EdgeInfo>* p = NULL;

	// ��ʼ��ÿ����������
	for (i = 0; i < vexnum; ++i) {
		indegree[i] = 0;
	}
	for (i = 0; i < vexnum; i++) {
		p = szVex[i].firstarc;
		while (p != NULL) {
			indegree[p->adjvex]++;
			p = p->nextarc;
		}
	}

	// ��ʼ��ջ�������Ϊ 0 �Ķ�����ջ
	for (i = 0; i < vexnum; ++i) {
		if (indegree[i] == 0) {
			stack[++top] = i;
		}
	}
	
	while (top != -1) {
		i = stack[top--];
		visit(i, szVex[i].val);
		count++;

		for (p = szVex[i].firstarc; p != NULL; p = p->nextarc) {
			i = p->adjvex;
			if (!(--indegree[i])) {
				stack[++top] = i;
			}
		}
	}

	delete[] indegree;
	delete[] stack;

	if (count < vexnum) {
		return false;
	}

	return true;
}
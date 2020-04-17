#ifndef __BFS_H__
#define __BFS_H__

#include "../AdjacencyMatrix/AdjacencyMatrix.hpp"
#include <queue>
#include <iostream>
using namespace std;

#define INFINITY_DISTANCE -1

typedef char VexType;
typedef int VexInfo;
typedef int EdgeType;
typedef BOOL(*FuncType)(VexType v1, VexType v2);

BOOL f(VexType v1, VexType v2) {
	if (v1 == v2) return TRUE;
	return FALSE;
}

/*
 * ����Ԫ�صĺ���ָ��
 *
 * ����
 *  index ����������
 *  v ���ڵ�ֵ
 */
typedef void(*VisitProc)(int index, VexType v);
void DefaultVisitProc(int index, VexType v) {
	cout << index << " " << v << endl;
}

class CGraph:public MGraph<VexType, VexInfo, EdgeType, FuncType>
{
public:
	CGraph(GraphKind kind = UDG);
	~CGraph();

	void BFSTraverse(VisitProc visit = DefaultVisitProc);
	void BFSMinDistance(VexType vex);

private:
	void BFS(int v, VisitProc visit);
	void Clear();

private:
	bool m_visited[MAX_VERTEX_NUM];// ���ʱ������
	queue<int> m_queue;// ��������
};

inline CGraph::CGraph(GraphKind kind /*= UDG*/) : MGraph(f, kind)
{
	memset(m_visited, 0, sizeof(bool) * MAX_VERTEX_NUM);
}

CGraph::~CGraph() 
{

}

/*
 * ������ȱ���
 */
void CGraph::BFSTraverse(VisitProc visit /*= DefaultVisitProc*/)
{
	Clear();
	int i = 0;

	for (i = 0; i < GetVexNum(); ++i) {
		m_visited[i] = false;
	}

	for (i = 0; i < GetVexNum(); ++i) {
		if (!(m_visited[i])) {
			BFS(i, visit);
		}
	}
}

/*
 * ʹ�� BFS ��ⵥԴ���·��(���÷Ǵ�Ȩͼ)
 *
 * ����
 *  vex ��·���Ŀ�ʼ����
 *
 * ����
 *  ���������·��ֱ�ӻ��ں����ڽ��д�ӡ���
 */
void CGraph::BFSMinDistance(VexType vex)
{
	int vexnum = GetVexNum();
	int u = this->LocateVex(vex);

	if (u < 0 || u >= vexnum) {
		return;
	}

	Clear();

	int* szDistance = new int[vexnum]();

	for (int i = 0; i < vexnum; ++i) {
		szDistance[i] = INFINITY_DISTANCE;
	}

	m_visited[u] = true;
	szDistance[u] = 0;
	m_queue.push(u);
	int v;

	while (!m_queue.empty()) {

		v = m_queue.front();
		m_queue.pop();
		for (int w = FirstAdjVexByIndex(v); w >= 0; w = NextAdjVexByIndex(v, w)) {

			if (!m_visited[w]) {
				m_visited[w] = true;
				szDistance[w] = szDistance[v] + 1;
				m_queue.push(w);
			}
		}
	}

	// �����ӡ
	cout << "��� index = " << u << " �Ķ��� " << this->GetVexVal(u) << " ������������ľ���Ϊ��" << endl;
	for (int i = 0; i < vexnum; ++i) {
		cout << "��������Ϊ " << i << " �Ķ��� " << this->GetVexVal(i) << " �ľ���Ϊ��" << szDistance[i] << endl;
	}
}

void CGraph::BFS(int v, VisitProc visit)
{
	visit(v, this->GetVexVal(v));
	m_visited[v] = true;
	m_queue.push(v);

	while (!m_queue.empty()) {

		v = m_queue.front();
		m_queue.pop();

		for (int w = FirstAdjVexByIndex(v); w >= 0; w = NextAdjVexByIndex(v, w)) {

			if (!m_visited[w]) {
				visit(w, this->GetVexVal(w));
				m_visited[w] = true;
				m_queue.push(w);
			}
		}
	}
}

inline void CGraph::Clear()
{
	int n = m_queue.size();
	while (n > 0) {
		m_queue.pop();
		--n;
	}

	memset(m_visited, 0, sizeof(bool) * MAX_VERTEX_NUM);
}

#endif
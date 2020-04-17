#ifndef __DFS_H__
#define __DFS_H__

#include "../AdjacencyList/AdjacencyList.hpp"

typedef char VexType;
typedef int VexInfo;
typedef int EdgeInfo;

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
typedef void(*VisitProc)(int i, VexType vex);
void DefaultVisitProc(int i, VexType vex) {
	cout << i << " " << vex << endl;
}

/*
 * �̳����ڽӱ��ͼ��
 */
class CGraph :public ALGraph<VexType, VexInfo, EdgeInfo, FuncType>
{
public:
	CGraph(GraphKind kind = DG):ALGraph(f, kind) {}
	~CGraph() {}

	void DFSTraverse(VisitProc visit = DefaultVisitProc);

private:
	void DFS(int i, VisitProc visit);
	void Clear();

private:
	bool m_visited[MAX_VERTEX_NUM];// ���ʱ������
};                                                                            

/*
 * ͼ�������������
 */
void CGraph::DFSTraverse(VisitProc visit)
{
	Clear();

	int i = 0;
	int vexnum = this->GetVexNum();

	for (i = 0; i < vexnum; ++i) {
		m_visited[i] = false;
	}

	for (i = 0; i < vexnum; ++i) {
		if (!(m_visited[i])) {
			DFS(i, visit);
		}
	}
}

void CGraph::DFS(int i, VisitProc visit)
{
	visit(i, this->GetVexVal(i));
	m_visited[i] = TRUE;
	for (int w = this->FirstAdjVexByIndex(i); w >= 0; w = this->NextAdjVexByIndex(i, w)) {
		if (!m_visited[w]) {
			DFS(w, visit);
		}
	}
}

void CGraph::Clear() {
	memset(m_visited, 0, sizeof(bool) * MAX_VERTEX_NUM);
}

#endif // !__DFS_H__
#ifndef __PRIM_H__
#define __PRIM_H__

#include "../AdjacencyMatrix/AdjacencyMatrix.hpp"
#include <string>

/***************** ��С�������㷨��Prim *****************/

#define INFINITY_DISTANCE (~(1 << (sizeof(int) * 8 - 1)))

typedef string VexType;
typedef int VexInfo;
typedef int EdgeInfo;
typedef BOOL(*FuncType)(VexType v1, VexType v2);
BOOL f(VexType v1, VexType v2) {
	if (0 == v1.compare(v2)) {
		return TRUE;
	}
	return FALSE;
}

// ���ﶥ��v����С����
struct LowCost {
	// ��СȨֵ�ı�(from, to). 
	// j Ϊ��һ�������������j Ϊ��ǰ���������
	int i, j;

	// i �� j ��Ӧ�ߵ�Ȩֵ
	int weight; 
};

// ����·���Ľṹ��
struct Path {
	int i;
	VexType v;
	int weight;
	struct Path* next;
};


// ͼ��
class CGraph :public MGraph<VexType, VexInfo, EdgeInfo, FuncType>
{
public:
	CGraph():MGraph(f, UDG) {}
	~CGraph() {}

	// �����ڴ�Ȩ����ͼ UDG
	Path* MSTPrim(VexType vex, VexType vexEnd);
};


Path* CGraph::MSTPrim(VexType vexBegin, VexType vexEnd)
{
	int i = 0, j = 0;
	int vexnum = GetVexNum();
	int nBeginVexIndex = LocateVex(vexBegin);
	int nEndVexIndex = LocateVex(vexEnd);
	int nMinArcWeight = 0;
	int nMinVexIndex = 0;
	BOOL* szVexSet = NULL;// �����Ƿ񱻷��ʡ�true���Ѿ������ʣ�false��δ������
	LowCost* szLowcost = NULL;
	LowCost lc;
	EdgeInfo weight;// ����Ϣ
	BOOL bRet = FALSE;
	
	if (nBeginVexIndex < 0 || nBeginVexIndex >= vexnum || nEndVexIndex < 0 ||nEndVexIndex >= vexnum) {
		return NULL;
	}

	szLowcost = new LowCost[vexnum]();
	szVexSet = new BOOL[vexnum]();

	// ��ʼ������ v ���������� i ��·������
	for (i = 0; i < vexnum; ++i) {
		memset(&lc, 0, sizeof(LowCost));

		lc.i = nBeginVexIndex;
		lc.j = i;
		if (i == nBeginVexIndex) {
			lc.weight = 0;
		}
		else {
			bRet = GetEdgeByIndex(nBeginVexIndex, i, weight);
			if (bRet == FALSE) {
				lc.weight = INFINITY_DISTANCE;
			}
			else {
				lc.weight = weight;
			}
		}

		szVexSet[i] = FALSE;
		szLowcost[i] = lc;
	}

	szVexSet[nBeginVexIndex] = TRUE;

	// �ҳ���С��
	for (i = 0; i < vexnum - 1; ++i) {
		nMinArcWeight = INFINITY_DISTANCE;
		
		// �ҳ���С�ߵĿ��� min_arc������ k ��¼��С��������Ӧ�Ķ������� j
		for (j = 0; j < vexnum; ++j) {
			if (szVexSet[j] == FALSE && szLowcost[j].weight < nMinArcWeight) {
				nMinArcWeight = szLowcost[j].weight;
				nMinVexIndex = j;
			}
		}

		// ��¼ min_vex Ϊ�ѷ���״̬
		szVexSet[nMinVexIndex] = TRUE;

		// ���ҵ�Ŀ�궥��
		if (nMinVexIndex == nEndVexIndex) {
			break;
		}
		
		// ���� min_vex ������δ���ʶ������С·������
		for (j = 0; j < vexnum; ++j) {

			if (szVexSet[j] == TRUE) {
				continue;
			}

			bRet = GetEdgeByIndex(nMinVexIndex, j, weight);

			if (bRet == FALSE) {
				continue;
			}
			else {
				if (weight < szLowcost[j].weight) { 
					memset(&lc, 0, sizeof(LowCost));
					lc.i = nMinVexIndex;
					lc.j = j;
					lc.weight = weight;
					szLowcost[j] = lc;
				}
			}
		}
	}

	// ��¼��̾���·��
	Path* p = NULL;
	Path* path = NULL;
	i = nEndVexIndex;
	j = 0;

	path = new Path;
	path->i = i;
	path->v = GetVexVal(i);
	path->weight = szLowcost[i].weight;
	path->next = p;
	p = path;

	while (i != nBeginVexIndex && j < vexnum) {
		i = szLowcost[i].i;
		j++;

		path = new Path;
		path->i = i;
		path->v = GetVexVal(i);
		if (i == nBeginVexIndex) {
			path->weight = 0;
		}
		else {
			path->weight = szLowcost[i].weight;
		}
		path->next = p;
		p = path;
	}

	delete[] szLowcost;
	delete[] szVexSet;

	// ��������֮��û��·��
	if (i != nBeginVexIndex) {
		while (p != NULL) {
			path = p;
			p->next;
			delete path;
		}

		return NULL;
	}

	return p;
}

#endif // !__PRIM_H__

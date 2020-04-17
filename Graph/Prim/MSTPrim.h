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

/*
 * ����֮����С���۽ṹ��
 *
 * ��Ա����
 *  i, j ��i Ϊ��һ�������������j Ϊ��ǰ���������
 *  weight ��i �� j ��Ӧ�ߵ�Ȩֵ
 */
struct LowCost {
	int i, j;
	int weight; 
};

/*
 * �̳����ڽӾ����ͼ��
 */
class CGraph :public MGraph<VexType, VexInfo, EdgeInfo, FuncType>
{
public:
	CGraph():MGraph(f, UDG) {}
	~CGraph() {}

	LowCost* MSTPrim(VexType v, int& sum);
};

/*
 * Prim ����С�������㷨�������ڴ�Ȩ����ͼ UDG��
 *
 * ����
 *  v ����Ϊ��С�������ĸ��ڵ�Ķ���
 *  sum ����С������������Ȩֵ�ĺ�
 *
 * ����ֵ
 *  LowCost ����С������
 */
LowCost* CGraph::MSTPrim(VexType v, int& sum)
{
	int i = 0, j = 0;
	int vexnum = GetVexNum();
	int nVexIndex = LocateVex(v);
	int nMinArcWeight = 0;
	int nMinVexIndex = 0;
	BOOL* szVexSet = NULL;// �����Ƿ񱻷��ʡ�true���Ѿ������ʣ�false��δ������
	LowCost* szLowcost = NULL;
	LowCost lc;
	EdgeInfo weight;// ����Ϣ
	BOOL bRet = FALSE;
	sum = 0;
	
	if (nVexIndex < 0 || nVexIndex >= vexnum) {
		return NULL;
	}

	szLowcost = new LowCost[vexnum]();
	szVexSet = new BOOL[vexnum]();

	// ��ʼ������ v ���������� i ��·������
	for (i = 0; i < vexnum; ++i) {
		memset(&lc, 0, sizeof(LowCost));

		lc.i = nVexIndex;
		lc.j = i;
		if (i == nVexIndex) {
			lc.weight = 0;
		}
		else {
			bRet = GetEdgeByIndex(nVexIndex, i, weight);
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

	szVexSet[nVexIndex] = TRUE;

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
		sum = sum + nMinArcWeight;

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

	delete[] szVexSet;

	return szLowcost;
}

#endif // !__PRIM_H__

#ifndef __ADJACENCYMATRIX_H__
#define __ADJACENCYMATRIX_H__

#include <memory.h>
#include <iostream>
using namespace std;

#define MAX_VERTEX_NUM 64

enum GraphKind {
	// ����ͼ����Ȩֵ��
	DG,

	// ������
	DN,

	// ����ͼ����Ȩֵ��
	UDG,

	// ������
	UDN,
};

typedef int BOOL;
#define TRUE 1
#define FALSE 0

#define EXIST 0x1
#define NOTEXIST 0x0

#define VALID 0x1
#define INVALID 0x0

/*
 * �߱�ڵ�
*/
template<typename EdgeInfo>
struct EdgeNode
{
	// �ñߴ��� exist = EXIST; �������� exist = NOTEXIST
	// ��ͼΪ DG/UDG,���ڱߴ��ڵ�����£�info��Ϊ�ա���֮Ϊ DN/UDN ������Ȩֵ
	unsigned int exist : 1;

	// �ñߵ������Ϣ
	EdgeInfo info;
};

/*
 * ����ڵ�
 */
template<typename VertexType, typename VertexInfo>
struct VNode
{
	// ����ֵ
	VertexType val;

	// ������Ϣ
	VertexInfo info;

	// info �м�¼ʱ valid = VALID; û��¼ valid = INVALID
	unsigned int valid : 1;
};

//
// CompareFunc ��д��ʽ
//
// typedef char VexType;
// typedef BOOL(*CompareFuncType)(VexType v1, VexType v2);
// BOOL DefaultCompare(VexType v1, VexType v2) {
// 	if (v1 == v2) {
// 		return TRUE;
// 	}
// 	return FALSE;
// }

template<typename VertexType, typename VertexInfo, typename EdgeInfo, typename CompareFunc>
class MGraph
{
public:
	MGraph(CompareFunc func, GraphKind typ = UDG);
	virtual ~MGraph();

	// ��ͼG�в��붥��, ���ص�ǰ�����������
	virtual int InsertVex(const VertexType v);

	// ���ö�����Ϣ. ������㲻���ڣ��򷵻�FALSE�������¼������Ϣ
	// modify �ֶ�: = TRUE,�򵱶�����Ϣ����ʱ�����޸Ķ�����Ϣ�������ڶ�����Ϣ���ڵ�����£�ֱ�ӷ��� FALSE
	virtual BOOL SetVex(const VertexType v, const VertexInfo info, BOOL modify = TRUE);
	virtual BOOL SetVexByIndex(int i, const VertexInfo info, BOOL modify = TRUE);
	
	// ��ȡ������Ϣ
	virtual BOOL GetVex(const VertexType v, VertexInfo& info);
	virtual BOOL GetVexByIndex(int i, VertexInfo& info);

	// ��ͼG��ɾ������
	virtual BOOL DeleteVex(const VertexType v);
	virtual BOOL DeleteVexByIndex(const int loc);

	// �������(x, y)�������<x, y>�����ڣ�����ͼG����Ӹñߡ�
	// ��� modify == true�����ڴ��� Edge ������½����޸ĸñߵĻ���Ϣ����� modify Ϊ false���ڱ��Ѿ����ڵ�����»᷵�� FALSE
	virtual BOOL InsertEdge(VertexType v, VertexType w, EdgeInfo* pInfo = NULL, BOOL modify = TRUE);
	virtual BOOL InsertEdgeByIndex(int i, int j, EdgeInfo* pInfo = NULL, BOOL modify = TRUE);

	// �������(x, y)�������<x, y>���ڣ����ͼG��ɾ���ñߡ����߲������򷵻� FALSE�����óɹ����� TRUE��
	virtual BOOL DeleteEdge(VertexType v, VertexType w);
	virtual BOOL DeleteEdgeByIndex(int i, int j);

	// ����ͼG�б�(x, y)��<x, y>��Ӧ��Ȩֵ value�����߲������򷵻� FALSE�����óɹ����� TRUE��
	virtual BOOL SetEdge(VertexType v, VertexType w, EdgeInfo* pInfo);
	virtual BOOL SetEdgeByIndex(int i, int j, EdgeInfo* pInfo);

	// ��ȡͼG�б�(x, y)��<x, y>��Ӧ��Ȩֵ��
	virtual BOOL GetEdge(VertexType v, VertexType w, EdgeInfo& info);
	virtual BOOL GetEdgeByIndex(int i, int j, EdgeInfo& info);

	// �Ƿ���ڱߣ�v, w���� <v, w>
	virtual BOOL ExistEdge(VertexType v, VertexType w);
	virtual BOOL ExistEdgeByIndex(int i, int j);

	// ��ͼG�ж��� x �ĵ�һ���ڽӵ㡣���У��򷵻ض���ţ��� x û���ڽӵ�򲻴��� x���򷵻�-1��
	virtual int FirstAdjVex(VertexType v);
	virtual int FirstAdjVexByIndex(int i);

	// ����ͼG�ж��� y �Ƕ��� x ��һ���ڽӵ㣬���س� y ֮�ⶥ�� x ����һ���ڽӵ�Ķ���ţ��� y �� x �����һ���ڽӵ㣬�򷵻� -1��
	virtual int NextAdjVex(VertexType v, VertexType w);
	virtual int NextAdjVexByIndex(int i, int j);

	// ��ͼ�д��ڶ��� value,�򷵻ظö�����ͼ��λ�ã����򷵻�-1
	virtual int LocateVex(const VertexType v);

	// ��ͼ�д��ڶ��� value���򷵻� TRUE�����򷵻� FALSE
	virtual BOOL ExistVex(const VertexType v);
	virtual BOOL ExistVexByIndex(const int i);

	// ��ȡ����Ϊ i �Ķ�����Ϣ
	virtual VertexType GetVexVal(int i) const { return m_szVex[i].val; }

	// ��ȡ��ǰͼ������
	virtual GraphKind GetGraphKind() const { return m_graphKind; }

	// ��ȡ��ǰ��������
	virtual int GetVexNum() const { return m_vexnum; }

	// ��ӡ����
	virtual void PrintInfo();

private:
	// ����Ĭ�Ϲ��캯��
	MGraph() {}

	// ����ͼ�бߵ�ƫ�Ƶ�ַ
	virtual int EdgeOffsetLoc(VertexType v, VertexType w);
	virtual int EdgeOffsetLocByIndex(int i, int j);

private:
	// �����
	VNode<VertexType, VertexInfo> m_szVex[MAX_VERTEX_NUM];
	
	// ����ͼ�ڽӾ���
	EdgeNode<EdgeInfo>** m_pDArcs;

	// ����ͼ,����������ѹ���洢
	EdgeNode<EdgeInfo>* m_pUDArcs;

	// ͼ�ĵ�ǰ������
	int m_vexnum;

	// ͼ�Ļ���
	int m_arcnum;

	// ͼ���ͣ�����ͼ �� ����ͼ
	GraphKind m_graphKind;

	// ����ıȽϺ���
	CompareFunc compare;
};


template<typename VertexType, typename VertexInfo, typename EdgeInfo, typename CompareFunc>
MGraph<VertexType, VertexInfo, EdgeInfo, CompareFunc>::MGraph(CompareFunc func, GraphKind kind)
{
	if (kind == DG || kind == DN) {

		// ��������ͼ�����������ڽӾ���
		m_pDArcs = new EdgeNode<EdgeInfo> * [MAX_VERTEX_NUM]();
		for (int i = 0; i < MAX_VERTEX_NUM; i++) {
			m_pDArcs[i] = new EdgeNode<EdgeInfo>[MAX_VERTEX_NUM]();
		}
		m_pUDArcs = NULL;
	}
	else if(kind == UDG || kind == UDN){

		// ��������ͼ����������������ѹ������
		int nUDSize = (1 + MAX_VERTEX_NUM) * MAX_VERTEX_NUM / 2;
		m_pUDArcs = new EdgeNode<EdgeInfo>[nUDSize]();
		m_pDArcs = NULL;
	}

	//memset(m_szVex, 0, sizeof(VNode<VertexType, VertexInfo>) * MAX_VERTEX_NUM);
	m_vexnum = 0;
	m_arcnum = 0;
	m_graphKind = kind;

	compare = func;
}

template<typename VertexType, typename VertexInfo, typename EdgeInfo, typename CompareFunc>
MGraph<VertexType, VertexInfo, EdgeInfo, CompareFunc>::~MGraph()
{
	if (m_pDArcs) {
		delete[] m_pDArcs;
		m_pDArcs = NULL;
	}

	if (m_pUDArcs) {
		delete[] m_pUDArcs;
		m_pUDArcs = NULL;
	}
}

template<typename VertexType, typename VertexInfo, typename EdgeInfo, typename CompareFunc>
int MGraph<VertexType, VertexInfo, EdgeInfo, CompareFunc>::InsertVex(const VertexType v)
{
	int loc = -1;

	do 
	{
		if (m_vexnum >= MAX_VERTEX_NUM) {
			break;
		}

		if (LocateVex(v) != -1) {
			break;
		}

		VNode<VertexType, VertexInfo> node;
		node.val = v;
		node.valid = INVALID;
		
		m_szVex[m_vexnum] = node;
		
		loc = m_vexnum++;

	} while (0);

	return loc;
}

template<typename VertexType, typename VertexInfo, typename EdgeInfo, typename CompareFunc>
inline BOOL MGraph<VertexType, VertexInfo, EdgeInfo, CompareFunc>::SetVex(const VertexType v, const VertexInfo info, BOOL modify /*=TRUE*/)
{
	return SetVexByIndex(LocateVex(v), info, TRUE);
}

template<typename VertexType, typename VertexInfo, typename EdgeInfo, typename CompareFunc>
inline BOOL MGraph<VertexType, VertexInfo, EdgeInfo, CompareFunc>::SetVexByIndex(int i, const VertexInfo info, BOOL modify /*=TRUE*/)
{
	if (i < 0 || i >= m_vexnum) {
		return FALSE;
	}

	if (m_szVex[i].valid == VALID && modify == FALSE){
		return FALSE;
	}

	m_szVex[i].info = info;
	m_szVex[i].valid = VALID;

	return TRUE;
}

template<typename VertexType, typename VertexInfo, typename EdgeInfo, typename CompareFunc>
inline BOOL MGraph<VertexType, VertexInfo, EdgeInfo, CompareFunc>::GetVex(const VertexType v, VertexInfo& info)
{
	return GetVexByIndex(LocateVex(v), info);
}

template<typename VertexType, typename VertexInfo, typename EdgeInfo, typename CompareFunc>
inline BOOL MGraph<VertexType, VertexInfo, EdgeInfo, CompareFunc>::GetVexByIndex(int i, VertexInfo& info)
{
	if (i < 0 || i >= m_vexnum) {
		return FALSE;
	}

	if (m_szVex[i].valid == INVALID) {
		return FALSE;
	}

	info = m_szVex[i].info;

	return TRUE;
}

template<typename VertexType, typename VertexInfo, typename EdgeInfo, typename CompareFunc>
BOOL MGraph<VertexType, VertexInfo, EdgeInfo, CompareFunc>::DeleteVex(const VertexType v)
{
	return DeleteVexByIndex(LocateVex(v));
}

template<typename VertexType, typename VertexInfo, typename EdgeInfo, typename CompareFunc>
inline BOOL MGraph<VertexType, VertexInfo, EdgeInfo, CompareFunc>::DeleteVexByIndex(const int loc)
{
	if (loc < 0 || loc >= m_vexnum) {
		return FALSE;
	}

	// ɾ�� m_szVex ������Ϊ loc �Ķ���
	for (int j = loc; j < m_vexnum; ++j) {
		m_szVex[j] = m_szVex[j + 1];
	}

	int l; // ��
	int c; // ��
	// ɾ��ͼ������Ϊ loc �Ķ�������صı�
	/*
	��Ϊ����ͼ

		 ���磺ɾ������ͼ������Ϊ2�Ķ���
		   0 1 2 3 4
		 0 0 1 1 0 1
		 1 1 0 0 0 0
		 2 1 1 0 1 0
		 3 1 1 0 0 0
		 4 0 0 1 1 0

		 ɾ����Ľ����*��Ϊ��Ҫɾ����λ�ã��� loc = 2
		   0 1 2 3 4
		 0 0 1 * 0 1 // l < loc
		 1 1 0 * 0 0
		 2 * * * * * // l == loc
		 3 1 1 * 0 0 // l > loc
		 4 0 0 * 1 0
	*/
	if (m_graphKind == DG || m_graphKind == DN) {
		for (l = 0; l < m_vexnum; ++l) {
			if (l < loc) {

				if (m_pDArcs[l][loc].exist == EXIST) {
					--m_arcnum;
				}

				for (c = loc; c < m_vexnum - 1; ++c) {
					m_pDArcs[l][c] = m_pDArcs[l][c + 1];
				}
			}
			else if (l == loc) {

				for (c = 0; c < m_vexnum; ++c) {
					if (m_pDArcs[l][c].exist == EXIST) {
						--m_arcnum;
					}
				}

				continue;
			}
			else if (l > loc) {

				for (c = 0; c < m_vexnum; ++c) {
					if (c < loc) {
						m_pDArcs[l - 1][c] = m_pDArcs[l][c];
					}
					else if (c == loc) {
						if (m_pDArcs[l][c].exist == EXIST) {
							--m_arcnum;
						}
					}
					else {
						m_pDArcs[l - 1][c - 1] = m_pDArcs[l][c];
					}
				}
			}
		}
	}
	/*
	��Ϊ����ͼ��ѹ���������Ǿ���

			 0 ----- 1
			 | \     |
			 2  4    |
			 |   \  /
			 |____3

		 ���磺ɾ������ͼ������Ϊ 2 �Ķ���
		   0 1 2 3 4
		 0 0
		 1 1 0
		 2 1 0 0
		 3 0 1 1 0
		 4 1 0 0 1 0

		 ɾ����Ľ����*��Ϊ��Ҫɾ����λ�ã���
		   0 1 2 3 4
		 0 0
		 1 1 0
		 2 * * *
		 3 0 1 * 0
		 4 1 0 * 1 0
	*/
	else if (m_graphKind == UDG || m_graphKind == UDN) {

		if (loc == m_vexnum - 1) {
			// ���һ�У��������κβ���
		}
		else {
			int usedSpace = (m_vexnum + 1) * m_vexnum / 2;// �Ѿ�ʹ�õĿռ�
			int index = 0;// ���ƶ�Ԫ�ص����� 
			int delCnt = 0;// ��ɾ����Ԫ�ظ���
			int toDelNum = 0;// ��ɾ������

			for (l = loc; l < m_vexnum; ++l) {
				index = (1 + l) * l / 2 + (loc + 1) - delCnt;

				if (l == loc) {
					toDelNum = loc + 1;
				}
				else if (l > loc) {
					toDelNum = 1;
				}

				// ���µ�ǰ�ߵ�����
				for (int i = index - toDelNum; i < index; i++) {
					if (m_pUDArcs[i].exist == EXIST) {
						--m_arcnum;
					}
				}

				// move elements
				for (int i = index; i < usedSpace; ++i) {
					m_pUDArcs[i - toDelNum] = m_pUDArcs[i];
				}

				delCnt = delCnt + toDelNum;
				usedSpace = usedSpace - toDelNum;
			}
		}
	}

	--m_vexnum;

	return TRUE;
}

template<typename VertexType, typename VertexInfo, typename EdgeInfo, typename CompareFunc>
BOOL MGraph<VertexType, VertexInfo, EdgeInfo, CompareFunc>::InsertEdge(VertexType v, VertexType w, EdgeInfo* pInfo /*=NULL*/, BOOL modify)
{
	return InsertEdgeByIndex(LocateVex(v), LocateVex(w), pInfo, modify);
}

template<typename VertexType, typename VertexInfo, typename EdgeInfo, typename CompareFunc>
inline BOOL MGraph<VertexType, VertexInfo, EdgeInfo, CompareFunc>::InsertEdgeByIndex(int i, int j, EdgeInfo* pInfo /*=NULL*/, BOOL modify)
{
	if (i < 0 || i >= m_vexnum || j < 0 || j >= m_vexnum) {
		return FALSE;
	}

	BOOL exist = ExistEdgeByIndex(i, j);
	EdgeNode<EdgeInfo> node;

	// ������ �� ������
	if (m_graphKind == DN || m_graphKind == UDN) {
		
		if (exist == TRUE) {
			return TRUE;
		}
		
		node.exist = EXIST;
	}
	// ����ͼ �� ����ͼ
	else {
		
		if ((exist == TRUE && modify == FALSE) || pInfo == NULL) {
			return FALSE;
		}

		node.exist = EXIST;
		node.info = *pInfo;
	}

	if (m_graphKind == DG || m_graphKind == DN) {
		m_pDArcs[i][j] = node;
	}
	else if (m_graphKind == UDG || m_graphKind == UDN) {
		m_pUDArcs[EdgeOffsetLocByIndex(i, j)] = node;
	}

	++m_arcnum;

	return TRUE;
}

template<typename VertexType, typename VertexInfo, typename EdgeInfo, typename CompareFunc>
BOOL MGraph<VertexType, VertexInfo, EdgeInfo, CompareFunc>::DeleteEdge(VertexType v, VertexType w)
{
	return DeleteEdgeByIndex(LocateVex(v), LocateVex(w));
}

template<typename VertexType, typename VertexInfo, typename EdgeInfo, typename CompareFunc>
inline BOOL MGraph<VertexType, VertexInfo, EdgeInfo, CompareFunc>::DeleteEdgeByIndex(int i, int j)
{
	BOOL bRet = FALSE;

	do 
	{
		if (i < 0 || i >= m_vexnum || j < 0 || j >= m_vexnum) {
			break;
		}

		if (m_graphKind == DG || m_graphKind == DN) {

			if (m_pDArcs[i][j].exist == EXIST) {
				m_pDArcs[i][j].exist = NOTEXIST;
				--m_arcnum;
				bRet = TRUE;
			}
		}
		else if (m_graphKind == UDG || m_graphKind == UDN) {

			int loc = EdgeOffsetLocByIndex(i, j);
			if (m_pUDArcs[loc].exist == EXIST) {
				m_pUDArcs[loc].exist = NOTEXIST;
				--m_arcnum;
				bRet = TRUE;
			}
		}
	} while (0);

	return bRet;
}

template<typename VertexType, typename VertexInfo, typename EdgeInfo, typename CompareFunc>
inline int MGraph<VertexType, VertexInfo, EdgeInfo, CompareFunc>::FirstAdjVex(VertexType v)
{
	return FirstAdjVexByIndex(LocateVex(v));
}

template<typename VertexType, typename VertexInfo, typename EdgeInfo, typename CompareFunc>
int MGraph<VertexType, VertexInfo, EdgeInfo, CompareFunc>::FirstAdjVexByIndex(int i)
{
	if (i < 0 || i >= m_vexnum) {
		return -1;
	}

	int k = 0;// ���صĶ�������ֵ

	if (m_graphKind == DG || m_graphKind == DN) {

		// �����ǻ�
		for (k = 0; (m_pDArcs[i][k]).exist == NOTEXIST && k < m_vexnum; ++k);

		if (k < m_vexnum) {
			return k;
		}
	}
	/*
		����ͼ���ڽӶ������ʾ��ͼ(������)��
			  a b c d e

			  0 1 2 3 4
		a	0 0
		b	1 1 0
		c	2 * * *
		d	3 0 1 + 0
		e	4 1 0 + 1 0

	*/
	else if (m_graphKind == UDG || m_graphKind == UDN) {

		// �������� * ��λ�� 
		for (k = 0; m_pUDArcs[(i + 1) * i / 2 + k].exist == NOTEXIST && k < i; ++k);

		if (k < i) {
			return k;
		}

		// �������� + ��λ��
		for (k = i + 1; m_pUDArcs[(k + 1) * k / 2 + i].exist == NOTEXIST && k < m_vexnum; k++);

		if (k < m_vexnum) {
			return k;
		}
	}

	return -1;
}

template<typename VertexType, typename VertexInfo, typename EdgeInfo, typename CompareFunc>
inline int MGraph<VertexType, VertexInfo, EdgeInfo, CompareFunc>::NextAdjVex(VertexType v, VertexType w)
{
	return NextAdjVexByIndex(LocateVex(v), LocateVex(w));
}

template<typename VertexType, typename VertexInfo, typename EdgeInfo, typename CompareFunc>
int MGraph<VertexType, VertexInfo, EdgeInfo, CompareFunc>::NextAdjVexByIndex(int i, int j)
{
	if (i >= m_vexnum || i < 0 || j >= m_vexnum || j < 0) {
		return -1;
	}

	int k = 0;

	if (m_graphKind == DG || m_graphKind == DN) {

		for (k = j + 1; (m_pDArcs[i][k]).exist == NOTEXIST && k < m_vexnum; ++k);

		if (k < m_vexnum) {
			return k;
		}
	} 
	else if (m_graphKind == UDG || m_graphKind == UDN) {

		k = j + 1;

		if (k < i) {

			for (k; m_pUDArcs[(i + 1) * i / 2 + k].exist == NOTEXIST && k < i; ++k);

			if (k < i) {
				return k;
			}

			for (k = i + 1; m_pUDArcs[(k + 1) * k / 2 + i].exist == NOTEXIST && k < m_vexnum; k++);

			if (k < m_vexnum) {
				return k;
			}
		}
		else {

			for (k; m_pUDArcs[(k + 1) * k / 2 + i].exist == NOTEXIST && k < m_vexnum; k++);

			if (k < m_vexnum) {
				return k;
			}
		}
	}

	return -1;
}

template<typename VertexType, typename VertexInfo, typename EdgeInfo, typename CompareFunc>
BOOL MGraph<VertexType, VertexInfo, EdgeInfo, CompareFunc>::GetEdge(VertexType v, VertexType w, EdgeInfo& info)
{
	return GetEdgeByIndex(LocateVex(v), LocateVex(w), info);
}

template<typename VertexType, typename VertexInfo, typename EdgeInfo, typename CompareFunc>
inline BOOL MGraph<VertexType, VertexInfo, EdgeInfo, CompareFunc>::GetEdgeByIndex(int i, int j, EdgeInfo& info)
{
	if (i >= 0 && i < m_vexnum && j >= 0 && j < m_vexnum) {

		if (m_graphKind == DG) {

			if ((m_pDArcs[i][j]).exist == EXIST) {
				info = m_pDArcs[i][j].info;
				return TRUE;
			}
		}
		else if (m_graphKind == UDG) {

			int loc = EdgeOffsetLocByIndex(i, j);
			if (m_pUDArcs[loc].exist == EXIST) {
				info = m_pUDArcs[loc].info;
				return TRUE;
			}
		}
		// m_graphKind == DN || m_graphKind == UDN
		else {
			// DN �� UDN ������Ȩֵ��Ϣ
		}
	}
	
	return FALSE;
}

template<typename VertexType, typename VertexInfo, typename EdgeInfo, typename CompareFunc>
BOOL MGraph<VertexType, VertexInfo, EdgeInfo, CompareFunc>::SetEdge(VertexType v, VertexType w, EdgeInfo* pInfo)
{
	return InsertEdgeByIndex(LocateVex(v), LocateVex(w), pInfo, TRUE);
}

template<typename VertexType, typename VertexInfo, typename EdgeInfo, typename CompareFunc>
inline BOOL MGraph<VertexType, VertexInfo, EdgeInfo, CompareFunc>::SetEdgeByIndex(int i, int j, EdgeInfo* pInfo)
{
	return InsertEdgeByIndex(i, j, pInfo, TRUE);
}

template<typename VertexType, typename VertexInfo, typename EdgeInfo, typename CompareFunc>
int MGraph<VertexType, VertexInfo, EdgeInfo, CompareFunc>::LocateVex(const VertexType v)
{
	for (int i = 0; i < m_vexnum; ++i) {

		if (TRUE == compare(m_szVex[i].val, v)) {
			return i;
		}
	}

	return -1;
}

template<typename VertexType, typename VertexInfo, typename EdgeInfo, typename CompareFunc>
BOOL MGraph<VertexType, VertexInfo, EdgeInfo, CompareFunc>::ExistVex(const VertexType v)
{
	return ExistVexByIndex(LocateVex(v));
}

template<typename VertexType, typename VertexInfo, typename EdgeInfo, typename CompareFunc>
inline BOOL MGraph<VertexType, VertexInfo, EdgeInfo, CompareFunc>::ExistVexByIndex(const int i)
{
	if (i < 0 || i > m_vexnum) {
		return FALSE;
	}

	return TRUE;
}

template<typename VertexType, typename VertexInfo, typename EdgeInfo, typename CompareFunc>
inline BOOL MGraph<VertexType, VertexInfo, EdgeInfo, CompareFunc>::ExistEdge(VertexType v, VertexType w)
{
	return ExistEdgeByIndex(LocateVex(v), LocateVex(w));
}

template<typename VertexType, typename VertexInfo, typename EdgeInfo, typename CompareFunc>
inline BOOL MGraph<VertexType, VertexInfo, EdgeInfo, CompareFunc>::ExistEdgeByIndex(int i, int j)
{
	if (i >= 0 && i < m_vexnum && j >= 0 && j < m_vexnum) {

		if (m_graphKind == DG || m_graphKind == DN) {
			if ((m_pDArcs[i][j]).exist == EXIST) {
				return TRUE;
			}
		}
		else if (m_graphKind == UDG || m_graphKind == UDN) {

			int loc = EdgeOffsetLocByIndex(i, j);
			if (m_pUDArcs[loc].exist == EXIST) {
				return TRUE;
			}
		}
	}

	return FALSE;
}

template<typename VertexType, typename VertexInfo, typename EdgeInfo, typename CompareFunc>
void MGraph<VertexType, VertexInfo, EdgeInfo, CompareFunc>::PrintInfo()
{
	cout << "��ǰ������ʹ�ÿռ䣺" << m_vexnum << endl;
	cout << "��ǰ����ʹ�õĿռ䣺";
	if (m_graphKind == DG || m_graphKind == DN) {
		cout << m_vexnum * m_vexnum << endl;
	}
	else {
		cout << (1 + m_vexnum) * m_vexnum / 2 << endl;
	}

	// ��ӡ������Ϣ
	cout << "��ǰ������Ϣ���£�(���� " << m_vexnum << " ������)" << endl;
	for (int i = 0; i < m_vexnum; i++) {
		cout << "�� " << i + 1 << " �������ֵ:" << m_szVex[i].val << "   �������ϢΪ:";
		if (m_szVex[i].valid == VALID) {
			cout << m_szVex[i].info << endl;
		}
		else {
			cout << "<��ǰû�м�¼������Ϣ>" << endl;
		}
	}

	// ��ӡ����Ϣ
	if (m_graphKind == DG || m_graphKind == DN) {
		cout << "��ǰ����߹��У�" << m_arcnum << "��" << endl;
		int num = 0;
		for (int i = 0; i < m_vexnum; i++) {
			for (int j = 0; j < m_vexnum; j++) {
				if (m_pDArcs[i][j].exist == EXIST) {
					cout << "�� " << ++num << " ���� <" << GetVexVal(i) << "," << GetVexVal(j) << "> ����ϢΪ:";
					if (m_graphKind == DG){
						cout << m_pDArcs[i][j].info;
					}
					else {
						cout << "<������û��Ȩֵ��Ϣ>";
					}
					cout << endl;
				}
			}
		}
	}
	else {
		cout << "��ǰ����߹��У�" << m_arcnum * 2 << "��" << endl;
		int num = 0;
		int loc = 0;
		for (int i = 0; i < m_vexnum; i++) {
			for (int j = 0; j < m_vexnum; j++) {
				loc = EdgeOffsetLocByIndex(i, j);
				if (m_pUDArcs[loc].exist == EXIST) {
					cout << "�� " << ++num << " ���� (" << GetVexVal(i) << "," << GetVexVal(j) << ") ����ϢΪ:";
					if (m_graphKind == UDG) {
						cout << m_pUDArcs[loc].info;
					}
					else {
						cout << "<������û��Ȩֵ��Ϣ>";
					}
					cout << endl;
				}

			}
		}
	}

	// ��ӡ����ͼ������ͼ�Ĵ洢�ṹ
	cout << endl;
	int n = 0;
	int j = 0;

	if (m_graphKind == DG || m_graphKind == DN) {
		cout << "����ͼ����洢�ڴ����У�1���бߣ�0���ޱߣ���" << endl;

		cout << "  ";
		for (int i = 0; i < m_vexnum; i++) {
			cout << m_szVex[i].val << " ";
		}
		cout << endl;

		for (j = 0; j < m_vexnum; j++) {
			cout << m_szVex[j].val << " ";
			for (int i = 0; i < m_vexnum; ++i) {
				if (m_pDArcs[j][i].exist == EXIST) {
					cout << "1 ";
				}
				else {
					cout << "0 ";
				}
			}
			cout << endl;
		}
			
	}
	else {
		n = 0;
		j = 0;

		cout << "����ͼ ������ѹ���洢�ڴ����У�1���бߣ�0���ޱߣ���" << endl;
		if (m_vexnum == 0) { 
			return;
		}

		cout << "  ";
		for (int i = 0; i < m_vexnum; i++) {
			cout << m_szVex[i].val << " ";
		}
		cout << endl;

		cout << m_szVex[n].val << " ";
		for (int i = 0; i < (1 + m_vexnum) * m_vexnum / 2; i++) {

			if (m_pUDArcs[i].exist == EXIST) {
				cout << "1 ";
			}
			else {
				cout << "0 ";
			}

			if (n == j) {
				n++;
				j = 0;
				cout << endl;
				cout << m_szVex[n].val << " ";
			}
			else {
				j++;
			}
		}
	}
}

template<typename VertexType, typename VertexInfo, typename EdgeInfo, typename CompareFunc>
inline int MGraph<VertexType, VertexInfo, EdgeInfo, CompareFunc>::EdgeOffsetLoc(VertexType v, VertexType w)
{
	return EdgeOffsetLocByIndex(LocateVex(v), LocateVex(w));
}

template<typename VertexType, typename VertexInfo, typename EdgeInfo, typename CompareFunc>
int MGraph<VertexType, VertexInfo, EdgeInfo, CompareFunc>::EdgeOffsetLocByIndex(int i, int j)
{
	int loc = -1;

	if (i >= 0 && i < m_vexnum && j >= 0 && j < m_vexnum) {
		if (j > i) {
			loc = (j + 1) * j / 2 + i;
		}
		else {
			loc = (i + 1) * i / 2 + j;
		}
	}

	return loc;
}

#endif // !__ADJACENCYMATRIX_H__
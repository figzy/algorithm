#ifndef __ADJACENCYMATRIX_H__
#define __ADJACENCYMATRIX_H__

#include <memory.h>
#include <iostream>
using namespace std;

#define MAX_VERTEX_NUM 64

/*
 * ͼ����������
 *
 * ����
 *  DG ������ͼ����Ȩֵ��
 *  DN ��������
 *  UDG ������ͼ����Ȩֵ��
 *  UDN ��������
 */
enum GraphKind {
	DG,
	DN,
	UDG,
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
 *
 * ��Ա����
 *  info ���ñ���ص���Ϣ
 *  exist ���ñ��Ƿ���� info ��¼��EXIST��˵������info��Ϣ��NOTEXIST˵��������info��Ϣ
 */
template<typename EdgeInfo>
struct EdgeNode
{
	unsigned int exist : 1;
	EdgeInfo info;
};

/*
 * ����ڵ�
 *
 * ��Ա����
 *  val ������ֵ
 *  info ��������Ϣ
 *  valid ���ñ��Ƿ���� info ��¼��VALID��˵������info��Ϣ��INVALID˵��������info��Ϣ
 */
template<typename VertexType, typename VertexInfo>
struct VNode
{
	VertexType val;
	VertexInfo info;
	unsigned int valid : 1;
};

/*
 * �ڽӾ���洢�ṹ
 *
 * ģ�����
 *  VertexType ������ֵ����
 *  VertexInfo ��������Ϣ����
 *  EdgeInfo ������Ϣ����
 *  CompareFunc: �Ƚ϶���ֵ�ĺ���ָ��
 *               ������ʽ�� BOOL FunctionName(VertexType v, VertexType w);
 *               Ҫ�󣺶���ֵv��w��ͬ����TRUE�����򷵻�FALSE
 */
template<typename VertexType, typename VertexInfo, typename EdgeInfo, typename CompareFunc>
class MGraph
{
public:
	MGraph(CompareFunc func, GraphKind typ = UDG);
	virtual ~MGraph();

	virtual int InsertVex(const VertexType& v);

	virtual BOOL SetVex(const VertexType& v, const VertexInfo& info, BOOL modify = TRUE);
	virtual BOOL SetVexByIndex(int i, const VertexInfo& info, BOOL modify = TRUE);
	
	virtual BOOL GetVex(const VertexType& v, VertexInfo& info);
	virtual BOOL GetVexByIndex(int i, VertexInfo& info);

	virtual BOOL DeleteVex(const VertexType& v);
	virtual BOOL DeleteVexByIndex(const int loc);

	virtual BOOL InsertEdge(const VertexType& v, const VertexType& w, EdgeInfo* pInfo = NULL, BOOL modify = TRUE);
	virtual BOOL InsertEdgeByIndex(int i, int j, EdgeInfo* pInfo = NULL, BOOL modify = TRUE);

	virtual BOOL DeleteEdge(const VertexType& v, const VertexType& w);
	virtual BOOL DeleteEdgeByIndex(int i, int j);

	virtual BOOL SetEdge(const VertexType& v, const VertexType& w, EdgeInfo* pInfo);
	virtual BOOL SetEdgeByIndex(int i, int j, EdgeInfo* pInfo);

	virtual BOOL GetEdge(const VertexType& v, const VertexType& w, EdgeInfo& info);
	virtual BOOL GetEdgeByIndex(int i, int j, EdgeInfo& info);

	virtual BOOL ExistEdge(const VertexType& v, const VertexType& w);
	virtual BOOL ExistEdgeByIndex(int i, int j);

	virtual int FirstAdjVex(const VertexType& v);
	virtual int FirstAdjVexByIndex(int i);

	virtual int NextAdjVex(const VertexType& v, const VertexType& w);
	virtual int NextAdjVexByIndex(int i, int j);

	virtual int LocateVex(const VertexType& v);

	virtual BOOL ExistVex(const VertexType& v);
	virtual BOOL ExistVexByIndex(const int i);

	virtual VertexType GetVexVal(int i) const;
	virtual GraphKind GetGraphKind() const;
	virtual int GetVexNum() const;

	// ��ӡ����
	virtual void PrintInfo();

private:
	// ����Ĭ�Ϲ��캯��
	MGraph() {}

	virtual int EdgeOffsetLoc(const VertexType& v, const VertexType& w);
	virtual int EdgeOffsetLocByIndex(int i, int j);

private:
	VNode<VertexType, VertexInfo> m_szVex[MAX_VERTEX_NUM];// �����
	EdgeNode<EdgeInfo>** m_pDArcs;// ����ͼ�ڽӾ��� 
	EdgeNode<EdgeInfo>* m_pUDArcs;// ����ͼ,����������ѹ���洢
	int m_vexnum;// ͼ�ĵ�ǰ������
	int m_arcnum;// ͼ�Ļ���
	GraphKind m_graphKind;// ͼ���ͣ�����ͼ �� ����ͼ
	CompareFunc compare;// ����ıȽϺ���
};

/*
 * ���캯��
 *
 * ����
 *  func ������ֵ�ȽϺ���ָ��
 *  kind ��ͼ���͡�Ĭ��Ϊ����ͼ UDG
 */
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

/*
 * ��������
 */
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

/*
 * ��ͼ�в���һ������
 *
 * ����
 *  v ������Ķ���ֵ
 *
 * ����ֵ
 *  int ����������ڸö���ֵ�������ɹ��������ز�����±�
 *        ������ڸö���ֵ������ -1
 */
template<typename VertexType, typename VertexInfo, typename EdgeInfo, typename CompareFunc>
int MGraph<VertexType, VertexInfo, EdgeInfo, CompareFunc>::InsertVex(const VertexType& v)
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

/*
 * ���ö������Ϣ
 *
 * ����
 *  v �������ֵ
 *  info �����õĶ�����Ϣ
 *  modify �����Ϊ TRUE���򵱶�����Ϣ����ʱ�������޸Ķ�����Ϣ��
 *           ���Ϊ FALSE���򶥵���Ϣ����ʱ���Ͳ��޸Ķ�����Ϣ
 *
 * ����ֵ
 *  BOOL ������/����ֵ�ɹ�������TRUE�����򷵻� FALSE
 */
template<typename VertexType, typename VertexInfo, typename EdgeInfo, typename CompareFunc>
inline BOOL MGraph<VertexType, VertexInfo, EdgeInfo, CompareFunc>::SetVex(const VertexType& v, const VertexInfo& info, BOOL modify /*=TRUE*/)
{
	return SetVexByIndex(LocateVex(v), info, TRUE);
}

template<typename VertexType, typename VertexInfo, typename EdgeInfo, typename CompareFunc>
inline BOOL MGraph<VertexType, VertexInfo, EdgeInfo, CompareFunc>::SetVexByIndex(int i, const VertexInfo& info, BOOL modify /*=TRUE*/)
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

/*
 * ��ȡͼ��ĳһ����Ķ�����Ϣ
 *
 * ����
 *  v ������ֵ
 *  info ����ȡ�Ķ�����Ϣ��
 *
 * ����ֵ
 *  BOOL ��������ڶ���ֵ v �Ķ�����Ϣ���򷵻� TRUE�����򷵻� FALSE
 */
template<typename VertexType, typename VertexInfo, typename EdgeInfo, typename CompareFunc>
inline BOOL MGraph<VertexType, VertexInfo, EdgeInfo, CompareFunc>::GetVex(const VertexType& v, VertexInfo& info)
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

/*
 * ��ͼ��ɾ������ֵ
 *
 * ����
 *  v ��ɾ���Ķ����ֵ
 *
 * ����ֵ
 *  BOOL ��������ڸö���ֵ v����ɾ���ɹ������� TRUE�����򷵻� FALSE
 */
template<typename VertexType, typename VertexInfo, typename EdgeInfo, typename CompareFunc>
BOOL MGraph<VertexType, VertexInfo, EdgeInfo, CompareFunc>::DeleteVex(const VertexType& v)
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

/*
 * ��ͼ�в���һ����
 *
 * ����
 *  v��w ������ߵ��������㡣���������ߣ���ñ�Ϊ <v, w>;���Ϊ����ߣ���ñ�Ϊ (v, w)
 *  pInfo ������ıߵı���Ϣָ��
 *  modify ����Ϊ TRUE�����ڱߴ��ڵ�������޸ĸñߵ���Ϣ����Ϊ FALSE�����ڱߴ��ڵ�����²���ʧ��
 *
 * ����ֵ
 *  BOOL ���߲���ɹ����� TRUE������ʧ�ܷ��� FALSE
 */
template<typename VertexType, typename VertexInfo, typename EdgeInfo, typename CompareFunc>
BOOL MGraph<VertexType, VertexInfo, EdgeInfo, CompareFunc>::InsertEdge(const VertexType& v, const VertexType& w, EdgeInfo* pInfo /*=NULL*/, BOOL modify)
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

/*
 * ��ͼ��ɾ����
 *
 * ����
 *  v��w ��ɾ���ıߵĶ���ֵ����Ϊ����ͼ����ɾ���ı�Ϊ(v, w)����Ϊ����ߣ���ɾ���ı�Ϊ<v, w>
 *
 * ����ֵ
 *  BOOL ��ɾ���ɹ�����TRUE��ʧ�ܷ���FALSE
 */
template<typename VertexType, typename VertexInfo, typename EdgeInfo, typename CompareFunc>
BOOL MGraph<VertexType, VertexInfo, EdgeInfo, CompareFunc>::DeleteEdge(const VertexType& v, const VertexType& w)
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

/*
 * �󶥵� v �ĵ�һ���ڽӵ������ֵ
 *
 * ����
 *  v ������ֵ
 *
 * ����ֵ
 *  int ����һ���ڽӵ������ֵ�����������ڽӵ㣬�򷵻� -1
 */
template<typename VertexType, typename VertexInfo, typename EdgeInfo, typename CompareFunc>
inline int MGraph<VertexType, VertexInfo, EdgeInfo, CompareFunc>::FirstAdjVex(const VertexType& v)
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

/*
 * �󶥵� v ���ڽӶ��� w ����һ���ڽӶ��������
 *
 * ����
 *  v ������ֵ
 *  w ������ֵ
 *
 * ����ֵ
 *  int ����һ���ڽӵ������ֵ�����������ڽӵ㣬�򷵻� -1
 */
template<typename VertexType, typename VertexInfo, typename EdgeInfo, typename CompareFunc>
inline int MGraph<VertexType, VertexInfo, EdgeInfo, CompareFunc>::NextAdjVex(const VertexType& v, const VertexType& w)
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

/*
 * ��ȡ����Ϣ
 *
 * ����
 *  v, w ���ߵ��������㡣��Ϊ����ߣ���Ϊ(v, w);��Ϊ����ߣ���Ϊ<v, w>
 *  info �����ջ�ȡ�ı���Ϣ
 *
 * ����ֵ
 *  BOOL �������ڱ��Լ���Ӧ�ı���Ϣ���򷵻� TRUE�����򷵻� FALSE
 */
template<typename VertexType, typename VertexInfo, typename EdgeInfo, typename CompareFunc>
BOOL MGraph<VertexType, VertexInfo, EdgeInfo, CompareFunc>::GetEdge(const VertexType& v, const VertexType& w, EdgeInfo& info)
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

/*
 * ���ñߵ���Ϣ
 *
 * ����
 *  v, w ���ߵ��������㡣��Ϊ����ߣ���Ϊ(v, w);��Ϊ����ߣ���Ϊ<v, w>
 *  pInfo ������Ϣָ�룬��ΪNULL�������ʧ��
 *
 * ����ֵ
 *  BOOL ������ɹ����򷵻�TRUE�����򷵻�FALSE
 */
template<typename VertexType, typename VertexInfo, typename EdgeInfo, typename CompareFunc>
BOOL MGraph<VertexType, VertexInfo, EdgeInfo, CompareFunc>::SetEdge(const VertexType& v, const VertexType& w, EdgeInfo* pInfo)
{
	return InsertEdgeByIndex(LocateVex(v), LocateVex(w), pInfo, TRUE);
}

template<typename VertexType, typename VertexInfo, typename EdgeInfo, typename CompareFunc>
inline BOOL MGraph<VertexType, VertexInfo, EdgeInfo, CompareFunc>::SetEdgeByIndex(int i, int j, EdgeInfo* pInfo)
{
	return InsertEdgeByIndex(i, j, pInfo, TRUE);
}

/*
 * ��ȡ���������λ��
 *
 * ����
 *  v ������ֵ
 *
 * ����ֵ
 *  int �������ڸö��㣬�򷵻ض������ڵ��������������ڣ��򷵻� -1
 */
template<typename VertexType, typename VertexInfo, typename EdgeInfo, typename CompareFunc>
int MGraph<VertexType, VertexInfo, EdgeInfo, CompareFunc>::LocateVex(const VertexType& v)
{
	for (int i = 0; i < m_vexnum; ++i) {

		if (TRUE == compare(m_szVex[i].val, v)) {
			return i;
		}
	}

	return -1;
}

/*
 * �Ƿ���ڶ���
 *
 * ����
 *  v ������ֵ
 *
 * ����ֵ
 *  BOOL �������ڸö��㣬�򷵻� TRUE�����򷵻� FALSE
 */
template<typename VertexType, typename VertexInfo, typename EdgeInfo, typename CompareFunc>
BOOL MGraph<VertexType, VertexInfo, EdgeInfo, CompareFunc>::ExistVex(const VertexType& v)
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

/*
 * ��ȡ����Ϊ i �Ķ�����Ϣ
 */
template<typename VertexType, typename VertexInfo, typename EdgeInfo, typename CompareFunc>
inline VertexType MGraph<VertexType, VertexInfo, EdgeInfo, CompareFunc>::GetVexVal(int i) const
{
	if (i < 0 || i >= m_vexnum) {
		throw "out of range";
	}
	return m_szVex[i].val;
}

/*
 * ��ȡ��ǰͼ������
 */
template<typename VertexType, typename VertexInfo, typename EdgeInfo, typename CompareFunc>
inline GraphKind MGraph<VertexType, VertexInfo, EdgeInfo, CompareFunc>::GetGraphKind() const
{
	return m_graphKind;
}

/*
 * ��ȡ��ǰ��������
 */
template<typename VertexType, typename VertexInfo, typename EdgeInfo, typename CompareFunc>
inline int MGraph<VertexType, VertexInfo, EdgeInfo, CompareFunc>::GetVexNum() const
{
	return m_vexnum;
}

/*
 * �Ƿ���ڱ�
 *
 * ����
 *  v, w ���ߵ��������㡣��Ϊ����ߣ���Ϊ(v, w);��Ϊ����ߣ���Ϊ<v, w>
 *
 * ����ֵ
 *  BOOL ��������ڱߣ��򷵻� TRUE�����򷵻� FALSE
 */
template<typename VertexType, typename VertexInfo, typename EdgeInfo, typename CompareFunc>
inline BOOL MGraph<VertexType, VertexInfo, EdgeInfo, CompareFunc>::ExistEdge(const VertexType& v, const VertexType& w)
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

/*
 * ����ͼ�бߵ�ƫ�Ƶ�ַ
 */
template<typename VertexType, typename VertexInfo, typename EdgeInfo, typename CompareFunc>
inline int MGraph<VertexType, VertexInfo, EdgeInfo, CompareFunc>::EdgeOffsetLoc(const VertexType& v, const VertexType& w)
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
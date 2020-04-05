#ifndef __ADJACENCYLIST_H__
#define __ADJACENCYLIST_H__

#include "../public/define.h"
#include <iostream>
using namespace std;

/**************************
 *     �ڽӱ�洢�ṹ
 *************************/

/*
 * �߱�ڵ�
 */
template<typename EdgeType>
struct EdgeNode
{
    // �ñ���ָ��Ķ����λ��
    int adjvex;

    // �ñ���ص���Ϣ
    EdgeType info;

    // �ñ���Ϣ info �Ƿ��м�¼
    unsigned int valid : 1;
    
    // ָ����һ���ߵ�ָ��
    struct EdgeNode<EdgeType>* nextarc;
};

/*
 * ����ڵ�
 */
template<typename VexType, typename EdgeType>
struct VNode 
{
    // ������Ϣ
    VexType data;

    // ָ���һ�������ö���Ļ���ָ��
	EdgeNode<EdgeType>* firstarc;
};

/*
 * Ĭ�ϵ�CompareFunc�����Լ�Ĭ�ϵĶ���ȽϺ���
 */
typedef BOOL(*CompareFuncType)(char* v1, char* v2);
BOOL DefaultCompare(char* v1, char* v2) {
	if (0 == strcmp(v1, v2)) {
		return TRUE;
	}
	return FALSE;
}

/*
 * �ڽӱ�洢�ṹ
 * CompareFunc: �Ƚ϶���ֵ�ĺ���ָ�룬������ʽ�� BOOL FunctionName(VertexType v, VertexType w);Ҫ����ͬ����TRUE�����򷵻�FALSE
 */
template<typename VexType, typename EdgeType, typename CompareFunc>
class ALGraph 
{
public:
    ALGraph(CompareFunc func, GraphKind kind = DG);
    virtual ~ALGraph();

	// ��ͼG�в��붥��, ���ص�ǰ�����������
	virtual int InsertVex(const VexType v);

	// ��ͼG��ɾ������
	virtual BOOL DeleteVex(const VexType v);
	virtual BOOL DeleteVexByIndex(const int i);

    // �������(x, y)�������<x, y>�����ڣ�����ͼG����Ӹñߡ�
    // ��� modify == true�����ڴ��� Edge ������½����޸ĸñߵĻ���Ϣ����� modify Ϊ false���ڱ��Ѿ����ڵ�����»᷵�� FALSE
	virtual BOOL InsertEdge(VexType v, VexType w, EdgeType* pInfo = NULL, BOOL modify = TRUE);
	virtual BOOL InsertEdgeByIndex(int i, int j, EdgeType* pInfo = NULL, BOOL modify = TRUE);

	// �������(x, y)�������<x, y>���ڣ����ͼG��ɾ���ñߡ����߲������򷵻� FALSE�����óɹ����� TRUE��
	virtual BOOL DeleteEdge(VexType v, VexType w);
	virtual BOOL DeleteEdgeByIndex(int i, int j);

    // �Ƿ���ڱߣ�v, w���� <v, w>
    virtual BOOL ExistEdge(VexType v, VexType w);
    virtual BOOL ExistEdgeByIndex(int i, int j);

	// ��ͼ�д��ڶ��� value,�򷵻ظö�����ͼ��λ�ã����򷵻�-1
	virtual int LocateVex(const VexType v);

	// ��ͼ�д��ڶ��� value���򷵻� TRUE�����򷵻� FALSE
	virtual BOOL ExistVex(const VexType v);
	virtual BOOL ExistVexByIndex(const int i);

	// ��ͼG�ж��� x �ĵ�һ���ڽӵ㡣���У��򷵻ض���ţ��� x û���ڽӵ�򲻴��� x���򷵻�-1��
	virtual int FirstAdjVex(const VexType v);
	virtual int FirstAdjVexByIndex(int i);

	// ����ͼG�ж��� y �Ƕ��� x ��һ���ڽӵ㣬���س� y ֮�ⶥ�� x ����һ���ڽӵ�Ķ���ţ��� y �� x �����һ���ڽӵ㣬�򷵻� -1��
	virtual int NextAdjVex(const VexType v, const VexType w);
	virtual int NextAdjVexByIndex(int i, int j);

	// ��ȡ����Ϊ i �Ķ�����Ϣ
	virtual VexType GetVexByIndex(int i) const { return m_szVex[i].data; }

	// ��ȡ��ǰͼ������
	virtual GraphKind GetGraphKind() const { return m_graphKind; }

	// ��ȡ��ǰ��������
	virtual int GetVexNum() const { return m_vexnum; }

	// �������
	void PrintInfo();

private:
	// ����Ĭ�Ϲ��캯��
	ALGraph() {}

	BOOL deleteEdge(int i, int j);

private:
    // �ڽӱ�
    VNode<VexType, EdgeType> m_szVex[MAX_VERTEX_NUM];

    // ͼ��ǰ�Ķ�����
    int m_vexnum;

    // ͼ��ǰ�Ļ���
    int m_arcnum;

    // ͼ�������־
    GraphKind m_graphKind;

	// ����ıȽϺ���
	CompareFunc compare;
};

template<typename VexType, typename EdgeType, typename CompareFunc>
ALGraph<VexType, EdgeType, CompareFunc>::ALGraph(CompareFunc func, GraphKind kind /*= DG*/)
{
    memset(m_szVex, 0, sizeof(VNode<VexType, EdgeType>) * MAX_VERTEX_NUM);
    m_arcnum = 0;
    m_vexnum = 0;
    m_graphKind = kind;
	compare = func;
}

template<typename VexType, typename EdgeType, typename CompareFunc>
ALGraph<VexType, EdgeType, CompareFunc>::~ALGraph()
{

}


template<typename VexType, typename EdgeType, typename CompareFunc>
int ALGraph<VexType, EdgeType, CompareFunc>::InsertVex(const VexType v)
{
    int loc = -1;

    if (!ExistVex(v)) {
		VNode<VexType, EdgeType> vex;
		vex.data = v;
		vex.firstarc = NULL;

		loc = m_vexnum++;
		m_szVex[loc] = vex;
    }

    return loc;
}

template<typename VexType, typename EdgeType, typename CompareFunc>
inline BOOL ALGraph<VexType, EdgeType, CompareFunc>::DeleteVex(const VexType v)
{
	return DeleteVexByIndex(LocateVex(v));
}

template<typename VexType, typename EdgeType, typename CompareFunc>
inline BOOL ALGraph<VexType, EdgeType, CompareFunc>::DeleteVexByIndex(const int i)
{
	if (i < 0 || i > m_vexnum) {
		return FALSE;
	}

	// ɾ����
	int j = 0;
	EdgeNode<EdgeType>* p = NULL;
	EdgeNode<EdgeType>* q = NULL;

	for (j = 0; j < m_vexnum; ++j) {

		p = m_szVex[j].firstarc;

		if (j == i) {
			while (p) {
				q = p->nextarc;
				delete p;
				p = q;
				--m_arcnum;
			}
			m_szVex[j].firstarc = NULL;
		}
		else {
			if (p != NULL && p->adjvex == i) {
				m_szVex[j].firstarc = p->nextarc;
				delete p;
				--m_arcnum;
			}
			else
			{
				while (p) {
					if (p->adjvex == i) {
						q->nextarc = p->nextarc;
						delete p;
						--m_arcnum;
						break;
					}
					q = p;
					p = p->nextarc;
				}
			}
		}
	}

	// ɾ������
	for (j = 0; j < m_vexnum; ++j) {

		if (j == i) {
			continue;
		}

		p = m_szVex[j].firstarc;
		while (p) {
			// ɾ������Ϊ i �Ķ����> i �Ķ�����ǰ�ƣ���������1
			if (p->adjvex > i) {
				(p->adjvex)--;
			}
			p = p->nextarc;
		}

		// �ƶ�Ԫ��
		if (j > i) {
			m_szVex[j - 1] = m_szVex[j];
		}
	}

	memset(&m_szVex[m_vexnum - 1], 0, sizeof(VNode<VexType, EdgeType>));
	--m_vexnum;

	return TRUE;
}

template<typename VexType, typename EdgeType, typename CompareFunc>
inline BOOL ALGraph<VexType, EdgeType, CompareFunc>::InsertEdge(VexType v, VexType w, EdgeType* pInfo /*=NULL*/, BOOL modify)
{
    return InsertEdgeByIndex(LocateVex(v), LocateVex(w), pInfo, modify);
}

template<typename VexType, typename EdgeType, typename CompareFunc>
inline BOOL ALGraph<VexType, EdgeType, CompareFunc>::InsertEdgeByIndex(int i, int j, EdgeType* pInfo /*=NULL*/, BOOL modify)
{
	if (i < 0 || i > m_vexnum || j < 0 || j > m_vexnum) {
		return FALSE;
	}

	if (ExistEdgeByIndex(i, j) && modify == FALSE) {
		return FALSE;
	}

	VNode<VexType, EdgeType>* p = NULL;

	p = &m_szVex[i];
	EdgeNode<EdgeType>* arc = new EdgeNode<EdgeType>();
	arc->adjvex = j;
	if (pInfo == NULL) {
		arc->valid = INVALID;
	}
	else {
		arc->info = *pInfo;
		arc->valid = VALID;
	}
	arc->nextarc = p->firstarc;
	p->firstarc = arc;

	if (m_graphKind == UDG || m_graphKind == UDN) {
		// ����ͼ
		p = &m_szVex[j];

		EdgeNode<EdgeType>* arc2 = new EdgeNode<EdgeType>();
		arc2->adjvex = i;
		if (pInfo == NULL) {
			arc2->valid = INVALID;
		}
		else {
			arc2->info = *pInfo;
			arc2->valid = VALID;
		}
		arc2->nextarc = p->firstarc;
		p->firstarc = arc2;

		++m_arcnum;
	}

	++m_arcnum;

	return TRUE;
}

template<typename VexType, typename EdgeType, typename CompareFunc>
inline BOOL ALGraph<VexType, EdgeType, CompareFunc>::DeleteEdge(VexType v, VexType w)
{
	return DeleteEdgeByIndex(LocateVex(v), LocateVex(w));
}

template<typename VexType, typename EdgeType, typename CompareFunc>
inline BOOL ALGraph<VexType, EdgeType, CompareFunc>::DeleteEdgeByIndex(int i, int j)
{
	if (deleteEdge(i, j) == FALSE) {
		return FALSE;
	}

	if (m_graphKind == UDG || m_graphKind == UDN) {
		deleteEdge(j, i);
	}

	return TRUE;
}

template<typename VexType, typename EdgeType, typename CompareFunc>
inline BOOL ALGraph<VexType, EdgeType, CompareFunc>::ExistEdge(VexType v, VexType w)
{
	return ExistEdgeByIndex(LocateVex(v), LocateVex(w));
}

template<typename VexType, typename EdgeType, typename CompareFunc>
inline BOOL ALGraph<VexType, EdgeType, CompareFunc>::ExistEdgeByIndex(int i, int j)
{
    if (i >= 0 && i < m_vexnum && j >= 0 && j < m_vexnum) {
		EdgeNode<EdgeType>* p = m_szVex[i].firstarc;
		while (p)
		{
			if (p->adjvex == j) {
				return TRUE;
			}

			p = p->nextarc;
		}
    }

    return FALSE;
}

template<typename VexType, typename EdgeType, typename CompareFunc>
inline int ALGraph<VexType, EdgeType, CompareFunc>::LocateVex(const VexType v)
{
    int i = 0;
	for (i; i < m_vexnum; ++i) {
		if (compare(m_szVex[i].data, v) == TRUE) {
			break;
		}
	}

    if (i == m_vexnum) {
        return -1;
    }

    return i;
}

template<typename VexType, typename EdgeType, typename CompareFunc>
inline BOOL ALGraph<VexType, EdgeType, CompareFunc>::ExistVex(const VexType v)
{
    return ExistVexByIndex(LocateVex(v));
}

template<typename VexType, typename EdgeType, typename CompareFunc>
inline BOOL ALGraph<VexType, EdgeType, CompareFunc>::ExistVexByIndex(const int i)
{
	if (i < 0 || i > m_vexnum) {
		return FALSE;
	}
	return TRUE;
}

template<typename VexType, typename EdgeType, typename CompareFunc>
inline int ALGraph<VexType, EdgeType, CompareFunc>::FirstAdjVex(const VexType v)
{
	return FirstAdjVexByIndex(LocateVex(v));
}

template<typename VexType, typename EdgeType, typename CompareFunc>
inline int ALGraph<VexType, EdgeType, CompareFunc>::FirstAdjVexByIndex(int i)
{
	if (i >= 0 && i < m_vexnum) {
		EdgeNode<EdgeType>* p = m_szVex[i].firstarc;
		if (p != NULL) {
			return p->adjvex;
		}
	}

	return -1;
}

template<typename VexType, typename EdgeType, typename CompareFunc>
inline int ALGraph<VexType, EdgeType, CompareFunc>::NextAdjVex(const VexType v, const VexType w)
{
	return NextAdjVexByIndex(LocateVex(v), LocateVex(w));
}

template<typename VexType, typename EdgeType, typename CompareFunc>
inline int ALGraph<VexType, EdgeType, CompareFunc>::NextAdjVexByIndex(int i, int j)
{
	if (i >= 0 && i < m_vexnum && j >= 0 && j < m_vexnum) {
		EdgeNode<EdgeType>* p = NULL;
		p = m_szVex[i].firstarc;
		while (p)
		{
			if (p->adjvex == j) {
				p = p->nextarc;
				break;
			}

			p = p->nextarc;
		}

		if (p != NULL) {
			return p->adjvex;
		}
	}

	return -1;
}

template<typename VexType, typename EdgeType, typename CompareFunc>
inline void ALGraph<VexType, EdgeType, CompareFunc>::PrintInfo()
{
	cout << "��ǰ������ʹ�ÿռ䣺" << m_vexnum << endl;
	cout << "��ǰ����ʹ�õĿռ䣺" << m_arcnum << endl;

	// ��ӡ������Ϣ
	cout << "��ǰ������Ϣ���£�(���� " << m_vexnum << " ������)" << endl;
	for (int i = 0; i < m_vexnum; i++) {
		cout << "�� " << i + 1 << " ���������ϢΪ��" << m_szVex[i].data << endl;
	}
	
	// ��ӡ����Ϣ
	if (m_graphKind == DG || m_graphKind == DN) {
		cout << "��ǰ����߹��У�" << m_arcnum << "��" << endl;
	}
	else {
		cout << "��ǰ����߹��У�" << m_arcnum << "��" << endl;
	}
	
	int num = 0;
	for (int i = 0; i < m_vexnum; i++) {
		EdgeNode<EdgeType>* p = m_szVex[i].firstarc;
		while (p) {
			if (m_graphKind == DG || m_graphKind == DN) {
				cout << "�� " << ++num << " ���� <" << m_szVex[i].data << "," << m_szVex[p->adjvex].data << "> ����ϢΪ: " << p->info << endl;
			}
			else {
				cout << "�� " << ++num << " ���� (" << m_szVex[i].data << "," << m_szVex[p->adjvex].data << ") ����ϢΪ: " << p->info << endl;
			}
			p = p->nextarc;
		}
	}
	
}

template<typename VexType, typename EdgeType, typename CompareFunc>
inline BOOL ALGraph<VexType, EdgeType, CompareFunc>::deleteEdge(int i, int j)
{
	if (i < 0 || i > m_vexnum || j < 0 || j > m_vexnum) {
		return FALSE;
	}

	if (!ExistEdgeByIndex(i, j)) {
		return FALSE;
	}

	EdgeNode<EdgeType>* p = NULL;
	EdgeNode<EdgeType>* q = NULL;

	p = m_szVex[i].firstarc;
	if (p == NULL) {
		return FALSE;
	}

	if (p->adjvex == j) {
		m_szVex[i].firstarc = p->nextarc;
		delete p;
	}
	else {
		while (p) {
			if (p->adjvex == j) {
				q->nextarc = p->nextarc;
				delete p;
				break;
			}
			q = p;
			p = p->nextarc;
		}
	}

	--m_arcnum;
	
	return TRUE;
}


#endif // !__ADJACENCYLIST_H__
#ifndef __ADJACENCYLIST_H__
#define __ADJACENCYLIST_H__

#include <iostream>
using namespace std;

/**************************
 *     �ڽӱ�洢�ṹ
 *************************/

#define MAX_VERTEX_NUM 64

typedef int BOOL;
#define TRUE 1
#define FALSE 0

#define EXIST 0x1
#define NOTEXIST 0x0

#define VALID 0x1
#define INVALID 0x0

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


/*
 * �߱�ڵ�
 */
template<typename EdgeInfo>
struct EdgeNode
{
    // �ñ���ָ��Ķ����λ��
    int adjvex;

    // �ñ���ص���Ϣ
    EdgeInfo info;

    // �ñ��Ƿ���� info ��¼
    unsigned int valid : 1;
    
    // ָ����һ���ߵ�ָ��
    struct EdgeNode<EdgeInfo>* nextarc;
};

/*
 * ����ڵ�
 */
template<typename VertexType, typename VertexInfo, typename EdgeInfo>
struct VNode 
{
    // ����
    VertexType val;

	// ������Ϣ
	VertexInfo info;

	// �ñ��Ƿ���� info ��¼
	unsigned int valid : 1;

    // ָ���һ�������ö���Ļ���ָ��
	EdgeNode<EdgeInfo>* firstarc;
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

/*
 * �ڽӱ�洢�ṹ
 * CompareFunc: �Ƚ϶���ֵ�ĺ���ָ�룬������ʽ�� BOOL FunctionName(VertexType v, VertexType w);Ҫ����ͬ����TRUE�����򷵻�FALSE
 */
template<typename VertexType, typename VertexInfo, typename EdgeInfo, typename CompareFunc>
class ALGraph 
{
public:
    ALGraph(CompareFunc func, GraphKind kind = DG);
    virtual ~ALGraph();

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
	virtual BOOL DeleteVexByIndex(const int i);

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

	// ��ͼ�д��ڶ��� value,�򷵻ظö�����ͼ��λ�ã����򷵻�-1
	virtual int LocateVex(const VertexType v);

	// ��ͼ�д��ڶ��� value���򷵻� TRUE�����򷵻� FALSE
	virtual BOOL ExistVex(const VertexType v);
	virtual BOOL ExistVexByIndex(const int i);

	// ��ͼG�ж��� x �ĵ�һ���ڽӵ㡣���У��򷵻ض���ţ��� x û���ڽӵ�򲻴��� x���򷵻�-1��
	virtual int FirstAdjVex(const VertexType v);
	virtual int FirstAdjVexByIndex(int i);

	// ����ͼG�ж��� y �Ƕ��� x ��һ���ڽӵ㣬���س� y ֮�ⶥ�� x ����һ���ڽӵ�Ķ���ţ��� y �� x �����һ���ڽӵ㣬�򷵻� -1��
	virtual int NextAdjVex(const VertexType v, const VertexType w);
	virtual int NextAdjVexByIndex(int i, int j);

	// ��ȡ����Ϊ i �Ķ�����Ϣ
	virtual VertexType GetVexVal(int i) const { return m_szVex[i].val; }

	// ��ȡ��ǰͼ������
	virtual GraphKind GetGraphKind() const { return m_graphKind; }

	// ��ȡ��ǰ��������
	virtual int GetVexNum() const { return m_vexnum; }

	// ��ȡ�ڽӱ�
	virtual const VNode<VertexType, VertexInfo, EdgeInfo>* GetAdjList() const { return m_szVex; }

	// �������
	void PrintInfo();

private:
	// ����Ĭ�Ϲ��캯��
	ALGraph() {}

	BOOL deleteEdge(int i, int j);

private:
    // �ڽӱ�
    VNode<VertexType, VertexInfo, EdgeInfo> m_szVex[MAX_VERTEX_NUM];

    // ͼ��ǰ�Ķ�����
    int m_vexnum;

    // ͼ��ǰ�Ļ���
    int m_arcnum;

    // ͼ�������־
    GraphKind m_graphKind;

	// ����ıȽϺ���
	CompareFunc compare;
};


template<typename VertexType, typename VertexInfo, typename EdgeInfo, typename CompareFunc>
ALGraph<VertexType, VertexInfo, EdgeInfo, CompareFunc>::ALGraph(CompareFunc func, GraphKind kind /*= DG*/)
{
    //memset(m_szVex, 0, sizeof(VNode<VexType, EdgeType>) * MAX_VERTEX_NUM);
    m_arcnum = 0;
    m_vexnum = 0;
    m_graphKind = kind;
	compare = func;
}

template<typename VertexType, typename VertexInfo, typename EdgeInfo, typename CompareFunc>
ALGraph<VertexType, VertexInfo, EdgeInfo, CompareFunc>::~ALGraph()
{

}


template<typename VertexType, typename VertexInfo, typename EdgeInfo, typename CompareFunc>
int ALGraph<VertexType, VertexInfo, EdgeInfo, CompareFunc>::InsertVex(const VertexType v)
{
    int loc = -1;

    if (!ExistVex(v)) {
		VNode<VertexType, VertexInfo, EdgeInfo> vex;
		vex.val = v;
		vex.firstarc = NULL;
		vex.valid = INVALID;

		loc = m_vexnum++;
		m_szVex[loc] = vex;
    }

    return loc;
}

template<typename VertexType, typename VertexInfo, typename EdgeInfo, typename CompareFunc>
inline BOOL ALGraph<VertexType, VertexInfo, EdgeInfo, CompareFunc>::SetVex(const VertexType v, const VertexInfo info, BOOL modify)
{
	return SetVexByIndex(LocateVex(v), info, modify);
}

template<typename VertexType, typename VertexInfo, typename EdgeInfo, typename CompareFunc>
inline BOOL ALGraph<VertexType, VertexInfo, EdgeInfo, CompareFunc>::SetVexByIndex(int i, const VertexInfo info, BOOL modify)
{
	if (i < 0 || i >= m_vexnum) {
		return FALSE;
	}

	if (m_szVex[i].valid == VALID && modify == FALSE) {
		return FALSE;
	}

	m_szVex[i].info = info;
	m_szVex[i].valid = VALID;

	return TRUE;
}

template<typename VertexType, typename VertexInfo, typename EdgeInfo, typename CompareFunc>
inline BOOL ALGraph<VertexType, VertexInfo, EdgeInfo, CompareFunc>::GetVex(const VertexType v, VertexInfo& info)
{
	return GetVexByIndex(LocateVex(v), info);
}

template<typename VertexType, typename VertexInfo, typename EdgeInfo, typename CompareFunc>
inline BOOL ALGraph<VertexType, VertexInfo, EdgeInfo, CompareFunc>::GetVexByIndex(int i, VertexInfo& info)
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
inline BOOL ALGraph<VertexType, VertexInfo, EdgeInfo, CompareFunc>::DeleteVex(const VertexType v)
{
	return DeleteVexByIndex(LocateVex(v));
}

template<typename VertexType, typename VertexInfo, typename EdgeInfo, typename CompareFunc>
inline BOOL ALGraph<VertexType, VertexInfo, EdgeInfo, CompareFunc>::DeleteVexByIndex(const int i)
{
	if (i < 0 || i > m_vexnum) {
		return FALSE;
	}

	// ɾ����
	int j = 0;
	EdgeNode<EdgeInfo>* p = NULL;
	EdgeNode<EdgeInfo>* q = NULL;

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

	memset(&m_szVex[m_vexnum - 1], 0, sizeof(VNode<VertexType, VertexInfo, EdgeInfo>));
	--m_vexnum;

	return TRUE;
}

template<typename VertexType, typename VertexInfo, typename EdgeInfo, typename CompareFunc>
inline BOOL ALGraph<VertexType, VertexInfo, EdgeInfo, CompareFunc>::InsertEdge(VertexType v, VertexType w, EdgeInfo* pInfo /*=NULL*/, BOOL modify)
{
    return InsertEdgeByIndex(LocateVex(v), LocateVex(w), pInfo, modify);
}

template<typename VertexType, typename VertexInfo, typename EdgeInfo, typename CompareFunc>
inline BOOL ALGraph<VertexType, VertexInfo, EdgeInfo, CompareFunc>::InsertEdgeByIndex(int i, int j, EdgeInfo* pInfo /*=NULL*/, BOOL modify)
{
	if (i < 0 || i > m_vexnum || j < 0 || j > m_vexnum) {
		return FALSE;
	}

	BOOL exist = ExistEdgeByIndex(i, j);
	EdgeNode<EdgeInfo>* node = NULL;
	VNode<VertexType, VertexInfo, EdgeInfo>* p = NULL;

	// ������ �� ������
	if (m_graphKind == DN || m_graphKind == UDN) {

		if (exist == TRUE) {
			return TRUE;
		}
		node = new EdgeNode<EdgeInfo>();
		node->valid = VALID;
	}
	// ����ͼ �� ����ͼ
	else {

		if ((exist == TRUE && modify == FALSE) || pInfo == NULL) {
			return FALSE;
		}
		node = new EdgeNode<EdgeInfo>();
		node->valid = VALID;
		node->info = *pInfo;
	}

	p = &m_szVex[i];
	node->adjvex = j;
	node->nextarc = p->firstarc;
	p->firstarc = node; // Insert head
	++m_arcnum;

	if (m_graphKind == UDG || m_graphKind == UDN) {
		// ����ͼ
		p = &m_szVex[j];
		EdgeNode<EdgeInfo>* node2 = new EdgeNode<EdgeInfo>();
		node2->adjvex = i;
		node2->info = node->info;
		node2->valid = node->valid;
		node2->nextarc = p->firstarc;
		p->firstarc = node2;

		++m_arcnum;
	}

	return TRUE;
}

template<typename VertexType, typename VertexInfo, typename EdgeInfo, typename CompareFunc>
inline BOOL ALGraph<VertexType, VertexInfo, EdgeInfo, CompareFunc>::DeleteEdge(VertexType v, VertexType w)
{
	return DeleteEdgeByIndex(LocateVex(v), LocateVex(w));
}

template<typename VertexType, typename VertexInfo, typename EdgeInfo, typename CompareFunc>
inline BOOL ALGraph<VertexType, VertexInfo, EdgeInfo, CompareFunc>::DeleteEdgeByIndex(int i, int j)
{
	if (deleteEdge(i, j) == FALSE) {
		return FALSE;
	}

	if (m_graphKind == UDG || m_graphKind == UDN) {
		deleteEdge(j, i);
	}

	return TRUE;
}

template<typename VertexType, typename VertexInfo, typename EdgeInfo, typename CompareFunc>
inline BOOL ALGraph<VertexType, VertexInfo, EdgeInfo, CompareFunc>::SetEdge(VertexType v, VertexType w, EdgeInfo* pInfo)
{
	return SetEdgeByIndex(LocateVex(v), LocateVex(w), pInfo);
}

template<typename VertexType, typename VertexInfo, typename EdgeInfo, typename CompareFunc>
inline BOOL ALGraph<VertexType, VertexInfo, EdgeInfo, CompareFunc>::SetEdgeByIndex(int i, int j, EdgeInfo* pInfo)
{
	return InsertEdgeByIndex(i, j, pInfo, TRUE);
}

template<typename VertexType, typename VertexInfo, typename EdgeInfo, typename CompareFunc>
inline BOOL ALGraph<VertexType, VertexInfo, EdgeInfo, CompareFunc>::GetEdge(VertexType v, VertexType w, EdgeInfo& info)
{
	return GetEdgeByIndex(LocateVex(v), LocateVex(w), info);
}

template<typename VertexType, typename VertexInfo, typename EdgeInfo, typename CompareFunc>
inline BOOL ALGraph<VertexType, VertexInfo, EdgeInfo, CompareFunc>::GetEdgeByIndex(int i, int j, EdgeInfo& info)
{
	if (i >= 0 && i < m_vexnum && j >= 0 && j < m_vexnum) {

		EdgeNode<EdgeInfo>* p = m_szVex[i].firstarc;

		while (p)
		{
			if (p->adjvex == j) {
				info = p->info;
				return TRUE;
			}

			p = p->nextarc;
		}
	}

	return FALSE;
}

template<typename VertexType, typename VertexInfo, typename EdgeInfo, typename CompareFunc>
inline BOOL ALGraph<VertexType, VertexInfo, EdgeInfo, CompareFunc>::ExistEdge(VertexType v, VertexType w)
{
	return ExistEdgeByIndex(LocateVex(v), LocateVex(w));
}

template<typename VertexType, typename VertexInfo, typename EdgeInfo, typename CompareFunc>
inline BOOL ALGraph<VertexType, VertexInfo, EdgeInfo, CompareFunc>::ExistEdgeByIndex(int i, int j)
{
    if (i >= 0 && i < m_vexnum && j >= 0 && j < m_vexnum) {
		EdgeNode<EdgeInfo>* p = m_szVex[i].firstarc;
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

template<typename VertexType, typename VertexInfo, typename EdgeInfo, typename CompareFunc>
inline int ALGraph<VertexType, VertexInfo, EdgeInfo, CompareFunc>::LocateVex(const VertexType v)
{
    int i = 0;
	for (i; i < m_vexnum; ++i) {
		if (compare(m_szVex[i].val, v) == TRUE) {
			break;
		}
	}

    if (i == m_vexnum) {
        return -1;
    }

    return i;
}

template<typename VertexType, typename VertexInfo, typename EdgeInfo, typename CompareFunc>
inline BOOL ALGraph<VertexType, VertexInfo, EdgeInfo, CompareFunc>::ExistVex(const VertexType v)
{
    return ExistVexByIndex(LocateVex(v));
}

template<typename VertexType, typename VertexInfo, typename EdgeInfo, typename CompareFunc>
inline BOOL ALGraph<VertexType, VertexInfo, EdgeInfo, CompareFunc>::ExistVexByIndex(const int i)
{
	if (i < 0 || i > m_vexnum) {
		return FALSE;
	}
	return TRUE;
}

template<typename VertexType, typename VertexInfo, typename EdgeInfo, typename CompareFunc>
inline int ALGraph<VertexType, VertexInfo, EdgeInfo, CompareFunc>::FirstAdjVex(const VertexType v)
{
	return FirstAdjVexByIndex(LocateVex(v));
}

template<typename VertexType, typename VertexInfo, typename EdgeInfo, typename CompareFunc>
inline int ALGraph<VertexType, VertexInfo, EdgeInfo, CompareFunc>::FirstAdjVexByIndex(int i)
{
	if (i >= 0 && i < m_vexnum) {
		EdgeNode<EdgeInfo>* p = m_szVex[i].firstarc;
		if (p != NULL) {
			return p->adjvex;
		}
	}

	return -1;
}

template<typename VertexType, typename VertexInfo, typename EdgeInfo, typename CompareFunc>
inline int ALGraph<VertexType, VertexInfo, EdgeInfo, CompareFunc>::NextAdjVex(const VertexType v, const VertexType w)
{
	return NextAdjVexByIndex(LocateVex(v), LocateVex(w));
}

template<typename VertexType, typename VertexInfo, typename EdgeInfo, typename CompareFunc>
inline int ALGraph<VertexType, VertexInfo, EdgeInfo, CompareFunc>::NextAdjVexByIndex(int i, int j)
{
	if (i >= 0 && i < m_vexnum && j >= 0 && j < m_vexnum) {
		EdgeNode<EdgeInfo>* p = NULL;
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

template<typename VertexType, typename VertexInfo, typename EdgeInfo, typename CompareFunc>
inline void ALGraph<VertexType, VertexInfo, EdgeInfo, CompareFunc>::PrintInfo()
{
	cout << "��ǰ������ʹ�ÿռ䣺" << m_vexnum << endl;
	cout << "��ǰ����ʹ�õĿռ䣺" << m_arcnum << endl;

	// ��ӡ������Ϣ
	cout << "��ǰ������Ϣ���£�(���� " << m_vexnum << " ������)" << endl;
	for (int i = 0; i < m_vexnum; i++) {
		cout << "�� " << i + 1 << " ������Ϊ��" << m_szVex[i].val << "\t������ϢΪ��";
		if (m_szVex[i].valid == INVALID) {
			cout << "<--û�м�¼������Ϣ-->";
		}
		else {
			cout << m_szVex[i].info;
		}
		cout << endl;
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
		EdgeNode<EdgeInfo>* p = m_szVex[i].firstarc;
		while (p) {
			if (m_graphKind == DG || m_graphKind == DN) {
				cout << "�� " << ++num << " ���� <" << m_szVex[i].val << "," << m_szVex[p->adjvex].val << "> ����ϢΪ: ";
			}
			else {
				cout << "�� " << ++num << " ���� (" << m_szVex[i].val << "," << m_szVex[p->adjvex].val << ") ����ϢΪ: ";
			}

			if (m_graphKind == UDN) {
				cout << "<--������û��Ȩֵ��Ϣ-->" << endl;
			}
			else if (m_graphKind == DN) {
				cout << "<--������û��Ȩֵ��Ϣ-->" << endl;
			}
			else {
				cout << p->info << endl;
			}

			p = p->nextarc;
		}
	}

	// �ڴ��Ų�
	cout << "�ڽӱ��ڴ����У�" << endl;
	for (int i = 0; i < m_vexnum; i++) {
		cout << "[" << i << "]";
		EdgeNode<EdgeInfo>* p = m_szVex[i].firstarc;
		if (p == NULL) {
			cout << "->NULL";
		}
		else {
			while (p) {
				cout << "->" << p->adjvex;
				p = p->nextarc;
			}
		}
		cout << endl;
	}
}

template<typename VertexType, typename VertexInfo, typename EdgeInfo, typename CompareFunc>
inline BOOL ALGraph<VertexType, VertexInfo, EdgeInfo, CompareFunc>::deleteEdge(int i, int j)
{
	if (i < 0 || i > m_vexnum || j < 0 || j > m_vexnum) {
		return FALSE;
	}

	if (!ExistEdgeByIndex(i, j)) {
		return FALSE;
	}

	EdgeNode<EdgeInfo>* p = NULL;
	EdgeNode<EdgeInfo>* q = NULL;

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
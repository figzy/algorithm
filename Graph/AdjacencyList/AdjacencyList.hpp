#ifndef __ADJACENCYLIST_H__
#define __ADJACENCYLIST_H__

#include <iostream>
using namespace std;

#define MAX_VERTEX_NUM 64

typedef int BOOL;
#define TRUE 1
#define FALSE 0

#define EXIST 0x1
#define NOTEXIST 0x0

#define VALID 0x1
#define INVALID 0x0

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


/*
 * �߱�ڵ�
 *
 * ��Ա����
 *  adjvex ���ñ���ָ��Ķ����λ��
 *  info ���ñ���ص���Ϣ
 *  valid ���ñ��Ƿ���� info ��¼��VALID��˵������info��Ϣ��INVALID˵��������info��Ϣ
 *  nextarc ��ָ����һ���ߵ�ָ��
 */
template<typename EdgeInfo>
struct EdgeNode
{
    int adjvex;
    EdgeInfo info;
    unsigned int valid : 1;
    struct EdgeNode<EdgeInfo>* nextarc;
};

/*
 * ����ڵ�
 *
 * ��Ա����
 *  val ������ֵ
 *  info ��������Ϣ
 *  valid ���ñ��Ƿ���� info ��¼��VALID��˵������info��Ϣ��INVALID˵��������info��Ϣ
 *  firstarc ��ָ���һ�������ö���Ļ���ָ��
 */
template<typename VertexType, typename VertexInfo, typename EdgeInfo>
struct VNode 
{
    VertexType val;
	VertexInfo info;
	unsigned int valid : 1;
	EdgeNode<EdgeInfo>* firstarc;
};

/*
 * �ڽӱ�洢�ṹ
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
class ALGraph 
{
public:
    ALGraph(CompareFunc func, GraphKind kind = DG);
    virtual ~ALGraph();

	virtual int InsertVex(const VertexType& v);

	virtual BOOL SetVex(const VertexType& v, const VertexInfo& info, BOOL modify = TRUE);
	virtual BOOL SetVexByIndex(int i, const VertexInfo& info, BOOL modify = TRUE);

	virtual BOOL GetVex(const VertexType& v, VertexInfo& info);
	virtual BOOL GetVexByIndex(int i, VertexInfo& info);

	virtual BOOL DeleteVex(const VertexType& v);
	virtual BOOL DeleteVexByIndex(int i);

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

	virtual int LocateVex(const VertexType& v);

	virtual BOOL ExistVex(const VertexType& v);
	virtual BOOL ExistVexByIndex(int i);

	virtual int FirstAdjVex(const VertexType& v);
	virtual int FirstAdjVexByIndex(int i);

	virtual int NextAdjVex(const VertexType& v, const VertexType& w);
	virtual int NextAdjVexByIndex(int i, int j);

	virtual VertexType GetVexVal(int i) const;
	virtual GraphKind GetGraphKind() const;
	virtual int GetVexNum() const;
	virtual const VNode<VertexType, VertexInfo, EdgeInfo>* GetAdjList() const;

	// �������
	void PrintInfo();

private:
	// ����Ĭ�Ϲ��캯��
	ALGraph() {}
	BOOL deleteEdge(int i, int j);

private:
    VNode<VertexType, VertexInfo, EdgeInfo> m_szVex[MAX_VERTEX_NUM];// �ڽӱ�
    int m_vexnum;// ͼ��ǰ�Ķ�����
    int m_arcnum;// ͼ��ǰ�Ļ���
    GraphKind m_graphKind;// ͼ�������־
	CompareFunc compare;// ����ıȽϺ���
};

/*
 * ���캯��
 *
 * ����
 *  func ������ֵ�ȽϺ���ָ��
 *  kind ��ͼ���͡�Ĭ��Ϊ����ͼ DG
 */
template<typename VertexType, typename VertexInfo, typename EdgeInfo, typename CompareFunc>
ALGraph<VertexType, VertexInfo, EdgeInfo, CompareFunc>::ALGraph(CompareFunc func, GraphKind kind /*= DG*/)
{
    //memset(m_szVex, 0, sizeof(VNode<VexType, EdgeType>) * MAX_VERTEX_NUM);
    m_arcnum = 0;
    m_vexnum = 0;
    m_graphKind = kind;
	compare = func;
}

/*
 * ��������
 */
template<typename VertexType, typename VertexInfo, typename EdgeInfo, typename CompareFunc>
ALGraph<VertexType, VertexInfo, EdgeInfo, CompareFunc>::~ALGraph()
{
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
int ALGraph<VertexType, VertexInfo, EdgeInfo, CompareFunc>::InsertVex(const VertexType& v)
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
inline BOOL ALGraph<VertexType, VertexInfo, EdgeInfo, CompareFunc>::SetVex(const VertexType& v, const VertexInfo& info, BOOL modify)
{
	return SetVexByIndex(LocateVex(v), info, modify);
}

template<typename VertexType, typename VertexInfo, typename EdgeInfo, typename CompareFunc>
inline BOOL ALGraph<VertexType, VertexInfo, EdgeInfo, CompareFunc>::SetVexByIndex(int i, const VertexInfo& info, BOOL modify)
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
inline BOOL ALGraph<VertexType, VertexInfo, EdgeInfo, CompareFunc>::GetVex(const VertexType& v, VertexInfo& info)
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
inline BOOL ALGraph<VertexType, VertexInfo, EdgeInfo, CompareFunc>::DeleteVex(const VertexType& v)
{
	return DeleteVexByIndex(LocateVex(v));
}

template<typename VertexType, typename VertexInfo, typename EdgeInfo, typename CompareFunc>
inline BOOL ALGraph<VertexType, VertexInfo, EdgeInfo, CompareFunc>::DeleteVexByIndex(int i)
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
inline BOOL ALGraph<VertexType, VertexInfo, EdgeInfo, CompareFunc>::InsertEdge(const VertexType& v, const VertexType& w, EdgeInfo* pInfo /*=NULL*/, BOOL modify)
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
inline BOOL ALGraph<VertexType, VertexInfo, EdgeInfo, CompareFunc>::DeleteEdge(const VertexType& v, const VertexType& w)
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
inline BOOL ALGraph<VertexType, VertexInfo, EdgeInfo, CompareFunc>::SetEdge(const VertexType& v, const VertexType& w, EdgeInfo* pInfo)
{
	return SetEdgeByIndex(LocateVex(v), LocateVex(w), pInfo);
}

template<typename VertexType, typename VertexInfo, typename EdgeInfo, typename CompareFunc>
inline BOOL ALGraph<VertexType, VertexInfo, EdgeInfo, CompareFunc>::SetEdgeByIndex(int i, int j, EdgeInfo* pInfo)
{
	return InsertEdgeByIndex(i, j, pInfo, TRUE);
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
inline BOOL ALGraph<VertexType, VertexInfo, EdgeInfo, CompareFunc>::GetEdge(const VertexType& v, const VertexType& w, EdgeInfo& info)
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
inline BOOL ALGraph<VertexType, VertexInfo, EdgeInfo, CompareFunc>::ExistEdge(const VertexType& v, const VertexType& w)
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
inline int ALGraph<VertexType, VertexInfo, EdgeInfo, CompareFunc>::LocateVex(const VertexType& v)
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
inline BOOL ALGraph<VertexType, VertexInfo, EdgeInfo, CompareFunc>::ExistVex(const VertexType& v)
{
    return ExistVexByIndex(LocateVex(v));
}

template<typename VertexType, typename VertexInfo, typename EdgeInfo, typename CompareFunc>
inline BOOL ALGraph<VertexType, VertexInfo, EdgeInfo, CompareFunc>::ExistVexByIndex(int i)
{
	if (i < 0 || i > m_vexnum) {
		return FALSE;
	}
	return TRUE;
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
inline int ALGraph<VertexType, VertexInfo, EdgeInfo, CompareFunc>::FirstAdjVex(const VertexType& v)
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
inline int ALGraph<VertexType, VertexInfo, EdgeInfo, CompareFunc>::NextAdjVex(const VertexType& v, const VertexType& w)
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

/*
 * ��ȡ����Ϊ i �Ķ�����Ϣ
 */
template<typename VertexType, typename VertexInfo, typename EdgeInfo, typename CompareFunc>
inline VertexType ALGraph<VertexType, VertexInfo, EdgeInfo, CompareFunc>::GetVexVal(int i) const
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
inline GraphKind ALGraph<VertexType, VertexInfo, EdgeInfo, CompareFunc>::GetGraphKind() const
{
	return m_graphKind;
}

/*
 * ��ȡ��ǰ��������
 */
template<typename VertexType, typename VertexInfo, typename EdgeInfo, typename CompareFunc>
inline int ALGraph<VertexType, VertexInfo, EdgeInfo, CompareFunc>::GetVexNum() const
{
	return m_vexnum;
}

/*
 * 	��ȡ�ڽӱ�
 */
template<typename VertexType, typename VertexInfo, typename EdgeInfo, typename CompareFunc>
inline const VNode<VertexType, VertexInfo, EdgeInfo>* ALGraph<VertexType, VertexInfo, EdgeInfo, CompareFunc>::GetAdjList() const
{
	return m_szVex;
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
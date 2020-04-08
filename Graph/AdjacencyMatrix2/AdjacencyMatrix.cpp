#include "AdjacencyMatrix.h"
#include <string>
#include <iostream>
using namespace std;


MGraph::MGraph(GraphKind kind)
{
	if (kind == DG || kind == DN) {

		// ��������ͼ�����������ڽӾ���
		m_pDArcs = new ArcCell * [MAX_VERTEX_NUM]();
		for (int i = 0; i < MAX_VERTEX_NUM; i++) {
			m_pDArcs[i] = new ArcCell[MAX_VERTEX_NUM]();
		}
		m_pUDArcs = NULL;
	}
	else if (kind == UDG || kind == UDN) {

		// ��������ͼ����������������ѹ������
		int nUDSize = (1 + MAX_VERTEX_NUM) * MAX_VERTEX_NUM / 2;
		m_pUDArcs = new ArcCell[nUDSize]();
		m_pDArcs = NULL;
	}

	memset(m_szVex, 0, sizeof(VNode) * MAX_VERTEX_NUM);
	m_vexnum = 0;
	m_arcnum = 0;
	m_graphKind = kind;
}


MGraph::~MGraph()
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


int MGraph::InsertVex(const VexType v, size_t len)
{
	int loc = -1;

	do
	{
		if (len <= 0){
			break;
		}

		if (m_vexnum >= MAX_VERTEX_NUM) {
			break;
		}

		if (LocateVex(v, len) != -1) {
			break;
		}

		VNode node;
		node.vex = sdsnewlen(v, len);
		node.info = sdsempty();

		m_szVex[m_vexnum] = node;

		loc = ++m_vexnum;

	} while (0);

	return loc;
}


inline BOOL MGraph::SetVex(const VexType v, size_t vLen, const VexInfo info, size_t infoLen, BOOL modify /*=TRUE*/)
{
	return SetVexByIndex(LocateVex(v, vLen), info, infoLen, TRUE);
}


inline BOOL MGraph::SetVexByIndex(int i, const VexInfo info, size_t infoLen, BOOL modify /*=TRUE*/)
{
	BOOL bRet = FALSE;

	do 
	{
		if (i < 0 || i >= m_vexnum) {
			break;
		}

		if (infoLen <= 0) {
			break;
		}

		if (sdslen(m_szVex[i].info) != 0 && modify == FALSE) {
			break;
		}

		m_szVex[i].info = sdscpylen(m_szVex[i].info, info, infoLen);
		if (m_szVex[i].info == NULL) {
			// ��չ�ڴ�ʧ��
			sdsfree(m_szVex[i].info);
			m_szVex[i].info = sdsempty();
			break;
		}

		bRet = TRUE;

	} while (0);

	return bRet;
}


inline VexInfo MGraph::GetVex(const VexType v, size_t len)
{
	return GetVexByIndex(LocateVex(v, len));
}


inline VexInfo MGraph::GetVexByIndex(int i)
{
	if (i >= 0 && i < m_vexnum) {
		return sdsdup(m_szVex[i].info);
	}

	return NULL;
}


BOOL MGraph::DeleteVex(const VexType v, size_t len)
{
	return DeleteVexByIndex(LocateVex(v, len));
}


inline BOOL MGraph::DeleteVexByIndex(const int loc)
{
	if (loc < 0 || loc >= m_vexnum) {
		return FALSE;
	}

	// ɾ�� m_szVex ������Ϊ loc �Ķ���
	sdsfree(m_szVex[loc].vex);
	sdsfree(m_szVex[loc].info);
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
					sdsfree(m_pDArcs[l][loc].info);
					--m_arcnum;
				}

				for (c = loc; c < m_vexnum - 1; ++c) {
					m_pDArcs[l][c] = m_pDArcs[l][c + 1];
				}
			}
			else if (l == loc) {

				for (c = 0; c < m_vexnum; ++c) {
					if (m_pDArcs[l][c].exist == EXIST) {
						sdsfree(m_pDArcs[l][c].info);
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
							sdsfree(m_pDArcs[l][c].info);
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
						sdsfree(m_pUDArcs[i].info);
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


BOOL MGraph::InsertArc(const VexType v, size_t vlen, const VexType w, size_t wlen, ArcInfo info /*= NULL*/, size_t infolen /*= 0*/, BOOL modify)
{
	return InsertArcByIndex(LocateVex(v, vlen), LocateVex(w, wlen), info, infolen, modify);
}


inline BOOL MGraph::InsertArcByIndex(int i, int j, ArcInfo info /*=NULL*/, size_t infolen /*= 0*/, BOOL modify)
{
	if (infolen <= 0) {
		return FALSE;
	}

	if (i < 0 || i >= m_vexnum || j < 0 || j >= m_vexnum) {
		return FALSE;
	}

	if (ExistArcByIndex(i, j) == TRUE && modify == FALSE) {
		return FALSE;
	}

	ArcCell node;
	memset(&node, 0, sizeof(ArcCell));
	node.exist = EXIST;
	node.info = sdsnewlen(info, infolen);

	if (m_graphKind == DG || m_graphKind == DN) {
		m_pDArcs[i][j] = node;
	}
	else if (m_graphKind == UDG || m_graphKind == UDN) {
		m_pUDArcs[ArcOffsetLocByIndex(i, j)] = node;
	}

	++m_arcnum;

	return TRUE;
}


BOOL MGraph::DeleteArc(const VexType v, size_t vlen, const VexType w, size_t wlen)
{
	return DeleteArcByIndex(LocateVex(v, vlen), LocateVex(w, wlen));
}


inline BOOL MGraph::DeleteArcByIndex(int i, int j)
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
				sdsfree((sds)(m_pDArcs[i][j].info));
				--m_arcnum;
				bRet = TRUE;
			}
		}
		else if (m_graphKind == UDG || m_graphKind == UDN) {

			int loc = ArcOffsetLocByIndex(i, j);
			if (m_pUDArcs[loc].exist == EXIST) {
				m_pUDArcs[loc].exist = NOTEXIST;
				sdsfree((sds)(m_pUDArcs[loc].info));
				--m_arcnum;
				bRet = TRUE;
			}
		}
	} while (0);

	return bRet;
}


inline int MGraph::FirstAdjVex(const VexType v, size_t len)
{
	return FirstAdjVexByIndex(LocateVex(v, len));
}


int MGraph::FirstAdjVexByIndex(int i)
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


inline int MGraph::NextAdjVex(const VexType v, size_t vlen, const VexType w, size_t wlen)
{
	return NextAdjVexByIndex(LocateVex(v, vlen), LocateVex(w, wlen));
}


int MGraph::NextAdjVexByIndex(int i, int j)
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


ArcInfo MGraph::GetArc(const VexType v, size_t vlen, const VexType w, size_t wlen)
{
	return GetArcByIndex(LocateVex(v, vlen), LocateVex(w, wlen));
}


inline ArcInfo MGraph::GetArcByIndex(int i, int j)
{
	if (i >= 0 && i < m_vexnum && j >= 0 && j < m_vexnum) {
		if (m_graphKind == DG || m_graphKind == DN) {

			if ((m_pDArcs[i][j]).exist == EXIST) {
				return sdsdup(m_pDArcs[i][j].info);
			}
		}
		else if (m_graphKind == UDG || m_graphKind == UDN) {

			int loc = ArcOffsetLocByIndex(i, j);
			if (m_pUDArcs[loc].exist == EXIST) {
				return sdsdup(m_pUDArcs[loc].info);
			}
		}

	}

	return NULL;
}


BOOL MGraph::SetArc(const VexType v, size_t vlen, const VexType w, size_t wlen, ArcInfo info)
{
	return InsertArcByIndex(LocateVex(v, vlen), LocateVex(w, wlen), info, TRUE);
}


inline BOOL MGraph::SetArcByIndex(int i, int j, ArcInfo info)
{
	if (sdslen(info) == 0) {
		return FALSE;
	}

	return InsertArcByIndex(i, j, info, TRUE);
}


int MGraph::LocateVex(const VexType v, size_t len)
{
	if (len <= 0) {
		return -1;
	}

	for (int i = 0; i < m_vexnum; ++i) {
		if (sdscmpstr(m_szVex[i].vex, v, len) == 0) {
			return i;
		}
	}

	return -1;
}


BOOL MGraph::ExistVex(const VexType v, size_t len)
{
	return ExistVexByIndex(LocateVex(v, len));
}


inline BOOL MGraph::ExistVexByIndex(const int i)
{
	if (i < 0 || i > m_vexnum) {
		return FALSE;
	}

	return TRUE;
}


inline BOOL MGraph::ExistArc(const VexType v, size_t vlen, const VexType w, size_t wlen)
{
	return ExistArcByIndex(LocateVex(v, vlen), LocateVex(w, wlen));
}


inline BOOL MGraph::ExistArcByIndex(int i, int j)
{
	if (i >= 0 && i < m_vexnum && j >= 0 && j < m_vexnum) {

		if (m_graphKind == DG || m_graphKind == DN) {
			if ((m_pDArcs[i][j]).exist == EXIST) {
				return TRUE;
			}
		}
		else if (m_graphKind == UDG || m_graphKind == UDN) {

			int loc = ArcOffsetLocByIndex(i, j);
			if (m_pUDArcs[loc].exist == EXIST) {
				return TRUE;
			}
		}
	}

	return FALSE;
}

inline int MGraph::ArcOffsetLoc(const VexType v, size_t vlen, const VexType w, size_t wlen)
{
	return ArcOffsetLocByIndex(LocateVex(v, vlen), LocateVex(w, wlen));
}


int MGraph::ArcOffsetLocByIndex(int i, int j)
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

void MGraph::PrintInfo()
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
		cout << "�� " << i + 1 << " �������ֵ:" << string(m_szVex[i].vex, sdslen(m_szVex[i].vex)) << "   �������ϢΪ:";
		if (0 != sdslen(m_szVex[i].info)) {
			cout << string(m_szVex[i].info, sdslen(m_szVex[i].info));
		}
		cout << endl;
	}

	// ��ӡ����Ϣ
	if (m_graphKind == DG || m_graphKind == DN) {
		cout << "��ǰ����߹��У�" << m_arcnum << "��" << endl;
		int num = 0;
		for (int i = 0; i < m_vexnum; i++) {
			for (int j = 0; j < m_vexnum; j++) {
				if (m_pDArcs[i][j].exist == EXIST) {
					cout << "�� " << ++num << " ���� <" << GetVexVal(i) << "," << GetVexVal(j) << "> ����ϢΪ:" << m_pDArcs[i][j].info << endl;
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
				loc = ArcOffsetLocByIndex(i, j);
				if (m_pUDArcs[loc].exist == EXIST) {
					cout << "�� " << ++num << " ���� (" << GetVexVal(i) << "," << GetVexVal(j) << ") ����ϢΪ:" << m_pUDArcs[loc].info << endl;
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

		for (int i = 0; i < m_vexnum; i++) {
			cout << m_szVex[i].vex << " ";
		}
		cout << endl;

		for (j = 0; j < m_vexnum; j++) {
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

		for (int i = 0; i < m_vexnum; i++) {
			cout << m_szVex[i].vex << " ";
		}
		cout << endl;

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
			}
			else {
				j++;
			}
		}
	}
}



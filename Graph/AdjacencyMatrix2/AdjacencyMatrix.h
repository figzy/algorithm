#ifndef __ADJACENCYMATRIX_H__
#define __ADJACENCYMATRIX_H__

#include "define.h"
#include <memory>

class MGraph
{
public:
	MGraph(GraphKind typ = UDG);
	virtual ~MGraph();

	// ��ͼG�в��붥��, ���ص�ǰ�����������; len: ����v��ռ�õĿռ��С
	virtual int InsertVex(const VexType v, size_t len);

	// ���ö�����Ϣ. ������㲻���ڣ��򷵻�FALSE�������¼������Ϣ
	// modify �ֶ�: = TRUE,�򵱶�����Ϣ����ʱ�����޸Ķ�����Ϣ�������ڶ�����Ϣ���ڵ�����£�ֱ�ӷ��� FALSE
	virtual BOOL SetVex(const VexType v, size_t vLen, const VexInfo info, size_t infoLen, BOOL modify = TRUE);
	virtual BOOL SetVexByIndex(int i, const VexInfo info, size_t infolen, BOOL modify = TRUE);
	
	// ��ȡ������Ϣ
	virtual VexInfo GetVex(const VexType v, size_t len);
	virtual VexInfo GetVexByIndex(int i);

	// ��ͼG��ɾ������
	virtual BOOL DeleteVex(const VexType v, size_t len);
	virtual BOOL DeleteVexByIndex(const int loc);

	// �������(x, y)�������<x, y>�����ڣ�����ͼG����Ӹñߡ�
	// ��� modify == true�����ڴ��� Edge ������½����޸ĸñߵĻ���Ϣ����� modify Ϊ false���ڱ��Ѿ����ڵ�����»᷵�� FALSE
	virtual BOOL InsertArc(const VexType v, size_t vlen, const VexType w, size_t wlen, ArcInfo info = NULL, size_t infolen = 0, BOOL modify = TRUE);
	virtual BOOL InsertArcByIndex(int i, int j, ArcInfo info = NULL, size_t infolen = 0, BOOL modify = TRUE);

	// �������(x, y)�������<x, y>���ڣ����ͼG��ɾ���ñߡ����߲������򷵻� FALSE�����óɹ����� TRUE��
	virtual BOOL DeleteArc(const VexType v, size_t vlen, const VexType w, size_t wlen);
	virtual BOOL DeleteArcByIndex(int i, int j);

	// ��ͼG�ж��� x �ĵ�һ���ڽӵ㡣���У��򷵻ض���ţ��� x û���ڽӵ�򲻴��� x���򷵻�-1��
	virtual int FirstAdjVex(const VexType v, size_t len);
	virtual int FirstAdjVexByIndex(int i);

	// ����ͼG�ж��� y �Ƕ��� x ��һ���ڽӵ㣬���س� y ֮�ⶥ�� x ����һ���ڽӵ�Ķ���ţ��� y �� x �����һ���ڽӵ㣬�򷵻� -1��
	virtual int NextAdjVex(const VexType v, size_t vlen, const VexType w, size_t wlen);
	virtual int NextAdjVexByIndex(int i, int j);

	// ��ȡͼG�б�(x, y)��<x, y>��Ӧ��Ȩֵ��
	virtual ArcInfo GetArc(const VexType v, size_t vlen, const VexType w, size_t wlen);
	virtual ArcInfo GetArcByIndex(int i, int j);

	// ����ͼG�б�(x, y)��<x, y>��Ӧ��Ȩֵ value�����߲������򷵻� FALSE�����óɹ����� TRUE��
	virtual BOOL SetArc(const VexType v, size_t vlen, const VexType w, size_t wlen, ArcInfo info);
	virtual BOOL SetArcByIndex(int i, int j, ArcInfo info);

	// ��ͼ�д��ڶ��� value,�򷵻ظö�����ͼ��λ�ã����򷵻�-1
	virtual int LocateVex(const VexType v, size_t len);

	// ��ͼ�д��ڶ��� value���򷵻� TRUE�����򷵻� FALSE
	virtual BOOL ExistVex(const VexType v, size_t len);
	virtual BOOL ExistVexByIndex(const int i);

	// �Ƿ���ڱߣ�v, w���� <v, w>
	virtual BOOL ExistArc(const VexType v, size_t vlen, const VexType w, size_t wlen);
	virtual BOOL ExistArcByIndex(int i, int j);

	// ��ȡ����Ϊ i �Ķ�����Ϣ
	virtual VexType GetVexVal(int i) const { return m_szVex[i].vex; }

	// ��ȡ��ǰͼ������
	virtual GraphKind GetGraphKind() const { return m_graphKind; }

	// ��ȡ��ǰ��������
	virtual int GetVexNum() const { return m_vexnum; }

	// ����ͼ�бߵ�ƫ�Ƶ�ַ
	virtual int ArcOffsetLoc(const VexType v, size_t vlen, const VexType w, size_t wlen);
	virtual int ArcOffsetLocByIndex(int i, int j);

	// ��ӡ����
	virtual void PrintInfo();

private:
	// �����
	VNode m_szVex[MAX_VERTEX_NUM];
	
	// ����ͼ�ڽӾ���
	ArcCell** m_pDArcs;

	// ����ͼ,����������ѹ���洢
	ArcCell* m_pUDArcs;

	// ͼ�ĵ�ǰ������
	int m_vexnum;

	// ͼ�Ļ���
	int m_arcnum;

	// ͼ���ͣ�����ͼ �� ����ͼ
	GraphKind m_graphKind;
};

#endif // !__ADJACENCYMATRIX_H__
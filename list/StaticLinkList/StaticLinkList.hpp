#pragma once

#define MAXSIZE 32
#define ENDTAG -1 // �ռ�ĩβ���
#define ERROR -1

template<typename ElemType>
struct SLNode
{
	ElemType data;
	int cur;
};

template<typename ElemType>
class SLList 
{
public:
	SLList();
	~SLList() {}

	bool Insert(int i, const ElemType e);
	bool Delete(int i, ElemType& e);
	bool Modify(int i, ElemType e);
	bool Get(int i, ElemType& e);
	int Len() const { return m_len; }
	int Cap() const { return m_cap; }

private:
	// ���ؿɷ���Ŀռ�����
	int Malloc();
	// �ͷ�����Ϊ i �Ŀռ�
	void Free(int i);
	// ��ȡ�� i ��Ԫ�ص�ǰ��ָ��
	int Prior(int i);
	// ��ȡָ��� i ��Ԫ�ص�ָ��
	int Locate(int i);

private:
	SLNode<ElemType> m_sllist[MAXSIZE + 2];
	int m_len; // ʹ�õĿռ���
	int m_cap; // ��̬����洢Ԫ�ص�������С
};

template<typename ElemType>
bool SLList<ElemType>::Get(int i, ElemType& e)
{
	int k = Locate(i);
	if (k == ERROR) {
		return false;
	}

	e = m_sllist[k].data;
	return true;
}

template<typename ElemType>
int SLList<ElemType>::Locate(int i)
{
	int k = Prior(i);
	if (k == ERROR) {
		return ERROR;
	}

	return m_sllist[k].cur;
}

template<typename ElemType>
bool SLList<ElemType>::Modify(int i, ElemType e)
{
	int k = Locate(i);
	if (k == ERROR) {
		return false;
	}

	m_sllist[k].data = e;
	return true;
}

template<typename ElemType>
void SLList<ElemType>::Free(int i)
{
	// ͷ�巨��������Ԫ�ز����������(m_sllist[0].cur)��ͷ��
	m_sllist[i].cur = m_sllist[0].cur;
	m_sllist[0].cur = i;
	--m_len;
}

template<typename ElemType>
int SLList<ElemType>::Malloc()
{
	int i = m_sllist[0].cur;
	if (i == MAXSIZE + 1) {
		return ERROR;
	}
	
	m_sllist[0].cur = m_sllist[i].cur;
	++m_len;
	return i;
}

template<typename ElemType>
int SLList<ElemType>::Prior(int i)
{
	if (i < 1 || i > m_len) {
		return ERROR;
	}

	int k = MAXSIZE + 1;
	for (int j = 1; j < i; j++) {
		k = m_sllist[k].cur;
	}

	return k;
}

template<typename ElemType>
bool SLList<ElemType>::Delete(int i, ElemType& e)
{
	int k = Prior(i);
	if (k == ERROR) {
		return false;
	}

	int j;
	j = m_sllist[k].cur;
	e = m_sllist[j].data;
	m_sllist[k].cur = m_sllist[j].cur;
	Free(j);
	
	return true;
}

template<typename ElemType>
bool SLList<ElemType>::Insert(int i, const ElemType e)
{
	if (i < 1 || i > m_len + 1) {
		return false;
	}

	int v = Malloc();
	if (v == ERROR) {
		return false;
	}

	// �ҵ��� i ���ڵ��ǰ�� k
	int k = Prior(i);
	if (k == ERROR) {
		return false;
	}

	// ����ͷ�巨����Ԫ��
	m_sllist[v].data = e;
	m_sllist[v].cur = m_sllist[k].cur;
	m_sllist[k].cur = v;

	return true;
}

template<typename ElemType>
inline SLList<ElemType>::SLList()
{
	/*
	* ���� 0 ��Ԫ��ָ��ǰ���пռ���±�
	* ����Ϊ MAXSIZE + 1 ��Ԫ��ָ�������ͷ�ڵ�
	*/
	for (int i = 0; i < MAXSIZE + 1; ++i) {
		m_sllist[i].cur = i + 1;
	}
	m_sllist[MAXSIZE + 1].cur = ENDTAG; 

	m_len = 0;
	m_cap = MAXSIZE;
}
#pragma once

#define ININITY (~(1 << (sizeof(int) * 8 - 1)))

typedef int ElementType;
typedef struct SkipNode{
	ElementType elem;
	SkipNode *right;
	SkipNode *down;
}SkipNode, *SkipList, *Position;

/*
 * ȷ������Ծ��1-2-3 ȷ������Ծ��
 */
class CSkipList
{
public:
	CSkipList();
	~CSkipList();

	Position Find(ElementType elem);
	void Insert(ElementType elem);
	void Delete(ElementType elem);

private:
	SkipList m_skiplist;
	Position m_bottom;
	Position m_tail;
};


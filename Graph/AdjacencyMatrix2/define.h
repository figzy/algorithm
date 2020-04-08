#ifndef __DEFINE_H__
#define __DEFINE_H__
#include "sds.h"

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

typedef sds VexType;
typedef sds VexInfo;
typedef sds ArcInfo;

/*
 * ����߽ڵ����Ľṹ
*/
struct ArcCell
{
	// �ñߴ��� exist = EXIST; �������� exist = NOTEXIST
	unsigned int exist : 1;

	// �ñߵ������Ϣ
	ArcInfo info;
};

/*
 * ���涥��ڵ����Ľṹ
 */
struct VNode
{
	// ����ֵ
	VexType vex;

	// ������Ϣ
	VexInfo info;
};

#endif // !__DEFINE_H__

#include "sds.h"
#include <string.h>
#include <assert.h>
#include <stdio.h>
#include <stdarg.h>
#include <ctype.h>
#include <malloc.h>

/*
 * ���ݸ����ĳ�ʼ���ַ��� init ���ַ������� initlen
 * ����һ���µ� sds
 *
 * ����
 *  init ����ʼ���ַ���ָ��
 *  initlen ����ʼ���ַ����ĳ���
 *
 * ����ֵ
 *  sds �������ɹ����� sdshdr ���Ӧ�� sds
 *        ����ʧ�ܷ��� NULL
 */
sds sdsnewlen(const void* init, size_t initlen) {

    struct sdshdr* sh;

    // �����Ƿ��г�ʼ�����ݣ�ѡ���ʵ����ڴ���䷽ʽ
    if (init) {
        sh = (sdshdr*)malloc(sizeof(struct sdshdr) + initlen + 1);
    }
    else {
        sh = (sdshdr*)calloc(sizeof(struct sdshdr) + initlen + 1, 1);
    }

    // �ڴ����ʧ�ܣ�����
    if (sh == NULL) return NULL;

    // ���ó�ʼ������
    sh->len = initlen;
    // �� sds ��Ԥ���κοռ�
    sh->free = 0;
    // �����ָ����ʼ�����ݣ������Ǹ��Ƶ� sdshdr �� buf ��
    // T = O(N)
    if (initlen && init)
        memcpy(sh->buf, init, initlen);
    // �� \0 ��β
    sh->buf[initlen] = '\0';

    // ���� buf ���֣����������� sdshdr
    return (char*)sh->buf;
}

/*
 * ����������һ��ֻ�����˿��ַ��� "" �� sds
 *
 * ����ֵ
 *  sds �������ɹ����� sdshdr ���Ӧ�� sds
 *        ����ʧ�ܷ��� NULL
 *
 * ���Ӷ�
 *  T = O(1)
 */
sds sdsempty(void) {
    return sdsnewlen("", 0);
}

/*
 * ���ݸ����ַ��� init ������һ������ͬ���ַ����� sds
 *
 * ����
 *  init ���������Ϊ NULL ����ô����һ���հ� sds
 *         �����´����� sds �а����� init ������ͬ�ַ���
 *
 * ����ֵ
 *  sds �������ɹ����� sdshdr ���Ӧ�� sds
 *        ����ʧ�ܷ��� NULL
 *
 * ���Ӷ�
 *  T = O(N)
 */
sds sdsnew(const char* init) {
    size_t initlen = (init == NULL) ? 0 : strlen(init);
    return sdsnewlen(init, initlen);
}

/*
 * ���Ƹ��� sds �ĸ���
 *
 * ����ֵ
 *  sds �������ɹ��������� sds �ĸ���
 *        ����ʧ�ܷ��� NULL
 *
 * ���Ӷ�
 *  T = O(N)
 */
sds sdsdup(const sds s) {
    return sdsnewlen(s, sdslen(s));
}

/*
 * �ͷŸ����� sds
 *
 * ���Ӷ�
 *  T = O(N)
 */
void sdsfree(sds s) {
    if (s == NULL) return;
    free(s - sizeof(struct sdshdr));
}

/*
 * �ڲ��ͷ� SDS ���ַ����ռ������£�
 * ���� SDS ��������ַ���Ϊ���ַ�����
 *
 * ���Ӷ�
 *  T = O(1)
 */
void sdsclear(sds s) {

    // ȡ�� sdshdr
    struct sdshdr* sh = (sdshdr*)(s - (sizeof(struct sdshdr)));

    // ���¼�������
    sh->free += sh->len;
    sh->len = 0;

    // ���������ŵ���ǰ�棨�൱�ڶ��Ե�ɾ�� buf �е����ݣ�
    sh->buf[0] = '\0';
}

 /*
  * �� sds �� buf �ĳ��Ƚ�����չ��ȷ���ں���ִ��֮��
  * buf ���ٻ��� addlen + 1 ���ȵĿ���ռ�
  * ������� 1 �ֽ���Ϊ \0 ׼���ģ�
  *
  * ����ֵ
  *  sds ����չ�ɹ�������չ��� sds
  *        ��չʧ�ܷ��� NULL
  *
  * ���Ӷ�
  *  T = O(N)
  */
sds sdsMakeRoomFor(sds s, size_t addlen) {

    struct sdshdr* sh, * newsh;

    // ��ȡ s Ŀǰ�Ŀ���ռ䳤��
    size_t free = sdsavail(s);

    size_t len, newlen;

    // s Ŀǰ�Ŀ���ռ��Ѿ��㹻�������ٽ�����չ��ֱ�ӷ���
    if (free >= addlen) return s;

    // ��ȡ s Ŀǰ��ռ�ÿռ�ĳ���
    len = sdslen(s);
    sh = (sdshdr*)(s - (sizeof(struct sdshdr)));

    // s ������Ҫ�ĳ���
    newlen = (len + addlen);

    // �����³��ȣ�Ϊ s �����¿ռ�����Ĵ�С
    if (newlen < SDS_MAX_PREALLOC)
        // ����³���С�� SDS_MAX_PREALLOC 
        // ��ôΪ���������������賤�ȵĿռ�
        newlen *= 2;
    else
        // ���򣬷��䳤��ΪĿǰ���ȼ��� SDS_MAX_PREALLOC
        newlen += SDS_MAX_PREALLOC;
    // T = O(N)
    newsh = (sdshdr*)realloc(sh, sizeof(struct sdshdr) + newlen + 1);

    // �ڴ治�㣬����ʧ�ܣ�����
    if (newsh == NULL) return NULL;

    // ���� sds �Ŀ��೤��
    newsh->free = newlen - len;

    // ���� sds
    return newsh->buf;
}

/*
 * ���� sds �еĿ��пռ䣬
 * ���ղ���� sds �б�����ַ����������κ��޸ġ�
 *
 * ����ֵ
 *  sds ���ڴ������� sds
 *
 * ���Ӷ�
 *  T = O(N)
 */
sds sdsRemoveFreeSpace(sds s) {
    struct sdshdr* sh;

    sh = (sdshdr*)(s - (sizeof(struct sdshdr)));

    // �����ڴ��ط��䣬�� buf �ĳ��Ƚ����㹻�����ַ�������
    // T = O(N)
    sh = (sdshdr*)realloc(sh, sizeof(struct sdshdr) + sh->len + 1);

    // ����ռ�Ϊ 0
    sh->free = 0;

    return sh->buf;
}

/*
 * ���ظ��� sds ������ڴ��ֽ���
 *
 * ���Ӷ�
 *  T = O(1)
 */
size_t sdsAllocSize(sds s) {
    struct sdshdr* sh = (sdshdr*)(s - (sizeof(struct sdshdr)));

    return sizeof(*sh) + sh->len + sh->free + 1;
}

/*
 * ���� incr ���������� sds �ĳ��ȣ���������ռ䣬
 * ���� \0 �ŵ����ַ�����β��
 *
 * ����������ڵ��� sdsMakeRoomFor() ���ַ���������չ��
 * Ȼ���û����ַ���β��д����ĳЩ����֮��
 * ������ȷ���� free �� len ���Եġ�
 *
 * ��� incr ����Ϊ��������ô���ַ��������ҽضϲ�����
 *
 *
 * ������ sdsIncrLen ��������
 *
 * oldlen = sdslen(s);
 * s = sdsMakeRoomFor(s, BUFFER_SIZE);
 * nread = read(fd, s+oldlen, BUFFER_SIZE);
 * ... check for nread <= 0 and handle it ...
 * sdsIncrLen(s, nread);
 *
 * ���Ӷ�
 *  T = O(1)
 */
void sdsIncrLen(sds s, int incr) {
    struct sdshdr* sh = (sdshdr*)(s - (sizeof(struct sdshdr)));

    // ȷ�� sds �ռ��㹻
    assert(sh->free >= incr);

    // ��������
    sh->len += incr;
    sh->free -= incr;

    // ��� assert ��ʵ���Ժ���
    // ��Ϊǰһ�� assert �Ѿ�ȷ�� sh->free - incr >= 0 ��
    assert(sh->free >= 0);

    // �����µĽ�β����
    s[sh->len] = '\0';
}

 /*
  * �� sds ������ָ�����ȣ�δʹ�õĿռ��� 0 �ֽ���䡣
  *
  * ����ֵ
  *  sds ������ɹ������� sds ��ʧ�ܷ��� NULL
  *
  * ���Ӷȣ�
  *  T = O(N)
  */
sds sdsgrowzero(sds s, size_t len) {
    struct sdshdr* sh = (sdshdr*)(s - (sizeof(struct sdshdr)));
    size_t totlen, curlen = sh->len;

    // ��� len ���ַ��������г���С��
    // ��ôֱ�ӷ��أ���������
    if (len <= curlen) return s;

    // ��չ sds
    // T = O(N)
    s = sdsMakeRoomFor(s, len - curlen);
    // ����ڴ治�㣬ֱ�ӷ���
    if (s == NULL) return NULL;

    /* Make sure added region doesn't contain garbage */
    // ���·���Ŀռ��� 0 ��䣬��ֹ������������
    // T = O(N)
    sh = (sdshdr*)(s - (sizeof(struct sdshdr)));
    memset(s + curlen, 0, (len - curlen + 1)); /* also set trailing \0 byte */

    // ��������
    totlen = sh->len + sh->free;
    sh->len = len;
    sh->free = totlen - sh->len;

    // �����µ� sds
    return s;
}

/*
 * ������Ϊ len ���ַ��� t ׷�ӵ� sds ���ַ���ĩβ
 *
 * ����ֵ
 *  sds ��׷�ӳɹ������� sds ��ʧ�ܷ��� NULL
 *
 * ���Ӷ�
 *  T = O(N)
 */
sds sdscatlen(sds s, const void* t, size_t len) {

    struct sdshdr* sh;

    // ԭ���ַ�������
    size_t curlen = sdslen(s);

    // ��չ sds �ռ�
    // T = O(N)
    s = sdsMakeRoomFor(s, len);

    // �ڴ治�㣿ֱ�ӷ���
    if (s == NULL) return NULL;

    // ���� t �е����ݵ��ַ�����
    // T = O(N)
    sh = (sdshdr*)(s - (sizeof(struct sdshdr)));
    memcpy(s + curlen, t, len);

    // ��������
    sh->len = curlen + len;
    sh->free = sh->free - len;

    // ����½�β����
    s[curlen + len] = '\0';

    // ������ sds
    return s;
}

/*
 * �������ַ��� t ׷�ӵ� sds ��ĩβ
 *
 * ����ֵ
 *  sds ��׷�ӳɹ������� sds ��ʧ�ܷ��� NULL
 *
 * ���Ӷ�
 *  T = O(N)
 */
sds sdscat(sds s, const char* t) {
    return sdscatlen(s, t, strlen(t));
}

/*
 * ����һ�� sds ׷�ӵ�һ�� sds ��ĩβ
 *
 * ����ֵ
 *  sds ��׷�ӳɹ������� sds ��ʧ�ܷ��� NULL
 *
 * ���Ӷ�
 *  T = O(N)
 */
sds sdscatsds(sds s, const sds t) {
    return sdscatlen(s, t, sdslen(t));
}

/*
 * ���ַ��� t ��ǰ len ���ַ����Ƶ� sds s ���У�
 * �����ַ������������ս����
 *
 * ��� sds �ĳ������� len ���ַ�����ô��չ sds
 *
 * ���Ӷ�
 *  T = O(N)
 *
 * ����ֵ
 *  sds �����Ƴɹ������µ� sds �����򷵻� NULL
 */
sds sdscpylen(sds s, const char* t, size_t len) {

    struct sdshdr* sh = (sdshdr*)(s - (sizeof(struct sdshdr)));

    // sds ���� buf �ĳ���
    size_t totlen = sh->free + sh->len;

    // ��� s �� buf ���Ȳ����� len ����ô��չ��
    if (totlen < len) {
        // T = O(N)
        s = sdsMakeRoomFor(s, len - sh->len);
        if (s == NULL) return NULL;
        sh = (sdshdr*)(s - (sizeof(struct sdshdr)));
        totlen = sh->free + sh->len;
    }

    // ��������
    // T = O(N)
    memcpy(s, t, len);

    // ����ս����
    s[len] = '\0';

    // ��������
    sh->len = len;
    sh->free = totlen - len;

    // �����µ� sds
    return s;
}

/*
 * ���ַ������Ƶ� sds ���У�
 * ����ԭ�е��ַ���
 *
 * ��� sds �ĳ��������ַ����ĳ��ȣ���ô��չ sds ��
 *
 * ���Ӷ�
 *  T = O(N)
 *
 * ����ֵ
 *  sds �����Ƴɹ������µ� sds �����򷵻� NULL
 */
sds sdscpy(sds s, const char* t) {
    return sdscpylen(s, t, strlen(t));
}


/*
 * �������Խ�ȡ sds �ַ���������һ��
 * start �� end ���Ǳ����䣨�������ڣ�
 *
 * ������ 0 ��ʼ�����Ϊ sdslen(s) - 1
 * ���������Ǹ����� sdslen(s) - 1 == -1
 *
 * ���Ӷ�
 *  T = O(N)
 */
void sdsrange(sds s, int start, int end) {
    struct sdshdr* sh = (sdshdr*)(s - (sizeof(struct sdshdr)));
    size_t newlen, len = sdslen(s);

    if (len == 0) return;
    if (start < 0) {
        start = len + start;
        if (start < 0) start = 0;
    }
    if (end < 0) {
        end = len + end;
        if (end < 0) end = 0;
    }
    newlen = (start > end) ? 0 : (end - start) + 1;
    if (newlen != 0) {
        if (start >= (signed)len) {
            newlen = 0;
        }
        else if (end >= (signed)len) {
            end = len - 1;
            newlen = (start > end) ? 0 : (end - start) + 1;
        }
    }
    else {
        start = 0;
    }

    // �������Ҫ�����ַ��������ƶ�
    // T = O(N)
    if (start && newlen) memmove(sh->buf, sh->buf + start, newlen);

    // ����ս��
    sh->buf[newlen] = 0;

    // ��������
    sh->free = sh->free + (sh->len - newlen);
    sh->len = newlen;
}

/*
 * �� sds �ַ����е������ַ�ת��ΪСд
 *
 * T = O(N)
 */
void sdstolower(sds s) {
    int len = sdslen(s), j;

    for (j = 0; j < len; j++) s[j] = tolower(s[j]);
}

/*
 * �� sds �ַ����е������ַ�ת��Ϊ��д
 *
 * T = O(N)
 */
void sdstoupper(sds s) {
    int len = sdslen(s), j;

    for (j = 0; j < len; j++) s[j] = toupper(s[j]);
}

/*
 * �Ա����� sds
 *
 * ����ֵ
 *  int ����ȷ��� 0 ��s1 �ϴ󷵻������� s2 �ϴ󷵻ظ���
 *
 * T = O(N)
 */
int sdscmp(const sds s1, const sds s2) {
    size_t l1, l2, minlen;
    int cmp;

    l1 = sdslen(s1);
    l2 = sdslen(s2);
    minlen = (l1 < l2) ? l1 : l2;
    cmp = memcmp(s1, s2, minlen);

    if (cmp == 0) return l1 - l2;

    return cmp;
}
int sdscmpstr(const sds s1, const char* s2, size_t l2) {
    size_t l1, minlen;
    int cmp;

	l1 = sdslen(s1);
	minlen = (l1 < l2) ? l1 : l2;
	cmp = memcmp(s1, s2, minlen);

	if (cmp == 0) return l1 - l2;

	return cmp;
}
#include "lrucache.h"
#include <cstdlib>

struct Cache
{
	int key;
	Value value;
	int count;
	Cache* next;
};

LRUCache::LRUCache(int capacity)
{
	m_szCache = new Cache*[capacity / 10 + 1]();
	if (!m_szCache) {
		abort();
	}
	m_cap = capacity;
	m_count = 0;
	m_len = 0;
	m_size = capacity / 10 + 1;
}

LRUCache::~LRUCache()
{
	if (m_szCache) {
		delete[] m_szCache;
	}
}

/*
 * ��ȡ����
 * 
 * �����Կ (key) �����ڻ����У����ȡ��Կ��ֵ�����������������򷵻� -1��
 */
Value LRUCache::Get(int key)
{
	Cache* cache = m_szCache[key % m_size];
	Cache* tmp = NULL;
	Value ret = -1;

	while (cache) {
		if (cache->key == key) {
			m_count++;
			cache->count = m_count;
			ret = cache->value;
			if (cache->next) {
				Cache* t = cache;
				if (!tmp) {
					m_szCache[key%m_size] = cache->next;
				}
				else {
					tmp->next = cache->next;
				}
				while (cache->next) {
					cache = cache->next;
				}
				cache->next = t;
				t->next = NULL;
			}
			break;
		}
		tmp = cache;
		cache = cache->next;
	}

	return ret;
}

/*
 * д������
 * 
 * �����Կ(key)�����ڣ���д��������ֵ�������������ﵽ����ʱ����Ӧ����д��������
 * ֮ǰɾ���������ʹ�õ�����ֵ���Ӷ�Ϊ�µ�����ֵ�����ռ䡣
 */
void LRUCache::Put(int key, Value value)
{
	Cache* cache = m_szCache[key % m_size];
	Cache* tmp = NULL;

	while (cache) {
		// key �Ѵ���
		if (cache->key == key) {
			m_count++;
			cache->count = m_count;
			cache->value = value;
			// �ŵ����
			if (cache->next) {
				Cache* t = cache;
				if (!tmp) {
					m_szCache[key % m_size] = cache->next;
				}
				else {
					tmp->next = cache->next;
				}
				while (cache->next) {
					cache = cache->next;
				}
				cache->next = t;
				t->next = NULL;
			}
			return;
		}
		tmp = cache;
		cache = cache->next;
	}

	// ������key��������cache
	while (tmp && tmp->next) {
		tmp = tmp->next;
	}

	// ��������
	if (m_len == m_cap) {
		int min = m_count + 1;
		int pin = 0;
		// �ҵ����ٷ��ʵĶ���
		for (int i = 0; i < m_size; i++) {
			if (m_szCache[i] && m_szCache[i]->count < min) {
				min = m_szCache[i]->count;
				pin = i;
			}
		}
		// tmp�Ǻ�keyһ�����е����һ��������������������˵������ֻ��tmp
		if (tmp && m_szCache[pin]->key == tmp->key) {
			tmp = NULL;
		}
		// ɾ������ʹ�õ�key
		m_szCache[pin] = m_szCache[pin]->next;
	}
	else {
		m_len++;
	}

	m_count++;
	Cache* t = new Cache;
	t->key = key;
	t->value = value;
	t->count = m_count;
	t->next = NULL;

	// ��Ϊ��Ӧ���еĵ�һ��
	if (!tmp) {
		m_szCache [key % m_size] = t;
	}
	else {
		tmp->next = t;
	}
}

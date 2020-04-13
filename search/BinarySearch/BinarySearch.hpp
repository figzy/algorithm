#pragma once

#define DEFAULTSIZE 36

template<typename Key, typename Value>
struct ElemType {
	Key key;
	Value val;
};

template<typename Key, typename Value>
class SSTable {
protected:
	typedef Key __elem_key;
	typedef Value __elem_value;
	typedef ElemType<__elem_key, __elem_value> __elem_type;

private:
	__elem_type* elem;
	size_t m_len;

public:
	SSTable(size_t initlen = DEFAULTSIZE) {
		elem = new __elem_type[initlen]();
		if (!elem) throw "new failed.";
		m_len = initlen;
	}

	~SSTable() {
		if (elem) delete elem;
	}

	size_t Len() const { return m_len; };

	/*
	 * �����±������
	 *
	 * ����
	 *  _n ��Ԫ�ص��±꣬�±귶ΧΪ 0~m_len-1
	 */
	__elem_type& operator[] (size_t _n) {
		if (_n >= m_len) {
			throw "out of range.";
		}
		return elem[_n];
	}

	/*
	 * ˳���Ĳ���
	 *
	 * ����
	 *  _key �����ҵ�keyֵ
	 *  equal ���Ƚ�ֵ�ĺ���ָ��
	 *        1����� v = w,����ֵ = 0
	 *		  2����� v > w,����ֵ > 0
	 *		  3����� v < w,����ֵ < 0
	 *
	 * ʱ�临�Ӷ� 
	 *  T = O(log2n)
	 */
	int search_binary(__elem_key _key, int (*compare)(__elem_key v, __elem_key w)) {
		size_t low = 0, high = m_len - 1, mid;
		int ret;

		while (low <= high) {
			mid = (low + high) / 2;
			ret = compare(elem[mid].key, _key);
			if (ret == 0) {
				return mid;
			}
			else if (ret > 0) {
				high = mid - 1;
			}
			else {
				low = mid + 1;
			}
		}

		return -1;
	}
};
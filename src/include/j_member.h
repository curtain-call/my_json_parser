#pragma once
#ifndef J_MEMBER_
#define J_MEMBER_
#include"../j_context.h"

class J_VALUE;
class J_MEMBER {
public:
	char* key;
	size_t k_len;
	J_VALUE *v;		// ��ȷ����С�ģ��淶����Ҫ������Ĳ��ֱ����Ƕ����ģ�ָ�������Ҳ�����
};

#endif // !J_MEMBER_

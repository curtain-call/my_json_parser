#pragma once
#ifndef J_MEMBER_
#define J_MEMBER_
#include"../j_context.h"

class J_VALUE;
class J_MEMBER {
public:
	char* key;
	size_t k_len;
	J_VALUE *v;		// 是确定大小的，规范：需要被处理的部分必须是定长的，指针的意义也是如此
};

#endif // !J_MEMBER_

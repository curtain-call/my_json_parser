#include"j_context.h"
#include<cassert>
#include<cstdlib>
void * j_context_push(j_context * pack, size_t size)
{
	void* ret;
	assert(size > 0);
	if (pack->top + size >= pack->size) {
		if (pack->size == 0)
			pack->size = 256;
		// 第一次识别字符串时再初始化栈，减少内存空载
		while (pack->top + size >= pack->size)
			pack->size += pack->size >> 1;
		// 右移一位相当于除二
		pack->stack = (char*)realloc(pack->stack, pack->size);
		// 相当于手动实现了vector的扩容机制
	}
	ret = pack->stack + pack->top;
	// c->stack是栈底的地址；c->top是到栈顶的距离（数目）
	// 如果声明一个空的vector，向里填充元素，是改变size而不是capacity
	pack->top += size;
	// 移动栈顶指针
	// 手动维护栈，只能一次接受一个对象的压栈，多压需要靠外部的循环实现
	return ret;
}

void* j_context_pop(j_context* pack,size_t size) {
	assert(pack->top >= size);
	// 保证弹出时栈顶指针不会越界
	return pack->stack + (pack->top -= size);
}
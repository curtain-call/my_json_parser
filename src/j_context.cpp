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
		// ��һ��ʶ���ַ���ʱ�ٳ�ʼ��ջ�������ڴ����
		while (pack->top + size >= pack->size)
			pack->size += pack->size >> 1;
		// ����һλ�൱�ڳ���
		pack->stack = (char*)realloc(pack->stack, pack->size);
		// �൱���ֶ�ʵ����vector�����ݻ���
	}
	ret = pack->stack + pack->top;
	// c->stack��ջ�׵ĵ�ַ��c->top�ǵ�ջ���ľ��루��Ŀ��
	// �������һ���յ�vector���������Ԫ�أ��Ǹı�size������capacity
	pack->top += size;
	// �ƶ�ջ��ָ��
	// �ֶ�ά��ջ��ֻ��һ�ν���һ�������ѹջ����ѹ��Ҫ���ⲿ��ѭ��ʵ��
	return ret;
}

void* j_context_pop(j_context* pack,size_t size) {
	assert(pack->top >= size);
	// ��֤����ʱջ��ָ�벻��Խ��
	return pack->stack + (pack->top -= size);
}
// CMakeProject1.cpp: 定义应用程序的入口点。
//

#include "../src/include/praser_lite.h"
#include"j_context.h"
#include<cassert>
#include<cmath>
#include<cerrno>

using namespace std;

J_VALUE::J_VALUE(){
	type = j_null;
}

J_VALUE::J_VALUE(const J_VALUE& a) {

}
 
j_type J_VALUE::j_get_type()
{
	assert(this != nullptr);
	return this->type;
}
void J_VALUE::j_set_null(){
	node_free(this);
}

const char * J_VALUE::getString()
{
	assert(this != nullptr&&this->type == j_string);
	return this->s;
}
size_t J_VALUE::j_get_string_length()
{
	assert(this != nullptr&&this->type == j_string);
	return this->len;
}
void J_VALUE::j_set_string(const char * s, size_t len)
{
	node_free(this);
	this->type = j_string;
	this->len = len;
	this->s = new char[len + 1];
	memcpy(this->s, s, len);
	this->s[len] = '\0';
}

size_t J_VALUE::j_get_array_size()
{
	assert(this != nullptr&&this->type == j_array);
	return this->capacity;
}

J_VALUE * J_VALUE::j_get_arr_element(size_t index) const
{
	assert(this != nullptr&&this->type == j_array);
	assert(index < this->capacity);
	return &this->e[index];
}

int J_VALUE::j_get_boolean(){
	// 完全测试用的，不影响使用者
	assert(this != nullptr && (this->type == j_true || this->type == j_false));
	return this->type == j_true;
}
void J_VALUE::j_set_boolean(int b){
	node_free(this);
	this->type = b ? j_true : j_false;
}

double J_VALUE::j_get_number()
{
	assert(this != nullptr&&this->type == j_number);
	return this->num;
}
void J_VALUE::j_set_number(double n)
{
	node_free(this);
	this->type = j_number;
	this->num = n;
}

get_condition j_parse(J_VALUE *v, char* json)
{
	assert(v!= nullptr);
	j_context pack;
	pack.stack = nullptr;
	pack.json = json;
	pack.top = 0;
	pack.size = 0;

	v->j_init();

	j_parse_ws(&pack);
	get_condition ret = j_parse_v(v, &pack);
	if (ret == J_PARSE_OK) {
		j_parse_ws(&pack);
		if (pack.json[0] != '\0')
			ret = J_ROOT_NOT_SINGULAR;
		// 检测输入字符串尾,json文本在字符串中才可能出现空字符，其他情况一律检出则错误
	}
	assert(pack.top == 0);
	delete pack.stack;
	return ret;
}

void j_parse_ws(j_context * pack)
{
	while(pack->json[0] == '\n' || pack->json[0] == '\t' || pack->json[0] == ' ' || pack->json[0] == '\r')
		pack->json++;
	//return get_condition();
}

get_condition j_parse_v(J_VALUE *v, j_context * pack)
{
	get_condition c;
	switch (pack->json[0])
	{
	case'n':
		return j_parse_null(v,pack);
	case't':
		return j_parse_true(v,pack);
	case'f':
		return j_parse_false(v,pack);
	case'\0':
		return J_EXPECTED_VALUE;
	case'"':
		return j_parse_string(v, pack);
	case'[':
		return j_parse_array(v, pack);
	default:
		return j_parse_number(v, pack);
	}
	//需要exception吗？？？
}


void node_free(J_VALUE* v)
{
	assert(v != nullptr);
	switch (v->type)
	{
	case j_string:
		delete v->s;
		break;
	case j_array:
		for (size_t i = 0; i < v->capacity; i++)
			node_free(&v->e[i]);
		delete[] v->e;
		// 递归释放
	default:
		break;
	}
	v->type = j_null;
}

get_condition j_parse_null(J_VALUE *v, j_context * pack)
{
	// 可能存在安全问题
	if (pack->json[1] != 'u' || pack->json[2] != 'l' || pack->json[3] != 'l')
		return J_INVALID_VALUE;
	pack->json += 4;
	v->type = j_null;
	return J_PARSE_OK;
}

get_condition j_parse_true(J_VALUE *v, j_context* pack)
{
	if (pack->json[1] != 'r' || pack->json[2] != 'u' || pack->json[3] != 'e')
		return J_INVALID_VALUE;
	pack->json += 4;
	v->type = j_true;
	return J_PARSE_OK;
}

get_condition j_parse_false(J_VALUE *v, j_context * pack)
{
	if (pack->json[1] != 'a' || pack->json[2] != 'l' || pack->json[3] != 's' || pack->json[4] != 'e')
		return J_INVALID_VALUE;
	pack->json += 5;
	v->type = j_false;
	return J_PARSE_OK;
}

get_condition j_parse_number(J_VALUE *v, j_context* pack)
{
	//不要改变传入的字符串指针指向，因为后面还要用到他字符转数字
	const char *p = pack->json;
	// 解析
	// 存在符号
	if (p[0] == '-')p++;
	// 整数部分
	if (p[0] == '0')p++;
	else if (!J_VALUE::ISDIGIT1TO9(p))
		return J_INVALID_VALUE;
	else for (p++; J_VALUE::ISDIGIT(p); p++);		//这里for内部已经实现递增改变p指向了！不要再p++！
	// 小数部分
	if (p[0] == '.'){
		p++;
		if (!J_VALUE::ISDIGIT(p)) return J_INVALID_VALUE;
		for (p++;J_VALUE::ISDIGIT(p); p++);
	}
	//指数部分
	if (p[0] == 'e' || p[0] == 'E') {
		p++;
		while (p[0] == '+' || p[0] == '-'||p[0] == '0')
			p++;
		if (!J_VALUE::ISDIGIT(p))
			return J_INVALID_VALUE;
		for (p++; J_VALUE::ISDIGIT(p); p++);
	}

	//存值
	v->num = strtod(pack->json, NULL);
	if (errno == ERANGE && (v->num == HUGE_VAL || v->num == -HUGE_VAL))
		return J_TOO_BIG;
	v->type = j_number;
	pack->json = p;
	return J_PARSE_OK;
}
// 简化代码，定义一个inline
inline void put(j_context* pack, char ch) {
	*(char*)j_context_push(pack, sizeof(char)) = ch;
}
get_condition j_parse_string(J_VALUE *v, j_context * pack)
{
	// 解析值
	size_t head = pack->top, len;
	const char *p = pack->json;
	if (p[0] == '"') {
		p++;
		for (;;) {
			char ch = *p++;
			switch (ch)
			{
			// 1.结束标志
			case'"':
				len = pack->top - head;  // head中存者top的原始值
				v->j_set_string((const char*)j_context_pop(pack, len), len);
				// 将pop出的字符串放进节点v
				pack->json = p;
				// 将已读取的从context中去掉
				return J_PARSE_OK;
			// 2.出现转义符  转义符合并进第一个字符
			case'\\':
				switch (*p++) {
				case'"': put(pack, '\"'); break;
				case'\\':put(pack, '\\'); break;
				case'/': put(pack, '/' ); break;
				case'b': put(pack, '\b'); break;
				case'f': put(pack, '\f'); break;
				case'n': put(pack, '\n'); break;
				case'r': put(pack, '\r'); break;
				case't': put(pack, '\t'); break;
				default: pack->top = head;			// 将待处理的context也重置，以保留原状态
					return J_INVALID_VALUE;
					// 非法转义值处理
				}
				break;
			// 3.丢失后引号
			case'\0':pack->top = head;
				return J_LOSE_MARK;
			// 4.一般字符
			default:
				if (ch < 0x20) {
					pack->top = head;
					return J_INVALID_ASCII;
				}
				put(pack, ch);
			}
	}
	}

	// 存储字符串

	return J_PARSE_OK;
	// 在字符串看来，栈缓冲只能起到预防非法输入打乱数据结构的作用
}

get_condition j_parse_array(J_VALUE *v, j_context * pack)
{
	v->j_init();
	get_condition ret;
	size_t elmtExt = 0;
	// 由上一个中间节点（或者根节点）指向而来，首先先初始化
	if (pack->json[0] == '[') {
		pack->json++;
		j_parse_ws(pack);
		if (pack->json[0] == ']') {
			//空数组不需要分配内存，因为它没有叶子节点
			pack->json++;
			v->e = nullptr;
			v->capacity = 0;
			v->type = j_array;
			return J_PARSE_OK;
		}

		for (;;) {
			J_VALUE e;
			e.j_init();
			ret = j_parse_v(&e, pack);
			// 递归，深度优先
			if (ret != J_PARSE_OK)
				break;
			memcpy(j_context_push(pack, sizeof(J_VALUE)), &e, sizeof(J_VALUE));
			//memcpy会重新申请内存空间，所以压对象栈的时候直接使用临时节点
			elmtExt++;
			j_parse_ws(pack);
			if (pack->json[0] == ',') {
				pack->json++;
				j_parse_ws(pack);
			}
			else if (pack->json[0] == ']')
			{
				pack->json++;
				size_t size = elmtExt * sizeof(J_VALUE);
				v->type = j_array;
				v->capacity = elmtExt;
				v->e = new J_VALUE[elmtExt];
				// 释放在node_free当中，注意delete[]
				memcpy(v->e, j_context_pop(pack, size), size);
				return J_PARSE_OK;
			}
			else {
				ret = J_LOSE_MARK;
				break;
			}
		}
		// 错误退出也要清空栈，不影响以后对同一数据结构的操作
		for (size_t i = 0; i < elmtExt; i++) {
			node_free((J_VALUE*)j_context_pop(pack, sizeof(J_VALUE)));
			// 转型，	
		}
			return ret;
	}
	return get_condition();
}


//UTF部分采用模板去支持ascii和unicode


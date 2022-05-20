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
	pack.json = json;
	v->type = j_null;
	j_parse_ws(&pack);
	get_condition ret = j_parse_v(v, &pack);
	if (ret == J_PARSE_OK) {
		j_parse_ws(&pack);
		if (pack.json[0] != '\0')
			ret = J_ROOT_NOT_SINGULAR;
		// 其实是只检测输入字符串尾
	}
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
	default:
		return j_parse_number(v, pack);
	}
	//需要exception吗？？？
}


void node_free(J_VALUE* v)
{
	assert(v != nullptr);
	if (v->type = j_string)
		delete v;
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

get_condition j_parse_string(J_VALUE *, j_context * pack)
{
	// 解析值

	// 存储字符串

	return J_PARSE_OK;
}


//UTF部分采用模板去支持ascii和unicode
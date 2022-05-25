// CMakeProject1.h: 标准系统包含文件的包含文件
// 或项目特定的包含文件。

#pragma once
#ifndef PRASER_LITE_H_
#define PRASER_LITE_H_

 
#include <iostream>
// TODO: 在此处引用程序需要的其他标头。
#include"../j_context.h"
class J_MEMBER;
#include"j_member.h"

typedef enum { j_null, j_true, j_false, j_number, j_string, j_array, j_object } j_type;
typedef enum { J_PARSE_OK, J_INVALID_VALUE, J_EXPECTED_VALUE,J_ROOT_NOT_SINGULAR,J_TOO_BIG,J_LOSE_MARK,J_INVALID_ASCII,J_MISS_KEY }get_condition;


class J_VALUE {
private:
	// data remain
	union 
	{
		struct { J_MEMBER* m; size_t capacityOBJ;};	// 对象
		struct { J_VALUE* e; size_t capacityARR; };	// 数组
		// 直到数组才可能出现嵌套和递归，解析数据结构也需要重新设置中间节点
		struct { char* s; size_t len; };			// 字符串
		double num;
	};
	j_type type;
	// inline接口
	inline static bool ISDIGIT1TO9(const char *p) { return p[0] >= '1'&&p[0] <= '9'; };
	inline static bool ISDIGIT(const char* p) { return p[0] >= '0'&&p[0] <= '9'; };

public:
	// access methods
	// default constructor
	J_VALUE();
	// copy constructor
	J_VALUE(const J_VALUE&);						//写到access再回来看

	inline void j_init() { this->type = j_null; };
	j_type j_get_type();
	void j_set_null();
	int j_get_boolean();
	void j_set_boolean(int b);
	double j_get_number();
	void j_set_number(double n);
	const char* getString();	//注意返回的是一个char数组而非一般意义的字符串
	size_t j_get_string_length();
	void j_set_string(const char* s, size_t len);
	size_t j_get_array_size();
	J_VALUE* j_get_arr_element(size_t index)const;
	size_t j_get_object_size()const;
	const char* j_get_objeci_key(size_t index)const;
	size_t j_get_object_key_length(size_t index)const;
	J_VALUE* j_get_object_value()const;


	friend void node_free(J_VALUE* v);
	friend get_condition j_parse(J_VALUE*, char* json);
//	friend get_condition j_parse_v(J_VALUE*, char*);
	friend get_condition j_parse_null(J_VALUE*, j_context * pack);
	friend get_condition j_parse_true(J_VALUE*, j_context* pack);
	friend get_condition j_parse_false(J_VALUE*, j_context * pack);
	friend get_condition j_parse_number(J_VALUE*, j_context* pack);
	friend get_condition j_parse_string(J_VALUE*, j_context* pack);
	friend get_condition j_parse_string_mem(j_context * pack, char**, size_t *len);
	friend get_condition j_parse_array(J_VALUE*, j_context* pack);
	friend get_condition j_parse_object(J_VALUE*, j_context*);
};


void j_parse_ws(j_context * pack);
get_condition j_parse_v(J_VALUE * v, j_context * pack);



#endif 
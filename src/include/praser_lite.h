// CMakeProject1.h: 标准系统包含文件的包含文件
// 或项目特定的包含文件。

#pragma once
#ifndef PRASER_LITE_H_
#define PRASER_LITE_H_

 
#include <iostream>
// TODO: 在此处引用程序需要的其他标头。
#include <fstream>
#include"file_operation.h"

typedef enum { j_null, j_true, j_false, j_number, j_string, j_array, j_object } j_type;
typedef enum { J_PRASE_OK, J_INVALID_VALUE, J_EXPECTED_VALUE }get_condition;

class J_VALUE {
private:
	// data remain
	j_type type;

public:
	// access methods
	// default constructor
	J_VALUE();
	// copy constructor
	J_VALUE(const J_VALUE&);						//写到access再回来看

	j_type getType(const J_VALUE*);
	friend get_condition j_parse(J_VALUE*,char*);
//	friend get_condition j_parse_v(J_VALUE*, char*);
	friend get_condition j_parse_null(J_VALUE*, char* json);
	friend get_condition j_parse_true(J_VALUE*, char* json);
	friend get_condition j_parse_false(J_VALUE*, char* json);
};

get_condition j_parse(J_VALUE*, char*);
get_condition j_parse_ws(char*);
get_condition j_parse_v(J_VALUE*, char*);
get_condition j_parse_null(J_VALUE*, char* json);
get_condition j_parse_true(J_VALUE*, char* json);
get_condition j_parse_false(J_VALUE*, char* json);

get_condition j_parse_number(J_VALUE*, char*json);

#endif 
// CMakeProject1.cpp: 定义应用程序的入口点。
//

#include "../src/include/praser_lite.h"
#include<assert.h>

using namespace std;

J_VALUE::J_VALUE(){
	type = j_null;
}

J_VALUE::J_VALUE(const J_VALUE& a) {

}
 
j_type J_VALUE::getType(const J_VALUE *a)
{
	return a->type;
}

get_condition j_parse(J_VALUE *v, char *json)
{
	assert(v!= nullptr);
	v->type = j_null;
	j_parse_ws(json);
	get_condition ret = j_parse_v(v, json);
	return ret;
}

get_condition j_parse_ws(char *json)
{
	if (json[0] == '\n' || json[0] == '\t' || json[0] == ' ' || json[0] == '\r')
		json++;
	return get_condition();
}

get_condition j_parse_v(J_VALUE *v,char*json)
{
	switch (json[0])
	{
	case'n':
		return j_parse_null(v,json);
	case't':
		return j_parse_true(v,json);
	case'f':
		return j_parse_false(v,json);
	case'\0':
		return J_EXPECTED_VALUE;
	default:
		break;
	}
	return J_INVALID_VALUE;
	//需要exception吗？？？
}

get_condition j_parse_null(J_VALUE *v, char* json)
{
	// 可能存在安全问题
	if (json[1] != 'u' || json[2] != 'l' || json[3] != 'l')
		return J_INVALID_VALUE;
	json += 4;
	v->type = j_null;
	return J_PRASE_OK;
}

get_condition j_parse_true(J_VALUE *v, char* json)
{
	if (json[1] != 'r' || json[2] != 'u' || json[3] != 'e')
		return J_INVALID_VALUE;
	json += 4;
	v->type = j_true;
	return J_PRASE_OK;
}

get_condition j_parse_false(J_VALUE *v, char* json)
{
	if (json[1] != 'a' || json[2] != 'l' || json[3] != 's' || json[4] != 'e')
		return J_INVALID_VALUE;
	v->type = j_false;
	return J_PRASE_OK;
}

get_condition j_parse_number(J_VALUE *, char * json)
{

	return J_PRASE_OK;
}



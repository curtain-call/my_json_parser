#include"gtest/gtest.h"
#include"../src/include/praser_lite.h"
#include"../src/j_context.h"


# define TEST_NUMBER(expect, json)\
	do {\
        J_VALUE v;\
        EXPECT_EQ(J_PARSE_OK, j_parse(&v, json));\
        EXPECT_EQ(j_number, v.J_VALUE::j_get_type());\
        EXPECT_DOUBLE_EQ(expect, v.J_VALUE::j_get_number());\
    } while(0)
#define TEST_STRINGP(expect,json)\
	do {\
		J_VALUE v;\
		EXPECT_EQ(J_PARSE_OK,j_parse(&v,json));\
		EXPECT_EQ(j_string, v.J_VALUE::j_get_type());\
		EXPECT_STREQ(expect, v.J_VALUE::getString());\
	} while(0)


TEST(parser, nulltest) {
	J_VALUE *v = new J_VALUE();
	EXPECT_EQ(J_PARSE_OK, j_parse(v,"null"));
	// 测试正常的null输入
	EXPECT_EQ(J_INVALID_VALUE, j_parse(v, "np"));
	//测试一个非正常输入
	EXPECT_EQ(J_EXPECTED_VALUE, j_parse(v, ""));
	//测试一个空输入
	EXPECT_EQ(J_ROOT_NOT_SINGULAR, j_parse(v, "null x"));
	delete v;
}


TEST(parser, numbertest) {
	TEST_NUMBER(0.0, "0");
	//json特殊语法“负零”
	TEST_NUMBER(0.0, "-0");
	TEST_NUMBER(0.0, "-0.0");
	TEST_NUMBER(1.0, "1");
	TEST_NUMBER(-1.0, "  -1");
	TEST_NUMBER(1.5, "1.5");
	TEST_NUMBER(-1.5, "-1.5");
	TEST_NUMBER(3.1416, "3.1416");
	//科学计数法
	TEST_NUMBER(1E10, "1E10");
	TEST_NUMBER(1e10, "1e10");
	TEST_NUMBER(1E+10, "1E+10");
	TEST_NUMBER(1E-10, "1E-10");
	TEST_NUMBER(-1E10, "-1E10");
	TEST_NUMBER(-1e10, "-1e10");
	TEST_NUMBER(-1E+10, "-1E+10");
	TEST_NUMBER(-1E-10, "-1E-10");
	TEST_NUMBER(1.234E+10, "1.234E+10");
	TEST_NUMBER(1.234E-10, "1.234E-10");
	TEST_NUMBER(0.0, "1e-10000");
	//支持指数位前导0
	TEST_NUMBER(1e10, "1e010");
}




TEST(parser, stringtest) {
	 //dumbtest
	TEST_STRINGP("pleasepass","pleasepass");
}
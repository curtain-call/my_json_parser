if(EXISTS "E:/Cpp_TEST/my_json_parser/build/hello_test[1]_tests.cmake")
  include("E:/Cpp_TEST/my_json_parser/build/hello_test[1]_tests.cmake")
else()
  add_test(hello_test_NOT_BUILT hello_test_NOT_BUILT)
endif()

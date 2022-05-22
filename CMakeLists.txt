cmake_minimum_required(VERSION 3.14)
project(my_json_parser)

# GoogleTest requires at least C++11
set(CMAKE_CXX_STANDARD 11)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)


enable_testing()

add_executable(parser_test "src/praser_lite.cpp" "src/include/praser_lite.h" "test.cpp" "src/j_context.h" "src/j_context.cpp")

target_link_libraries(
  parser_test
  gtest_main
)

include(GoogleTest)
gtest_discover_tests(parser_test)
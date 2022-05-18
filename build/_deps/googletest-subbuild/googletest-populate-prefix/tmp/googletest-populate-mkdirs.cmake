# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "E:/Cpp_TEST/my_json_parser/build/_deps/googletest-src"
  "E:/Cpp_TEST/my_json_parser/build/_deps/googletest-build"
  "E:/Cpp_TEST/my_json_parser/build/_deps/googletest-subbuild/googletest-populate-prefix"
  "E:/Cpp_TEST/my_json_parser/build/_deps/googletest-subbuild/googletest-populate-prefix/tmp"
  "E:/Cpp_TEST/my_json_parser/build/_deps/googletest-subbuild/googletest-populate-prefix/src/googletest-populate-stamp"
  "E:/Cpp_TEST/my_json_parser/build/_deps/googletest-subbuild/googletest-populate-prefix/src"
  "E:/Cpp_TEST/my_json_parser/build/_deps/googletest-subbuild/googletest-populate-prefix/src/googletest-populate-stamp"
)

set(configSubDirs Debug;Release;MinSizeRel;RelWithDebInfo)
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "E:/Cpp_TEST/my_json_parser/build/_deps/googletest-subbuild/googletest-populate-prefix/src/googletest-populate-stamp/${subDir}")
endforeach()

# my_json_parser
## log
- 2021.5.18  完成null，true，false解析，通过literalTest
- 2021.5.20  完成number，string解析，通过stringTest
- 2021.5.22  完成array解析
- 2021.5.25  完成object解析


# 目录结构

- build：
  - bin：远程fetch的gtest库pdb文件
  - lib：gtest本地已编译静态库
  - Debug：test文件编译后储存位置
  - my_json_parser.sln:cmake生成的vs解决方案
- src：
  - include：
    - parser_lite.h      解析器实现头文件
    - file_operation.h   文件操作
    - j_context.h        解析文本需要的数据结构和内容
    - j_member.h         对象成员的存储结构
  - parser_lite.cpp      解析器实现
  - j_context.cpp        缓冲区栈结构实现
- CMakeLists：项目根设置，可修改配置文件

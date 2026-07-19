cmake_minimum_required(VERSION 3.21)
project(statemachinev2)

set(CMAKE_CXX_STANDARD 17)

# 可以使用中文正常显示
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/source-charset:utf-8>")

# 链接头文件
include_directories(${PROJECT_SOURCE_DIR}/include)

# 添加可执行文件
add_executable(${PROJECT_NAME} test_statemachinev2.cc)

# 链接源文件
target_sources(${PROJECT_NAME} PRIVATE
    src/statemachinev2.cc
)
cmake_minimum_required(VERSION 3.10)
project(todostatemachine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 源文件
set(SOURCES
    src/todostatemachine.cc
    test_todostatemachine.cc
)

# 头文件目录
include_directories(include)

# 生成可执行文件
add_executable(test_todostatemachine ${SOURCES})

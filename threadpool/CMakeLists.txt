cmake_minimum_required(VERSION 3.21)
project(threadpool)

set(CMAKE_CXX_STANDARD 11)

# 可以使用中文正常显示
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/source-charset:utf-8>")

# 头文件
include_directories(${PROJECT_SOURCE_DIR}/include)

# # 源文件
# target_source(threadpool PRIVATE
#     src/ringbuffer.cc
# )

add_executable(${PROJECT_NAME} main.cpp)
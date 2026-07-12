cmake_minimum_required(VERSION 3.21)
project(cpp_usage)

set(CMAKE_CXX_STANDARD 11)

# 添加子模块
add_subdirectory(threadpool)
add_subdirectory(ringbuffer)
add_subdirectory(ringbufferv2)
add_subdirectory(todostatemachine)


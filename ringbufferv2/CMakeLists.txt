cmake_minimum_required(VERSION 3.21)
project(ringbufferV2)

set(CMAKE_CXX_STANDARD 17)

# 可以使用中文正常显示
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/source-charset:utf-8>")

# 头文件
include_directories(${PROJECT_SOURCE_DIR}/include)

# 添加可执行文件
add_executable(${PROJECT_NAME} test_ringbufferv2.cc)

# 源文件
target_sources(${PROJECT_NAME} PRIVATE
    src/ringbufferv2.cc
)
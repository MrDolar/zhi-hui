cmake_minimum_required(VERSION 3.10)
project(embedded-common C)

# C 标准
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

# 编译选项
add_compile_options(-Wall -Wextra -O2 -g)
add_compile_definitions(DEBUG)

# 头文件目录
set(INCLUDE_DIRS
    include
    include/hal
    include/protocol
    include/data_structure
    include/driver
    include/system
    include/security
    include/utility
)

# 源文件
file(GLOB_RECURSE SOURCES "src/*.c")

# 静态库
add_library(embedded STATIC ${SOURCES})
target_include_directories(embedded PUBLIC ${INCLUDE_DIRS})

# 示例
add_executable(example_basic examples/basic/main.c)
target_link_libraries(example_basic embedded)

# 安装
install(TARGETS embedded ARCHIVE DESTINATION lib)
install(DIRECTORY include/ DESTINATION include)

# 测试
option(BUILD_TESTS "Build tests" OFF)
if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(test)
endif()

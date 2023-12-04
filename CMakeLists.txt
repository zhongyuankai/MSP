# 指定最低版本号
cmake_minimum_required(VERSION 3.12)

# 设置c++的版本为11
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")

# 项目名称
project(MSP)


# set(CMAKE_CXX_STANDARD 14)

# 设置头文件的搜索路径
#include_directories(./include)
# 链接外部静态库头文件搜索路径
#include_directories("/Users/didi/CLionProjects/lib/include")

include_directories(src)
include_directories(contrib/boost)

add_subdirectory(src)
add_subdirectory(contrib)

# 编译的可执行程序
add_executable(main main.cpp)

# 链接内部静态库
target_link_libraries(main PRIVATE Core)
target_include_directories(main PRIVATE src/Core)

target_link_libraries(main PRIVATE Disks)
target_include_directories(main PRIVATE src/Disks)

target_link_libraries(main PRIVATE Thread)
target_include_directories(main PRIVATE src/Thread)

#Threadtarget_link_libraries(main PRIVATE contrib)
#target_link_libraries(main PRIVATE src/contrib)

# 链接外部静态库头文件搜索路径
#target_link_libraries(hello "/Users/didi/CLionProjects/lib/static_libs/libTest.a")
# 搜索目录下所有.a的文件
#file(GLOB libs_path "/Users/didi/CLionProjects/lib/static_libs/*.a")
# 用于调试
#message("=========")
#message(${libs_path})
#message("=========")
#target_link_libraries(hello ${libs_path})
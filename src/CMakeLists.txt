message(${CMAKE_CURRENT_SOURCE_DIR})
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")

add_subdirectory(Core)
add_subdirectory(Disks)
add_subdirectory(Thread)
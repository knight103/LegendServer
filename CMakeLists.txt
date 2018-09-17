# CMake 最低版本号要求
cmake_minimum_required (VERSION 2.8)

# 项目名称
project (GameServer)

# 头文件搜索目录
include_directories(extern/libuv/include)
include_directories(extern/log.h)

add_subdirectory(extern/libuv)

# 编译项目内的文件
add_executable(GameServer 
    src/main.cpp
    src/GameServer.h
    src/GameServer.cpp
)

target_link_libraries(GameServer uv_a)

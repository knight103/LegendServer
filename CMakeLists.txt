# CMake 最低版本号要求
cmake_minimum_required (VERSION 2.8)

# 项目名称
project (GameServer)

# 头文件搜索目录
include_directories(src)
include_directories(extern/libuv/include)
include_directories(extern/plog/include)
include_directories(extern/lua)
include_directories(extern/LuaBridge/Source)

add_subdirectory(extern/libuv)
add_subdirectory(extern/lua)

set(SERVER_SRC 
    src/main.cpp
    src/logic/GameServer.h
    src/logic/GameServer.cpp
    src/logic/GameProtocol.h
    src/core/TCPServer.h
    src/core/TCPServer.cpp
    src/core/TCPClient.h
    src/core/TCPClient.cpp
    src/core/log.h
    src/core/log.cpp
    src/core/Constant.h
    src/core/ProtocolDefine.h
    src/core/cocos/CCAutoreleasePool.h
    src/core/cocos/CCAutoreleasePool.cpp
    src/core/cocos/ccMacros.h
    src/core/cocos/CCRef.h
    src/core/cocos/CCRef.cpp
#    src/core/cocos/CCLuaBridge.h
#    src/core/cocos/CCLuaBridge.cpp
    src/core/cocos/CCLuaEngine.h
    src/core/cocos/CCLuaEngine.cpp
    src/core/cocos/CCLuaStack.h
    src/core/cocos/CCLuaStack.cpp
    src/core/cocos/CCLuaValue.h
    src/core/cocos/CCLuaValue.cpp
    src/core/cocos/CCScriptSupport.h
    src/core/cocos/CCScriptSupport.cpp
)

# 编译项目内的文件
add_executable(GameServer ${SERVER_SRC})

target_link_libraries(GameServer uv_a)
target_link_libraries(GameServer lua)

# Client

project (TestClient)

set(CLIENT_SRC 
    src/test/main.cpp
    src/test/TestClient.h
    src/test/TestClient.cpp
)

add_executable(TestClient ${CLIENT_SRC})

target_link_libraries(TestClient uv_a)



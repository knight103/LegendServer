#include "logic/GameServer.h"
#include <iostream>
#include <uv.h>
#include "core/FileUtils.h"

#include "core/cocos/CCLuaEngine.h"

int sayHello(lua_State* L) {
	std::cout << "start lua\n";
	return 0;
}

#ifdef __WIN32
const char* rootPath = "";
#endif

#if defined(__APPLE__)
const char* rootPath = "/Users/harry/Desktop/MyGameServer/";
#endif


int main(int argc, char** argv) {
    
    FileUtils::getInstance()->setDefaultResourceRootPath(rootPath);
    
    
	LuaEngine* engine = LuaEngine::getInstance();
	engine->addSearchPath(rootPath);
	engine->executeScriptFile("script/main.lua");

	GameServer server("0.0.0.0", 7300);
	server.run();

    return 0;
}

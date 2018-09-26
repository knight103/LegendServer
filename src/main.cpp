#include "logic/GameServer.h"
#include <iostream>
#include <uv.h>
#include "core/PlatformConfig.h"
#include "core/FileUtils.h"
#include "core/cocos/CCLuaEngine.h"


#if (TARGET_PLATFORM == PLATFORM_MAC)
const char* rootPath = "/Users/harry/Desktop/MyGameServer/";

#elif (TARGET_PLATFORM == PLATFORM_WIN32)
const char* rootPath = "C:\\Users\\zhuhui.harry\\Desktop\\MyGameServer\\";
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

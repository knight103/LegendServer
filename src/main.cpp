#include "logic/GameServer.h"
#include <iostream>
#include <uv.h>
#include "core/PlatformConfig.h"
#include "core/FileUtils.h"
#include "core/cocos/CCLuaEngine.h"


#if (TARGET_PLATFORM == PLATFORM_MAC)
const char* rootPath = "/Users/harry/Desktop/LegendServer/";

#elif (TARGET_PLATFORM == PLATFORM_WIN32)
const char* rootPath = "C:\\Users\\zhuhui.harry\\Desktop\\MyGameServer\\";
#endif



int main(int argc, char** argv) {
    
    FileUtils::getInstance()->setDefaultResourceRootPath(rootPath);
    FileUtils::getInstance()->addSearchPath("script");
    
	LuaEngine* engine = LuaEngine::getInstance();
	engine->executeScriptFile("main.lua");

	GameServer server("0.0.0.0", 7300);
	server.run();

    return 0;
}

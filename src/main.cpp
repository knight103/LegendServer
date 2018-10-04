#include "logic/GameServer.h"
#include <iostream>
#include <uv.h>
#include "core/PlatformConfig.h"
#include "core/FileUtils.h"
#include "core/cocos/CCLuaEngine.h"
#include "core/cocos/CCLuaStack.h"

extern "C" {
#include <lua.h>
#include <lauxlib.h>
}

#include <LuaBridge/LuaBridge.h>


#if (TARGET_PLATFORM == PLATFORM_MAC)
const char* rootPath = "/Users/harry/Desktop/LegendServer/";

#elif (TARGET_PLATFORM == PLATFORM_WIN32)
const char* rootPath = "C:\\Users\\zhuhui.harry\\Desktop\\LegendServer\\";
#endif



int main(int argc, char** argv) {
    FileUtils::getInstance()->setDefaultResourceRootPath(rootPath);
    FileUtils::getInstance()->addSearchPath("script");
    
	LuaEngine* engine = LuaEngine::getInstance();
	lua_State* L = engine->getLuaStack()->getLuaState();
	luabridge::getGlobalNamespace(L)
		.beginClass<GameServer>("GameServer")
		.addConstructor<void(*)(const char*, int)>()
		.endClass();

	engine->executeScriptFile("main.lua");

    return 0;
}

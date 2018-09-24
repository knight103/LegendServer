#include "logic/GameServer.h"
#include <iostream>
#include <uv.h>

#include "core/cocos/CCLuaEngine.h"

int sayHello(lua_State* L) {
	std::cout << "start lua\n";
	return 0;
}

int main(int argc, char** argv) {

	LuaEngine* engine = LuaEngine::getInstance();
	engine->addSearchPath("C:\\Users\\zhuhui.harry\\Desktop\\MyGameServer\\script");
	engine->executeScriptFile("main.lua");

	GameServer server("0.0.0.0", 7300);
	server.run();

    return 0;
}

#include "logic/GameServer.h"
#include <iostream>
#include <uv.h>

extern "C" {
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
}

#include <LuaBridge/LuaBridge.h>

int sayHello(lua_State* L) {
	std::cout << "start lua\n";
	return 0;
}

int main(int argc, char** argv) {

	lua_State *L = luaL_newstate();
	luaL_openlibs(L);

	luabridge::getGlobalNamespace(L).
		addCFunction("sayHello", sayHello);

	luaL_dostring(L, "sayHello()");

	GameServer server("0.0.0.0", 7300);
	server.run();

    return 0;
}

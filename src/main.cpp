#include <uv.h>
#include "logic/GameServer.h"

int main(int argc, char** argv) {
	
	GameServer server("0.0.0.0", 7300);

	server.run();

    return 0;
}

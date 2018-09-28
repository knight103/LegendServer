#include "GameServer.h"

GameServer::GameServer(const char* ip, int port) {
	_ip = ip;
	_port = port;
	run();
}

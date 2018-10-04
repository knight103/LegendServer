#include "GameServer.h"
#include "GameClient.h"

GameServer::GameServer(const char* ip, int port) {
	_ip = ip;
	_port = port;
	run();
}

TCPClient* GameServer::newClient(uv_tcp_t* handle) {
	return GameClient::create(handle);
}
#include "GameServer.h"
#include "GameClient.h"

void write_cb(uv_write_t* req, int status);

GameServer::GameServer(const char* ip, int port) {
	_ip = ip;
	_port = port;
	run();
}

TCPClient* GameServer::newClient(uv_tcp_t* handle) {
	GameClient* client = GameClient::create(handle);
	return client;
}

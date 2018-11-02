#ifndef __GAMESERVER_H__
#define __GAMESERVER_H__

#include "core/TCPServer.h"

class GameServer : public TCPServer {
public:
	GameServer(const char* ip, int port);

	virtual TCPClient* newClient(uv_tcp_t* handle) override;


};

#endif

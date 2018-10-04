#ifndef __GAMECLIENT_H__
#define __GAMECLIENT_H__
#include "core/TCPClient.h"

class GameProtocol;

class GameClient : public TCPClient {
public:
	static GameClient * create(uv_tcp_t* uv_handle);

    virtual void on_data_read(const char* data, size_t size) override;

	void onReadProtocol(GameProtocol* protocol);
};

#endif
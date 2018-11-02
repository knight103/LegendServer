#ifndef __GAMECLIENT_H__
#define __GAMECLIENT_H__
#include "core/TCPClient.h"
#include "GameProtocol.h"
#include <map>
#include <string>

class GameProtocol;

class GameClient : public TCPClient {
public:
	static GameClient * create(uv_tcp_t* uv_handle);

	void sendLogin();

	void protocolSend(uint32_t protoId, void* buf, size_t size);

    virtual void on_data_read(const char* data, size_t size) override;

	void onReadProtocol(GameProtocol* protocol);

	void onProtocol(uint32_t pid, const char* data);

	virtual void onInit() override;

protected:
	void onCM_LOGIN(uint32_t pid, const char* data);

};

#endif
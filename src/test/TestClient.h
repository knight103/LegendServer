#ifndef __TESTCLIENT_H__
#define __TESTCLIENT_H__

#include <uv.h>
#include "core/ProtocolDefine.h"

class GameProtocol;

class TestClient {
public:
	TestClient(uv_loop_t* loop);
	~TestClient();
	void connect(const char* ip, int port);

	void disconnect();

	void sendString(const char* str);

	void sendData(void* data, size_t size);

	void send(char* buf, size_t size);

	void on_recv(const char* data, size_t readn);

	void on_data_read(const char* data, size_t size);

	void onProtocol(GameProtocol* gp);

	void protocolSend(uint32_t protoId, void* buf, size_t size);

	uv_loop_t* _loop;
	uv_tcp_t* _uv_handle;
	uv_connect_t* _uv_connect;

	char* _buffer;
	size_t _bufsize;
	ProtocolHeader* _cur_header = nullptr;
};

#endif

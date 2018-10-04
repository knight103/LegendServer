#ifndef __TESTCLIENT_H__
#define __TESTCLIENT_H__

#include <uv.h>

class TestClient {
public:
	TestClient(uv_loop_t* loop);
	~TestClient();
	void connect(const char* ip, int port);

	void disconnect();

	void sendString(const char* str);

	void sendData(void* data, size_t size);

	void send(char* buf, size_t size);

private:
	uv_loop_t* _loop;
	uv_tcp_t* _uv_handle;
	uv_connect_t* _uv_connect;
};

#endif

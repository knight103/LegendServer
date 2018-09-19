
#include "TestClient.h"
#include "core/ProtocolDefine.h"

void on_connect(uv_connect_t*, int);
void write_cb(uv_write_t*, int);

TestClient::TestClient(uv_loop_t* loop) {
	_loop = loop;
}

TestClient::~TestClient() {
}

void TestClient::connect(const char* ip, int port) {

	int r;

	_uv_handle = new uv_tcp_t;
	r = uv_tcp_init(_loop, _uv_handle);
	if (r) {
		printf("uv_tcp_init failed");
		return;
	}

	_uv_connect = new uv_connect_t;
	_uv_connect->data = this;

	struct sockaddr_in dest;
	r = uv_ip4_addr(ip, port, &dest);
	if (r) {
		printf("uv_ip4_addr failed");
		return;
	}

	uv_tcp_connect(_uv_connect, _uv_handle, (const sockaddr*)&dest, on_connect);
}

void TestClient::disconnect() {
	uv_close((uv_handle_t*)_uv_handle, NULL);
	delete _uv_handle;
	delete _uv_connect;
}

void TestClient::sendString(const char* str) {

	int headLen = sizeof(ProtocolHeader);
	int bodyLen = strlen(str) + 1;

	char* sendBuf = new char[headLen + bodyLen];
	ProtocolHeader header;
	header.len = headLen + bodyLen;

	memcpy(sendBuf, &header, headLen);
	memcpy(sendBuf + headLen, str, bodyLen);
	
	send(sendBuf, header.len);
}

void TestClient::send(char* buf, size_t size) {
	uv_write_t *req = new uv_write_t;
	req->data = this;
	req->write_buffer = uv_buf_init(buf, size);
	
	uv_write(req, (uv_stream_t*)_uv_handle, &req->write_buffer, 1, write_cb);
}

void on_connect(uv_connect_t* req, int status) {
	TestClient* client = (TestClient*) req->data;
	client->sendString("11111");
	printf("·¢ËÍ......\n");
}

void write_cb(uv_write_t* req, int status) {
	TestClient* client = (TestClient*)req->data;
	client->disconnect();

	delete[] req->write_buffer.base;
	delete req;
}


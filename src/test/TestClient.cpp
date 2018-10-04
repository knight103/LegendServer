
#include "TestClient.h"
#include "core/ProtocolDefine.h"
#include "logic/GameProtocol.h"

struct ProtocolSend {
	uint32_t _protoId;  // id
	uint32_t _dataLen;  // 整体长度
};

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

void TestClient::sendData(void* data, size_t size) {

	int headLen = sizeof(ProtocolHeader);
	int bodyLen = size;

	char* sendBuf = new char[headLen + bodyLen];
	ProtocolHeader header;
	header.len = headLen + bodyLen;

	memcpy(sendBuf, &header, headLen);
	memcpy(sendBuf + headLen, data, bodyLen);

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

	size_t extDataLen = 100;
	size_t dataLen = 100 + 8;

	char* buffer = new char[dataLen];
	memset(buffer, 0, dataLen);

	ProtocolSend proto;
	proto._protoId = 6666;
	proto._dataLen = dataLen;

	memcpy(buffer, &proto, sizeof(ProtocolSend));
	memcpy(buffer + sizeof(ProtocolSend), "fuck", 5);

	client->sendData(buffer, dataLen);

	printf("sending......\n");
}

void write_cb(uv_write_t* req, int status) {
	TestClient* client = (TestClient*)req->data;
	client->disconnect();
	delete req;
}



#include "TestClient.h"
#include "core/ProtocolDefine.h"
#include "logic/GameProtocol.h"

struct ProtocolSend {
	uint32_t _protoId;  // id
	uint32_t _dataLen;  // 整体长度
};

void on_connect(uv_connect_t*, int);
void write_cb(uv_write_t*, int);
void alloc_cb(uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf);
void on_read(uv_stream_t* stream, ssize_t nread, const uv_buf_t* buf);

TestClient::TestClient(uv_loop_t* loop) {
	_loop = loop;
	_buffer = new char[DEFAULT_RCVBUF_SIZE * 2];
	_bufsize = 0;
}

TestClient::~TestClient() {
	delete[] _buffer;
}

void TestClient::connect(const char* ip, int port) {

	int r;

	_uv_handle = new uv_tcp_t;
	_uv_handle->data = this;
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

	TestClient* client = (TestClient*)req->data;

	uv_read_start((uv_stream_t*)client->_uv_handle, alloc_cb, on_read);
}

void write_cb(uv_write_t* req, int status) {
	//TestClient* client = (TestClient*)req->data;
	//client->disconnect();
	delete req;
}

void alloc_cb(uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf) {
	// TODO: 封装buffer管理
	char *base = new char[suggested_size];
	memset(base, 0, suggested_size);
	*buf = uv_buf_init(base, suggested_size);
}

void on_read(uv_stream_t* stream, ssize_t nread, const uv_buf_t* buf) {
	TestClient* client = (TestClient*)stream->data;
	do {
		if (nread <= 0) {
			if (nread == UV_ECONNRESET || nread == UV_EOF) {
			}
			break;
		}
		else {
			client->on_recv(buf->base, nread);
		}
	} while (0);


	if (buf->base) {
		delete[]buf->base;
	}
}

void TestClient::on_recv(const char* data, size_t readn) {

	// 读到的数据放入buffer中
	memcpy(_buffer, data, readn);

	const char* read_p = _buffer; // buffer游标
	uint32_t curReadSize = readn;

	for (;;) {
		if (curReadSize < sizeof(ProtocolHeader)) {
			break;
		}

		ProtocolHeader header = *(ProtocolHeader*)read_p;
		if (curReadSize >= header.len) {
			this->on_data_read(read_p, header.len);
			// 删除buffer中已读部分
			curReadSize -= header.len;
			memcpy(_buffer, read_p + header.len, curReadSize);
			// 游标移向下一个协议
			read_p = _buffer + header.len;
		}
	}
}

void TestClient::on_data_read(const char* data, size_t size) {
	ProtocolHeader protoHeader = *(ProtocolHeader*)data;

	const char* pBody = data + sizeof(ProtocolHeader); // 包体
	size_t curReadSize = sizeof(ProtocolHeader);

	while (curReadSize < size) {
		GPReader reader(pBody, size - sizeof(ProtocolHeader));

		if (!reader.isOK()) // 协议有问题，直接抛弃后续的包
			return;

		GameProtocol * gp = new GameProtocol;
		gp->protoId = reader.getClassID();
		gp->data.resize(reader.getDataLen());
		memcpy(&gp->data[0], pBody, reader.getDataLen());

		pBody += reader.getDataLen();
		curReadSize += reader.getDataLen();
	}
	return;
}


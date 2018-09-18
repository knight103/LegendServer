#include "uv.h"

// 协议结构体
struct ProtocolHeader {
	uint16_t Sign;
	uint16_t DataLen;
};

uv_loop_t *loop;

void on_write(uv_write_t* req, int status) {
	printf("写完毕....\n");
}

void on_connect(uv_connect_t* req, int status) {
	
	const char say[] = "fucku motsssssssssssssssssssssssssssssssssssssssssssher!!!!!";

	ProtocolHeader header = {111, 0};
	char* ext_data = new char[sizeof(ProtocolHeader) + 100];

	header.DataLen = sizeof(ProtocolHeader) + strlen(say) + 1;

	memcpy(ext_data, &header, sizeof(ProtocolHeader));
	memcpy(ext_data + sizeof(ProtocolHeader), say, strlen(say) + 1);

	uv_buf_t buf;
	uv_buf_init(ext_data, header.DataLen);

	uv_write_t wreq;

	uv_write(&wreq, req->handle, &buf, 1, on_write);
}

int main() {

	loop = uv_default_loop();

	uv_tcp_t *socket = new uv_tcp_t();
	uv_tcp_init(loop, socket);

	uv_connect_t* connect = new uv_connect_t;

	struct sockaddr_in dest;
	uv_ip4_addr("0.0.0.0", 7300, &dest);

	uv_tcp_connect(connect, socket, (const sockaddr*)&dest, on_connect);

	uv_run(loop, UV_RUN_DEFAULT);

    return 0;
}
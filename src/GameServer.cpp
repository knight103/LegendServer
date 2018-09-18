//
//  GameServer.cpp
//  01_system_info
//
//  Created by harry on 2018/9/13.
//

#include "GameServer.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include "Constant.h"

#define CHECK(r, msg) if (r) {                                                       \
log_error("%s: [%s(%d): %s]\n", msg, uv_err_name((r)), (int) r, uv_strerror((r))); \
exit(1);                                                                           \
}

void alloc_cb(uv_handle_t* handle,size_t suggested_size,uv_buf_t* buf) {
    ClientSession* session = (ClientSession*) handle->data;
    // TODO: 封装buffer管理
    char *base = new char[suggested_size];
    memset(base, 0, suggested_size);
    *buf = uv_buf_init(base, suggested_size);
}

/// 处理客户端消息
void on_read(uv_stream_t* stream, ssize_t nread, const uv_buf_t* buf) {
    if (nread < 0) {
        if (nread == UV_EOF) {
            log_info("client close connection");
            
        } else {
            log_error("error to read");
        }
        return;
    }
    
    ClientSession* session = (ClientSession*) stream->data;
    
    session->on_recv(buf->base, nread);
    
    if (buf->base) {
        delete []buf->base;
    }
}

/// 处理新客户端连接
void on_new_connection(uv_stream_t *server_stream, int status) {
    int r;
    
    GameServer* server = (GameServer*) server_stream->data;
    uv_tcp_t* client = new uv_tcp_t;
    
    r = uv_tcp_init(server->loop(), client);
    CHECK(r, "uv_tcp_init失败");
    
    r = uv_accept((uv_stream_t*)server->uv_handle(), (uv_stream_t*)client);
    CHECK(r, "uv_accept失败");
    
    // 获取客户端信息
    struct sockaddr sockname, peername;
    int namelen;
    
    namelen = sizeof(sockname);
    r = uv_tcp_getsockname(client, &sockname, &namelen);
    CHECK(r, "uv_tcp_getsockname失败");
    
    namelen = sizeof(peername);
    r = uv_tcp_getpeername(client, &peername, &namelen);
    CHECK(r, "uv_tcp_getpeername失败");
    
    // 建立连接成功，创建ClientSession
	ClientSession * session = ClientSession::create(client);
	server->addClient(session);
    
    r = uv_read_start((uv_stream_t*)client, alloc_cb, on_read);
    CHECK(r, "uv_read_start失败");
    
	log_verbose("新客户端连接");
}

GameServer::GameServer() {
	_loop = uv_default_loop();
    _uv_handle = new uv_tcp_t;
}

GameServer::~GameServer() {
    delete _uv_handle;
}

void GameServer::start()
{
    int r;
    struct sockaddr_in addr;
    
    _uv_handle->data = this;
    
    r = uv_tcp_init(_loop, _uv_handle);
    CHECK(r, "建立TCP句柄失败\n");
    
    // 绑定地址和端口
    r = uv_ip4_addr(IP, PORT, &addr);
    CHECK(r, "创建端口失败");
    
    r = uv_tcp_bind(_uv_handle, (const struct sockaddr*)&addr, 0);
    CHECK(r, "uv_tcp_bind失败");
    
    r = uv_listen((uv_stream_t*)_uv_handle, 128, on_new_connection);
    CHECK(r, "uv_listen失败");
    
	log_verbose("开始主循环......");
    uv_run(_loop, UV_RUN_DEFAULT);
}

void GameServer::addClient(ClientSession* session) {
	_clients.push_back(session);
	session->retain();
}

ClientSession::ClientSession() {
    _buffer = new char[DEFAULT_RCVBUF_SIZE * 2];
	_bufsize = 0;
}

ClientSession::~ClientSession() {
	delete[] _buffer;
}

ClientSession * ClientSession::create(uv_tcp_t* uv_handle) {
	ClientSession *sprite = new (std::nothrow) ClientSession();
	if (sprite && sprite->init(uv_handle))
	{
		return sprite;
	}
	CC_SAFE_DELETE(sprite);
	return nullptr;
}

bool ClientSession::init(uv_tcp_t* uv_handle) {
	_uv_handler = uv_handle;
	_uv_handler->data = this;
	return true;
}

void ClientSession::on_recv(const char* data, size_t readn) {
    // TODO:
    // 每个Client都有一个buffer
    // 读取的字节追加到buffer中
    // 处理buffer中的数据...

	// 读到的数据放入buffer中
	memcpy(_buffer, data, readn);

	const char* read_p = _buffer; // buffer游标
	uint32_t curReadSize = readn;
		
	for (;;) {
		if (curReadSize < sizeof(ProtocolHeader)) {
			break;
		}

		ProtocolHeader header = *(ProtocolHeader*)read_p;
		if (curReadSize >= header.DataLen) {
			this->on_data_read(read_p, header.DataLen);
			// 删除buffer中已读部分
			curReadSize -= header.DataLen;
			memcpy(_buffer, read_p + header.DataLen, curReadSize);
			// 游标移向下一个协议
			read_p = _buffer;
		}
	}
}

// 处理完整的协议数据
void ClientSession::on_data_read(const char* data, size_t size) {
	ProtocolHeader protoHeader = *(ProtocolHeader*)data;
	log_info("read proto");
}


void encryptData(void* buff, uint32_t buffLen, uint8_t key){
    uint8_t *p8 = (uint8_t*)buff;
    uint32_t xorKey = (uint32_t)key;
    
    memset(&((uint8_t*)(&xorKey))[1], (uint8_t)xorKey, 1);
    memset(&((uint8_t*)(&xorKey))[2], key ^ 53, 2);
    uint32_t *xorData = (uint32_t*)p8;
    for (uint32_t i = 0; i < buffLen / 4; i++) {
        uint32_t tmp = (xorData[i] + 157) ^ xorKey;
        xorData[i] = tmp;
        xorKey = tmp ^ 0x70E08121;
    }
    
    if (buffLen % 4 != 0) {
        uint32_t remainData = 0;
        memcpy(&remainData, &p8[buffLen / 4 * 4], buffLen % 4);
        remainData ^= xorKey;
        memcpy(&p8[buffLen / 4 * 4], &remainData, buffLen % 4);
    }
}

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
#include "log.h"

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
            log_info("客户端关闭连接");
            
        } else {
            log_error("读取消息发生错误");
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
    
    GameServer* s = (GameServer*) server_stream->data;
    uv_tcp_t* client = new uv_tcp_t;
    
    r = uv_tcp_init(s->_loop, client);
    CHECK(r, "uv_tcp_init失败");
    
    r = uv_accept((uv_stream_t*)s->_server, (uv_stream_t*)client);
    CHECK(r, "uv_accept失败");
    
    // 建立连接成功，创建ClientSession
    ClientSession * session = new ClientSession();
    session->_client = client;
    client->data = session;
    
    r = uv_read_start((uv_stream_t*)client, alloc_cb, on_read);
    CHECK(r, "uv_read_start失败");
    
    log_info("新客户端连接: %d\n", client->accepted_fd);
}

GameServer::GameServer() {
    _loop = uv_default_loop();
    _server = new uv_tcp_t;
}

GameServer::~GameServer() {
    delete _server;
}

void GameServer::start()
{
    int r;
    struct sockaddr_in addr;
    
    _server->data = this;
    
    r = uv_tcp_init(_loop, _server);
    CHECK(r, "建立TCP句柄失败\n");
    
    // 绑定地址和端口
    r = uv_ip4_addr(IP, PORT, &addr);
    CHECK(r, "创建端口失败");
    
    r = uv_tcp_bind(_server, (const struct sockaddr*)&addr, 0);
    CHECK(r, "uv_tcp_bind失败");
    
    r = uv_listen((uv_stream_t*)_server, 128, on_new_connection);
    CHECK(r, "uv_listen失败");
    
    log_info("开始主循环......");
    uv_run(_loop, UV_RUN_DEFAULT);
}

void ClientSession::on_recv(char* data, size_t readn) {
    log_info("客户端消息: %s \n", data);
}

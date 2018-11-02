/****************************************************************************
Copyright (c) 2018 harry.zhu

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/

#include "TCPServer.h"
#include "TCPClient.h"
#include "Constant.h"

// 新客户端连接
void on_new_connection(uv_stream_t*, int);
// 收到客户端消息
void alloc_cb(uv_handle_t*, size_t,uv_buf_t*);
void on_read(uv_stream_t*, ssize_t, const uv_buf_t*);


TCPServer::TCPServer()
{
    _loop = uv_default_loop();
    _uv_handle = new uv_tcp_t;
}

TCPServer::~TCPServer() {
    delete _uv_handle;
}

void TCPServer::run() {
    int r;
    struct sockaddr_in addr;
    
    _uv_handle->data = this;
    
    r = uv_tcp_init(_loop, _uv_handle);
    if (r) {
        log_error("can not init tcp handle");
        return;
    }
    
    // 绑定地址和端口
    r = uv_ip4_addr(_ip, _port, &addr);
    r = uv_tcp_bind(_uv_handle, (const struct sockaddr*)&addr, 0);
    r = uv_listen((uv_stream_t*)_uv_handle, 128, on_new_connection);
    if (r) {
        log_error("can not init tcp handle");
        return;
    }
    
	log_verbose("start main loop......");
    uv_run(_loop, UV_RUN_DEFAULT);
}

void TCPServer::addClient(TCPClient* client) {
	_clients.push_back(client);
	client->setServer(this);
	client->retain();
}

void TCPServer::removeClient(TCPClient* session) {
	for (auto it = _clients.begin(); it != _clients.end(); ++it) {
		if (*it == session)
			(*it)->release();
			it = _clients.erase(it);
		break;
	}
}

TCPClient* TCPServer::newClient(uv_tcp_t* handle) {
	TCPClient* tcpClient = TCPClient::create(handle);
	return tcpClient;
}

/// 处理新客户端连接
void on_new_connection(uv_stream_t *server_stream, int status) {
    int r;
    
    TCPServer* server = (TCPServer*) server_stream->data;
    uv_tcp_t* client = new uv_tcp_t;
    
    r = uv_tcp_init(server->loop(), client);
    if(r) {
        log_error("uv_tcp_init failed");
        return;
    }

    r = uv_accept((uv_stream_t*)server->uv_handle(), (uv_stream_t*)client);
    if(r) {
        log_error("uv_accept failed");
        return;
    }
    
    // 获取客户端信息
    struct sockaddr sockname, peername;
    int namelen;
    
    namelen = sizeof(sockname);
    r = uv_tcp_getsockname(client, &sockname, &namelen);
    if(r) {
        log_error("uv_tcp_getsockname failed");
        return;
    }
    
    namelen = sizeof(peername);
    r = uv_tcp_getpeername(client, &peername, &namelen);
    if(r) {
        log_error("uv_tcp_getpeername failed");
        return;
    }
    
    // 建立连接成功，创建ClientSession
	TCPClient* session = server->newClient(client);
    server->addClient(session);

    r = uv_read_start((uv_stream_t*)client, alloc_cb, on_read);
    if(r) {
        log_error("uv_read_start failed");
        return;
    }
    
	log_verbose("client connected");
	session->onInit();
}

void alloc_cb(uv_handle_t* handle,size_t suggested_size,uv_buf_t* buf) {
    TCPClient* session = (TCPClient*) handle->data;
    // TODO: 封装buffer管理
    char *base = new char[suggested_size];
    memset(base, 0, suggested_size);
    *buf = uv_buf_init(base, suggested_size);
}

/// 处理客户端消息
void on_read(uv_stream_t* stream, ssize_t nread, const uv_buf_t* buf) {
	TCPClient* session = (TCPClient*)stream->data;

	do {
		if (nread <= 0) {
			if (nread == UV_ECONNRESET || nread == UV_EOF) {
				log_info("client disconnected");
				session->onDisconnect();
			}
			break;
		} else {
			session->on_recv(buf->base, nread);
		}
	} while(0);


	if (buf->base) {
		delete[]buf->base;
	}
}



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

#ifndef __TCPCLIENT_H__
#define __TCPCLIENT_H__

#include <uv.h>
#include "core/Ref.h"
#include "ProtocolDefine.h"

class TCPServer;

class TCPClient : public Ref {
public:
	static TCPClient * create(uv_tcp_t* uv_handle);

	bool init(uv_tcp_t* uv_handle);


	void setServer(TCPServer* server) { _server = server; }
	TCPServer* getServer() { return _server; }

	void onDisconnect();

    void on_recv(const char* data, size_t readn);

	void on_data_read(const char* data, size_t size);
private:
    TCPClient();
	~TCPClient();

private:
    char* _buffer;
	size_t _bufsize;

    ProtocolHeader* _cur_header = nullptr;
	uv_tcp_t * _uv_handler;
	TCPServer* _server;
};

#endif

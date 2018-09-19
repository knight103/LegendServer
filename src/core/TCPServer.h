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

#ifndef __TCPSERVER_H__
#define __TCPSERVER_H__

#include <uv.h>
#include <list>
#include "cocos/CCRef.h"

class TCPClient;

class TCPServer : public Ref {
public:

	TCPServer();
    ~TCPServer();

    void run();

	void addClient(TCPClient* session);

public: // 属性方法
    uv_loop_t * loop() { return _loop; }
	uv_tcp_t* uv_handle() { return _uv_handle; }

protected:
	const char* _ip;
	int _port;

	uv_loop_t *_loop;
	uv_tcp_t *_uv_handle;
	std::list<TCPClient*> _clients;
};


#endif
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

#include <iostream>
#include "TCPClient.h"
#include "Constant.h"

TCPClient::TCPClient() {
    _buffer = new char[DEFAULT_RCVBUF_SIZE * 2];
	_bufsize = 0;
}

TCPClient::~TCPClient() {
	delete[] _buffer;
}

TCPClient * TCPClient::create(uv_tcp_t* uv_handle) {
	TCPClient *sprite = new (std::nothrow) TCPClient();
	if (sprite && sprite->init(uv_handle))
	{
		return sprite;
	}
	CC_SAFE_DELETE(sprite);
	return nullptr;
}

bool TCPClient::init(uv_tcp_t* uv_handle) {
	_uv_handler = uv_handle;
	_uv_handler->data = this;
	return true;
}

void TCPClient::on_recv(const char* data, size_t readn) {
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

// 处理完整的协议数据
void TCPClient::on_data_read(const char* data, size_t size) {
	ProtocolHeader protoHeader = *(ProtocolHeader*)data;
	const char* ext = data + sizeof(ProtocolHeader);
	log_info("read proto%s", ext);
}

/*
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

*/
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


#ifndef __PROTOCOLDEFINE_H__
#define __PROTOCOLDEFINE_H__

#include <stdint.h>
#include "core/Constant.h"

#define PROTOCOL_SIGN (0x0000)

#define PROTOCOL_FLAG_STRING (0x0010)
#define PROTOCOL_FLAG_OTHER (0x0100)

struct ProtocolHeader {
    uint16_t sign;  // 标识符
    uint16_t len;   // 总大小(包括头大小)
	uint16_t flag;	

    inline ProtocolHeader() {
        sign = PROTOCOL_SIGN;
    }
};

/*

// 数据区分隔标识
#define cTcpDataSignFlag    (0xD825)
// 封包序号异或值
#define PACKET_ORDER_XOR_VALUE  (0x7E)
// 封包长度异或值
#define PACKET_DATA_LEN_XOR_VALUE  (0x31BC)
// 数据包标记
enum MirNetPackFlag {
	mnf_Normal = 0,
	mnf_Encrypt = 1,
	mnf_Zlib_Compressed = 1 << 1,
	mnf_Lzo_Compressed = 1 << 2,
};
// 数据包头
struct MirNetPacketHeader {
	uint16_t Sign;
	uint16_t DataLen;
	uint16_t PacketOrder;
	uint16_t Flag;

	inline MirNetPacketHeader() {
		Sign = cTcpDataSignFlag;
		Flag = 0;
	}

	inline void encode() {
		uint16_t oldOrder = PacketOrder;
		PacketOrder ^= PACKET_ORDER_XOR_VALUE;
		*(((uint8_t *)(&oldOrder)) + 1) = oldOrder;
		DataLen ^= PACKET_DATA_LEN_XOR_VALUE;
	}

	inline void decode() {
		PacketOrder ^= PACKET_ORDER_XOR_VALUE;
		DataLen ^= PACKET_DATA_LEN_XOR_VALUE;
	}
};

*/

#endif

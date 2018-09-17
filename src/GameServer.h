//
//  GameServer.h
//  01_system_info
//
//  Created by harry on 2018/9/13.
//

#ifndef GameServer_h
#define GameServer_h

#include "uv.h"
#include "cocos/CCRef.h"
#include <list>

class ClientSession;

class GameServer : public Ref {
public:

	inline uv_loop_t * loop() { return _loop; }
	inline uv_tcp_t* uv_handle() { return _uv_handle; }


	void addClient(ClientSession* session);

    GameServer();
    ~GameServer();
    
    void start();

private:
	const char* IP = "0.0.0.0";
	const int PORT = 7300;

	uv_loop_t *_loop;
	uv_tcp_t *_uv_handle;
	std::list<ClientSession*> _clients;
};

class ClientSession : public Ref {
public:
	static ClientSession * create(uv_tcp_t* uv_handle);

	bool init(uv_tcp_t* uv_handle);
public:
    void on_recv(char* data, size_t readn);

private:
	uv_tcp_t * _uv_handler;
};

// 数据区分隔标识
#define cTcpDataSignFlag    (0xD825)
// 封包序号异或值
#define PACKET_ORDER_XOR_VALUE  (0x7E)
// 封包长度异或值
#define PACKET_DATA_LEN_XOR_VALUE  (0x31BC)
// 数据包标记
enum MirNetPackFlag {
    mnf_Normal              = 0,
    mnf_Encrypt             = 1,
    mnf_Zlib_Compressed     = 1 << 1,
    mnf_Lzo_Compressed      = 1 << 2,
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


#endif /* GameServer_h */

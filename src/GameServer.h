//
//  GameServer.h
//  01_system_info
//
//  Created by harry on 2018/9/13.
//

#ifndef GameServer_h
#define GameServer_h

#include "uv.h"

class GameServer {
private:
    const char* IP = "0.0.0.0";
    const int PORT = 7300;
    
public:
    uv_loop_t *_loop;
    uv_tcp_t *_server;
    
public:
    GameServer();
    ~GameServer();
    
    void start();
};

class ClientSession {
    
public:
    uv_tcp_t *_client;

public:
    
    void close_connection();
    
    void on_recv(char* data, size_t readn);
    
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

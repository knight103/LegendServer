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
    void on_recv(char* data, size_t readn);
    
};



#endif /* GameServer_h */

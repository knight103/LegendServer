import(".init")
import(".protocol")

local server = GameServer.getInstance()

server:bind(S_CLIENT_CONNECT, function()
    print("新客户端连接")
end)

server:bind(S_CLIENT_MESSAGE, function()
    print("收到客户端消息")
end)

server:bind(S_CLIENT_DISCONNECT, function()
    print("客户端关闭")
end)

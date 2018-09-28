local server = class("server")

function server:ctor(ip, port)
    self.ip = ip
    self.port = port
    print(self.ip)
    print(self.port)
    math.randomseed(os.time())
end

function server:run()
    GameServer(self.ip, self.port)
end

return server
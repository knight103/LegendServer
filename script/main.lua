
require "ls.init"

local function main()
    local server = require("app.server").new("0.0.0.0", 7300)
    server:run()
end

local status, msg = xpcall(main, __G__TRACKBACK__)
if not status then
    print(msg)
end

# LegendServer
用lua写的服务器
PS: 写着玩玩~~~ 本帅不负责任何逻辑功能

# Build
```
git submodule update --init
mkdir build && cd build
cmake ..  （Macos执行 cmake -G Xcode ..）
```

# TODO
* 文件管理系统 (FileUtils) ⭕️
  * 基本操作
  * 搜索路径管理
  * 缓存管理
* lua引擎 (LuaEngine)
  * 自动绑定脚本
* 数据库管理系统 (FileUtils)
* ...


# Thank
感谢以下开源库的无私奉献

* libuv
* lua
* cocos2d-x
* rapidjson
* tinyxml2
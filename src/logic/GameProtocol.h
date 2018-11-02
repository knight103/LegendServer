#include "core/LegendServer.h"
#include <stdint.h>
#include <vector>

struct GameProtocol {
	uint32_t protoId;
	std::vector<char> data;
};

struct GPReader {
	GPReader(const char* buffer, size_t bufferLen) {
		_ok = false;
		_classId = *((uint32_t*)buffer);
		_headLen = 4;
		
		_dataLen = *((uint32_t*)&buffer[_headLen]);
		
		// 判断数据长度是否合法
		if (_dataLen <= bufferLen)
			_ok = true;
	}

	inline bool isOK() { return _ok; }
	inline uint32_t getHeadLen() { return _headLen; }
	inline uint32_t getClassID() { return _classId; }
	inline uint32_t getDataLen() { return _dataLen; }
private:
	bool _ok;
	uint32_t _classId;
	uint32_t _headLen;
	uint32_t _dataLen;
};

#define CM_BASE		0x0000
#define CM_LOGIN	(CM_BASE + 1)


#define SM_BASE		0X1000
#define SM_LOGIN	(SM_BASE + 1)

struct TCM_BASE {
};

struct TCM_LOGIN : TCM_BASE {
	char name[16];
	char pwd[16];
};

struct TSM_LOGIN : TCM_BASE {
};

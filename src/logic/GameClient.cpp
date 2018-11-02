#include "GameClient.h"
#include "GameServer.h"

struct ProtocolSend {
	uint32_t _protoId;  // id
	uint32_t _dataLen;  // 整体长度
};


GameClient * GameClient::create(uv_tcp_t* uv_handle) {
	GameClient *sprite = new (std::nothrow) GameClient();
	if (sprite && sprite->init(uv_handle))
	{
		return sprite;
	}
	CC_SAFE_DELETE(sprite);
	return nullptr;
}

void GameClient::onInit() {
	TSM_LOGIN t;
	protocolSend(SM_LOGIN, (void*) &t, sizeof(t)); 
}

void GameClient::protocolSend(uint32_t protoId, void* buf, size_t size) {

	ProtocolSend ps;
	ps._protoId = protoId;
	ps._dataLen = 8 + size;

	char* buffer = new char[ps._dataLen];
	memset(buffer, 0, ps._dataLen);

	memcpy(buffer, &ps, sizeof(ProtocolSend));
	memcpy(buffer + sizeof(ProtocolSend), buf, size);

	GameClient::sendData((void*)buffer, ps._dataLen);
}

void GameClient::sendLogin() {
	TSM_LOGIN data;
	protocolSend(SM_LOGIN, (void*)&data, sizeof(TSM_LOGIN));
}

void GameClient::on_data_read(const char* data, size_t size) {
	ProtocolHeader protoHeader = *(ProtocolHeader*)data;

	const char* pBody = data + sizeof(ProtocolHeader); // 包体
	size_t curReadSize = sizeof(ProtocolHeader);
	
	while (curReadSize < size) {
		GPReader reader(pBody, size - sizeof(ProtocolHeader));

		if (!reader.isOK()) // 协议有问题，直接抛弃后续的包
			return;

		GameProtocol * gp = new GameProtocol;
		gp->protoId = reader.getClassID();
		gp->data.resize(reader.getDataLen());
		memcpy(&gp->data[0], pBody, reader.getDataLen());

		onReadProtocol(gp);
		pBody += reader.getDataLen();
		curReadSize += reader.getDataLen();
	}
	return;
}

void GameClient::onReadProtocol(GameProtocol* protocol) {
	log_info("GameProtocol(%d)", protocol->protoId);
	log_info("%s\n", &protocol->data[8]);
	onProtocol(protocol->protoId, &protocol->data[8]);
}

void GameClient::onProtocol(uint32_t pid, const char* data) {
	switch (pid)
	{
	case CM_LOGIN:
		onCM_LOGIN(pid, data);
		break;
	}
}

void GameClient::onCM_LOGIN(uint32_t pid, const char* data) {
	TCM_LOGIN ss = *((TCM_LOGIN*)data);

	log_info("name: %s", ss.name);
	log_info("pwd: %s", ss.pwd);

}



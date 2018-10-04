#include "GameClient.h"
#include "GameProtocol.h"

GameClient * GameClient::create(uv_tcp_t* uv_handle) {
	GameClient *sprite = new (std::nothrow) GameClient();
	if (sprite && sprite->init(uv_handle))
	{
		return sprite;
	}
	CC_SAFE_DELETE(sprite);
	return nullptr;
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
}


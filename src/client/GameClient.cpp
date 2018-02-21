#include "GameClient.h"

void GameClient::start(const ClientType clientType, const char* serverPotr)
{
	if (clientType == LocalConnection && serverPotr != nullptr)
		throw std::invalid_argument("serverPotr should be nullptr while clientType is LocalConnection");

	mClientChunkGroup.init();
}

void ClientChunkGroup::init(const size_t poolSize)
{
	if (mChunkPool.size() != poolSize)
		mChunkPool.resize(poolSize);

	//clean chunk
	for (auto& chunk : mChunkPool)
	{
		chunk = std::shared_ptr<ClientChunk>(new ClientChunk());
		chunk->clear();
	}
}

void ClientChunk::clear()
{
	for (auto& blockData: blockList)
		blockData.blockData = 0;
}

ClientBlock& ClientChunk::getBlock(const uint8_t x, const uint8_t y, const uint8_t z)
{
	return blockList[x + y * 16 + z * 256];
}
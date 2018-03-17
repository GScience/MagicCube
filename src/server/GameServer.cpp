#include "GameServer.h"
#include "BlockList.h"
#include "BlockGrass.h"

void GameServer::loadChunk(const int32_t chunkX, const int32_t chunkY, const int32_t chunkZ)
{
	if (!mServerChunkGroup.addChunk(chunkX, chunkY, chunkZ))
		return;

	getChunk(chunkX, chunkY, chunkZ)->getBlock(8, 8, 8) = BlockGrass;
}

std::shared_ptr<Chunk> GameServer::getChunk(const int32_t chunkX, const int32_t chunkY, const int32_t chunkZ) const
{
	return mServerChunkGroup.getChunk(chunkX, chunkY, chunkZ);
}

void GameServer::onReceivePlayerCommand(NetPlayer&, NetPackageBase&&)
{
	
}
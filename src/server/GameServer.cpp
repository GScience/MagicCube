#include "GameServer.h"
#include "BlockList.h"
#include "BlockGrass.h"
#include "NetPackage.h"
#include "NetPackage/NetPackageChunk.h"

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

void GameServer::asyncOnReceivePlayerCommand(NetPlayer&, NetPackageBase&&)
{
	
}

void GameServer::asyncOnPlayerJoin(NetPlayer& player)
{
	sendChunkToPlayer(player, 0, 0, 0);
	mPlayerList.emplace_back(&player);
}

void GameServer::sendChunkToPlayer(NetPlayer& player, int32_t chunkX, int32_t chunkY, int32_t chunkZ)
{
	NetPackageClientCommand package;
	auto packageContent = std::make_shared<NetPackageChunk>();
	package.clientCommand = SERVER_NET_PACKAGE_CHUNK;
	package.netPackage = packageContent;

	loadChunk(chunkX, chunkY, chunkZ);

	packageContent->chunkX = chunkX;
	packageContent->chunkY = chunkY;
	packageContent->chunkZ = chunkZ;
	packageContent->blockIdList.resize(4096);
	packageContent->blockIdList[0] = 1;

	player.asyncSendPackage(package);
}

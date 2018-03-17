#include <SDL2/SDL_log.h>
#include "GameClient.h"
#include "BlockList.h"
#include "NetPackage/NetPackageShakehand.h"

#define LOCAL_PORT 23333

void GameClient::connect(const ClientType clientType, const char* serverPotr)
{
	if (clientType == LocalServer && serverPotr != nullptr)
		throw std::invalid_argument("serverPotr should be nullptr while clientType is LocalConnection");

	if (clientType == LocalServer)
	{
		mLocalServer = std::make_shared<GameServer>("127.0.0.1", LOCAL_PORT);
	}
	else
		throw std::invalid_argument("don't support to connect to the remote server");
	
	//init local chunk group
	mLocalChunkGroupCache.init();

	//connect to server
	mNetClient = std::make_unique<NetClient>("127.0.0.1", LOCAL_PORT);
	auto shakeHandPackage = NetPackageShakehand();
	shakeHandPackage.playerName = "Test Player";
	mNetClient->sendPackage(shakeHandPackage);

	SDL_Log("[Server]Connect to server...");
}

void GameClient::refresh(const double timePassed)
{
	//load chunk nearby
	for (auto i = 0; i < 30; i++)
		for (auto j = 0; j < 16; j++)
			for (auto k = 0; k < 30; k++)
				loadChunk(i, j, k);

	mLoadChunkTasks.refresh();
}

void GameClient::loadChunk(const int32_t chunkX, const int32_t chunkY, const int32_t chunkZ)
{
	if (!mLocalChunkGroupCache.addChunk(chunkX, chunkY, chunkZ))
		return;

	mLoadChunkTasks.addTask(Task([&]
	()
	{
		return downloadChunkData(chunkX, chunkY, chunkZ);
	}), [&]
	(const Task& task)
	{
		auto result = task.get();
	});
}

std::shared_ptr<Chunk> GameClient::getChunk(const int32_t chunkX, const int32_t chunkY, const int32_t chunkZ) const
{
	return mLocalChunkGroupCache.getChunk(chunkX, chunkY, chunkZ);
}

std::shared_ptr<NetPackageChunk> GameClient::downloadChunkData(const int32_t chunkX, const int32_t chunkY, const int32_t chunkZ)
{
	auto test = std::make_shared<NetPackageChunk>();

	return test;
}

void GameClient::close()
{

}
#include <SDL2/SDL_log.h>
#include "GameClient.h"
#include "BlockList.h"
#include "NetPackage.h"

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
	mNetClient = std::unique_ptr<NetClient>(new NetClient("127.0.0.1", LOCAL_PORT));
	auto shakeHandPackage = NetPackageShakehand();
	shakeHandPackage.playerName = "Test Player";
	mNetClient->asyncSendPackage(shakeHandPackage);

	SDL_Log("[Server]Connect to server...");
}

void GameClient::refresh(const double timePassed)
{
	//load chunk nearby
	/*for (auto i = 0; i < 30; i++)
		for (auto j = 0; j < 16; j++)
			for (auto k = 0; k < 30; k++)
				loadChunk(i, j, k);*/

	mLoadChunkTasks.refresh();
	if (mNetClient->getReceivedPackageCount() != 0)
	{
		NetPackageBase test;
		auto package = mNetClient->getFrontPackage();

		if (package.serverCommand == SERVER_NET_PACKAGE_CHUNK)
			loadChunkFromPackage(*package.getPackage<NetPackageChunk>());

		mNetClient->popFrontPackage();
	}
}

void GameClient::loadChunkFromPackage(NetPackageChunk& chunkPackage)
{
	if (!mLocalChunkGroupCache.addChunk(chunkPackage.chunkX, chunkPackage.chunkY, chunkPackage.chunkZ))
		return;

	std::cout << "Loaded chunk " << chunkPackage.chunkX << " "<< chunkPackage.chunkY << " " << chunkPackage.chunkZ << " " << std::endl;
}

std::shared_ptr<Chunk> GameClient::getChunk(const int32_t chunkX, const int32_t chunkY, const int32_t chunkZ) const
{
	return mLocalChunkGroupCache.getChunk(chunkX, chunkY, chunkZ);
}

void GameClient::close()
{

}
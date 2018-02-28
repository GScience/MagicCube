#include "GameClient.h"
#include "BlockList.h"

void GameClient::start(const ClientType clientType, const char* serverPotr)
{
	if (clientType == LocalConnection && serverPotr != nullptr)
		throw std::invalid_argument("serverPotr should be nullptr while clientType is LocalConnection");

	mClientChunkGroup.init();
}

void GameClient::refresh(const double timePassed)
{
	//load chunk nearby
	for (auto i = 0; i < 30; i++)
		for (auto j = 0; j < 30; j++)
			mClientChunkGroup.loadChunk(30, 30, 30);

	//do tasks
	for (const auto& func:mTasks)
		func();
}

void GameClient::downloadChunk(ClientChunk& chunk)
{
	
}
#pragma once

#include "Chunk.h"
#include <functional>
#include "NetPackage/NetPackageChunk.h"
#include "TaskList.h"
#include "GameServer.h"
#include "Net.h"

enum ClientType
{
	LocalServer, RemoteServer
};

class GameClient
{
	//!all client chunk
	ChunkGroup mLocalChunkGroupCache;

	//!load chunk task
	TaskList mLoadChunkTasks;

	//!local server
	std::shared_ptr<GameServer> mLocalServer = nullptr;

	//!client io server
	std::unique_ptr<NetClient> mNetClient = nullptr;

public:
	//!load chunk
	void loadChunkFromPackage(NetPackageChunk& chunkPackage);
	
	//!get chunk
	std::shared_ptr<Chunk> getChunk(int32_t chunkX, int32_t chunkY, int32_t chunkZ) const;

	//!get client instance
	static GameClient& getInstance()
	{
		static GameClient gameClient;
		return gameClient;
	}

	//!get local server
	std::shared_ptr<GameServer> getLocalServer() const
	{
		return mLocalServer;
	}

	//!get chunk list
	ChunkGroup& getLocalChunkGroupCache() { return mLocalChunkGroupCache; }

	//!start client
	void connect(ClientType clientType = LocalServer, const char* serverPotr = nullptr);

	//!refresh
	void refresh(double timePassed);

	//!close
	void close();
};

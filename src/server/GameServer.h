#pragma once

#include "Net.h"
#include "Chunk.h"

class GameServer
{
	NetServer mServer;
	std::vector<NetPlayer*> mPlayerList;
	ChunkGroup mServerChunkGroup;

	//!receive player command
	void asyncOnReceivePlayerCommand(NetPlayer&, NetPackageBase&&);

	//!when new player join in
	void asyncOnPlayerJoin(NetPlayer&);

public:
	explicit GameServer(const char* address, const unsigned short port) :
		mServer(address, port,
			std::function<void(NetPlayer&, NetPackageBase&&)>(std::bind(&GameServer::asyncOnReceivePlayerCommand, this, std::placeholders::_1, std::placeholders::_2)),
			std::function<void(NetPlayer&)>(std::bind(&GameServer::asyncOnPlayerJoin, this, std::placeholders::_1))
		)
	{
		mServerChunkGroup.init();
	}

	//!load chunk
	void loadChunk(int32_t chunkX, int32_t chunkY, int32_t chunkZ);

	//!get chunk
	std::shared_ptr<Chunk> getChunk(int32_t chunkX, int32_t chunkY, int32_t chunkZ) const;

	//!send chunk to player
	void sendChunkToPlayer(NetPlayer& player, int32_t chunkX, int32_t chunkY, int32_t chunkZ);

	//!get player list
	std::vector<NetPlayer*>& getPlayers()
	{
		return mPlayerList;
	}
};
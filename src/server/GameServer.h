#pragma once

#include "Net.h"
#include "Chunk.h"

class GameServer
{
	NetServer mServer;
	ChunkGroup mServerChunkGroup;

public:
	explicit GameServer(const char* address, const unsigned short port) :
		mServer(address, port)
	{}

	//!load chunk
	void loadChunk(int32_t chunkX, int32_t chunkY, int32_t chunkZ);

	//!get chunk
	std::shared_ptr<Chunk> getChunk(int32_t chunkX, int32_t chunkY, int32_t chunkZ) const;

};
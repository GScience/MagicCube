#pragma once

#include <vector>
#include <memory>

union ClientBlock
{
	uint32_t	blockData;
	uint8_t		blockLight;
	uint16_t	blockId;
	uint8_t		blockSubId;
};

class ClientChunk
{
	friend class ClientChunkGroup;

	ClientChunk() = default;

public:
	ClientBlock blockList[4096];
	ClientBlock& getBlock(uint8_t x, uint8_t y, uint8_t z);

	void clear();
};

class ClientChunkGroup
{
	//!all chunk is in the list
	std::vector<std::shared_ptr<ClientChunk>> mChunkPool;

public:
	//!init chunks
	void init(size_t poolSize = 16384);
};

class GameClient
{
	//!all client chunk
	ClientChunkGroup mClientChunkGroup;
	GameClient() = default;

public:
	enum ClientType
	{
		LocalConnection, RemoteConnection
	};
	//!get client instance
	static GameClient& getInstance()
	{
		static GameClient gameClient;
		return gameClient;
	}

	void start(ClientType clientType = LocalConnection, const char* serverPotr = nullptr);
};
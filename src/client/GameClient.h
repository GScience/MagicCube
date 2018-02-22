#pragma once

#include "ClientChunk.h"

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

	//!get chunk list
	ClientChunkGroup& getClientChunkGroup() { return mClientChunkGroup; }

	//!start client
	void start(ClientType clientType = LocalConnection, const char* serverPotr = nullptr);
};

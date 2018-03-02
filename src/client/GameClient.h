#pragma once

#include "ClientChunk.h"
#include <functional>
#include "NetPackage/NetPackageChunk.h"

class GameClient
{
	//!all client chunk
	ClientChunkGroup mClientChunkGroup;
	GameClient() = default;

	//!all task should do
	std::vector<std::function<void()>> mTasks;

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

	//!add task
	void addTask(const std::function<void()>& function)
	{
		mTasks.push_back(function);
	}

	//!refresh
	void refresh(double timePassed);
	
	/*!gen chunk data
	 * should be thread-safety
	 */
	std::shared_ptr<NetPackageChunk> downloadChunkData(int32_t chunkX, int32_t chunkY, int32_t chunkZ);
};

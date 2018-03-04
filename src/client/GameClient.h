#pragma once

#include "ClientChunk.h"
#include <functional>
#include "NetPackage/NetPackageChunk.h"
#include "TaskList.h"
#include "GameServer.h"
#include <asio.hpp>

enum ClientType
{
	LocalServer, RemoteServer
};

class GameClient
{
	//!all client chunk
	ClientChunkGroup mClientChunkGroup;
	explicit GameClient() : mSocket(mIoServer) {}

	//!load chunk task
	TaskList mLoadChunkTasks;

	//!local server
	std::shared_ptr<GameServer> mLocalServer = nullptr;

	//!client io server
	asio::io_service mIoServer;
	//!socket
	asio::ip::tcp::socket mSocket;

public:
	//!get client instance
	static GameClient& getInstance()
	{
		static GameClient gameClient;
		return gameClient;
	}

	//!get chunk list
	ClientChunkGroup& getClientChunkGroup() { return mClientChunkGroup; }

	//!start client
	void connect(ClientType clientType = LocalServer, const char* serverPotr = nullptr);

	//!add task
	void addLoadChunkTask(Task&& task, const std::function<void(const Task&)>& onFinish)
	{
		mLoadChunkTasks.addTask(std::move(task), onFinish);
	}

	//!refresh
	void refresh(double timePassed);
	
	/*!gen chunk data
	 * should be thread-safety
	 */
	std::shared_ptr<NetPackageChunk> downloadChunkData(int32_t chunkX, int32_t chunkY, int32_t chunkZ);
};

#include "GameClient.h"
#include "BlockList.h"
#include "NetPackage/NetPackageShakehand.h"

#define LOCAL_PORT 23333

std::thread clientThread;

void GameClient::connect(const ClientType clientType, const char* serverPotr)
{
	if (clientType == LocalServer && serverPotr != nullptr)
		throw std::invalid_argument("serverPotr should be nullptr while clientType is LocalConnection");

	if (clientType == LocalServer)
	{
		mLocalServer = std::make_shared<GameServer>("127.0.0.1", LOCAL_PORT);
		mLocalServer->start(true);
	}
	else
		throw std::invalid_argument("don't support to connect to the remote server");
	
	//init local chunk group
	mClientChunkGroup.init();

	//connect to server
	mSocket.connect(asio::ip::tcp::endpoint(asio::ip::address::from_string("127.0.0.1"), LOCAL_PORT));

	//create client thread
	clientThread = std::thread([&]
	()
	{
		mIoServer.run();
	});

	//send shake hand package
	auto result = mSocket.send(asio::buffer(NetPackageShakehand().toString()));
}

void GameClient::refresh(const double timePassed)
{
	//load chunk nearby
	for (auto i = 0; i < 30; i++)
		for (auto j = 0; j < 30; j++)
			mClientChunkGroup.loadChunk(30, 30, 30);

	mLoadChunkTasks.refresh();
}

std::shared_ptr<NetPackageChunk> GameClient::downloadChunkData(const int32_t chunkX, const int32_t chunkY, const int32_t chunkZ)
{
	auto test = std::make_shared<NetPackageChunk>();

	return test;
}
#include "GameServer.h"
#include <thread>
#include "NetPackage/NetPackageBase.h"

std::thread serverThread;

void GameServer::start(const bool async)
{
	asyncAccept();

	if (async)
		serverThread = std::thread([&]
		()
		{
			mIoServer.run();
		});
	else
		mIoServer.run();
}

void GameServer::asyncAccept()
{
	NetPlayer netPlayer(mIoServer);

	mAcceptor.async_accept(netPlayer.mSocket, [&](const asio::error_code& ec)
	{
		if (!ec)
			return;

		netPlayer.asyncReceive();
		mNetPlayerList.emplace_back(std::move(netPlayer));

		asyncAccept();
	});
}

NetPlayer::NetPlayer(asio::io_service& ioServer) :mSocket(ioServer){}

void NetPlayer::asyncReceive()
{
	//package size
	std::vector<char> buffer(sizeof(PkSize), 0);

	mSocket.async_read_some(asio::buffer(buffer), [&]
	(const asio::error_code& ec, size_t readSize)
	{
		auto packageSize = *reinterpret_cast<PkSize*>(&buffer[0]);
		buffer.resize(packageSize);
		asio::error_code readErr;
		mSocket.read_some(asio::buffer(buffer), readErr);
	});
}
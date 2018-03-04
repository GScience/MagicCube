#include "GameServer.h"
#include <thread>

void GameServer::start()
{
	asyncAccept();
}

void GameServer::asyncAccept()
{
	asio::ip::tcp::socket socket(mIoServer);

	mAcceptor.async_accept(socket, [&](std::error_code ec)
	{
		if (ec)
			return;

		mNetPlayerList.push_back(socket);
	});
}

#include "GameServer.h"
#include <thread>
#include "NetPackage/NetPackageBase.h"

void GameServer::start()
{
	asyncAccept();
}

void GameServer::asyncAccept()
{
	asio::ip::tcp::socket socket(mIoServer);

	mAcceptor.async_accept(socket, [&](const asio::error_code& ec)
	{
		if (ec)
			return;

		PkSize packageSize;

		std::vector<char> buffer(sizeof(PkSize), 0);

		socket.read_some(asio::buffer(buffer));
		mNetPlayerList.emplace_back(std::move(socket));

		asyncAccept();
	});
}

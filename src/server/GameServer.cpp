#include "GameServer.h"
#include <thread>
#include "NetPackage/NetPackageBase.h"
#include "NetPackage/NetPackageShakehand.h"

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
	if (!mNewNetPlayer)
		mNewNetPlayer = new NetPlayer(mIoServer);

	mAcceptor.async_accept(mNewNetPlayer->mSocket, [&](const asio::error_code& er)
	{
		if (er)
			return;

		mNetPlayerList.emplace_back(std::move(*mNewNetPlayer));
		mNetPlayerList.back().asyncReceive();

		mNewNetPlayer = nullptr;

		asyncAccept();
	});
}

NetPlayer::NetPlayer(asio::io_service& ioServer) :
	mSocket(ioServer),
	mTmpPkSizeBuffer{ 0 }
{}

void NetPlayer::asyncReceive()
{
	if (mTmpPkBuffer.empty())
	{
		//read package size
		asio::async_read(mSocket, asio::buffer(mTmpPkSizeBuffer), [&]
		(const asio::error_code& er, size_t readSize)
		{
			if (er || readSize != mTmpPkSizeBuffer.size() * sizeof(char))
				return;

			//get package size
			auto packageSize = *reinterpret_cast<PkSize*>(&mTmpPkSizeBuffer[0]);
			mTmpPkBuffer.resize(packageSize);

			asyncReceive();
		});
	}
	else
	{
		//read package
		asio::async_read(mSocket, asio::buffer(mTmpPkBuffer), [&]
		(const asio::error_code& er, size_t readSize)
		{
			if (er || readSize != mTmpPkSizeBuffer.size() * sizeof(char))
				return;
			
			std::stringstream packageStream(
				std::string(mTmpPkSizeBuffer.begin(), mTmpPkSizeBuffer.end()) +  
				std::string(mTmpPkBuffer.begin(), mTmpPkBuffer.end())
			);

			//check stage
			switch (mLoginStage)
			{
			case LoginShakehand:
			{
				NetPackageShakehand shakehandPackage;
				shakehandPackage.fromStringStream(packageStream);

				if (shakehandPackage.version != 1)
					throw std::runtime_error("Wrong client version");

				break;
			}
			default:
				break;
			}
			mTmpPkBuffer.clear();
			asyncReceive();
		});
	}
}
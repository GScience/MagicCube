#include "GameServer.h"
#include <thread>
#include <iostream>
#include "NetPackage/NetPackageShakehand.h"

std::thread serverThread;

std::atomic_bool shouldStopRefresh = false;

void GameServer::start(const bool async)
{
	asyncAccept();

	serverThread = std::thread([&]()
	{
		while (!shouldStopRefresh)
			mIoServer.run_one();
	});
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
	mTmpPkSizeBuffer(sizeof(PkSize), 0)
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
				else
					std::cout << "[Server]accept a connection" << std::endl;

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

GameServer::~GameServer()
{
	shouldStopRefresh = true;
	serverThread.join();

	delete mNewNetPlayer;
}
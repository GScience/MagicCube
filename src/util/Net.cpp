#include "Net.h"
#include "NetPackage/NetPackageShakehand.h"
#include <iostream>
#include "NetPackage/NetPackageClientCommand.h"
#include "NetPackage/NetPackageServerCommand.h"

NetIoServerBase::NetIoServerBase()
{
	mIsStopped = false;
}

NetIoServerBase::~NetIoServerBase()
{
	//stop io thread
	mIoServer.stop();

	mIsStopped = true;

	//wait
	for (auto& thread : mIoThreadPool)
		if (thread.joinable())
			thread.join();
}

void NetIoServerBase::createRefreshThread()
{
	mIoThreadPool.emplace_back(std::thread([&]()
	{
		while (!mIsStopped)
			mIoServer.run_one();
	}));
}

std::unique_ptr<asio::ip::tcp::socket> NetIoServerBase::createSocket()
{
	return std::unique_ptr<asio::ip::tcp::socket>(new asio::ip::tcp::socket(mIoServer));
}

NetClient::NetClient(const std::string& address, const unsigned short port)
{
	//create refresh thread
	createRefreshThread();

	//create socket
	mClientSocket = createSocket();
	//connect
	mClientSocket->connect(asio::ip::tcp::endpoint(asio::ip::address::from_string(address), port));
}

void NetClient::sendPackage(const NetPackageBase& package) const
{
	mClientSocket->send(asio::buffer(package.toBinaryString()));
}

void NetClient::asyncReceive()
{
	//read package size
	asio::async_read(*mClientSocket, asio::buffer(mTmpPkSizeBuffer), [&]
	(const asio::error_code& er, size_t readSize)
	{
		if (er || readSize != mTmpPkSizeBuffer.size() * sizeof(char))
			return;

		//get package size
		auto packageSize = *reinterpret_cast<PkSize*>(&mTmpPkSizeBuffer[0]);
		mTmpPkBuffer.resize(packageSize);

		//read package
		auto readPackageSize = asio::read(*mClientSocket, asio::buffer(mTmpPkBuffer));

		if (readPackageSize != mTmpPkBuffer.size() * sizeof(char))
			return;

		std::stringstream packageStream(
			std::string(mTmpPkBuffer.begin(), mTmpPkBuffer.end())
		);

		NetPackageServerCommand serverCommandPackage;
		serverCommandPackage.fromStringStream(packageStream);

		mReceivedPackage.emplace_back(serverCommandPackage);
	});
}


NetServer::NetServer(const std::string& address, const unsigned short port, decltype(mReceiveCallback) receiveCallback) :
	NetIoServerBase(),
	mAcceptor(asio::ip::tcp::acceptor(getIoServer(), asio::ip::tcp::endpoint(asio::ip::address::from_string(address), port))),
	mReceiveCallback(receiveCallback)
{
	//create refresh thread
	createRefreshThread();
	asyncAccept();
}

void NetServer::asyncAccept()
{
	auto newNetPlayer = new NetPlayer(*this, createSocket());

	std::function<void(const asio::error_code&)> acceptFunc = std::bind([&](const asio::error_code& er, NetPlayer* netPlayer)
	{
		if (er)
		{
			delete netPlayer;
			return;
		}

		mNetPlayers.emplace_back(netPlayer);
		mNetPlayers.back()->asyncReceive();

		asyncAccept();
	}, std::placeholders::_1, newNetPlayer);

	mAcceptor.async_accept(*newNetPlayer->mSocket, acceptFunc);
}


NetPlayer::NetPlayer(NetServer& server, std::unique_ptr<asio::ip::tcp::socket>&& ioServer) :
	mSocket(std::move(ioServer)),
	mTmpPkSizeBuffer(sizeof(PkSize), 0),
	mServer(server)
{}

void NetPlayer::asyncReceive()
{
	//read package size
	asio::async_read(*mSocket, asio::buffer(mTmpPkSizeBuffer), [&]
	(const asio::error_code& er, size_t readSize)
	{
		if (er || readSize != mTmpPkSizeBuffer.size() * sizeof(char))
			return;

		//get package size
		auto packageSize = *reinterpret_cast<PkSize*>(&mTmpPkSizeBuffer[0]);
		mTmpPkBuffer.resize(packageSize);

		//read package
		auto readPackageSize = asio::read(*mSocket, asio::buffer(mTmpPkBuffer));

		if (readPackageSize != mTmpPkBuffer.size() * sizeof(char))
			return;

		std::stringstream packageStream(
			std::string(mTmpPkBuffer.begin(), mTmpPkBuffer.end())
		);
		
		//check stage
		switch (mLoginStage)
		{
		case LoginShakehand:
		{
			NetPackageShakehand shakehandPackage;
			shakehandPackage.fromStringStream(packageStream);

			if (shakehandPackage.version != NET_PACKAGE_VERSION)
				throw std::runtime_error("Wrong client version");
			else
				std::cout << "[Server]accept a connection " << shakehandPackage.playerName << std::endl;

			mLoginStage = LoginDone;
			break;
		}
		case LoginDone:
		{
			NetPackageClientCommand clientCommandPackage;
			clientCommandPackage.fromStringStream(packageStream);

			mServer.mReceiveCallback(*this, std::move(clientCommandPackage));
		}
		default:
			break;
		}
		mTmpPkBuffer.clear();

		asyncReceive();
	});
}

void NetPlayer::sendPackage(const NetPackageBase& package) const
{
	mSocket->send(asio::buffer(package.toBinaryString()));
}
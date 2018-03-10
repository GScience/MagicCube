#include "Net.h"
#include "NetPackage/NetPackageShakehand.h"
#include <iostream>

NetIoServerBase::NetIoServerBase()
{
	//create refresh thread
	for (unsigned i = 0; i < std::thread::hardware_concurrency();++i)
		createRefreshThread();
}

NetIoServerBase::~NetIoServerBase()
{
	//stop io thread
	mIoServer.stop();

	//wait
	for (auto& thread : mIoThreadPool)
		if (thread.joinable())
			thread.join();
}

void NetIoServerBase::createRefreshThread()
{
	mIoThreadPool.emplace_back(std::thread([&]()
	{
		while (!mIoServer.stopped())
			mIoServer.run_one();
	}));
}

std::unique_ptr<asio::ip::tcp::socket> NetIoServerBase::createSocket()
{
	return std::unique_ptr<asio::ip::tcp::socket>(new asio::ip::tcp::socket(mIoServer));
}

NetClient::NetClient(const std::string& address, const unsigned short port)
{
	mClientSocket = createSocket();
	mClientSocket->connect(asio::ip::tcp::endpoint(asio::ip::address::from_string(address), port));
}

void NetClient::sendPackage(const NetPackageBase& package) const
{
	const auto sentSize = mClientSocket->send(asio::buffer(package.toString()));
}

NetServer::NetServer(const std::string& address, const unsigned short port) :
	NetIoServerBase(),
	mAcceptor(asio::ip::tcp::acceptor(getIoServer(), asio::ip::tcp::endpoint(asio::ip::address::from_string(address), port)))
{
	asyncAccept();
	createRefreshThread();
}

void NetServer::asyncAccept()
{
	auto newNetPlayer = new NetPlayer(createSocket());

	mAcceptor.async_accept(*newNetPlayer->mSocket, [&](const asio::error_code& er)
	{
		if (er)
			return;

		mNetPlayers.emplace_back(newNetPlayer);
		mNetPlayers.back()->asyncReceive();

		asyncAccept();
	});
}


NetPlayer::NetPlayer(std::unique_ptr<asio::ip::tcp::socket>&& ioServer) :
	mSocket(std::move(ioServer)),
	mTmpPkSizeBuffer(sizeof(PkSize), 0)
{}

void NetPlayer::asyncReceive()
{
	if (mTmpPkBuffer.empty())
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

			asyncReceive();
		});
	}
	else
	{
		//read package
		asio::async_read(*mSocket, asio::buffer(mTmpPkBuffer), [&]
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

				if (shakehandPackage.version != NET_PACKAGE_VERSION)
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
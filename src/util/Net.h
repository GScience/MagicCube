#pragma once

#include <atomic>
#include <asio.hpp>
#include <thread>
#include "NetPackageBase.h"

/*!package some basic operation with io_service*/
class NetIoServerBase
{
	//!io server
	asio::io_service mIoServer;

	//!io refresh thread
	std::vector<std::thread> mIoThreadPool;

	//!is stopped
	std::atomic_bool mIsStopped;

protected:
	//!create an event refresh thread
	void createRefreshThread();

	//!create a socket
	std::unique_ptr<asio::ip::tcp::socket> createSocket();

	~NetIoServerBase();
	NetIoServerBase();

	//!get io server
	asio::io_service& getIoServer()
	{
		return mIoServer;
	}
};

class NetPlayer;

/*!a simple net server*/
class NetServer : NetIoServerBase
{
	friend class NetPlayer;

	//!server acceptor
	asio::ip::tcp::acceptor mAcceptor;

	//!save all net player
	std::vector<std::unique_ptr<NetPlayer>> mNetPlayers;
	std::function<void(NetPlayer&, NetPackageBase&&)> mReceiveCallback;

public:
	NetServer(const std::string& address, unsigned short port, decltype(mReceiveCallback) receiveCallback);
	void asyncAccept();
};

/*!a simple net player class*/
class NetPlayer
{
	friend class NetServer;

	enum LoginStage
	{
		LoginShakehand, LoginDone
	};

	//!player client socket
	std::unique_ptr<asio::ip::tcp::socket> mSocket;

	//!package size buffer
	std::vector<char> mTmpPkSizeBuffer;
	std::vector<char> mTmpPkBuffer;

	//!login stage
	LoginStage mLoginStage = LoginShakehand;

	//!server
	NetServer& mServer;

public:
	NetPlayer(NetServer& server, std::unique_ptr<asio::ip::tcp::socket>&& ioServer);

	void sendPackage(const NetPackageBase& package) const;
	void asyncReceive();
};

/*!a simple client*/
class NetClient : NetIoServerBase
{
	//!client socket
	std::unique_ptr<asio::ip::tcp::socket> mClientSocket = nullptr;
	std::vector<NetPackageBase> mReceivedPackage;

	//!package size buffer
	std::vector<char> mTmpPkSizeBuffer;
	std::vector<char> mTmpPkBuffer;

public:
	size_t getReceivedPackageCount() const
	{
		return mReceivedPackage.size();
	}

	NetPackageBase&& popPackage()
	{
		auto&& package = std::move(mReceivedPackage.front());
		mReceivedPackage.erase(mReceivedPackage.begin());

		return std::move(package);
	}

	NetClient(const std::string& address, unsigned short port);

	void sendPackage(const NetPackageBase& package) const;
	void asyncReceive();
};
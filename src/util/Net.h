#pragma once

#include <asio.hpp>
#include <thread>
#include <atomic>
#include "NetPackageBase.h"

/*!package some basic operation with io_service*/
class NetIoServerBase
{
	//!io server
	asio::io_service mIoServer;

	//!io refresh thread
	std::vector<std::thread> mIoThreadPool;

	//!is stopped
	std::atomic_bool mIsStopped = false;

protected:
	//!create an event refresh thread
	void createRefreshThread();

	//!create a socket
	std::unique_ptr<asio::ip::tcp::socket> createSocket();

	//!init
	~NetIoServerBase();

	//!get io server
	asio::io_service& getIoServer()
	{
		return mIoServer;
	}
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

public:
	explicit NetPlayer(std::unique_ptr<asio::ip::tcp::socket>&& ioServer);

	//!receive net package
	void asyncReceive();
};

/*!a simple net server*/
class NetServer : NetIoServerBase
{
	//!server acceptor
	asio::ip::tcp::acceptor mAcceptor;

	//!save all net player
	std::vector<std::unique_ptr<NetPlayer>> mNetPlayers;
	
public:
	NetServer(const std::string& address, unsigned short port);
	void asyncAccept();
};

/*!a simple client*/
class NetClient : NetIoServerBase
{
	//!client socket
	std::unique_ptr<asio::ip::tcp::socket> mClientSocket = nullptr;

public:
	NetClient(const std::string& address, unsigned short port);
	void sendPackage(const NetPackageBase& package) const;
};
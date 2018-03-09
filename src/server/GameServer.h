#pragma once

#include <asio.hpp>
#include <vector>
#include <queue>
#include <mutex>
#include "NetPackageBase.h"

class NetPlayer
{
	friend class GameServer;

	enum LoginStage
	{
		LoginShakehand, LoginDone
	};

	//!player client socket
	asio::ip::tcp::socket mSocket;

	//!package size buffer
	std::vector<char> mTmpPkSizeBuffer;
	std::vector<char> mTmpPkBuffer;

	//!login stage
	LoginStage mLoginStage = LoginShakehand;

public:
	explicit NetPlayer(asio::io_service& ioServer);

	//!receive net package
	void asyncReceive();
};

class GameServer
{
	//!save all socket
	std::vector<NetPlayer> mNetPlayerList;

	//!server io
	asio::io_service mIoServer;
	//!server acceptor
	asio::ip::tcp::acceptor mAcceptor;

	//!new client
	NetPlayer* mNewNetPlayer = nullptr;

public:
	explicit GameServer(const char* address, const unsigned short port) :
		mAcceptor(asio::ip::tcp::acceptor(mIoServer, asio::ip::tcp::endpoint(asio::ip::address::from_string(address), port)))
	{}

	~GameServer();

	void start(bool async = false);
	void asyncAccept();
};
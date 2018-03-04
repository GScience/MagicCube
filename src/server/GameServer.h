#pragma once

#include <asio.hpp>
#include <deque>

class GameServer
{
	std::vector<asio::ip::tcp::socket> mNetPlayerList;
	asio::io_service mIoServer;
	asio::ip::tcp::acceptor mAcceptor;

public:
	explicit GameServer(const char* address, const unsigned short port) :
	mAcceptor(asio::ip::tcp::acceptor(mIoServer, asio::ip::tcp::endpoint(asio::ip::address::from_string(address), port)))
	{
		
	}

	void start();
	void asyncAccept();
};
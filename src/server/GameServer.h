#pragma once

#include <asio.hpp>
#include <deque>

class GameServer
{
	std::vector<asio::ip::tcp::socket> mNetPlayerList;
	asio::io_service mIoServer;
	asio::ip::tcp::acceptor mAcceptor;

public:
	explicit GameServer(const unsigned short port) :mAcceptor(asio::ip::tcp::acceptor(mIoServer, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port))) {}

	void start();
	void asyncAccept();
};
#pragma once

#include <asio.hpp>
#include <thread>
#include "NetPackageBase.h"

class NetIoServerBase
{
	//!io server
	asio::io_service mIoServer;

	//!io refresh thread
	std::thread mIoThread;

protected:
	//!create a socket
	std::unique_ptr<asio::ip::tcp::socket> createSocket();

	//!init
	NetIoServerBase();
	~NetIoServerBase();

	//!is stopped
	std::atomic_bool mShouldStopRefresh = false;
};

class NetServer
{
	
};

/*class NetPlayer
{
	
};*/

class NetClient : NetIoServerBase
{
	//!client socket
	std::unique_ptr<asio::ip::tcp::socket> mClientSocket = nullptr;

public:
	NetClient(const std::string& address, unsigned short port);
	void sendPackage(const NetPackageBase& package) const;
};
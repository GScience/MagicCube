#include "Net.h"

NetIoServerBase::NetIoServerBase()
{
	mIoThread = std::thread([&]()
	{
		while (!mShouldStopRefresh)
			mIoServer.run_one();
	});
}

NetIoServerBase::~NetIoServerBase()
{
	mShouldStopRefresh = true;

	mIoThread.join();
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
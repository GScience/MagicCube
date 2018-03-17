#pragma once

#include <cstring>
#include <sstream>
#include <memory>

#define CLIENT_COMMAND_NULL 0
#define SERVER_COMMAND_NULL 0

#define NET_PACKAGE_VERSION 1

using PkSize = uint32_t;
using PkListSize = uint16_t;
using PkId = uint16_t;

class NetPackageBase
{
public:
	PkId packageId;

	virtual PkSize getPackageSize() const { return sizeof(PkId); }
	virtual void fromStringStream(std::stringstream& stream)
	{
		packageId = fromBinaryStream<PkId>(stream);
	}
	virtual std::string toBinaryString() const
	{
		const auto size = getPackageSize();

		auto totalBinaryStr =
			//package size
			toBinary(&size) +
			//package id(for callback)
			toBinary(&packageId);

		return totalBinaryStr;
	}

	virtual ~NetPackageBase() = default;

	static std::string toBinary(const void* data, const size_t size)
	{
		std::string buffer(size, 0);

		memcpy(&buffer[0], data, size);
		return buffer;
	}

	template<class t> static std::string toBinary(t* data)
	{
		return toBinary(data, sizeof(t));
	}
	static std::string toBinary(const std::string& data)
	{
		const auto size = static_cast<PkListSize>(data.size());
		const auto sizeBinary = toBinary<>(&size);

		return sizeBinary + toBinary(data.c_str(), size);
	}
	template<class t> static std::string toBinaryFromVector(const std::vector<t>& data)
	{
		const auto size = static_cast<PkListSize>(data.size()) * getSize<t>();
		const auto sizeBinary = toBinary<>(&size);

		return sizeBinary + toBinary(&data[0], size);
	}

	static std::string fromBinaryStream(std::stringstream& stream)
	{
		const auto size = fromBinaryStream<PkListSize>(stream);
		std::string str(size, 0);
		stream.read(const_cast<char*>(&str.c_str()[0]), size);

		return str;
	}

	template<class t> static std::vector<t> fromBinaryStreamToVector(std::stringstream& stream)
	{
		const auto size = fromBinaryStream<PkListSize>(stream);
		std::vector<t> data(size / getSize<t>(), 0);
		stream.read(reinterpret_cast<char*>(&data[0]), size);

		return data;
	}

	template<class t> static t fromBinaryStream(std::stringstream& stream)
	{
		char sizeBuffer[sizeof(t)];
		stream.read(sizeBuffer, sizeof(t));

		return *reinterpret_cast<t*>(sizeBuffer);
	}

	template<class t> static size_t getSize() { return sizeof(t); }
	template<class t> static size_t getSize(const t& obj) { return getSize<t>(); }
	
	template<class t> static size_t getVectorSize(const std::vector<t>& obj)
	{
		return sizeof(PkListSize) + obj.size() * getSize<t>();
	}

	static size_t getSize(const std::string& str)
	{
		return sizeof(PkListSize) + str.size();
	}
};

//!send from client to server
class NetPackageClientCommand :public NetPackageBase
{
	std::stringstream mStream;

public:
	uint16_t clientCommand = CLIENT_COMMAND_NULL;
	std::shared_ptr<NetPackageBase> netPackage = nullptr;

	template <class packageType> std::shared_ptr<packageType> getPackage()
	{
		if (netPackage)
			return std::static_pointer_cast<packageType>(netPackage);
		auto package = std::make_shared<packageType>();
		package->fromStringStream(mStream);
		netPackage = package;
		return package;
	}

	void fromStringStream(std::stringstream& packageData) override
	{
		NetPackageBase::fromStringStream(packageData);
		clientCommand = fromBinaryStream<uint16_t>(packageData);

		packageData.swap(mStream);
	}

	std::string toBinaryString() const override
	{
		const auto data = NetPackageBase::toBinaryString() + toBinary(&clientCommand);

		if (clientCommand == CLIENT_COMMAND_NULL)
			return data;

		return data + netPackage->toBinaryString();
	}

	PkSize getPackageSize() const override
	{
		const auto size = NetPackageBase::getPackageSize() + getSize(clientCommand);

		if (clientCommand == CLIENT_COMMAND_NULL)
			return size;

		return size + netPackage->getPackageSize();
	}
};

//!send from server to client
class NetPackageServerCommand :public NetPackageBase
{
	std::stringstream mStream;

public:
	uint16_t serverCommand = SERVER_COMMAND_NULL;
	std::shared_ptr<NetPackageBase> netPackage = nullptr;

	template <class packageType> std::shared_ptr<packageType> getPackage()
	{
		if (netPackage)
			return std::static_pointer_cast<packageType>(netPackage);
		auto package = std::make_shared<packageType>();
		package->fromStringStream(mStream);
		netPackage = package;
		return package;
	}

	void fromStringStream(std::stringstream& packageData) override
	{
		NetPackageBase::fromStringStream(packageData);
		serverCommand = fromBinaryStream<uint16_t>(packageData);

		packageData.swap(mStream);
	}

	std::string toBinaryString() const override
	{
		return NetPackageBase::toBinaryString() +
			netPackage->toBinaryString();
	}

	PkSize getPackageSize() const override
	{
		return NetPackageBase::getPackageSize() +
			getSize(serverCommand) +
			netPackage->getPackageSize();
	}
};

class NetPackageShakehand : public NetPackageBase
{
public:
	int version = NET_PACKAGE_VERSION;
	std::string playerName;

	void fromStringStream(std::stringstream& packageData) override
	{
		NetPackageBase::fromStringStream(packageData);
		version = fromBinaryStream<decltype(version)>(packageData);
		playerName = fromBinaryStream(packageData);
	}

	std::string toBinaryString() const override
	{
		return NetPackageBase::toBinaryString() +
			toBinary(&version) +
			toBinary(playerName);
	}

	PkSize getPackageSize() const override
	{
		return NetPackageBase::getPackageSize() +
			getSize(version) +
			getSize(playerName);
	}
};
#pragma once

#include "NetPackageBase.h"
#include <memory>

#define SERVER_COMMAND_NULL 0
#define SERVER_COMMAND_LOAD_CHUNK 1

//!send from server to client
class NetPackageServerCommand :public NetPackageBase
{ 
public:
	uint16_t serverCommand = SERVER_COMMAND_NULL;
	std::shared_ptr<NetPackageBase> netPackage;

	void fromStringStream(std::stringstream& packageData) override
	{
		NetPackageBase::fromStringStream(packageData);
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

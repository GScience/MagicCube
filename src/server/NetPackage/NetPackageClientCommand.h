#pragma once

#include "NetPackageBase.h"
#include <memory>

#define CLIENT_COMMAND_NULL 0
#define CLIENT_COMMAND_LOAD_CHUNK 1

//!send from client to server
class NetPackageClientCommand :public NetPackageBase
{
public:
	uint16_t clientCommand = CLIENT_COMMAND_NULL;
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
			getSize(clientCommand) +
			netPackage->getPackageSize();
	}
};

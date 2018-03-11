#pragma once

#include "NetPackageBase.h"
#define NET_PACKAGE_VERSION 1

class NetPackageShakehand : public NetPackageBase
{
public:
	int version = 0;
	std::string playerName;

	explicit NetPackageShakehand()
	{
		version = NET_PACKAGE_VERSION;
	}

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
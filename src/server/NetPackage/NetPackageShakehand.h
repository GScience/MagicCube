#pragma once

#include "NetPackageBase.h"
#define NET_PACKAGE_VERSION 1

class NetPackageShakehand : public NetPackageBase
{
public:
	int version = 0;

	explicit NetPackageShakehand()
	{
		mSize = sizeof(NetPackageShakehand);
		version = NET_PACKAGE_VERSION;
	}

	void fromStringStream(std::stringstream& packageData) override
	{
		NetPackageBase::fromStringStream(packageData);
		version = *fromBinaryStream<decltype(version)>(packageData);
	}

	std::string toString() const override
	{
		const auto data = toBinary(&version);

		return NetPackageBase::toString() + data;
	}

	PkSize getPackageSize() const override
	{
		return NetPackageBase::getPackageSize() + sizeof(int);
	}
};
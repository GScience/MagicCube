#pragma once

#include "NetPackageBase.h"

class NetPackageShakehand : NetPackageBase
{
public:
	const int version = 1;

	NetPackageShakehand()
	{
		mSize = sizeof(NetPackageShakehand);
	}

	void fromStringStream(std::stringstream& packageData) override
	{
		NetPackageBase::fromStringStream(packageData);
	}

	std::string toString() override
	{
		mSize = sizeof(NetPackageShakehand);

		return NetPackageBase::toString() + toBinary(&version);
	}
};
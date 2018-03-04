#pragma once

#include "NetPackageBase.h"

class NetPackageShakehand : NetPackageBase
{
public:
	int version = 0;

	explicit NetPackageShakehand(const int version = 0)
	{
		mSize = sizeof(NetPackageShakehand);
		this->version = version;
	}

	void fromStringStream(std::stringstream& packageData) override
	{
		NetPackageBase::fromStringStream(packageData);
		version = *fromBinaryStream<decltype(version)>(packageData);
	}

	std::string toString() override
	{
		auto data = toBinary(&version);

		mSize = data.size() * sizeof(char);

		return NetPackageBase::toString() + data;
	}
};
#pragma once

#include "NetPackageBase.h"

class NetPackageChunk :public NetPackageBase
{
public:
	void fromStringStream(std::stringstream& packageData) override
	{
		NetPackageBase::fromStringStream(packageData);
	}
};

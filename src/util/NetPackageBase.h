#pragma once

#include <sstream>

using PkSize = int32_t;

class NetPackageBase
{
protected:
	PkSize mSize = -1;

public:
	virtual PkSize getPackageSize() { return 0; }
	virtual void fromStringStream(std::stringstream& data)
	{
		char sizeBuffer[sizeof(PkSize)];
		data.read(sizeBuffer, sizeof(PkSize));
	}
	virtual std::string toString()
	{
		char sizeBuffer[sizeof(PkSize) + sizeof(char)]{ 0 };
		memcpy(sizeBuffer, &mSize, sizeof(PkSize));
		return sizeBuffer;
	}

	virtual ~NetPackageBase() = default;
};
#pragma once

#include <cstring>
#include <sstream>

using PkSize = uint32_t;

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
		return toBinary(&mSize);
	}

	virtual ~NetPackageBase() = default;

	static std::string toBinary(const void* data, const size_t size)
	{
		const auto buffer = new char[size + sizeof(char)]{ 0 };
		memcpy(buffer, data, size);
		return buffer;
	}

	template<class t> static std::string toBinary(t* data)
	{
		return toBinary(data, sizeof(t));
	}
};
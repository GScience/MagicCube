#pragma once

#include <cstring>
#include <sstream>

using PkSize = uint32_t;

class NetPackageBase
{
protected:
	PkSize mSize = -1;

public:
	virtual PkSize getPackageSize() const { return 0; }
	virtual void fromStringStream(std::stringstream& stream)
	{
		mSize = *fromBinaryStream<PkSize>(stream);
	}
	virtual std::string toString() const
	{
		const auto size = getPackageSize();

		return toBinary(&size);
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

	template<class t> static t* fromBinary(void* data)
	{
		return reinterpret_cast<t*>(data);
	}

	template<class t> static t* fromBinaryStream(std::stringstream& stream)
	{
		char sizeBuffer[sizeof(t)];
		stream.read(sizeBuffer, sizeof(t));

		return fromBinary<t>(sizeBuffer);
	}
};
#pragma once

#include <cstring>
#include <sstream>

using PkSize = uint32_t;
using PkStrSize = uint16_t;
using PkId = uint16_t;

class NetPackageBase
{
public:
	PkId packageId;

	virtual PkSize getPackageSize() const { return sizeof(PkId); }
	virtual void fromStringStream(std::stringstream& stream)
	{
		packageId = fromBinaryStream<PkId>(stream);
	}
	virtual std::string toBinaryString() const
	{
		const auto size = getPackageSize();

		auto totalBinaryStr =
			//package size
			toBinary(&size) +
			//package id(for callback)
			toBinary(&packageId);

		return totalBinaryStr;
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
	static std::string toBinary(const std::string& data)
	{
		const auto size = static_cast<PkStrSize>(data.size());
		const auto sizeBinary = toBinary<>(&size);

		return sizeBinary + toBinary(data.c_str(), size);
	}

	static std::string fromBinaryStream(std::stringstream& stream)
	{
		const auto size = fromBinaryStream<PkStrSize>(stream);
		std::string str(size, 0);
		stream.read(const_cast<char*>(&str.c_str()[0]), size);

		return str;
	}

	template<class t> static t fromBinaryStream(std::stringstream& stream)
	{
		char sizeBuffer[sizeof(t)];
		stream.read(sizeBuffer, sizeof(t));

		return *reinterpret_cast<t*>(sizeBuffer);
	}

	template<class t> static size_t getSize(const t& obj)
	{
		return sizeof(t);
	}

	static size_t getSize(const std::string& str)
	{
		return sizeof(PkStrSize) + str.size();
	}
};
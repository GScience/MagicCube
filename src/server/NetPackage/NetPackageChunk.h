#pragma once

#include "NetPackage.h"

#define SERVER_NET_PACKAGE_CHUNK 1

class NetPackageChunk :public NetPackageBase
{
public:
	int32_t chunkX = 0, chunkY = 0, chunkZ = 0;

	std::vector<uint16_t> blockIdList;

	PkSize getPackageSize() const override
	{
		return NetPackageBase::getPackageSize() +
			getSize(chunkX) +
			getSize(chunkY) +
			getSize(chunkZ) +
			getVectorSize(blockIdList);
	}

	std::string toBinaryString() const override
	{
		return NetPackageBase::toBinaryString() + toBinary(&chunkX) + toBinary(&chunkY) + toBinary(&chunkZ) +
			toBinaryFromVector(blockIdList);
	}

	void fromStringStream(std::stringstream& packageData) override
	{
		NetPackageBase::fromStringStream(packageData);
		auto pkSize = fromBinaryStream<PkSize>(packageData);

		chunkX = fromBinaryStream<int32_t>(packageData);
		chunkY = fromBinaryStream<int32_t>(packageData);
		chunkZ = fromBinaryStream<int32_t>(packageData);

		blockIdList = fromBinaryStreamToVector<decltype(blockIdList)::value_type>(packageData);
	}
};

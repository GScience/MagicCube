#pragma once

#include <vector>
#include <memory>
#include "IBlock.h"

#define MAKE_LIGHT_COLOR(r,g,b) (static_cast<uint8_t>(r) + (static_cast<uint8_t>(g) << 4) + (static_cast<uint8_t>(b) << 8))
#define GET_LIGHT_COLOR_R(color) (static_cast<uint16_t>(static_cast<uint16_t>(color) << 12) >> 12)
#define GET_LIGHT_COLOR_G(color) (static_cast<uint16_t>(static_cast<uint16_t>(color) << 8) >> 12)
#define GET_LIGHT_COLOR_B(color) (static_cast<uint16_t>(static_cast<uint16_t>(color) << 4) >> 12)

class ClientBlock
{
public:
	ClientBlock() :
		blockId(0),
		blockDataIndex(0),
		blockLight(0),
		hasBlockDataInChunk(false)
	{}
	void clear()
	{
		blockId = 0;
		blockLight = 0;
		hasBlockDataInChunk = false;
	}
	uint16_t blockId : 12;
	uint16_t blockDataIndex : 12;
	uint16_t blockLight : 12;

	//!whether has a block data
	bool hasBlockDataInChunk;
};

class ClientChunk
{
	friend class ClientChunkGroup;

	ClientChunk() = default;
	//!save all block
	ClientBlock mBlockList[4096];
	//!save block data
	std::vector<std::shared_ptr<BlockData>> mBlockData;

public:
	ClientBlock & getBlock(uint8_t x, uint8_t y, uint8_t z);
	std::shared_ptr<BlockData> getBlockData(uint8_t x, uint8_t y, uint8_t z);
	template <class blockDataType, class ...t> void setBlockData(const uint8_t x, const uint8_t y, const uint8_t z, const t&&... args)
	{
		auto& block = getBlock(x, y, z);
		auto data = std::make_shared<blockDataType>(static_cast<uint16_t>(block.blockId), args...);

		if (block.hasBlockDataInChunk)
			mBlockData[block.blockDataIndex] = data;
		else
		{
			block.hasBlockDataInChunk = true;
			block.blockDataIndex = static_cast<uint16_t>(mBlockData.size());
			mBlockData.emplace_back(std::move(data));
		}

	}

	//!clean chunk
	void clear();
};

class ClientChunkGroup
{
	//!all chunk is in the list
	std::vector<std::shared_ptr<ClientChunk>> mChunkPool;

public:
	//!init chunks
	void init(size_t poolSize = 16384);

	//!get all chunks
	decltype(mChunkPool)& getChunksPool() { return mChunkPool; }
};
#pragma once

#include <vector>
#include <memory>
#include <tuple>
#include "IBlock.h"
#include <map>
#include <atomic>

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
	friend class GameClient;

	//!save all block
	ClientBlock mBlocks[4096];
	//!save block data
	std::vector<std::shared_ptr<BlockData>> mBlockData;
	
	//!chunk location
	int32_t mChunkX = 0;
	int8_t mChunkY = 0;
	int32_t mChunkZ = 0;

	//!load chunk with location
	bool load(int32_t chunkX, int32_t chunkY, int32_t chunkZ);
	void unload();
	
	//!is valid
	std::atomic_bool mIsValid;

public:
	//is a valid chunk
	bool isValid() const
	{
		return mIsValid;
	}

	int32_t getChunkX() const { return mChunkX; }
	int8_t getChunkY() const { return mChunkY; }
	int32_t getChunkZ() const { return mChunkZ; }

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

using ClientChunkPtr = std::shared_ptr<ClientChunk>;

class ClientChunkGroup
{
	//!all chunk is in the list
	std::vector<ClientChunkPtr> mFreeChunkPool;

	/*!chunk map
	 * all chunk in chunk map is valid
	 */
	std::map<std::tuple<int32_t, int32_t, int32_t>, ClientChunkPtr> mChunkMap;

public:
	//!init chunks
	void init(size_t poolSize = 16384);

	//!get all chunks
	std::shared_ptr<ClientChunk> getChunk(int32_t chunkX, int32_t chunkY, int32_t chunkZ) const;

	/*!load a chunk
	 * @return whether successfully load the chunk
	 */
	bool loadChunk(int32_t chunkX, int32_t chunkY, int32_t chunkZ);

	//!unload a chunk
	void unloadChunk(int32_t chunkX, int32_t chunkY, int32_t chunkZ);
};
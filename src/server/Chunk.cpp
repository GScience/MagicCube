#include "Chunk.h"
#include "Task.h"

void ChunkGroup::init(const size_t poolSize)
{
	if (mFreeChunkPool.size() != poolSize)
		mFreeChunkPool.resize(poolSize);

	//clean chunk
	for (auto& chunk : mFreeChunkPool)
	{
		chunk = std::make_shared<Chunk>();
		chunk->clear();
	}
}

void Chunk::clear()
{
	for (auto& block : mBlocks)
		block.clear();

	mBlockData.clear();
}

ChunkBlock& Chunk::getBlock(const uint8_t x, const uint8_t y, const uint8_t z)
{
	return mBlocks[x + y * 16 + z * 256];
}

std::shared_ptr<BlockData> Chunk::getBlockData(const uint8_t x, const uint8_t y, const uint8_t z)
{
	const auto& block = getBlock(x, y, z);

	if (block.hasBlockDataInChunk && mBlockData[block.blockDataIndex]->getBlockId() == block.blockId)
		return mBlockData[block.blockDataIndex];
	return nullptr;
}

void Chunk::unload()
{
	mBlockData.clear();
}

ChunkPtr ChunkGroup::getChunk(const int32_t chunkX, const int32_t chunkY, const int32_t chunkZ) const
{
	//find in chunk map
	const auto result = mChunkMap.find(std::make_tuple(chunkX, chunkY, chunkZ));
	
	if (result == mChunkMap.end())
		return nullptr;

	return result->second;
}

bool ChunkGroup::addChunk(const int32_t chunkX, const int32_t chunkY, const int32_t chunkZ)
{
	//is chunk pool empty?
	if (mFreeChunkPool.empty())
		mFreeChunkPool.resize(1024);

	//has load?
	if (mChunkMap.find(std::make_tuple(chunkX, chunkY, chunkZ)) != mChunkMap.end())
		return false;

	//get an empty chunk from chunk pool
	const auto newChunk = mFreeChunkPool.back();
	mFreeChunkPool.pop_back();

	//load a new chunk
	newChunk->mChunkX = chunkX;
	newChunk->mChunkY = chunkY;
	newChunk->mChunkZ = chunkZ;

	//add to chunk map
	mChunkMap[std::make_tuple(chunkX, chunkY, chunkZ)] = newChunk;

	return true;
}

void ChunkGroup::removeChunk(int32_t chunkX, int32_t chunkY, int32_t chunkZ)
{
	const auto result = mChunkMap.find(std::make_tuple(chunkX, chunkY, chunkZ));

	if (result == mChunkMap.end())
		return;

	//remove
	result->second->unload();
	mFreeChunkPool.push_back(result->second);
	mChunkMap.erase(result);
}
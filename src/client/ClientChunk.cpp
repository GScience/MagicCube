#include "ClientChunk.h"

void ClientChunkGroup::init(const size_t poolSize)
{
	if (mChunkPool.size() != poolSize)
		mChunkPool.resize(poolSize);

	//clean chunk
	for (auto& chunk : mChunkPool)
	{
		chunk = std::shared_ptr<ClientChunk>(new ClientChunk());
		chunk->clear();
	}
}

void ClientChunk::clear()
{
	for (auto& blockData : mBlockList)
		blockData.clear();

	setBlockData<BlockData>(10, 10, 10);
}

ClientBlock& ClientChunk::getBlock(const uint8_t x, const uint8_t y, const uint8_t z)
{
	return mBlockList[x + y * 16 + z * 256];
}

std::shared_ptr<BlockData> ClientChunk::getBlockData(const uint8_t x, const uint8_t y, const uint8_t z)
{
	const auto& block = getBlock(x, y, z);

	if (block.hasBlockDataInChunk)
		return mBlockData[block.blockDataIndex];
	return nullptr;
}
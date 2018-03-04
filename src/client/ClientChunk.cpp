#include "ClientChunk.h"
#include "GameClient.h"
#include "Task.h"

void ClientChunkGroup::init(const size_t poolSize)
{
	if (mFreeChunkPool.size() != poolSize)
		mFreeChunkPool.resize(poolSize);

	//clean chunk
	for (auto& chunk : mFreeChunkPool)
	{
		chunk = std::shared_ptr<ClientChunk>(new ClientChunk());
		chunk->clear();
	}
}

void ClientChunk::clear()
{
	for (auto& blockData : mBlocks)
		blockData.clear();

	setBlockData<BlockData>(10, 10, 10);
}

ClientBlock& ClientChunk::getBlock(const uint8_t x, const uint8_t y, const uint8_t z)
{
	return mBlocks[x + y * 16 + z * 256];
}

std::shared_ptr<BlockData> ClientChunk::getBlockData(const uint8_t x, const uint8_t y, const uint8_t z)
{
	const auto& block = getBlock(x, y, z);

	if (block.hasBlockDataInChunk)
		return mBlockData[block.blockDataIndex];
	return nullptr;
}

bool ClientChunk::load(const int32_t chunkX, const int32_t chunkY, const int32_t chunkZ)
{
	unload();

	mChunkX = chunkX;
	mChunkY = chunkY;
	mChunkZ = chunkZ;


	GameClient::getInstance().addLoadChunkTask(Task([&]
	()
	{
		return GameClient::getInstance().downloadChunkData(chunkX, chunkY, chunkZ);
	}), [&]
	(const Task& task)
	{
		auto result = task.get();
	});

	return true;
}

void ClientChunk::unload()
{
	mIsValid = false;
	mBlockData.clear();
}

ClientChunkPtr ClientChunkGroup::getChunk(const int32_t chunkX, const int32_t chunkY, const int32_t chunkZ) const
{
	//find in chunk map
	const auto result = mChunkMap.find(std::make_tuple(chunkX, chunkY, chunkZ));
	
	if (result == mChunkMap.end())
		return nullptr;

	return result->second;
}

bool ClientChunkGroup::loadChunk(const int32_t chunkX, const int32_t chunkY, const int32_t chunkZ)
{
	//is chunk pool empty?
	if (mFreeChunkPool.empty())
		return false;

	//has load?
	if (mChunkMap.find(std::make_tuple(chunkX, chunkY, chunkZ)) != mChunkMap.end())
		return false;

	//get an empty chunk from chunk pool
	auto newChunk = mFreeChunkPool.back();
	mFreeChunkPool.pop_back();

	//load a new chunk
	if (!newChunk->load(chunkX, chunkY, chunkZ))
		return false;

	//add to chunk map
	mChunkMap[std::make_tuple(chunkX, chunkY, chunkZ)] = newChunk;

	return true;
}

void ClientChunkGroup::unloadChunk(int32_t chunkX, int32_t chunkY, int32_t chunkZ)
{
	const auto result = mChunkMap.find(std::make_tuple(chunkX, chunkY, chunkZ));

	if (result == mChunkMap.end())
		return;

	//remove
	result->second->unload();
	mFreeChunkPool.push_back(result->second);
	mChunkMap.erase(result);
}
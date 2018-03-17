#pragma once

#include <vector>
#include <unordered_map>
#include <memory>
#include <iostream>
#include "IBlock.h"

//!save all block
class BlockList
{
	//!save all block
	std::vector<std::unique_ptr<IBlock>> mBlockList;
	std::unordered_map<size_t, size_t> mBlockMap;

	BlockList() = default;

public:
	//!register a new block
	template<class t> void registerBlock()
	{
		std::unique_ptr<IBlock> blockPtr(new t());
		const auto blockId = blockPtr->getId();

		if (blockId >= mBlockList.size())
			mBlockList.resize(blockId + 1);

		mBlockList[blockId] = std::move(blockPtr);
		mBlockMap[typeid(t).hash_code()] = blockId;

		std::cout << "[BlockList] Register a new block id = " << blockId << " with hash " << typeid(t).hash_code() << std::endl;
	}

	//!init all block
	void init();

	/*!get block info by id
	 * @param blockId Block ID
	 */
	IBlock& getBlock(const uint16_t blockId) { return *mBlockList[blockId].get(); }

	//!get block info by type
	template <class blockType> IBlock& getBlock()
	{
		return getBlock(mBlockMap[typeid(blockType).hash_code()]);
	}

	//!get block list
	static BlockList& getInstance()
	{
		static BlockList blockList;
		return blockList;
	}
};

#define BlockList BlockList::getInstance()
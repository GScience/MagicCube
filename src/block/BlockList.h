#pragma once

#include <vector>
#include <memory>
#include <iostream>
#include "IBlock.h"

//!save all block
class BlockList
{
	//!save all block
	std::vector<std::unique_ptr<IBlock>> mBlockList;

	BlockList() = default;

public:
	//!register a new block
	template<class t> void registerBlock()
	{
		std::unique_ptr<IBlock> blockPtr(new t());
		const auto blockId = blockPtr->getId();

		if (blockPtr->getId() >= mBlockList.size())
			mBlockList.resize(blockPtr->getId() + 1);

		mBlockList[blockPtr->getId()] = std::move(blockPtr);

		std::cout << "[BlockList] Register a new block id = " << blockId << " with name " << typeid(t).name() << std::endl;
	}

	//!init all block
	void init();

	/*!get a block
	 * @param blockId Block ID
	 */
	IBlock* getBlock(const uint16_t blockId) { return mBlockList[blockId].get(); }


	//!get block list
	static BlockList& getInstance()
	{
		static BlockList blockList;
		return blockList;
	}
};

#define BlockList BlockList::getInstance()
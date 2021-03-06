#pragma once

#include "IBlockCube.h"

class BlockGrass :public IBlockCube
{
public:
	bool hasData() override { return false; }
	uint16_t getId() override { return 0; }
	uint32_t getBlockTextureLoc(BlockSurface surface) override { return 0; }
};

#ifndef FOR_REGISTER
#define BlockGrass BlockList.getBlock<BlockGrass>()
#endif
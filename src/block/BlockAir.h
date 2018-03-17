#pragma once

#include "IBlock.h"

class BlockAir :public IBlock
{
public:
	uint16_t getId() override { return 0; }
	bool hasData() override { return false; }
	bool canRender() override { return false; }
	std::vector<float> createRenderData(BlockSurface surface, std::shared_ptr<BlockData> data) override { return {}; }
	bool checkCollision(double x, double y, double z) override { return false; }
};

#ifndef FOR_REGISTER
#define BlockAir BlockList.getBlock<BlockAir>()
#endif
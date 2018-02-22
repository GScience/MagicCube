#pragma once

#include "IBlock.h"

class IBlockCube :public IBlock
{
public:
	/*!get texture id
	 * @param surface Texture used on which surface
	 */
	virtual uint32_t getBlockTextureLoc(BlockSurface surface) = 0;
	bool canRender() override { return true; }
	std::vector<float> createRenderData(BlockSurface surface, std::shared_ptr<BlockData> data) override { return {}; }

	bool checkCollision(double x, double y, double z) override { return true; }
};
#pragma once

#include <cstdint>
#include <memory>
#include <vector>

enum BlockSurface
{
	SurfaceUp, SurfaceDown, SurfaceLeft, SurfaceRight, SurfaceBack, SurfaceFront
};

class BlockData
{
public:
	//!block id
	const uint16_t blockId;

	//create a block data
	explicit BlockData(const uint16_t blockId) : blockId(blockId) { }
};

class IBlock
{
	friend class Blocklist;
	
protected:
	IBlock() = default;

public:
	virtual ~IBlock() = default;

	//!get block id
	virtual uint16_t getId() = 0;
	//!has data structure
	virtual bool hasData() = 0;
	//!whether can be used to draw
	virtual bool canRender() = 0;
	/*!create render vertex array
	* @param surface Render data used on which surface
	* @param data Block data
	*/
	virtual std::vector<float> createRenderData(BlockSurface surface, std::shared_ptr<BlockData> data) = 0;
	/*!chech whether point is in the block
	* @param x Location x in block
	* @param y Location y in block
	* @param z Location z in block
	*/
	virtual bool checkCollision(double x, double y, double z) = 0;
};
#include "BlockList.h"
#include "BlockGrass.h"
#include "BlockAir.h"

#undef BlockList

void BlockList::init()
{
	registerBlock<BlockGrass>();
	registerBlock<BlockAir>();
}

#include <gtest/gtest.h>
#include "BlockList.h"
#include "GameClient.h"

TEST(ClientTest, ChunkTest)
{
	auto& client = GameClient::getInstance();

	BlockList.init();
	client.start();
	
	GTEST_SUCCESS_("Succeesfully init block list and start local client");

	auto& testChunk = client.getClientChunkGroup().getChunksPool()[0];

	auto& testBlock1 = testChunk->getBlock(12, 12, 12);
	testBlock1.blockId = 4090;
	testBlock1.blockDataIndex = 4089;
	testBlock1.blockLight = MAKE_LIGHT_COLOR(7, 5, 15);

	ASSERT_EQ(testBlock1.blockId, 4090);
	ASSERT_EQ(testBlock1.blockDataIndex, 4089);
	ASSERT_EQ(GET_LIGHT_COLOR_R(testBlock1.blockLight), 7);
	ASSERT_EQ(GET_LIGHT_COLOR_G(testBlock1.blockLight), 5);
	ASSERT_EQ(GET_LIGHT_COLOR_B(testBlock1.blockLight), 15);
	ASSERT_EQ(sizeof(ClientBlock), 8);
}
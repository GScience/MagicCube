
#include <gtest/gtest.h>
#include "BlockList.h"
#include "GameClient.h"
#include <chrono>

volatile TEST(ClientTest, ChunkTest)
{
	auto& client = GameClient::getInstance();

	BlockList.init();
	client.connect(LocalServer);
	
	GTEST_SUCCESS_("Succeesfully init block list and start local client");
	GTEST_MESSAGE_("Test ChunkGroup", testing::TestPartResult::kSuccess);

	for (auto i = 0; i < 30; i++)
		for (auto j = 0; j < 30; j++)
			for (auto k = 0; k < 16; k++)
				client.getClientChunkGroup().loadChunk(i, j, k);

	for (auto i = 0; i < 30; i++)
		for (auto j = 0; j < 30; j++)
			for (auto k = 0; k < 16; k++)
				ASSERT_NE(client.getClientChunkGroup().getChunk(i, j, k), nullptr);

	auto testChunk = client.getClientChunkGroup().getChunk(10, 10, 10);

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

volatile TEST(ClientTest, FindChunkSpeedTest)
{
	auto& client = GameClient::getInstance();

	for (auto loopTime = 0; loopTime != 1000; ++loopTime)
	{
		for (auto i = 0; i < 30; i++)
			for (auto j = 0; j < 30; j++)
				for (auto k = 0; k < 16; k++)
					ASSERT_NE(client.getClientChunkGroup().getChunk(i, j, k), nullptr);
	}
}
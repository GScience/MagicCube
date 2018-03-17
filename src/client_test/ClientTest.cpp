
#include <gtest/gtest.h>
#include "BlockList.h"
#include "GameClient.h"

TEST(ClientTest, ChunkTest)
{
	auto& client = GameClient::getInstance();

	BlockList.init();
	client.connect(LocalServer);
	
	GTEST_SUCCESS_("Succeesfully init block list and start local client");
	GTEST_MESSAGE_("Test ChunkGroup", testing::TestPartResult::kSuccess);

	while (client.getLocalServer()->getPlayers().empty());

	for (auto i = 0; i < 4; i++)
		for (auto j = 0; j < 4; j++)
			for (auto k = 0; k < 2; k++)
				client.getLocalServer()->sendChunkToPlayer(*client.getLocalServer()->getPlayers()[0], i, j, k);

	for (auto i = 0; i < 4; i++)
		for (auto j = 0; j < 4; j++)
			for (auto k = 0; k < 2; k++)
				ASSERT_NE(client.getChunk(i, j, k), nullptr);

	auto testChunk = client.getChunk(2, 3, 1);

	auto& testBlock1 = testChunk->getBlock(12, 12, 12);

	for (auto i = 0; i < 10; i++)
	{
		const auto testLightR = rand() % 16;
		const auto testLightG = rand() % 16;
		const auto testLightB = rand() % 16;

		const auto testLight = MAKE_LIGHT_COLOR(testLightR, testLightG, testLightB);

		const auto testBlockId = rand() % 4096;
		const auto testBlockDataIndex = rand() % 4096;

		std::cout	<< "test block data with light: " << testLight 
					<< "\tBlock id: " << testBlockId 
					<< "\tBlock data index: " << testBlockDataIndex << std::endl;

		testBlock1.blockId = testBlockId;
		testBlock1.blockDataIndex = testBlockDataIndex;
		testBlock1.blockLight = testLight;

		ASSERT_EQ(testBlock1.blockId, testBlockId);
		ASSERT_EQ(testBlock1.blockDataIndex, testBlockDataIndex);
		ASSERT_EQ(GET_LIGHT_COLOR_R(testBlock1.blockLight), testLightR);
		ASSERT_EQ(GET_LIGHT_COLOR_G(testBlock1.blockLight), testLightG);
		ASSERT_EQ(GET_LIGHT_COLOR_B(testBlock1.blockLight), testLightB);
	}
	ASSERT_EQ(sizeof(ChunkBlock), 8);
}

TEST(ClientTest, FindChunkSpeedTest)
{
	auto& client = GameClient::getInstance();

	for (auto loopTime = 0; loopTime != 1000; ++loopTime)
	{
		for (auto i = 0; i < 30; i++)
			for (auto j = 0; j < 30; j++)
				for (auto k = 0; k < 16; k++)
					ASSERT_NE(client.getChunk(i, j, k), nullptr);
	}
}
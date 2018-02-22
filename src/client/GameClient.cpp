#include "GameClient.h"
#include "BlockList.h"

void GameClient::start(const ClientType clientType, const char* serverPotr)
{
	if (clientType == LocalConnection && serverPotr != nullptr)
		throw std::invalid_argument("serverPotr should be nullptr while clientType is LocalConnection");

	mClientChunkGroup.init();
}

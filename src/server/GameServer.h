#pragma once

#include "Net.h"

class GameServer
{
	NetServer mServer;
public:
	explicit GameServer(const char* address, const unsigned short port) :
		mServer(address, port)
	{}
};
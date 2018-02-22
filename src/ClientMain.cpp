#include <SDL2/SDL.h>
#include <vector>
#include "Application.h"
#include <cassert>

int main(const int argc, char* argv[])
{
	std::vector<std::string> args(argc);

	for (auto i = 0; i < argc; i++)
		args[i] = argv[i];

	try
	{
		return Application(args).run();
	}
	catch (std::exception& e)
	{
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Program throw an error: %s", e.what());
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "ERROR", e.what(), nullptr);

		assert(1);
		return -1;
	}
}
